/*==============  SD Engine License ==============
MIT License

Copyright (c) 2019 Kuan-Chih, Chen

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

/*! \file      VulkanWrapper.h
 *  \brief     Introduce of VulkanWrapper
 *  \author    Kuan-Chih, Chen
 *  \date      2020/01/28
 *  \copyright MIT License.
 */

#pragma once

#define VK_NO_PROTOTYPES

#if defined(_WIN32) || defined(_WIN64)
    //#define VK_USE_PLATFORM_WIN32_KHR
    //#include <vulkan/vulkan_win32.h>
#elif defined(__linux__) 
    //#define VK_USE_PLATFORM_XLIB_KHR
    //#include <vulkan/vulkan_xlib.h>
    //#include <vulkan/vulkan_xlib_xrandr.h>
#elif defined(__ANDROID__)
    //#define VK_USE_PLATFORM_ANDROID_KHR
    //#include <vulkan/vulkan_android.h>
#endif

#if USE_DEBUG_EXTENTIONS
    #include <vulkan/vk_sdk_platform.h>
#endif
#include <vulkan/vulkan.h>

#include "SDEngineMacro.h"

///*
#define vkCreateInstance GetVulkanFunc()->m_vkCreateInstance
#define vkDestroyInstance GetVulkanFunc()->m_vkDestroyInstance
#define vkEnumeratePhysicalDevices GetVulkanFunc()->m_vkEnumeratePhysicalDevices
#define vkGetPhysicalDeviceFeatures GetVulkanFunc()->m_vkGetPhysicalDeviceFeatures
#define vkGetPhysicalDeviceFormatProperties GetVulkanFunc()->m_vkGetPhysicalDeviceFormatProperties
#define vkGetPhysicalDeviceImageFormatProperties GetVulkanFunc()->m_vkGetPhysicalDeviceImageFormatProperties
#define vkGetPhysicalDeviceProperties GetVulkanFunc()->m_vkGetPhysicalDeviceProperties
#define vkGetPhysicalDeviceQueueFamilyProperties GetVulkanFunc()->m_vkGetPhysicalDeviceQueueFamilyProperties
#define vkGetPhysicalDeviceMemoryProperties GetVulkanFunc()->m_vkGetPhysicalDeviceMemoryProperties
#define vkGetInstanceProcAddr GetVulkanFunc()->m_vkGetInstanceProcAddr
#define vkGetDeviceProcAddr GetVulkanFunc()->m_vkGetDeviceProcAddr
#define vkCreateDevice GetVulkanFunc()->m_vkCreateDevice
#define vkDestroyDevice GetVulkanFunc()->m_vkDestroyDevice
#define vkEnumerateInstanceExtensionProperties GetVulkanFunc()->m_vkEnumerateInstanceExtensionProperties
#define vkEnumerateDeviceExtensionProperties GetVulkanFunc()->m_vkEnumerateDeviceExtensionProperties
#define vkEnumerateInstanceLayerProperties GetVulkanFunc()->m_vkEnumerateInstanceLayerProperties
#define vkEnumerateDeviceLayerProperties GetVulkanFunc()->m_vkEnumerateDeviceLayerProperties
#define vkGetDeviceQueue GetVulkanFunc()->m_vkGetDeviceQueue
#define vkQueueSubmit GetVulkanFunc()->m_vkQueueSubmit
#define vkQueueWaitIdle GetVulkanFunc()->m_vkQueueWaitIdle
#define vkDeviceWaitIdle GetVulkanFunc()->m_vkDeviceWaitIdle
#define vkAllocateMemory GetVulkanFunc()->m_vkAllocateMemory
#define vkFreeMemory GetVulkanFunc()->m_vkFreeMemory
#define vkMapMemory GetVulkanFunc()->m_vkMapMemory
#define vkUnmapMemory GetVulkanFunc()->m_vkUnmapMemory
#define vkFlushMappedMemoryRanges GetVulkanFunc()->m_vkFlushMappedMemoryRanges
#define vkInvalidateMappedMemoryRanges GetVulkanFunc()->m_vkInvalidateMappedMemoryRanges
#define vkGetDeviceMemoryCommitment GetVulkanFunc()->m_vkGetDeviceMemoryCommitment
#define vkBindBufferMemory GetVulkanFunc()->m_vkBindBufferMemory
#define vkBindImageMemory GetVulkanFunc()->m_vkBindImageMemory
#define vkGetBufferMemoryRequirements GetVulkanFunc()->m_vkGetBufferMemoryRequirements
#define vkGetImageMemoryRequirements GetVulkanFunc()->m_vkGetImageMemoryRequirements
#define vkGetImageSparseMemoryRequirements GetVulkanFunc()->m_vkGetImageSparseMemoryRequirements
#define vkGetPhysicalDeviceSparseImageFormatProperties GetVulkanFunc()->m_vkGetPhysicalDeviceSparseImageFormatProperties
#define vkQueueBindSparse GetVulkanFunc()->m_vkQueueBindSparse
#define vkCreateFence GetVulkanFunc()->m_vkCreateFence
#define vkDestroyFence GetVulkanFunc()->m_vkDestroyFence
#define vkResetFences GetVulkanFunc()->m_vkResetFences
#define vkGetFenceStatus GetVulkanFunc()->m_vkGetFenceStatus
#define vkWaitForFences GetVulkanFunc()->m_vkWaitForFences
#define vkCreateSemaphore GetVulkanFunc()->m_vkCreateSemaphore
#define vkDestroySemaphore GetVulkanFunc()->m_vkDestroySemaphore
#define vkCreateEvent GetVulkanFunc()->m_vkCreateEvent
#define vkDestroyEvent GetVulkanFunc()->m_vkDestroyEvent
#define vkGetEventStatus GetVulkanFunc()->m_vkGetEventStatus
#define vkSetEvent GetVulkanFunc()->m_vkSetEvent
#define vkResetEvent GetVulkanFunc()->m_vkResetEvent
#define vkCreateQueryPool GetVulkanFunc()->m_vkCreateQueryPool
#define vkDestroyQueryPool GetVulkanFunc()->m_vkDestroyQueryPool
#define vkGetQueryPoolResults GetVulkanFunc()->m_vkGetQueryPoolResults
#define vkCreateBuffer GetVulkanFunc()->m_vkCreateBuffer
#define vkDestroyBuffer GetVulkanFunc()->m_vkDestroyBuffer
#define vkCreateBufferView GetVulkanFunc()->m_vkCreateBufferView
#define vkDestroyBufferView GetVulkanFunc()->m_vkDestroyBufferView
#define vkCreateImage GetVulkanFunc()->m_vkCreateImage
#define vkDestroyImage GetVulkanFunc()->m_vkDestroyImage
#define vkGetImageSubresourceLayout GetVulkanFunc()->m_vkGetImageSubresourceLayout
#define vkCreateImageView GetVulkanFunc()->m_vkCreateImageView
#define vkDestroyImageView GetVulkanFunc()->m_vkDestroyImageView
#define vkCreateShaderModule GetVulkanFunc()->m_vkCreateShaderModule
#define vkDestroyShaderModule GetVulkanFunc()->m_vkDestroyShaderModule
#define vkCreatePipelineCache GetVulkanFunc()->m_vkCreatePipelineCache
#define vkDestroyPipelineCache GetVulkanFunc()->m_vkDestroyPipelineCache
#define vkGetPipelineCacheData GetVulkanFunc()->m_vkGetPipelineCacheData
#define vkMergePipelineCaches GetVulkanFunc()->m_vkMergePipelineCaches
#define vkCreateGraphicsPipelines GetVulkanFunc()->m_vkCreateGraphicsPipelines
#define vkCreateComputePipelines GetVulkanFunc()->m_vkCreateComputePipelines
#define vkDestroyPipeline GetVulkanFunc()->m_vkDestroyPipeline
#define vkCreatePipelineLayout GetVulkanFunc()->m_vkCreatePipelineLayout
#define vkDestroyPipelineLayout GetVulkanFunc()->m_vkDestroyPipelineLayout
#define vkCreateSampler GetVulkanFunc()->m_vkCreateSampler
#define vkDestroySampler GetVulkanFunc()->m_vkDestroySampler
#define vkCreateDescriptorSetLayout GetVulkanFunc()->m_vkCreateDescriptorSetLayout
#define vkDestroyDescriptorSetLayout GetVulkanFunc()->m_vkDestroyDescriptorSetLayout
#define vkCreateDescriptorPool GetVulkanFunc()->m_vkCreateDescriptorPool
#define vkDestroyDescriptorPool GetVulkanFunc()->m_vkDestroyDescriptorPool
#define vkResetDescriptorPool GetVulkanFunc()->m_vkResetDescriptorPool
#define vkAllocateDescriptorSets GetVulkanFunc()->m_vkAllocateDescriptorSets
#define vkFreeDescriptorSets GetVulkanFunc()->m_vkFreeDescriptorSets
#define vkUpdateDescriptorSets GetVulkanFunc()->m_vkUpdateDescriptorSets
#define vkCreateFramebuffer GetVulkanFunc()->m_vkCreateFramebuffer
#define vkDestroyFramebuffer GetVulkanFunc()->m_vkDestroyFramebuffer
#define vkCreateRenderPass GetVulkanFunc()->m_vkCreateRenderPass
#define vkDestroyRenderPass GetVulkanFunc()->m_vkDestroyRenderPass
#define vkGetRenderAreaGranularity GetVulkanFunc()->m_vkGetRenderAreaGranularity
#define vkCreateCommandPool GetVulkanFunc()->m_vkCreateCommandPool
#define vkDestroyCommandPool GetVulkanFunc()->m_vkDestroyCommandPool
#define vkResetCommandPool GetVulkanFunc()->m_vkResetCommandPool
#define vkAllocateCommandBuffers GetVulkanFunc()->m_vkAllocateCommandBuffers
#define vkFreeCommandBuffers GetVulkanFunc()->m_vkFreeCommandBuffers
#define vkBeginCommandBuffer GetVulkanFunc()->m_vkBeginCommandBuffer
#define vkEndCommandBuffer GetVulkanFunc()->m_vkEndCommandBuffer
#define vkResetCommandBuffer GetVulkanFunc()->m_vkResetCommandBuffer
#define vkCmdBindPipeline GetVulkanFunc()->m_vkCmdBindPipeline
#define vkCmdSetViewport GetVulkanFunc()->m_vkCmdSetViewport
#define vkCmdSetScissor GetVulkanFunc()->m_vkCmdSetScissor
#define vkCmdSetLineWidth GetVulkanFunc()->m_vkCmdSetLineWidth
#define vkCmdSetDepthBias GetVulkanFunc()->m_vkCmdSetDepthBias
#define vkCmdSetBlendConstants GetVulkanFunc()->m_vkCmdSetBlendConstants
#define vkCmdSetDepthBounds GetVulkanFunc()->m_vkCmdSetDepthBounds
#define vkCmdSetStencilCompareMask GetVulkanFunc()->m_vkCmdSetStencilCompareMask
#define vkCmdSetStencilWriteMask GetVulkanFunc()->m_vkCmdSetStencilWriteMask
#define vkCmdSetStencilReference GetVulkanFunc()->m_vkCmdSetStencilReference
#define vkCmdBindDescriptorSets GetVulkanFunc()->m_vkCmdBindDescriptorSets
#define vkCmdBindIndexBuffer GetVulkanFunc()->m_vkCmdBindIndexBuffer
#define vkCmdBindVertexBuffers GetVulkanFunc()->m_vkCmdBindVertexBuffers
#define vkCmdDraw GetVulkanFunc()->m_vkCmdDraw
#define vkCmdDrawIndexed GetVulkanFunc()->m_vkCmdDrawIndexed
#define vkCmdDrawIndirect GetVulkanFunc()->m_vkCmdDrawIndirect
#define vkCmdDrawIndexedIndirect GetVulkanFunc()->m_vkCmdDrawIndexedIndirect
#define vkCmdDispatch GetVulkanFunc()->m_vkCmdDispatch
#define vkCmdDispatchIndirect GetVulkanFunc()->m_vkCmdDispatchIndirect
#define vkCmdCopyBuffer GetVulkanFunc()->m_vkCmdCopyBuffer
#define vkCmdCopyImage GetVulkanFunc()->m_vkCmdCopyImage
#define vkCmdBlitImage GetVulkanFunc()->m_vkCmdBlitImage
#define vkCmdCopyBufferToImage GetVulkanFunc()->m_vkCmdCopyBufferToImage
#define vkCmdCopyImageToBuffer GetVulkanFunc()->m_vkCmdCopyImageToBuffer
#define vkCmdUpdateBuffer GetVulkanFunc()->m_vkCmdUpdateBuffer
#define vkCmdFillBuffer GetVulkanFunc()->m_vkCmdFillBuffer
#define vkCmdClearColorImage GetVulkanFunc()->m_vkCmdClearColorImage
#define vkCmdClearDepthStencilImage GetVulkanFunc()->m_vkCmdClearDepthStencilImage
#define vkCmdClearAttachments GetVulkanFunc()->m_vkCmdClearAttachments
#define vkCmdResolveImage GetVulkanFunc()->m_vkCmdResolveImage
#define vkCmdSetEvent GetVulkanFunc()->m_vkCmdSetEvent
#define vkCmdResetEvent GetVulkanFunc()->m_vkCmdResetEvent
#define vkCmdWaitEvents GetVulkanFunc()->m_vkCmdWaitEvents
#define vkCmdPipelineBarrier GetVulkanFunc()->m_vkCmdPipelineBarrier
#define vkCmdBeginQuery GetVulkanFunc()->m_vkCmdBeginQuery
#define vkCmdEndQuery GetVulkanFunc()->m_vkCmdEndQuery
#define vkCmdResetQueryPool GetVulkanFunc()->m_vkCmdResetQueryPool
#define vkCmdWriteTimestamp GetVulkanFunc()->m_vkCmdWriteTimestamp
#define vkCmdCopyQueryPoolResults GetVulkanFunc()->m_vkCmdCopyQueryPoolResults
#define vkCmdPushConstants GetVulkanFunc()->m_vkCmdPushConstants
#define vkCmdBeginRenderPass GetVulkanFunc()->m_vkCmdBeginRenderPass
#define vkCmdNextSubpass GetVulkanFunc()->m_vkCmdNextSubpass
#define vkCmdEndRenderPass GetVulkanFunc()->m_vkCmdEndRenderPass
#define vkCmdExecuteCommands GetVulkanFunc()->m_vkCmdExecuteCommands

// VK_KHR_surface
#define vkDestroySurfaceKHR GetVulkanFunc()->m_vkDestroySurfaceKHR
#define vkGetPhysicalDeviceSurfaceSupportKHR GetVulkanFunc()->m_vkGetPhysicalDeviceSurfaceSupportKHR
#define vkGetPhysicalDeviceSurfaceCapabilitiesKHR GetVulkanFunc()->m_vkGetPhysicalDeviceSurfaceCapabilitiesKHR
#define vkGetPhysicalDeviceSurfaceFormatsKHR GetVulkanFunc()->m_vkGetPhysicalDeviceSurfaceFormatsKHR
#define vkGetPhysicalDeviceSurfacePresentModesKHR GetVulkanFunc()->m_vkGetPhysicalDeviceSurfacePresentModesKHR

// VK_KHR_swapchain
#define vkCreateSwapchainKHR GetVulkanFunc()->m_vkCreateSwapchainKHR
#define vkDestroySwapchainKHR GetVulkanFunc()->m_vkDestroySwapchainKHR
#define vkGetSwapchainImagesKHR GetVulkanFunc()->m_vkGetSwapchainImagesKHR
#define vkAcquireNextImageKHR GetVulkanFunc()->m_vkAcquireNextImageKHR
#define vkQueuePresentKHR GetVulkanFunc()->m_vkQueuePresentKHR

// VK_KHR_display
#define vkGetPhysicalDeviceDisplayPropertiesKHR GetVulkanFunc()->m_vkGetPhysicalDeviceDisplayPropertiesKHR
#define vkGetPhysicalDeviceDisplayPlanePropertiesKHR GetVulkanFunc()->m_vkGetPhysicalDeviceDisplayPlanePropertiesKHR
#define vkGetDisplayPlaneSupportedDisplaysKHR GetVulkanFunc()->m_vkGetDisplayPlaneSupportedDisplaysKHR
#define vkGetDisplayModePropertiesKHR GetVulkanFunc()->m_vkGetDisplayModePropertiesKHR
#define vkCreateDisplayModeKHR GetVulkanFunc()->m_vkCreateDisplayModeKHR
#define vkGetDisplayPlaneCapabilitiesKHR GetVulkanFunc()->m_vkGetDisplayPlaneCapabilitiesKHR
#define vkCreateDisplayPlaneSurfaceKHR GetVulkanFunc()->m_vkCreateDisplayPlaneSurfaceKHR

// VK_KHR_display_swapchain
#define vkCreateSharedSwapchainsKHR GetVulkanFunc()->m_vkCreateSharedSwapchainsKHR

#ifdef VK_USE_PLATFORM_XLIB_KHR
// VK_KHR_xlib_surface
#define vkCreateXlibSurfaceKHR GetVulkanFunc()->m_vkCreateXlibSurfaceKHR
#define vkGetPhysicalDeviceXlibPresentationSupportKHR GetVulkanFunc()->m_vkGetPhysicalDeviceXlibPresentationSupportKHR
#endif

#ifdef VK_USE_PLATFORM_XCB_KHR
// VK_KHR_xcb_surface
#define vkCreateXcbSurfaceKHR GetVulkanFunc()->m_vkCreateXcbSurfaceKHR
#define vkGetPhysicalDeviceXcbPresentationSupportKHR GetVulkanFunc()->m_vkGetPhysicalDeviceXcbPresentationSupportKHR
#endif

#ifdef VK_USE_PLATFORM_WAYLAND_KHR
// VK_KHR_wayland_surface
#define vkCreateWaylandSurfaceKHR GetVulkanFunc()->m_vkCreateWaylandSurfaceKHR
#define vkGetPhysicalDeviceWaylandPresentationSupportKHR GetVulkanFunc()->m_vkGetPhysicalDeviceWaylandPresentationSupportKHR
#endif

#ifdef VK_USE_PLATFORM_MIR_KHR
// VK_KHR_mir_surface
#define vkCreateMirSurfaceKHR GetVulkanFunc()->m_vkCreateMirSurfaceKHR
#define vkGetPhysicalDeviceMirPresentationSupportKHR GetVulkanFunc()->m_vkGetPhysicalDeviceMirPresentationSupportKHR
#endif

#ifdef VK_USE_PLATFORM_ANDROID_KHR
// VK_KHR_android_surface
#define vkCreateAndroidSurfaceKHR GetVulkanFunc()->m_vkCreateAndroidSurfaceKHR
#endif

#ifdef VK_USE_PLATFORM_WIN32_KHR
// VK_KHR_win32_surface
#define vkCreateWin32SurfaceKHR GetVulkanFunc()->m_vkCreateWin32SurfaceKHR
#define vkGetPhysicalDeviceWin32PresentationSupportKHR GetVulkanFunc()->m_vkGetPhysicalDeviceWin32PresentationSupportKHR
#endif

#ifdef USE_DEBUG_EXTENTIONS
// VK_EXT_debug_report
#define vkCreateDebugReportCallbackEXT GetVulkanFunc()->m_vkCreateDebugReportCallbackEXT
#define vkDestroyDebugReportCallbackEXT GetVulkanFunc()->m_vkDestroyDebugReportCallbackEXT
#define vkDebugReportMessageEXT GetVulkanFunc()->m_vkDebugReportMessageEXT
#endif


struct VulkanFunc
{
public:
    PFN_vkCreateInstance m_vkCreateInstance;
    PFN_vkDestroyInstance m_vkDestroyInstance;
    PFN_vkEnumeratePhysicalDevices m_vkEnumeratePhysicalDevices;
    PFN_vkGetPhysicalDeviceFeatures m_vkGetPhysicalDeviceFeatures;
    PFN_vkGetPhysicalDeviceFormatProperties m_vkGetPhysicalDeviceFormatProperties;
    PFN_vkGetPhysicalDeviceImageFormatProperties m_vkGetPhysicalDeviceImageFormatProperties;
    PFN_vkGetPhysicalDeviceProperties m_vkGetPhysicalDeviceProperties;
    PFN_vkGetPhysicalDeviceQueueFamilyProperties m_vkGetPhysicalDeviceQueueFamilyProperties;
    PFN_vkGetPhysicalDeviceMemoryProperties m_vkGetPhysicalDeviceMemoryProperties;
    PFN_vkGetInstanceProcAddr m_vkGetInstanceProcAddr;
    PFN_vkGetDeviceProcAddr m_vkGetDeviceProcAddr;
    PFN_vkCreateDevice m_vkCreateDevice;
    PFN_vkDestroyDevice m_vkDestroyDevice;
    PFN_vkEnumerateInstanceExtensionProperties m_vkEnumerateInstanceExtensionProperties;
    PFN_vkEnumerateDeviceExtensionProperties m_vkEnumerateDeviceExtensionProperties;
    PFN_vkEnumerateInstanceLayerProperties m_vkEnumerateInstanceLayerProperties;
    PFN_vkEnumerateDeviceLayerProperties m_vkEnumerateDeviceLayerProperties;
    PFN_vkGetDeviceQueue m_vkGetDeviceQueue;
    PFN_vkQueueSubmit m_vkQueueSubmit;
    PFN_vkQueueWaitIdle m_vkQueueWaitIdle;
    PFN_vkDeviceWaitIdle m_vkDeviceWaitIdle;
    PFN_vkAllocateMemory m_vkAllocateMemory;
    PFN_vkFreeMemory m_vkFreeMemory;
    PFN_vkMapMemory m_vkMapMemory;
    PFN_vkUnmapMemory m_vkUnmapMemory;
    PFN_vkFlushMappedMemoryRanges m_vkFlushMappedMemoryRanges;
    PFN_vkInvalidateMappedMemoryRanges m_vkInvalidateMappedMemoryRanges;
    PFN_vkGetDeviceMemoryCommitment m_vkGetDeviceMemoryCommitment;
    PFN_vkBindBufferMemory m_vkBindBufferMemory;
    PFN_vkBindImageMemory m_vkBindImageMemory;
    PFN_vkGetBufferMemoryRequirements m_vkGetBufferMemoryRequirements;
    PFN_vkGetImageMemoryRequirements m_vkGetImageMemoryRequirements;
    PFN_vkGetImageSparseMemoryRequirements m_vkGetImageSparseMemoryRequirements;
    PFN_vkGetPhysicalDeviceSparseImageFormatProperties m_vkGetPhysicalDeviceSparseImageFormatProperties;
    PFN_vkQueueBindSparse m_vkQueueBindSparse;
    PFN_vkCreateFence m_vkCreateFence;
    PFN_vkDestroyFence m_vkDestroyFence;
    PFN_vkResetFences m_vkResetFences;
    PFN_vkGetFenceStatus m_vkGetFenceStatus;
    PFN_vkWaitForFences m_vkWaitForFences;
    PFN_vkCreateSemaphore m_vkCreateSemaphore;
    PFN_vkDestroySemaphore m_vkDestroySemaphore;
    PFN_vkCreateEvent m_vkCreateEvent;
    PFN_vkDestroyEvent m_vkDestroyEvent;
    PFN_vkGetEventStatus m_vkGetEventStatus;
    PFN_vkSetEvent m_vkSetEvent;
    PFN_vkResetEvent m_vkResetEvent;
    PFN_vkCreateQueryPool m_vkCreateQueryPool;
    PFN_vkDestroyQueryPool m_vkDestroyQueryPool;
    PFN_vkGetQueryPoolResults m_vkGetQueryPoolResults;
    PFN_vkCreateBuffer m_vkCreateBuffer;
    PFN_vkDestroyBuffer m_vkDestroyBuffer;
    PFN_vkCreateBufferView m_vkCreateBufferView;
    PFN_vkDestroyBufferView m_vkDestroyBufferView;
    PFN_vkCreateImage m_vkCreateImage;
    PFN_vkDestroyImage m_vkDestroyImage;
    PFN_vkGetImageSubresourceLayout m_vkGetImageSubresourceLayout;
    PFN_vkCreateImageView m_vkCreateImageView;
    PFN_vkDestroyImageView m_vkDestroyImageView;
    PFN_vkCreateShaderModule m_vkCreateShaderModule;
    PFN_vkDestroyShaderModule m_vkDestroyShaderModule;
    PFN_vkCreatePipelineCache m_vkCreatePipelineCache;
    PFN_vkDestroyPipelineCache m_vkDestroyPipelineCache;
    PFN_vkGetPipelineCacheData m_vkGetPipelineCacheData;
    PFN_vkMergePipelineCaches m_vkMergePipelineCaches;
    PFN_vkCreateGraphicsPipelines m_vkCreateGraphicsPipelines;
    PFN_vkCreateComputePipelines m_vkCreateComputePipelines;
    PFN_vkDestroyPipeline m_vkDestroyPipeline;
    PFN_vkCreatePipelineLayout m_vkCreatePipelineLayout;
    PFN_vkDestroyPipelineLayout m_vkDestroyPipelineLayout;
    PFN_vkCreateSampler m_vkCreateSampler;
    PFN_vkDestroySampler m_vkDestroySampler;
    PFN_vkCreateDescriptorSetLayout m_vkCreateDescriptorSetLayout;
    PFN_vkDestroyDescriptorSetLayout m_vkDestroyDescriptorSetLayout;
    PFN_vkCreateDescriptorPool m_vkCreateDescriptorPool;
    PFN_vkDestroyDescriptorPool m_vkDestroyDescriptorPool;
    PFN_vkResetDescriptorPool m_vkResetDescriptorPool;
    PFN_vkAllocateDescriptorSets m_vkAllocateDescriptorSets;
    PFN_vkFreeDescriptorSets m_vkFreeDescriptorSets;
    PFN_vkUpdateDescriptorSets m_vkUpdateDescriptorSets;
    PFN_vkCreateFramebuffer m_vkCreateFramebuffer;
    PFN_vkDestroyFramebuffer m_vkDestroyFramebuffer;
    PFN_vkCreateRenderPass m_vkCreateRenderPass;
    PFN_vkDestroyRenderPass m_vkDestroyRenderPass;
    PFN_vkGetRenderAreaGranularity m_vkGetRenderAreaGranularity;
    PFN_vkCreateCommandPool m_vkCreateCommandPool;
    PFN_vkDestroyCommandPool m_vkDestroyCommandPool;
    PFN_vkResetCommandPool m_vkResetCommandPool;
    PFN_vkAllocateCommandBuffers m_vkAllocateCommandBuffers;
    PFN_vkFreeCommandBuffers m_vkFreeCommandBuffers;
    PFN_vkBeginCommandBuffer m_vkBeginCommandBuffer;
    PFN_vkEndCommandBuffer m_vkEndCommandBuffer;
    PFN_vkResetCommandBuffer m_vkResetCommandBuffer;
    PFN_vkCmdBindPipeline m_vkCmdBindPipeline;
    PFN_vkCmdSetViewport m_vkCmdSetViewport;
    PFN_vkCmdSetScissor m_vkCmdSetScissor;
    PFN_vkCmdSetLineWidth m_vkCmdSetLineWidth;
    PFN_vkCmdSetDepthBias m_vkCmdSetDepthBias;
    PFN_vkCmdSetBlendConstants m_vkCmdSetBlendConstants;
    PFN_vkCmdSetDepthBounds m_vkCmdSetDepthBounds;
    PFN_vkCmdSetStencilCompareMask m_vkCmdSetStencilCompareMask;
    PFN_vkCmdSetStencilWriteMask m_vkCmdSetStencilWriteMask;
    PFN_vkCmdSetStencilReference m_vkCmdSetStencilReference;
    PFN_vkCmdBindDescriptorSets m_vkCmdBindDescriptorSets;
    PFN_vkCmdBindIndexBuffer m_vkCmdBindIndexBuffer;
    PFN_vkCmdBindVertexBuffers m_vkCmdBindVertexBuffers;
    PFN_vkCmdDraw m_vkCmdDraw;
    PFN_vkCmdDrawIndexed m_vkCmdDrawIndexed;
    PFN_vkCmdDrawIndirect m_vkCmdDrawIndirect;
    PFN_vkCmdDrawIndexedIndirect m_vkCmdDrawIndexedIndirect;
    PFN_vkCmdDispatch m_vkCmdDispatch;
    PFN_vkCmdDispatchIndirect m_vkCmdDispatchIndirect;
    PFN_vkCmdCopyBuffer m_vkCmdCopyBuffer;
    PFN_vkCmdCopyImage m_vkCmdCopyImage;
    PFN_vkCmdBlitImage m_vkCmdBlitImage;
    PFN_vkCmdCopyBufferToImage m_vkCmdCopyBufferToImage;
    PFN_vkCmdCopyImageToBuffer m_vkCmdCopyImageToBuffer;
    PFN_vkCmdUpdateBuffer m_vkCmdUpdateBuffer;
    PFN_vkCmdFillBuffer m_vkCmdFillBuffer;
    PFN_vkCmdClearColorImage m_vkCmdClearColorImage;
    PFN_vkCmdClearDepthStencilImage m_vkCmdClearDepthStencilImage;
    PFN_vkCmdClearAttachments m_vkCmdClearAttachments;
    PFN_vkCmdResolveImage m_vkCmdResolveImage;
    PFN_vkCmdSetEvent m_vkCmdSetEvent;
    PFN_vkCmdResetEvent m_vkCmdResetEvent;
    PFN_vkCmdWaitEvents m_vkCmdWaitEvents;
    PFN_vkCmdPipelineBarrier m_vkCmdPipelineBarrier;
    PFN_vkCmdBeginQuery m_vkCmdBeginQuery;
    PFN_vkCmdEndQuery m_vkCmdEndQuery;
    PFN_vkCmdResetQueryPool m_vkCmdResetQueryPool;
    PFN_vkCmdWriteTimestamp m_vkCmdWriteTimestamp;
    PFN_vkCmdCopyQueryPoolResults m_vkCmdCopyQueryPoolResults;
    PFN_vkCmdPushConstants m_vkCmdPushConstants;
    PFN_vkCmdBeginRenderPass m_vkCmdBeginRenderPass;
    PFN_vkCmdNextSubpass m_vkCmdNextSubpass;
    PFN_vkCmdEndRenderPass m_vkCmdEndRenderPass;
    PFN_vkCmdExecuteCommands m_vkCmdExecuteCommands;

// VK_KHR_surface
    PFN_vkDestroySurfaceKHR m_vkDestroySurfaceKHR;
    PFN_vkGetPhysicalDeviceSurfaceSupportKHR m_vkGetPhysicalDeviceSurfaceSupportKHR;
    PFN_vkGetPhysicalDeviceSurfaceCapabilitiesKHR m_vkGetPhysicalDeviceSurfaceCapabilitiesKHR;
    PFN_vkGetPhysicalDeviceSurfaceFormatsKHR m_vkGetPhysicalDeviceSurfaceFormatsKHR;
    PFN_vkGetPhysicalDeviceSurfacePresentModesKHR m_vkGetPhysicalDeviceSurfacePresentModesKHR;

// VK_KHR_swapchain
    PFN_vkCreateSwapchainKHR m_vkCreateSwapchainKHR;
    PFN_vkDestroySwapchainKHR m_vkDestroySwapchainKHR;
    PFN_vkGetSwapchainImagesKHR m_vkGetSwapchainImagesKHR;
    PFN_vkAcquireNextImageKHR m_vkAcquireNextImageKHR;
    PFN_vkQueuePresentKHR m_vkQueuePresentKHR;

// VK_KHR_display
    PFN_vkGetPhysicalDeviceDisplayPropertiesKHR m_vkGetPhysicalDeviceDisplayPropertiesKHR;
    PFN_vkGetPhysicalDeviceDisplayPlanePropertiesKHR m_vkGetPhysicalDeviceDisplayPlanePropertiesKHR;
    PFN_vkGetDisplayPlaneSupportedDisplaysKHR m_vkGetDisplayPlaneSupportedDisplaysKHR;
    PFN_vkGetDisplayModePropertiesKHR m_vkGetDisplayModePropertiesKHR;
    PFN_vkCreateDisplayModeKHR m_vkCreateDisplayModeKHR;
    PFN_vkGetDisplayPlaneCapabilitiesKHR m_vkGetDisplayPlaneCapabilitiesKHR;
    PFN_vkCreateDisplayPlaneSurfaceKHR m_vkCreateDisplayPlaneSurfaceKHR;

// VK_KHR_display_swapchain
    PFN_vkCreateSharedSwapchainsKHR m_vkCreateSharedSwapchainsKHR;

#ifdef VK_USE_PLATFORM_XLIB_KHR
// VK_KHR_xlib_surface
    PFN_vkCreateXlibSurfaceKHR m_vkCreateXlibSurfaceKHR;
    PFN_vkGetPhysicalDeviceXlibPresentationSupportKHR m_vkGetPhysicalDeviceXlibPresentationSupportKHR;
#endif

#ifdef VK_USE_PLATFORM_XCB_KHR
// VK_KHR_xcb_surface
    PFN_vkCreateXcbSurfaceKHR m_vkCreateXcbSurfaceKHR;
    PFN_vkGetPhysicalDeviceXcbPresentationSupportKHR m_vkGetPhysicalDeviceXcbPresentationSupportKHR;
#endif

#ifdef VK_USE_PLATFORM_WAYLAND_KHR
// VK_KHR_wayland_surface
    PFN_vkCreateWaylandSurfaceKHR m_vkCreateWaylandSurfaceKHR;
    PFN_vkGetPhysicalDeviceWaylandPresentationSupportKHR m_vkGetPhysicalDeviceWaylandPresentationSupportKHR;
#endif

#ifdef VK_USE_PLATFORM_MIR_KHR
// VK_KHR_mir_surface
    PFN_vkCreateMirSurfaceKHR m_vkCreateMirSurfaceKHR;
    PFN_vkGetPhysicalDeviceMirPresentationSupportKHR m_vkGetPhysicalDeviceMirPresentationSupportKHR;
#endif

#ifdef VK_USE_PLATFORM_ANDROID_KHR
// VK_KHR_android_surface
    PFN_vkCreateAndroidSurfaceKHR m_vkCreateAndroidSurfaceKHR;
#endif

#ifdef VK_USE_PLATFORM_WIN32_KHR
// VK_KHR_win32_surface
    PFN_vkCreateWin32SurfaceKHR m_vkCreateWin32SurfaceKHR;
    PFN_vkGetPhysicalDeviceWin32PresentationSupportKHR m_vkGetPhysicalDeviceWin32PresentationSupportKHR;
#endif

#ifdef USE_DEBUG_EXTENTIONS
// VK_EXT_debug_report
    PFN_vkCreateDebugReportCallbackEXT m_vkCreateDebugReportCallbackEXT;
    PFN_vkDestroyDebugReportCallbackEXT m_vkDestroyDebugReportCallbackEXT;
    PFN_vkDebugReportMessageEXT m_vkDebugReportMessageEXT;
#endif
};

SDENGINE_API VulkanFunc* GetVulkanFunc();

//*/

 /* Initialize the Vulkan function pointer variables declared in this header.
  * Returns false if vulkan is not available, non-zero if it is available.
  */
bool InitVulkanFunction();
