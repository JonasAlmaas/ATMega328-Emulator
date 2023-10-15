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
		while (cycles > 0) {
			Word instruction = FetchWord(cycles, memory);

			if (!handleInstruction(instruction, cycles, memory)) {
				std::cout << "Instruction not handled " << std::hex << instruction << std::endl;
			}
		}
	}

	bool CPU::handleInstruction(Word instruction, int& cycles, Memory& memory)
	{
		using namespace Instruction;
		
		switch (instruction & 0b1111'1100'0000'0000)
		{
			case ADC: Handle_ADC(instruction, this); return true;
			case ADD: Handle_ADD(instruction, this); return true;
			case AND: Handle_AND(instruction, this); return true;
			case EOR: Handle_EOR(instruction, this); return true;
			default: break;
		}
		
		switch (instruction & 0b1111'1111'0000'0000)
		{
			case ADIW: Handle_ADIW(instruction, cycles, this); return true;
			default: break;
		}
		
		switch (instruction & 0b1111'0000'0000'0000)
		{
			case ANDI: Handle_ANDI(instruction, this); return true;
			default: break;
		}
		
		switch (instruction & 0b1111'1110'0000'1111)
		{
			case COM: Handle_COM(instruction, this); return true;
			case DEC: Handle_DEC(instruction, this); return true;
			case INC: Handle_INC(instruction, this); return true;
			case NEG: Handle_NEG(instruction, this); return true;
			default: break;
		}
		
		return false;
	}

}
