#pragma once

// Detect Windows + shared build
#if defined(_WIN32) && defined(SFENGINE_SHARED)
  #ifdef SFENGINE_EXPORTS
    // When DLL compiles
    #define SFENGINE_API __declspec(dllexport)
  #else
    // When a client use it
    #define SFENGINE_API __declspec(dllimport)
  #endif
#else
  // In other systems or with static build
  #define SFENGINE_API
#endif
