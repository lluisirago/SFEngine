/**
 * @file FilesystemReader.cpp
 * @author Lluís Irago Montaña <dev.lluisirago@gmail.com>
 * @date 03/07/2025
 *
 * @brief Low-level class for assets and assets metadata reading from the filesystem.
 */

#include "SFEngine/assets/readers/FilesystemReader.hpp"
#include <fstream>
#include <iostream>
#include <limits>

namespace sfe {

    /**
     * @brief Construct a new FilesystemReader object
     *
     * Expects assetRoot to be a relative path to the assets directory.
     *
     * @param assetRoot Path to the assets directory (relative)
     */
    FilesystemReader::FilesystemReader(const std::string& assetRoot) noexcept : assetRoot_(assetRoot) {}

    /**
     * @brief Get the asset data from its filename
     *
     * The asset looked for will be assetRoot_/filename. It is used buffered file reading with size-based preallocation
     * using seek/tell.
     *
     * @param filename Name of the asset file (located in assetRoot_)
     * @return std::vector<char> Asset data. Returns an empty vector if the file does not exist or cannot be read.
     */
    std::vector<char> FilesystemReader::getAsset(const std::string& filename) {
        std::filesystem::path assetPath = std::filesystem::path(assetRoot_) / (filename);
        std::ifstream in(assetPath, std::ios::binary);
        if (!in) return {};

        // Seek to file end to get size
        in.seekg(0, std::ios::end);
        std::streamsize size = in.tellg();
        // Validate size
        if (size <= 0 || static_cast<uint64_t>(size) > static_cast<uint64_t>(std::numeric_limits<size_t>::max())) {
            return {};
        }
        // Reset to beginning for reading
        in.seekg(0, std::ios::beg);

        std::vector<char> buffer(static_cast<size_t>(size));
        if (!in.read(buffer.data(), size)) {
            return {};
        }
        return buffer;
    }

    /**
     * @brief Get the asset metadata from its filename
     *
     * The asset metadata looked for will be assetRoot_/filename.meta.json
     *
     * @param filename Name of the asset file (located in assetRoot_)
     * @return nlohmann::json Asset metadata. Returns an empty JSON object if the file does not exist or cannot be
     * read.
     */
    nlohmann::json FilesystemReader::getMetadata(const std::string& filename) const {
        std::filesystem::path metaPath = std::filesystem::path(assetRoot_) / (filename + ".meta.json");
        std::ifstream in(metaPath);
        if (!in) return {};
        nlohmann::json meta;
        in >> meta;
        return meta;
    }

}  // namespace sfe
