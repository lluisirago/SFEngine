/**
 * @file AssetManager.hpp
 * @author Lluís Irago Montaña <dev.lluisirago@gmail.com>
 * @date 27/06/2025
 *
 * @brief High-level interface for accessing and managing game assets from a PakFile.
 */

#pragma once

#include "nlohmann/json.hpp"
#include "readers/PakFileReader.hpp"
#include <string>
#include <unordered_map>
#include <vector>

namespace sfe {

    /**
     * @class AssetManager
     * @brief Manages loading and querying assets from a PakFile.
     *
     * Provides asset access via memory buffers and metadata in JSON format.
     */
    class AssetManager {
       public:

        /**
         * @brief Constructs an AssetManager from a valid Pak file.
         *
         * @param fileName Pak filename
         */
        explicit AssetManager(const std::string& fileName);

        /**
         * @brief Gets the asset data from its pathname
         *
         * @param path Asset pathname
         * @return std::vector<char> Asset data
         */
        std::vector<char> getAsset(const std::string& path);

        /**
         * @brief Gets the asset metadata from its pathname
         *
         * @param path Asset pathname
         * @return nlohmann::json Asset metadata
         */
        nlohmann::json getMetadata(const std::string& path) const;

       private:

        PakFileReader pakFile_;  ///< File with the packaged assets

        /** Asset cache **/
        static const int SIZE_ASSET_CACHE = 100;                         ///< Max number of assets in asset cache
        std::unordered_map<std::string, std::vector<char>> assetCache_;  ///< Cache with the recent used assets
        std::list<std::string> usageOrder_;                              ///< Assets in cache ordered by last use
        std::unordered_map<std::string, std::list<std::string>::iterator> usageIterators_;  ///< For fast searching

        /**
         * @brief Moves the asset to the front of the cache
         *
         * @param path Asset pathname
         */
        void touchCacheEntry(const std::string& path);

        /**
         * @brief Adds an asset to the cache
         *
         * @param path Asset pathname
         * @param data Asset data
         */
        void addToCache(const std::string& path, std::vector<char>&& data);
    };
}  // namespace sfe
