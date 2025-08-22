# 2D Menger Sponge

This project is a simple interactive visualization of the Menger Sponge fractal using [SDL3](https://github.com/libsdl-org/SDL) for cross-platform graphics.
![Recorded Demo](docs/recording.gif)

## Features

- Real-time rendering of the Menger Sponge fractal.
- Adjustable fractal depth using mouse wheel or number keys (`0`-`9`).
- Smooth color transitions animated over time.
- Resizable window.

## Controls

- **Mouse Wheel:** Increase/decrease fractal depth.
- **Number Keys (0-9):** Set fractal depth directly.
- **Window Resize:** The fractal adapts to the new window size.

## Live Demo

[Web Demo](https://gnulut.github.io/simple-2d-menger-sponge/)

## Download (Windows)

(WIP)

## Building
### Build Requirement
- C++ Compiler (Recommended C++20 and newer)
- git
- cmake (Recommended cmake 3.16 and newer)

### Build commands

```shell
# You need to clone with submodules, otherwise SDL will not download.
git clone https://github.com/Gnulut/simple-2d-menger-sponge --depth=1 --recurse-submodules
cd simple-2d-menger-sponge
cmake -S . -B build
cmake --build build
```

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.
---

*Made with SDL3 and C++*