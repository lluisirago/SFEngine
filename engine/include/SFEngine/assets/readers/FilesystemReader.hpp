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
         * @throws std::invalid_argument if assetRoot is an empty or absolute path.
         * @warning Logs a warning if assetRoot is an invalid path.
         */
        explicit FilesystemReader(const string& assetRoot);

        /**
         * @brief Get the asset data from its filename
         *
         * The asset looked for will be assetRoot_/filename. Recommendation: enable exception mode with throwOnFailure
         * for essential assets only.
         *
         * @param filename Name of the asset file (located in assetRoot_)
         * @param throwOnFailure Exception mode
         * @return std::vector<char> Asset data. Returns an empty vector if the file does not exist or cannot be read.
         *
         * @throws std::runtime_error if asset file cannot be opened and exception mode is enabled.
         * @throws std::runtime_error if asset file size is invalid and exception mode is enabled.
         * @throws std::runtime_error if asset file cannot be read and exception mode is enabled.
         * @warning Logs a warning if the file cannot be opened and exceptions are disabled.
         * @warning Logs a warning if the file size is invalid and exceptions are disabled.
         * @warning Logs a warning if the file cannot be read and exceptions are disabled.
         */
        [[nodiscard]] vector<char> getAsset(const string& filename) override;

        /**
         * @brief Get the asset metadata from its filename
         *
         * The asset metadata looked for will be assetRoot_/filename.meta.json
         *
         * @param filename Name of the asset file (located in assetRoot_)
         * @return json Asset metadata. Returns an empty JSON object if the file does not exist or cannot be
         * read.
         */
        [[nodiscard]] json getMetadata(const string& filename) const override;

       private:

        string assetRoot_;  ///< Path to the assets directory
    };

}  // namespace sfe
