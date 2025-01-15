# Notes

## Code organization

- svm.h: Contains all definitions and declarations
- processor.c: Handles instruction execution and processor state
- memory.c: Manages memory access and program loading
- main.c: Program entry point


## Current features

- 16-bit processor with 2 data registers and 2 address registers
- Program counter and status flags (Z, N, O)
- 32KB memory (MEMORY_SIZE = 32768 bytes)
- Support for all basic instructions defined in the spec
- Memory bounds checking
- Error handling for invalid operations


## To build and run the program:

1. Run `make` to build
2. Run `cat program.bin | ./svm` to execute a program


## Still need to:
- [ ] Correct execute_instruction doc comment in header file
- [ ] Finish implementing the core functions (already prototyped)
- [ ] Add the instruction implementations in processor.c
- [ ] Enhance error checking and validation
- [ ] Make sure all this stuff coincides with the "design issues" section of the doc


## *Should*...
- [ ] Add more detailed documentation
- [ ] Implement unit tests
- [ ] Add debugging output capabilities
