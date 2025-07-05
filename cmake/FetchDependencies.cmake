include(FetchContent)

function(fetch_sfml) # Unused
    FetchContent_Declare(
        sfml
        GIT_REPOSITORY https://github.com/SFML/SFML.git
        GIT_TAG 2.6.0
    )
    FetchContent_MakeAvailable(sfml)
endfunction()

function(fetch_json)
    FetchContent_Declare(
        json
        GIT_REPOSITORY https://github.com/nlohmann/json.git
        GIT_TAG v3.12.0
    )
    FetchContent_MakeAvailable(json)
endfunction()

function(fetch_zlib)
    FetchContent_Declare(
        zlib
        URL https://zlib.net/current/zlib.tar.gz
        DOWNLOAD_EXTRACT_TIMESTAMP TRUE
    )
    FetchContent_MakeAvailable(zlib)
endfunction()
