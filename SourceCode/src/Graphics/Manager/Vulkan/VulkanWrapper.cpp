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

#include "VulkanWrapper.h"

#if defined(_WIN32) || defined(_WIN64)
    #include <windows.h>
    #define LoadFunc(handle, name) GetProcAddress(reinterpret_cast<HMODULE>(handle), name)
#elif defined(__ANDROID__) || defined(__linux__)
    #include <dlfcn.h>
    #define LoadFunc(handle, name) dlsym(handle, name)
#endif

///*
VulkanFunc gVulkanFunc;

VulkanFunc* GetVulkanFunc()
{
    return &gVulkanFunc;
}

bool InitVulkanFunction()
{
    void* libvulkan = nullptr;
#if defined(_WIN32) || defined(_WIN64)
    libvulkan = LoadLibrary(TEXT("vulkan-1.dll"));
#elif defined(__ANDROID__) || defined(__linux__)
    libvulkan = dlopen("libvulkan.so", RTLD_NOW | RTLD_LOCAL);
#endif

    if (libvulkan == nullptr) {
        return false;
    }

    // Vulkan supported, set function addresses
    gVulkanFunc.m_vkCreateInstance = reinterpret_cast<PFN_vkCreateInstance>(LoadFunc(libvulkan, "vkCreateInstance"));
    gVulkanFunc.m_vkDestroyInstance = reinterpret_cast<PFN_vkDestroyInstance>(LoadFunc(libvulkan, "vkDestroyInstance"));
    gVulkanFunc.m_vkEnumeratePhysicalDevices = reinterpret_cast<PFN_vkEnumeratePhysicalDevices>(LoadFunc(libvulkan, "vkEnumeratePhysicalDevices"));
    gVulkanFunc.m_vkGetPhysicalDeviceFeatures = reinterpret_cast<PFN_vkGetPhysicalDeviceFeatures>(LoadFunc(libvulkan, "vkGetPhysicalDeviceFeatures"));
    gVulkanFunc.m_vkGetPhysicalDeviceFormatProperties = reinterpret_cast<PFN_vkGetPhysicalDeviceFormatProperties>(LoadFunc(libvulkan, "vkGetPhysicalDeviceFormatProperties"));
    gVulkanFunc.m_vkGetPhysicalDeviceImageFormatProperties = reinterpret_cast<PFN_vkGetPhysicalDeviceImageFormatProperties>(LoadFunc(libvulkan, "vkGetPhysicalDeviceImageFormatProperties"));
    gVulkanFunc.m_vkGetPhysicalDeviceProperties = reinterpret_cast<PFN_vkGetPhysicalDeviceProperties>(LoadFunc(libvulkan, "vkGetPhysicalDeviceProperties"));
    gVulkanFunc.m_vkGetPhysicalDeviceQueueFamilyProperties = reinterpret_cast<PFN_vkGetPhysicalDeviceQueueFamilyProperties>(LoadFunc(libvulkan, "vkGetPhysicalDeviceQueueFamilyProperties"));
    gVulkanFunc.m_vkGetPhysicalDeviceMemoryProperties = reinterpret_cast<PFN_vkGetPhysicalDeviceMemoryProperties>(LoadFunc(libvulkan, "vkGetPhysicalDeviceMemoryProperties"));
    gVulkanFunc.m_vkGetInstanceProcAddr = reinterpret_cast<PFN_vkGetInstanceProcAddr>(LoadFunc(libvulkan, "vkGetInstanceProcAddr"));
    gVulkanFunc.m_vkGetDeviceProcAddr = reinterpret_cast<PFN_vkGetDeviceProcAddr>(LoadFunc(libvulkan, "vkGetDeviceProcAddr"));
    gVulkanFunc.m_vkCreateDevice = reinterpret_cast<PFN_vkCreateDevice>(LoadFunc(libvulkan, "vkCreateDevice"));
    gVulkanFunc.m_vkDestroyDevice = reinterpret_cast<PFN_vkDestroyDevice>(LoadFunc(libvulkan, "vkDestroyDevice"));
    gVulkanFunc.m_vkEnumerateInstanceExtensionProperties = reinterpret_cast<PFN_vkEnumerateInstanceExtensionProperties>(LoadFunc(libvulkan, "vkEnumerateInstanceExtensionProperties"));
    gVulkanFunc.m_vkEnumerateDeviceExtensionProperties = reinterpret_cast<PFN_vkEnumerateDeviceExtensionProperties>(LoadFunc(libvulkan, "vkEnumerateDeviceExtensionProperties"));
    gVulkanFunc.m_vkEnumerateInstanceLayerProperties = reinterpret_cast<PFN_vkEnumerateInstanceLayerProperties>(LoadFunc(libvulkan, "vkEnumerateInstanceLayerProperties"));
    gVulkanFunc.m_vkEnumerateDeviceLayerProperties = reinterpret_cast<PFN_vkEnumerateDeviceLayerProperties>(LoadFunc(libvulkan, "vkEnumerateDeviceLayerProperties"));
    gVulkanFunc.m_vkGetDeviceQueue = reinterpret_cast<PFN_vkGetDeviceQueue>(LoadFunc(libvulkan, "vkGetDeviceQueue"));
    gVulkanFunc.m_vkQueueSubmit = reinterpret_cast<PFN_vkQueueSubmit>(LoadFunc(libvulkan, "vkQueueSubmit"));
    gVulkanFunc.m_vkQueueWaitIdle = reinterpret_cast<PFN_vkQueueWaitIdle>(LoadFunc(libvulkan, "vkQueueWaitIdle"));
    gVulkanFunc.m_vkDeviceWaitIdle = reinterpret_cast<PFN_vkDeviceWaitIdle>(LoadFunc(libvulkan, "vkDeviceWaitIdle"));
    gVulkanFunc.m_vkAllocateMemory = reinterpret_cast<PFN_vkAllocateMemory>(LoadFunc(libvulkan, "vkAllocateMemory"));
    gVulkanFunc.m_vkFreeMemory = reinterpret_cast<PFN_vkFreeMemory>(LoadFunc(libvulkan, "vkFreeMemory"));
    gVulkanFunc.m_vkMapMemory = reinterpret_cast<PFN_vkMapMemory>(LoadFunc(libvulkan, "vkMapMemory"));
    gVulkanFunc.m_vkUnmapMemory = reinterpret_cast<PFN_vkUnmapMemory>(LoadFunc(libvulkan, "vkUnmapMemory"));
    gVulkanFunc.m_vkFlushMappedMemoryRanges = reinterpret_cast<PFN_vkFlushMappedMemoryRanges>(LoadFunc(libvulkan, "vkFlushMappedMemoryRanges"));
    gVulkanFunc.m_vkInvalidateMappedMemoryRanges = reinterpret_cast<PFN_vkInvalidateMappedMemoryRanges>(LoadFunc(libvulkan, "vkInvalidateMappedMemoryRanges"));
    gVulkanFunc.m_vkGetDeviceMemoryCommitment = reinterpret_cast<PFN_vkGetDeviceMemoryCommitment>(LoadFunc(libvulkan, "vkGetDeviceMemoryCommitment"));
    gVulkanFunc.m_vkBindBufferMemory = reinterpret_cast<PFN_vkBindBufferMemory>(LoadFunc(libvulkan, "vkBindBufferMemory"));
    gVulkanFunc.m_vkBindImageMemory = reinterpret_cast<PFN_vkBindImageMemory>(LoadFunc(libvulkan, "vkBindImageMemory"));
    gVulkanFunc.m_vkGetBufferMemoryRequirements = reinterpret_cast<PFN_vkGetBufferMemoryRequirements>(LoadFunc(libvulkan, "vkGetBufferMemoryRequirements"));
    gVulkanFunc.m_vkGetImageMemoryRequirements = reinterpret_cast<PFN_vkGetImageMemoryRequirements>(LoadFunc(libvulkan, "vkGetImageMemoryRequirements"));
    gVulkanFunc.m_vkGetImageSparseMemoryRequirements = reinterpret_cast<PFN_vkGetImageSparseMemoryRequirements>(LoadFunc(libvulkan, "vkGetImageSparseMemoryRequirements"));
    gVulkanFunc.m_vkGetPhysicalDeviceSparseImageFormatProperties = reinterpret_cast<PFN_vkGetPhysicalDeviceSparseImageFormatProperties>(LoadFunc(libvulkan, "vkGetPhysicalDeviceSparseImageFormatProperties"));
    gVulkanFunc.m_vkQueueBindSparse = reinterpret_cast<PFN_vkQueueBindSparse>(LoadFunc(libvulkan, "vkQueueBindSparse"));
    gVulkanFunc.m_vkCreateFence = reinterpret_cast<PFN_vkCreateFence>(LoadFunc(libvulkan, "vkCreateFence"));
    gVulkanFunc.m_vkDestroyFence = reinterpret_cast<PFN_vkDestroyFence>(LoadFunc(libvulkan, "vkDestroyFence"));
    gVulkanFunc.m_vkResetFences = reinterpret_cast<PFN_vkResetFences>(LoadFunc(libvulkan, "vkResetFences"));
    gVulkanFunc.m_vkGetFenceStatus = reinterpret_cast<PFN_vkGetFenceStatus>(LoadFunc(libvulkan, "vkGetFenceStatus"));
    gVulkanFunc.m_vkWaitForFences = reinterpret_cast<PFN_vkWaitForFences>(LoadFunc(libvulkan, "vkWaitForFences"));
    gVulkanFunc.m_vkCreateSemaphore = reinterpret_cast<PFN_vkCreateSemaphore>(LoadFunc(libvulkan, "vkCreateSemaphore"));
    gVulkanFunc.m_vkDestroySemaphore = reinterpret_cast<PFN_vkDestroySemaphore>(LoadFunc(libvulkan, "vkDestroySemaphore"));
    gVulkanFunc.m_vkCreateEvent = reinterpret_cast<PFN_vkCreateEvent>(LoadFunc(libvulkan, "vkCreateEvent"));
    gVulkanFunc.m_vkDestroyEvent = reinterpret_cast<PFN_vkDestroyEvent>(LoadFunc(libvulkan, "vkDestroyEvent"));
    gVulkanFunc.m_vkGetEventStatus = reinterpret_cast<PFN_vkGetEventStatus>(LoadFunc(libvulkan, "vkGetEventStatus"));
    gVulkanFunc.m_vkSetEvent = reinterpret_cast<PFN_vkSetEvent>(LoadFunc(libvulkan, "vkSetEvent"));
    gVulkanFunc.m_vkResetEvent = reinterpret_cast<PFN_vkResetEvent>(LoadFunc(libvulkan, "vkResetEvent"));
    gVulkanFunc.m_vkCreateQueryPool = reinterpret_cast<PFN_vkCreateQueryPool>(LoadFunc(libvulkan, "vkCreateQueryPool"));
    gVulkanFunc.m_vkDestroyQueryPool = reinterpret_cast<PFN_vkDestroyQueryPool>(LoadFunc(libvulkan, "vkDestroyQueryPool"));
    gVulkanFunc.m_vkGetQueryPoolResults = reinterpret_cast<PFN_vkGetQueryPoolResults>(LoadFunc(libvulkan, "vkGetQueryPoolResults"));
    gVulkanFunc.m_vkCreateBuffer = reinterpret_cast<PFN_vkCreateBuffer>(LoadFunc(libvulkan, "vkCreateBuffer"));
    gVulkanFunc.m_vkDestroyBuffer = reinterpret_cast<PFN_vkDestroyBuffer>(LoadFunc(libvulkan, "vkDestroyBuffer"));
    gVulkanFunc.m_vkCreateBufferView = reinterpret_cast<PFN_vkCreateBufferView>(LoadFunc(libvulkan, "vkCreateBufferView"));
    gVulkanFunc.m_vkDestroyBufferView = reinterpret_cast<PFN_vkDestroyBufferView>(LoadFunc(libvulkan, "vkDestroyBufferView"));
    gVulkanFunc.m_vkCreateImage = reinterpret_cast<PFN_vkCreateImage>(LoadFunc(libvulkan, "vkCreateImage"));
    gVulkanFunc.m_vkDestroyImage = reinterpret_cast<PFN_vkDestroyImage>(LoadFunc(libvulkan, "vkDestroyImage"));
    gVulkanFunc.m_vkGetImageSubresourceLayout = reinterpret_cast<PFN_vkGetImageSubresourceLayout>(LoadFunc(libvulkan, "vkGetImageSubresourceLayout"));
    gVulkanFunc.m_vkCreateImageView = reinterpret_cast<PFN_vkCreateImageView>(LoadFunc(libvulkan, "vkCreateImageView"));
    gVulkanFunc.m_vkDestroyImageView = reinterpret_cast<PFN_vkDestroyImageView>(LoadFunc(libvulkan, "vkDestroyImageView"));
    gVulkanFunc.m_vkCreateShaderModule = reinterpret_cast<PFN_vkCreateShaderModule>(LoadFunc(libvulkan, "vkCreateShaderModule"));
    gVulkanFunc.m_vkDestroyShaderModule = reinterpret_cast<PFN_vkDestroyShaderModule>(LoadFunc(libvulkan, "vkDestroyShaderModule"));
    gVulkanFunc.m_vkCreatePipelineCache = reinterpret_cast<PFN_vkCreatePipelineCache>(LoadFunc(libvulkan, "vkCreatePipelineCache"));
    gVulkanFunc.m_vkDestroyPipelineCache = reinterpret_cast<PFN_vkDestroyPipelineCache>(LoadFunc(libvulkan, "vkDestroyPipelineCache"));
    gVulkanFunc.m_vkGetPipelineCacheData = reinterpret_cast<PFN_vkGetPipelineCacheData>(LoadFunc(libvulkan, "vkGetPipelineCacheData"));
    gVulkanFunc.m_vkMergePipelineCaches = reinterpret_cast<PFN_vkMergePipelineCaches>(LoadFunc(libvulkan, "vkMergePipelineCaches"));
    gVulkanFunc.m_vkCreateGraphicsPipelines = reinterpret_cast<PFN_vkCreateGraphicsPipelines>(LoadFunc(libvulkan, "vkCreateGraphicsPipelines"));
    gVulkanFunc.m_vkCreateComputePipelines = reinterpret_cast<PFN_vkCreateComputePipelines>(LoadFunc(libvulkan, "vkCreateComputePipelines"));
    gVulkanFunc.m_vkDestroyPipeline = reinterpret_cast<PFN_vkDestroyPipeline>(LoadFunc(libvulkan, "vkDestroyPipeline"));
    gVulkanFunc.m_vkCreatePipelineLayout = reinterpret_cast<PFN_vkCreatePipelineLayout>(LoadFunc(libvulkan, "vkCreatePipelineLayout"));
    gVulkanFunc.m_vkDestroyPipelineLayout = reinterpret_cast<PFN_vkDestroyPipelineLayout>(LoadFunc(libvulkan, "vkDestroyPipelineLayout"));
    gVulkanFunc.m_vkCreateSampler = reinterpret_cast<PFN_vkCreateSampler>(LoadFunc(libvulkan, "vkCreateSampler"));
    gVulkanFunc.m_vkDestroySampler = reinterpret_cast<PFN_vkDestroySampler>(LoadFunc(libvulkan, "vkDestroySampler"));
    gVulkanFunc.m_vkCreateDescriptorSetLayout = reinterpret_cast<PFN_vkCreateDescriptorSetLayout>(LoadFunc(libvulkan, "vkCreateDescriptorSetLayout"));
    gVulkanFunc.m_vkDestroyDescriptorSetLayout = reinterpret_cast<PFN_vkDestroyDescriptorSetLayout>(LoadFunc(libvulkan, "vkDestroyDescriptorSetLayout"));
    gVulkanFunc.m_vkCreateDescriptorPool = reinterpret_cast<PFN_vkCreateDescriptorPool>(LoadFunc(libvulkan, "vkCreateDescriptorPool"));
    gVulkanFunc.m_vkDestroyDescriptorPool = reinterpret_cast<PFN_vkDestroyDescriptorPool>(LoadFunc(libvulkan, "vkDestroyDescriptorPool"));
    gVulkanFunc.m_vkResetDescriptorPool = reinterpret_cast<PFN_vkResetDescriptorPool>(LoadFunc(libvulkan, "vkResetDescriptorPool"));
    gVulkanFunc.m_vkAllocateDescriptorSets = reinterpret_cast<PFN_vkAllocateDescriptorSets>(LoadFunc(libvulkan, "vkAllocateDescriptorSets"));
    gVulkanFunc.m_vkFreeDescriptorSets = reinterpret_cast<PFN_vkFreeDescriptorSets>(LoadFunc(libvulkan, "vkFreeDescriptorSets"));
    gVulkanFunc.m_vkUpdateDescriptorSets = reinterpret_cast<PFN_vkUpdateDescriptorSets>(LoadFunc(libvulkan, "vkUpdateDescriptorSets"));
    gVulkanFunc.m_vkCreateFramebuffer = reinterpret_cast<PFN_vkCreateFramebuffer>(LoadFunc(libvulkan, "vkCreateFramebuffer"));
    gVulkanFunc.m_vkDestroyFramebuffer = reinterpret_cast<PFN_vkDestroyFramebuffer>(LoadFunc(libvulkan, "vkDestroyFramebuffer"));
    gVulkanFunc.m_vkCreateRenderPass = reinterpret_cast<PFN_vkCreateRenderPass>(LoadFunc(libvulkan, "vkCreateRenderPass"));
    gVulkanFunc.m_vkDestroyRenderPass = reinterpret_cast<PFN_vkDestroyRenderPass>(LoadFunc(libvulkan, "vkDestroyRenderPass"));
    gVulkanFunc.m_vkGetRenderAreaGranularity = reinterpret_cast<PFN_vkGetRenderAreaGranularity>(LoadFunc(libvulkan, "vkGetRenderAreaGranularity"));
    gVulkanFunc.m_vkCreateCommandPool = reinterpret_cast<PFN_vkCreateCommandPool>(LoadFunc(libvulkan, "vkCreateCommandPool"));
    gVulkanFunc.m_vkDestroyCommandPool = reinterpret_cast<PFN_vkDestroyCommandPool>(LoadFunc(libvulkan, "vkDestroyCommandPool"));
    gVulkanFunc.m_vkResetCommandPool = reinterpret_cast<PFN_vkResetCommandPool>(LoadFunc(libvulkan, "vkResetCommandPool"));
    gVulkanFunc.m_vkAllocateCommandBuffers = reinterpret_cast<PFN_vkAllocateCommandBuffers>(LoadFunc(libvulkan, "vkAllocateCommandBuffers"));
    gVulkanFunc.m_vkFreeCommandBuffers = reinterpret_cast<PFN_vkFreeCommandBuffers>(LoadFunc(libvulkan, "vkFreeCommandBuffers"));
    gVulkanFunc.m_vkBeginCommandBuffer = reinterpret_cast<PFN_vkBeginCommandBuffer>(LoadFunc(libvulkan, "vkBeginCommandBuffer"));
    gVulkanFunc.m_vkEndCommandBuffer = reinterpret_cast<PFN_vkEndCommandBuffer>(LoadFunc(libvulkan, "vkEndCommandBuffer"));
    gVulkanFunc.m_vkResetCommandBuffer = reinterpret_cast<PFN_vkResetCommandBuffer>(LoadFunc(libvulkan, "vkResetCommandBuffer"));
    gVulkanFunc.m_vkCmdBindPipeline = reinterpret_cast<PFN_vkCmdBindPipeline>(LoadFunc(libvulkan, "vkCmdBindPipeline"));
    gVulkanFunc.m_vkCmdSetViewport = reinterpret_cast<PFN_vkCmdSetViewport>(LoadFunc(libvulkan, "vkCmdSetViewport"));
    gVulkanFunc.m_vkCmdSetScissor = reinterpret_cast<PFN_vkCmdSetScissor>(LoadFunc(libvulkan, "vkCmdSetScissor"));
    gVulkanFunc.m_vkCmdSetLineWidth = reinterpret_cast<PFN_vkCmdSetLineWidth>(LoadFunc(libvulkan, "vkCmdSetLineWidth"));
    gVulkanFunc.m_vkCmdSetDepthBias = reinterpret_cast<PFN_vkCmdSetDepthBias>(LoadFunc(libvulkan, "vkCmdSetDepthBias"));
    gVulkanFunc.m_vkCmdSetBlendConstants = reinterpret_cast<PFN_vkCmdSetBlendConstants>(LoadFunc(libvulkan, "vkCmdSetBlendConstants"));
    gVulkanFunc.m_vkCmdSetDepthBounds = reinterpret_cast<PFN_vkCmdSetDepthBounds>(LoadFunc(libvulkan, "vkCmdSetDepthBounds"));
    gVulkanFunc.m_vkCmdSetStencilCompareMask = reinterpret_cast<PFN_vkCmdSetStencilCompareMask>(LoadFunc(libvulkan, "vkCmdSetStencilCompareMask"));
    gVulkanFunc.m_vkCmdSetStencilWriteMask = reinterpret_cast<PFN_vkCmdSetStencilWriteMask>(LoadFunc(libvulkan, "vkCmdSetStencilWriteMask"));
    gVulkanFunc.m_vkCmdSetStencilReference = reinterpret_cast<PFN_vkCmdSetStencilReference>(LoadFunc(libvulkan, "vkCmdSetStencilReference"));
    gVulkanFunc.m_vkCmdBindDescriptorSets = reinterpret_cast<PFN_vkCmdBindDescriptorSets>(LoadFunc(libvulkan, "vkCmdBindDescriptorSets"));
    gVulkanFunc.m_vkCmdBindIndexBuffer = reinterpret_cast<PFN_vkCmdBindIndexBuffer>(LoadFunc(libvulkan, "vkCmdBindIndexBuffer"));
    gVulkanFunc.m_vkCmdBindVertexBuffers = reinterpret_cast<PFN_vkCmdBindVertexBuffers>(LoadFunc(libvulkan, "vkCmdBindVertexBuffers"));
    gVulkanFunc.m_vkCmdDraw = reinterpret_cast<PFN_vkCmdDraw>(LoadFunc(libvulkan, "vkCmdDraw"));
    gVulkanFunc.m_vkCmdDrawIndexed = reinterpret_cast<PFN_vkCmdDrawIndexed>(LoadFunc(libvulkan, "vkCmdDrawIndexed"));
    gVulkanFunc.m_vkCmdDrawIndirect = reinterpret_cast<PFN_vkCmdDrawIndirect>(LoadFunc(libvulkan, "vkCmdDrawIndirect"));
    gVulkanFunc.m_vkCmdDrawIndexedIndirect = reinterpret_cast<PFN_vkCmdDrawIndexedIndirect>(LoadFunc(libvulkan, "vkCmdDrawIndexedIndirect"));
    gVulkanFunc.m_vkCmdDispatch = reinterpret_cast<PFN_vkCmdDispatch>(LoadFunc(libvulkan, "vkCmdDispatch"));
    gVulkanFunc.m_vkCmdDispatchIndirect = reinterpret_cast<PFN_vkCmdDispatchIndirect>(LoadFunc(libvulkan, "vkCmdDispatchIndirect"));
    gVulkanFunc.m_vkCmdCopyBuffer = reinterpret_cast<PFN_vkCmdCopyBuffer>(LoadFunc(libvulkan, "vkCmdCopyBuffer"));
    gVulkanFunc.m_vkCmdCopyImage = reinterpret_cast<PFN_vkCmdCopyImage>(LoadFunc(libvulkan, "vkCmdCopyImage"));
    gVulkanFunc.m_vkCmdBlitImage = reinterpret_cast<PFN_vkCmdBlitImage>(LoadFunc(libvulkan, "vkCmdBlitImage"));
    gVulkanFunc.m_vkCmdCopyBufferToImage = reinterpret_cast<PFN_vkCmdCopyBufferToImage>(LoadFunc(libvulkan, "vkCmdCopyBufferToImage"));
    gVulkanFunc.m_vkCmdCopyImageToBuffer = reinterpret_cast<PFN_vkCmdCopyImageToBuffer>(LoadFunc(libvulkan, "vkCmdCopyImageToBuffer"));
    gVulkanFunc.m_vkCmdUpdateBuffer = reinterpret_cast<PFN_vkCmdUpdateBuffer>(LoadFunc(libvulkan, "vkCmdUpdateBuffer"));
    gVulkanFunc.m_vkCmdFillBuffer = reinterpret_cast<PFN_vkCmdFillBuffer>(LoadFunc(libvulkan, "vkCmdFillBuffer"));
    gVulkanFunc.m_vkCmdClearColorImage = reinterpret_cast<PFN_vkCmdClearColorImage>(LoadFunc(libvulkan, "vkCmdClearColorImage"));
    gVulkanFunc.m_vkCmdClearDepthStencilImage = reinterpret_cast<PFN_vkCmdClearDepthStencilImage>(LoadFunc(libvulkan, "vkCmdClearDepthStencilImage"));
    gVulkanFunc.m_vkCmdClearAttachments = reinterpret_cast<PFN_vkCmdClearAttachments>(LoadFunc(libvulkan, "vkCmdClearAttachments"));
    gVulkanFunc.m_vkCmdResolveImage = reinterpret_cast<PFN_vkCmdResolveImage>(LoadFunc(libvulkan, "vkCmdResolveImage"));
    gVulkanFunc.m_vkCmdSetEvent = reinterpret_cast<PFN_vkCmdSetEvent>(LoadFunc(libvulkan, "vkCmdSetEvent"));
    gVulkanFunc.m_vkCmdResetEvent = reinterpret_cast<PFN_vkCmdResetEvent>(LoadFunc(libvulkan, "vkCmdResetEvent"));
    gVulkanFunc.m_vkCmdWaitEvents = reinterpret_cast<PFN_vkCmdWaitEvents>(LoadFunc(libvulkan, "vkCmdWaitEvents"));
    gVulkanFunc.m_vkCmdPipelineBarrier = reinterpret_cast<PFN_vkCmdPipelineBarrier>(LoadFunc(libvulkan, "vkCmdPipelineBarrier"));
    gVulkanFunc.m_vkCmdBeginQuery = reinterpret_cast<PFN_vkCmdBeginQuery>(LoadFunc(libvulkan, "vkCmdBeginQuery"));
    gVulkanFunc.m_vkCmdEndQuery = reinterpret_cast<PFN_vkCmdEndQuery>(LoadFunc(libvulkan, "vkCmdEndQuery"));
    gVulkanFunc.m_vkCmdResetQueryPool = reinterpret_cast<PFN_vkCmdResetQueryPool>(LoadFunc(libvulkan, "vkCmdResetQueryPool"));
    gVulkanFunc.m_vkCmdWriteTimestamp = reinterpret_cast<PFN_vkCmdWriteTimestamp>(LoadFunc(libvulkan, "vkCmdWriteTimestamp"));
    gVulkanFunc.m_vkCmdCopyQueryPoolResults = reinterpret_cast<PFN_vkCmdCopyQueryPoolResults>(LoadFunc(libvulkan, "vkCmdCopyQueryPoolResults"));
    gVulkanFunc.m_vkCmdPushConstants = reinterpret_cast<PFN_vkCmdPushConstants>(LoadFunc(libvulkan, "vkCmdPushConstants"));
    gVulkanFunc.m_vkCmdBeginRenderPass = reinterpret_cast<PFN_vkCmdBeginRenderPass>(LoadFunc(libvulkan, "vkCmdBeginRenderPass"));
    gVulkanFunc.m_vkCmdNextSubpass = reinterpret_cast<PFN_vkCmdNextSubpass>(LoadFunc(libvulkan, "vkCmdNextSubpass"));
    gVulkanFunc.m_vkCmdEndRenderPass = reinterpret_cast<PFN_vkCmdEndRenderPass>(LoadFunc(libvulkan, "vkCmdEndRenderPass"));
    gVulkanFunc.m_vkCmdExecuteCommands = reinterpret_cast<PFN_vkCmdExecuteCommands>(LoadFunc(libvulkan, "vkCmdExecuteCommands"));
    gVulkanFunc.m_vkDestroySurfaceKHR = reinterpret_cast<PFN_vkDestroySurfaceKHR>(LoadFunc(libvulkan, "vkDestroySurfaceKHR"));
    gVulkanFunc.m_vkGetPhysicalDeviceSurfaceSupportKHR = reinterpret_cast<PFN_vkGetPhysicalDeviceSurfaceSupportKHR>(LoadFunc(libvulkan, "vkGetPhysicalDeviceSurfaceSupportKHR"));
    gVulkanFunc.m_vkGetPhysicalDeviceSurfaceCapabilitiesKHR = reinterpret_cast<PFN_vkGetPhysicalDeviceSurfaceCapabilitiesKHR>(LoadFunc(libvulkan, "vkGetPhysicalDeviceSurfaceCapabilitiesKHR"));
    gVulkanFunc.m_vkGetPhysicalDeviceSurfaceFormatsKHR = reinterpret_cast<PFN_vkGetPhysicalDeviceSurfaceFormatsKHR>(LoadFunc(libvulkan, "vkGetPhysicalDeviceSurfaceFormatsKHR"));
    gVulkanFunc.m_vkGetPhysicalDeviceSurfacePresentModesKHR = reinterpret_cast<PFN_vkGetPhysicalDeviceSurfacePresentModesKHR>(LoadFunc(libvulkan, "vkGetPhysicalDeviceSurfacePresentModesKHR"));
    gVulkanFunc.m_vkCreateSwapchainKHR = reinterpret_cast<PFN_vkCreateSwapchainKHR>(LoadFunc(libvulkan, "vkCreateSwapchainKHR"));
    gVulkanFunc.m_vkDestroySwapchainKHR = reinterpret_cast<PFN_vkDestroySwapchainKHR>(LoadFunc(libvulkan, "vkDestroySwapchainKHR"));
    gVulkanFunc.m_vkGetSwapchainImagesKHR = reinterpret_cast<PFN_vkGetSwapchainImagesKHR>(LoadFunc(libvulkan, "vkGetSwapchainImagesKHR"));
    gVulkanFunc.m_vkAcquireNextImageKHR = reinterpret_cast<PFN_vkAcquireNextImageKHR>(LoadFunc(libvulkan, "vkAcquireNextImageKHR"));
    gVulkanFunc.m_vkQueuePresentKHR = reinterpret_cast<PFN_vkQueuePresentKHR>(LoadFunc(libvulkan, "vkQueuePresentKHR"));
    gVulkanFunc.m_vkGetPhysicalDeviceDisplayPropertiesKHR = reinterpret_cast<PFN_vkGetPhysicalDeviceDisplayPropertiesKHR>(LoadFunc(libvulkan, "vkGetPhysicalDeviceDisplayPropertiesKHR"));
    gVulkanFunc.m_vkGetPhysicalDeviceDisplayPlanePropertiesKHR = reinterpret_cast<PFN_vkGetPhysicalDeviceDisplayPlanePropertiesKHR>(LoadFunc(libvulkan, "vkGetPhysicalDeviceDisplayPlanePropertiesKHR"));
    gVulkanFunc.m_vkGetDisplayPlaneSupportedDisplaysKHR = reinterpret_cast<PFN_vkGetDisplayPlaneSupportedDisplaysKHR>(LoadFunc(libvulkan, "vkGetDisplayPlaneSupportedDisplaysKHR"));
    gVulkanFunc.m_vkGetDisplayModePropertiesKHR = reinterpret_cast<PFN_vkGetDisplayModePropertiesKHR>(LoadFunc(libvulkan, "vkGetDisplayModePropertiesKHR"));
    gVulkanFunc.m_vkCreateDisplayModeKHR = reinterpret_cast<PFN_vkCreateDisplayModeKHR>(LoadFunc(libvulkan, "vkCreateDisplayModeKHR"));
    gVulkanFunc.m_vkGetDisplayPlaneCapabilitiesKHR = reinterpret_cast<PFN_vkGetDisplayPlaneCapabilitiesKHR>(LoadFunc(libvulkan, "vkGetDisplayPlaneCapabilitiesKHR"));
    gVulkanFunc.m_vkCreateDisplayPlaneSurfaceKHR = reinterpret_cast<PFN_vkCreateDisplayPlaneSurfaceKHR>(LoadFunc(libvulkan, "vkCreateDisplayPlaneSurfaceKHR"));
    gVulkanFunc.m_vkCreateSharedSwapchainsKHR = reinterpret_cast<PFN_vkCreateSharedSwapchainsKHR>(LoadFunc(libvulkan, "vkCreateSharedSwapchainsKHR"));

#ifdef VK_USE_PLATFORM_XLIB_KHR
    gVulkanFunc.m_vkCreateXlibSurfaceKHR = reinterpret_cast<PFN_vkCreateXlibSurfaceKHR>(LoadFunc(libvulkan, "vkCreateXlibSurfaceKHR"));
    gVulkanFunc.m_vkGetPhysicalDeviceXlibPresentationSupportKHR = reinterpret_cast<PFN_vkGetPhysicalDeviceXlibPresentationSupportKHR>(LoadFunc(libvulkan, "vkGetPhysicalDeviceXlibPresentationSupportKHR"));
#endif

#ifdef VK_USE_PLATFORM_XCB_KHR
    gVulkanFunc.m_vkCreateXcbSurfaceKHR = reinterpret_cast<PFN_vkCreateXcbSurfaceKHR>(LoadFunc(libvulkan, "vkCreateXcbSurfaceKHR"));
    gVulkanFunc.m_vkGetPhysicalDeviceXcbPresentationSupportKHR = reinterpret_cast<PFN_vkGetPhysicalDeviceXcbPresentationSupportKHR>(LoadFunc(libvulkan, "vkGetPhysicalDeviceXcbPresentationSupportKHR"));
#endif

#ifdef VK_USE_PLATFORM_WAYLAND_KHR
    gVulkanFunc.m_vkCreateWaylandSurfaceKHR = reinterpret_cast<PFN_vkCreateWaylandSurfaceKHR>(LoadFunc(libvulkan, "vkCreateWaylandSurfaceKHR"));
    gVulkanFunc.m_vkGetPhysicalDeviceWaylandPresentationSupportKHR = reinterpret_cast<PFN_vkGetPhysicalDeviceWaylandPresentationSupportKHR>(LoadFunc(libvulkan, "vkGetPhysicalDeviceWaylandPresentationSupportKHR"));
#endif

#ifdef VK_USE_PLATFORM_MIR_KHR
    gVulkanFunc.m_vkCreateMirSurfaceKHR = reinterpret_cast<PFN_vkCreateMirSurfaceKHR>(LoadFunc(libvulkan, "vkCreateMirSurfaceKHR"));
    gVulkanFunc.m_vkGetPhysicalDeviceMirPresentationSupportKHR = reinterpret_cast<PFN_vkGetPhysicalDeviceMirPresentationSupportKHR>(LoadFunc(libvulkan, "vkGetPhysicalDeviceMirPresentationSupportKHR"));
#endif

#ifdef VK_USE_PLATFORM_ANDROID_KHR
    gVulkanFunc.m_vkCreateAndroidSurfaceKHR = reinterpret_cast<PFN_vkCreateAndroidSurfaceKHR>(LoadFunc(libvulkan, "vkCreateAndroidSurfaceKHR"));
#endif

#ifdef VK_USE_PLATFORM_WIN32_KHR
    gVulkanFunc.m_vkCreateWin32SurfaceKHR = reinterpret_cast<PFN_vkCreateWin32SurfaceKHR>(LoadFunc(libvulkan, "vkCreateWin32SurfaceKHR"));
    gVulkanFunc.m_vkGetPhysicalDeviceWin32PresentationSupportKHR = reinterpret_cast<PFN_vkGetPhysicalDeviceWin32PresentationSupportKHR>(LoadFunc(libvulkan, "vkGetPhysicalDeviceWin32PresentationSupportKHR"));
#endif
#ifdef USE_DEBUG_EXTENTIONS
    gVulkanFunc.m_vkCreateDebugReportCallbackEXT = reinterpret_cast<PFN_vkCreateDebugReportCallbackEXT>(LoadFunc(libvulkan, "vkCreateDebugReportCallbackEXT"));
    gVulkanFunc.m_vkDestroyDebugReportCallbackEXT = reinterpret_cast<PFN_vkDestroyDebugReportCallbackEXT>(LoadFunc(libvulkan, "vkDestroyDebugReportCallbackEXT"));
    gVulkanFunc.m_vkDebugReportMessageEXT = reinterpret_cast<PFN_vkDebugReportMessageEXT>(LoadFunc(libvulkan, "vkDebugReportMessageEXT"));
#endif
    return true;
}
//*/
