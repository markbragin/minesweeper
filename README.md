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

#### For MinGW

```
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Release -G "MinGW Makefiles" ..
cmake --build .
cd ..
```

---

If **raylib** is installed on your machine in a custom path you can pass
it via `-DRAYLIB_PATH=path/to/raylib` to **cmake**:

```
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Release -DRAYLIB_PATH=lib/ ..
cmake --build .
cd ..
```

Binary will be located in the root directory of the project.


## Changelog

- **v0.2.0**
    - added main menu
- **v0.1.2**
    - added clickface
    - fixed min window size on windows
    - fixed bug when clicking around the grid
- **v0.1.1**
    - added mines generation after first click
    - added ability to start new game in the middle of current one
    - added setting cells down when mouse left key down
    - fixed the display of negative numbers
- **v0.1.0**
    - First bad version
