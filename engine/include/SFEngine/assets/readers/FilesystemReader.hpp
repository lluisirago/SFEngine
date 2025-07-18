/**
 * @file FilesystemReader.hpp
 * @author Lluís Irago Montaña <dev.lluisirago@gmail.com>
 * @date 03/07/2025
 *
 * @brief Low-level class for assets and assets metadata reading from the filesystem.
 */

#pragma once

#include "AssetReader.hpp"
#include "nlohmann/json.hpp"

using namespace std;
using json = nlohmann::json;

namespace sfe {

    /**
     * @class FilesystemReader
     * @brief Read filesystem assets and assets metadata
     *
     * Provides asset access via memory buffers and metadata in JSON format.
     */
    class FilesystemReader : public AssetReader {
       public:

        /**
         * @brief Construct a new FilesystemReader object
         *
         * Expects assetRoot to be a relative path to the assets directory.
         *
         * @param assetRoot Path to the assets directory (relative)
         *
         * @throws std::invalid_argument if assetRoot is an empty, absolute or invalid path (when exceptions are
         * enabled).
         * @warning Errors are logged (when logging is enabled).
         */
        explicit FilesystemReader(const string& assetRoot);

        /**
         * @brief Get the asset data from its filename
         *
         * The asset looked for will be assetRoot_/filename. It is used buffered file reading with size-based
         * preallocation using seek/tell.
         *
         * @param filename Name of the asset file (located in assetRoot_)
         * @return std::vector<char> Asset data. Returns an empty vector if an error ocurr and exceptions are disabled.
         *
         * @throws std::runtime_error if the file cannot be opened, has invalid size, or cannot be read (when exceptions
         * are enabled).
         * @warning Errors are logged (when logging is enabled).
         */
        [[nodiscard]] vector<char> getAsset(const string& filename) override;

        /**
         * @brief Get the asset metadata from its filename
         *
         * The asset metadata looked for will be assetRoot_/filename.meta.json
         *
         * @param filename Name of the asset file (located in assetRoot_)
         * @return nlohmann::json Asset metadata. Returns an empty JSON object if the file does not exist or cannot be
         * read and exceptions are disabled.
         *
         * @throws std::runtime_error if the meta file cannot be opened (when exceptions are enabled).
         * @warning Errors are logged (when logging is enabled).
         */
        [[nodiscard]] json getMetadata(const string& filename) const override;

       private:

        string assetRoot_;  ///< Path to the assets directory
    };

}  // namespace sfe
