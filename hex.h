#ifndef HEX_H
#define HEX_H

//******************************************************************************
//
// hex.h
//
// CSCI 463 Assignment 5
//
// Author: Gavin St. George (Z1909350)
//
// RISC-V Simulator
//
//******************************************************************************

#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>

/**
 * @brief Class to handle formatting hex values.
 * ****************************************************************************/
class hex
{
    public:
        /**
         * @brief Formats an 8bit value as hex for output.
         * @param i The value to format.
         * @returns A std::string containing the formatted output.
         * ********************************************************************/
        static std::string to_hex8(uint8_t i);

        /**
         * @brief Formats a 32bit value as hex for output.
         * @param i The value to format.
         * @returns A std::string containing the formatted output.
         * ********************************************************************/
        static std::string to_hex32(uint32_t i);

        /**
         * @brief Formats a 12bit value as hex (with leading "0x") for output.
         * @param i The value to format.
         * @returns A std::string containing the formatted output.
         * ********************************************************************/
        static std::string to_hex0x12(uint32_t i);

        /**
         * @brief Formats a 20bit value as hex (with leading "0x") for output.
         * @param i The value to format.
         * @returns A std::string containing the formatted output.
         * ********************************************************************/
        static std::string to_hex0x20(uint32_t i);

        /**
         * @brief Formats a 32bit value as hex (with leading "0x") for output.
         * @param i The value to format.
         * @returns A std::string containing the formatted output.
         * ********************************************************************/
        static std::string to_hex0x32(uint32_t i);
};

#endif
