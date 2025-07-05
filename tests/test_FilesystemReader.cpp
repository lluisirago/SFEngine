// tests/test_FilesystemReader.cpp
#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

#include "SFEngine/assets/readers/FilesystemReader.hpp"
#include <filesystem>
#include <fstream>

using namespace sfe;

TEST_CASE("FilesystemReader reads txt correctly", "[FilesystemReader]") {
    // Setup test asset folder
    const std::string testDir = "tests/assets_test";
    const std::string assetPath = testDir + "/hello.txt";

    // Ensure the file exists
    std::filesystem::create_directories(testDir);
    std::ofstream(assetPath) << "hello world";

    REQUIRE(std::filesystem::exists(assetPath));

    std::ifstream f(assetPath);
    std::string content((std::istreambuf_iterator<char>(f)), std::istreambuf_iterator<char>());

    REQUIRE(content == "hello world");

    FilesystemReader reader(testDir);
    auto data = reader.getAsset("hello.txt");

    REQUIRE(std::string(data.begin(), data.end()) == "hello world");
}

TEST_CASE("FilesystemReader reads metadata JSON", "[FilesystemReader]") {
    const std::string testDir = "tests/assets_test";
    const std::string jsonPath = testDir + "/hello.txt.meta.json";

    std::ofstream(jsonPath) << R"({ "author": "test", "version": 1 })";

    REQUIRE(std::filesystem::exists(jsonPath));

    FilesystemReader reader(testDir);
    auto meta = reader.getMetadata("hello.txt");

    REQUIRE(meta["author"] == "test");
    REQUIRE(meta["version"] == 1);
}

TEST_CASE("FilesystemReader loads JPEG correctly", "[FilesystemReader][JPEG]") {
    const std::string testDir = "tests/assets_test";
    const std::string imagePath = testDir + "/image.jpeg";

    // Copia de referencia directa con ifstream
    std::ifstream ref(imagePath, std::ios::binary);
    REQUIRE(ref);  // el archivo debe existir

    std::vector<char> refData((std::istreambuf_iterator<char>(ref)), {});

    FilesystemReader reader(testDir);
    std::vector<char> data = reader.getAsset("image.jpeg");

    REQUIRE(data == refData);
}
