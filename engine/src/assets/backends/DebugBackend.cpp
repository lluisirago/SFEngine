#include "SFEngine/assets/backends/DebugBackend.hpp"
#include <fstream>
#include <iostream>

namespace sfe {

    DebugBackend::DebugBackend(const std::string& assetRoot) : assetRoot_(assetRoot) {}

    std::vector<char> DebugBackend::getAsset(const std::string& path) {
        std::ifstream in(path, std::ios::binary);
        return std::vector<char>((std::istreambuf_iterator<char>(in)), {});
    }

    nlohmann::json DebugBackend::getMetadata(const std::string& path) const {
        std::filesystem::path metaPath = std::filesystem::path(assetRoot_) / (path + ".meta.json");
        std::ifstream in(metaPath);
        if (!in) return {};
        nlohmann::json meta;
        in >> meta;
        return meta;
    }

    void DebugBackend::reloadIfChanged(const std::string& path) {
        std::filesystem::path fullPath = std::filesystem::path(assetRoot_) / path;
        auto modTime = std::filesystem::last_write_time(fullPath);

        if (!timestamps_.contains(path) || timestamps_[path] != modTime) {
            assetCache_[path] = getAsset(fullPath.string());
            metadata_[path] = getMetadata(fullPath.string() + ".meta.json");
            timestamps_[path] = modTime;
            std::cout << "[HotReload] Reloaded: " << path << std::endl;
        }
    }

}  // namespace sfe