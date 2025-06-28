/**
 * @file AssetManager.hpp
 * @author Lluís Irago Montaña <dev.lluisirago@gmail.com>
 * @date 27/06/2025
 *
 * @brief AssetManager handles loading and accessing assets from a packed file.
 *
 * This class provides functionality for reading a custom asset pack file (.pak),
 * decompressing assets (currently supporting zlib), and retrieving both the raw
 * asset data and its associated metadata stored in JSON format.
 */

#pragma once

#include <fstream>
#include <nlohmann/json.hpp>
#include <string>
#include <unordered_map>
#include <vector>

/**
 * @struct PakEntry
 * @brief Represents an entry within a packed asset file.
 */
struct PakEntry {
    std::string path_;         ///< Logical path or identifier of the asset.
    size_t offset_;            ///< Offset from the beginning of the .pak file.
    size_t size_;              ///< Compressed size of the asset.
    size_t originalSize_;      ///< Original (uncompressed) size.
    bool compressed_;          ///< Whether the asset is compressed.
    std::string compression_;  ///< Compression algorithm used (e.g., "zlib").
    nlohmann::json meta_;      ///< Arbitrary metadata for the asset (JSON object).
};

/**
 * @class AssetManager
 * @brief Manages loading and lifetime of game assets stored in a packed file.
 *
 * AssetManager allows efficient access to assets stored in a custom binary format.
 * It supports compressed assets and metadata querying via JSON.
 */
class AssetManager {
   public:

    /**
     * @brief Loads and parses a .pak file.
     * @param pakFile Path to the packed asset file.
     * @return True if the file was successfully loaded and parsed, false otherwise.
     */
    bool loadPak(const std::string& pakFile);

    /**
     * @brief Retrieves an asset's data by its path.
     * @param path Logical path of the asset within the .pak file.
     * @return Byte buffer containing the (possibly decompressed) asset.
     */
    std::vector<char> getAsset(const std::string& path);

    /**
     * @brief Retrieves the metadata associated with an asset.
     * @param path Logical path of the asset.
     * @return JSON object containing the metadata.
     */
    const nlohmann::json& getMetadata(const std::string& path);

   private:

    std::ifstream pakStream_;                               ///< Stream used to access the .pak file.
    std::unordered_map<std::string, PakEntry> assetTable_;  ///< Internal asset lookup table.

    /**
     * @brief Decompresses zlib-compressed data
     * @param compressed Byte buffer with compressed data.
     * @param original_size Expected size after decompression.
     * @return Decompressed byte buffer.
     */
    std::vector<char> decompress_zlib(const std::vector<char>& compressed, size_t original_size);
};
