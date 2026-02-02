#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "tester.h"

#define REQUIRE(X)                                                                                    \
	if (!(X))                                                                                         \
	{                                                                                                 \
		fprintf(stderr, "Assertion failed: %s. File: %s. Line number: %d\n", #X, __FILE__, __LINE__); \
		printf("TEST_FAILED\n");                                                                      \
		exit(1);                                                                                      \
	}

static y86_state_t create_empty_state()
{
	y86_state_t state;
	state.flags = 0;
	state.pc = 0;
	state.start_addr = 0;
	state.valid_mem = 0;
	for (int i = 0; i < 16; i++)
	{
		state.registers[i] = 0;
	}
	for (int i = 0; i < 1024; i++)
	{
		state.memory[i] = 0;
	}
	return state;
}

#define X 0xAB
#define Y 0xBC

void test_is_equal()
{
	printf("Here come some tests for is_equal. Of course, these are just the\n");
	printf("tests the instructors gave me so far, which I know are incomplete.\n");
	printf("I vow to my beloved instructors to write more test cases, such as\n");
	printf("some for the 'unusual conditions' documented for is_equal and other functions.\n");

	y86_state_t a = create_empty_state();
	y86_state_t b = create_empty_state();
	REQUIRE(is_equal(&a, &b));

	a.pc = X;
	b.pc = Y;
	REQUIRE(!is_equal(&a, &b));

	a.pc = X;
	b.pc = X;
	REQUIRE(is_equal(&a, &b));

	a.valid_mem = X;
	b.valid_mem = Y;
	REQUIRE(!is_equal(&a, &b));

	a.valid_mem = X;
	b.valid_mem = X;
	REQUIRE(is_equal(&a, &b));

	a.start_addr = X;
	b.start_addr = Y;
	REQUIRE(!is_equal(&a, &b));

	a.start_addr = X;
	b.start_addr = X;
	REQUIRE(is_equal(&a, &b));

	a.flags = FLAG_Z;
	b.flags = FLAG_S;
	REQUIRE(!is_equal(&a, &b));

	a.flags = FLAG_Z;
	b.flags = FLAG_Z;
	REQUIRE(is_equal(&a, &b));

	a.memory[0] = X;
	b.memory[0] = Y;
	REQUIRE(!is_equal(&a, &b));

	a.memory[0] = X;
	b.memory[0] = X;
	REQUIRE(is_equal(&a, &b));

	a.registers[0] = X;
	b.registers[0] = Y;
	REQUIRE(!is_equal(&a, &b));

	a.registers[0] = X;
	b.registers[0] = X;
	REQUIRE(is_equal(&a, &b));
}

void test_read_quad()
{
	uint8_t bytes[8] = {0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF};

	y86_state_t a = create_empty_state();
	a.start_addr = 0x30;
	a.valid_mem = 4 * 8;
	for (int i = 0; i < 8; i++)
	{
		a.memory[i] = bytes[i];
	}

	uint64_t value;
	REQUIRE(read_quad(&a, 0x30 + 0 * 8, &value));
	REQUIRE(value == 0xEFCDAB8967452301);

	REQUIRE(read_quad(&a, 0x30 + 1 * 8, &value));
	REQUIRE(read_quad(&a, 0x30 + 2 * 8, &value));
	REQUIRE(read_quad(&a, 0x30 + 3 * 8, &value));

	REQUIRE(read_quad(&a, 0x30 + 2 * 8 + 1, &value));
	REQUIRE(read_quad(&a, 0x30 + 2 * 8 + 2, &value));
	REQUIRE(read_quad(&a, 0x30 + 2 * 8 + 3, &value));

	REQUIRE(!read_quad(&a, 0x30 + 3 * 8 + 1, &value));
	REQUIRE(!read_quad(&a, 0x30 + 3 * 8 + 2, &value));
	REQUIRE(!read_quad(&a, 0x30 + 3 * 8 + 3, &value));

	REQUIRE(!read_quad(&a, 0x30 - 1, &value));
	REQUIRE(!read_quad(&a, 0x30 - 2, &value));
	REQUIRE(!read_quad(&a, 0x30 - 3, &value));

	printf("Test test_read_quad: PASSED\n");
}

void test_write_quad()
{
	uint8_t bytes[8] = {0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF};

	y86_state_t a = create_empty_state();
	a.start_addr = 0x30;
	a.valid_mem = 4 * 8;

	uint64_t value = 0xEFCDAB8967452301;
	REQUIRE(write_quad(&a, 0x30 + 0 * 8, value));

	for (int i = 0; i < 8; i++)
	{
		REQUIRE(a.memory[i] == bytes[i]);
	}

	REQUIRE(write_quad(&a, 0x30 + 1 * 8, value));
	REQUIRE(write_quad(&a, 0x30 + 2 * 8, value));
	REQUIRE(write_quad(&a, 0x30 + 3 * 8, value));

	REQUIRE(!write_quad(&a, 0x30 + 3 * 8 + 1, value));
	REQUIRE(!write_quad(&a, 0x30 + 3 * 8 + 2, value));
	REQUIRE(!write_quad(&a, 0x30 + 3 * 8 + 3, value));

	REQUIRE(!write_quad(&a, 0x30 - 1, value));
	REQUIRE(!write_quad(&a, 0x30 - 2, value));
	REQUIRE(!write_quad(&a, 0x30 - 3, value));

	printf("Test test_write_quad: PASSED\n");
}

void test_y86_check(const char *name)
{
	y86_state_t state;
	y86_inst_t *instructions;
	int n_inst;

	REQUIRE(load_test_case(name, &state, &instructions, &n_inst) == 0);
	REQUIRE(y86_check(&state, instructions, n_inst, sim_good) == 0);
	REQUIRE(load_test_case(name, &state, &instructions, &n_inst) == 0);
	REQUIRE(y86_check(&state, instructions, n_inst, sim_bad) != 0);
	printf("Test test_y86_check %s: PASSED\n", name);
}

int main(int argc, char *argv[])
{

	test_is_equal();
	test_read_quad();
	test_write_quad();

	test_y86_check("tests/halt");
	test_y86_check("tests/irmovq");
	test_y86_check("tests/rrmovq");
	test_y86_check("tests/addq1");
	test_y86_check("tests/addq2");
	test_y86_check("tests/addq3");
	test_y86_check("tests/subq1");
	test_y86_check("tests/subq2");
	test_y86_check("tests/subq3");
	test_y86_check("tests/cmovle_good1");
	test_y86_check("tests/cmovle_good2");
	test_y86_check("tests/cmovle_bad");
	test_y86_check("tests/jmp");
	test_y86_check("tests/jl_good");
	test_y86_check("tests/jl_bad");
	test_y86_check("tests/mrmovq");
	test_y86_check("tests/rmmovq");
	test_y86_check("tests/pushq");
	test_y86_check("tests/popq");
	test_y86_check("tests/call");
	test_y86_check("tests/ret");

}
