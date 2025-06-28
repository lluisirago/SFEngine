/**
 * @file AssetManager.cpp
 * @author Lluís Irago Montaña <dev.lluisirago@gmail.com>
 * @date 27/06/2025
 *
 * @brief
 */

#include "SFEngine/assets/AssetManager.hpp"
#include <zlib.h>
#include <iostream>

bool AssetManager::loadPak(const std::string& pakFile) {
    pakStream_.open(pakFile, std::ios::binary);
    if (!pakStream_) {
        std::cerr << "No se puede abrir el archivo .pak" << std::endl;
        return false;
    }

    pakStream_.seekg(0, std::ios::end);
    size_t file_size = pakStream_.tellg();
    size_t max_index_size = 1024 * 1024;
    size_t search_start = file_size > max_index_size ? file_size - max_index_size : 0;

    pakStream_.seekg(search_start);
    std::vector<char> tail(file_size - search_start);
    pakStream_.read(tail.data(), tail.size());

    std::string tail_str(tail.begin(), tail.end());
    size_t json_pos = tail_str.find('[');
    if (json_pos == std::string::npos) return false;

    std::string json_str = tail_str.substr(json_pos);
    auto index = nlohmann::json::parse(json_str);

    for (const auto& entry : index) {
        PakEntry e;
        e.path_ = entry["path"];
        e.offset_ = entry["offset"];
        e.size_ = entry["size"];
        e.originalSize_ = entry["originalSize"];
        e.compressed_ = entry["compressed"];
        e.compression_ = entry["compression"];
        if (entry.contains("meta")) {
            e.meta_ = entry["meta"];
        }
        assetTable_[e.path_] = e;
    }

    return true;
}

std::vector<char> AssetManager::getAsset(const std::string& path) {
    auto it = assetTable_.find(path);
    if (it == assetTable_.end()) {
        throw std::runtime_error("Asset no encontrado: " + path);
    }

    const PakEntry& entry = it->second;
    pakStream_.seekg(entry.offset_);
    std::vector<char> buffer(entry.size_);
    pakStream_.read(buffer.data(), entry.size_);

    if (entry.compressed_ && entry.compression_ == "zlib") {
        return decompress_zlib(buffer, entry.originalSize_);
    }

    return buffer;
}

const nlohmann::json& AssetManager::getMetadata(const std::string& path) {
    return assetTable_.at(path).meta_;
}

std::vector<char> AssetManager::decompress_zlib(const std::vector<char>& compressed, size_t original_size) {
    std::vector<char> decompressed(original_size);
    uLongf destLen = original_size;

    if (uncompress(reinterpret_cast<Bytef*>(decompressed.data()),
                   &destLen,
                   reinterpret_cast<const Bytef*>(compressed.data()),
                   compressed.size()) != Z_OK) {
        throw std::runtime_error("Error al descomprimir asset");
    }
    return decompressed;
}
