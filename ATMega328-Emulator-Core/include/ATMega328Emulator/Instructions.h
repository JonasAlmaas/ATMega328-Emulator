#pragma once

#include "ATMega328Emulator/CPU.h"

// Minimal AVR1 Core:
// ADC  - IMPL
// ADD  - IMPL - TESTED
// AND  - IMPL
// ANDI - IMPL - TESTED
// CBR  - ANDI with inverted mask (Handled by assembler)
// CLR  - EOR with itself (Handled by assembler)
// COM  - IMPL - TESTED
// DEC  - IMPL - TESTED
// EOR  - IMPL - TESTED
// INC  - IMPL - TESTED
// NEG  - IMPL - TESTED
// OR
// ORI
// SBC
// SBCI
// SBR
// SER
// SUB
// SUBI
// TST

namespace ATMega328Emulator {
	
	namespace Instruction {

		// d - Destination (and source) register in the Register File
		// r - Source register in the Register File
		// K - Constant data
		// k - Constant address
	
		static constexpr Word
			ADC  = 0b0001'1100'0000'0000, // ADC  - Add with Carry             - 0001'11rd'dddd'rrrr
			ADD  = 0b0000'1100'0000'0000, // ADD  - Add without Carry          - 0000'11rd'dddd'rrrr
			ADIW = 0b1001'0110'0000'0000, // ADIW - Add Immediate to Word      - 1001'0110'KKdd'KKKK
			AND  = 0b0010'0000'0000'0000, // AND  - Logical AND                - 0010'00rd'dddd'rrrr
			ANDI = 0b0111'0000'0000'0000, // ANDI - Logical AND with Immediate - 0111'KKKK'dddd'KKKK
			COM  = 0b1001'0100'0000'0000, // COM  - One's Complement           - 1001'010d'dddd'0000
			DEC  = 0b1001'0100'0000'1010, // DEC  - Decrement                  - 1001'010d'dddd'1010
			EOR  = 0b0010'0100'0000'0000, // EOR  - Logical Exclusive OR       - 0010'01rd'dddd'rrrr
			INC  = 0b1001'0100'0000'0011, // INC  - Increment                  - 1001'010d'dddd'0011
			NEG  = 0b1001'0100'0000'0001, // NEG  - Two's Complement           - 1001'010d'dddd'0001
			OR   = 0b0010'1000'0000'0000; // OR   - Logical OR                 - 0010'10rd'dddd'rrrr

		// ADC - Add with Carry
		void Handle_ADC(Word instruction, CPU* cpu);

		// ADD - Add without Carry
		void Handle_ADD(Word instruction, CPU* cpu);
		
		// ADIW - Add Immediate to Word
		void Handle_ADIW(Word instruction, int& cycles, CPU* cpu);

		// AND - Logical AND
		void Handle_AND(Word instruction, CPU* cpu);

		// ANDI - Logical AND with Immediate
		void Handle_ANDI(Word instruction, CPU* cpu);
		
		// COM - One's Complement
		void Handle_COM(Word instruction, CPU* cpu);

		// DEC - Decrement
		void Handle_DEC(Word instruction, CPU* cpu);

		// EOR - Logical Exclusive OR
		void Handle_EOR(Word instruction, CPU* cpu);

		// INC - Increment
		void Handle_INC(Word instruction, CPU* cpu);

		// NEG - Two's Complement
		void Handle_NEG(Word instruction, CPU* cpu);
		
		// OR - Logical OR
		void Handle_OR(Word instruction, CPU* cpu);

	}
}
