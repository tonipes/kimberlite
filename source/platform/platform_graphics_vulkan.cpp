// ============================================================================
//  Kimberlite
//
//  Copyright 2020 Toni Pesola. All Rights Reserved.
// ============================================================================

#include <kb/platform.h>

#include <kb/graphics.h>
#include <kb/core.h>
#include <kb/alloc.h>
#include <kb/alg.h>
#include <kb/geometry.h>
#include <kb/table.h>
#include <kb/vertex.h>
#include <kb/handle.h>
#include <kb/resource.h>
#include <kb/log.h>
#include <kb/math.h>
#include <kb/table.h>
#include <kb/time.h>

#define KB_VULKAN_VERSION                       VK_API_VERSION_1_0
#define VMA_VULKAN_VERSION                      1000000

#define VMA_DEDICATED_ALLOCATION                0
#define VMA_HEAVY_ASSERT(expr)                  assert(expr)
#define VMA_DEBUG_MARGIN                        16
#define VMA_DEBUG_DETECT_CORRUPTION             1
#define VMA_DEBUG_INITIALIZE_ALLOCATIONS        1
#define VMA_DEBUG_MIN_BUFFER_IMAGE_GRANULARITY  256
#define VMA_DEBUG_GLOBAL_MUTEX                  1
#define VMA_MEMORY_BUDGET                       0
#define VMA_STATIC_VULKAN_FUNCTIONS             0
#define VMA_DYNAMIC_VULKAN_FUNCTIONS            1

#define MAX_DESCRIPTOR_UPDATES                  512

#include <vulkan/vulkan.h>

#define VMA_IMPLEMENTATION
#include <vk_mem_alloc.h>

#include "platform_graphics_vulkan.h"

#include "spirv_util.h"

VkCullModeFlags cv_cull_mode[] {
  VK_CULL_MODE_NONE,
  VK_CULL_MODE_BACK_BIT,
  VK_CULL_MODE_FRONT_BIT,
  VK_CULL_MODE_FRONT_AND_BACK,
};

VkPrimitiveTopology cv_topology[] {
  VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP,
  VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
  VK_PRIMITIVE_TOPOLOGY_LINE_STRIP,
  VK_PRIMITIVE_TOPOLOGY_LINE_LIST,
  VK_PRIMITIVE_TOPOLOGY_POINT_LIST,
};

KB_INTERNAL VkIndexType cv_index_type[] {
  VK_INDEX_TYPE_UINT16,
  VK_INDEX_TYPE_UINT32,
};

// TODO: Not accurate, since triangle list etc. are not properly calculated
KB_INTERNAL uint32_t cv_primitive_size[] = { 1, 3, 1, 2, 1 };

KB_INTERNAL uint32_t cv_format_size[] { 0, 1, 2, 3, 4 };

KB_INTERNAL VkFormat cv_vk_format[] {
  VK_FORMAT_UNDEFINED,
  VK_FORMAT_R8_SRGB,
  VK_FORMAT_R8G8_SRGB,
  VK_FORMAT_R8G8B8_SRGB,
  VK_FORMAT_R8G8B8A8_SRGB,
};

KB_INTERNAL VkFormat cv_vertex_attribute[][5] {
  /* FLOAT32 */ { VK_FORMAT_UNDEFINED, VK_FORMAT_R32_SFLOAT, VK_FORMAT_R32G32_SFLOAT,  VK_FORMAT_R32G32B32_SFLOAT, VK_FORMAT_R32G32B32A32_SFLOAT },
  /* SINT    */ { VK_FORMAT_UNDEFINED, VK_FORMAT_R32_SINT,   VK_FORMAT_R32G32_SINT,    VK_FORMAT_R32G32B32_SINT,   VK_FORMAT_R32G32B32A32_SINT   },
  /* UINT    */ { VK_FORMAT_UNDEFINED, VK_FORMAT_R32_UINT,   VK_FORMAT_R32G32_UINT,    VK_FORMAT_R32G32B32_UINT,   VK_FORMAT_R32G32B32A32_UINT   },
};


KB_INTERNAL VkFilter cv_filter[] {
  VK_FILTER_NEAREST,
  VK_FILTER_LINEAR,
};

//#####################################################################################################################
// Structs
//#####################################################################################################################

struct Buffer {
  VkBuffer                          buffer;
  VmaAllocation                     alloc = VK_NULL_HANDLE;
  VmaAllocationInfo                 alloc_info {};
};

struct Image {
  VkImage                           image;
  VmaAllocation                     alloc = VK_NULL_HANDLE;
  VmaAllocationInfo                 alloc_info {};
};

struct QueueFamilyIndices {
  uint32_t                          graphics = UINT32_MAX;
  uint32_t                          compute  = UINT32_MAX;
  uint32_t                          transfer = UINT32_MAX;
  uint32_t                          present  = UINT32_MAX;
};

struct SwapChainSupportDetails {
  VkSurfaceCapabilitiesKHR          capabilities;
  VkSurfaceFormatKHR*               formats;
  uint32_t                          format_count;
  VkPresentModeKHR*                 present_modes;
  uint32_t                          present_mode_count;
};

struct kb_transient_buffer {
  Buffer                            buffer;
  uint64_t                          memory_position;
};

//#####################################################################################################################
// Resource structs
//#####################################################################################################################

struct kb_texture_ref {
  Image                             image;
  VkImageView                       view;
  VkSampler                         sampler;
  uint32_t                          mip_levels;
};

struct kb_buffer_ref {
  Buffer                            vk_buffer;
};

struct kb_vertex_buffer_ref {
  kb_vertex_layout                  layout;
  Buffer                            vk_buffer;
};

struct kb_index_buffer_ref {
  Buffer                            vk_buffer;
  VkIndexType                       index_type;
  uint32_t                          count;
};

struct kb_pipeline_ref {
  VkPipelineLayout                  pipeline_layout;
  VkPipeline                        pipeline;
  VkVertexInputAttributeDescription vertex_input_descriptions       [KB_CONFIG_MAX_VERTEX_ATTRIBS];
  VkDescriptorSetLayout             descriptor_set_layouts          [KB_CONFIG_MAX_UNIFORM_BINDINGS];
  VkDescriptorSetLayoutBinding      descriptor_set_layout_bindings  [KB_CONFIG_MAX_UNIFORM_BINDINGS][KB_CONFIG_MAX_UNIFORM_BINDINGS];
  uint32_t                          set_layout_count;
  uint32_t                          primitive_size;
};

KB_RESOURCE_STORAGE_DEF (pipeline,  kb_pipeline,  kb_pipeline_ref,  KB_CONFIG_MAX_PROGRAMS);
KB_RESOURCE_STORAGE_DEF (texture,   kb_texture,   kb_texture_ref,   KB_CONFIG_MAX_TEXTURES);
KB_RESOURCE_STORAGE_DEF (buffer,    kb_buffer,    kb_buffer_ref,    KB_CONFIG_MAX_BUFFERS);

struct CommandBufferPool {
  VkCommandPool       command_pool;
  uint16_t            count;
  VkCommandBuffer     command_buffers [KB_CONFIG_MAX_COMMAND_BUFFERS];
};

struct DescriptorPool {
  VkDescriptorPool    descriptor_pool;
  uint16_t            count;
  VkDescriptorSet     descriptors [KB_CONFIG_MAX_UNIFORM_BINDINGS * KB_CONFIG_MAX_DRAW_CALLS];
};

struct FrameResources {
  VkCommandBuffer     primary_command_buffers [KB_CONFIG_MAX_FRAMES_IN_FLIGHT];
  DescriptorPool      descriptor_pools        [KB_CONFIG_MAX_FRAMES_IN_FLIGHT];
  VkImage             swapchain_images        [KB_CONFIG_MAX_FRAMES_IN_FLIGHT];
  VkImageView         swapchain_imageviews    [KB_CONFIG_MAX_FRAMES_IN_FLIGHT];
  VkFramebuffer       swapchain_framebuffers  [KB_CONFIG_MAX_FRAMES_IN_FLIGHT];
  CommandBufferPool   command_buffer_pools    [KB_CONFIG_MAX_FRAMES_IN_FLIGHT];
};

const char* vk_validation_layers[] = {
#if KB_CONFIG_GRAPHICS_VALIDATE
  "VK_LAYER_KHRONOS_validation",
  "VK_LAYER_LUNARG_standard_validation",
#endif
};

const char* vk_instance_extensions[] = {
#if KB_CONFIG_GRAPHICS_VALIDATE
  VK_EXT_DEBUG_UTILS_EXTENSION_NAME
#endif
};

const char* vk_device_extensions[] = {
  VK_KHR_SWAPCHAIN_EXTENSION_NAME
};

static uint32_t                   vulkan_surface_extension_count = 0;
const char**                      vulkan_surface_extensions;

kb_transient_buffer               transient_buffers       [KB_CONFIG_MAX_FRAMES_IN_FLIGHT];

uint32_t                          current_frame_slot    = 0;
uint32_t                          current_resource_slot = 0;

bool                              vk_want_vsync = false;

VmaAllocator                      vk_allocator;

VkAllocationCallbacks*            vk_allocation_callbacks;
VkInstance                        vk_instance;
VkDebugUtilsMessengerEXT          vk_debug_messenger;
VkPhysicalDevice                  vk_physical_device;
VkPhysicalDeviceProperties        vk_device_properties;
VkPhysicalDeviceMemoryProperties  vk_memory_properties;
VkPhysicalDeviceFeatures          vk_device_features;
VkDevice                          vk_device;
VkSurfaceKHR                      vk_surface;
VkQueue                           vk_queue_graphics;
VkQueue                           vk_queue_compute;
VkQueue                           vk_queue_present;
VkQueue                           vk_queue_transfer;
VkPipelineCache                   vk_pipeline_cache;
VkSwapchainKHR                    vk_swapchain;
VkFormat                          vk_swapchain_image_format;
VkExtent2D                        vk_swapchain_extent;
VkImageView                       vk_color_image_view;
VkImageView                       vk_depth_image_view;
VkRenderPass                      vk_render_pass;
VkCommandPool                     vk_command_pool;
VkSampleCountFlagBits             vk_msaa_sample_count;

VkSemaphore                       image_available_semaphores [KB_CONFIG_MAX_FRAMES_IN_FLIGHT];
VkSemaphore                       render_finished_semaphores [KB_CONFIG_MAX_FRAMES_IN_FLIGHT];
VkFence                           images_in_flight           [KB_CONFIG_MAX_FRAMES_IN_FLIGHT]; 
VkFence                           fences_in_flight           [KB_CONFIG_MAX_FRAMES_IN_FLIGHT];

QueueFamilyIndices                vk_queue_family_indices;
SwapChainSupportDetails           vk_swapchain_support_details;
Image                             vk_depth_image;
Image                             vk_color_image;
FrameResources                    frame_resources;
uint32_t                          vk_swapchain_frame_slots;


//#####################################################################################################################
// Internal Functions
//#####################################################################################################################

// Debug messenger
KB_INTERNAL VKAPI_ATTR VkBool32 VKAPI_CALL vk_debug_report_callback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData) {
  kb_log_debug("{}", pCallbackData->pMessage);
  return true;
}

KB_INTERNAL VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger) {
  auto func = (PFN_vkCreateDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
  if (func != nullptr) {
    return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
  } else {
    return VK_ERROR_EXTENSION_NOT_PRESENT;
  }
}

KB_INTERNAL void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator) {
  auto func = (PFN_vkDestroyDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
  if (func != nullptr) {
    func(instance, debugMessenger, pAllocator);
  }
}

// Allocation callbacks
KB_INTERNAL void* vk_allocation_alloc(void* user_data, size_t size, size_t alignment, VkSystemAllocationScope allocation_scope) {
  void* ptr = KB_DEFAULT_ALLOC(size);
  kb_memset(ptr, 0, size);
  return ptr;  
}

KB_INTERNAL void vk_allocation_free(void* user_data, void* ptr) {
  KB_DEFAULT_FREE(ptr);
}

KB_INTERNAL void* vk_allocation_realloc(void* user_data, void* ptr, size_t size, size_t alignment, VkSystemAllocationScope allocation_scope) {       
  return KB_DEFAULT_REALLOC(ptr, size);
}

KB_INTERNAL void vk_allocation_internal_alloc(void* user_data, size_t size, VkInternalAllocationType allocation_type, VkSystemAllocationScope allocation_scope) {
  kb_log_debug("vk_allocation_internal_alloc {}", size);
}

KB_INTERNAL void vk_allocation_internal_free(void* user_data, size_t size,  VkInternalAllocationType  allocation_type, VkSystemAllocationScope allocation_scope) {
  kb_log_debug("vk_allocation_internal_free {}", size);
}

KB_INTERNAL void init_vk_allocation_callbacks() {
  vk_allocation_callbacks = KB_DEFAULT_ALLOC_TYPE(VkAllocationCallbacks, 1);
  
  kb_memset(vk_allocation_callbacks, 0, sizeof(VkAllocationCallbacks));

  vk_allocation_callbacks->pUserData              = nullptr;
  vk_allocation_callbacks->pfnAllocation          = (PFN_vkAllocationFunction)              &vk_allocation_alloc;
  vk_allocation_callbacks->pfnReallocation        = (PFN_vkReallocationFunction)            &vk_allocation_realloc;
  vk_allocation_callbacks->pfnFree                = (PFN_vkFreeFunction)                    &vk_allocation_free;
  vk_allocation_callbacks->pfnInternalAllocation  = (PFN_vkInternalAllocationNotification)  &vk_allocation_internal_alloc;
  vk_allocation_callbacks->pfnInternalFree        = (PFN_vkInternalFreeNotification)        &vk_allocation_internal_free;
}

KB_INTERNAL uint32_t get_min_uniform_buffer_offset_alignment() {
  return vk_device_properties.limits.minUniformBufferOffsetAlignment;
}

KB_INTERNAL VkSampleCountFlagBits get_max_usable_sample_count(uint32_t target) {
  VkSampleCountFlags counts = vk_device_properties.limits.framebufferColorSampleCounts & vk_device_properties.limits.framebufferDepthSampleCounts;

  target = target == 0 ? UINT32_MAX : target; // Zero means MAX
  
  VkSampleCountFlagBits selected = VK_SAMPLE_COUNT_1_BIT;

  if (target >= 64 && counts & VK_SAMPLE_COUNT_64_BIT)  { selected = VK_SAMPLE_COUNT_64_BIT;  }
  if (target >= 32 && counts & VK_SAMPLE_COUNT_32_BIT)  { selected = VK_SAMPLE_COUNT_32_BIT;  }
  if (target >= 16 && counts & VK_SAMPLE_COUNT_16_BIT)  { selected = VK_SAMPLE_COUNT_16_BIT;  }
  if (target >=  8 && counts & VK_SAMPLE_COUNT_8_BIT)   { selected = VK_SAMPLE_COUNT_8_BIT;   }
  if (target >=  4 && counts & VK_SAMPLE_COUNT_4_BIT)   { selected = VK_SAMPLE_COUNT_4_BIT;   }
  if (target >=  2 && counts & VK_SAMPLE_COUNT_2_BIT)   { selected = VK_SAMPLE_COUNT_2_BIT;   }

  return selected;
}

KB_INTERNAL uint32_t find_vk_memory_type(uint32_t type_filter, VkMemoryPropertyFlags properties) {
  for (uint32_t i = 0; i < vk_memory_properties.memoryTypeCount; i++) {
    if ((type_filter & (1 << i)) && (vk_memory_properties.memoryTypes[i].propertyFlags & properties) == properties) {
      return i;
    }
  }

  return -1;
}

KB_INTERNAL VkCommandBuffer command_buffer_begin(VkCommandBufferUsageFlagBits usage_flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT) {
  VkCommandBufferAllocateInfo command_buffer_create_info {};
  command_buffer_create_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
  command_buffer_create_info.pNext                = NULL;
  command_buffer_create_info.commandPool          = vk_command_pool;
  command_buffer_create_info.level                = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
  command_buffer_create_info.commandBufferCount   = 1;
  
  VkCommandBuffer command_buffer;
  VK_CHECK_RESULT(vkAllocateCommandBuffers(vk_device, &command_buffer_create_info, &command_buffer));
  
  VkCommandBufferBeginInfo begin_info {};
  begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
  begin_info.pNext            = NULL;
  begin_info.flags            = usage_flags;
  begin_info.pInheritanceInfo = NULL;
  VK_CHECK_RESULT(vkBeginCommandBuffer(command_buffer, &begin_info));
  
  return command_buffer;
}

KB_INTERNAL void command_buffer_submit_and_wait(VkCommandBuffer command_buffer) {
  vkEndCommandBuffer(command_buffer);

  VkSubmitInfo submit_info {};
  submit_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
  submit_info.pNext                 = NULL;
  submit_info.commandBufferCount    = 1;
  submit_info.pCommandBuffers       = &command_buffer;
  submit_info.waitSemaphoreCount    = 0;
  submit_info.pWaitSemaphores       = NULL;
  submit_info.signalSemaphoreCount  = 0;
  submit_info.pSignalSemaphores     = NULL;
  submit_info.pWaitDstStageMask     = NULL;

  VK_CHECK_RESULT(vkQueueSubmit(vk_queue_graphics, 1, &submit_info, VK_NULL_HANDLE));
  VK_CHECK_RESULT(vkQueueWaitIdle(vk_queue_graphics));

  vkFreeCommandBuffers(vk_device, vk_command_pool, 1, &command_buffer);
}

KB_INTERNAL void destroy_buffer(Buffer* buffer) {
  vmaDestroyBuffer(vk_allocator, buffer->buffer, buffer->alloc);
}

KB_INTERNAL Buffer create_buffer(uint32_t size, VkBufferUsageFlags usage, VmaMemoryUsage memory_usage = VMA_MEMORY_USAGE_GPU_ONLY, VmaAllocationCreateFlags alloc_flags = 0, VkMemoryPropertyFlags memory_property_flags = 0) {
  Buffer buffer;

  VkBufferCreateInfo buffer_create_info {};
  buffer_create_info.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
  buffer_create_info.size                   = size;
  buffer_create_info.sharingMode            = VK_SHARING_MODE_EXCLUSIVE;
  buffer_create_info.queueFamilyIndexCount  = 0;
  buffer_create_info.pQueueFamilyIndices    = NULL;
  buffer_create_info.usage                  = usage;

  VmaAllocationCreateInfo alloc_create_info {};
  alloc_create_info.usage           = memory_usage;
  alloc_create_info.flags           = alloc_flags;
  alloc_create_info.requiredFlags   = memory_property_flags;

  VK_CHECK_RESULT(vmaCreateBuffer(vk_allocator, &buffer_create_info, &alloc_create_info, &buffer.buffer, &buffer.alloc, &buffer.alloc_info));

  return buffer;
}

KB_INTERNAL void copy_to_buffer(void* data, uint32_t size, Buffer& buffer) {
  Buffer staging_buffer = create_buffer(size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VMA_MEMORY_USAGE_CPU_ONLY, VMA_ALLOCATION_CREATE_MAPPED_BIT);

  kb_memcpy(staging_buffer.alloc_info.pMappedData, data, size);

  VkCommandBuffer command_buffer = command_buffer_begin();
  
  {
    VkBufferCopy region;
    region.srcOffset = 0;
    region.dstOffset = 0;
    region.size      = size;

    vkCmdCopyBuffer(command_buffer, staging_buffer.buffer, buffer.buffer, 1, &region);
  }
  
  command_buffer_submit_and_wait(command_buffer);
  
  destroy_buffer(&staging_buffer);
}

KB_INTERNAL void init_vk_instance() {
  uint32_t sdl_extension_count = vulkan_surface_extension_count;
  const char** sdl_extensions = vulkan_surface_extensions;

  uint32_t extension_count = sdl_extension_count + KB_COUNTOF(vk_instance_extensions);
  const char** extensions = (const char**) KB_DEFAULT_ALLOC(sizeof(const char*) * extension_count);

  for (uint32_t i = 0; i < KB_COUNTOF(vk_instance_extensions); i++) { 
    extensions[i] = vk_instance_extensions[i]; 
  } 

  for (uint32_t i = 0; i < sdl_extension_count; i++) { 
    extensions[KB_COUNTOF(vk_instance_extensions) + i] = sdl_extensions[i];
  }

  VkApplicationInfo app_info {};
  app_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
  app_info.pApplicationName   = "KBVulkan";
  app_info.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
  app_info.pEngineName        = "Kimberlite";
  app_info.engineVersion      = VK_MAKE_VERSION(1, 0, 0);
  app_info.apiVersion         = KB_VULKAN_VERSION;

  VkDebugUtilsMessengerCreateInfoEXT debug_create_info {};
  debug_create_info.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
  debug_create_info.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
  debug_create_info.messageType     = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
  debug_create_info.pfnUserCallback = vk_debug_report_callback;
  debug_create_info.pUserData       = nullptr; // Optional
  
  VkInstanceCreateInfo create_info {};
  create_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
  create_info.pApplicationInfo        = &app_info;
  create_info.enabledExtensionCount   = extension_count;
  create_info.ppEnabledExtensionNames = extensions;
  create_info.enabledLayerCount       = KB_COUNTOF(vk_validation_layers);
  create_info.ppEnabledLayerNames     = vk_validation_layers;
  create_info.pNext                   = (VkDebugUtilsMessengerCreateInfoEXT*) &debug_create_info;
  
  VK_CHECK_RESULT(vkCreateInstance(&create_info, vk_allocation_callbacks, &vk_instance));

  KB_DEFAULT_FREE(extensions);
}

KB_INTERNAL void init_vk_debug_messenger() {
  VkDebugUtilsMessengerCreateInfoEXT create_info {};
  create_info.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
  create_info.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
  create_info.messageType     = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
  create_info.pfnUserCallback = vk_debug_report_callback;
  create_info.pUserData       = nullptr; // Optional
  
  VK_CHECK_RESULT(CreateDebugUtilsMessengerEXT(vk_instance, &create_info, vk_allocation_callbacks, &vk_debug_messenger));
}

KB_INTERNAL SwapChainSupportDetails query_swap_chain_support(VkPhysicalDevice device) {
  SwapChainSupportDetails details;
  
  vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, vk_surface, &details.capabilities);
  vkGetPhysicalDeviceSurfaceFormatsKHR(device, vk_surface, &details.format_count, nullptr);
  if (details.format_count != 0) {
    details.formats = KB_DEFAULT_ALLOC_TYPE(VkSurfaceFormatKHR, details.format_count);
    vkGetPhysicalDeviceSurfaceFormatsKHR(device, vk_surface, &details.format_count, details.formats);
  }

  vkGetPhysicalDeviceSurfacePresentModesKHR(device, vk_surface, &details.present_mode_count, nullptr);
  if (details.present_mode_count != 0) {
    details.present_modes = KB_DEFAULT_ALLOC_TYPE(VkPresentModeKHR, details.present_mode_count);
    vkGetPhysicalDeviceSurfacePresentModesKHR(device, vk_surface, &details.present_mode_count, details.present_modes);
  }

  return details;
}

KB_INTERNAL VkSurfaceFormatKHR select_swapchain_surface_format(const VkSurfaceFormatKHR* formats, uint32_t count) {
  for (uint32_t i = 0; i < count; ++i) {
    if (formats[i].format == VK_FORMAT_B8G8R8_UNORM && formats[i].colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
      return formats[i];
    }
  }

  return formats[0];
}

KB_INTERNAL VkPresentModeKHR select_swapchain_present_mode(const VkPresentModeKHR* present_modes, uint32_t count, bool want_vsync) {
  for (uint32_t i = 0; i < count; ++i) {
    if (want_vsync  && present_modes[i] == VK_PRESENT_MODE_MAILBOX_KHR)   return present_modes[i];
    if (!want_vsync && present_modes[i] == VK_PRESENT_MODE_IMMEDIATE_KHR) return present_modes[i];
  }

  return present_modes[0];
}

KB_INTERNAL VkExtent2D select_swapchain_extent(VkSurfaceCapabilitiesKHR& capabilities) {
  if (capabilities.currentExtent.width != UINT32_MAX) {
    return capabilities.currentExtent;
  } else {
    Int2 size = kb_platform_surface_get_size();
    
    return {
      CLAMP(size.x, capabilities.minImageExtent.width, capabilities.maxImageExtent.width),
      CLAMP(size.y, capabilities.minImageExtent.height, capabilities.maxImageExtent.height)
    };
  }
}

KB_INTERNAL QueueFamilyIndices find_queue_families(VkPhysicalDevice device) {
  QueueFamilyIndices indices;

  uint32_t queue_family_count = 0;
  vkGetPhysicalDeviceQueueFamilyProperties(device, &queue_family_count, nullptr);
  VkQueueFamilyProperties* queue_family_properties = KB_DEFAULT_ALLOC_TYPE(VkQueueFamilyProperties, queue_family_count);
  vkGetPhysicalDeviceQueueFamilyProperties(device, &queue_family_count, queue_family_properties);
  
  for (uint32_t i = 0; i < queue_family_count; ++i) {
    if (queue_family_properties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT && indices.graphics == UINT32_MAX) {
      indices.graphics = i;
    }

    if (queue_family_properties[i].queueFlags & VK_QUEUE_COMPUTE_BIT && indices.compute == UINT32_MAX) {
      indices.compute = i;
    }

    if (queue_family_properties[i].queueFlags & VK_QUEUE_TRANSFER_BIT && indices.transfer == UINT32_MAX) {
      indices.transfer = i;
    }

    VkBool32 present_support = false;
    vkGetPhysicalDeviceSurfaceSupportKHR(device, i, vk_surface, &present_support);
    if (present_support && indices.present == UINT32_MAX) {
      indices.present = i;
    }
  }
  
  KB_DEFAULT_FREE(queue_family_properties);

  return indices;
}

KB_INTERNAL bool check_vk_physical_device_extension_support(VkPhysicalDevice device) {
  uint32_t extension_count = 0;
  vkEnumerateDeviceExtensionProperties(device, nullptr, &extension_count, nullptr);
  VkExtensionProperties* extension_properties = KB_DEFAULT_ALLOC_TYPE(VkExtensionProperties, extension_count);
  vkEnumerateDeviceExtensionProperties(device, nullptr, &extension_count, extension_properties);

  for (uint32_t i = 0; i < KB_COUNTOF(vk_device_extensions); ++i) {
    bool found = false;
    for (uint32_t e = 0; e < extension_count; ++e) {
      if (kb_strcmp(vk_device_extensions[i], extension_properties[e].extensionName) == 0) {
        found = true;
        break;
      }
    }
    if (!found) { return false; }
  }
  
  KB_DEFAULT_FREE(extension_properties);
  
  return true;
}

// TODO: Rate devices. Do not require discrete device
KB_INTERNAL bool check_vk_physical_device(VkPhysicalDevice device, SwapChainSupportDetails& swapchain_support_details, QueueFamilyIndices& queue_family_indices, VkPhysicalDeviceProperties properties, VkPhysicalDeviceFeatures features) {
  return true 
    && properties.deviceType                        == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU
    && queue_family_indices.graphics                != UINT32_MAX
    && queue_family_indices.present                 != UINT32_MAX
    && queue_family_indices.compute                 != UINT32_MAX
    && queue_family_indices.transfer                != UINT32_MAX
    && swapchain_support_details.format_count       != 0
    && swapchain_support_details.present_mode_count != 0
  ;
}

KB_INTERNAL VkFormat find_supported_vk_format(const VkFormat* candidates, uint32_t count, VkImageTiling tiling, VkFormatFeatureFlags features) {
  for (uint32_t i = 0; i < count; ++i) {
    VkFormatProperties props;
    vkGetPhysicalDeviceFormatProperties(vk_physical_device, candidates[i], &props);
    
    if (tiling == VK_IMAGE_TILING_LINEAR && (props.linearTilingFeatures & features) == features) {
      return candidates[i];
    } else if (tiling == VK_IMAGE_TILING_OPTIMAL && (props.optimalTilingFeatures & features) == features) {
      return candidates[i];
    }
  }

  return VK_FORMAT_UNDEFINED;
}

KB_INTERNAL VkFormat get_vk_depth_format() {
  VkFormat formats[] = { VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT };

  return find_supported_vk_format(
    formats,
    KB_COUNTOF(formats),
    VK_IMAGE_TILING_OPTIMAL,
    VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT
  );
}

KB_INTERNAL bool has_stencil_component(VkFormat format) {
  return format == VK_FORMAT_D32_SFLOAT_S8_UINT || format == VK_FORMAT_D24_UNORM_S8_UINT;
}

KB_INTERNAL void dump_physical_device_details(VkPhysicalDeviceProperties props, VkPhysicalDeviceFeatures features, VkPhysicalDeviceMemoryProperties memory) {
  kb_log_debug("Physical device {}, API: {}", props.deviceName, props.apiVersion);
  kb_log_debug("\tminUniformBufferOffsetAlignment: {}", props.limits.minUniformBufferOffsetAlignment);
  kb_log_debug("\tlineWidthRange: {} {}", props.limits.lineWidthRange[0], props.limits.lineWidthRange[1]);
  kb_log_debug("\tlineWidthGranularity: {}", props.limits.lineWidthGranularity);
}

KB_INTERNAL void init_vk_physical_device() {
  uint32_t device_count = 0;
  vkEnumeratePhysicalDevices(vk_instance, &device_count, nullptr);
  if (device_count == 0) { kb_log_error("No GPUs found!"); }

  VkPhysicalDevice* devices = KB_DEFAULT_ALLOC_TYPE(VkPhysicalDevice, device_count);
  vkEnumeratePhysicalDevices(vk_instance, &device_count, devices);

  for (uint32_t i = 0; i < device_count; ++i) {
    if (!check_vk_physical_device_extension_support(devices[i])) { continue; }

    VkPhysicalDeviceProperties        device_properties;
    VkPhysicalDeviceFeatures          device_features;
    VkPhysicalDeviceMemoryProperties  device_memory_properties;
    
    vkGetPhysicalDeviceMemoryProperties (devices[i], &device_memory_properties);
    vkGetPhysicalDeviceProperties       (devices[i], &device_properties);
    vkGetPhysicalDeviceFeatures         (devices[i], &device_features);

    QueueFamilyIndices queue_family_indices = find_queue_families(devices[i]);
    SwapChainSupportDetails swapchain_support_details = query_swap_chain_support(devices[i]);

    if (check_vk_physical_device(devices[i], swapchain_support_details, queue_family_indices, device_properties, device_features)) {
      vk_physical_device            = devices[i];
      vk_queue_family_indices       = queue_family_indices;
      vk_device_properties          = device_properties;
      vk_device_features            = device_features;
      vk_swapchain_support_details  = swapchain_support_details;
      vk_memory_properties          = device_memory_properties;

      dump_physical_device_details(vk_device_properties, vk_device_features, vk_memory_properties);

      break;
    } else {
      // Destroy SwapChainSupportDetails
      KB_DEFAULT_FREE(swapchain_support_details.formats);
      KB_DEFAULT_FREE(swapchain_support_details.present_modes);
    }
  }

  if (vk_physical_device == VK_NULL_HANDLE) {
    kb_log_error("Failed to select physical device\n");
  }

  KB_DEFAULT_FREE(devices);
}

float queue_priority = 1.0f;
KB_INTERNAL void init_vk_device() {
  uint32_t queue_indices[] = {
    vk_queue_family_indices.compute,
    vk_queue_family_indices.graphics,
    vk_queue_family_indices.present,
    vk_queue_family_indices.transfer
  };
  
  uint32_t unique_queue_indices[KB_COUNTOF(queue_indices)];
  uint32_t unique_queue_count = kb_alg_select_unique(queue_indices,unique_queue_indices, KB_COUNTOF(queue_indices));

  VkDeviceQueueCreateInfo* queue_create_infos = KB_DEFAULT_ALLOC_TYPE(VkDeviceQueueCreateInfo, unique_queue_count);

  for (uint32_t i = 0; i < unique_queue_count; ++i) {
    queue_create_infos[i] = {};
    queue_create_infos[i].sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queue_create_infos[i].queueFamilyIndex  = unique_queue_indices[i];
    queue_create_infos[i].queueCount        = 1;
    queue_create_infos[i].pQueuePriorities  = &queue_priority;
  }

  VkPhysicalDeviceFeatures device_features {};
  device_features.samplerAnisotropy       = VK_TRUE;
  device_features.sampleRateShading       = VK_TRUE;
  // device_features.pipelineStatisticsQuery = VK_TRUE;

  VkDeviceCreateInfo device_create_info {};
  device_create_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
  device_create_info.pQueueCreateInfos        = queue_create_infos;
  device_create_info.queueCreateInfoCount     = unique_queue_count;
  device_create_info.pEnabledFeatures         = &device_features;  
  device_create_info.enabledLayerCount        = KB_COUNTOF(vk_validation_layers);
  device_create_info.ppEnabledLayerNames      = vk_validation_layers;
  device_create_info.enabledExtensionCount    = KB_COUNTOF(vk_device_extensions);
  device_create_info.ppEnabledExtensionNames  = vk_device_extensions;
  
  VK_CHECK_RESULT(vkCreateDevice(vk_physical_device, &device_create_info, vk_allocation_callbacks, &vk_device));

  KB_DEFAULT_FREE(queue_create_infos);
}

KB_INTERNAL void init_vk_queues() {
  vkGetDeviceQueue(vk_device, vk_queue_family_indices.graphics, 0, &vk_queue_graphics);
  vkGetDeviceQueue(vk_device, vk_queue_family_indices.compute,  0, &vk_queue_compute);
  vkGetDeviceQueue(vk_device, vk_queue_family_indices.present,  0, &vk_queue_present);
  vkGetDeviceQueue(vk_device, vk_queue_family_indices.transfer, 0, &vk_queue_transfer);
}

KB_INTERNAL void init_vk_allocator() {
  VmaAllocatorCreateInfo alloc_create_info {};
  alloc_create_info.physicalDevice        = vk_physical_device;
  alloc_create_info.device                = vk_device;
  alloc_create_info.instance              = vk_instance;
  alloc_create_info.vulkanApiVersion      = KB_VULKAN_VERSION;
  alloc_create_info.flags                 = 0;
  alloc_create_info.pAllocationCallbacks  = vk_allocation_callbacks;
  // alloc_create_info.flags |= VMA_ALLOCATOR_CREATE_KHR_DEDICATED_ALLOCATION_BIT;

  VK_CHECK_RESULT(vmaCreateAllocator(&alloc_create_info, &vk_allocator));
}

KB_INTERNAL void init_vk_swapchain() {
  SwapChainSupportDetails details = query_swap_chain_support(vk_physical_device);

  assert(details.capabilities.minImageCount < KB_CONFIG_MAX_FRAMES_IN_FLIGHT);

  uint32_t image_count_wanted = KB_CONFIG_MAX_FRAMES_IN_FLIGHT;
  uint32_t image_count_min    = details.capabilities.minImageCount;
  uint32_t image_count_max    = details.capabilities.maxImageCount > 0 ? details.capabilities.maxImageCount : UINT32_MAX;
  image_count_max             = min_integer(image_count_max, KB_CONFIG_MAX_FRAMES_IN_FLIGHT);

  uint32_t image_count = CLAMP(image_count_wanted, image_count_min, image_count_max);
  
  VkSurfaceFormatKHR  swapchain_surface_format  = select_swapchain_surface_format(details.formats, details.format_count);
  VkPresentModeKHR    swapchain_present_mode    = select_swapchain_present_mode(details.present_modes, details.present_mode_count, vk_want_vsync);
  VkExtent2D          swapchain_extent          = select_swapchain_extent(details.capabilities);

  uint32_t queue_family_indices[] = {vk_queue_family_indices.graphics, vk_queue_family_indices.present};

  VkSwapchainCreateInfoKHR swapchain_create_info {};
  swapchain_create_info.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
  swapchain_create_info.surface           = vk_surface;
  swapchain_create_info.minImageCount     = image_count;
  swapchain_create_info.imageFormat       = swapchain_surface_format.format;
  swapchain_create_info.imageColorSpace   = swapchain_surface_format.colorSpace;
  swapchain_create_info.imageExtent       = swapchain_extent;
  swapchain_create_info.imageArrayLayers  = 1;
  swapchain_create_info.imageUsage        = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
  swapchain_create_info.preTransform      = details.capabilities.currentTransform;
  swapchain_create_info.compositeAlpha    = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
  swapchain_create_info.presentMode       = swapchain_present_mode;
  swapchain_create_info.clipped           = VK_TRUE;
  swapchain_create_info.oldSwapchain      = VK_NULL_HANDLE;

  if (vk_queue_family_indices.graphics != vk_queue_family_indices.present) {
    swapchain_create_info.imageSharingMode      = VK_SHARING_MODE_CONCURRENT;
    swapchain_create_info.queueFamilyIndexCount = 2;
    swapchain_create_info.pQueueFamilyIndices   = queue_family_indices;
  } else {
    swapchain_create_info.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
  }

  VK_CHECK_RESULT(vkCreateSwapchainKHR(vk_device, &swapchain_create_info, vk_allocation_callbacks, &vk_swapchain));
  
  vkGetSwapchainImagesKHR(vk_device, vk_swapchain, &vk_swapchain_frame_slots, nullptr);
  
  assert(vk_swapchain_frame_slots <= KB_CONFIG_MAX_FRAMES_IN_FLIGHT);

  vkGetSwapchainImagesKHR(vk_device, vk_swapchain, &vk_swapchain_frame_slots, frame_resources.swapchain_images);

  vk_swapchain_image_format = swapchain_surface_format.format;
  vk_swapchain_extent = swapchain_extent;
  
  // Destroy SwapChainSupportDetails
  KB_DEFAULT_FREE(details.formats);
  KB_DEFAULT_FREE(details.present_modes);
}

KB_INTERNAL VkImageView create_vk_imageview(VkImage image, VkFormat format, VkImageAspectFlags aspect_flags, uint32_t mip_levels = 1) {
  VkImageViewCreateInfo view_create_info {};
  view_create_info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
  view_create_info.image                            = image;
  view_create_info.viewType                         = VK_IMAGE_VIEW_TYPE_2D;
  view_create_info.format                           = format;
  view_create_info.subresourceRange.aspectMask      = aspect_flags;
  view_create_info.subresourceRange.baseMipLevel    = 0;
  view_create_info.subresourceRange.levelCount      = mip_levels;
  view_create_info.subresourceRange.baseArrayLayer  = 0;
  view_create_info.subresourceRange.layerCount      = 1;

  VkImageView view;
  VK_CHECK_RESULT(vkCreateImageView(vk_device, &view_create_info, vk_allocation_callbacks, &view));
  
  return view;
}

KB_INTERNAL void init_vk_swapchain_imageviews() {
  for (size_t i = 0; i < vk_swapchain_frame_slots; i++) {
    frame_resources.swapchain_imageviews[i] = create_vk_imageview(frame_resources.swapchain_images[i], vk_swapchain_image_format, VK_IMAGE_ASPECT_COLOR_BIT);
  }
}

KB_INTERNAL void init_vk_render_pass() {
  // TODO: Render pass is not recreated at recreate_swapchain
  // This should be OK, if vk_swapchain_image_format does not change.

  // Disable color resolve if msaa is disabled
  // https://www.khronos.org/registry/vulkan/specs/1.1-extensions/html/vkspec.html#VUID-VkSubpassDescription-pResolveAttachments-00848
  bool msaa_enabled = vk_msaa_sample_count != VK_SAMPLE_COUNT_1_BIT;

  VkAttachmentDescription color_attachment {};
  color_attachment.format         = vk_swapchain_image_format;
  color_attachment.samples        = vk_msaa_sample_count;
  color_attachment.loadOp         = VK_ATTACHMENT_LOAD_OP_CLEAR;
  color_attachment.storeOp        = VK_ATTACHMENT_STORE_OP_STORE;
  color_attachment.stencilLoadOp  = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
  color_attachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
  color_attachment.initialLayout  = VK_IMAGE_LAYOUT_UNDEFINED;
  if (msaa_enabled) {
    color_attachment.finalLayout    = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
  } else {
    color_attachment.finalLayout    = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
  }

  VkAttachmentReference color_attachment_ref {};
  color_attachment_ref.attachment = 0;
  color_attachment_ref.layout     = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

  VkAttachmentDescription depth_attachment {};
  depth_attachment.format         = get_vk_depth_format();
  depth_attachment.samples        = vk_msaa_sample_count;
  depth_attachment.loadOp         = VK_ATTACHMENT_LOAD_OP_CLEAR;
  depth_attachment.storeOp        = VK_ATTACHMENT_STORE_OP_DONT_CARE;
  depth_attachment.stencilLoadOp  = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
  depth_attachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
  depth_attachment.initialLayout  = VK_IMAGE_LAYOUT_UNDEFINED;
  depth_attachment.finalLayout    = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

  VkAttachmentReference depth_attachment_ref {};
  depth_attachment_ref.attachment = 1;
  depth_attachment_ref.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

  VkAttachmentReference color_attachment_refs[] = { color_attachment_ref };
  
  VkSubpassDescription subpass {};
  subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
  subpass.colorAttachmentCount    = KB_COUNTOF(color_attachment_refs);
  subpass.pColorAttachments       = color_attachment_refs;
  subpass.pDepthStencilAttachment = &depth_attachment_ref;

  VkAttachmentDescription color_resolve_attachment {};
  color_resolve_attachment.format         = vk_swapchain_image_format;
  color_resolve_attachment.samples        = VK_SAMPLE_COUNT_1_BIT;
  color_resolve_attachment.loadOp         = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
  color_resolve_attachment.storeOp        = VK_ATTACHMENT_STORE_OP_STORE;
  color_resolve_attachment.stencilLoadOp  = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
  color_resolve_attachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
  color_resolve_attachment.initialLayout  = VK_IMAGE_LAYOUT_UNDEFINED;
  color_resolve_attachment.finalLayout    = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
  
  if (msaa_enabled) {
    VkAttachmentReference color_resolve_attachment_ref {};
    color_resolve_attachment_ref.attachment = 2;
    color_resolve_attachment_ref.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    subpass.pResolveAttachments = &color_resolve_attachment_ref;
  }

  VkSubpassDescription subpasses[] = { subpass };

  VkAttachmentDescription attachments[] = { color_attachment, depth_attachment, color_resolve_attachment };
  uint32_t attachment_count = KB_COUNTOF(attachments);
  
  if (!msaa_enabled) { attachment_count--; } // drop color_resolve_attachment

  VkRenderPassCreateInfo render_pass_create_info {};
  render_pass_create_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
  render_pass_create_info.attachmentCount = attachment_count;
  render_pass_create_info.pAttachments    = attachments;
  render_pass_create_info.subpassCount    = KB_COUNTOF(subpasses);
  render_pass_create_info.pSubpasses      = subpasses;

  VK_CHECK_RESULT(vkCreateRenderPass(vk_device, &render_pass_create_info, vk_allocation_callbacks, &vk_render_pass));
}

KB_INTERNAL void fill_buffer_from_rwops(Buffer buffer, kb_rwops* rwops, uint32_t size) {
  void* data = KB_DEFAULT_ALLOC(size);
  kb_rwops_read(rwops, data, size);

  copy_to_buffer(data, size, buffer);

  KB_DEFAULT_FREE(data);
}

KB_INTERNAL Buffer create_and_fill_buffer_with_usage(uint32_t size, VkBufferUsageFlags usage, VmaMemoryUsage memory_usage, kb_rwops* rwops = NULL, VmaAllocationCreateFlags alloc_flags = 0, VkMemoryPropertyFlags memory_property_flags = 0) {
  Buffer buffer = create_buffer(
    size, 
    usage,
    memory_usage,
    alloc_flags,
    memory_property_flags
  );

  if (rwops != NULL) {
    uint32_t read_size = min(size, kb_rwops_size(rwops));
    fill_buffer_from_rwops(buffer, rwops, read_size);
  }

  return buffer;
}

KB_INTERNAL Image create_image(uint32_t width, uint32_t height, uint32_t mip_levels, VkFormat format, VkSampleCountFlagBits samples, VkImageTiling tiling, VkImageUsageFlags usage, VmaMemoryUsage memory_usage = VMA_MEMORY_USAGE_GPU_ONLY, VmaAllocationCreateFlags alloc_flags = 0) {
  Image image;

  VkImageCreateInfo image_create_info {};
  image_create_info.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
  image_create_info.imageType       = VK_IMAGE_TYPE_2D;
  image_create_info.extent.width    = width;
  image_create_info.extent.height   = height;
  image_create_info.extent.depth    = 1;
  image_create_info.mipLevels       = mip_levels;
  image_create_info.arrayLayers     = 1;
  image_create_info.format          = format;
  image_create_info.tiling          = tiling;
  image_create_info.initialLayout   = VK_IMAGE_LAYOUT_UNDEFINED;
  image_create_info.usage           = usage;
  image_create_info.sharingMode     = VK_SHARING_MODE_EXCLUSIVE;
  image_create_info.samples         = samples;
  image_create_info.flags           = 0; // Optional

  VmaAllocationCreateInfo alloc_create_info {};
  alloc_create_info.usage = memory_usage;
  alloc_create_info.flags = alloc_flags;

  VK_CHECK_RESULT(vmaCreateImage(vk_allocator, &image_create_info, &alloc_create_info, &image.image, &image.alloc, &image.alloc_info));

  return image;
}

KB_INTERNAL void destroy_image(Image* image) {
  vmaDestroyImage(vk_allocator, image->image, image->alloc);
}

KB_INTERNAL void copy_buffer_to_image(Buffer* buffer, Image* image, uint32_t width, uint32_t height) {
  VkCommandBuffer command_buffer = command_buffer_begin();
  
  {
    VkBufferImageCopy region {};
    region.bufferOffset                     = 0;
    region.bufferRowLength                  = 0;
    region.bufferImageHeight                = 0;
    region.imageSubresource.aspectMask      = VK_IMAGE_ASPECT_COLOR_BIT;
    region.imageSubresource.mipLevel        = 0;
    region.imageSubresource.baseArrayLayer  = 0;
    region.imageSubresource.layerCount      = 1;
    region.imageOffset                      = { 0, 0, 0 };
    region.imageExtent                      = { width, height, 1 };
    
    vkCmdCopyBufferToImage(command_buffer, buffer->buffer, image->image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &region);
  }

  command_buffer_submit_and_wait(command_buffer);
}

KB_INTERNAL void transition_image_layout(Image* image, VkFormat format, VkImageLayout old_layout, VkImageLayout new_layout, uint32_t mip_levels = 1) {
  VkCommandBuffer command_buffer = command_buffer_begin();
  
  {
    VkImageMemoryBarrier barrier {};
    barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
    barrier.oldLayout                       = old_layout;
    barrier.newLayout                       = new_layout;
    barrier.srcQueueFamilyIndex             = VK_QUEUE_FAMILY_IGNORED;
    barrier.dstQueueFamilyIndex             = VK_QUEUE_FAMILY_IGNORED;
    barrier.image                           = image->image;
    barrier.subresourceRange.baseMipLevel   = 0;
    barrier.subresourceRange.levelCount     = mip_levels;
    barrier.subresourceRange.baseArrayLayer = 0;
    barrier.subresourceRange.layerCount     = 1;
    barrier.srcAccessMask                   = 0; // TODO
    barrier.dstAccessMask                   = 0; // TODO

    VkPipelineStageFlags src_stage;
    VkPipelineStageFlags dst_stage;

    if (old_layout == VK_IMAGE_LAYOUT_UNDEFINED && new_layout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL) {
      barrier.srcAccessMask = 0;
      barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

      src_stage = VK_PIPELINE_STAGE_TOP_OF_TILE_BIT_MARKER;
      dst_stage = VK_PIPELINE_STAGE_TRANSFER_BIT;
    } else if (old_layout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && new_layout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) {
      barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
      barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

      src_stage = VK_PIPELINE_STAGE_TRANSFER_BIT;
      dst_stage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
    } else if (old_layout == VK_IMAGE_LAYOUT_UNDEFINED && new_layout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL) {
      barrier.srcAccessMask = 0;
      barrier.dstAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

      src_stage = VK_PIPELINE_STAGE_TOP_OF_TILE_BIT_MARKER;
      dst_stage = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT; 
    } else {
      printf("Invalid layout transition!");
      assert(false);
    }
    
    if (new_layout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL) {
      barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;

      if (has_stencil_component(format)) {
        barrier.subresourceRange.aspectMask |= VK_IMAGE_ASPECT_STENCIL_BIT;
      }
    } else {
      barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    }

    vkCmdPipelineBarrier(command_buffer, src_stage, dst_stage, 0, 0, nullptr, 0, nullptr, 1, &barrier);
  }
  
  command_buffer_submit_and_wait(command_buffer);
}

KB_INTERNAL VkSampler create_vk_sampler(uint32_t mip_levels, kb_filter filter) {
  VkSamplerCreateInfo sampler_create_info {};
  sampler_create_info.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
  sampler_create_info.magFilter                 = cv_filter[filter];
  sampler_create_info.minFilter                 = cv_filter[filter];
  sampler_create_info.addressModeU              = VK_SAMPLER_ADDRESS_MODE_REPEAT;
  sampler_create_info.addressModeV              = VK_SAMPLER_ADDRESS_MODE_REPEAT;
  sampler_create_info.addressModeW              = VK_SAMPLER_ADDRESS_MODE_REPEAT;
  sampler_create_info.anisotropyEnable          = VK_TRUE;
  sampler_create_info.maxAnisotropy             = 16.0f;
  sampler_create_info.borderColor               = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
  sampler_create_info.unnormalizedCoordinates   = VK_FALSE;
  sampler_create_info.compareEnable             = VK_FALSE;
  sampler_create_info.compareOp                 = VK_COMPARE_OP_ALWAYS;
  sampler_create_info.mipmapMode                = VK_SAMPLER_MIPMAP_MODE_LINEAR;
  sampler_create_info.mipLodBias                = 0.0f;
  sampler_create_info.minLod                    = 0.0f;
  sampler_create_info.maxLod                    = (float) mip_levels;

  VkSampler sampler;
  
  VK_CHECK_RESULT(vkCreateSampler(vk_device, &sampler_create_info, vk_allocation_callbacks, &sampler));
  
  return sampler;
}

KB_INTERNAL void create_descriptor_pool(DescriptorPool& pool, uint32_t descriptor_count) {  
  VkDescriptorPoolSize descriptor_pool_sizes[] = {
    { VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,  descriptor_count },
    { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,          descriptor_count },
  };

  VkDescriptorPoolCreateInfo pool_create_info {};
  pool_create_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
  pool_create_info.poolSizeCount  = KB_COUNTOF(descriptor_pool_sizes);
  pool_create_info.pPoolSizes     = descriptor_pool_sizes;
  pool_create_info.maxSets        = descriptor_count; // TODO: 
  pool_create_info.flags          = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
  
  VK_CHECK_RESULT(vkCreateDescriptorPool(vk_device, &pool_create_info, vk_allocation_callbacks, &pool.descriptor_pool));
}

KB_INTERNAL void reset_descriptor_pool(DescriptorPool& pool) {
  if (pool.count > 0) {
    vkFreeDescriptorSets(vk_device, pool.descriptor_pool, pool.count, pool.descriptors);
  }

  VK_CHECK_RESULT(vkResetDescriptorPool(vk_device, pool.descriptor_pool, 0));
  pool.count = 0;
}

KB_INTERNAL void destroy_vk_descriptor_pool(DescriptorPool& pool) {
  reset_descriptor_pool(pool);
  
  vkDestroyDescriptorPool(vk_device, pool.descriptor_pool, vk_allocation_callbacks);
}

KB_INTERNAL void create_transient_buffer(kb_transient_buffer* transient_buffer, uint32_t size) {
  transient_buffer->memory_position = 0;

  transient_buffer->buffer = create_buffer(size, 
    VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, 
    VMA_MEMORY_USAGE_CPU_TO_GPU, 
    VMA_ALLOCATION_CREATE_MAPPED_BIT, 
    VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
  );
}

KB_INTERNAL void reset_transient_buffer(kb_transient_buffer* transient_buffer) {
  transient_buffer->memory_position = 0;
}

KB_INTERNAL void reset_transient_alloc(kb_allocator* alloc) {
  alloc->stats.count  = 0;
  alloc->stats.mem    = 0;
  alloc->stats.high_water_mark = 0;
}

KB_INTERNAL void destroy_transient_buffer(kb_transient_buffer* transient_buffer) {  
  destroy_buffer(&transient_buffer->buffer);
}

KB_INTERNAL void reset_encoder_pool(kb_encoder_pool* pool) {  
  for (uint32_t i = 0; i < pool->count; ++i) {
    pool->states[i] = {};
  }

  pool->count = 0;
}

KB_INTERNAL void reset_command_buffer_pool(CommandBufferPool* pool) {
  if (pool->count > 0) {
    vkFreeCommandBuffers(vk_device, pool->command_pool, pool->count, pool->command_buffers);
  }

  pool->count = 0;
  vkResetCommandPool(vk_device, pool->command_pool, VK_COMMAND_POOL_RESET_RELEASE_RESOURCES_BIT);
}

KB_INTERNAL CommandBufferPool& get_current_command_buffer_pool() {
  return frame_resources.command_buffer_pools[current_resource_slot];
}

KB_INTERNAL kb_transient_buffer& get_current_transient_buffer() {
  return transient_buffers[current_resource_slot];
}

KB_INTERNAL VkBuffer get_current_transient_vk_buffer() {
  return transient_buffers[current_resource_slot].buffer.buffer;
}

KB_INTERNAL VkDescriptorSet allocate_vk_descriptor_set(VkDescriptorSetLayout layout) {
  DescriptorPool& pool = frame_resources.descriptor_pools[current_resource_slot];

  VkDescriptorSetLayout layouts[] = { layout };

  VkDescriptorSetAllocateInfo alloc_info {};
  alloc_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
  alloc_info.descriptorPool     = pool.descriptor_pool;
  alloc_info.descriptorSetCount = 1;
  alloc_info.pSetLayouts        = layouts;

  uint32_t idx = pool.count++;
  VK_CHECK_RESULT(vkAllocateDescriptorSets(vk_device, &alloc_info, &pool.descriptors[idx]));
  
  return pool.descriptors[idx];
}

KB_INTERNAL VkShaderModule create_vk_shader_module(void* code, uint32_t size) {  
  VkShaderModuleCreateInfo create_info {};
  create_info.sType     = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
  create_info.codeSize  = size;
  create_info.pCode     = (uint32_t*) code;

  VkShaderModule shader_module;
  
  VK_CHECK_RESULT(vkCreateShaderModule(vk_device, &create_info, vk_allocation_callbacks, &shader_module));
  
  return shader_module;
}

KB_INTERNAL void init_vk_framebuffers() {
  // Disable color resolve if msaa is disabled
  // if msaa is disabled, render directly to swapchain image as resolve attachment can't be used
  // https://www.khronos.org/registry/vulkan/specs/1.1-extensions/html/vkspec.html#VUID-VkSubpassDescription-pResolveAttachments-00848
  bool msaa_enabled = vk_msaa_sample_count != VK_SAMPLE_COUNT_1_BIT;

  for (size_t i = 0; i < vk_swapchain_frame_slots; i++) {
    uint32_t attachment_count = 0;

    VkImageView attachments[3] = {};
    if (msaa_enabled) {
      attachment_count = 3;

      attachments[0] = vk_color_image_view;
      attachments[1] = vk_depth_image_view;
      attachments[2] = frame_resources.swapchain_imageviews[i];
    } else {
      attachment_count = 2;
      attachments[0] = frame_resources.swapchain_imageviews[i];
      attachments[1] = vk_depth_image_view;
    }

    VkFramebufferCreateInfo framebuffer_create_info {};
    framebuffer_create_info.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
    framebuffer_create_info.renderPass      = vk_render_pass;
    framebuffer_create_info.attachmentCount = attachment_count;
    framebuffer_create_info.pAttachments    = attachments;
    framebuffer_create_info.width           = vk_swapchain_extent.width;
    framebuffer_create_info.height          = vk_swapchain_extent.height;
    framebuffer_create_info.layers          = 1;

    VK_CHECK_RESULT(vkCreateFramebuffer(vk_device, &framebuffer_create_info, vk_allocation_callbacks, &frame_resources.swapchain_framebuffers[i]));
  }
}

KB_INTERNAL void init_vk_command_pool() {
  VkCommandPoolCreateInfo pool_create_info {};
  pool_create_info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
  pool_create_info.queueFamilyIndex = vk_queue_family_indices.graphics;
  pool_create_info.flags            = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;

  VK_CHECK_RESULT(vkCreateCommandPool(vk_device, &pool_create_info, vk_allocation_callbacks, &vk_command_pool));
  
  for (size_t i = 0; i < vk_swapchain_frame_slots; i++) {
    VkCommandPoolCreateInfo pool_create_info {};
    pool_create_info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    pool_create_info.queueFamilyIndex = vk_queue_family_indices.graphics;
    pool_create_info.flags            = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;

    VK_CHECK_RESULT(vkCreateCommandPool(vk_device, &pool_create_info, vk_allocation_callbacks, &frame_resources.command_buffer_pools[i].command_pool));
  }
}

KB_INTERNAL void init_vk_command_buffers() {
  VkCommandBufferAllocateInfo alloc_info {};
  alloc_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
  alloc_info.commandPool          = vk_command_pool;
  alloc_info.level                = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
  alloc_info.commandBufferCount   = KB_CONFIG_MAX_FRAMES_IN_FLIGHT;

  VK_CHECK_RESULT(vkAllocateCommandBuffers(vk_device, &alloc_info, frame_resources.primary_command_buffers));
}

KB_INTERNAL void init_vk_sync_objects() {
  for (size_t i = 0; i < KB_CONFIG_MAX_FRAMES_IN_FLIGHT; i++) {
    images_in_flight[i] = VK_NULL_HANDLE;
  }

  VkSemaphoreCreateInfo semaphore_create_info {};
  semaphore_create_info.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

  VkFenceCreateInfo fence_create_info{};
  fence_create_info.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
  fence_create_info.flags = VK_FENCE_CREATE_SIGNALED_BIT;
  
  for (size_t i = 0; i < KB_CONFIG_MAX_FRAMES_IN_FLIGHT; i++) {
    VK_CHECK_RESULT(vkCreateSemaphore(vk_device, &semaphore_create_info, vk_allocation_callbacks, &image_available_semaphores[i]));
    VK_CHECK_RESULT(vkCreateSemaphore(vk_device, &semaphore_create_info, vk_allocation_callbacks, &render_finished_semaphores[i]));
    VK_CHECK_RESULT(vkCreateFence(vk_device, &fence_create_info, vk_allocation_callbacks, &fences_in_flight[i]));
  }
}

KB_INTERNAL void init_vk_color_resources() {
  VkFormat color_format = vk_swapchain_image_format;
  
  vk_color_image = create_image(
    vk_swapchain_extent.width, vk_swapchain_extent.height, 
    1, color_format, vk_msaa_sample_count, 
    VK_IMAGE_TILING_OPTIMAL, 
    VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT | VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT, 
    VMA_MEMORY_USAGE_GPU_ONLY
  );

  vk_color_image_view = create_vk_imageview(vk_color_image.image, color_format, VK_IMAGE_ASPECT_COLOR_BIT);
}

KB_INTERNAL void init_vk_depth_resources() {
  VkFormat depth_format = get_vk_depth_format();

  vk_depth_image = create_image(
    vk_swapchain_extent.width, vk_swapchain_extent.height, 
    1, depth_format, vk_msaa_sample_count, 
    VK_IMAGE_TILING_OPTIMAL, 
    VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT, 
    VMA_MEMORY_USAGE_GPU_ONLY
  );

  vk_depth_image_view = create_vk_imageview(vk_depth_image.image, depth_format, VK_IMAGE_ASPECT_DEPTH_BIT);
}

KB_INTERNAL void init_vk_pipeline_cache() { // TODO:
  VkPipelineCacheCreateInfo pipeline_cache_create_info;
  pipeline_cache_create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_CACHE_CREATE_INFO;
  pipeline_cache_create_info.pNext            = NULL;
  pipeline_cache_create_info.flags            = 0;
  pipeline_cache_create_info.initialDataSize  = 0;
  pipeline_cache_create_info.pInitialData     = NULL;

  VK_CHECK_RESULT(vkCreatePipelineCache(vk_device, &pipeline_cache_create_info, vk_allocation_callbacks, &vk_pipeline_cache));
}

KB_INTERNAL void init_transient_buffers() {
  for (uint32_t i = 0; i < KB_CONFIG_MAX_FRAMES_IN_FLIGHT; i++) {
    create_transient_buffer(&transient_buffers[i], KB_CONFIG_TRANSIENT_BUFFER_SIZE);
  }
}

KB_INTERNAL void init_descriptor_pools() {
  for (uint32_t i = 0; i < KB_CONFIG_MAX_FRAMES_IN_FLIGHT; i++) {
    create_descriptor_pool(frame_resources.descriptor_pools[i], KB_CONFIG_MAX_DRAW_CALLS * KB_CONFIG_MAX_UNIFORM_BINDINGS);
  }
}

KB_INTERNAL void cleanup_swapchain() {
  vkDestroyImageView(vk_device, vk_depth_image_view, vk_allocation_callbacks);
  destroy_image(&vk_depth_image);

  vkDestroyImageView(vk_device, vk_color_image_view, vk_allocation_callbacks);
  destroy_image(&vk_color_image);

  for (size_t i = 0; i < vk_swapchain_frame_slots; i++) {
    vkDestroyFramebuffer(vk_device, frame_resources.swapchain_framebuffers[i], vk_allocation_callbacks);
  }

  for (size_t i = 0; i < vk_swapchain_frame_slots; i++) {
    vkDestroyImageView(vk_device, frame_resources.swapchain_imageviews[i], vk_allocation_callbacks);
  }

  vkDestroySwapchainKHR(vk_device, vk_swapchain, vk_allocation_callbacks);
}

KB_INTERNAL void recreate_vk_swapchain() {
  kb_log_info("Recreate swapchain");
  Int2 size { 0, 0 };

  while (size.x == 0 || size.y == 0) {
    size = kb_platform_surface_get_size();
  }
  
  float scale = kb_platform_surface_get_scaling();
  
  vkDeviceWaitIdle(vk_device);
  
  cleanup_swapchain();
  
  init_vk_swapchain();
  init_vk_swapchain_imageviews();
  init_vk_color_resources();
  init_vk_depth_resources();
  init_vk_framebuffers();
}

KB_INTERNAL void wait_until_current_frame_available() {
  VkResult result = vkWaitForFences(vk_device, 1, &fences_in_flight[current_frame_slot], VK_TRUE, UINT64_MAX);
  if (result != VK_SUCCESS) {
    kb_log_error("Failed to wait frame slot!");
  }
}

KB_INTERNAL bool acquire_current_frame_resources() {
  wait_until_current_frame_available();

  uint32_t new_resource_slot;
  
  // We use image indices to manage frame resources
  VkResult result = vkAcquireNextImageKHR(vk_device, vk_swapchain, UINT64_MAX, image_available_semaphores[current_frame_slot], VK_NULL_HANDLE, &new_resource_slot);

  if (result == VK_ERROR_OUT_OF_DATE_KHR) {
    kb_log_debug("Failed to acquire swapchain image. Swapchain out-of-date");
    recreate_vk_swapchain();
    return false;
  } else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
    kb_log_error("Failed to acquire swapchain image");
    return false;
  }
  
  if (images_in_flight[new_resource_slot] != VK_NULL_HANDLE) {
    vkWaitForFences(vk_device, 1, &images_in_flight[new_resource_slot], VK_TRUE, UINT64_MAX);
  }

  reset_descriptor_pool      (frame_resources.descriptor_pools[new_resource_slot]);
  reset_command_buffer_pool  (&frame_resources.command_buffer_pools[new_resource_slot]);
  reset_transient_buffer     (&transient_buffers[new_resource_slot]);
  
  current_resource_slot = new_resource_slot;
  
  return true;
}

KB_INTERNAL void update_primary_command_buffer() {
  CommandBufferPool& cb_pool = get_current_command_buffer_pool();

  VkCommandBuffer& primary_command_buffer = frame_resources.primary_command_buffers[current_resource_slot];
  VkFramebuffer& framebuffer              = frame_resources.swapchain_framebuffers[current_resource_slot];

  VkRenderPass& renderpass  = vk_render_pass;
  VkExtent2D&   extent      = vk_swapchain_extent;
  
  VkClearValue clear_values[2];
  clear_values[0].color         = { { 0.2f, 0.2f, 0.2f, 1.0f } };
  clear_values[1].depthStencil  = { 1.0f, 0 };

  VkRenderPassBeginInfo render_pass_info {};
  render_pass_info.sType              = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
  render_pass_info.renderPass         = renderpass;
  render_pass_info.framebuffer        = framebuffer;
  render_pass_info.renderArea.offset  = { 0, 0 };
  render_pass_info.renderArea.extent  = extent;
  render_pass_info.clearValueCount    = KB_COUNTOF(clear_values);
  render_pass_info.pClearValues       = clear_values;

  VkCommandBufferBeginInfo begin_info {};
  begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

  VK_CHECK_RESULT(vkBeginCommandBuffer(primary_command_buffer, &begin_info));
    vkCmdBeginRenderPass(primary_command_buffer, &render_pass_info, VK_SUBPASS_CONTENTS_SECONDARY_COMMAND_BUFFERS);
      if (cb_pool.count > 0) {
        vkCmdExecuteCommands(primary_command_buffer, cb_pool.count, cb_pool.command_buffers);
      }
    vkCmdEndRenderPass(primary_command_buffer);
  VK_CHECK_RESULT(vkEndCommandBuffer(primary_command_buffer));
}

KB_INTERNAL void submit_current_frame() {
  images_in_flight[current_resource_slot] = fences_in_flight[current_frame_slot];

  VkPipelineStageFlags wait_stages[]  = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
  VkCommandBuffer command_buffers[]   = { frame_resources.primary_command_buffers[current_resource_slot] };

  VkSemaphore wait_semaphores[]       = { image_available_semaphores[current_frame_slot] };
  VkSemaphore signal_semaphores[]     = { render_finished_semaphores[current_frame_slot] };

  VkSubmitInfo submit_info {};
  submit_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
  submit_info.pWaitDstStageMask     = wait_stages;
  submit_info.waitSemaphoreCount    = KB_COUNTOF(wait_semaphores);
  submit_info.pWaitSemaphores       = wait_semaphores;
  submit_info.commandBufferCount    = KB_COUNTOF(command_buffers);
  submit_info.pCommandBuffers       = command_buffers;
  submit_info.signalSemaphoreCount  = KB_COUNTOF(signal_semaphores);
  submit_info.pSignalSemaphores     = signal_semaphores;
  
  vkResetFences(vk_device, 1, &fences_in_flight[current_frame_slot]);
  
  VK_CHECK_RESULT(vkQueueSubmit(vk_queue_graphics, 1, &submit_info, fences_in_flight[current_frame_slot]));
}

KB_INTERNAL void present_current_frame() {
  VkSemaphore     wait_semaphores[] = { render_finished_semaphores[current_frame_slot] };
  VkSwapchainKHR  swapchains[]      = { vk_swapchain };

  VkPresentInfoKHR present_info {};
  present_info.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
  present_info.waitSemaphoreCount = KB_COUNTOF(wait_semaphores);
  present_info.pWaitSemaphores    = wait_semaphores;
  present_info.swapchainCount     = KB_COUNTOF(swapchains);
  present_info.pSwapchains        = swapchains;
  present_info.pImageIndices      = &current_resource_slot;

  VkResult result = vkQueuePresentKHR(vk_queue_present, &present_info);

  if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR) {
    kb_log_debug("Swapchain out-of-date or suboptimal.");
    recreate_vk_swapchain();
  } else if (result != VK_SUCCESS) {
    kb_log_error("Failed to present swapchain image");
  }
}

KB_INTERNAL void create_mipmaps(Image* image, VkFormat format, uint32_t image_width, uint32_t image_height, uint32_t mip_levels) {
  VkFormatProperties format_props;
  vkGetPhysicalDeviceFormatProperties(vk_physical_device, format, &format_props);

  if (!(format_props.optimalTilingFeatures & VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT)) {
    kb_log_warn("kb_texture_data image format does not support linear blitting! Can't create mipmaps");
    return;
  }

  VkCommandBuffer command_buffer = command_buffer_begin();

  {
    VkImageMemoryBarrier barrier {};
    barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
    barrier.image = image->image;
    barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    barrier.subresourceRange.baseArrayLayer = 0;
    barrier.subresourceRange.layerCount = 1;
    barrier.subresourceRange.levelCount = 1;
    
    int32_t mip_width  = image_width;
    int32_t mip_height = image_height;

    for (uint32_t i = 1; i < mip_levels; i++) {
      barrier.subresourceRange.baseMipLevel = i - 1;
      barrier.oldLayout     = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
      barrier.newLayout     = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
      barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
      barrier.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;

      vkCmdPipelineBarrier(command_buffer,
        VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, 0,
        0, nullptr,
        0, nullptr,
        1, &barrier
      );

      VkImageBlit blit {};
      blit.srcOffsets[0] = { 0, 0, 0 };
      blit.srcOffsets[1] = { mip_width, mip_height, 1 };
      blit.srcSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
      blit.srcSubresource.mipLevel = i - 1;
      blit.srcSubresource.baseArrayLayer = 0;
      blit.srcSubresource.layerCount = 1;
      blit.dstOffsets[0] = { 0, 0, 0 };
      blit.dstOffsets[1] = { mip_width > 1 ? mip_width / 2 : 1, mip_height > 1 ? mip_height / 2 : 1, 1 };
      blit.dstSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
      blit.dstSubresource.mipLevel = i;
      blit.dstSubresource.baseArrayLayer = 0;
      blit.dstSubresource.layerCount = 1;

      vkCmdBlitImage(command_buffer,
        image->image, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
        image->image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
        1, &blit,
        VK_FILTER_LINEAR
      );

      barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
      barrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
      barrier.srcAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
      barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

      vkCmdPipelineBarrier(command_buffer,
        VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0,
        0, nullptr,
        0, nullptr,
        1, &barrier
      );

      if (mip_width   > 1) mip_width  /= 2;
      if (mip_height  > 1) mip_height /= 2;
    }

    barrier.subresourceRange.baseMipLevel = mip_levels - 1;
    barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
    barrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
    barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

    vkCmdPipelineBarrier(command_buffer,
      VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0,
      0, nullptr,
      0, nullptr,
      1, &barrier
    );
  
  }
  
  command_buffer_submit_and_wait(command_buffer);
}

//#####################################################################################################################
// Object lifetime
//#####################################################################################################################

KB_API void kb_platform_pipeline_construct(kb_pipeline handle, const kb_pipeline_create_info info) {
  KB_ASSERT_VALID(handle);

  kb_pipeline_ref& pipeline = pipeline_ref(handle);

  pipeline.primitive_size = cv_primitive_size[info.topology];

  VkShaderModule vert_shader_module;
  uint32_t vert_code_size = kb_rwops_size(info.vert_code_rwops);
  void* vert_code = NULL;

  if (vert_code_size > 0) {
    vert_code = KB_DEFAULT_ALLOC(vert_code_size);
    kb_rwops_read(info.vert_code_rwops, vert_code, vert_code_size);
    vert_shader_module = create_vk_shader_module(vert_code, vert_code_size);
  } else {
    kb_log_error("Failed to load vertex shader");
  }

  VkShaderModule frag_shader_module;
  uint32_t frag_code_size = kb_rwops_size(info.frag_code_rwops);
  void* frag_code = NULL;
  if (frag_code_size > 0) {
    frag_code = KB_DEFAULT_ALLOC(frag_code_size);
    kb_rwops_read(info.frag_code_rwops, frag_code, frag_code_size);
    frag_shader_module = create_vk_shader_module(frag_code, frag_code_size);
  } else {
    kb_log_error("Failed to load fragment shader");
  }

  kb_shader_desc& bindings = *kb_graphics_pipeline_bindings(handle);
  kb_shader_desc_begin(&bindings);
    add_shader_description(&bindings, vert_code, vert_code_size);
    add_shader_description(&bindings, frag_code, frag_code_size);
  kb_shader_desc_end(&bindings);
  
  kb_shader_desc_dump(&bindings);
  
  uint32_t set_bindings_count[KB_CONFIG_MAX_UNIFORM_BINDINGS] = { 0 };

  // Buffers
  for (uint32_t buffer_binding_i = 0; buffer_binding_i < bindings.buffer_binding_count; ++buffer_binding_i) {
    kb_shader_buffer_binding& bind = bindings.buffer_bindings[buffer_binding_i];
    uint32_t vk_set     = get_set(bind.index);
    uint32_t vk_binding = get_binding(bind.index);

    uint32_t binding_index = set_bindings_count[vk_set]++;
    VkDescriptorSetLayoutBinding* set_layout_binding = &pipeline.descriptor_set_layout_bindings[vk_set][binding_index];

    set_layout_binding->binding             = vk_binding;
    set_layout_binding->descriptorCount     = 1;
    set_layout_binding->descriptorType      = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    set_layout_binding->stageFlags          = 0;
    set_layout_binding->pImmutableSamplers  = nullptr;
    
    if (bind.stages & KB_SHADER_STAGE_FRAGMENT)
      set_layout_binding->stageFlags |= VK_SHADER_STAGE_FRAGMENT_BIT;
    
    if (bind.stages & KB_SHADER_STAGE_VERTEX)
      set_layout_binding->stageFlags |= VK_SHADER_STAGE_VERTEX_BIT;
  }
  
  // Textures
  for (uint32_t texture_binding_i = 0; texture_binding_i < bindings.texture_binding_count; ++texture_binding_i) {
    kb_shader_texture_binding& bind = bindings.texture_bindings[texture_binding_i];
    uint32_t vk_set     = get_set(bind.index);
    uint32_t vk_binding = get_binding(bind.index);

    uint32_t binding_index = set_bindings_count[vk_set]++;
    VkDescriptorSetLayoutBinding* set_layout_binding = &pipeline.descriptor_set_layout_bindings[vk_set][binding_index];

    set_layout_binding->binding             = vk_binding;
    set_layout_binding->descriptorCount     = 1;
    set_layout_binding->descriptorType      = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    set_layout_binding->stageFlags          = 0;
    set_layout_binding->pImmutableSamplers  = nullptr;
    
    if (bind.stages & KB_SHADER_STAGE_FRAGMENT)
      set_layout_binding->stageFlags |= VK_SHADER_STAGE_FRAGMENT_BIT;
    
    if (bind.stages & KB_SHADER_STAGE_VERTEX)
      set_layout_binding->stageFlags |= VK_SHADER_STAGE_VERTEX_BIT;
  }

  // Create set layouts
  // uint32_t set_layout_count = 0;
  VkDescriptorSetLayout set_layouts[KB_CONFIG_MAX_UNIFORM_BINDINGS];
  for (uint32_t set_i = 0; set_i < KB_CONFIG_MAX_UNIFORM_BINDINGS; ++set_i) {
    if (set_bindings_count[set_i] == 0) continue;
  
    VkDescriptorSetLayoutCreateInfo layout_create_info {};
    layout_create_info.sType        = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    layout_create_info.bindingCount = set_bindings_count[set_i];
    layout_create_info.pBindings    = pipeline.descriptor_set_layout_bindings[set_i];

    VK_CHECK_RESULT(vkCreateDescriptorSetLayout(vk_device, &layout_create_info, vk_allocation_callbacks, &pipeline.descriptor_set_layouts[set_i]));
    set_layouts[pipeline.set_layout_count++] = pipeline.descriptor_set_layouts[set_i];
  }

  kb_vertex_layout* layout = kb_graphics_pipeline_vertex_layout(handle);

  add_vertex_layout(layout, vert_code, vert_code_size);

  kb_vertex_layout_dump(layout);
  
  // Create vertex input attribute descriptions
  VkVertexInputAttributeDescription* descs = pipeline.vertex_input_descriptions;

  for (uint32_t i = 0; i < layout->attrib_count; i++) {
    descs[i].binding  = 0; // TODO: Support multiple vertex bindings
    descs[i].location = layout->attribs[i].location;
    descs[i].format   = cv_vertex_attribute[layout->attribs[i].type][layout->attribs[i].count];
    descs[i].offset   = layout->attribs[i].offset;
  }

  VkPipelineShaderStageCreateInfo vert_shader_stage_info {};
  vert_shader_stage_info.sType  = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
  vert_shader_stage_info.stage  = VK_SHADER_STAGE_VERTEX_BIT;
  vert_shader_stage_info.module = vert_shader_module;
  vert_shader_stage_info.pName  = info.vert_shader.entry == NULL ? "main" : info.vert_shader.entry;

  VkPipelineShaderStageCreateInfo frag_shader_stage_info {};
  frag_shader_stage_info.sType  = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
  frag_shader_stage_info.stage  = VK_SHADER_STAGE_FRAGMENT_BIT;
  frag_shader_stage_info.module = frag_shader_module;
  frag_shader_stage_info.pName  = info.frag_shader.entry == NULL ? "main" : info.frag_shader.entry;

  VkDynamicState dynamic_states[] = { VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR };

  VkPipelineDynamicStateCreateInfo dynamic_state_create_info {};
  dynamic_state_create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
  dynamic_state_create_info.dynamicStateCount = KB_COUNTOF(dynamic_states);
  dynamic_state_create_info.pDynamicStates    = dynamic_states;

  VkPipelineShaderStageCreateInfo shader_stages[] = { vert_shader_stage_info, frag_shader_stage_info };
  
  VkVertexInputBindingDescription vertex_binding_description {};
  vertex_binding_description.binding    = 0;
  vertex_binding_description.stride     = kb_vertex_layout_stride(layout);
  vertex_binding_description.inputRate  = VK_VERTEX_INPUT_RATE_VERTEX;

  VkVertexInputBindingDescription vertex_binding_descriptions[] = { vertex_binding_description };

  VkPipelineVertexInputStateCreateInfo vertex_input_info {};
  vertex_input_info.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
  vertex_input_info.vertexBindingDescriptionCount   = KB_COUNTOF(vertex_binding_descriptions);
  vertex_input_info.pVertexBindingDescriptions      = vertex_binding_descriptions;
  vertex_input_info.vertexAttributeDescriptionCount = layout->attrib_count;
  vertex_input_info.pVertexAttributeDescriptions    = pipeline.vertex_input_descriptions;

  VkPipelineInputAssemblyStateCreateInfo input_assembly_create_info {};
  input_assembly_create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
  input_assembly_create_info.topology                 = cv_topology[info.topology];
  input_assembly_create_info.primitiveRestartEnable   = VK_FALSE;
  
  VkPipelineViewportStateCreateInfo viewport_state_create_info {};
  viewport_state_create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
  viewport_state_create_info.viewportCount = 1;
  viewport_state_create_info.scissorCount  = 1;

  VkPipelineRasterizationStateCreateInfo rasterizer_create_info {};
  rasterizer_create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
  rasterizer_create_info.depthClampEnable         = VK_FALSE;
  rasterizer_create_info.rasterizerDiscardEnable  = VK_FALSE;
  rasterizer_create_info.polygonMode              = VK_POLYGON_MODE_FILL;
  rasterizer_create_info.lineWidth                = 1.0f;
  rasterizer_create_info.cullMode                 = cv_cull_mode[info.cull];
  rasterizer_create_info.frontFace                = VK_FRONT_FACE_COUNTER_CLOCKWISE;
  rasterizer_create_info.depthBiasEnable          = VK_FALSE;

  VkPipelineMultisampleStateCreateInfo multisampling_state_create_info {};
  multisampling_state_create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
  multisampling_state_create_info.rasterizationSamples  = vk_msaa_sample_count;
  multisampling_state_create_info.sampleShadingEnable   = VK_TRUE;
  multisampling_state_create_info.minSampleShading      = 0.2f;

  VkPipelineDepthStencilStateCreateInfo depth_stencil_state_create_info {};
  depth_stencil_state_create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
  depth_stencil_state_create_info.depthTestEnable       = info.depth_test   ? VK_TRUE : VK_FALSE;
  depth_stencil_state_create_info.depthWriteEnable      = info.depth_write  ? VK_TRUE : VK_FALSE;
  depth_stencil_state_create_info.depthCompareOp        = VK_COMPARE_OP_LESS;
  depth_stencil_state_create_info.depthBoundsTestEnable = VK_FALSE;
  depth_stencil_state_create_info.stencilTestEnable     = VK_FALSE;
  
  VkPipelineColorBlendAttachmentState color_blend_attachment_state {};
  color_blend_attachment_state.colorWriteMask       = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
  color_blend_attachment_state.blendEnable          = VK_TRUE;
  color_blend_attachment_state.srcColorBlendFactor  = VK_BLEND_FACTOR_SRC_ALPHA;
  color_blend_attachment_state.dstColorBlendFactor  = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
  color_blend_attachment_state.colorBlendOp         = VK_BLEND_OP_ADD;
  color_blend_attachment_state.srcAlphaBlendFactor  = VK_BLEND_FACTOR_SRC_ALPHA;
  color_blend_attachment_state.dstAlphaBlendFactor  = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
  color_blend_attachment_state.alphaBlendOp         = VK_BLEND_OP_ADD;
  
  VkPipelineColorBlendAttachmentState attachments[] = { color_blend_attachment_state };

  VkPipelineColorBlendStateCreateInfo color_blending_create_info {};
  color_blending_create_info.sType              = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
  color_blending_create_info.logicOpEnable      = VK_FALSE;
  color_blending_create_info.logicOp            = VK_LOGIC_OP_COPY;
  color_blending_create_info.attachmentCount    = KB_COUNTOF(attachments);
  color_blending_create_info.pAttachments       = attachments;
  color_blending_create_info.blendConstants[0]  = 0.0f;
  color_blending_create_info.blendConstants[1]  = 0.0f;
  color_blending_create_info.blendConstants[2]  = 0.0f;
  color_blending_create_info.blendConstants[3]  = 0.0f;

  VkPipelineLayoutCreateInfo pipeline_layout_create_info {};
  pipeline_layout_create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
  pipeline_layout_create_info.setLayoutCount    = pipeline.set_layout_count;
  pipeline_layout_create_info.pSetLayouts       = set_layouts;

  VK_CHECK_RESULT(vkCreatePipelineLayout(vk_device, &pipeline_layout_create_info, vk_allocation_callbacks, &pipeline.pipeline_layout));

  VkGraphicsPipelineCreateInfo pipeline_create_info {};
  pipeline_create_info.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
  pipeline_create_info.stageCount           = KB_COUNTOF(shader_stages);
  pipeline_create_info.pStages              = shader_stages;
  pipeline_create_info.pVertexInputState    = &vertex_input_info;
  pipeline_create_info.pInputAssemblyState  = &input_assembly_create_info;
  pipeline_create_info.pViewportState       = &viewport_state_create_info;
  pipeline_create_info.pRasterizationState  = &rasterizer_create_info;
  pipeline_create_info.pMultisampleState    = &multisampling_state_create_info;
  pipeline_create_info.pDepthStencilState   = &depth_stencil_state_create_info;
  pipeline_create_info.pColorBlendState     = &color_blending_create_info;
  pipeline_create_info.pDynamicState        = &dynamic_state_create_info;
  pipeline_create_info.layout               = pipeline.pipeline_layout;
  pipeline_create_info.renderPass           = vk_render_pass;
  pipeline_create_info.subpass              = 0;
  pipeline_create_info.basePipelineHandle   = VK_NULL_HANDLE;

  VK_CHECK_RESULT(vkCreateGraphicsPipelines(vk_device, vk_pipeline_cache, 1, &pipeline_create_info, vk_allocation_callbacks, &pipeline.pipeline));

  // TODO: Recycle shader modules for extra fasts
  vkDestroyShaderModule(vk_device, frag_shader_module, vk_allocation_callbacks);
  vkDestroyShaderModule(vk_device, vert_shader_module, vk_allocation_callbacks);

  KB_DEFAULT_FREE(vert_code);
  KB_DEFAULT_FREE(frag_code);
}

KB_API void kb_platform_pipeline_destruct(kb_pipeline handle) {
  KB_ASSERT_VALID(handle);

  kb_pipeline_ref& pipeline = pipeline_ref(handle);
  kb_shader_desc* bindings = kb_graphics_pipeline_bindings(handle);

  // TODO: Check that array is counted correctly when using more than 1 sets
  // Should the array be dense vs sparse?
  for (uint32_t i = 0; i < pipeline.set_layout_count; i++) {
    vkDestroyDescriptorSetLayout(vk_device, pipeline.descriptor_set_layouts[i], vk_allocation_callbacks);
  }

  vkDestroyPipeline(vk_device, pipeline.pipeline, vk_allocation_callbacks);
  vkDestroyPipelineLayout(vk_device, pipeline.pipeline_layout, vk_allocation_callbacks);
}

KB_API void kb_platform_texture_construct(kb_texture handle, const kb_texture_create_info info) {
  KB_ASSERT_VALID(handle);

  kb_texture_ref& texture = texture_ref(handle);
  
  const kb_texture_info& tex_info = info.texture;

  VkFormat format       = cv_vk_format[tex_info.format];
  uint32_t format_size  = cv_format_size[tex_info.format];
  uint32_t image_size   = format_size * tex_info.height * tex_info.width;

  texture.mip_levels = info.mipmaps ? floor(kb_log2(max(tex_info.width, tex_info.height))) + 1 : 1;
  
  // Fill staging
  Buffer staging_buffer = create_buffer(image_size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VMA_MEMORY_USAGE_CPU_TO_GPU, VMA_ALLOCATION_CREATE_MAPPED_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
  
  if (info.rwops != NULL) {
    uint32_t read_size = min(image_size, kb_rwops_size(info.rwops));
    kb_rwops_read(info.rwops, staging_buffer.alloc_info.pMappedData, read_size);
  }

  texture.image = create_image(
    tex_info.width, tex_info.height, 
    texture.mip_levels, 
    format,
    VK_SAMPLE_COUNT_1_BIT,
    VK_IMAGE_TILING_OPTIMAL, 
    VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_SAMPLED_BIT
  );

  // Staging buffer to image transition
  transition_image_layout  (&texture.image, format, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, texture.mip_levels);
  
  // Copy image data
  copy_buffer_to_image     (&staging_buffer, &texture.image, tex_info.width, tex_info.height);
  destroy_buffer           (&staging_buffer);
  
  // NOTE: create_mipmaps does the pipeline barrier, so transition_image_layout is not needed when create_mipmaps is called.
  if (info.mipmaps) {
    create_mipmaps(&texture.image, format, tex_info.width, tex_info.height, texture.mip_levels);
  } else {
    transition_image_layout(&texture.image, format, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, texture.mip_levels);
  }

  texture.view    = create_vk_imageview(texture.image.image, format, VK_IMAGE_ASPECT_COLOR_BIT, texture.mip_levels);
  texture.sampler = create_vk_sampler(texture.mip_levels, info.filter);
}

KB_API uint32_t kb_graphics_get_current_resource_slot() {
  return current_resource_slot;
}

KB_API void kb_platform_texture_destruct(kb_texture handle) {
  KB_ASSERT_VALID(handle);

  vkDestroySampler(vk_device, texture_ref(handle).sampler, vk_allocation_callbacks);
  vkDestroyImageView(vk_device, texture_ref(handle).view, vk_allocation_callbacks);
  destroy_image(&texture_ref(handle).image);
}

KB_API void kb_platform_buffer_construct(kb_buffer handle, const kb_buffer_create_info info) {
  KB_ASSERT_VALID(handle);

  KB_ASSERT(info.size > 0, "Buffer size must be greater than zero");
  buffer_ref(handle).vk_buffer = create_and_fill_buffer_with_usage(
    info.size, 
    VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT, 
    info.host_mapped ? VMA_MEMORY_USAGE_CPU_TO_GPU : VMA_MEMORY_USAGE_GPU_ONLY,
    info.rwops,
    info.host_mapped ? VMA_ALLOCATION_CREATE_MAPPED_BIT : 0,
    info.host_mapped ? VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT : 0
  );
}

KB_API void kb_platform_buffer_destruct(kb_buffer handle) { 
  KB_ASSERT_VALID(handle);

  destroy_buffer(&buffer_ref(handle).vk_buffer);
}

KB_API void* kb_platform_graphics_transient_alloc(uint64_t size, uint64_t align) {
  kb_transient_buffer* buffer = &get_current_transient_buffer();

  uint64_t p = align_up(buffer->memory_position, align);
  buffer->memory_position = p + size;

  return (uint8_t*) buffer->buffer.alloc_info.pMappedData + p;
}

KB_API void* kb_platform_graphics_transient_at(uint64_t offset) {
  kb_transient_buffer& trans = transient_buffers[current_resource_slot];
  
  if (offset > trans.memory_position) return NULL;

  return ((uint8_t*) trans.buffer.alloc_info.pMappedData) + offset;
}

KB_API uint64_t kb_platform_graphics_transient_offset(void* ptr) {
  uint8_t* zero = (uint8_t*) transient_buffers[current_resource_slot].buffer.alloc_info.pMappedData;
  return ((uint8_t*) ptr) - zero;
}

KB_API void kb_platform_graphics_init(const kb_graphics_init_info info) {
  vk_want_vsync = info.vsync;
  
  kb_platform_surface_create(info.resolution.x, info.resolution.y);

  vulkan_surface_extension_count = kb_platform_graphics_vulkan_get_surface_extension_count();

  vulkan_surface_extensions = (const char**) KB_DEFAULT_ALLOC(sizeof(const char*) * vulkan_surface_extension_count);
  
  kb_platform_graphics_vulkan_get_surface_extensions(vulkan_surface_extensions);

  init_vk_allocation_callbacks  ();
  init_vk_instance              ();
  
  #if KB_CONFIG_GRAPHICS_VALIDATE
    init_vk_debug_messenger       ();
  #endif
  
  kb_platform_graphics_vulkan_create_surface(vk_instance, &vk_surface, vk_allocation_callbacks);

  init_vk_physical_device       ();

  // Select msaa
  vk_msaa_sample_count = get_max_usable_sample_count(info.msaa);

  init_vk_device                ();
  init_vk_allocator             ();
  init_vk_queues                ();
  init_vk_pipeline_cache        ();
  init_vk_swapchain             ();
  init_vk_swapchain_imageviews  ();
  init_vk_render_pass           ();
  init_vk_command_pool          ();
  init_vk_color_resources       ();
  init_vk_depth_resources       ();
  init_vk_framebuffers          ();
  init_vk_command_buffers       ();
  init_vk_sync_objects          ();
  init_descriptor_pools         ();
  init_transient_buffers        ();

  // Acquire frame resources for the first frame
  bool acquire_successful = acquire_current_frame_resources();
  if (!acquire_successful) { 
    kb_log_error("Failed to aquire frame resources for the first frame!\n");
  }
}

KB_API void kb_platform_graphics_wait_device_idle() {
  vkDeviceWaitIdle(vk_device);
}

KB_API void kb_platform_graphics_deinit() {  
  cleanup_swapchain();
  
  vkDestroyRenderPass(vk_device, vk_render_pass, vk_allocation_callbacks);
  vkDestroyPipelineCache(vk_device, vk_pipeline_cache, vk_allocation_callbacks);

  for (size_t i = 0; i < KB_CONFIG_MAX_FRAMES_IN_FLIGHT; i++) {
    vkDestroySemaphore(vk_device, image_available_semaphores[i], vk_allocation_callbacks);
    vkDestroySemaphore(vk_device, render_finished_semaphores[i], vk_allocation_callbacks);
    vkDestroyFence(vk_device, fences_in_flight[i], vk_allocation_callbacks);
  
    destroy_vk_descriptor_pool(frame_resources.descriptor_pools[i]);
    destroy_transient_buffer(&transient_buffers[i]);

    vkDestroyCommandPool(vk_device, frame_resources.command_buffer_pools[i].command_pool, vk_allocation_callbacks);
  }

  vkDestroyCommandPool(vk_device, vk_command_pool, vk_allocation_callbacks);

  vmaDestroyAllocator(vk_allocator);
  
  vkDestroyDevice(vk_device, vk_allocation_callbacks);

  DestroyDebugUtilsMessengerEXT(vk_instance, vk_debug_messenger, vk_allocation_callbacks);
  
  vkDestroySurfaceKHR(vk_instance, vk_surface, vk_allocation_callbacks);
  
  vkDestroyInstance(vk_instance, vk_allocation_callbacks);
  
  kb_platform_surface_destroy();
}

KB_API Int2 kb_graphics_get_extent() {
  return { (int) vk_swapchain_extent.width, (int) vk_swapchain_extent.height};
}

KB_API void kb_platform_graphics_frame() {
  kb_graphics_run_encoders();

  update_primary_command_buffer();
  submit_current_frame();
  present_current_frame();
  
  current_frame_slot = (current_frame_slot + 1) % KB_CONFIG_MAX_FRAMES_IN_FLIGHT;
  
  // Acquire frame resources for the next frame
  bool acquire_successful = acquire_current_frame_resources();
  
  if (!acquire_successful) { 
    kb_log_error("Failed to aquire frame resources for the next frame!\n");
  }
}

KB_API uint64_t kb_platform_graphics_transient_used() {
  return get_current_transient_buffer().memory_position;
}

KB_API uint64_t kb_platform_graphics_transient_capacity() {
  return KB_CONFIG_TRANSIENT_BUFFER_SIZE;
}

// Platform call
KB_API void kb_platform_graphics_submit_calls(kb_graphics_call* calls, uint32_t call_count) {
  if (call_count == 0) return;
  
  KB_ASSERT_NOT_NULL(calls);

  uint32_t                descriptor_write_count        = 0;
  VkWriteDescriptorSet    descriptor_writes         [MAX_DESCRIPTOR_UPDATES]  = {};

  uint32_t                descriptor_image_info_count   = 0;
  uint32_t                descriptor_buffer_info_count  = 0;
  VkDescriptorImageInfo   descriptor_image_infos    [MAX_DESCRIPTOR_UPDATES]  = {};
  VkDescriptorBufferInfo  descriptor_buffer_infos   [MAX_DESCRIPTOR_UPDATES]  = {};

  VkCommandBuffer cb;
  Int2 extent = kb_graphics_get_extent();

  { // Allocate command buffer
    CommandBufferPool& pool = get_current_command_buffer_pool();
    uint32_t cb_idx = pool.count++;

    VkCommandBufferAllocateInfo alloc_info {};
    alloc_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    alloc_info.pNext                = NULL;
    alloc_info.commandPool          = pool.command_pool;
    alloc_info.level                = VK_COMMAND_BUFFER_LEVEL_SECONDARY;
    alloc_info.commandBufferCount   = 1;

    VK_CHECK_RESULT(vkAllocateCommandBuffers(vk_device, &alloc_info, &pool.command_buffers[cb_idx]));
    
    VkCommandBufferInheritanceInfo inheritance_info {};
    inheritance_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_INHERITANCE_INFO;
    inheritance_info.renderPass   = vk_render_pass;
    inheritance_info.framebuffer  = frame_resources.swapchain_framebuffers[current_resource_slot];
      
    VkCommandBufferBeginInfo begin_info {};
    begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    begin_info.flags            = VK_COMMAND_BUFFER_USAGE_RENDER_PASS_CONTINUE_BIT;
    begin_info.pInheritanceInfo = &inheritance_info;
    
    VK_CHECK_RESULT(vkBeginCommandBuffer(pool.command_buffers[cb_idx], &begin_info));

    cb = pool.command_buffers[cb_idx];
  }

  { // Set scissors and viewport
    VkRect2D scissor {};
    scissor.extent.width  = extent.x;
    scissor.extent.height = extent.y;
    scissor.offset.x      = 0;
    scissor.offset.y      = 0;

    VkViewport viewport {};
    viewport.width     = (float) extent.x;
    viewport.height    = (float) extent.y;
    viewport.x         = (float) 0.0f;
    viewport.y         = (float) 0.0f;
    viewport.minDepth  = (float) 0.0f;
    viewport.maxDepth  = (float) 1.0f;
    
    vkCmdSetScissor(cb, 0, 1, &scissor);
    vkCmdSetViewport(cb, 0, 1, &viewport);
  }

  // Run calls
  for (uint32_t call_i = 0; call_i < call_count; ++call_i) {
    const kb_graphics_call& call = calls[call_i];

    kb_pipeline_ref& pipeline = pipeline_ref(call.pipeline);
    vkCmdBindPipeline(cb, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline.pipeline);

    VkDescriptorSet descriptor_set = allocate_vk_descriptor_set(pipeline.descriptor_set_layouts[0]);

    { // Vertex buffer
      VkBuffer vb = kb_is_valid(call.vertex_buffer.buffer) ?
        buffer_ref(call.vertex_buffer.buffer).vk_buffer.buffer : 
        get_current_transient_buffer().buffer.buffer;

      VkDeviceSize vb_offset = call.vertex_buffer.offset;

      vkCmdBindVertexBuffers(cb, 0, 1, &vb, &vb_offset);
    }

    { // Index buffer
      VkBuffer ib = kb_is_valid(call.index_buffer.buffer) ?
        buffer_ref(call.index_buffer.buffer).vk_buffer.buffer : 
        get_current_transient_buffer().buffer.buffer;
 
      VkDeviceSize ib_offset = call.index_buffer.offset;

      vkCmdBindIndexBuffer(cb, ib, ib_offset, cv_index_type[call.index_buffer.index_type]);
    }

    for (uint32_t bind_i = 0; bind_i < KB_CONFIG_MAX_UNIFORM_BINDINGS; ++bind_i) {
      { // Vertex uniform
        const kb_uniform_binding& uniform_bind = call.vert_uniform_bindings[bind_i];
        if (uniform_bind.size > 0) {        
          VkDescriptorBufferInfo& buffer_info = descriptor_buffer_infos[descriptor_buffer_info_count++];
          buffer_info = {};
          buffer_info.buffer  = get_current_transient_vk_buffer();
          buffer_info.offset  = uniform_bind.offset;
          buffer_info.range   = uniform_bind.size;

          VkWriteDescriptorSet& descriptor_write = descriptor_writes[descriptor_write_count++];
          descriptor_write = {};
          descriptor_write.sType            = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
          descriptor_write.dstSet           = descriptor_set;
          descriptor_write.dstBinding       = get_binding(uniform_bind.index);
          descriptor_write.dstArrayElement  = 0;
          descriptor_write.descriptorType   = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
          descriptor_write.descriptorCount  = 1;
          descriptor_write.pBufferInfo      = &buffer_info;
        }
      }

      { // Fragment uniform
        const kb_uniform_binding& uniform_bind = call.frag_uniform_bindings[bind_i];
        if (uniform_bind.size > 0) {        
          VkDescriptorBufferInfo& buffer_info = descriptor_buffer_infos[descriptor_buffer_info_count++];
          buffer_info = {};
          buffer_info.buffer  = get_current_transient_vk_buffer();
          buffer_info.offset  = uniform_bind.offset;
          buffer_info.range   = uniform_bind.size;

          VkWriteDescriptorSet& descriptor_write = descriptor_writes[descriptor_write_count++];
          descriptor_write = {};
          descriptor_write.sType            = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
          descriptor_write.dstSet           = descriptor_set;
          descriptor_write.dstBinding       = get_binding(uniform_bind.index);
          descriptor_write.dstArrayElement  = 0;
          descriptor_write.descriptorType   = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
          descriptor_write.descriptorCount  = 1;
          descriptor_write.pBufferInfo      = &buffer_info;
        }
      }

      { // Vertex texture
        const kb_texture_binding& texture_bind = call.vert_texture_bindings[bind_i];
        if (kb_is_valid(texture_bind.texture)) {
          VkDescriptorImageInfo& image_info = descriptor_image_infos[descriptor_image_info_count++];
          image_info = {};
          image_info.imageLayout  = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
          image_info.imageView    = texture_ref(texture_bind.texture).view;
          image_info.sampler      = texture_ref(texture_bind.texture).sampler;

          VkWriteDescriptorSet& descriptor_write = descriptor_writes[descriptor_write_count++];
          descriptor_write = {};
          descriptor_write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
          descriptor_write.dstSet           = descriptor_set;
          descriptor_write.dstBinding       = get_binding(texture_bind.index);
          descriptor_write.dstArrayElement  = 0;
          descriptor_write.descriptorType   = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
          descriptor_write.descriptorCount  = 1;
          descriptor_write.pImageInfo       = &image_info;
        }
      }

      { // Fragment texture
        const kb_texture_binding& texture_bind = call.frag_texture_bindings[bind_i];
        if (kb_is_valid(texture_bind.texture)) {
          VkDescriptorImageInfo& image_info = descriptor_image_infos[descriptor_image_info_count++];
          image_info = {};
          image_info.imageLayout  = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
          image_info.imageView    = texture_ref(texture_bind.texture).view;
          image_info.sampler      = texture_ref(texture_bind.texture).sampler;

          VkWriteDescriptorSet& descriptor_write = descriptor_writes[descriptor_write_count++];
          descriptor_write = {};
          descriptor_write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
          descriptor_write.dstSet           = descriptor_set;
          descriptor_write.dstBinding       = get_binding(texture_bind.index);
          descriptor_write.dstArrayElement  = 0;
          descriptor_write.descriptorType   = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
          descriptor_write.descriptorCount  = 1;
          descriptor_write.pImageInfo       = &image_info;
        }
      }
    }
    
    if (descriptor_write_count > 0) {
      vkUpdateDescriptorSets(vk_device, descriptor_write_count, descriptor_writes, 0, NULL);
      vkCmdBindDescriptorSets(
        cb, 
        VK_PIPELINE_BIND_POINT_GRAPHICS, 
        pipeline.pipeline_layout, 
        0, 1, 
        &descriptor_set,
        0, nullptr
      );
      
      descriptor_write_count        = 0;
      descriptor_image_info_count   = 0;
      descriptor_buffer_info_count  = 0;
    }

    vkCmdDrawIndexed(cb, call.info.index_count, 1, call.info.first_index, call.info.first_vertex, 0);
  }
 
  vkEndCommandBuffer(cb);
}
