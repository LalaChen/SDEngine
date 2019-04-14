#pragma once

#include "Sample1_DrawTriangle.h"
#include "VulkanManager.h"

class VulkanAPITestManager : public SDE::Graphics::VulkanManager
{
public:
    explicit VulkanAPITestManager();
    virtual ~VulkanAPITestManager();
public:
    void InitializeGraphicsSystem(const EventArg &i_arg) override;
    void ReleaseGraphicsSystem() override;
public: //Buffer Related.
    VkResult CreateBuffer(VkBufferUsageFlags i_buffer_usage, VkSharingMode i_sharing_mode, VkDeviceSize i_size, VkBuffer &io_VK_buffer);
    VkResult AllocateMemoryAndBindToBuffer(VkMemoryPropertyFlagBits i_memo_prop_flags, VkDeviceSize i_VK_offset, VkBuffer i_VK_buffer, VkDeviceMemory &io_VK_memory);
    VkResult RefreshLocalDeviceBufferData(VkBuffer i_VK_buffer, void *i_data_ptr, VkDeviceSize i_size);
    void ReleaseMemory(VkDeviceMemory i_VK_memory);
    void DestroyBuffer(VkBuffer i_VK_buffer);
public: //Shader Related.
    VkResult CreateDescriptorSetLayout(std::vector<VkDescriptorSetLayoutCreateInfo> &i_c_infos, std::vector<VkDescriptorSetLayout> &io_layouts);
    void DestroyDesciptorSetLayout(VkDescriptorSetLayout i_VK_layout);
public:
    void RenderToScreen() override;
    void RenderDebug();
protected:
    Sample1_DrawTriangle m_draw_triangle;
};