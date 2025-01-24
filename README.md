# CMPT 360 Machine Simulator

---

### Code organization

- *svm.h*: Contains all definitions and declarations
- *processor.c*: Handles instruction execution and processor state
- *memory.c*: Manages memory access and program loading
- *main.c*: Program entry point
- *sasm*: Assembler executable for the svm code

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

