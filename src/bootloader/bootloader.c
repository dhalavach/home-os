// __asm__(".code16gcc");          // Tell GCC to generate 16-bit code
// __asm__("jmpl $0x0000, $main"); // Jump to main at 0x0000:main for execution

void print_string(const char *str)
{
  while (*str)
  {
    // __asm__ __volatile__(
        "movb $0x0E, %%ah\n" // BIOS teletype function
        "movb %0, %%al\n"    // Load character into AL
        "int $0x10\n"        // Call BIOS interrupt 0x10
        : ""                   // No output
        : "r"(*str)          // Input is character from *str
        : "ah", "al"         // Clobbered registers
    );
        str++;
  }
}

void read_sector(int sector)
{
  (                        //__asm__ __volatile__(
      "movb $0x02, %%ah\n" // BIOS read sector function
      "movb $0x01, %%al\n" // Read 1 sector
      "movb $0x00, %%ch\n" // Cylinder number
      "movb $0x00, %%dh\n" // Head number
      "movb %0, %%cl\n"    // Sector number
      "int $0x13\n"        // Call BIOS interrupt 0x13
      :
      : "r"(sector)
      : "ax", "bx", "cx", "dx" // Clobbered registers
  );
}

void main()
{
  print_string("Loading the OS...\r\n");

  // Load `main.asm` code from the second sector into memory at 0x0000:0x7E00
  //__asm__ __volatile__("movw $0x7E00, %bx"); // ES:BX points to 0x0000:0x7E00
  read_sector(2); // Load from sector 2

  print_string("Running the OS...\r\n");

  // Jump to `main.asm` start at 0x7C00
  //__asm__ __volatile__("jmp 0x0000:0x7C00");
}

// Padding to 510 bytes, and the boot signature 0xAA55
unsigned char padding[510 - 2] = {0};
unsigned short boot_signature = 0xAA55;
