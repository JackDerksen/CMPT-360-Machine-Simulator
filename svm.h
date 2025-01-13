#ifndef SVM_H
#define SVM_H

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

/* 80d201 opcodes */
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

/* 80d201 registers */
#define A1 3
#define A2 2
#define R1 1
#define R2 0

/* Memory size constants */
#define MEMORY_SIZE 32768

/* Processor structure */
typedef struct {
  int16_t data_reg[2]; /* R1, R2 */
  int16_t addr_reg[2]; /* A1, A2 */
  uint16_t pc;         /* Program Counter */
  uint8_t z_flag;      /* Zero Flag */
  uint8_t n_flag;      /* Negative Flag */
  uint8_t o_flag;      /* Overflow Flag */
} Processor;

/* Memory structure */
typedef struct {
  uint8_t data[MEMORY_SIZE];
} Memory;

/* Function prototypes */
void init_processor(Processor *proc);
void init_memory(Memory *mem);

#endif
