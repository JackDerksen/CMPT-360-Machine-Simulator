#include "svm.h"

int main(void) {
  Processor proc;
  Memory mem;
  int program_size;

  /* Initialize processor and memory */
  init_processor(&proc);
  init_memory(&mem);

  /* Load program file from stdin */
  program_size = load_program(&mem, stdin);
  if (program_size < 0) {
    fprintf(stderr, "Error loading program\n");
    return 1;
  }

  /* Start execution from address 0 */
  proc.pc = 0;
  execute_program(&proc, &mem);

  return 0;
}
