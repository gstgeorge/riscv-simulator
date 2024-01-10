#ifndef RV32I_HART_H
#define RV32I_HART_H

//******************************************************************************
//
// rv32i_hart.h
//
// CSCI 463 Assignment 5
//
// Author: Gavin St. George (Z1909350)
//
// RISC-V Simulator
//
//******************************************************************************

#include "rv32i_decode.h"
#include "registerfile.h"
#include "memory.h"

/**
 * @brief Class to represent the execution unit of a RV32I hart.
 * ********************************************************************/
class rv32i_hart : public rv32i_decode
{
    public:
        /**
         * @brief Constructor.
         * ********************************************************************/
        rv32i_hart(memory &m) : mem(m) { }

        /**
         * @brief Mutator for show_instructions. 
         * 
         * When true, show each instruction that is executed with a comment 
         * displaying the register values used.
         * 
         * @param b Value to set show_instructions.
         * ********************************************************************/
        void set_show_instructions(bool b) { show_instructions = b; }

        /**
         * @brief Mutator for show_registers.
         * 
         * When true, dump the registers before instruction is executed.
         * 
         * @param b Value to set show_registers.
         * ********************************************************************/
        void set_show_registers(bool b) { show_registers = b; }

        /**
         * @brief Accessor for halt. 
         * @return true if the hart has been halted for any reason.
         * ********************************************************************/        
        bool is_halted() const { return halt; }

        /**
         * @brief The reason the hart has been halted.
         * @return A string indicating the reason the hart has been halted.
         * ********************************************************************/
        const std::string &get_halt_reason() const { return halt_reason; }

        /**
         * @brief Reset the rv32i object and the registerfile.
         * ********************************************************************/
        void reset();
        
        /**
         * @brief Dump the entire state of the hart.
         * @param hdr A string to print at the beginning of each line.
         * ********************************************************************/
        void dump(const std::string &hdr="") const;

        /**
         * @brief Accessor for insn_counter.
         * @return The number of instructions that have been executed by the 
         *         simulator since the last reset().
         * ********************************************************************/
        uint64_t get_insn_counter() const { return insn_counter; }

        /**
         * @brief Mutator for mhartid. 
         * 
         * This is used to set the ID value to be returned by the csrrs 
         * instruction for CSR register number 0xf14. (This will always be zero 
         * on processors that only havea single-hart.)
         * 
         * @param i The ID value.
         * ********************************************************************/
        void set_mhardid(int i) { mhartid = i; }

        /**
         * @brief Tells the simulator to execute an instruction.
         * @param hdr Printed on the left of any and all output that is 
         *        displayed as a result of calling this method.
         * ********************************************************************/
        void tick(const std::string &hdr="");

    private:
        /**
         * @brief 
         * ********************************************************************/
        static constexpr int instruction_width = 35;

        /**
         * @brief Execute the given RV32I instruction.
         * @param insn The instruction to execute.
         * @param pos The output stream.
         * ********************************************************************/
        void exec(uint32_t insn, std::ostream* pos);

        /**
         * @brief Execute illegal instruction.
         * @param insn The instruction to execute.
         * @param pos The output stream.
         * ********************************************************************/
        void exec_illegal_insn(uint32_t insn, std::ostream* pos);

        /**
         * @brief Execute lui instruction.
         * @param insn The instruction to execute.
         * @param pos The output stream.
         * ********************************************************************/
        void exec_lui(uint32_t insn, std::ostream* pos);

        /**
         * @brief Execute auipc instruction.
         * @param insn The instruction to execute.
         * @param pos The output stream.
         * ********************************************************************/
        void exec_auipc(uint32_t insn, std::ostream* pos);

        /**
         * @brief Execute jal instruction.
         * @param insn The instruction to execute.
         * @param pos The output stream.
         * ********************************************************************/
        void exec_jal(uint32_t insn, std::ostream* pos);

        /**
         * @brief Execute jalr instruction.
         * @param insn The instruction to execute.
         * @param pos The output stream.
         * ********************************************************************/
        void exec_jalr(uint32_t insn, std::ostream* pos);

        /**
         * @brief Execute btype instruction.
         * @param insn The instruction to execute.
         * @param pos The output stream.
         * ********************************************************************/
        void exec_btype(uint32_t insn, std::ostream *pos);

        /**
         * @brief Execute load instruction.
         * @param insn The instruction to execute.
         * @param pos The output stream.
         * ********************************************************************/
        void exec_load_imm(uint32_t insn, std::ostream* pos);

        /**
         * @brief Execute stype instruction.
         * @param insn The instruction to execute.
         * @param pos The output stream.
         * ********************************************************************/
        void exec_stype(uint32_t insn, std::ostream* pos);

        /**
         * @brief Execute addi instruction.
         * @param insn The instruction to execute.
         * @param pos The output stream.
         * ********************************************************************/
        void exec_addi(uint32_t insn, std::ostream* pos);

        /**
         * @brief Execute slti instruction.
         * @param insn The instruction to execute.
         * @param pos The output stream.
         * ********************************************************************/
        void exec_slti(uint32_t insn, std::ostream* pos);

        /**
         * @brief Execute sltiu instruction.
         * @param insn The instruction to execute.
         * @param pos The output stream.
         * ********************************************************************/
        void exec_sltiu(uint32_t insn, std::ostream* pos);
        
        /**
         * @brief Execute xori instruction.
         * @param insn The instruction to execute.
         * @param pos The output stream.
         * ********************************************************************/
        void exec_xori(uint32_t insn, std::ostream* pos);

        /**
         * @brief Execute ori instruction.
         * @param insn The instruction to execute.
         * @param pos The output stream.
         * ********************************************************************/
        void exec_ori(uint32_t insn, std::ostream* pos);

        /**
         * @brief Execute andi instruction.
         * @param insn The instruction to execute.
         * @param pos The output stream.
         * ********************************************************************/
        void exec_andi(uint32_t insn, std::ostream* pos);
        
        /**
         * @brief Execute slli instruction.
         * @param insn The instruction to execute.
         * @param pos The output stream.
         * ********************************************************************/
        void exec_slli(uint32_t insn, std::ostream* pos);

        /**
         * @brief Execute srli instruction.
         * @param insn The instruction to execute.
         * @param pos The output stream.
         * ********************************************************************/
        void exec_srli(uint32_t insn, std::ostream* pos);

        /**
         * @brief Execute srai instruction.
         * @param insn The instruction to execute.
         * @param pos The output stream.
         * ********************************************************************/
        void exec_srai(uint32_t insn, std::ostream* pos);

        /**
         * @brief Execute add instruction.
         * @param insn The instruction to execute.
         * @param pos The output stream.
         * ********************************************************************/
        void exec_add(uint32_t insn, std::ostream* pos);

        /**
         * @brief Execute sub instruction.
         * @param insn The instruction to execute.
         * @param pos The output stream.
         * ********************************************************************/
        void exec_sub(uint32_t insn, std::ostream* pos);

        /**
         * @brief Execute sll instruction.
         * @param insn The instruction to execute.
         * @param pos The output stream.
         * ********************************************************************/
        void exec_sll(uint32_t insn, std::ostream* pos);

        /**
         * @brief Execute slt instruction.
         * @param insn The instruction to execute.
         * @param pos The output stream.
         * ********************************************************************/
        void exec_slt(uint32_t insn, std::ostream* pos);

        /**
         * @brief Execute sltu instruction.
         * @param insn The instruction to execute.
         * @param pos The output stream.
         * ********************************************************************/
        void exec_sltu(uint32_t insn, std::ostream* pos);

        /**
         * @brief Execute xor instruction.
         * @param insn The instruction to execute.
         * @param pos The output stream.
         * ********************************************************************/
        void exec_xor(uint32_t insn, std::ostream* pos);

        /**
         * @brief Execute srl instruction.
         * @param insn The instruction to execute.
         * @param pos The output stream.
         * ********************************************************************/
        void exec_srl(uint32_t insn, std::ostream* pos);

        /**
         * @brief Execute sra instruction.
         * @param insn The instruction to execute.
         * @param pos The output stream.
         * ********************************************************************/
        void exec_sra(uint32_t insn, std::ostream* pos);

        /**
         * @brief Execute or instruction.
         * @param insn The instruction to execute.
         * @param pos The output stream.
         * ********************************************************************/
        void exec_or(uint32_t insn, std::ostream* pos);

        /**
         * @brief Execute and instruction.
         * @param insn The instruction to execute.
         * @param pos The output stream.
         * ********************************************************************/
        void exec_and(uint32_t insn, std::ostream* pos);

        /**
         * @brief Execute csrrs instruction.
         * @param insn The instruction to execute.
         * @param pos The output stream.
         * ********************************************************************/
        void exec_csrrs(uint32_t insn, std::ostream* pos);

        /**
         * @brief Execute ebreak instruction.
         * @param insn The instruction to execute.
         * @param pos The output stream.
         * ********************************************************************/
        void exec_ebreak(uint32_t insn, std::ostream* pos);

        /**
         * @brief Execute ecall instruction.
         * @param insn The instruction to execute.
         * @param pos The output stream.
         * ********************************************************************/
        void exec_ecall(uint32_t insn, std::ostream* pos);

        /**
         * @brief Flag to stop the hart from executing instructions.
         * ********************************************************************/
        bool halt = { false };

        /**
         * @brief Description of the reason for the halt.
         * ********************************************************************/
        std::string halt_reason = { "none" };

        /**
         * @brief Flag to indicate whether instructions should be printed.
         * ********************************************************************/
        bool show_instructions = { false };

        /**
         * @brief Flag to indicate whether to dump regs with each instruction.
         * ********************************************************************/
        bool show_registers = { false };

        /**
         * @brief Count of the number of instructions that have been executed.
         * ********************************************************************/
        uint64_t insn_counter = { 0 };

        /**
         * @brief Address of the instruction being decoded/disassembled.
         * ********************************************************************/
        uint32_t pc = { 0 };

        /**
         * @brief The CSR register value to return by a csrrs instruction that 
         *        reads register 0xf14.
         * ********************************************************************/
        uint32_t mhartid = { 0 };

    protected:
        /**
         * @brief The GP-regs
         * ********************************************************************/
        registerfile regs;

        /**
         * @brief Simulated memory.
         * ********************************************************************/
        memory &mem;
};

#endif
