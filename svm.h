#ifndef SVM_H
#define SVM_H

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

/* Opcodes */
#define HALT 0x31
#define LOAD 0x60
#define LOADI 0x61
#define STORE 0x62
#define STOREI 0x63
#define JMP 0x64
#define JMPZ 0x65
#define JMPN 0x66
#define JMPO 0x67
#define ADD 0x68
#define ADDR 0x69
#define SUB 0x6a
#define SUBR 0x6b
#define OUT 0x6c
#define OUTC 0x6d
#define OUTR 0x6e
#define OUTRC 0x6f
#define OUTI 0x70
#define OUTIC 0x71

/* Registers */
#define A1 3
#define A2 2
#define R1 1
#define R2 0

#define MEMORY_SIZE 32768

/*
 * Processor structure - represents the state of the 80d201 CPU
 * Contains data registers (R1, R2), address registers (A1, A2),
 * program counter (PC), and status flags (Z, N, O)
 */
typedef struct {
  int16_t data_reg[2]; /* R1, R2 */
  int16_t addr_reg[2]; /* A1, A2 */
  uint16_t pc;         /* Program Counter */
  uint8_t z_flag;      /* Zero Flag */
  uint8_t n_flag;      /* Negative Flag */
  uint8_t o_flag;      /* Overflow Flag */
} Processor;

/*
 * Memory structure - represents the 32KB memory space
 * Each byte is individually addressable
 */
typedef struct {
  uint8_t data[MEMORY_SIZE];
} Memory;

/*
 * Initialize processor state
 * Sets all registers to 0, clears flags, sets PC to 0
 * proc - pointer to processor structure to initialize
 */
void init_processor(Processor *proc);

/*
 * Initialize memory state
 * Sets all memory locations to 0
 * mem - pointer to memory structure to initialize
 */
void init_memory(Memory *mem);

/*
 * Load program from input stream into memory
 * Reads binary data starting at memory address 0
 * mem - pointer to memory structure
 * input - file pointer to read from (typically stdin)
 * Returns: number of bytes loaded or -1 on error
 */
int load_program(Memory *mem, FILE *input);

/*
 * Main execution loop
 * Fetches and executes instructions until HALT or error
 * proc - pointer to processor state
 * mem - pointer to memory state
 */
void execute_program(Processor *proc, Memory *mem);

/*
 * Core instruction execution
 * Fetches opcode and registers from memory
 * proc - pointer to processor state
 * mem - pointer to memory state
 */
void execute_instruction(Processor *proc, Memory *mem);

/*
 * Read a single byte from memory
 * Includes bounds checking for memory access
 * mem - pointer to memory structure
 * address - memory address to read (0-32767)
 * Returns: byte value at specified address
 */
uint8_t fetch_byte(Memory *mem, uint16_t address);

/*
 * Read a 16-bit word from memory
 * Reads two consecutive bytes and combines them
 * mem - pointer to memory structure
 * address - memory address to read (0-32766)
 * Returns: 16-bit word value at specified address
 */
uint16_t fetch_word(Memory *mem, uint16_t address);

/*
 * Write a single byte to memory
 * Includes bounds checking for memory access
 * mem - pointer to memory structure
 * address - memory address to write (0-32767)
 * value - byte value to write
 */
void store_byte(Memory *mem, uint16_t address, uint8_t value);

/*
 * Write a 16-bit word to memory
 * Splits word into two bytes and writes them consecutively
 * mem - pointer to memory structure
 * address - memory address to write (0-32766)
 * value - 16-bit word to write
 */
void store_word(Memory *mem, uint16_t address, uint8_t value);

/*
 * Update processor flags based on operation result
 * Sets Zero flag if result is 0
 * Sets Negative flag if result is negative
 * Sets Overflow flag if arithmetic overflow occurred
 * proc - pointer to processor structure
 * result - result value to check
 */
void update_flags(Processor *proc, int16_t result);

#endif
