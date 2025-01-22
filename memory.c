#include "svm.h"

/*
 * Initialize all memory locations to zero
 * mem - pointer to memory structure to initialize
 */
void init_memory(Memory *mem) {
  int i;

  /* Using C89 style loop with declaration outside */
  for (i = 0; i < MEMORY_SIZE; i++) {
    mem->data[i] = 0;
  }
}

/*
 * Read a single byte from memory with bounds checking
 * mem - pointer to memory structure
 * address - memory location to read (0-32767)
 * Returns: byte value at specified address
 * Note: Exits program if address is out of bounds
 */
uint8_t fetch_byte(Memory *mem, uint16_t address) {
  /* Verify address is within memory bounds */
  if (address >= MEMORY_SIZE) {
    fprintf(stderr, "Memory access error: address 0x%04x out of bounds\n",
            address);
    exit(1);
  }
  return mem->data[address];
}

/*
 * Read a 16-bit word from memory with bounds checking
 * Reads two consecutive bytes and combines them (big-endian)
 * mem - pointer to memory structure
 * address - memory location to read (0-32766)
 * Returns: 16-bit word value composed of bytes at address and address+1
 * Note: Exits program if address or address+1 is out of bounds
 */
uint16_t fetch_word(Memory *mem, uint16_t address) {
  /* Check if we can safely read two bytes */
  if (address >= MEMORY_SIZE - 1) {
    fprintf(
        stderr,
        "Memory access error: address 0x%04x out of bounds for word access\n",
        address);
    exit(1);
  }

  /* Read low byte */
  uint16_t low_byte = (uint16_t)mem->data[address];
  /* Read high byte */
  uint16_t high_byte = (uint16_t)mem->data[address + 1] << 8;

  /* Combine bytes into a 16-bit word */
  return high_byte | low_byte;
}

/*
 * Write a single byte to memory with bounds checking
 * mem - pointer to memory structure
 * address - memory location to write (0-32767)
 * value - byte value to write
 * Note: Exits program if address is out of bounds
 */
void store_byte(Memory *mem, uint16_t address, uint8_t value) {
  /* Verify address is within memory bounds */
  if (address >= MEMORY_SIZE) {
    fprintf(stderr, "Memory access error: address 0x%04x out of bounds\n",
            address);
    exit(1);
  }
  mem->data[address] = value;
}

/*
 * Write a 16-bit word to memory with bounds checking
 * Splits word into two bytes and writes them consecutively (big-endian)
 * mem - pointer to memory structure
 * address - memory location to write (0-32766)
 * value - 16-bit word to write
 * Note: Exits program if address or address+1 is out of bounds
 */
void store_word(Memory *mem, uint16_t address, int16_t value) {
  if (address >= MEMORY_SIZE - 1) {
    fprintf(
        stderr,
        "Memory access error: address 0x%04x out of bounds for word access\n",
        address);
    exit(1);
  }

  /* Store low byte first */
  mem->data[address] = value & 0xFF;
  /* Store high byte second */
  mem->data[address + 1] = (value >> 8) & 0xFF;
}

/*
 * Load program from input file into memory
 * Reads binary data from input and stores at start of memory
 * mem - pointer to memory structure
 * input - file pointer to read from (typically stdin)
 * Returns: number of bytes loaded, or -1 if program too large
 */
int load_program(Memory *mem, FILE *input) {
  uint16_t address;
  int c;

  address = 0;

  while ((c = fgetc(input)) != EOF && address < MEMORY_SIZE) {
    mem->data[address] = (uint8_t)c;
    address++;
  }

  /* Make sure the program isn't too large for memory */
  if (address >= MEMORY_SIZE) {
    fprintf(stderr, "Program too large for memory\n");
    return -1;
  }

  return (int)address;
}
