# RISC-V Simulator

 A memory simulator capable of executing GCC-compiled binaries using the RV32I instruction set.

Written for CSCI 463 (Computer Architecture and Systems Organization) at Northern Illinois University.

## Usage

    $ rv32i [-d] [-i] [-r] [-z] [-l exec-limit] [-m hex-mem-size] infile 

### Options

| Option | Description | Default Value
|-|-|-
| l | Maximum number of instructions to execute | Unlimited
| m | Set memory size | 0x100

### Flags

| Flag | Description
|-|-
| d | Show disassembly before program execution
| i | Print instructions during execution
| r | Print registers during execution
| z | Dump registers & memory on program completion
