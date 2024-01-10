//******************************************************************************
//
// rv32i_decode.cpp
//
// CSCI 463 Assignment 5
//
// Author: Gavin St. George (Z1909350)
//
// RISC-V Simulator
//
//******************************************************************************

#include "rv32i_decode.h"

// Decodes an instruction
std::string rv32i_decode::decode(uint32_t addr, uint32_t insn)
{
    switch (get_opcode(insn))
    {
        default:                            return render_illegal_insn(insn);
        case opcode_lui:                    return render_lui(insn);
        case opcode_auipc:                  return render_auipc(insn);
        case opcode_jal:                    return render_jal(addr, insn);
        case opcode_jalr:                   return render_jalr(insn);

        case opcode_btype:
            switch (get_funct3(insn))
            {
                default:                    return render_illegal_insn(insn);
                case funct3_beq:            return render_btype(addr, insn, "beq");
                case funct3_bne:            return render_btype(addr, insn, "bne");
                case funct3_blt:            return render_btype(addr, insn, "blt");
                case funct3_bge:            return render_btype(addr, insn, "bge");
                case funct3_bltu:           return render_btype(addr, insn, "bltu");
                case funct3_bgeu:           return render_btype(addr, insn, "bgeu");
            }
            assert(0 && "unrecognized funct3");
        
        case opcode_load_imm:
            switch (get_funct3(insn))
            {
                default:                    return render_illegal_insn(insn);
                case funct3_lb:             return render_itype_load(insn, "lb");
                case funct3_lh:             return render_itype_load(insn, "lh");
                case funct3_lw:             return render_itype_load(insn, "lw");
                case funct3_lbu:            return render_itype_load(insn, "lbu");
                case funct3_lhu:            return render_itype_load(insn, "lhu");
            }
            assert(0 && "unrecognized funct3");
        
        case opcode_stype:
            switch (get_funct3(insn))
            {
                default:                    return render_illegal_insn(insn);
                case funct3_sb:             return render_stype(insn, "sb");
                case funct3_sh:             return render_stype(insn, "sh");
                case funct3_sw:             return render_stype(insn, "sw");
            }
            assert(0 && "unrecognized funct3");

        case opcode_alu_imm:
            switch (get_funct3(insn))
            {
                default:                    return render_illegal_insn(insn);
                case funct3_add:            return render_itype_alu(insn, "addi", get_imm_i(insn));
                case funct3_sll:            return render_itype_alu(insn, "slli", get_imm_i(insn) % XLEN);
                case funct3_slt:            return render_itype_alu(insn, "slti", get_imm_i(insn));
                case funct3_sltu:           return render_itype_alu(insn, "sltiu", get_imm_i(insn));
                case funct3_xor:            return render_itype_alu(insn, "xori", get_imm_i(insn));
                case funct3_srx:
                    switch(get_funct7(insn))
                    {
                        default:            return render_illegal_insn(insn);
                        case funct7_srl:    return render_itype_alu(insn, "srli", get_imm_i(insn) % XLEN);
                        case funct7_sra:    return render_itype_alu(insn, "srai", get_imm_i(insn) % XLEN);
                    }
                    assert(0 && "unrecognized funct7");
                case funct3_or:             return render_itype_alu(insn, "ori", get_imm_i(insn));
                case funct3_and:            return render_itype_alu(insn, "andi", get_imm_i(insn));
            }
            assert(0 && "unrecognized funct3");

        case opcode_rtype:
            switch (get_funct3(insn))
            {
                default:                    return render_illegal_insn(insn);
                case funct3_add:
                    switch (get_funct7(insn))
                    {
                        default:            return render_illegal_insn(insn);
                        case funct7_add:    return render_rtype(insn, "add");
                        case funct7_sub:    return render_rtype(insn, "sub");
                    }
                    assert(0 && "unrecognized funct7");
                case funct3_sll:            return render_rtype(insn, "sll");
                case funct3_slt:            return render_rtype(insn, "slt");
                case funct3_sltu:           return render_rtype(insn, "sltu");
                case funct3_xor:            return render_rtype(insn, "xor");
                case funct3_srx:
                    switch (get_funct7(insn))
                    {
                        default:            return render_illegal_insn(insn);
                        case funct7_srl:    return render_rtype(insn, "srl");
                        case funct7_sra:    return render_rtype(insn, "sra");
                    }
                    assert(0 && "unrecognized funct7");
                case funct3_or:             return render_rtype(insn, "or");
                case funct3_and:            return render_rtype(insn, "and");
            }
            assert(0 && "unrecognized funct3");

        case opcode_system:
            switch (insn)
            {
                default:
                    switch (get_funct3(insn))
                    {
                        default:            return render_illegal_insn(insn);
                        case funct3_csrrw:  return render_csrrx(insn, "csrrw");
                        case funct3_csrrs:  return render_csrrx(insn, "csrrs");
                        case funct3_csrrc:  return render_csrrx(insn, "csrrc");
                        case funct3_csrrwi: return render_csrrxi(insn, "csrrwi");
                        case funct3_csrrsi: return render_csrrxi(insn, "csrrsi");
                        case funct3_csrrci: return render_csrrxi(insn, "csrrci");
                    }
                    assert(0 && "unrecognized funct3");
                case insn_ecall:            return render_ecall(insn);
                case insn_ebreak:           return render_ebreak(insn);
            }
            assert(0 && "unrecognized instruction");
    }
    assert(0 && "unrecognized opcode");
}

// Extracts the opcode from an instruction.
uint32_t rv32i_decode::get_opcode(uint32_t insn)
{
    return (insn & 0x0000007f);
}

// Extracts the rd field from an instruction.
uint32_t rv32i_decode::get_rd(uint32_t insn)
{
    return (insn & 0x00000f80) >> (7 - 0);
}

// Extracts the func3 field from an instruction.
uint32_t rv32i_decode::get_funct3(uint32_t insn)
{
    return (insn & 0x00007000) >> (12 - 0);
}

// Extracts the rs1 field from an instruction.
uint32_t rv32i_decode::get_rs1(uint32_t insn)
{
    return (insn & 0x000f8000) >> (15 - 0);
}

// Extracts the rs2 field from an instruction.
uint32_t rv32i_decode::get_rs2(uint32_t insn)
{
    return (insn & 0x01f00000) >> (20 - 0);
}

// Extracts the func7 field from an instruction.
uint32_t rv32i_decode::get_funct7(uint32_t insn)
{
    return (insn & 0xfe000000) >> (25 - 0);
}

// Extracts the imm_i field from an instruction.
int32_t rv32i_decode::get_imm_i(uint32_t insn)
{
    int32_t imm_i = (insn & 0xfff00000) >> (20 - 0);
    return (insn & 0x80000000) ? imm_i |= 0xfffff800 : imm_i;
}

// Extracts the func3 field from an instruction.
int32_t rv32i_decode::get_imm_u(uint32_t insn)
{
    return (insn & 0xfffff000);
}

// Extracts the imm_b field from an instruction.
int32_t rv32i_decode::get_imm_b(uint32_t insn)
{
    int32_t imm_b = (insn & 0x00000080) << (11 - 7);
    imm_b |= (insn & 0x7e000000) >> (25 - 5);
    imm_b |= (insn & 0x00000f00) >> (8 - 1);
    return (insn & 0x80000000) ? imm_b |= 0xfffff000 : imm_b;
}

// Extracts the imm_s field from an instruction.
int32_t rv32i_decode::get_imm_s(uint32_t insn)
{
    int32_t imm_s = (insn & 0xfe000000) >> (25 - 5);
    imm_s |= (insn & 0x00000f80) >> (7 - 0);
    return (insn & 0x80000000) ? imm_s |= 0xfffff800 : imm_s;
}

// Extracts the imm_j field from an instruction.
int32_t rv32i_decode::get_imm_j(uint32_t insn)
{
    int32_t imm_j = (insn & 0x7fe00000) >> (21 - 1);
    imm_j |= (insn & 0x00100000) >> (20 - 11);
    imm_j |= (insn & 0x000ff000);
    return (insn & 0x80000000) ? imm_j |= 0xfff00000 : imm_j;
}

// Handles illegal instructions.
std::string rv32i_decode::render_illegal_insn(uint32_t insn)
{
    return "ERROR: UNIMPLEMENTED INSTRUCTION";
}

// Handles lui instructions.
std::string rv32i_decode::render_lui(uint32_t insn)
{
    uint32_t rd = get_rd(insn);
    int32_t imm_u = get_imm_u(insn);

    std::ostringstream os;
    os << render_mnemonic("lui") << render_reg(rd) << "," << to_hex0x20((imm_u >> (12-0)) & 0x0fffff);
    return os.str();
}

// Handles auipc instructions.
std::string rv32i_decode::render_auipc(uint32_t insn)
{
    uint32_t rd = get_rd(insn);
    int32_t imm_u = get_imm_u(insn);

    std::ostringstream os;
    os << render_mnemonic("auipc") << render_reg(rd) << "," << to_hex0x20((imm_u >> (12 - 0)) & 0x0fffff);
    return os.str();
}

// Handles jal instructions.
std::string rv32i_decode::render_jal(uint32_t addr, uint32_t insn)
{
    uint32_t rd = get_rd(insn);
    int32_t imm_j = get_imm_j(insn);

    std::ostringstream os;
    os << render_mnemonic("jal") << render_reg(rd) << "," << to_hex0x32(imm_j + addr);
    return os.str();
}

// Handles jalr instructions.
std::string rv32i_decode::render_jalr(uint32_t insn)
{
    uint32_t rd = get_rd(insn);
    uint32_t rs1 = get_rs1(insn);
    int32_t imm_i = get_imm_i(insn);

    std::ostringstream os;
    os << render_mnemonic("jalr") << render_reg(rd) << "," << imm_i << "(" << render_reg(rs1) << ")";
    return os.str();
}

// Handles btype instructions.
std::string rv32i_decode::render_btype(uint32_t addr, uint32_t insn, const char *mnemonic)
{
    uint32_t rs1 = get_rs1(insn);
    uint32_t rs2 = get_rs2(insn);
    int32_t imm_b = get_imm_b(insn);

    std::ostringstream os;
    os << render_mnemonic(mnemonic) << render_reg(rs1) << "," << render_reg(rs2) << "," << to_hex0x32(imm_b + addr);
    return os.str();
}

// Handles itype load instructions.
std::string rv32i_decode::render_itype_load(uint32_t insn, const char *mnemonic)
{
    uint32_t rd = get_rd(insn);
    uint32_t rs1 = get_rs1(insn);
    int32_t imm_i = get_imm_i(insn);

    std::ostringstream os;
    os << render_mnemonic(mnemonic) << render_reg(rd) << "," << imm_i << "(" << render_reg(rs1) << ")";
    return os.str();
}

// Handles stype instructions.
std::string rv32i_decode::render_stype(uint32_t insn, const char *mnemonic)
{
    uint32_t rs1 = get_rs1(insn);
    uint32_t rs2 = get_rs2(insn);
    int32_t imm_s = get_imm_s(insn);

    std::ostringstream os;
    os << render_mnemonic(mnemonic) << render_reg(rs2) << "," << imm_s << "(" << render_reg(rs1) << ")";
    return os.str();
}

// Handles itype alu instructions.
std::string rv32i_decode::render_itype_alu(uint32_t insn, const char *mnemonic, int32_t imm_i)
{
    uint32_t rd = get_rd(insn);
    uint32_t rs1 = get_rs1(insn);

    std::ostringstream os;
    os << render_mnemonic(mnemonic) << render_reg(rd) << "," << render_reg(rs1) << "," << imm_i;
    return os.str();
}

// Handles rtype instructions.
std::string rv32i_decode::render_rtype(uint32_t insn, const char *mnemonic)
{
    uint32_t rd = get_rd(insn);
    uint32_t rs1 = get_rs1(insn);
    uint32_t rs2 = get_rs2(insn);

    std::ostringstream os;
    os << render_mnemonic(mnemonic) << render_reg(rd) << "," << render_reg(rs1) << "," << render_reg(rs2);
    return os.str();
}

// Handles ecall instructions.
std::string rv32i_decode::render_ecall(uint32_t insn)
{
    return "ecall";
}

// Handles ebreak instructions.
std::string rv32i_decode::render_ebreak(uint32_t insn)
{
    return "ebreak";
}

// Handles csrrx instructions.
std::string rv32i_decode::render_csrrx(uint32_t insn, const char *mnemonic)
{
    uint32_t rd = get_rd(insn);
    uint32_t csr = get_imm_i(insn);
    uint32_t rs1 = get_rs1(insn);

    std::ostringstream os;
    os << render_mnemonic(mnemonic) << render_reg(rd) << "," << to_hex0x12(csr & 0xfff) << "," << render_reg(rs1);
    return os.str();
}

// Handles csrrxi instructions.
std::string rv32i_decode::render_csrrxi(uint32_t insn, const char *mnemonic)
{
    uint32_t rd = get_rd(insn);
    uint32_t csr = get_imm_i(insn);
    uint32_t zimm = get_rs1(insn);

    std::ostringstream os;
    os << render_mnemonic(mnemonic) << render_reg(rd) << "," << to_hex0x12(csr & 0xfff) << "," << zimm;
    return os.str();
}

// Formats a register number for output
std::string rv32i_decode::render_reg(int r)
{
    std::ostringstream os;
    os << "x" << r;
    return os.str();
}

// Formats base displacement for output.
std::string rv32i_decode::render_base_disp(uint32_t base, int32_t disp)
{
    std::ostringstream os;
    os << disp << "(" << render_reg(base) << ")";
    return os.str();
}

// Formats the mnemonic for output.
std::string rv32i_decode::render_mnemonic(const std::string &m)
{
    std::ostringstream os;
    os << std::left << std::setfill(' ') << std::setw(mnemonic_width) << m;
    return os.str();
}
