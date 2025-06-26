#pragma once

#include <fstream>
#include <nlohmann/json.hpp>
#include <string>
#include <unordered_map>
#include <vector>

struct PakEntry {
    std::string path;
    size_t offset;
    size_t size;
    size_t original_size;
    bool compressed;
    std::string compression;
    nlohmann::json meta;
};

class AssetManager {
   public:

    bool loadPak(const std::string& pakFile);
    std::vector<char> getAsset(const std::string& path);
    const nlohmann::json& getMetadata(const std::string& path);

   private:

    std::ifstream pak_stream;
    std::unordered_map<std::string, PakEntry> asset_table;
    std::vector<char> decompress_zlib(const std::vector<char>& compressed, size_t original_size);
};
