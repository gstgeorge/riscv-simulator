#ifndef RV32I_DECODE_H
#define RV32I_DECODE_H

//******************************************************************************
//
// rv32i_decode.h
//
// CSCI 463 Assignment 5
//
// Author: Gavin St. George (Z1909350)
//
// RISC-V Simulator
//
//******************************************************************************

#include <string>
#include <sstream>
#include <cctype>
#include <cassert>

#include "hex.h"

/**
 * @brief Class to handle decoding RV32I instructions.
 * ****************************************************************************/
class rv32i_decode : public hex
{
	public:
		/**
		 * @brief Decodes an instruction.
		 * @param addr The memory address where the insn is stored.
		 * @param insn An instruction.
		 * @returns A formatted string containing the decoded instruction.
		 * ****************************************************************************/
		static std::string decode(uint32_t addr, uint32_t insn);

	protected:
		static constexpr int mnemonic_width             = 8; ///< Width of the mnemonic output field.

		static constexpr uint32_t opcode_lui			= 0b0110111; ///< Opcode for lui.
		static constexpr uint32_t opcode_auipc			= 0b0010111; ///< Opcode for auipc.
		static constexpr uint32_t opcode_jal			= 0b1101111; ///< Opcode for jal.
		static constexpr uint32_t opcode_jalr			= 0b1100111; ///< Opcode for jalr.
		static constexpr uint32_t opcode_btype			= 0b1100011; ///< Opcode for btype instructions.
		static constexpr uint32_t opcode_load_imm		= 0b0000011; ///< Opcode for load immediate instructions.
		static constexpr uint32_t opcode_stype			= 0b0100011; ///< Opcode for stype instructions.
		static constexpr uint32_t opcode_alu_imm		= 0b0010011; ///< Opcode for alu instructions.
		static constexpr uint32_t opcode_rtype			= 0b0110011; ///< Opcode for rtype instructions.
		static constexpr uint32_t opcode_system			= 0b1110011; ///< Opcode for system instructions.

		static constexpr uint32_t funct3_beq			= 0b000; ///< Funct3 value for beq.
		static constexpr uint32_t funct3_bne			= 0b001; ///< Funct3 value for bne.
		static constexpr uint32_t funct3_blt			= 0b100; ///< Funct3 value for blt.
		static constexpr uint32_t funct3_bge			= 0b101; ///< Funct3 value for bge.
		static constexpr uint32_t funct3_bltu			= 0b110; ///< Funct3 value for bltu.
		static constexpr uint32_t funct3_bgeu			= 0b111; ///< Funct3 value for bgeu.

		static constexpr uint32_t funct3_lb				= 0b000; ///< Funct3 value for lb.
		static constexpr uint32_t funct3_lh				= 0b001; ///< Funct3 value for lh.
		static constexpr uint32_t funct3_lw				= 0b010; ///< Funct3 value for lw.
		static constexpr uint32_t funct3_lbu			= 0b100; ///< Funct3 value for lbu.
		static constexpr uint32_t funct3_lhu			= 0b101; ///< Funct3 value for lhu.

		static constexpr uint32_t funct3_sb				= 0b000; ///< Funct3 value for sb.
		static constexpr uint32_t funct3_sh				= 0b001; ///< Funct3 value for sh.
		static constexpr uint32_t funct3_sw				= 0b010; ///< Funct3 value for sw.

		static constexpr uint32_t funct3_add			= 0b000; ///< Funct3 value for add.
		static constexpr uint32_t funct3_sll			= 0b001; ///< Funct3 value for sll.
		static constexpr uint32_t funct3_slt			= 0b010; ///< Funct3 value for slt.
		static constexpr uint32_t funct3_sltu			= 0b011; ///< Funct3 value for sltu.
		static constexpr uint32_t funct3_xor			= 0b100; ///< Funct3 value for xor.
		static constexpr uint32_t funct3_srx			= 0b101; ///< Funct3 value for srx.
		static constexpr uint32_t funct3_or				= 0b110; ///< Funct3 value for or.
		static constexpr uint32_t funct3_and			= 0b111; ///< Funct3 value for and.

		static constexpr uint32_t funct7_srl			= 0b0000000; ///< Funct7 value for srl.
		static constexpr uint32_t funct7_sra			= 0b0100000; ///< Funct7 value for sra.

		static constexpr uint32_t funct7_add			= 0b0000000; ///< Funct7 value for add.
		static constexpr uint32_t funct7_sub			= 0b0100000; ///< Funct7 value for sub.

		static constexpr uint32_t insn_ecall			= 0x00000073; ///< Instruction value for ecall.
		static constexpr uint32_t insn_ebreak			= 0x00100073; ///< Instruction value for ebreak.

		static constexpr uint32_t funct3_csrrw			= 0b001; ///< Funct3 value for csrrw.
		static constexpr uint32_t funct3_csrrs			= 0b010; ///< Funct3 value for csrrs.
		static constexpr uint32_t funct3_csrrc			= 0b011; ///< Funct3 value for csrrc.
		static constexpr uint32_t funct3_csrrwi			= 0b101; ///< Funct3 value for csrrwi.
		static constexpr uint32_t funct3_csrrsi			= 0b110; ///< Funct3 value for csrrsi.
		static constexpr uint32_t funct3_csrrci			= 0b111; ///< Funct3 value for csrrci.

		/**
		 * @brief Extracts the opcode from an instruction.
		 * @param insn An instruction.
		 * @returns The opcode.
		 * ****************************************************************************/
		static uint32_t get_opcode(uint32_t insn);

		/**
		 * @brief Extracts the rd field from an instruction.
		 * @param insn An instruction.
		 * @returns The rd field.
		 * ****************************************************************************/
		static uint32_t get_rd(uint32_t insn);
		
		/**
		 * @brief Extracts the func3 field from an instruction.
		 * @param insn An instruction.
		 * @returns The func3 field.
		 * ****************************************************************************/
		static uint32_t get_funct3(uint32_t insn);

		/**
		 * @brief Extracts the rs1 field from an instruction.
		 * @param insn An instruction.
		 * @returns The rs1 field.
		 * ****************************************************************************/
		static uint32_t get_rs1(uint32_t insn);
		
		/**
		 * @brief Extracts the rs2 field from an instruction.
		 * @param insn An instruction.
		 * @returns The rs2 field.
		 * ****************************************************************************/
		static uint32_t get_rs2(uint32_t insn);
		
		/**
		 * @brief Extracts the func7 field from an instruction.
		 * @param insn An instruction.
		 * @returns The func7 field.
		 * ****************************************************************************/
		static uint32_t get_funct7(uint32_t insn);
		
		/**
		 * @brief Extracts the imm_i field from an instruction.
		 * @param insn An instruction.
		 * @returns The imm_i field.
		 * ****************************************************************************/
		static int32_t get_imm_i(uint32_t insn);

		/**
		 * @brief Extracts the imm_u field from an instruction.
		 * @param insn An instruction.
		 * @returns The imm_u field.
		 * ****************************************************************************/
		static int32_t get_imm_u(uint32_t insn);

		/**
		 * @brief Extracts the imm_b field from an instruction.
		 * @param insn An instruction.
		 * @returns The imm_b field.
		 * ****************************************************************************/
		static int32_t get_imm_b(uint32_t insn);

		/**
		 * @brief Extracts the imm_s field from an instruction.
		 * @param insn An instruction.
		 * @returns The imm_s field.
		 * ****************************************************************************/
		static int32_t get_imm_s(uint32_t insn);

		/**
		 * @brief Extracts the imm_j field from an instruction.
		 * @param insn An instruction.
		 * @returns The imm_j field.
		 * ****************************************************************************/
		static int32_t get_imm_j(uint32_t insn);

		static constexpr uint32_t XLEN = 32; ///< Register bit-length.

		/**
		 * @brief Handles illegal instructions.
		 * @param insn An instruction.
		 * @returns A formatted string containing an error message.
		 * ****************************************************************************/
		static std::string render_illegal_insn(uint32_t insn);

		/**
		 * @brief Handles lui instructions.
		 * @param insn An instruction.
		 * @returns A formatted string containing the decoded lui instruction.
		 * ****************************************************************************/
		static std::string render_lui(uint32_t insn);
		
		/**
		 * @brief Handles auipc instructions.
		 * @param insn An instruction.
		 * @returns A formatted string containing the decoded auipc instruction.
		 * ****************************************************************************/	
		static std::string render_auipc(uint32_t insn);
		
		/**
		 * @brief Handles jal instructions.
		 * @param addr The memory address where the insn is stored.
		 * @param insn An instruction.
		 * @returns A formatted string containing the decoded jal instruction.
		 * ****************************************************************************/
		static std::string render_jal(uint32_t addr, uint32_t insn);

		/**
		 * @brief Handles jalr instructions.
		 * @param insn An instruction.
		 * @returns A formatted string containing the decoded jalr instruction.
		 * ****************************************************************************/
		static std::string render_jalr(uint32_t insn);

		/**
		 * @brief Handles btype instructions.
		 * @param addr The memory address where the insn is stored.
		 * @param insn An instruction.
		 * @param mnemonic The mnemonic.
		 * @returns A formatted string containing the decoded btype instruction.
		 * ****************************************************************************/
		static std::string render_btype(uint32_t addr, uint32_t insn, const char *mnemonic);

		/**
		 * @brief Handles itype load instructions.
		 * @param insn An instruction.
		 * @param mnemonic The mnemonic.
		 * @returns A formatted string containing the decoded itype load instruction.
		 * ****************************************************************************/
		static std::string render_itype_load(uint32_t insn, const char *mnemonic);

		/**
		 * @brief Handles stype instructions.
		 * @param insn An instruction.
		 * @param mnemonic The mnemonic.
		 * @returns A formatted string containing the decoded stype instruction.
		 * ****************************************************************************/
		static std::string render_stype(uint32_t insn, const char *mnemonic);
		
		/**
		 * @brief Handles itype alu instructions.
		 * @param insn An instruction.
		 * @param mnemonic The mnemonic.
		 * @param imm_i The imm/shamt field.
		 * @returns A formatted string containing the decoded itype alu instruction.
		 * ****************************************************************************/
		static std::string render_itype_alu(uint32_t insn, const char *mnemonic, int32_t imm_i);
		
		/**
		 * @brief Handles rtype instructions.
		 * @param insn An instruction.
		 * @param mnemonic The mnemonic.
		 * @returns A formatted string containing the decoded rtype instruction.
		 * ****************************************************************************/		
		static std::string render_rtype(uint32_t insn, const char *mnemonic);
		
		/**
		 * @brief Handles ecall instructions.
		 * @param insn An instruction.
		 * @returns A formatted string containing the decoded ecall instruction.
		 * ****************************************************************************/	
		static std::string render_ecall(uint32_t insn);

		/**
		 * @brief Handles ebreak instructions.
		 * @param insn An instruction.
		 * @returns A formatted string containing the decoded ebreak instruction.
		 * ****************************************************************************/
		static std::string render_ebreak(uint32_t insn);
		
		/**
		 * @brief Handles csrrx instructions.
		 * @param insn An instruction.
		 * @param mnemonic The mnemonic.
		 * @returns A formatted string containing the decoded csrrx instruction.
		 * ****************************************************************************/		
		static std::string render_csrrx(uint32_t insn, const char *mnemonic);
		
		/**
		 * @brief Handles csrrxi instructions.
		 * @param insn An instruction.
		 * @param mnemonic The mnemonic.
		 * @returns A formatted string containing the decoded csrrxi instruction.
		 * ****************************************************************************/		
		static std::string render_csrrxi(uint32_t insn, const char *mnemonic);

		/**
		 * @brief Formats a register number for output.
		 * @param r The register number.
		 * @returns A string containing the formatted register number.
		 * ****************************************************************************/
		static std::string render_reg(int r);

		/**
		 * @brief Formats base displacement for output.
		 * @param base The base register.
		 * @param disp The displacement.
		 * @returns A string containing the formatted base displacement.
		 * ****************************************************************************/
		static std::string render_base_disp(uint32_t base, int32_t disp);

		/**
		 * @brief Formats the mnemonic for output.
		 * @param m The unformatted mnemonic.
		 * @returns A string containing the formatted mnemonic.
		 * ****************************************************************************/
		static std::string render_mnemonic(const std::string &m);
};

#endif
