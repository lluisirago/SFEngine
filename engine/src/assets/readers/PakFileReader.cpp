#include "SFEngine/assets/readers/PakFileReader.hpp"
#include "miniminiz.h"
#include <filesystem>
#include <iostream>
#include <stdexcept>

namespace sfe {

    PakFileReader::PakFileReader(const std::string& filename) {
        open(filename);
    }

    bool PakFileReader::open(const std::string& filename) {
        file_.open(filename, std::ios::binary);
        if (!file_) {
            std::cerr << "[PakFile] Cannot open file: " << filename << std::endl;
            return false;
        }

        std::error_code ec;
        size_t fileSize = std::filesystem::file_size(filename, ec);
        if (ec) {
            std::cerr << "[PakFile] Failed to get file size: " << ec.message() << std::endl;
            return false;
        }

        // Firma + tamaño del índice JSON (12 bytes al final)
        file_.seekg(-12, std::ios::end);
        uint64_t indexSize;
        char magic[4];
        file_.read(reinterpret_cast<char*>(&indexSize), sizeof(uint64_t));
        file_.read(magic, 4);

        if (strncmp(magic, "SPAK", 4) != 0) {
            std::cerr << "[PakFile] Invalid file signature" << std::endl;
            return false;
        }

        // Leer el JSON desde el final
        file_.seekg(static_cast<std::streamoff>(fileSize - 12 - indexSize));
        std::vector<char> jsonBuffer(indexSize);
        file_.read(jsonBuffer.data(), indexSize);
        auto index = nlohmann::json::parse(jsonBuffer);

        for (const auto& entry : index) {
            PakEntry e;
            e.path = entry["path"];
            e.offset = entry["offset"];
            e.size = entry["size"];
            e.originalSize = entry["originalSize"];
            e.compressed = entry["compressed"];
            e.compression = entry["compression"];
            if (entry.contains("meta")) {
                e.meta = entry["meta"];
            }
            entries_[e.path] = std::move(e);
        }

        return true;
    }

    bool PakFileReader::isOpen() const {
        return file_.is_open();
    }

    std::vector<char> PakFileReader::getAsset(const std::string& path) {
        auto it = entries_.find(path);
        if (it == entries_.end()) {
            throw std::runtime_error("Asset not found: " + path);
        }

        const PakEntry& entry = it->second;
        file_.seekg(entry.offset);
        std::vector<char> buffer(entry.size);
        file_.read(buffer.data(), entry.size);

        if (entry.compressed && entry.compression == "zlib") {
            return decompress(buffer, entry.originalSize);
        }

        return buffer;
    }

    nlohmann::json PakFileReader::getMetadata(const std::string& path) const {
        auto it = entries_.find(path);
        if (it == entries_.end()) {
            throw std::out_of_range("Metadata not found: " + path);
        }
        return it->second.meta;
    }

    std::vector<char> PakFileReader::decompress(const std::vector<char>& compressed, size_t original_size) const {
        std::vector<char> output(original_size);
        mz_ulong destLen = static_cast<mz_ulong>(original_size);

        int status = mz_uncompress(reinterpret_cast<unsigned char*>(output.data()),
                                   &destLen,
                                   reinterpret_cast<const unsigned char*>(compressed.data()),
                                   static_cast<mz_ulong>(compressed.size()));

        if (status != MZ_OK || destLen != original_size) {
            throw std::runtime_error("Failed to decompress asset using zlib");
        }

        return output;
    }

}  // namespace sfe