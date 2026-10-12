# Impala
Impala Game Engine — a small C++17 engine on [SDL3](https://github.com/libsdl-org/SDL), plus two games built with it.

```
impala-engine/   engine library (Impala::Engine)
games/dzone/     2D game
games/golf/      3D game
```

SDL3 is downloaded and built automatically by CMake on the first configure.

## Setup: macOS

1. Install the Xcode command line tools and the build tools:
   ```bash
   xcode-select --install
   brew install cmake ninja clang-format
   ```
2. Install [VS Code](https://code.visualstudio.com/), open the repo folder, and accept the
   recommended extensions (clangd, CMake Tools, C/C++, CodeLLDB, EditorConfig).

## Setup: Windows

1. Install [Visual Studio 2022](https://visualstudio.microsoft.com/downloads/) (Community or
   Build Tools) with the **Desktop development with C++** workload. This provides MSVC, CMake
   and Ninja.
2. Install Git, LLVM (for a current clang-format / clang-tidy) and VS Code:
   ```powershell
   winget install Git.Git LLVM.LLVM Microsoft.VisualStudioCode
   ```
3. Always launch VS Code from **Developer PowerShell for VS 2022** so the MSVC tools are on
   the path:
   ```powershell
   cd path\to\Impala
   code .
   ```
   Accept the recommended extensions when prompted.

## Both platforms

Once per clone, enable the repo's git hooks (rejects commits with unformatted C++):

```bash
git config core.hooksPath .githooks
```

In VS Code, pick the **Debug** preset in the CMake Tools status bar and build. Or from a
terminal:

```bash
cmake --preset debug
cmake --build --preset debug
```

Games are written to `build/bin/` (`golf`, `dzone`, plus the SDL3 library). Use the
`release` preset for an optimized build in `build-release/`.

To reformat all sources: `cmake --build --preset debug --target format`

## Conventions

- Formatting: `.clang-format` (format on save is enabled in VS Code)
- Naming: `.clang-tidy` — PascalCase types/functions, `I`-prefixed interfaces, camelCase locals, `_camelCase` members
- Line endings: LF everywhere (`.gitattributes`), UTF-8 source
