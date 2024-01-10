//******************************************************************************
//
// memory.cpp
//
// CSCI 463 Assignment 5
//
// Author: Gavin St. George (Z1909350)
//
// RISC-V Simulator
//
//******************************************************************************

#include "memory.h"

// Allocate siz bytes in the mem vector and initialize every byte/element to 0xa5.
memory::memory(uint32_t siz)
{
    siz = (siz+15)&0xfffffff0; // round the length up, mod-16
    mem.resize(siz);

    for (size_t i = 0; i < mem.size(); ++i)
    {
        mem.at(i) = 0xa5;
    }
}

// Destructor
memory::~memory()
{
    mem.clear();
}

 // Prints a warning message if an address is out of bounds.
bool memory::check_illegal(uint32_t addr) const
{
    if (addr >= get_size())
    {
        std::cerr << "WARNING: Address out of range: " << to_hex0x32(addr) << std::endl;
        return true;
    }
    else return false;
}

// Gets the size of simulated memory.
uint32_t memory::get_size() const
{
    return mem.size();
}

// Gets an 8bit value from memory.
uint8_t memory::get8(uint32_t addr) const
{
    return check_illegal(addr) ? 0 : mem.at(addr);
}

// Gets a 16bit value from memory.
uint16_t memory::get16(uint32_t addr) const
{
    return get8(addr) | get8(addr + 1) << 8;
}

// Gets a 32bit value from memory.
uint32_t memory::get32(uint32_t addr) const
{
    return get16(addr) | get16(addr + 2) << 16;
}

// Extends the sign of an 8bit value.
int32_t memory::get8_sx(uint32_t addr) const
{
    return (get8(addr) << 24) >> 24;
}

// Extends the sign of a 16bit value.
int32_t memory::get16_sx(uint32_t addr) const
{
    return (get16(addr) << 16) >> 16;
}

// Gets a 32bit value as a signed 32bit integet.
int32_t memory::get32_sx(uint32_t addr) const
{
    return get32(addr);
}

// Writes a 8bit value into memory if address is valid.
void memory::set8(uint32_t addr, uint8_t val)
{
    if (check_illegal(addr)) { return; }
    else mem.at(addr) = val;
}

// Writes a 16bit value into memory if address is valid.
void memory::set16(uint32_t addr, uint16_t val)
{
    if (check_illegal(addr)) { return; }
    else
    {
        set8(addr, val & 0x00ff);
        set8(addr + 1, val >> 8);
    }
}

// Writes a 32bit value into memory if address is valid.
void memory::set32(uint32_t addr, uint32_t val)
{
    if (check_illegal(addr)) { return; }
    else
    {
        set16(addr, val & 0x0000ffff);
        set16(addr + 2, val >> 16);
    }
}

// Formats and dumps the contents of memory.
void memory::dump() const
{
    for (uint32_t i = 0; i < get_size(); ++i)
    {
        if (i % 16 == 0)
        {
            std::cout << to_hex32(i) << ": ";
        }

        if (i && (i % 8 == 0) && (i % 16 != 0))
        {
            std::cout << " ";
        }

        std::cout << to_hex8(get8(i)) << " ";

        if (i && (i + 1) % 16 == 0)
        {
            std::cout << "*";

            for (uint32_t j = i - 15; j <= i; ++j)
            {
                isprint(get8(j)) ? std::cout << get8(j) : std::cout << ".";
            }

            std::cout << "*" << std::endl;
        }
    }
}

// Loads contents of an input file into memory.
bool memory::load_file(const std::string & fname )
{
    std::ifstream infile(fname, std::ios::in|std::ios::binary);

    if (!infile)
    {
        std::cerr << "Can't open file '" << fname << "' for reading." << std::endl;
        return false;
    }

    uint8_t i;
    infile >> std::noskipws;
    
    for (uint32_t addr = 0; infile >> i; ++addr)
    {
        if (check_illegal(addr))
        {
            std::cerr << "Program too big." << std::endl;
            return false;
        }

        set8(addr, i);
    }

    infile.close();

    return true;
}
