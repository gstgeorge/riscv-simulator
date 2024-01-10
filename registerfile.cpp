//******************************************************************************
//
// registerfile.cpp
//
// CSCI 463 Assignment 5
//
// Author: Gavin St. George (Z1909350)
//
// RISC-V Simulator
//
//******************************************************************************

#include "registerfile.h"

// Constructor.
registerfile::registerfile()
{
    regs.resize(32);
    reset();
}

// Initialize register x0 to zero, and all other registers to 0xf0f0f0f0.
void registerfile::reset()
{
    regs.at(0) = 0;
    for (size_t i = 1; i < regs.size(); ++i)
    {
        regs.at(i) = 0xf0f0f0f0;
    }
}

// Assign register r the given val. If r is zero then do nothing.
void registerfile::set(uint32_t r, int32_t val)
{
    if (r)
    {
        regs.at(r) = val;
    }
}

// Return the value of register r. If r is zero then return zero.
int32_t registerfile::get(uint32_t r) const
{
    if (r)
    {
        return regs.at(r);
    }
    else return 0;
}

// Dump the contents of the registers to stdout.
void registerfile::dump(const std::string &hdr) const
{
    for (size_t i = 0; i < regs.size(); ++i)
    {
        if (i % 8 == 0) // new line: print header & reg num
        {
            if (i) { std::cout << std::endl; }
            std::cout << hdr << std::right << std::setw(3) << "x" + std::to_string(i);
        }

        if (i && i % 4 == 0 && i % 8 != 0) // space gap
        {
            std::cout << " ";
        }

        std::cout << " " << to_hex32(regs.at(i)); // print reg contents
    }
    std::cout << std::endl;
}
