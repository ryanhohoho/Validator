#include <inttypes.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>

/*
 * Definitions for y86 test harness assignment.
 *
 * Students write the function y86_check as defined below.
 *
 * state: a pointer to the starting machine state
 * instructions: an array of 1 or more y86 instructions
 * n_inst: the number of instructions in the array
 * simfunc: the simulator being tested
 */

/* y86 Condition codes. */
#define FLAG_O 0x20
#define FLAG_Z 0x40
#define FLAG_S 0x04

/*
 * Our simulator supports no more than 1024 bytes of memory, but unlike
 * in a real processor, we are not required to store our programs in
 * memory. For the most part, we'll use memory here only to hold data.
 * start_addr tells you the y86 address of the first byte of the data
 *	stored in the memory field. So, memory[0] is the byte at address
 *      start_addr (assuming valid_mem > 0).
 * valid_mem indicates how many bytes in the memory field contain valid
 *	data; assume that any addresses greater than or equal to
 *	start_addr + valid_mem are invalid.
 */
typedef struct _y86_state
{
        uint8_t memory[1024];   // Up to 1024 bytes of memory
        uint64_t start_addr;    // Address at which memory starts
        uint64_t valid_mem;     // Number of bytes in memory that
                                // are valid
        uint64_t registers[16]; // Contents of the registers
        uint64_t pc;            // Program counter
        uint8_t flags;          // Holds O, Z, and S flags
} y86_state_t;

typedef struct _y86_inst
{
        uint8_t rA;
        uint8_t rB;
        uint64_t constval;
        char instruction[10];	// nul-terminated
} y86_inst_t;

/*
 * Your tester will be called with a parameter that is a function
 * that implements a y86 simulator. The type of that function is
 * defined below (and it's called a y86sim_func).
 */
typedef void (*y86sim_func)(y86_state_t *, y86_inst_t *, int n_inst);

/****** ENUMERATED TYPES FOR INSTRUCTIONS *****/

/* Here are the enumerated types for each instruction. */
typedef enum _inst_t
{
        I_NOP,
        I_HALT,
        I_RRMOVQ,
        I_IRMOVQ,
        I_RMMOVQ,
        I_MRMOVQ,
        I_PUSHQ,
        I_POPQ,
        I_CALL,
        I_RET,
        I_J,
        I_JEQ,
        I_JNE,
        I_JL,
        I_JLE,
        I_JG,
        I_JGE,
        I_ADDQ,
        I_SUBQ,
        I_MULQ,
        I_MODQ,
        I_DIVQ,
        I_ANDQ,
        I_XORQ,
        I_CMOVEQ,
        I_CMOVNE,
        I_CMOVL,
        I_CMOVLE,
        I_CMOVG,
        I_CMOVGE,
        I_INVALID
} inst_t;

/****** HELPER FUNCTIONS WE PROVIDE TO YOU *****/
/* 
 * Test Simulators that you can use for testing your checker.
 */
void sim_good(y86_state_t *state, y86_inst_t *instructions, int n_inst);
void sim_bad(y86_state_t *state, y86_inst_t *instructions, int n_inst);

/* Produces an easy to read dump of your machine state. */
void dump_state(y86_state_t *state);

/* Loads a test case. */
int load_test_case(const char *name, y86_state_t *state, y86_inst_t **insts, int *n_insts);

/* Maps a string to an enum for each y86 instruction.
   This is already defined for you in utils.o. */
inst_t inst_to_enum(const char *str);

/****** FUNCTIONS YOU MUST WRITE (in tester.c) *****/
/* 
 * Given two y86 machine states, return 1 if they are the same and 0 otherwise.
 */
int is_equal(y86_state_t *s1, y86_state_t *s2);

/*
 * Read an 8-byte item from memory. (Recall that the y86 is little endian.)
 */
int read_quad(y86_state_t *state, uint64_t address, uint64_t *value);

/*
 * Write an 8-byte item to memory. (Recall that the y86 is little endian.)
 */
int write_quad(y86_state_t *state, uint64_t address, uint64_t value);

/*
 * This is the function that implements your simulator validator.
 *
 * y86_check returns 0 if the y86sim_func properly simulates
 * the n_inst instructions described in the instructions array, and
 * non-zero otherwise. Note that even though the simulator should update
 * the value of the program counter as a real program would, it always
 * executes the n_inst instructions sequentially. That is, instructions
 * such as call, ret, or jXX do not change which instruction is to be
 * executed next.
 *
 * Unusual conditions:
 * On halt: stop execution, the state should reflect the last successfully
 *	executed instruction.
 * On an invalid command: stop execution, the state should reflect the last
 * 	executed instruction.
 * On any bad arguments, e.g., invalid register number or access to an
 *	invalid address, divide by 0, stop execution, the state should
 * 	reflect the last successfully executed instruction
 */
int y86_check(y86_state_t *state, y86_inst_t *instructions, int n_inst, y86sim_func simfunc);
