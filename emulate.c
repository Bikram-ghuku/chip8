// Following http://devernay.free.fr/hacks/chip8/C8TECH10.HTM#0.1

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include<string.h>
#include<SDL2/SDL.h>
#include<SDL2/SDL_render.h>
#include <SDL2/SDL_timer.h>

// Memory
uint8_t ram[4096] = {0};

// Register
uint8_t V[16] = {0};
uint16_t I = 0;
uint8_t delay = 0, sound = 0;
uint16_t pc = 0;
uint16_t stack[16] = {0};
uint16_t *sp = stack;

// Keyboard
uint32_t keypad[16] = {0};

// Display
uint32_t display[64 * 32] = {0};

// internal
int cont = 1;

//font
const char font[] = {
    0xF0, 0x90, 0x90, 0x90, 0xF0,
    0x20, 0x60, 0x20, 0x20, 0x70,
    0xF0, 0x10, 0xF0, 0x80, 0xF0,
    0xF0, 0x10, 0xF0, 0x10, 0xF0,
    0x90, 0x90, 0xF0, 0x10, 0x10,
    0xF0, 0x80, 0xF0, 0x10, 0xF0,
    0xF0, 0x80, 0xF0, 0x90, 0xF0,
    0xF0, 0x10, 0x20, 0x40, 0x40,
    0xF0, 0x90, 0xF0, 0x90, 0xF0,
    0xF0, 0x90, 0xF0, 0x10, 0xF0,
    0xF0, 0x90, 0xF0, 0x90, 0x90,
    0xE0, 0x90, 0xE0, 0x90, 0xE0,
    0xF0, 0x80, 0x80, 0x80, 0xF0,
    0xE0, 0x90, 0x90, 0x90, 0xE0,
    0xF0, 0x80, 0xF0, 0x80, 0xF0, 
    0xF0, 0x80, 0xF0, 0x80, 0x80 
};

void handleEvent(){
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch(event.type){
            case SDL_QUIT: cont = 0; break;
            case SDL_KEYDOWN:
                switch(event.key.keysym.scancode){
                    case SDL_SCANCODE_0: keypad[0] = 1; break;
                    case SDL_SCANCODE_1: keypad[1] = 1; break;
                    case SDL_SCANCODE_2: keypad[2] = 1; break;
                    case SDL_SCANCODE_3: keypad[3] = 1; break;

                    case SDL_SCANCODE_Q: keypad[4] = 1; break;
                    case SDL_SCANCODE_W: keypad[5] = 1; break;
                    case SDL_SCANCODE_E: keypad[6] = 1; break;
                    case SDL_SCANCODE_R: keypad[7] = 1; break;

                    case SDL_SCANCODE_A: keypad[8] = 1; break;
                    case SDL_SCANCODE_S: keypad[9] = 1; break;
                    case SDL_SCANCODE_D: keypad[10] = 1; break;
                    case SDL_SCANCODE_F: keypad[11] = 1; break;

                    case SDL_SCANCODE_Z: keypad[12] = 1; break;
                    case SDL_SCANCODE_X: keypad[13] = 1; break;
                    case SDL_SCANCODE_C: keypad[14] = 1; break;
                    case SDL_SCANCODE_V: keypad[15] = 1; break;

                    default: break;
                }
                break;

            case SDL_KEYUP:
                switch(event.key.keysym.scancode){
                    case SDL_SCANCODE_0: keypad[0] = 0; break;
                    case SDL_SCANCODE_1: keypad[1] = 0; break;
                    case SDL_SCANCODE_2: keypad[2] = 0; break;
                    case SDL_SCANCODE_3: keypad[3] = 0; break;

                    case SDL_SCANCODE_Q: keypad[4] = 0; break;
                    case SDL_SCANCODE_W: keypad[5] = 0; break;
                    case SDL_SCANCODE_E: keypad[6] = 0; break;
                    case SDL_SCANCODE_R: keypad[7] = 0; break;

                    case SDL_SCANCODE_A: keypad[8] = 0; break;
                    case SDL_SCANCODE_S: keypad[9] = 0; break;
                    case SDL_SCANCODE_D: keypad[10] = 0; break;
                    case SDL_SCANCODE_F: keypad[11] = 0; break;

                    case SDL_SCANCODE_Z: keypad[12] = 0; break;
                    case SDL_SCANCODE_X: keypad[13] = 0; break;
                    case SDL_SCANCODE_C: keypad[14] = 0; break;
                    case SDL_SCANCODE_V: keypad[15] = 0; break;

                    default: break;
                }
                break;

            default: break;
        }
    }
}

void clear_screen(SDL_Renderer *rendrer){
    SDL_SetRenderDrawColor(rendrer, 0, 0, 255, 100);
    SDL_RenderClear(rendrer);
}

int main(int argc, char** argv){

    srand(time(0));

    if (argc < 2) {
        printf(" NO ROM PASSED\n");
        exit(EXIT_FAILURE);
    }

    FILE *file = fopen(argv[1], "rb");

    if (!file) {
        printf(" ROM File is invalid\n");
    }

    SDL_Init(SDL_INIT_EVERYTHING);

    SDL_Window* win = SDL_CreateWindow(strcat(argv[1], " - Chip8"), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 320, 0);
    SDL_Renderer* rendered = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);

    clear_screen(rendered);
    while(cont){
        SDL_Delay(1000 / 60);

        handleEvent();

        if(delay > 0) delay--;
        if(sound > 0) sound--;
    }


    SDL_DestroyRenderer(rendered);
    SDL_DestroyWindow(win);
    SDL_Quit();
    return 0;
}