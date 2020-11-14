// ============================================================================
//  Kimberlite
//
//  Copyright 2020 Toni Pesola. All Rights Reserved.
// ============================================================================

#pragma once

#include <vulkan/vulkan.h>

#define VK_GET_EXT_FUNC(_name, _instance) PFN_##_name _name = (PFN_##_name) vkGetInstanceProcAddr(_instance, #_name)

#define VK_CHECK_RESULT(f) {                                                          \
  VkResult res = (f);                                                                 \
  if (res != VK_SUCCESS) {                                                            \
    kb_log_error("Fatal : VkResult == VK_SUCCESS in {} : {}", __FILE__, __LINE__);    \
    assert(res == VK_SUCCESS);                                                        \
  }                                                                                   \
}

uint32_t kb_platform_graphics_vulkan_get_surface_extension_count();
void kb_platform_graphics_vulkan_get_surface_extensions(const char** names);
void kb_platform_graphics_vulkan_create_surface(VkInstance instance, VkSurfaceKHR* surface, VkAllocationCallbacks* alloc_cbs);
