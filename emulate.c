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
int draw = 1;
#define WINDOW_HEIGHT 320
#define WINDOW_WIDTH 640

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

void do_instruct(){
    uint16_t op_code = ram[pc] << 8 | ram[pc + 1];
    printf("0x%04X\n", op_code);
    uint8_t type = (op_code & 0xF000) >> 12;
    switch(type){
        case 0x0:
            switch(op_code & 0x000F){
                case 0x0000: // CLS
                    for (int i = 0; i < 64 * 32; i++) display[i] = 0;
                    draw = 1;
                    pc += 2;
                    break;
                case 0x000E: // RET
                    pc = *sp;
                    sp--;
                    break;
                default:
                    break;
            }
            break;

        case 0x1:  // (1nnn) JP addr - Jump to location nnn.
            pc = (op_code & 0x0FFF); break;

        case 0x2: // (2nnn) CALL addr - Call subroutine at nnn.
            sp++;
            *sp = pc + 2;
            pc = (op_code & 0x0FFF);
            break;

        case 0x3: // SE Vx, byte
            int x = (op_code & 0x0F00) >> 8;
            int kk = (op_code & 0x00FF);
            if(V[x] == kk) pc += 2;
            pc += 2;
            break;

        case 0x4: // SNE Vx, byte
            x = (op_code & 0x0F00) >> 8;
            kk = (op_code & 0x00FF);
            if(V[x] != kk) pc += 2;
            pc += 2;
            break; 

        case 0x5: // SE Vx, Vy
            x = (op_code & 0x0F00) >> 8;
            int y = (op_code & 0x00F0) >> 4;
            if(V[x] == V[y]) pc += 2;
            pc += 2;
            break;

        case 0x6: // LD Vx, byte
            x = (op_code & 0x0F00) >> 8;
            kk = (op_code & 0x00FF);
            V[x] = kk;
            pc += 2;break;

        case 0x7: // ADD Vx, byte
            x = (op_code & 0x0F00) >> 8;
            kk = (op_code & 0x00FF);
            V[x] += kk;
            pc += 2; break;

        case 0x8:
            x = (op_code & 0x0F00) >> 8;
            y = (op_code & 0x00F0) >> 4;
            switch (op_code & 0x000F){
                case 0x0: // LD Vx, Vy
                    V[x] = V[y];
                    pc += 2; break;

                case 0x1: // OR Vx, Vy
                    V[x] |= V[y];
                    pc += 2;break;

                case 0x2: // AND Vx, Vy
                    V[x] &= V[y];
                    pc += 2;break;

                case 0x3: // XOR Vx, Vy
                    V[x] ^= V[y];
                    pc += 2;break;

                case 0x4: // ADD Vx, Vy
                    V[x] += V[y];
                    if(V[x] > 255) V[0xF] = 1;
                    else V[0xF] = 0;
                    V[x] &= 0xFF;
                    pc += 2;break;

                case 0x5:// SUB Vx, Vy
                    if (V[x] > V[y]) V[0xF] = 1;
                    else V[0xF] = 0;
                    V[x] -= V[y];
                    pc += 2; break;

                case 0x6: // SHR Vx {, Vy}
                    V[0xF] = V[x] & 0x1;
                    V[x] /= 2;
                    pc += 2; break;

                case 0x7: // SUBN Vx, Vy
                    if(V[y] > V[x]) V[0xF] = 1;
                    else V[0xF] = 0;
                    V[x] -= V[y];
                    pc += 2; break;

                case 0xE: // SHL Vx {, Vy}
                    V[0xF] = (V[x] & 0x80) >> 7;
                    V[x] *= 2;
                    pc += 2; break;

                default: break;
            }
            break;

        case 0x9: // SNE Vx, Vy
            x = (op_code & 0x0F00) >> 8;
            y = (op_code & 0x00F0) >> 4;
            pc += 2;
            if(V[x] != V[y]) pc += 2;
            break;

        case 0xA: // LD I, addr
            int nnn = (op_code & 0x0FFF);
            I = nnn;
            pc += 2;
            break;

        case 0xB: // JP V0, addr
            nnn = (op_code & 0x0FFF);
            pc = V[0] + nnn;
            break;
        
        case 0xC: // RND Vx, byte
            x = (op_code & 0x0F00) >> 8;
            kk = (op_code & 0x00FF);
            uint8_t random = rand();
            V[x] = random & kk;
            pc += 2;
            break;

        case 0xD: // DRW Vx, Vy, nibble
            x = (op_code & 0x0F00) >> 8;
            y = (op_code & 0x00F0) >> 4;
            int n = (op_code & 0x000F);
            for(int i = 0; i < n; i++){
                uint8_t pixel8 = ram[I + i];
                for (int j = 0; j < 8; j++) {
                    if ((display[(V[y] + i) * 64 + V[x] + j] == 1) &&(pixel8 >> (7 - j) == 1))  V[0xF] = 1;
                    display[(V[y] + i) * 64 + V[x] + j] ^= (pixel8 >> (7 - j)) & 1;
                }
            }
            draw = 1;
            pc += 2;
            break;

        case 0xE:
            x = (op_code & 0x0F00);
            switch(op_code & 0x000F){
                case 0x000E: // SKP Vx
                    if(keypad[V[x]] == 1) pc += 2;
                    pc += 2;
                    break;
                case 0x0001: // SKNP Vx
                    if(keypad[V[x]] == 0) pc += 2;
                    pc += 2;
                    break;
                default: break;
            }
            break;

        case 0xF:
            x = (op_code & 0x0F00) >> 8;
            switch(op_code & 0x00FF){
                case 0x0007: // LD Vx, DT
                    V[x] = delay; 
                    pc += 2;
                    break;

                case 0x000A: // LD Vx, K
                    for(int i = 0; i < 16; i++){
                        if(keypad[i] == 1){
                            V[x] = i;
                            pc += 2;
                            break;
                        }
                    }
                    break;

                case 0x0015: // LD DT, Vx
                    delay = V[x];
                    pc += 2;
                    break;

                case 0x0018: // LD ST, Vx
                    sound = V[x];
                    pc += 2;
                    break;
                
                case 0x001E: // ADD I, Vx
                    I += V[x];
                    pc += 2;
                    break;

                case 0x0029: // LD F, Vx
                    I = ram[5 * V[x]];
                    pc += 2;
                    break;

                case 0x0033: // LD B, Vx
                    ram[I + 2] = V[x] % 10;
                    ram[I + 1] = (V[x] / 10) % 10;
                    ram[I] = (V[x] / 100);
                    pc += 2;
                    break;

                case 0x0055: // LD [I], Vx
                    for(int i = 0; i <= x; i++) ram[I + i] = V[i];
                    pc += 2;
                    break;

                case 0x0065: // LD Vx, [I]
                    for(int i = 0; i <= x; i++) V[i] = ram[I + i];
                    pc += 2;
                    break;

                default: break;
            }
            break;
        
        default: break;
    }
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

    size_t read_bytes = fread(ram + 0x200, 1, sizeof(ram) - 0x200, file);
    for(int i = 0x200; i < read_bytes + 0x200; i += 1){
        printf("memory[%03X] = %02X \n", i, ram[i]);
    }

    for (int i = 0; i < sizeof(font); i++)  ram[i] = font[i];

    pc = 0x200;

    SDL_Init(SDL_INIT_EVERYTHING);

    SDL_Window* win = SDL_CreateWindow(strcat(argv[1], " - Chip8"), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, 0);
    SDL_Renderer* rendered = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);

    //clear_screen(rendered);

    while(cont){
        for(int i = 0; i < 60; i++) do_instruct();
        
        SDL_Delay(1000 / 60);
        if(draw == 1){
            for (int i = 0; i < 64 * 32; i++) {
                SDL_Rect pixel = {.x = (i % 64) * (WINDOW_WIDTH / 64), .y = (i / 64) * (WINDOW_HEIGHT / 32), .w = WINDOW_WIDTH / 64, .h = WINDOW_HEIGHT / 32};
                if (display[i]) {
                    SDL_SetRenderDrawColor(rendered, 255, 255, 255, 255);
                    SDL_RenderFillRect(rendered, &pixel);
                } else {
                    SDL_SetRenderDrawColor(rendered, 0, 0, 255, 0);
                    SDL_RenderFillRect(rendered, &pixel);
                }
            }
            
            
            SDL_RenderPresent(rendered);

            draw = 0;

        }

        handleEvent();

        if(delay > 0) delay--;
        if(sound > 0) sound--;
    }


    SDL_DestroyRenderer(rendered);
    SDL_DestroyWindow(win);
    SDL_Quit();
    return 0;
}