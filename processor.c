#include "svm.h"

/*
 * Initialize the processor to its starting state
 * Sets all registers and flags to 0
 * proc - pointer to processor structure to initialize
 */
void init_processor(Processor *proc) {
  proc->data_reg[0] = 0; /* R2 */
  proc->data_reg[1] = 0; /* R1 */

  proc->addr_reg[0] = 0; /* A2 */
  proc->addr_reg[1] = 0; /* A1 */

  /* Set PC to the start of memory */
  proc->pc = 0;

  /* Clear status flags */
  proc->z_flag = 0; /* Zero flag */
  proc->n_flag = 0; /* Negative flag */
  proc->o_flag = 0; /* Overflow flag */
}

/*
 * Update processor flags based on a result value
 * proc - pointer to processor structure
 * result - value to check for setting flags
 */
void update_flags(Processor *proc, int16_t result) {
  proc->z_flag = (result == 0);

  proc->n_flag = (result < 0);

  /* TODO: Complete overflow flag logic (depends on operation) */
  proc->o_flag = 0;
}

/*
 * Execute a single instruction
 * Fetches, decodes, and executes one instruction from memory
 * proc - pointer to processor structure
 * mem - pointer to memory structure
 */
void execute_instruction(Processor *proc, Memory *mem) {
  /* Fetch instruction components */
  uint8_t opcode = fetch_byte(mem, proc->pc);
  uint8_t reg_byte = fetch_byte(mem, proc->pc + 1);

  /* Extract register numbers from reg_byte */
  uint8_t reg1 = reg_byte & 0x03;        /* Bottom two bits, first reg */
  uint8_t reg2 = (reg_byte >> 6) & 0x03; /* Top two bits, second reg */

  /* Used for instructions with immediate values */
  uint16_t immediate;

  /* Update PC based on instruction size */
  /* (imm = 4 bytes, reg = 2 bytes) */
  proc->pc += (opcode >= LOAD && opcode <= OUTIC) ? 4 : 2;

  /* Execute the relevant instruction based on opcode */
  switch (opcode) {
  case LOAD:
    immediate = fetch_word(mem, proc->pc - 2);
    if (reg1 <= 1) {
      /* Load into data register (R1 or R2) */
      proc->data_reg[reg1] = immediate;
    } else {
      /* Load into address register (A1 or A2) */
      proc->addr_reg[reg1 - 2] = immediate;
    }
    update_flags(proc, immediate);
    break;

  case LOADI:
    if (reg1 <= 1) {
      /* Load into data register */
      proc->data_reg[reg1] = fetch_word(mem, proc->addr_reg[reg2 - 2]);
    } else {
      /* Load into address register */
      proc->addr_reg[reg1 - 2] = fetch_word(mem, proc->addr_reg[reg2 - 2]);
    }
    update_flags(proc, proc->data_reg[reg1]);
    break;

  case ADD:
    immediate = fetch_word(mem, proc->pc - 2);
    proc->data_reg[reg1] += immediate;
    update_flags(proc, proc->data_reg[reg1]);
    break;

  case ADDR:
    if (reg1 != reg2) {
      proc->data_reg[reg1] += proc->data_reg[reg2];
      update_flags(proc, proc->data_reg[reg1]);
    }
    break;

  case OUTR:
    printf("%d", proc->data_reg[reg1]);
    break;

  case OUTRC:
    printf("%c", (char)(proc->data_reg[reg1] & 0xFF));
    break;

  case HALT:
    exit(0);
    break;
  }
}

/*
 * Main execution loop of the processor
 * Continuously fetches and executes instructions until HALT
 * proc - pointer to processor structure
 * mem - pointer to memory structure
 */
void execute_program(Processor *proc, Memory *mem) {
  while (1) {
    execute_instruction(proc, mem);
  }
}
