//******************************************************************************
//
// main.cpp
//
// CSCI 463 Assignment 5
//
// Author: Gavin St. George (Z1909350)
//
// RISC-V Simulator
//
//******************************************************************************

#include <getopt.h>

#include "hex.h"
#include "memory.h"
#include "rv32i_decode.h"
#include "cpu_single_hart.h"

using std::cerr;
using std::cout;
using std::endl;

/**
 * @brief Prints error message if program is launched incorrectly.
 * ****************************************************************************/
static void usage()
{
	cerr << "Usage: rv32i [-d] [-i] [-r] [-z] [-l exec-limit] [-m hex-mem-size] infile" << endl;
	cerr << "    -d show disassembly before program execution" << endl;
	cerr << "    -i show instruction printing during execution" << endl;
	cerr << "    -l maximum number of instructions to exec" << endl;
	cerr << "    -m specify memory size (default = 0x100)" << endl;
	cerr << "    -r show register printing during execution" << endl;
	cerr << "    -z show a dump of the regs & memory after simulation" << endl;
	exit(1);
}

/**
 * @brief Disassembles all instructions in simulated memory.
 * @param mem The simulated memory.
 * ****************************************************************************/
static void disassemble(const memory &mem)
{
	for (uint32_t i = 0; i < mem.get_size(); i += 4)
	{
		cout << hex::to_hex32(i) << ": " << hex::to_hex32(mem.get32(i)) << "  " << rv32i_decode::decode(i, mem.get32(i)) << endl;
	}
}

/**
 * @brief 
 * ****************************************************************************/
int main(int argc, char **argv)
{
	uint32_t memory_limit = 0x100;	// default memory size is 0x100
	uint64_t exec_limit = 0;
	bool show_disassemble = false;
	bool show_instructions = false;
	bool show_registers = false;
	bool dump_hart = false;

	int opt;
	while ((opt = getopt(argc, argv, "dil:m:rz")) != -1)
	{
		switch(opt)
		{
			case 'd':
				show_disassemble = true;
				break;

			case 'i':
				show_instructions = true;
				break;

			case 'l':
				{
					std::istringstream iss(optarg);
					iss >> std::hex >> exec_limit;
				}
				break;

			case 'm':
				{
					std::istringstream iss(optarg);
					iss >> std::hex >> memory_limit;
				}
				break;

			case 'r':
				show_registers = true;
				break;

			case 'z':
				dump_hart = true;
				break;

			default:
				usage();
		}
	}

	if (optind >= argc)
		usage();

	memory mem(memory_limit);

	if (!mem.load_file(argv[optind]))
		usage();

	if (show_disassemble)
	{
		disassemble(mem);
	}

	cpu_single_hart cpu(mem);
	cpu.reset();
	cpu.set_show_instructions(show_instructions);
	cpu.set_show_registers(show_registers);
	cpu.run(exec_limit);

	if (dump_hart)
	{
		cpu.dump();
		mem.dump();
	}

	return 0;
}
