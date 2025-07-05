#pragma once

#include "nlohmann/json.hpp"

namespace sfe {

    class AssetReader {
       public:

        virtual std::vector<char> getAsset(const std::string& path) = 0;
        virtual nlohmann::json getMetadata(const std::string& path) const = 0;
        virtual ~AssetReader() = default;
    };
}  // namespace sfe