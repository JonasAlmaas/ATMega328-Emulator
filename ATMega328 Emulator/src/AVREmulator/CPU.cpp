#include "AVREmulator/CPU.h"

#include <iostream>

#include "AVREmulator/Instructions.h"

void CPU::Reset(Memory& memory)
{
	memory.Initialize();
}

void CPU::Execute(uint32_t cycles, Memory& memory)
{
	using namespace Instructions;
	
	while (cycles > 0) {
		Word instruction = FetchWord(cycles, memory);

		if ((instruction & OpcodeMask::ADC) == ADC) {
			Handle_ADC(instruction, memory, this);
		}
		else if ((instruction & OpcodeMask::ADD) == ADD) {
			Handle_ADD(instruction, memory, this);
		}
		else if ((instruction & OpcodeMask::ADIW) == ADIW) {
			Handle_ADIW(instruction, memory, this);
		}
		else if ((instruction & OpcodeMask::AND) == AND) {
			Handle_AND(instruction, memory, this);
		}
		else if ((instruction & OpcodeMask::ANDI) == ANDI) {
			Handle_ANDI(instruction, memory, this);
		}
		else {
			std::cout << "Instruction not handled " << std::hex << instruction << std::endl;
		}
	}
}
