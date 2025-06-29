# SFEngine

```cmake
   _______  ______  ______    
  /  ____/ /  ___/ /  ___/ _  __  _____  __  _  __  _____
 /____  / /  ___/ /  __/  / \/ / / /__  / / / \/ / /  /_/  
/______/ /_ /    /_____/ /_/\_/ /____/ /_/ /_/\_/ /____/  version 0.1.0
```

SFEngine is a simple C++ graphics engine designed to facilitate the development of high-performance and flexible 2D games. Includes integration with SFML, static code analysis tools, and multi-platform configuration.

---

## Requirements

- git

---

## Instalation and configuration

1. Clone the repository:
    ```bash
    git clone https://github.com/lluisirago/SFEngine.git
    cd SFEngine
    ```
2. Run the setup script (used for auto installing required tools)
    ```bash
    chmod +x ./scripts/setup
    ./scripts/setup
    ```
3. Build the project
    ```bash
    cmake -S . -B build
    cmake --build build
    ```

[![Ask DeepWiki](https://deepwiki.com/badge.svg)](https://deepwiki.com/lluisirago/SFEngine)