#include "SFEngine/assets/AssetManager.hpp"
#include <zlib.h>
#include <iostream>

bool AssetManager::loadPak(const std::string& pakFile) {
    pak_stream.open(pakFile, std::ios::binary);
    if (!pak_stream) {
        std::cerr << "No se puede abrir el archivo .pak" << std::endl;
        return false;
    }

    pak_stream.seekg(0, std::ios::end);
    size_t file_size = pak_stream.tellg();
    size_t max_index_size = 1024 * 1024;
    size_t search_start = file_size > max_index_size ? file_size - max_index_size : 0;

    pak_stream.seekg(search_start);
    std::vector<char> tail(file_size - search_start);
    pak_stream.read(tail.data(), tail.size());

    std::string tail_str(tail.begin(), tail.end());
    size_t json_pos = tail_str.find('[');
    if (json_pos == std::string::npos) return false;

    std::string json_str = tail_str.substr(json_pos);
    auto index = nlohmann::json::parse(json_str);

    for (const auto& entry : index) {
        PakEntry e;
        e.path = entry["path"];
        e.offset = entry["offset"];
        e.size = entry["size"];
        e.original_size = entry["original_size"];
        e.compressed = entry["compressed"];
        e.compression = entry["compression"];
        if (entry.contains("meta")) {
            e.meta = entry["meta"];
        }
        asset_table[e.path] = e;
    }

    return true;
}

std::vector<char> AssetManager::getAsset(const std::string& path) {
    auto it = asset_table.find(path);
    if (it == asset_table.end()) {
        throw std::runtime_error("Asset no encontrado: " + path);
    }

    const PakEntry& entry = it->second;
    pak_stream.seekg(entry.offset);
    std::vector<char> buffer(entry.size);
    pak_stream.read(buffer.data(), entry.size);

    if (entry.compressed && entry.compression == "zlib") {
        return decompress_zlib(buffer, entry.original_size);
    }

    return buffer;
}

const nlohmann::json& AssetManager::getMetadata(const std::string& path) {
    return asset_table.at(path).meta;
}

std::vector<char> AssetManager::decompress_zlib(const std::vector<char>& compressed, size_t original_size) {
    std::vector<char> decompressed(original_size);
    uLongf dest_len = original_size;
    if (uncompress(reinterpret_cast<Bytef*>(decompressed.data()),
                   &dest_len,
                   reinterpret_cast<const Bytef*>(compressed.data()),
                   compressed.size()) != Z_OK) {
        throw std::runtime_error("Error al descomprimir asset");
    }
    return decompressed;
}
