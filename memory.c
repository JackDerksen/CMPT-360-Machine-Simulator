#include "svm.h"

void init_memory(Memory *mem) {
  for (int i = 0; i < MEMORY_SIZE; i++) {
    mem->data[i] = 0;
  }
}

uint8_t fetch_byte(Memory *mem, uint16_t address);

uint16_t fetch_word(Memory *mem, uint16_t address);

void store_byte(Memory *mem, uint16_t address, uint8_t value);

void store_word(Memory *mem, uint16_t address, uint8_t value);

int load_program(Memory *mem, FILE *input);
