#include "src/headers/gdt.h"

const uint8_t MAX_COLS = 80;
const uint8_t MAX_ROWS = 25;

void clearVgaBuffer(){
    static uint16_t* vga_address = (uint16_t*)0xb8000;
    for (uint8_t i = 0; i < MAX_ROWS; i++) {
      for (uint8_t j = 0; j < MAX_COLS; j++) {
        vga_address[i * MAX_COLS + j] = ' ';
      }
    }
}

void printf(const char* str) {
    static uint16_t* vga_address = (uint16_t*)0xb8000;

    static uint8_t row = 0, col = 0;
    const unsigned short default_color = 0x07;

    for (int i = 0; str[i] != '\0'; i++) {
        if (str[i] == '\n'){
          row++;
          col = 0;
        }else{
          vga_address[MAX_COLS*row + col] = (default_color << 8) | str[i];
          col++;
        }
        if(col >= MAX_COLS){
          row++;
          if (row == MAX_ROWS){
            clearVgaBuffer();
            row = 0;
          }
          col = 0;
        }
    }
}

typedef void (*constructor)() ;
extern "C" constructor start_ctors;
extern "C" constructor end_ctors;
extern "C" void callConstructors(){
  for(constructor* i = &start_ctors; i != &end_ctors; i++){
    (*i)();
  }
}

extern "C" void kernel_main(const void* multiboot_struct, uint32_t magicnumber){
  clearVgaBuffer();
  printf("Hello World!!\n");
  for (uint8_t i = 0; i < 27; i++) {
    printf("Again\n");
  }
  GlobalDescriptorTable gdt;

  while(1){
    
  };
}
