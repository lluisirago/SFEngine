#pragma once

#include "nlohmann/json.hpp"

using namespace std;
using json = nlohmann::json;

namespace sfe {

    class AssetReader {
       public:

        virtual vector<char> getAsset(const string& path) = 0;
        virtual json getMetadata(const string& path) const = 0;
        virtual ~AssetReader() = default;
    };
}  // namespace sfe