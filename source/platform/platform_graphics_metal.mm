// ============================================================================
//  Kimberlite
//
//  Copyright 2020 Toni Pesola. All Rights Reserved.
// ============================================================================

#include <kb/platform.h>

#include <kb/log.h>
#include <kb/rwops.h>
#include <kb/vertex.h>
#include <kb/math.h>
#include <kb/crt.h>

#import <Cocoa/Cocoa.h>
#import <Foundation/Foundation.h>
#import <QuartzCore/QuartzCore.h>
#import <QuartzCore/CAMetalLayer.h>
#import <Metal/Metal.h>

KB_INTERNAL MTLCullMode cv_cull_mode_mtl(kb_cull_mode mode) {
  switch (mode) {
    case KB_CULL_NONE:  return MTLCullModeNone;
    case KB_CULL_BACK:  return MTLCullModeBack;
    case KB_CULL_FRONT: return MTLCullModeFront;
    default:            return MTLCullModeNone;
  }
}

KB_INTERNAL MTLWinding cv_winding_mtl(kb_winding winding) {
  switch (winding) {
    case KB_WINDING_CW:   return MTLWindingClockwise;
    case KB_WINDING_CCW:  return MTLWindingCounterClockwise;
    default:              return MTLWindingClockwise;
  }
}

KB_INTERNAL MTLVertexStepFunction cv_step_function_mtl(kb_step_func func) {
  switch (func) {
    case KB_STEP_FUNC_VERTEX:   return MTLVertexStepFunctionPerVertex;
    case KB_STEP_FUNC_INSTANCE: return MTLVertexStepFunctionPerInstance;
    default:                    return MTLVertexStepFunctionPerVertex;
  }
}

KB_INTERNAL MTLPrimitiveType cv_topology_mtl(kb_topology_type topology) {
  switch (topology) {
    case KB_TOPOLOGY_TRIANGLE_STRIP:  return MTLPrimitiveTypeTriangleStrip;
    case KB_TOPOLOGY_TRIANGLE_LIST:   return MTLPrimitiveTypeTriangle;
    case KB_TOPOLOGY_LINE_STRIP:      return MTLPrimitiveTypeLineStrip;
    case KB_TOPOLOGY_LINE_LIST:       return MTLPrimitiveTypeLine;
    case KB_TOPOLOGY_POINT_LIST:      return MTLPrimitiveTypePoint;
    default:                          return MTLPrimitiveTypeTriangle;
  }
}

KB_INTERNAL MTLVertexFormat cv_vertex_attrib_format_mtl(kb_vertex_attrib_format format) {
  switch (format) {
    case KB_VERTEX_FORMAT_FLOAT:  return MTLVertexFormatFloat;
    case KB_VERTEX_FORMAT_FLOAT2: return MTLVertexFormatFloat2;
    case KB_VERTEX_FORMAT_FLOAT3: return MTLVertexFormatFloat3;
    case KB_VERTEX_FORMAT_FLOAT4: return MTLVertexFormatFloat4;
    default:                      return MTLVertexFormatInvalid;
  }
}

KB_INTERNAL uint32_t cv_vertex_attrib_format_size(kb_vertex_attrib_format format) {
  switch (format) {
    case KB_VERTEX_FORMAT_FLOAT:  return 4;
    case KB_VERTEX_FORMAT_FLOAT2: return 8;
    case KB_VERTEX_FORMAT_FLOAT3: return 12;
    case KB_VERTEX_FORMAT_FLOAT4: return 16;
    default:                      return 0;
  }
}

KB_INTERNAL MTLPixelFormat cv_format_mtl(kb_format format) {
  switch (format) {
    case KB_FORMAT_R8:              return MTLPixelFormatR8Unorm;
    case KB_FORMAT_R8G8:            return MTLPixelFormatRG8Unorm;
    case KB_FORMAT_R8G8B8A8:        return MTLPixelFormatRGBA8Unorm;
    case KB_FORMAT_DEPTH:           return MTLPixelFormatDepth32Float;
    case KB_FORMAT_DEPTH_STENCIL:   return MTLPixelFormatDepth32Float_Stencil8;
    default:                        return MTLPixelFormatInvalid;
  }
}

KB_INTERNAL MTLIndexType cv_index_type_mtl(kb_index_type index_type) {
  switch (index_type) {
    case KB_INDEX_TYPE_16:  return MTLIndexTypeUInt16;
    case KB_INDEX_TYPE_32:  return MTLIndexTypeUInt32;
    default:                return MTLIndexTypeUInt32;
  }
}

KB_INTERNAL uint32_t cv_index_size[] {
  2,
  4,
};

KB_INTERNAL MTLVertexFormat cv_vertex_attribute[][5] {
  { MTLVertexFormatInvalid, MTLVertexFormatFloat,   MTLVertexFormatFloat2,    MTLVertexFormatFloat3,  MTLVertexFormatFloat4 },
  { MTLVertexFormatInvalid, MTLVertexFormatInt,     MTLVertexFormatInt2,      MTLVertexFormatInt3,    MTLVertexFormatInt4   },
  { MTLVertexFormatInvalid, MTLVertexFormatUInt,    MTLVertexFormatUInt2,     MTLVertexFormatUInt3,   MTLVertexFormatUInt4  },
};

struct buffer_ref {
  id<MTLBuffer> buffer;
};

struct pipeline_ref {
  id<MTLRenderPipelineState>  pipeline_state;
  id<MTLDepthStencilState>    depth_stencil_state;
  MTLPrimitiveType            primitive_type;
  MTLWinding                  winding;
  MTLCullMode                 cull_mode;
};

struct texture_ref {
  id<MTLTexture>      texture;
  id<MTLSamplerState> sampler;
};

struct transient_buffer {
  id<MTLBuffer> buffer;
  uint64_t      memory_position;
};

struct renderpass_ref {
  MTLRenderPassDescriptor* desc;
};

KB_RESOURCE_STORAGE_DEF (buffer,        kb_buffer,        buffer_ref,         KB_CONFIG_MAX_BUFFERS);
KB_RESOURCE_STORAGE_DEF (pipeline,      kb_pipeline,      pipeline_ref,       KB_CONFIG_MAX_PROGRAMS);
KB_RESOURCE_STORAGE_DEF (texture,       kb_texture,       texture_ref,        KB_CONFIG_MAX_TEXTURES);
KB_RESOURCE_STORAGE_DEF (renderpass,    kb_renderpass,    renderpass_ref,     KB_CONFIG_MAX_RENDERPASSES);

CAMetalLayer*             metal_layer;
NSView*                   content_view;
id<MTLDevice>             device;
id<MTLLibrary>            library;
id<MTLCommandQueue>       command_queue;
id<CAMetalDrawable>       current_drawable;
id<MTLTexture>            depth_texture;
id<MTLCommandBuffer>      current_command_buffer;
CGFloat                   contents_scale;

transient_buffer transient_buffers[KB_CONFIG_MAX_FRAMES_IN_FLIGHT];

dispatch_semaphore_t in_flight_semaphore;

uint32_t current_resource_slot = 0;

Int2 current_extent;

KB_INTERNAL transient_buffer& get_current_transient_buffer() {
  return transient_buffers[current_resource_slot];
}

KB_INTERNAL bool acquire_frame_resources() {
  dispatch_semaphore_wait(in_flight_semaphore, DISPATCH_TIME_FOREVER);
  
  current_resource_slot = (current_resource_slot + 1) % KB_CONFIG_MAX_FRAMES_IN_FLIGHT;

  // Reset transient buffer
  get_current_transient_buffer().memory_position = 0;

  return true;
}

KB_INTERNAL kb_shader_binding_type get_binding_type(MTLArgumentType type) {
  switch (type) {
    case MTLArgumentTypeBuffer:   return KB_SHADER_BINDING_TYPE_UNIFORM_BUFFER;
    case MTLArgumentTypeTexture:  return KB_SHADER_BINDING_TYPE_TEXTURE;
    default:                      return KB_SHADER_BINDING_TYPE_UNSUPPORTED;
  }
}

KB_INTERNAL id<MTLBuffer> create_buffer_from_rwops(kb_rwops* rwops, uint64_t size) {
  uint64_t rwops_size = kb_rwops_size(rwops);
  uint64_t data_size = min(rwops_size, size);
  void* data = KB_DEFAULT_ALLOC(data_size);
  kb_rwops_read(rwops, data, data_size);
  
  // TODO: Make device private
  id<MTLBuffer> buffer = [device 
    newBufferWithBytes  : data 
    length              : data_size
    options             : MTLResourceStorageModeManaged
    // options             : MTLResourceStorageModePrivate
  ];

  KB_DEFAULT_FREE(data);
  
  return buffer;
}

KB_API void kb_platform_pipeline_construct(kb_pipeline handle, const kb_pipeline_create_info info) {
  NSError* err = NULL;

  struct pipeline_ref& pipeline = pipeline_ref(handle);

  id<MTLLibrary>  vert_lib  = nil;
  id<MTLLibrary>  frag_lib  = nil;
  id<MTLFunction> vert_func = nil;
  id<MTLFunction> frag_func = nil;

  int64_t vert_code_size = kb_rwops_size(info.vert_shader.rwops);
  char*  vert_code = NULL;
  if (vert_code_size > 0) {
    vert_code = (char*) KB_DEFAULT_ALLOC(vert_code_size + 1);
    kb_rwops_read(info.vert_shader.rwops, vert_code, vert_code_size);
    vert_code[vert_code_size] = '\0';
  
    vert_lib = [device
      newLibraryWithSource  : [NSString stringWithUTF8String: vert_code]
      options               : nil
      error                 : &err
    ];
  } else {
    kb_log_error("Failed to load vertex shader");
  }
  
  int64_t frag_code_size = kb_rwops_size(info.frag_shader.rwops);
  char*  frag_code = NULL;
  if (frag_code_size > 0) {
    frag_code = (char*) KB_DEFAULT_ALLOC(frag_code_size + 1);
    kb_rwops_read(info.frag_shader.rwops, frag_code, frag_code_size);
    frag_code[frag_code_size] = '\0';
    
    frag_lib = [device
      newLibraryWithSource  : [NSString stringWithUTF8String: frag_code]
      options               : nil
      error                 : &err
    ];
  } else {
    kb_log_error("Failed to load fragment shader");
  }

  if (vert_lib == nil || frag_lib == nil) {
    kb_log_error("Failed to create pipeline. (Compile)");
    kb_log_warn("{}", [err.localizedDescription UTF8String]);
    return;
  }

  MTLFunctionConstantValues* constant_values = [MTLFunctionConstantValues new];

  vert_func = [vert_lib 
    newFunctionWithName : [NSString stringWithUTF8String:info.vert_shader.entry]
    constantValues : constant_values
    error : &err
  ];
  
  frag_func = [frag_lib 
    newFunctionWithName : [NSString stringWithUTF8String:info.frag_shader.entry]
    constantValues : constant_values
    error : &err
  ];
  
  if (vert_func == nil || frag_func == nil) {
    kb_log_error("Failed to create pipeline. (Function init)");
    kb_log_warn("{}", [err.localizedDescription UTF8String]);
    return;
  }

  MTLVertexDescriptor* vertex_description = [[MTLVertexDescriptor alloc] init];

  // Vertex attributes
  uint32_t strides[KB_CONFIG_MAX_VERTEX_ATTRIB_BUFFERS] = { 0 };

  for (uint32_t i = 0; i < KB_CONFIG_MAX_VERTEX_ATTRIBS; i++) {
    const kb_vertex_attrib_info& attrib = info.vertex_layout.attribs[i];
    
    if (attrib.format == KB_VERTEX_FORMAT_UNKNOWN) break;
    
    vertex_description.attributes[i].bufferIndex  = attrib.buffer;
    vertex_description.attributes[i].offset       = attrib.offset;
    vertex_description.attributes[i].format       = cv_vertex_attrib_format_mtl(attrib.format);
    strides[attrib.buffer] += cv_vertex_attrib_format_size(attrib.format);
  }

  // Vertex buffers
  for (uint32_t i = 0; i < KB_CONFIG_MAX_VERTEX_ATTRIB_BUFFERS; i++) {
    const kb_vertex_buffer_info& buffer = info.vertex_layout.buffers[i];
    
    if (buffer.step_func == KB_STEP_FUNC_UNKNOWN) break;
    
    vertex_description.layouts[i].stride        = buffer.stride == 0 ? strides[i] : buffer.stride;
    vertex_description.layouts[i].stepRate      = buffer.step_rate;
    vertex_description.layouts[i].stepFunction  = cv_step_function_mtl(buffer.step_func);
  }

  MTLRenderPipelineDescriptor* pipeline_description = [[MTLRenderPipelineDescriptor alloc] init];

  pipeline_description.vertexDescriptor       = vertex_description;
  pipeline_description.vertexFunction         = vert_func;
  pipeline_description.fragmentFunction       = frag_func;
  pipeline_description.sampleCount            = 4;
  pipeline_description.alphaToCoverageEnabled = YES;
  pipeline_description.alphaToOneEnabled      = NO;
  pipeline_description.rasterizationEnabled   = YES;

  pipeline_description.colorAttachments[0].pixelFormat                  = MTLPixelFormatBGRA8Unorm;
  pipeline_description.colorAttachments[0].blendingEnabled              = YES;
  pipeline_description.colorAttachments[0].alphaBlendOperation          = MTLBlendOperationAdd;
  pipeline_description.colorAttachments[0].rgbBlendOperation            = MTLBlendOperationAdd;
  pipeline_description.colorAttachments[0].destinationAlphaBlendFactor  = MTLBlendFactorOneMinusSourceAlpha;
  pipeline_description.colorAttachments[0].destinationRGBBlendFactor    = MTLBlendFactorOneMinusSourceAlpha;
  pipeline_description.colorAttachments[0].sourceAlphaBlendFactor       = MTLBlendFactorSourceAlpha;
  pipeline_description.colorAttachments[0].sourceRGBBlendFactor         = MTLBlendFactorSourceAlpha;

  pipeline.pipeline_state = [device newRenderPipelineStateWithDescriptor:pipeline_description error: &err];

  MTLDepthStencilDescriptor* depth_descriptor = [[MTLDepthStencilDescriptor alloc] init];
  depth_descriptor.depthWriteEnabled    = YES;
  depth_descriptor.depthCompareFunction = MTLCompareFunctionLess;
  
  depth_descriptor.backFaceStencil.depthFailureOperation = MTLStencilOperationKeep;
  depth_descriptor.frontFaceStencil.depthFailureOperation = MTLStencilOperationKeep;
    
  pipeline.depth_stencil_state = [device newDepthStencilStateWithDescriptor : depth_descriptor];
  
  [depth_descriptor release];

  if (err) {
    kb_log_warn("Pipeline creation error: {}", [err.localizedDescription UTF8String]);
  }

  if (err) {
    kb_log_warn("Pipeline creation error: {}", [err.localizedDescription UTF8String]);
  }

  pipeline.primitive_type = cv_topology_mtl(info.topology);
  pipeline.winding        = cv_winding_mtl(info.rasterizer.winding);
  pipeline.cull_mode      = cv_cull_mode_mtl(info.rasterizer.cull_mode);

  [vertex_description release];
  [pipeline_description release];
}

KB_API void kb_platform_pipeline_destruct(kb_pipeline handle) {
  // TODO:
}

KB_INTERNAL MTLSamplerMinMagFilter cv_filter[] {
  MTLSamplerMinMagFilterNearest,
  MTLSamplerMinMagFilterLinear,
};

KB_API void kb_platform_texture_construct(kb_texture handle, const kb_texture_create_info info) {
  int mip_levels = info.mipmaps ? floor(kb_log2(max(info.texture.width, info.texture.height))) + 1 : 1;

  MTLSamplerDescriptor* sampler_descriptor = [[MTLSamplerDescriptor alloc] init];
  sampler_descriptor.rAddressMode     = MTLSamplerAddressModeRepeat;
  sampler_descriptor.sAddressMode     = MTLSamplerAddressModeRepeat;
  sampler_descriptor.tAddressMode     = MTLSamplerAddressModeRepeat;
  sampler_descriptor.borderColor      = MTLSamplerBorderColorOpaqueBlack;
  sampler_descriptor.minFilter        = cv_filter[info.filter];
  sampler_descriptor.magFilter        = cv_filter[info.filter];
  sampler_descriptor.mipFilter        = MTLSamplerMipFilterLinear;
  sampler_descriptor.maxAnisotropy    = 16.0f;
  sampler_descriptor.lodMaxClamp      = (float) mip_levels;
  sampler_descriptor.lodMinClamp      = 0.0f;
  sampler_descriptor.compareFunction  = MTLCompareFunctionAlways;

  texture_ref(handle).sampler = [device newSamplerStateWithDescriptor:sampler_descriptor];

  MTLTextureDescriptor* texture_descriptor = [[MTLTextureDescriptor alloc] init];
  texture_descriptor.pixelFormat      = cv_format_mtl(info.texture.format);
  texture_descriptor.width            = info.texture.width;
  texture_descriptor.height           = info.texture.height;
  texture_descriptor.mipmapLevelCount = mip_levels;

  if (info.texture.render_target) {
    texture_descriptor.storageMode  = MTLStorageModePrivate;
    texture_descriptor.usage        = MTLTextureUsageRenderTarget;
  } else {

  }

  texture_ref(handle).texture = [device newTextureWithDescriptor:texture_descriptor];
  
  if (!info.texture.render_target && info.rwops != NULL) {
    uint32_t src_size = kb_rwops_size(info.rwops);
    void* src_data = (char*) KB_DEFAULT_ALLOC(src_size);
    kb_rwops_read(info.rwops, src_data, src_size);

    MTLRegion region = MTLRegionMake2D(0, 0, info.texture.width, info.texture.height);

    [texture_ref(handle).texture replaceRegion : region 
      mipmapLevel : 0 
      withBytes   : src_data 
      bytesPerRow : 4 * info.texture.width
    ];

    if (mip_levels > 1) {
      id<MTLCommandBuffer> command_buffer = [command_queue commandBuffer];

      id<MTLBlitCommandEncoder> encoder = [command_buffer blitCommandEncoder];

      [encoder generateMipmapsForTexture: texture_ref(handle).texture];
      [encoder endEncoding];

      [command_buffer commit];

      [command_buffer waitUntilCompleted];
    }

    KB_DEFAULT_FREE(src_data);
  }

  [sampler_descriptor release];
  [texture_descriptor release];
}

KB_API uint32_t kb_graphics_get_current_resource_slot() { 
  return 0;
}

KB_API void kb_platform_texture_destruct(kb_texture handle) {

}

KB_API void kb_platform_renderpass_construct(kb_renderpass handle, const kb_renderpass_create_info info) {
  renderpass_ref(handle).desc = [MTLRenderPassDescriptor renderPassDescriptor];
  
  for (uint32_t i = 0; i < KB_CONFIG_MAX_RENDERPASS_ATTACHMENTS; ++i) {
    if (!kb_is_valid(info.color_attachments[i])) continue;
    MTLRenderPassColorAttachmentDescriptor *color_attachment = renderpass_ref(handle).desc.colorAttachments[i];
    color_attachment.texture      = texture_ref(info.color_attachments[i]).texture;
    color_attachment.clearColor   = MTLClearColorMake(0.0f, 0.0f, 0.0f, 1.0f);
    color_attachment.loadAction   = MTLLoadActionClear;
    color_attachment.storeAction  = MTLStoreActionStore;
  }

  if (kb_is_valid(info.depth_attachment)) {
    MTLRenderPassDepthAttachmentDescriptor* depth_attachment = renderpass_ref(handle).desc.depthAttachment;
    depth_attachment.texture      = texture_ref(info.depth_attachment).texture;
    depth_attachment.clearDepth   = 1.0;
    depth_attachment.loadAction   = MTLLoadActionClear;
    depth_attachment.storeAction  = MTLStoreActionDontCare;
  }

  if (kb_is_valid(info.stencil_attachment)) {  
    MTLRenderPassStencilAttachmentDescriptor* stencil_attachment = renderpass_ref(handle).desc.stencilAttachment;
    stencil_attachment.texture     = texture_ref(info.stencil_attachment).texture;
    stencil_attachment.loadAction  = MTLLoadActionDontCare;
    stencil_attachment.storeAction = MTLStoreActionDontCare;
  }
}

KB_API void kb_platform_renderpass_destruct(kb_renderpass handle) {
  // TODO:
}

KB_API void kb_platform_buffer_construct(kb_buffer handle, const kb_buffer_create_info info) {
  KB_ASSERT_VALID(handle);
  KB_ASSERT(info.size > 0, "Buffer size must be greater than zero");
  
  buffer_ref(handle).buffer = create_buffer_from_rwops(info.rwops, info.size);
}

KB_API void kb_platform_buffer_destruct(kb_buffer handle) { 
  [buffer_ref(handle).buffer release];
}

KB_API void* kb_platform_graphics_transient_alloc(uint64_t size, uint64_t align) {
  transient_buffer& buffer = get_current_transient_buffer();
  uint64_t p = align_up(buffer.memory_position, align);
  buffer.memory_position = p + size;
  return ((uint8_t*) buffer.buffer.contents) + p;
}

KB_API void* kb_platform_graphics_transient_at(uint64_t offset) {
  transient_buffer& buffer = get_current_transient_buffer();
  if (offset > buffer.memory_position) return NULL;
  return ((uint8_t*) buffer.buffer.contents) + offset;
}

KB_API uint64_t kb_platform_graphics_transient_offset(void* ptr) {
  uint8_t* zero = (uint8_t*) get_current_transient_buffer().buffer.contents;
  return ((uint8_t*) ptr) - zero;
}

KB_INTERNAL void create_depth_resources(Int2 size) {
  MTLTextureDescriptor* depth_texture_descriptor = [[MTLTextureDescriptor alloc] init];
  depth_texture_descriptor.width            = size.width;
  depth_texture_descriptor.height           = size.height;
  depth_texture_descriptor.pixelFormat      = MTLPixelFormatDepth32Float_Stencil8;
  depth_texture_descriptor.storageMode      = MTLStorageModePrivate;
  depth_texture_descriptor.usage            = MTLTextureUsageRenderTarget;

  depth_texture = [device newTextureWithDescriptor : depth_texture_descriptor];

  [depth_texture_descriptor release];
}

KB_API void kb_platform_graphics_init(const kb_graphics_init_info info) {
  kb_platform_surface_create(info.resolution.x, info.resolution.y);
  kb_platform_surface_set_system_cursor_visible(!info.hide_cursor);

  device = MTLCreateSystemDefaultDevice();
  library = [device newDefaultLibrary];
  command_queue = [device newCommandQueue];

  NSWindow* window = (NSWindow*) kb_platform_surface_get_native_window_handle();
  
  content_view  = (NSView*) window.contentView;

  content_view.wantsLayer = YES;
    
  contents_scale = content_view.layer.contentsScale;
  NSSize layer_size = content_view.layer.frame.size;

  metal_layer = CAMetalLayer.layer;
  metal_layer.device          = device;
  metal_layer.pixelFormat     = MTLPixelFormatBGRA8Unorm;
  metal_layer.frame           = content_view.layer.frame;
  metal_layer.framebufferOnly = true;
  metal_layer.displaySyncEnabled = info.vsync;

  metal_layer.frame = content_view.bounds;
  metal_layer.contentsScale = contents_scale;
  metal_layer.drawableSize = NSMakeSize(layer_size.width * contents_scale, layer_size.height * contents_scale);

  content_view.layer = metal_layer;

  current_extent = {(int32_t)(layer_size.width * contents_scale), (int32_t)(layer_size.height * contents_scale)};

  create_depth_resources(current_extent);

  for (uint32_t i = 0; i < KB_CONFIG_MAX_FRAMES_IN_FLIGHT; i++) {
    transient_buffers[i].memory_position = 0;
    transient_buffers[i].buffer = [device 
      newBufferWithLength : KB_CONFIG_TRANSIENT_BUFFER_SIZE
      options             : MTLResourceStorageModeShared
    ];
  }
  
  in_flight_semaphore = dispatch_semaphore_create(KB_CONFIG_MAX_FRAMES_IN_FLIGHT);

  acquire_frame_resources();
}

KB_API void kb_platform_graphics_wait_device_idle() {

}

KB_API void kb_platform_graphics_deinit() {  
  kb_platform_surface_destroy();
}

KB_API Int2 kb_graphics_get_extent() {
  return { (Int32) (metal_layer.drawableSize.width), (Int32) (metal_layer.drawableSize.height) };
}

KB_API void kb_platform_graphics_frame() {
  Int2 new_extent = {
    (int32_t) (content_view.layer.frame.size.width * contents_scale), 
    (int32_t) (content_view.layer.frame.size.height * contents_scale)
  };

  if (new_extent.x != current_extent.x || new_extent.y != current_extent.y) {
    current_extent = new_extent;
    create_depth_resources(current_extent);
    metal_layer.drawableSize = {content_view.layer.frame.size.width * contents_scale, content_view.layer.frame.size.height * contents_scale};
  }

  @autoreleasepool {
    current_drawable = metal_layer.nextDrawable;

    current_command_buffer = [command_queue commandBuffer];  
    
    { // Run encoders
      kb_graphics_run_encoders();
    }

    { // Preset and commit command buffer
      [current_command_buffer presentDrawable:current_drawable];

      __block dispatch_semaphore_t block_semaphore = in_flight_semaphore;
      [current_command_buffer addCompletedHandler:^(id<MTLCommandBuffer> buffer)
      {
        dispatch_semaphore_signal(block_semaphore);
      }];

      [current_command_buffer commit];
    }
    
    acquire_frame_resources();
  }
}

KB_API uint64_t kb_platform_graphics_transient_used() {
  return get_current_transient_buffer().memory_position;
}

KB_API uint64_t kb_platform_graphics_transient_capacity() {
  return KB_CONFIG_TRANSIENT_BUFFER_SIZE;
}

KB_API void* kb_platform_graphics_buffer_mapped(kb_buffer buffer) {
  return buffer_ref(buffer).buffer.contents;
}

KB_API void kb_platform_graphics_submit_calls(kb_renderpass pass, kb_graphics_call* calls, uint32_t call_count) {
  MTLRenderPassDescriptor* render_pass_desc;

  if (!kb_is_valid(pass)) { // Default
    render_pass_desc = [MTLRenderPassDescriptor renderPassDescriptor];

    MTLRenderPassColorAttachmentDescriptor *color_attachment = render_pass_desc.colorAttachments[0];
    color_attachment.texture      = current_drawable.texture;
    color_attachment.clearColor   = MTLClearColorMake(0.0f, 0.0f, 0.0f, 0.0f);
    color_attachment.loadAction   = MTLLoadActionClear;
    color_attachment.storeAction  = MTLStoreActionStore;

    MTLRenderPassDepthAttachmentDescriptor* depth_attachment = render_pass_desc.depthAttachment;
    depth_attachment.texture      = depth_texture;
    depth_attachment.clearDepth   = 1.0;
    depth_attachment.loadAction   = MTLLoadActionClear;
    depth_attachment.storeAction  = MTLStoreActionDontCare;

    MTLRenderPassStencilAttachmentDescriptor* stencil_attachment = render_pass_desc.stencilAttachment;
    stencil_attachment.texture     = depth_texture;
    stencil_attachment.loadAction  = MTLLoadActionDontCare;
    stencil_attachment.storeAction = MTLStoreActionDontCare;
  } else {
    render_pass_desc = renderpass_ref(pass).desc;
  }

  id<MTLRenderCommandEncoder> render_encoder = [current_command_buffer renderCommandEncoderWithDescriptor : render_pass_desc];
  
  for (uint32_t call_i = 0; call_i < call_count; ++call_i) {
    const kb_graphics_call& call = calls[call_i];
    struct pipeline_ref& pipeline = pipeline_ref(call.pipeline);

    [render_encoder setRenderPipelineState  : pipeline.pipeline_state];
    [render_encoder setCullMode             : pipeline.cull_mode];    
    [render_encoder setFrontFacingWinding   : pipeline.winding];
    [render_encoder setDepthStencilState    : pipeline.depth_stencil_state];
    
    // Bindings
    for (uint32_t bind_i = 0; bind_i < KB_CONFIG_MAX_UNIFORM_BINDINGS; ++bind_i) {
      { // Vertex uniform
        const kb_uniform_binding& uniform_bind = call.vert_uniform_bindings[bind_i];
        if (uniform_bind.size > 0) {
          [render_encoder setVertexBuffer : get_current_transient_buffer().buffer
            offset  : uniform_bind.offset
            atIndex : uniform_bind.index
          ];
        }  
      }
      
      { // Fragment uniform
        const kb_uniform_binding& uniform_bind = call.frag_uniform_bindings[bind_i];
        if (uniform_bind.size > 0) {
          [render_encoder setFragmentBuffer : get_current_transient_buffer().buffer
            offset  : uniform_bind.offset
            atIndex : uniform_bind.index
          ];
        }  
      }

      { // Vertex texture
        const kb_texture_binding& texture_bind = call.vert_texture_bindings[bind_i];
        if (kb_is_valid(texture_bind.texture)) {
          [render_encoder setVertexTexture : texture_ref(texture_bind.texture).texture 
            atIndex : texture_bind.index
          ];
          [render_encoder setVertexSamplerState : texture_ref(texture_bind.texture).sampler
            atIndex : texture_bind.index
          ];
        }  
      }

      { // Fragment texture
        const kb_texture_binding& texture_bind = call.frag_texture_bindings[bind_i];
        if (kb_is_valid(texture_bind.texture)) {
          [render_encoder setFragmentTexture : texture_ref(texture_bind.texture).texture 
            atIndex : texture_bind.index
          ];
          [render_encoder setFragmentSamplerState : texture_ref(texture_bind.texture).sampler
            atIndex : texture_bind.index
          ];
        }
      }
    }
    
    { // Vertex buffers
      for (uint32_t bind_i = 0; bind_i < KB_CONFIG_MAX_VERTEX_BUFFERS_BINDINGS; ++bind_i) {
        const kb_vertex_buffer_binding& binding = call.vertex_buffer_bindings[bind_i];
        if (binding.is_set) {
          id<MTLBuffer> vb = kb_is_valid(binding.buffer) ?
            buffer_ref(binding.buffer).buffer : 
            get_current_transient_buffer().buffer;

          uint64_t vb_offset = binding.offset;

          [render_encoder setVertexBuffer : vb 
            offset  : vb_offset 
            atIndex : bind_i
          ];
        }
      }
    }

    { // Call
      id<MTLBuffer> ib = kb_is_valid(call.index_buffer.buffer) ?
        buffer_ref(call.index_buffer.buffer).buffer : 
        get_current_transient_buffer().buffer;

      uint64_t ib_offset    = call.index_buffer.offset;
      uint64_t index_size   = cv_index_size[call.index_buffer.index_type];
      uint64_t full_offset  = ib_offset + (call.info.first_index * index_size);

      [render_encoder drawIndexedPrimitives : pipeline.primitive_type
        indexCount        : call.info.index_count
        indexType         : cv_index_type_mtl(call.index_buffer.index_type)
        indexBuffer       : ib
        indexBufferOffset : full_offset
        instanceCount     : call.info.instance_count
        baseVertex        : call.info.first_vertex
        baseInstance      : 0 // TODO:
      ];
    }

  }

  [render_encoder endEncoding];
}
