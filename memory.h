#ifndef MEMORY_H
#define MEMORY_H

//******************************************************************************
//
// memory.h
//
// CSCI 463 Assignment 5
//
// Author: Gavin St. George (Z1909350)
//
// RISC-V Simulator
//
//******************************************************************************

#include <vector>
#include <cctype>
#include <fstream>

#include "hex.h"

/**
 * @brief Class to handle reading from/writing to simulated memory.
 * ****************************************************************************/
class memory : public hex
{
    public :
        /**
         * @brief Allocate siz bytes in the mem vector and initialize every 
         *        byte/element to 0xa5.
         * ****************************************************************************/
        memory(uint32_t siz); 

        /**
         * @brief Destructor
         * ****************************************************************************/
        ~memory();

        /**
         * @brief Prints a warning message if an address is out of bounds.
         * @param addr The address to check.
         * @return true if out of bounds.
         * ****************************************************************************/
        bool check_illegal(uint32_t addr) const;

        /**
         * @brief Gets the size of simulated memory.
         * @return The (rounded up) number of bytes in the simulated memory.
         * ****************************************************************************/
        uint32_t get_size() const;

        /**
         * @brief Gets an 8bit value from memory.
         * @param addr The address to check.
         * @return Value at address if address is valid. 
         *         Returns 0 if address is invalid.
         * ****************************************************************************/
        uint8_t get8(uint32_t addr) const;

        /**
         * @brief Gets a 16bit value from memory.
         * @param addr The address to check.
         * @return Value at address if address is valid (in little-endian order).
         *         Returns 0 if address is invalid.
         * ****************************************************************************/
        uint16_t get16(uint32_t addr) const;

        /**
         * @brief Gets a 32bit value from memory.
         * @param addr The address to check.
         * @return Value at address if address is valid (in little-endian order). 
         *         Returns 0 if address is invalid.
         * ****************************************************************************/
        uint32_t get32(uint32_t addr) const;

        /**
         * @brief Extends the sign of an 8bit value.
         * @param addr The address of the value.
         * @return Value at address if address is valid, sign-extended as a signed
         *         32bit integer. Returns 0 if address is invalid.
         * ****************************************************************************/
        int32_t get8_sx(uint32_t addr) const;

        /**
         * @brief Extends the sign of a 16bit value.
         * @param addr The address of the value.
         * @return Value at address if address is valid, sign-extended as a signed
         *          32bit integer. Returns 0 if address is invalid.
         * ****************************************************************************/
        int32_t get16_sx(uint32_t addr) const;

        /**
         * @brief Gets a 32bit value as a signed 32bit integer.
         * @param addr The address of the value.
         * @return Value at address if address is valid, as a signed 32bit integer.
         *         Returns 0 if address is invalid.
         * ****************************************************************************/        
        int32_t get32_sx(uint32_t addr) const;

        /**
         * @brief Writes a 8bit value into memory if address is valid.
         * @param addr The address being written to.
         * @param val The value to write.
         * ****************************************************************************/
        void set8(uint32_t addr, uint8_t val);

        /**
         * @brief Writes a 16bit value into memory if address is valid.
         * @param addr The address being written to.
         * @param val The value to write.
         * ****************************************************************************/
        void set16(uint32_t addr, uint16_t val);

        /**
         * @brief Writes a 32bit value into memory if address is valid.
         * @param addr The address being written to.
         * @param val The value to write.
         * ****************************************************************************/
        void set32(uint32_t addr, uint32_t val);

        /**
         * @brief Formats and dumps the contents of memory.
         * ****************************************************************************/
        void dump() const;

        /**
         * @brief Loads contents of an input file into memory.
         * @param fname File name of the file to load.
         * @returns True if file imported successfully, otherwise false.
         * ****************************************************************************/
        bool load_file(const std::string & fname );

    private:
    
        /**
         * @brief A vector of bytes representing the simulated memory.
         * ****************************************************************************/
        std::vector<uint8_t> mem;
};

#endif
