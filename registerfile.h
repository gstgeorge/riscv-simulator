#ifndef REGISTERFILE_H
#define REGISTERFILE_H

//******************************************************************************
//
// registerfile.h
//
// CSCI 463 Assignment 5
//
// Author: Gavin St. George (Z1909350)
//
// RISC-V Simulator
//
//******************************************************************************

#include <vector>
#include <cstdint>
#include <string>
#include "hex.h"

/**
 * @brief Class to store the state of the general-purpose registers of 
 *        one RISC-V hart.
 * ********************************************************************/
class registerfile : hex
{
    private:
        /**
         * @brief Vector to store the state of one hart's registers.
         * ********************************************************************/
        std::vector<int32_t> regs;
    
    public:
        /**
         * @brief Constructor.
         * ********************************************************************/
        registerfile();

        /**
         * @brief Initialize register x0 to zero, and all other registers to 
         *        0xf0f0f0f0.
         * ********************************************************************/
        void reset();

        /**
         * @brief Assign register r the given val. If r is zero then do nothing.
         * @param r The register to be assigned a value.
         * @param val The value to be assigned to the register.
         * ********************************************************************/
        void set(uint32_t r, int32_t val);

        /**
         * @brief Return the value of register r. If r is zero then return zero.
         * @param r The register from which to get a value.
         * ********************************************************************/
        int32_t get(uint32_t r) const;

        /**
         * @brief Dump the contents of the registers to stdout.
         * @param hdr A string to print at the beginning of each line.
         * ********************************************************************/
        void dump(const std::string &hdr) const;
};

#endif
