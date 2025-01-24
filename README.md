# CMPT 360 Machine Simulator

---

## Overview
This project implements a virtual machine simulator for the fictional StunTel 80d201 processor. The simulator executes binary machine code that can be generated from assembly using the provided `sasm` assembler.


## Architecture
The simulator implements a 16-bit processor with:
- 2 data registers (R1, R2)
- 2 address registers (A1, A2)  
- Program counter (PC)
- Status flags (Z, N, O)
- 32KB memory space (byte-addressable)


## Code organization

- *svm.h*: Contains all definitions and declarations
- *processor.c*: Handles instruction execution and processor state
- *memory.c*: Manages memory access and program loading
- *main.c*: Program entry point


### Example Test Code (test.svm)

```
         LOAD  A1,DATA1   # load address DATA1 into A1
         LOADI R1,A1      # load contents of address in A1 into R1
         LOAD  A2,DATA2   # load address DATA2 into A2
         LOADI R2,A2      # load contents of address in A2 into R2
         OUTI  A1         # output contents of address in A1 as a number
         OUTC  43         # output ASCII character 43 ('+')
         OUTI  A2         # output contents of address in A2 as a number
         OUTC  61         # output ASCII character 61 ('=')
         ADDR  R1,R2      # add contents of register R2 to R1 and store in R1
         STORE R1,RESULT  # store contents of register R1 to address RESULT
         LOAD  A2,RESULT  # load the address of RESULT
         LOADI R2,A2      # load the value of RESULT
         OUTR  R2         # output contents of register R2 as a number
         OUTC 13          # output a carriage return
         OUTC 10          # output a line feed
         HALT             # Adieu, mon ami!
DATA1    DATA  4
DATA2    DATA  3
RESULT   DATA  0
```


### To run the program:

1. Build the program: `make all` (`make clean` to clean)
2. Create a binary file from Test1.svm: `cat test.svm | ./sasm > test.bin`
3. Run the binary with the simulator: `cat test.bin | ./svm`
4. Or all in one line: `cat test.svm | ./sasm | ./svm`


## Implementation Notes

- Memory is byte-addressable with 32KB total space
- Instructions are either 2 or 4 bytes long
- Register encoding uses 2 bits for each register in instruction byte
- Proper bounds checking implemented for all memory access
- Status flags updated after arithmetic and memory operations
- Error detection for invalid opcodes and register numbers


### Error Handling

The simulator detects and reports:

- Invalid opcodes
- Invalid register numbers
- Memory access violations
- Basic format validation
