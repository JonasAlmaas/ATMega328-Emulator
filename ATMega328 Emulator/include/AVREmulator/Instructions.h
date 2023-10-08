#pragma once

#include "AVREmulator/CPU.h"

// Minimal AVR1 Core:
//	ADC - DONE
//	ADD - DONE
//	AND - DONE
//	ANDI - DONE
//	CBR - DONE
//	CLR - DONE
//	COM - DONE
//	DEC
//	EOR
//	INC
//	NEG
//	OR
//	ORI
//	SBC
//	SBCI
//	SBR
//	SER
//	SUB
//	SUBI
//	TST

namespace Instructions {
	
	static constexpr Word
		ADC = 0b0001'1100'0000'0000, // ADC - Add with Carry - 0001'11rd'dddd'rrrr;
		ADD = 0b0000'1100'0000'0000, // ADD - Add without Carry - 0000'11rd'dddd'rrrr
		ADIW = 0b1001'0110'0000'0000, // ADIW - Add Immediate to Word
		AND = 0b0010'0000'0000'0000, // AND - Logical AND - 0010'00rd'dddd'rrrr
		ANDI = 0b0111'0000'0000'0000, // ANDI - Logical AND with Immediate - 0111'KKKK'dddd'KKKK
		CBR = 0b0, // CBR - Clear Bits in Register - TODO: Figure this one out
		CLR = 0b0010'0100'0000'000, // CBR - Clear Bits in Register - 0010'01dd'dddd'dddd
		COM = 0b1001'0100'0000'0000; // COM - One's Complement - 1001'010d'dddd'0000
	
	namespace OpcodeMask {

		static constexpr Word
			ADC = 0b1111'1100'0000'0000,
			ADD = 0b1111'1100'0000'0000,
			ADIW = 0b1111'1111'0000'0000,
			AND = 0b1111'1100'0000'0000,
			ANDI = 0b1111'0000'0000'0000,
			CBR = 0b0, // TODO: Figure this one out
			CLR = 0b1111'1100'0000'0000,
			COM = 0b1111'1110'0000'1111;
		
	}
	
	// ADC - Add with Carry
	void Handle_ADC(Word instruction, Memory& memory, CPU* cpu);

	// ADD - Add without Carry
	void Handle_ADD(Word instruction, Memory& memory, CPU* cpu);

	// ADIW - Add Immediate to Word
	void Handle_ADIW(Word instruction, Memory& memory, CPU* cpu);

	// AND - Logical AND
	void Handle_AND(Word instruction, Memory& memory, CPU* cpu);

	// ANDI - Logical AND with Immediate
	void Handle_ANDI(Word instruction, Memory& memory, CPU* cpu);

	// CBR - Clear Bits in Register
	//void Handle_CBR(uint32_t& cycles, Memory& memory, CPU* cpu);

	// CLR - Clear Register
	//void Handle_CLR(uint32_t& cycles, Memory& memory, CPU* cpu);

	// COM - One's Complement
	//void Handle_COM(uint32_t& cycles, Memory& memory, CPU* cpu);
	
}
