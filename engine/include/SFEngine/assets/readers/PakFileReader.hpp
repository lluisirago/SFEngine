#pragma once

#include "AssetReader.hpp"
#include "nlohmann/json.hpp"
#include <fstream>
#include <string>
#include <unordered_map>
#include <vector>

namespace sfe {

    struct PakEntry {
        std::string path;
        size_t offset;
        size_t size;
        size_t originalSize;
        bool compressed;
        std::string compression;
        nlohmann::json meta;
    };

    class PakFileReader : public sfe::AssetReader {
       public:

        PakFileReader() = default;
        explicit PakFileReader(const std::string& filename);

        bool open(const std::string& filename);
        bool isOpen() const;

        std::vector<char> getAsset(const std::string& path) override;
        nlohmann::json getMetadata(const std::string& path) const override;

       private:

        std::ifstream file_;
        std::unordered_map<std::string, PakEntry> entries_;
        std::vector<char> decompress(const std::vector<char>& compressed, size_t original_size) const;
    };
}  // namespace sfe
