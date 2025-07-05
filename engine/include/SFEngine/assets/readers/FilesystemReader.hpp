/**
 * @file FilesystemReader.hpp
 * @author Lluís Irago Montaña <dev.lluisirago@gmail.com>
 * @date 03/07/2025
 *
 * @brief Low-level class for assets and assets metadata reading from the filesystem.
 */

#include "AssetReader.hpp"
#include "nlohmann/json.hpp"

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
         */
        explicit FilesystemReader(const std::string& assetRoot) noexcept;

        /**
         * @brief Get the asset data from its filename
         *
         * The asset looked for will be assetRoot_/filename
         *
         * @param filename Name of the asset file (located in assetRoot_)
         * @return std::vector<char> Asset data. Returns an empty vector if the file does not exist or cannot be read.
         */
        [[nodiscard]] std::vector<char> getAsset(const std::string& filename) override;

        /**
         * @brief Get the asset metadata from its filename
         *
         * The asset metadata looked for will be assetRoot_/filename.meta.json
         *
         * @param filename Name of the asset file (located in assetRoot_)
         * @return nlohmann::json Asset metadata. Returns an empty JSON object if the file does not exist or cannot be
         * read.
         */
        [[nodiscard]] nlohmann::json getMetadata(const std::string& filename) const override;

       private:

        std::string assetRoot_;  ///< Path to the assets directory
    };

}  // namespace sfe
