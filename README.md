## Chip8 Emulator using c

Refer: http://devernay.free.fr/hacks/chip8/C8TECH10.HTM#0.1

An attempt at building a CHIP-8 emulator using C and SDL2.

## Running this locally

### Install dependencies

1) Arch linux:
    ```sh
        sudo pacman -S sdl2
    ```
    
    OR

    Ubuntu:
    ```sh
        sudo apt-get install libsdl2-dev
    ```

### Building
1) Build using gcc

```sh
    gcc emulate.c -lSDL2 -lSDL2main chip8
```

### Running

1) On linux
    ```sh
        ./chip8 <name-of-rom>
    ```

    OR

    WSL on windows
    ```ps
        LIBGL_ALWAYS_SOFTWARE=1 ./chip8 <name-of-rom>
    ```