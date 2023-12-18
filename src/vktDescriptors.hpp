#pragma once
 
#include "vktEngineDevice.hpp"
 
// std
#include <memory>
#include <unordered_map>
#include <vector>
 
namespace vkt {
 
class vktDescriptorSetLayout {
 public:
  class Builder {
   public:
    Builder(vktEngineDevice &device) : vktDevice{device} {}
 
    Builder &addBinding(
        uint32_t binding,
        VkDescriptorType descriptorType,
        VkShaderStageFlags stageFlags,
        uint32_t count = 1);
    std::unique_ptr<vktDescriptorSetLayout> build() const;
 
   private:
    vktEngineDevice &vktDevice;
    std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> bindings{};
  };
 
  vktDescriptorSetLayout(
      vktEngineDevice &device, std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> bindings);
  ~vktDescriptorSetLayout();
  vktDescriptorSetLayout(const vktDescriptorSetLayout &) = delete;
  vktDescriptorSetLayout &operator=(const vktDescriptorSetLayout &) = delete;
 
  VkDescriptorSetLayout getDescriptorSetLayout() const { return descriptorSetLayout; }
 
 private:
  vktEngineDevice &vktDevice;
  VkDescriptorSetLayout descriptorSetLayout;
  std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> bindings;
 
  friend class vktDescriptorWriter;
};

class vktDescriptorPool {
 public:
  class Builder {
   public:
    Builder(vktEngineDevice &device) : vktDevice{device} {}
 
    Builder &addPoolSize(VkDescriptorType descriptorType, uint32_t count);
    Builder &setPoolFlags(VkDescriptorPoolCreateFlags flags);
    Builder &setMaxSets(uint32_t count);
    std::unique_ptr<vktDescriptorPool> build() const;
 
   private:
    vktEngineDevice &vktDevice;
    std::vector<VkDescriptorPoolSize> poolSizes{};
    uint32_t maxSets = 1000;
    VkDescriptorPoolCreateFlags poolFlags = 0;
  };
 
  vktDescriptorPool(
      vktEngineDevice &device,
      uint32_t maxSets,
      VkDescriptorPoolCreateFlags poolFlags,
      const std::vector<VkDescriptorPoolSize> &poolSizes);
  ~vktDescriptorPool();
  vktDescriptorPool(const vktDescriptorPool &) = delete;
  vktDescriptorPool &operator=(const vktDescriptorPool &) = delete;
 
  bool allocateDescriptor(
      const VkDescriptorSetLayout descriptorSetLayout, VkDescriptorSet &descriptor) const;
 
  void freeDescriptors(std::vector<VkDescriptorSet> &descriptors) const;
 
  void resetPool();
 
 private:
  vktEngineDevice &vktDevice;
  VkDescriptorPool descriptorPool;
 
  friend class vktDescriptorWriter;
};

class vktDescriptorWriter {
 public:
  vktDescriptorWriter(vktDescriptorSetLayout &setLayout, vktDescriptorPool &pool);
 
  vktDescriptorWriter &writeBuffer(uint32_t binding, VkDescriptorBufferInfo *bufferInfo);
  vktDescriptorWriter &writeImage(uint32_t binding, VkDescriptorImageInfo *imageInfo);
 
  bool build(VkDescriptorSet &set);
  void overwrite(VkDescriptorSet &set);
 
 private:
  vktDescriptorSetLayout &setLayout;
  vktDescriptorPool &pool;
  std::vector<VkWriteDescriptorSet> writes;
};


}  // namespace vkt