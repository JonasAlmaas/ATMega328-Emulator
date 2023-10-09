#include "ATMega328Emulator/CPU.h"

#include <iostream>

#include "ATMega328Emulator/Instructions.h"

namespace ATMega328Emulator {
	
	void CPU::Reset(Memory& memory)
	{
		// I have no idea if this is correct
		PC = 0x0;
		SPL = 0x100; // First address of SRAM
		SPH = 0x0;
		
		SREG.I = SREG.T = SREG.H = SREG.S = SREG.V = SREG.N = SREG.Z = SREG.C = 0;
		
		memory.Initialize();
	}

	void CPU::Execute(int cycles, Memory& memory)
	{
		using namespace Instructions;
	
		while (cycles > 0) {
			Word instruction = FetchWord(cycles, memory);

			if ((instruction & OpcodeMask::ADC) == ADC) {
				Handle_ADC(instruction, this);
			}
			else if ((instruction & OpcodeMask::ADD) == ADD) {
				Handle_ADD(instruction, this);
			}
			else if ((instruction & OpcodeMask::ADIW) == ADIW) {
				Handle_ADIW(instruction, this);
			}
			else if ((instruction & OpcodeMask::AND) == AND) {
				Handle_AND(instruction, this);
			}
			else if ((instruction & OpcodeMask::ANDI) == ANDI) {
				Handle_ANDI(instruction, this);
			}
			else {
				std::cout << "Instruction not handled " << std::hex << instruction << std::endl;
			}
		}
	}

}
