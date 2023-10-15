#include "ATMega328Emulator/CPU.h"

#include <iostream>

#include "ATMega328Emulator/Instructions.h"

namespace ATMega328Emulator {
	
	void CPU::Reset(Memory& memory)
	{
		// I have no idea if this is correct
		PC = 0x0;
		SPL = 0x100; // First address of SRAM (Not true atm)
		SPH = 0x0;
		
		SREG.I = SREG.T = SREG.H = SREG.S = SREG.V = SREG.N = SREG.Z = SREG.C = 0;
		
		memory.Initialize();
	}

	void CPU::Execute(int cycles, Memory& memory)
	{
		using namespace Instructions;
	
		while (cycles > 0) {
			Word instruction = FetchWord(cycles, memory);

			switch (instruction & 0b1111'1100'0000'0000)
			{
				case ADC: return Handle_ADC(instruction, this);
				case ADD: return Handle_ADD(instruction, this);
				case AND: return Handle_AND(instruction, this);
				case EOR: return Handle_EOR(instruction, this);
				default: break;
			}

			switch (instruction & 0b1111'1111'0000'0000)
			{
				case ADIW: return Handle_ADIW(instruction, cycles, this);
				default: break;
			}

			switch (instruction & 0b1111'0000'0000'0000)
			{
				case ANDI: return Handle_ANDI(instruction, this);
				default: break;
			}
			
			switch (instruction & 0b1111'1110'0000'1111)
			{
				case COM: return Handle_COM(instruction, this);
				case DEC: return Handle_DEC(instruction, this);
				case INC: return Handle_INC(instruction, this);
				default: break;
			}

			std::cout << "Instruction not handled " << std::hex << instruction << std::endl;
		}
	}

}
