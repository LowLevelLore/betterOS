#include "src/headers/types.h"
#include "src/headers/gdt.h"

void printf(const char* str) {
    static uint16_t* vga_address = (uint16_t*)0xb8000;
    const unsigned short default_color = 0x07; 
    for (int i = 0; str[i] != '\0'; i++) {
        vga_address[i] = (default_color << 8) | str[i];
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
  printf("Hello World!! ... We will go further this time !!!");
  
  GlobalDescriptorTable gdt;

  while(1){
    
  };
}
