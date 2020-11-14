// ============================================================================
//  Kimberlite
//
//  Copyright 2020 Toni Pesola. All Rights Reserved.
// ============================================================================

#import <Cocoa/Cocoa.h>
#import <Foundation/Foundation.h>
#import <QuartzCore/QuartzCore.h>
#import <Metal/Metal.h>

#include <kb/core.h>
#include <kb/platform.h>

static CAMetalLayer* create_metal_layer() {
  NSWindow* window      = (NSWindow*) kb_platform_surface_get_native_window_handle();
  NSView* view_content  = (NSView*) window.contentView;
  
  CAMetalLayer* layer = CAMetalLayer.layer;

  layer.contentsScale = [window backingScaleFactor];

  [view_content setWantsLayer : YES];
  [view_content setLayer : layer];
  
  return layer;
}

#if KB_GRAPHICS_LIB_VULKAN

#include <vulkan/vulkan.h>
#include <vulkan/vulkan_macos.h>

#include "platform_graphics_vulkan.h"

static const char* const vk_surface_extensions_macos[] = {
  VK_KHR_SURFACE_EXTENSION_NAME, 
  VK_MVK_MACOS_SURFACE_EXTENSION_NAME,
};

uint32_t kb_platform_graphics_vulkan_get_surface_extension_count() {
  return KB_COUNTOF(vk_surface_extensions_macos);
};

void kb_platform_graphics_vulkan_get_surface_extensions(const char** names) {
  for (uint32_t i = 0; i < KB_COUNTOF(vk_surface_extensions_macos); i++) {
    names[i] = vk_surface_extensions_macos[i];
  }
};

void kb_platform_graphics_vulkan_create_surface(VkInstance instance, VkSurfaceKHR* surface, VkAllocationCallbacks* alloc_cbs) {
  VK_GET_EXT_FUNC(vkCreateMacOSSurfaceMVK, instance);
      
  VkMacOSSurfaceCreateInfoMVK surface_create_info;
  surface_create_info.sType = VK_STRUCTURE_TYPE_MACOS_SURFACE_CREATE_INFO_MVK;
  surface_create_info.pNext = NULL;
  surface_create_info.flags = 0;
  surface_create_info.pView = (__bridge void*) create_metal_layer();

  VK_CHECK_RESULT(vkCreateMacOSSurfaceMVK(instance, &surface_create_info, alloc_cbs, surface));
}

#endif
