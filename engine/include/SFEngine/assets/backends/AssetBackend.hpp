#pragma once

#include "nlohmann/json.hpp"

namespace sfe {

    class AssetBackend {
       public:

        virtual std::vector<char> getAsset(const std::string& path) = 0;
        virtual nlohmann::json getMetadata(const std::string& path) const = 0;
        virtual void update() {}  // Para hot-reload (noop en Release)
        virtual ~AssetBackend() = default;
    };
}  // namespace sfe
