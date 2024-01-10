#ifndef CPU_SINGLE_HART_H
#define CPU_SINGLE_HART_H

//******************************************************************************
//
// cpu_single_hart.h
//
// CSCI 463 Assignment 5
//
// Author: Gavin St. George (Z1909350)
//
// RISC-V Simulator
//
//******************************************************************************

#include "rv32i_hart.h"

/**
 * @brief Class to represent a CPU with a single hart.
 * ********************************************************************/
class cpu_single_hart : public rv32i_hart
{
    public:
        /**
         * @brief Constructor.
         * ********************************************************************/
        cpu_single_hart(memory &mem) : rv32i_hart(mem) { }

        /**
         * @brief 
         * ********************************************************************/
        void run(uint64_t exec_limit);
};

#endif
