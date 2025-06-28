module Peio.Graphics.Instance;

import Peio.Essentials.Types;

std::vector<std::string> Peio::Graphics::Instance::GetAvailableValidationLayers()
{
    uint layerCount;
    vk::Result result = vk::enumerateInstanceLayerProperties(&layerCount, nullptr);
    if (result != vk::Result::eSuccess) {
        throw FailedAvailableValidationLayerRetrievalException(result);
    }

    std::vector<vk::LayerProperties> layers(layerCount);
    result = vk::enumerateInstanceLayerProperties(&layerCount, &layers[0]);
    if (result != vk::Result::eSuccess) {
        throw FailedAvailableValidationLayerRetrievalException(result);
    }

    std::vector<std::string> layerNames{};
    for (auto layer : layers) {
        layerNames.push_back(std::string(&layer.layerName[0]));
    }
    return layerNames;
}

std::vector<std::string> Peio::Graphics::Instance::GetAvailableExtensions()
{
    uint extensionCount;
    vk::Result result = vk::enumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
    if (result != vk::Result::eSuccess) {
        throw FailedAvailableInstanceExtensionRetrievalException(result);
    }

    if (extensionCount == 0)
        return {};

    std::vector<vk::ExtensionProperties> properties(extensionCount);
    result = vk::enumerateInstanceExtensionProperties(nullptr, &extensionCount, &properties[0]);
    if (result != vk::Result::eSuccess) {
        throw FailedAvailableInstanceExtensionRetrievalException(result);
    }

    std::vector<std::string> names(extensionCount);
    for (uint i = 0; i < extensionCount; i++)
        names[i] = std::string(&properties[i].extensionName[0]);
    return names;
}

void Peio::Graphics::Instance::Init(const std::string& appName, const std::vector<const char*>& extensions)
{
    vk::ApplicationInfo appInfo(appName.c_str(), 1, nullptr, 0, vk::ApiVersion14);
    vk::InstanceCreateInfo createInfo({}, &appInfo);

    if (extensions.size() > 0) {
        createInfo.enabledExtensionCount = (uint)extensions.size();
        createInfo.ppEnabledExtensionNames = &extensions[0];
    }

    if (validationLayers.size() > 0) {
        auto availableLayers = GetAvailableValidationLayers();

        for (auto layer : validationLayers) {
            std::string curName(layer);
            if (std::find(availableLayers.begin(), availableLayers.end(), curName) == availableLayers.end()) {
                throw ValidationLayerNotAvailableException();
            }
        }

        createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
        createInfo.ppEnabledLayerNames = validationLayers.data();
    }

    vk::Result result = vk::createInstance(&createInfo, nullptr, &instance);
    if (result != vk::Result::eSuccess) {
        throw FailedInstanceCreationException(result);
    }
}

vk::Instance& Peio::Graphics::Instance::Get()
{
	return instance;
}

const std::vector<const char*>& Peio::Graphics::Instance::GetValidationLayers() const
{
    return validationLayers;
}

void Peio::Graphics::Instance::Destroy()
{
    instance.destroy(nullptr);
}