module Peio.Graphics.Instance;

import Peio.Essentials.Types;

std::vector<std::string> Peio::Graphics::GetAvailableValidationLayers()
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

void Peio::Graphics::Instance::Init(std::string appName)
{
    vk::ApplicationInfo appInfo(appName.c_str(), 1, nullptr, 0, vk::ApiVersion14);
    vk::InstanceCreateInfo createInfo({}, &appInfo);

    const char* extensions[] = {
        vk::KHRSurfaceExtensionName,
        vk::KHRWin32SurfaceExtensionName
    };

    createInfo.enabledExtensionCount = 2;
    createInfo.ppEnabledExtensionNames = extensions;

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

