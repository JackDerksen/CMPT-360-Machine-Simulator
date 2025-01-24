#include "svm.h"

/*
 * Initialize the processor
 * Sets all registers and flags to 0
 * proc - Pointer to the processor structure
 */
void init_processor(Processor *proc) {
  proc->data_reg[0] = 0; /* R2 */
  proc->data_reg[1] = 0; /* R1 */
  proc->addr_reg[0] = 0; /* A2 */
  proc->addr_reg[1] = 0; /* A1 */
  proc->pc = 0;
  proc->z_flag = 0;
  proc->n_flag = 0;
  proc->o_flag = 0;
}

/*
 * Update processor flags based on operation result
 * Sets Zero flag if result is 0
 * Sets Negative flag if result is negative
 * Sets Overflow flag if arithmetic overflow occurred
 * proc - Pointer to processor structure
 * result - Result value to check
 */
void update_flags(Processor *proc, int16_t result) {
  proc->z_flag = (result == 0);
  proc->n_flag = (result < 0);
}

/*
 * Verify if opcode is valid
 * opcode - Opcode to verify
 * Returns: 1 if opcode is valid, 0 otherwise
 */
static int is_valid_opcode(uint8_t opcode) {
  if (opcode == HALT)
    return 1;
  if (opcode >= LOAD && opcode <= OUTIC)
    return 1;
  return 0;
}

/*
 * Verify if register number is valid (0-3)
 * reg - Register to verify
 * Returns: 1 if register is valid, 0 otherwise
 */
static int is_valid_register(uint8_t reg) { return reg >= 0 && reg <= 3; }

/*
 * Enormous function which executes a single instruction
 * Fetches, decodes and executes one instruction
 * proc - Pointer ot the processor structure
 * mem - Pointer to the memory structure
 */
void execute_instruction(Processor *proc, Memory *mem) {
  uint16_t curr_pc = proc->pc;
  uint8_t opcode = fetch_byte(mem, curr_pc);
  uint8_t reg_byte = fetch_byte(mem, curr_pc + 1);
  uint8_t reg1 = reg_byte & 0x03;
  uint8_t reg2 = (reg_byte >> 6) & 0x03;
  uint16_t immediate;
  int16_t old_value;

  /* Update PC for next instruction based on instruction type */
  /* Not very elegant, but it works */
  switch (opcode) {
  case LOAD:
  case STORE:
  case JMP:
  case JMPZ:
  case JMPN:
  case JMPO:
  case ADD:
  case SUB:
  case OUT:
  case OUTC:
    proc->pc += 4; /* Instructions with immediate values */
    break;
  default:
    proc->pc += 2; /* All other instructions are 2 bytes */
    break;
  }

  /* Verify opcode validity */
  if (!is_valid_opcode(opcode)) {
    fprintf(stderr, "Error: Invalid opcode 0x%02x at address 0x%04x\n", opcode,
            proc->pc);
    exit(1);
  }

  /* Verify register validity */
  if (!is_valid_register(reg1) || !is_valid_register(reg2)) {
    fprintf(stderr, "Error: Invalid register at address 0x%04x\n", proc->pc);
    exit(1);
  }

  /* Switch cases for each opcode type */
  switch (opcode) {
  case LOAD:
    immediate = fetch_word(mem, curr_pc + 2);
    if (reg1 <= 1) {
      proc->data_reg[reg1] = immediate;
    } else {
      proc->addr_reg[reg1 - 2] = immediate;
    }
    update_flags(proc, immediate);
    break;

  case LOADI:
    if (reg2 <= 1) {
      fprintf(stderr, "Error: LOADI requires address register\n");
      exit(1);
    }
    uint16_t addr = proc->addr_reg[reg2 - 2];
    if (reg1 <= 1) {
      proc->data_reg[reg1] = fetch_word(mem, addr);
    } else {
      proc->addr_reg[reg1 - 2] = fetch_word(mem, addr);
    }
    update_flags(proc, proc->data_reg[reg1]);
    break;

  case STORE:
    immediate = fetch_word(mem, curr_pc + 2);
    store_word(mem, immediate,
               reg1 <= 1 ? proc->data_reg[reg1] : proc->addr_reg[reg1 - 2]);
    update_flags(proc, fetch_word(mem, immediate));
    break;

  case STOREI:
    if (reg2 <= 1) {
      fprintf(stderr, "Error: STOREI requires address register\n");
      exit(1);
    }
    store_word(mem, proc->addr_reg[reg2 - 2],
               reg1 <= 1 ? proc->data_reg[reg1] : proc->addr_reg[reg1 - 2]);
    update_flags(proc, fetch_word(mem, proc->addr_reg[reg2 - 2]));
    break;

  case JMP:
    immediate = fetch_word(mem, proc->pc - 2);
    proc->pc = immediate;
    break;

  case JMPZ:
    immediate = fetch_word(mem, proc->pc - 2);
    if (proc->z_flag) {
      proc->pc = immediate;
    }
    break;

  case JMPN:
    immediate = fetch_word(mem, proc->pc - 2);
    if (proc->n_flag) {
      proc->pc = immediate;
    }
    break;

  case JMPO:
    immediate = fetch_word(mem, proc->pc - 2);
    if (proc->o_flag) {
      proc->pc = immediate;
    }
    break;

  case ADD:
    if (reg1 > 1) {
      fprintf(stderr, "Error: ADD requires data register\n");
      exit(1);
    }
    immediate = fetch_word(mem, proc->pc - 2);
    old_value = proc->data_reg[reg1];
    proc->data_reg[reg1] += immediate;
    /* Detect overflow */
    proc->o_flag =
        (old_value > 0 && immediate > 0 && proc->data_reg[reg1] < 0) ||
        (old_value < 0 && immediate < 0 && proc->data_reg[reg1] > 0);
    update_flags(proc, proc->data_reg[reg1]);
    break;

  case ADDR:
    if (reg1 > 1 || reg2 > 1) {
      fprintf(stderr, "Error: ADDR requires data registers\n");
      exit(1);
    }
    if (reg1 != reg2) {
      proc->data_reg[reg1] =
          (int16_t)proc->data_reg[reg1] + (int16_t)proc->data_reg[reg2];
      update_flags(proc, proc->data_reg[reg1]);
    }
    break;

  case SUB:
    if (reg1 > 1) {
      fprintf(stderr, "Error: SUB requires data register\n");
      exit(1);
    }
    immediate = fetch_word(mem, proc->pc - 2);
    old_value = proc->data_reg[reg1];
    proc->data_reg[reg1] -= immediate;
    /* Detect overflow */
    proc->o_flag =
        (old_value > 0 && immediate < 0 && proc->data_reg[reg1] < 0) ||
        (old_value < 0 && immediate > 0 && proc->data_reg[reg1] > 0);
    update_flags(proc, proc->data_reg[reg1]);
    break;

  case SUBR:
    if (reg1 > 1 || reg2 > 1) {
      fprintf(stderr, "Error: SUBR requires data registers\n");
      exit(1);
    }
    if (reg1 != reg2) {
      old_value = proc->data_reg[reg1];
      proc->data_reg[reg1] -= proc->data_reg[reg2];
      /* Detect overflow */
      proc->o_flag = (old_value > 0 && proc->data_reg[reg2] < 0 &&
                      proc->data_reg[reg1] < 0) ||
                     (old_value < 0 && proc->data_reg[reg2] > 0 &&
                      proc->data_reg[reg1] > 0);
      update_flags(proc, proc->data_reg[reg1]);
    }
    break;

  case OUT:
    immediate = fetch_word(mem, curr_pc - 2);
    printf("%d", immediate);
    break;

  case OUTC:
    immediate = fetch_word(mem, curr_pc + 2);
    printf("%c", (char)(immediate & 0xFF));
    break;

  case OUTR:
    if (reg1 > 1) {
      fprintf(stderr, "Error: OUTR requires data register\n");
      exit(1);
    }
    printf("%d", (int16_t)proc->data_reg[reg1]);
    break;

  case OUTRC:
    if (reg1 > 1) {
      fprintf(stderr, "Error: OUTRC requires data register\n");
      exit(1);
    }
    printf("%c", (char)(proc->data_reg[reg1] & 0xFF));
    break;

  case OUTI:
    if (reg1 <= 1) {
      fprintf(stderr, "Error: OUTI requires address register\n");
      exit(1);
    }
    printf("%d", (int16_t)fetch_word(mem, proc->addr_reg[reg1 - 2]));
    break;

  case OUTIC:
    if (reg1 <= 1) {
      fprintf(stderr, "Error: OUTIC requires address register\n");
      exit(1);
    }
    printf("%c", (char)(fetch_word(mem, proc->addr_reg[reg1 - 2]) & 0xFF));
    break;

  case HALT:
    exit(0);
    break;

  default:
    fprintf(stderr, "Error: Unimplemented opcode 0x%02x\n", opcode);
    exit(1);
  }
}

/*
 * Main execution loop
 * Continuously executes instructions until HALT
 * proc - Pointer to the processor structure
 * mem - Pointer to the memory structure
 */
void execute_program(Processor *proc, Memory *mem) {
  while (1) {
    execute_instruction(proc, mem);
  }
}
