#include "ATMega328Emulator/CPU.h"

#include <iostream>

#include "ATMega328Emulator/Instructions.h"

namespace ATMega328Emulator {
	
	void CPU::Reset(Memory& memory)
	{
		// I have no idea if this is correct
		PC = 0x0;
		IO.SP = 0x100; // First address of SRAM (Not true atm)

		IO.PORTB = IO.PORTC = IO.PORTD = 0;
		IO.DDRB = IO.DDRC = IO.DDRD = 0;
		IO.PINB = IO.PINC = IO.PIND = 0;

		*(Byte*)&IO.SREG = 0;
		
		memory.Initialize();
	}

	void CPU::Execute(int cycles, Memory& memory)
	{
		while (cycles > 0) {
			Word instruction = FetchWord(cycles, memory);

			bool success = handleInstruction(instruction, cycles, memory);

			if (!success) {
				std::cout << "Instruction not handled " << std::hex << instruction << std::endl;
			}
		}
	}

	bool CPU::handleInstruction(Word instruction, int& cycles, Memory& memory)
	{
		using namespace Instruction;

		switch (instruction)
		{
			case BREAK: return true; // I don't know how to implement the break instruction
			case NOP: return true;
			default: break;
		}
		
		switch (instruction & 0b1111'1111'1000'1111)
		{
			case BSET: Handle_BSET(instruction, this); return true;
			default: break;
		}

		switch (instruction & 0b1111'1110'1000'1111)
		{
			case BCLR: Handle_BCLR(instruction, this); return true;
			default: break;
		}
		
		switch (instruction & 0b1111'1110'0000'1111)
		{
			case ASR: Handle_ASR(instruction, this); return true;
			case COM: Handle_COM(instruction, this); return true;
			case DEC: Handle_DEC(instruction, this); return true;
			case INC: Handle_INC(instruction, this); return true;
			case LAC: Handle_LAC(instruction, cycles, this); return true;
			case LAS: Handle_LAS(instruction, cycles, this); return true;
			case LAT: Handle_LAT(instruction, cycles, this); return true;
			case LDI: Handle_LDI(instruction, this); return true;
			case LDS: Handle_LDS(instruction, cycles, this, memory); return true;
			case LSR: Handle_LSR(instruction, this); return true;
			case NEG: Handle_NEG(instruction, this); return true;
			default: break;
		}
		
		switch (instruction & 0b1111'1111'1000'1000)
		{
			case FMUL: Handle_FMUL(instruction, cycles, this); return true;
			case FMULS: Handle_FMULS(instruction, cycles, this); return true;
			case FMULSU: Handle_FMULSU(instruction, cycles, this); return true;
			case MULSU: Handle_MULSU(instruction, cycles, this); return true;
			default: break;
		}
		
		switch (instruction & 0b1111'1111'0000'0000)
		{
			case ADIW: Handle_ADIW(instruction, cycles, this); return true;
			case CBI: Handle_CBI(instruction, this); return true;
			case MOVW: Handle_MOVW(instruction, this); return true;
			case SBI: Handle_SBI(instruction, this); return true;
			default: break;
		}
		
		switch (instruction & 0b1111'1110'0000'1000)
		{
			case BLD: Handle_BLD(instruction, this); return true;
			case BST: Handle_BST(instruction, this); return true;
			default: break;
		}

		switch (instruction & 0b1111'1100'0000'0000)
		{
			case ADC: Handle_ADC(instruction, this); return true;
			case ADD: Handle_ADD(instruction, this); return true;
			case AND: Handle_AND(instruction, this); return true;
			case BRBC: Handle_BRBC(instruction, cycles, this); return true;
			case BRBS: Handle_BRBS(instruction, cycles, this); return true;
			case CP: Handle_CP(instruction, this); return true;
			case CPC: Handle_CPC(instruction, this); return true;
			case CPSE: Handle_CPSE(instruction, cycles, this); return true;
			case EOR: Handle_EOR(instruction, this); return true;
			case MOV: Handle_MOV(instruction, this); return true;
			case MUL: Handle_MUL(instruction, cycles, this); return true;
			case MULS: Handle_MULS(instruction, cycles, this); return true;
			case OR: Handle_OR(instruction, this); return true;
			case SBC: Handle_SBC(instruction, this); return true;
			case SUB: Handle_SUB(instruction, this); return true;
			default: break;
		}

		switch (instruction & 0b1111'0000'0000'0000)
		{
			case ANDI: Handle_ANDI(instruction, this); return true;
			case CPI: Handle_CPI(instruction, this); return true;
			case ORI: Handle_ORI(instruction, this); return true;
			case SBCI: Handle_SBCI(instruction, this); return true;
			case SUBI: Handle_SUBI(instruction, this); return true;
			default: break;
		}

		return false;
	}

}
