//******************************************************************************
//
// hex.cpp
//
// CSCI 463 Assignment 5
//
// Author: Gavin St. George (Z1909350)
//
// RISC-V Simulator
//
//******************************************************************************

#include "hex.h"

// Formats an 8bit value as hex for output.
std::string hex::to_hex8(uint8_t i)
{
    std::ostringstream os;
    os << std::hex << std::setfill('0') << std::setw(2) << static_cast<uint16_t>(i);
    return os.str();
}

// Formats a 32bit value as hex for output.
std::string hex::to_hex32(uint32_t i)
{
    std::ostringstream os;
    os << std::hex << std::setfill('0') << std::setw(8) << i;
    return os.str();
}

// Formats a 12bit value as hex (with leading "0x") for output.
std::string hex::to_hex0x12(uint32_t i)
{
    std::ostringstream os;
    os << "0x" << std::hex << std::setfill('0') << std::setw(3) << i;
    return os.str();
}

// Formats a 20bit value as hex (with leading "0x") for output.
std::string hex::to_hex0x20(uint32_t i)
{
    std::ostringstream os;
    os << "0x" << std::hex << std::setfill('0') << std::setw(5) << i;
    return os.str();
}

// Formats a 32bit value as hex (with leading "0x") for output.
std::string hex::to_hex0x32(uint32_t i)
{
    return std::string("0x")+to_hex32(i);
}
