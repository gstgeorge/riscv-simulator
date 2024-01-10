#
# PROGRAM: RISC-V Simulator
# AUTHOR:  Gavin St. George (Z1909350)
#

CXXFLAGS = -g -ansi -pedantic -Wall -Werror -std=c++14

rv32i: main.o rv32i_decode.o memory.o hex.o registerfile.o rv32i_hart.o cpu_single_hart.o
	g++ $(CXXFLAGS) -o rv32i $^

main.o: main.cpp

rv32i_decode.o: rv32i_decode.cpp rv32i_decode.h

memory.o: memory.cpp memory.h

hex.o: hex.cpp hex.h

registerfile.o: registerfile.cpp registerfile.h

rv32i_hart.o: rv32i_hart.cpp rv32i_hart.h

cpu_single_hart.o: cpu_single_hart.cpp cpu_single_hart.h

clean:
	rm -f *.o rv32i

