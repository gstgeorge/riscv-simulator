//******************************************************************************
//
// cpu_single_hart.cpp
//
// CSCI 463 Assignment 5
//
// Author: Gavin St. George (Z1909350)
//
// RISC-V Simulator
//
//******************************************************************************

#include "cpu_single_hart.h"

void cpu_single_hart::run(uint64_t exec_limit)
{
    regs.set(2, mem.get_size());

    if (exec_limit)
    {
        while (!is_halted() && get_insn_counter() < exec_limit)
        {
            tick();
        }
    }
    else 
    {
        while (!is_halted()) 
        { 
            tick(); 
        }
    }

    if (is_halted())
    {
        std::cout << "Execution terminated. Reason: " << get_halt_reason() << std::endl;
    }

    std::cout << get_insn_counter() << " instructions executed" << std::endl;
}