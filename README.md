# Minesweeper

_Powered by [raylib](https://github.com/raysan5/raylib)_

![demo](./demos/demo1.gif "demo")

## Installation

### Make

`make clean; make release`

If **raylib** is installed on your machine in a custom path you can pass
it via `RAYLIB_PATH=path/to/raylib` to **make**:

`make clean; make release RAYLIB_PATH=lib/`

### Cmake

```
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
cmake --build .
cd ..
```
---

If **raylib** is installed on your machine in a custom path you can pass
it via `-DRAYLIB_PATH=path/to/raylib` to **cmake**:

```
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Release .. -DRAYLIB_PATH=lib/
cmake --build .
cd ..
```

Binary will be located in the root directory of the project.
