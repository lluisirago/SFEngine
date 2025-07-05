/**
 * @file AssetManager.cpp
 * @author Lluís Irago Montaña <dev.lluisirago@gmail.com>
 * @date 27/06/2025
 *
 * @brief Implementation of AssetManager for loading and managing assets.
 */

#include "SFEngine/assets/AssetManager.hpp"

namespace sfe {

    /**
     * @brief Construct an AssetManager from a valid Pak file.
     * @param fileName Pak filename
     */
    AssetManager::AssetManager(const std::string& fileName) {
        pakFile_ = PakFileReader(fileName);
    }

    /**
     * @brief Get the asset data from its pathname
     *
     * The asset extraction uses a cache to avoid unnecessary file readings.
     * If the file is not in the cache, it is loaded for the next extraction.
     *
     * @param path Asset pathname
     * @return std::vector<char> Asset data
     */
    std::vector<char> AssetManager::getAsset(const std::string& path) {
        auto it = assetCache_.find(path);
        if (it != assetCache_.end()) {
            touchCacheEntry(path);
            return it->second;
        }

        std::vector<char> data = pakFile_.getAsset(path);

        addToCache(path, std::move(data));
        return assetCache_.at(path);
    }

    /**
     * @brief Get the asset metadata from its pathname
     *
     * @param path Asset pathname
     * @return const nlohmann::json& Asset metadata
     */
    nlohmann::json AssetManager::getMetadata(const std::string& path) const {
        return pakFile_.getMetadata(path);
    }

    /**
     * @brief Move the asset to the front of the cache
     *
     * @param path Asset pathname
     */
    void AssetManager::touchCacheEntry(const std::string& path) {
        auto it = usageIterators_.find(path);
        if (it != usageIterators_.end()) {
            usageOrder_.erase(it->second);
        }
        usageOrder_.push_front(path);
        usageIterators_[path] = usageOrder_.begin();
    }

    /**
     * @brief Adds an asset to the cache
     *
     * Check if the asset is already in the cache or not. If true it is moved to the front, otherwise it is emplaced.
     * If the added asset exceeds the cache size, the last asset to be used is removed.
     *
     * @param path Asset pathname
     * @param data Asset data
     */
    void AssetManager::addToCache(const std::string& path, std::vector<char>&& data) {
        if (assetCache_.find(path) != assetCache_.end()) {
            assetCache_[path] = std::move(data);
            touchCacheEntry(path);
            return;
        }

        assetCache_.emplace(path, std::move(data));
        usageOrder_.push_front(path);
        usageIterators_[path] = usageOrder_.begin();

        if ((int) assetCache_.size() > SIZE_ASSET_CACHE) {
            std::string leastUsed = usageOrder_.back();
            usageOrder_.pop_back();
            assetCache_.erase(leastUsed);
            usageIterators_.erase(leastUsed);
        }
    }

}  // namespace sfe
