# Writing Local Test Cases

Note: to view an `.md` file in the workspace, you can use the "Open Preview to the Side" icon (a book with a magnifying glass) located at the top right corner.

All test cases are located in the tests folder, in separate directories, each with two files:
- `n.insts` file: contains a sequence of instructions to be executed in the y86 simulator.

- `start.state` file: the initial state of the y86 simulator before any instruction is executed.

For example in the **addq** test case, the simulator will start with the state specified by [`tests/addq/start.state`](tests/add/start.state) and execute the instructions provided in [`tests/addq/n.insts`](tests/add/n.insts). The parsing is done by calling the `load_test_case` function provided in [`tester.h`](tester.h) file with the name **"tests/addq"**.

You may modify/add/delete any test in this directory, with the following guidelines:
 
## general:
- No line should exceed 1024 characters in length.

- You may have empty lines or lines starting with `//` as comments in these files, both of which will be skipped by the parser.

- If your tests are invalid, although we try our best to catch the issue, the parser **MIGHT NOT** necessarily report it.

- Any numerical value could be in the form of a hexadecimal (start with `0x` or `0X`), a decimal or a binary (start with `0b` or `0B`). All but the binary representation could be signed with a leading `-` to indicate a negative number.

## .insts:
- Every line (that is neither a comment nor empty) is a valid instruction following the y86 syntax, which could be verified via the [online simulator](https://www.students.cs.ubc.ca/~cs-313/simulator).

## .state:
- The file must contain all 6 properties of a state: `REGS` for register file, `FLAGS` for flags, `PC` for program counter, `STARTADDR` for the start address of the valid memory, `VALIDMEM` for the range of the valid memory, and `MEM` for the memory starting at `STARTADDR`. Except `MEM`, every other properties must occupy only one line. Please use [`add.state`](tests/add.state) as a reference.

- `REGS`: 15 8-byte numerical values should be provided with space in between.

- `FLAGS`: 3 characters in the order of **'O'** (overflow flag), **'S'** (sign flag) and **'Z'** (zero flag). If any flag is not set, it should be replaced with **'-'**. For example, a state in which only the sign flag is set is represented as `FLAGS: -S-`.

- `PC`: The program counter as an 8-byte numerical value.

- `STARTADDR`: The start address of the valid memory as an 8-byte numerical value.

- `VALID_MEM`: The range of the valid memory as an 8-byte numerical value.

- `MEM`: The data memory starting at `STARTADDR` as a sequence of 1 byte values. Every 2 bytes should be separated with space in between. The recommended number of bytes per line is 16; you may put more as long as it does not exceed the 1024-character line limit.
