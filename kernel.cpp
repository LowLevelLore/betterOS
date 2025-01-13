void printf(const char* str) {
    unsigned short* vga_address = (unsigned short*)0xb8000;
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

extern "C" void kernel_main(void* multiboot_struct, unsigned int magicnumber){
  printf("Hello World!! ... We will go further this time !!!");
  while(1){
    
  };
}
