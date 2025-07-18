/**
 * @file FilesystemReader.cpp
 * @author Lluís Irago Montaña <dev.lluisirago@gmail.com>
 * @date 03/07/2025
 *
 * @brief Low-level class for assets and assets metadata reading from the filesystem.
 */

#include "SFEngine/assets/readers/FilesystemReader.hpp"
#include "SFEngine/utils/ErrorHandlingMacros.hpp"
#include <fstream>
#include <limits>

using namespace std;
using json = nlohmann::json;

namespace sfe {

    /**
     * @brief Construct a new FilesystemReader object
     *
     * Expects assetRoot to be a relative path to the assets directory.
     *
     * @param assetRoot Path to the assets directory (relative)
     *
     * @throws std::invalid_argument if assetRoot is an empty, absolute or invalid path (when exceptions are enabled).
     * @warning Errors are logged (when logging is enabled).
     */
    FilesystemReader::FilesystemReader(const string& assetRoot) : assetRoot_(assetRoot) {
        SFE_THROW_IF(assetRoot.empty(), invalid_argument, "assetRoot must be a non-empty path");
        SFE_THROW_IF(filesystem::path(assetRoot).is_absolute() || !filesystem::exists(filesystem::absolute(assetRoot)),
                     invalid_argument,
                     "assetRoot must be a valid relative path: " + assetRoot);
    }

    /**
     * @brief Get the asset data from its filename
     *
     * The asset looked for will be assetRoot_/filename. It is used buffered file reading with size-based preallocation
     * using seek/tell.
     *
     * @param filename Name of the asset file (located in assetRoot_)
     * @return std::vector<char> Asset data. Returns an empty vector if an error ocurr and exceptions are disabled.
     *
     * @throws std::runtime_error if the file cannot be opened, has invalid size, or cannot be read (when exceptions are
     * enabled).
     * @warning Errors are logged (when logging is enabled).
     */
    vector<char> FilesystemReader::getAsset(const string& filename) {
        filesystem::path assetPath = filesystem::path(assetRoot_) / filename;
        ifstream in(assetPath, ios::binary);
        bool fileNotOpened = !in;
        SFE_THROW_OR_RETURN_IF(!in, runtime_error, "Failed to open asset file: " + assetPath.string(), {});

        // Seek to the end of the file to get its size
        in.seekg(0, ios::end);
        streamsize size = in.tellg();
        SFE_THROW_OR_RETURN_IF(
            size <= 0 || static_cast<uint64_t>(size) > static_cast<uint64_t>(numeric_limits<size_t>::max()),
            runtime_error,
            "Invalid asset file size: " + assetPath.string(),
            {});

        // Seek to the begining to read the file
        in.seekg(0, ios::beg);

        vector<char> buffer(static_cast<size_t>(size));
        SFE_THROW_OR_RETURN_IF(
            !in.read(buffer.data(), size), runtime_error, "Failed to read asset file: " + assetPath.string(), {});
        return buffer;
    }

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
    json FilesystemReader::getMetadata(const string& filename) const {
        filesystem::path metaPath = filesystem::path(assetRoot_) / (filename + ".meta.json");
        ifstream in(metaPath);
        SFE_THROW_OR_RETURN_IF(!in, runtime_error, "Failed to open meta file: " + metaPath.string(), {});
        json meta;
        in >> meta;
        return meta;
    }

}  // namespace sfe
