#include "svm.h"

void init_processor(Processor *proc) {
  proc->data_reg[0] = proc->data_reg[1] = 0;
  proc->addr_reg[0] = proc->addr_reg[1] = 0;
  proc->pc = 0;
  proc->z_flag = proc->n_flag = proc->o_flag = 0;
}

/* update_flags */

/* execute_instruction */

void execute_program(Processor *proc, Memory *mem) {
  while (1) {
    /* execute_instruction(proc, mem); */
  }
}
