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

#include "LogManager.h"
#include "VulkanManager.h"

_____________SD_START_GRAPHICS_NAMESPACE_____________

void VulkanManager::CreateGraphicsSwapchain(GraphicsSwapchainIdentity &io_identity)
{
    //
    VkPhysicalDevice &phy_device   = reinterpret_cast<VkPhysicalDevice&>(io_identity.m_phy_device);
    VkDevice         &device       = reinterpret_cast<VkDevice&>(io_identity.m_device);
    VkSurfaceKHR     &surface      = reinterpret_cast<VkSurfaceKHR&>(io_identity.m_surface);
    VkSwapchainKHR   &swapchain    = reinterpret_cast<VkSwapchainKHR&>(io_identity.m_handle);

    phy_device = m_phy_device;
    device     = m_device;
    surface    = m_surface;

    if (m_surface_capabilities.currentExtent.width != 0 && m_surface_capabilities.currentExtent.height != 0) {
        io_identity.m_screen_size.SetResolution(m_surface_capabilities.currentExtent.width, m_surface_capabilities.currentExtent.height);
    }
    else {
        io_identity.m_screen_size.SetResolution(
            std::max(m_surface_capabilities.minImageExtent.width, std::min(m_surface_capabilities.maxImageExtent.width, m_surface_capabilities.minImageExtent.width)),
            std::max(m_surface_capabilities.minImageExtent.height, std::min(m_surface_capabilities.maxImageExtent.height, m_surface_capabilities.minImageExtent.height)));
    }

    uint32_t image_count = m_surface_capabilities.minImageCount + 1;
    if (image_count > m_surface_capabilities.maxImageCount) {
        image_count = m_surface_capabilities.maxImageCount;
    }

    //Write Create Info
    std::vector<uint32_t> queue_fam_ids = { m_queue_family.GetQueueFamilyID() };
    VkSwapchainCreateInfoKHR info = InitializeVkSwapchainCreateInfoKHR(
        m_surface,
        m_final_sur_format,
        image_count,
        io_identity.m_screen_size.GetWidth(),
        io_identity.m_screen_size.GetHeight(),
        1, queue_fam_ids, m_final_p_mode);

    VkResult result = CreateVkSwapchain(swapchain, device, info);
    if (result != VK_SUCCESS) {
        SDLOGE("Create VkSwapchain failure(%d)!!!", result);
        return;
    }
    else {
        std::vector<VkImage> images;
        result = CollectVkImageForVkSwapchain(images, device, swapchain);
        if (result != VK_SUCCESS) {
            SDLOGE("Get images from VkSwapchain failure(%d)!!!", result);
        }
        else {
            io_identity.m_swapchain_images.resize(images.size());
            for (uint32_t i = 0; i < images.size(); ++i) {
                io_identity.m_swapchain_images[i] = reinterpret_cast<CompHandle>(images[i]);
            }
            io_identity.SetValid();
        }
    }
}

void VulkanManager::DestroyGraphicsSwapchain(GraphicsSwapchainIdentity &io_identity)
{
    VkDevice         &device    = reinterpret_cast<VkDevice&>(io_identity.m_device);
    VkSurfaceKHR     &surface   = reinterpret_cast<VkSurfaceKHR&>(io_identity.m_surface);
    VkSwapchainKHR   &swapchain = reinterpret_cast<VkSwapchainKHR&>(io_identity.m_handle);
    DestroyVkSwapchain(swapchain, device);
    io_identity.SetInvalid();
    io_identity = GraphicsSwapchainIdentity();
}

void VulkanManager::GetReadyTextureOfSwapchain(const GraphicsSwapchainIdentity &i_identity, const GraphicsSemaphoreWeakReferenceObject &i_acq_sema, uint32_t &io_idx)
{
    VkResult         result = VK_SUCCESS;
    VkDevice         device = reinterpret_cast<VkDevice>(i_identity.m_device);
    VkSurfaceKHR     surface = reinterpret_cast<VkSurfaceKHR>(i_identity.m_surface);
    VkSwapchainKHR   swapchain = reinterpret_cast<VkSwapchainKHR>(i_identity.m_handle);

    const GraphicsSemaphoreIdentity &acq_sema_identity = SD_SREF(m_graphics_identity_getter).GetIdentity(i_acq_sema);
    VkSemaphore acq_sema = reinterpret_cast<VkSemaphore>(acq_sema_identity.m_handle);
    //1. Acquire Image Idx.
    result = GetCurrentVkSwapchainIdx(
        io_idx, device, swapchain, acq_sema);
    if (result != VK_SUCCESS) {
        SDLOGE("Get vulkan swapchain image idx failure(%d)", result);
        return;
    }
}

void VulkanManager::RenderTextureToSwapchain(
    const GraphicsSwapchainIdentity &i_identity, uint32_t i_idx,
    const GraphicsQueueWeakReferenceObject &i_queue,
    const CommandBufferWeakReferenceObject &i_cmd_buffer,
    const GraphicsSemaphoreWeakReferenceObject &i_present_sema,
    const TextureWeakReferenceObject &i_texture)
{
    
    VkResult result = VK_SUCCESS;
    VkDevice         device    = reinterpret_cast<VkDevice>(i_identity.m_device);
    VkSurfaceKHR     surface   = reinterpret_cast<VkSurfaceKHR>(i_identity.m_surface);
    VkSwapchainKHR   swapchain = reinterpret_cast<VkSwapchainKHR>(i_identity.m_handle);

    const CommandBufferIdentity &cmd_identity = SD_WREF(m_graphics_identity_getter).GetIdentity(i_cmd_buffer);
    VkCommandBuffer cmd_buffer = reinterpret_cast<VkCommandBuffer>(cmd_identity.m_handle);

    const TextureIdentity &tex_identity = SD_WREF(m_graphics_identity_getter).GetIdentity(i_texture);
    VkImage src_image = reinterpret_cast<VkImage>(tex_identity.m_handle);

    //1. Record Image Copy to Swapchain Image.
    BeginCommandBuffer(cmd_identity, CommandBufferInheritanceInfo());

    VkImageBlit blit_param = {};
    blit_param.srcSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    blit_param.srcSubresource.baseArrayLayer = 0;
    blit_param.srcSubresource.mipLevel = 0;
    blit_param.srcSubresource.layerCount = tex_identity.m_array_layers;
    blit_param.srcOffsets[0].x = 0;
    blit_param.srcOffsets[0].y = 0;
    blit_param.srcOffsets[0].z = 0;
    blit_param.srcOffsets[1].x = i_identity.m_screen_size.GetWidth();
    blit_param.srcOffsets[1].y = i_identity.m_screen_size.GetHeight();
    blit_param.srcOffsets[1].z = 1;
    blit_param.dstSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    blit_param.dstSubresource.baseArrayLayer = 0;
    blit_param.dstSubresource.mipLevel = 0;
    blit_param.dstSubresource.layerCount = i_identity.m_layer_size;
    blit_param.dstOffsets[0].x = 0;
    blit_param.dstOffsets[0].y = 0;
    blit_param.dstOffsets[0].z = 0;
    blit_param.dstOffsets[1].x = i_identity.m_screen_size.GetWidth();
    blit_param.dstOffsets[1].y = i_identity.m_screen_size.GetHeight();
    blit_param.dstOffsets[1].z = 1;

    VkImage dst_image = reinterpret_cast<VkImage>(i_identity.m_swapchain_images[i_idx]);

    BlitVkImages(cmd_buffer, src_image, dst_image, blit_param);

    EndCommandBuffer(cmd_identity);
    //3. submit to present queue.
    SD_SREF(i_queue).SubmitCommandBuffers({i_cmd_buffer});

    //4. present queue.
    SD_SREF(i_queue).Present(i_identity, i_idx, {i_present_sema});
}

void VulkanManager::RenderTextureToScreen(const TextureWeakReferenceObject &i_tex)
{
    if (m_swapchain.IsNull() == false) {
        if (i_tex.IsNull() == true) {
            return;
        }

        if (SD_WREF(i_tex).IsInitialized() == false) {
            return;
        }

        SD_SREF(m_swapchain).RenderTextureToSwapchain(i_tex);

        m_fps_counter.AddCount();
    }
}

______________SD_END_GRAPHICS_NAMESPACE______________