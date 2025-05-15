## Chip8 Emulator using c

<p align="center">
  <img src="https://github.com/user-attachments/assets/57921802-f965-49aa-8afc-a94da907dff7" alt="CHIP-8 Emulator Banner"/>
  
</p>
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
        gcc emulate.c -lSDL2 -lSDL2main -o chip8
    ```

### Running

1) On linux
    ```sh
        ./chip8 <name-of-rom>
    ```

    OR

    WSL on windows
    ```ps
        export LIBGL_ALWAYS_SOFTWARE=1 && ./chip8 'name-of-rom'
    ```
