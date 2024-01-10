//******************************************************************************
//
// rv32i_hart.cpp
//
// CSCI 463 Assignment 5
//
// Author: Gavin St. George (Z1909350)
//
// RISC-V Simulator
//
//******************************************************************************

#include "rv32i_hart.h"

// Reset the rv32i object and the registerfile.
void rv32i_hart::reset()
{
    pc = 0;
    regs.reset();
    insn_counter = 0;
    halt = false;
    halt_reason = "none";
}

// Dump the entire state of the hart.
void rv32i_hart::dump(const std::string &hdr) const
{
    regs.dump(hdr);
    std::cout << hdr << " pc " << to_hex32(pc) << std::endl;
}

// Tells the simulator to execute an instruction.
void rv32i_hart::tick(const std::string &hdr)
{
    if (!is_halted())
    {
        insn_counter++;

        if (show_registers)
        {
            dump(hdr);
        }

        uint32_t insn = mem.get32(pc);

        if (show_instructions)
        {
            std::cout << hex::to_hex32(pc) << ": " << hex::to_hex32(insn) << "  ";
            exec(insn, &std::cout);
            std::cout << std::endl;
        }
        else exec(insn, nullptr);
    }
}

// Execute the given RV32I instruction.
void rv32i_hart::exec(uint32_t insn, std::ostream* pos)
{
    switch (get_opcode(insn))
    {
        default:                            return exec_illegal_insn(insn, pos);
        case opcode_lui:                    return exec_lui(insn, pos);
        case opcode_auipc:                  return exec_auipc(insn, pos);
        case opcode_jal:                    return exec_jal(insn, pos);
        case opcode_jalr:                   return exec_jalr(insn, pos);
        case opcode_btype:                  return exec_btype(insn, pos);
        case opcode_load_imm:               return exec_load_imm(insn, pos);
        case opcode_stype:                  return exec_stype(insn, pos);

        case opcode_alu_imm:
            switch (get_funct3(insn))
            {
                default:                    return exec_illegal_insn(insn, pos);
                case funct3_add:            return exec_addi(insn, pos);
                case funct3_sll:            return exec_slli(insn, pos);
                case funct3_slt:            return exec_slti(insn, pos);
                case funct3_sltu:           return exec_sltiu(insn, pos);
                case funct3_xor:            return exec_xori(insn, pos);
                case funct3_srx:
                    switch(get_funct7(insn))
                    {
                        default:            return exec_illegal_insn(insn, pos);
                        case funct7_srl:    return exec_srli(insn, pos);
                        case funct7_sra:    return exec_srai(insn, pos);
                    }
                    assert(0 && "unrecognized funct7");
                case funct3_or:             return exec_ori(insn, pos);
                case funct3_and:            return exec_andi(insn, pos);
            }
            assert(0 && "unrecognized funct3");

        case opcode_rtype:
            switch (get_funct3(insn))
            {
                default:                    return exec_illegal_insn(insn, pos);
                case funct3_add:
                    switch (get_funct7(insn))
                    {
                        default:            return exec_illegal_insn(insn, pos);
                        case funct7_add:    return exec_add(insn, pos);
                        case funct7_sub:    return exec_sub(insn, pos);
                    }
                    assert(0 && "unrecognized funct7");
                case funct3_sll:            return exec_sll(insn, pos);
                case funct3_slt:            return exec_slt(insn, pos);
                case funct3_sltu:           return exec_sltu(insn, pos);
                case funct3_xor:            return exec_xor(insn, pos);
                case funct3_srx:
                    switch (get_funct7(insn))
                    {
                        default:            return exec_illegal_insn(insn, pos);
                        case funct7_srl:    return exec_srl(insn, pos);
                        case funct7_sra:    return exec_sra(insn, pos);
                    }
                    assert(0 && "unrecognized funct7");
                case funct3_or:             return exec_or(insn, pos);
                case funct3_and:            return exec_and(insn, pos);
            }
            assert(0 && "unrecognized funct3");

        case opcode_system:
            switch (insn)
            {
                default:
                    switch (get_funct3(insn))
                    {
                        default:            return exec_illegal_insn(insn, pos);
                        case funct3_csrrw:  return exec_illegal_insn(insn, pos);
                        case funct3_csrrs:  return exec_csrrs(insn, pos);
                        case funct3_csrrc:  return exec_illegal_insn(insn, pos);
                        case funct3_csrrwi: return exec_illegal_insn(insn, pos);
                        case funct3_csrrsi: return exec_illegal_insn(insn, pos);
                        case funct3_csrrci: return exec_illegal_insn(insn, pos);
                    }
                    assert(0 && "unrecognized funct3");
                case insn_ecall:            return exec_ecall(insn, pos);
                case insn_ebreak:           return exec_ebreak(insn, pos);
            }
            assert(0 && "unrecognized instruction");
    }
    assert(0 && "unrecognized opcode");
}

// Execute illegal instruction.
void rv32i_hart::exec_illegal_insn(uint32_t insn, std::ostream* pos)
{
    (void)insn;
    if (pos)
    {
        *pos << render_illegal_insn(insn);
    }
    halt = true;
    halt_reason = "Illegal instruction";
}

// Execute ebreak instruction
void rv32i_hart::exec_ebreak(uint32_t insn, std::ostream* pos)
{
    (void)insn;
    if (pos)
    {
        std::string s = render_ebreak(insn);
        *pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
        *pos << "// HALT";
    }

    halt = true;
    halt_reason = "EBREAK instruction";
}

// Execute ecall instruction
void rv32i_hart::exec_ecall(uint32_t insn, std::ostream* pos)
{
    (void)insn;
    if (pos)
    {
        std::string s = render_ecall(insn);
        *pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
        *pos << "// HALT";
    }

    halt = true;
    halt_reason = "ECALL instruction";
}

// Execute lui instruction.
void rv32i_hart::exec_lui(uint32_t insn, std::ostream* pos)
{
    uint32_t rd = get_rd(insn);
    int32_t imm_u = get_imm_u(insn);

    if (pos)
    {
        std::string s = render_lui(insn);
        *pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
        *pos << "// " << render_reg(rd) << " = " << hex::to_hex0x32(imm_u);
    }
    
    regs.set(rd, imm_u);
    pc += 4;
}

// Execute auipc instruction.
void rv32i_hart::exec_auipc(uint32_t insn, std::ostream* pos)
{
    uint32_t rd = get_rd(insn);
    int32_t imm_u = get_imm_u(insn);
    int32_t val = pc + imm_u;

    if (pos)
    {
        std::string s = render_auipc(insn);
        *pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
        *pos << "// " << render_reg(rd) << " = " << hex::to_hex0x32(pc) << " + "
             << hex::to_hex0x32(imm_u) << " = " << hex::to_hex0x32(val);
    }

    regs.set(rd, val);
    pc += 4;
}

// Execute jal instruction.
void rv32i_hart::exec_jal(uint32_t insn, std::ostream* pos)
{
    uint32_t rd = get_rd(insn);
    int32_t imm_j = get_imm_j(insn);
    int32_t val = pc + imm_j;

    if (pos)
    {
        std::string s = render_jal(pc, insn);
        *pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
        *pos << "// " << render_reg(rd) << " = " << hex::to_hex0x32(pc+4) << ",  "
             << "pc = " << hex::to_hex0x32(pc) << " + " << hex::to_hex0x32(imm_j)
             << " = " << hex::to_hex0x32(val);
    }

    regs.set(rd, pc+4);
    pc = val;
}

// Execute jalr instruction.
void rv32i_hart::exec_jalr(uint32_t insn, std::ostream* pos)
{
    uint32_t rd = get_rd(insn);
    int32_t rs1 = regs.get(get_rs1(insn));
    int32_t imm_i = get_imm_i(insn);
    int32_t val = (imm_i + rs1) & 0xfffffffe;

    if (pos)
    {
        std::string s = render_jalr(insn);
        *pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
        *pos << "// " << render_reg(rd) << " = " << hex::to_hex0x32(pc+4) << ",  "
             << "pc = (" << hex::to_hex0x32(imm_i) << " + " << hex::to_hex0x32(rs1)
             << ") & 0xfffffffe" << " = " << hex::to_hex0x32(val);
    }

    regs.set(rd, pc+4);
    pc = val;
}

// Execute btype instruction.
void rv32i_hart::exec_btype(uint32_t insn, std::ostream *pos)
{
    uint32_t funct3 = get_funct3(insn);
    int32_t rs1 = regs.get(get_rs1(insn));
    int32_t rs2 = regs.get(get_rs2(insn));
    int32_t imm_b = get_imm_b(insn);
    int32_t val;
    const char* mnemonic[] = { "beq", "bne", "", "", "blt", "bge", "bltu", "bgeu" };
    std::string op[] = {" == ", " != ", "", "", " < ", " >= ", " <U ", " >=U "};

    switch (funct3)
    {
        default: 
            exec_illegal_insn(insn, pos);
            break;

        case funct3_beq:
            val = rs1 == rs2 ? imm_b : 4;
            break;

        case funct3_bne:
            val = rs1 != rs2 ? imm_b : 4;
            break;

        case funct3_blt:
            val = rs1 < rs2 ? imm_b : 4;
            break;

        case funct3_bge:
            val = rs1 >= rs2 ? imm_b : 4;
            break;

        case funct3_bltu:
            val = (uint32_t)rs1 < (uint32_t)rs2 ? imm_b : 4;
            break;

        case funct3_bgeu:
            val = (uint32_t)rs1 >= (uint32_t)rs2 ? imm_b : 4;
            break;
    }

    if (pos)
    {
        std::string s = render_btype(pc, insn, mnemonic[funct3]);
        *pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
        *pos << "// pc += (" << hex::to_hex0x32(rs1) << op[funct3] << hex::to_hex0x32(rs2)
             << " ? " << hex::to_hex0x32(imm_b) << " : 4) = " << hex::to_hex0x32(pc+val);
    }

    pc += val;
}

// Execute load instruction.
void rv32i_hart::exec_load_imm(uint32_t insn, std::ostream* pos)
{
    uint32_t funct3 = get_funct3(insn);
    uint32_t rd = get_rd(insn);
    int32_t rs1 = regs.get(get_rs1(insn));
    int32_t imm_i = get_imm_i(insn);
    int32_t val;
    const char* mnemonic[] = { "lb", "lh", "lw", "", "lbu", "lhu" };
    std::string prefix[] = { "sx(m8(", "sx(m16(", "sx(m32(", "", "zx(m8(", "zx(m16(" };


    switch (funct3)
    {
        default:
            exec_illegal_insn(insn, pos);
            break;

        case funct3_lb:
            val = mem.get8_sx(rs1 + imm_i);
            break;

        case funct3_lh:
            val = mem.get16_sx(rs1 + imm_i);
            break;

        case funct3_lw:
            val = mem.get32_sx(rs1 + imm_i);
            break;

        case funct3_lbu:
            val = mem.get8(rs1 + imm_i);
            break;

        case funct3_lhu:
            val = mem.get16(rs1 + imm_i);
            break;
    }

    if (pos)
    {
        std::string s = render_itype_load(insn, mnemonic[funct3]);
        *pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
        *pos << "// " << render_reg(rd) << " = " << prefix[funct3] << hex::to_hex0x32(rs1)
             << " + " << hex::to_hex0x32(imm_i) << ")) = " << hex::to_hex0x32(val);
    }

    regs.set(rd, val);
    pc += 4;
}

// Execute stype instruction.
void rv32i_hart::exec_stype(uint32_t insn, std::ostream* pos)
{
    uint32_t funct3 = get_funct3(insn);
    int32_t rs1 = regs.get(get_rs1(insn));
    int32_t rs2 = regs.get(get_rs2(insn));
    int32_t imm_s = get_imm_s(insn);
    const char* mnemonic[] = { "sb", "sh", "sw" };
    std::string prefix[] = { "m8(", "m16(", "m32(" };

    switch (funct3)
    {
        default:
            exec_illegal_insn(insn, pos);
            break;

        case funct3_sb:
            rs2 &= 0xff;
            mem.set8(rs1 + imm_s, rs2);
            break;
        case funct3_sh:
            rs2 &= 0xffff;
            mem.set16(rs1 + imm_s, rs2);
            break;
        case funct3_sw:
            mem.set32(rs1 + imm_s, rs2);
            break;
    }

    if (pos)
    {
        std::string s = render_stype(insn, mnemonic[funct3]);
        *pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
        *pos << "// " << prefix[funct3] << hex::to_hex0x32(rs1) << " + " << hex::to_hex0x32(imm_s)
             << ") = " << hex::to_hex0x32(rs2);
    }

    pc += 4;
}

// Execute addi instruction.
void rv32i_hart::exec_addi(uint32_t insn, std::ostream* pos)
{
    uint32_t rd = get_rd(insn);
    int32_t rs1 = regs.get(get_rs1(insn));
    int32_t imm_i = get_imm_i(insn);
    int32_t val = rs1 + imm_i;
    
    if (pos)
    {
        std::string s = render_itype_alu(insn, "addi", imm_i);
        *pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
        *pos << "// " << render_reg(rd) << " = " << hex::to_hex0x32(rs1) << " + "
             << hex::to_hex0x32(imm_i) << " = " << hex::to_hex0x32(val);
    }

    regs.set(rd, val);
    pc += 4;
}

// Execute slti instruction.
void rv32i_hart::exec_slti(uint32_t insn, std::ostream* pos)
{
    uint32_t rd = get_rd(insn);
    int32_t rs1 = regs.get(get_rs1(insn));
    int32_t imm_i = get_imm_i(insn);
    int32_t val = rs1 < imm_i ? 1 : 0;

    if (pos)
    {
        std::string s = render_itype_alu(insn, "slti", get_imm_i(insn));
        *pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
        *pos << "// " << render_reg(rd) << " = (" << hex::to_hex0x32(rs1) << " < "
             << imm_i << ") ? 1 : 0 = " << hex::to_hex0x32(val);
    }

    regs.set(rd, val);
    pc += 4;
}

// Execute sltiu instruction.
void rv32i_hart::exec_sltiu(uint32_t insn, std::ostream* pos)
{
    uint32_t rd = get_rd(insn);
    uint32_t rs1 = regs.get(get_rs1(insn));
    uint32_t imm_i = get_imm_i(insn);
    uint32_t val = rs1 < imm_i ? 1 : 0;

    if (pos)
    {
        std::string s = render_itype_alu(insn, "sltiu", get_imm_i(insn));
        *pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
        *pos << "// " << render_reg(rd) << " = (" << hex::to_hex0x32(rs1) << " <U "
             << imm_i << ") ? 1 : 0 = " << hex::to_hex0x32(val);
    }

    regs.set(rd, val);
    pc += 4;
}

// Execute xori instruction.
void rv32i_hart::exec_xori(uint32_t insn, std::ostream* pos)
{
    uint32_t rd = get_rd(insn);
    int32_t rs1 = regs.get(get_rs1(insn));
    int32_t imm_i = get_imm_i(insn);
    int32_t val = rs1 ^ imm_i;

    if (pos)
    {
        std::string s = render_itype_alu(insn, "xori", get_imm_i(insn));
        *pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
        *pos << "// " << render_reg(rd) << " = " << hex::to_hex0x32(rs1) << " ^ "
             << hex::to_hex0x32(imm_i) << " = " << hex::to_hex0x32(val);
    }

    regs.set(rd, val);
    pc += 4;
}

// Execute ori instruction.
void rv32i_hart::exec_ori(uint32_t insn, std::ostream* pos)
{
    uint32_t rd = get_rd(insn);
    int32_t rs1 = regs.get(get_rs1(insn));
    int32_t imm_i = get_imm_i(insn);
    int32_t val = rs1 | imm_i;

    if (pos)
    {
        std::string s = render_itype_alu(insn, "ori", get_imm_i(insn));
        *pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
        *pos << "// " << render_reg(rd) << " = " << hex::to_hex0x32(rs1) << " | "
             << hex::to_hex0x32(imm_i) << " = " << hex::to_hex0x32(val);
    }

    regs.set(rd, val);
    pc += 4;
}

// Execute andi instruction.
void rv32i_hart::exec_andi(uint32_t insn, std::ostream* pos)
{
    uint32_t rd = get_rd(insn);
    int32_t rs1 = regs.get(get_rs1(insn));
    int32_t imm_i = get_imm_i(insn);
    int32_t val = rs1 & imm_i;

    if (pos)
    {
        std::string s = render_itype_alu(insn, "andi", get_imm_i(insn));
        *pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
        *pos << "// " << render_reg(rd) << " = " << hex::to_hex0x32(rs1) << " & "
             << hex::to_hex0x32(imm_i) << " = " << hex::to_hex0x32(val);
    }

    regs.set(rd, val);
    pc += 4;
}

// Execute slli instruction.
void rv32i_hart::exec_slli(uint32_t insn, std::ostream* pos)
{
    uint32_t rd = get_rd(insn);
    int32_t rs1 = regs.get(get_rs1(insn));
    uint32_t shamt = get_imm_i(insn) % XLEN;
    int32_t val = rs1 << shamt;

    if (pos)
    {
        std::string s = render_itype_alu(insn, "slli", shamt);
        *pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
        *pos << "// " << render_reg(rd) << " = " << hex::to_hex0x32(rs1) << " << "
             << shamt << " = " << hex::to_hex0x32(val);
    }

    regs.set(rd, val);
    pc += 4;
}

// Execute srli instruction.
void rv32i_hart::exec_srli(uint32_t insn, std::ostream* pos)
{
    uint32_t rd = get_rd(insn);
    uint32_t rs1 = regs.get(get_rs1(insn));
    uint32_t shamt = get_imm_i(insn) % XLEN;
    uint32_t val = rs1 >> shamt;

    if (pos)
    {
        std::string s = render_itype_alu(insn, "srli", shamt);
        *pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
        *pos << "// " << render_reg(rd) << " = " << hex::to_hex0x32(rs1) << " >> "
             << shamt << " = " << hex::to_hex0x32(val);
    }

    regs.set(rd, val);
    pc += 4;
}

// Execute srai instruction.
void rv32i_hart::exec_srai(uint32_t insn, std::ostream* pos)
{
    uint32_t rd = get_rd(insn);
    int32_t rs1 = regs.get(get_rs1(insn));
    uint32_t shamt = get_imm_i(insn) % XLEN;
    int32_t val = rs1 >> shamt;

    if (pos)
    {
        std::string s = render_itype_alu(insn, "srai", shamt);
        *pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
        *pos << "// " << render_reg(rd) << " = " << hex::to_hex0x32(rs1) << " >> "
             << shamt << " = " << hex::to_hex0x32(val);
    }

    regs.set(rd, val);
    pc += 4;
}

// Execute add instruction.
void rv32i_hart::exec_add(uint32_t insn, std::ostream* pos)
{
    uint32_t rd = get_rd(insn);
    int32_t rs1 = regs.get(get_rs1(insn));
    int32_t rs2 = regs.get(get_rs2(insn));
    int32_t val = rs1 + rs2;

    if (pos)
    {
        std::string s = render_rtype(insn, "add");
        *pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;   
        *pos << "// " << render_reg(rd) << " = " << hex::to_hex0x32(rs1) << " + "
             << hex::to_hex0x32(rs2) << " = " << hex::to_hex0x32(val);
    }

    regs.set(rd, val);
    pc += 4;
}

// Execute sub instruction.
void rv32i_hart::exec_sub(uint32_t insn, std::ostream* pos)
{
    uint32_t rd = get_rd(insn);
    int32_t rs1 = regs.get(get_rs1(insn));
    int32_t rs2 = regs.get(get_rs2(insn));
    int32_t val = rs1 - rs2;

    if (pos)
    {
        std::string s = render_rtype(insn, "sub");
        *pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;   
        *pos << "// " << render_reg(rd) << " = " << hex::to_hex0x32(rs1) << " - "
             << hex::to_hex0x32(rs2) << " = " << hex::to_hex0x32(val);
    }

    regs.set(rd, val);
    pc += 4;
}

// Execute sll instruction.
void rv32i_hart::exec_sll(uint32_t insn, std::ostream* pos)
{
    uint32_t rd = get_rd(insn);
    int32_t rs1 = regs.get(get_rs1(insn));
    uint32_t shamt = regs.get(get_rs2(insn)) & 0x1f;
    int32_t val = rs1 << shamt;

    if (pos)
    {
        std::string s = render_rtype(insn, "sll");
        *pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;   
        *pos << "// " << render_reg(rd) << " = " << hex::to_hex0x32(rs1) << " << "
             << shamt << " = " << hex::to_hex0x32(val);
    }

    regs.set(rd, val);
    pc += 4;
}

// Execute slt instruction
void rv32i_hart::exec_slt(uint32_t insn, std::ostream* pos)
{
    uint32_t rd = get_rd(insn);
    int32_t rs1 = regs.get(get_rs1(insn));
    int32_t rs2 = regs.get(get_rs2(insn));
    uint32_t val = rs1 < rs2 ? 1 : 0;

    if (pos)
    {
        std::string s = render_rtype(insn, "slt");
        *pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
        *pos << "// " << render_reg(rd) << " = (" << hex::to_hex0x32(rs1) << " < " 
             << hex::to_hex0x32(rs2) << ") ? 1 : 0 = " << hex::to_hex0x32(val);
    }

    regs.set(rd, val);
    pc += 4;
}

// Execute sltu instruction
void rv32i_hart::exec_sltu(uint32_t insn, std::ostream* pos)
{
    uint32_t rd = get_rd(insn);
    uint32_t rs1 = regs.get(get_rs1(insn));
    uint32_t rs2 = regs.get(get_rs2(insn));
    uint32_t val = rs1 < rs2 ? 1 : 0;

    if (pos)
    {
        std::string s = render_rtype(insn, "sltu");
        *pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
        *pos << "// " << render_reg(rd) << " = (" << hex::to_hex0x32(rs1) << " <U " 
             << hex::to_hex0x32(rs2) << ") ? 1 : 0 = " << hex::to_hex0x32(val);
    }

    regs.set(rd, val);
    pc += 4;
}

// Execute xor instruction.
void rv32i_hart::exec_xor(uint32_t insn, std::ostream* pos)
{
    uint32_t rd = get_rd(insn);
    int32_t rs1 = regs.get(get_rs1(insn));
    int32_t rs2 = regs.get(get_rs2(insn));
    int32_t val = rs1 ^ rs2;

    if (pos)
    {
        std::string s = render_rtype(insn, "xor");
        *pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
        *pos << "// " << render_reg(rd) << " = " << hex::to_hex0x32(rs1) << " ^ "
             << hex::to_hex0x32(rs2) << " = " << hex::to_hex0x32(val);
    }

    regs.set(rd, val);
    pc += 4;
}

// Execute srl instruction.
void rv32i_hart::exec_srl(uint32_t insn, std::ostream* pos)
{
    uint32_t rd = get_rd(insn);
    uint32_t rs1 = regs.get(get_rs1(insn));
    uint32_t shamt = regs.get(get_rs2(insn)) & 0x1f;
    uint32_t val = rs1 >> shamt;

    if (pos)
    {
        std::string s = render_rtype(insn, "srl");
        *pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
        *pos << "// " << render_reg(rd) << " = " << hex::to_hex0x32(rs1) << " >> "
             << shamt << " = " << hex::to_hex0x32(val);
    }

    regs.set(rd, val);
    pc += 4;
}

// Execute sra instruction.
void rv32i_hart::exec_sra(uint32_t insn, std::ostream* pos)
{
    uint32_t rd = get_rd(insn);
    int32_t rs1 = regs.get(get_rs1(insn));
    uint32_t shamt = regs.get(get_rs2(insn)) & 0x1f;
    int32_t val = rs1 >> shamt;

    if (pos)
    {
        std::string s = render_rtype(insn, "sra");
        *pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
        *pos << "// " << render_reg(rd) << " = " << hex::to_hex0x32(rs1) << " >> "
             << shamt << " = " << hex::to_hex0x32(val);
    }

    regs.set(rd, val);
    pc += 4;
}

// Execute or instruction.
void rv32i_hart::exec_or(uint32_t insn, std::ostream* pos)
{
    uint32_t rd = get_rd(insn);
    int32_t rs1 = regs.get(get_rs1(insn));
    int32_t rs2 = regs.get(get_rs2(insn));
    int32_t val = rs1 | rs2;

    if (pos)
    {
        std::string s = render_rtype(insn, "or");
        *pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
        *pos << "// " << render_reg(rd) << " = " << hex::to_hex0x32(rs1) << " | "
             << hex::to_hex0x32(rs2) << " = " << hex::to_hex0x32(val);
    }

    regs.set(rd, val);
    pc += 4;
}

// Execute and instruction.
void rv32i_hart::exec_and(uint32_t insn, std::ostream* pos)
{
    uint32_t rd = get_rd(insn);
    int32_t rs1 = regs.get(get_rs1(insn));
    int32_t rs2 = regs.get(get_rs2(insn));
    int32_t val = rs1 & rs2;

    if (pos)
    {
        std::string s = render_rtype(insn, "and");
        *pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
        *pos << "// " << render_reg(rd) << " = " << hex::to_hex0x32(rs1) << " & "
             << hex::to_hex0x32(rs2) << " = " << hex::to_hex0x32(val);
    }

    regs.set(rd, val);
    pc += 4;
}

// Execute csrrs instruction.
void rv32i_hart::exec_csrrs(uint32_t insn, std::ostream* pos)
{
    uint32_t rd = get_rd(insn);
    uint32_t csr = get_imm_i(insn);
    uint32_t rs1 = regs.get(get_rs1(insn));
    uint32_t val = csr & rs1;

    if (pos)
    {
        std::string s = render_csrrx(insn, "csrrs");
        *pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
        *pos << "// " << render_reg(rd) << " = " << val;
    }

    regs.set(rd, val);
    pc += 4;  
}
