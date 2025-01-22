# Notes

## TODO:

- [ ] Clean up debug statements

## Code organization

- svm.h: Contains all definitions and declarations
- processor.c: Handles instruction execution and processor state
- memory.c: Manages memory access and program loading
- main.c: Program entry point


## To run the program:

1. Build the program: `make all`

2. Create a binary file from Test1.svm: `cat Test1.svm | ./sasm > test1.bin`

3. Run the binary with your simulator: `cat test1.bin | ./svm`

4. Or all in one line: `cat Test1.svm | ./sasm | ./svm`
