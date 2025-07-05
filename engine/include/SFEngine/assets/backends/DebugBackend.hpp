#pragma once
#include "AssetBackend.hpp"
#include <chrono>
#include <filesystem>
#include <nlohmann/json.hpp>
#include <unordered_map>

namespace sfe {

    class DebugBackend : public AssetBackend {
       public:

        explicit DebugBackend(const std::string& assetRoot);

        std::vector<char> getAsset(const std::string& path) override;
        nlohmann::json getMetadata(const std::string& path) const override;
        void update() override;

       private:

        std::string assetRoot_;
        std::unordered_map<std::string, std::vector<char>> assetCache_;
        std::unordered_map<std::string, std::filesystem::file_time_type> timestamps_;
        std::unordered_map<std::string, nlohmann::json> metadata_;

        void reloadIfChanged(const std::string& path);
    };

}  // namespace sfe