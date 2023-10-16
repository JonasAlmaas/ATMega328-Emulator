#pragma once

#include "ATMega328Emulator/CPU.h"

/*
 * Instructions w/o testing
 *
 * ASR
 * BCLR
 * BLD
 */

namespace ATMega328Emulator {
	
	namespace Instruction {

		// d - Destination (and source) register in the Register File
		// r - Source register in the Register File
		// K - Constant data
		// k - Constant address
		// b - Bit in the Register File or I/O Register (3-bit)
		// s - Bit in the Status Register (3-bit)

		// A - I/O location address
	
		static constexpr Word
			ADC  = 0b0001'1100'0000'0000, // ADC  - Add with Carry                - 0001'11rd'dddd'rrrr
			ADD  = 0b0000'1100'0000'0000, // ADD  - Add without Carry             - 0000'11rd'dddd'rrrr
			ADIW = 0b1001'0110'0000'0000, // ADIW - Add Immediate to Word         - 1001'0110'KKdd'KKKK
			AND  = 0b0010'0000'0000'0000, // AND  - Logical AND                   - 0010'00rd'dddd'rrrr
			ANDI = 0b0111'0000'0000'0000, // ANDI - Logical AND with Immediate    - 0111'KKKK'dddd'KKKK
			ASR  = 0b1001'0100'0000'0101, // ASR  - Arithmetic Shift Right        - 1001'010d'dddd'0101
			BCLR = 0b1001'0100'1000'1000, // BCLR - Bit Clear in SREG             - 1001'0100'1sss'1000
			BLD  = 0b1111'1000'0000'0000, // BLD  - Bit Load from the T Flag      - 1111'100d'dddd'0bbb
			                              // CBR  - Clear Bits in Register - ANDI Rd,(0xFF-K) (Handled by assembler)
			                              // CLR  - Clear Register - EOR Rd,Rd (Handled by assembler)
			COM  = 0b1001'0100'0000'0000, // COM  - One's Complement              - 1001'010d'dddd'0000
			DEC  = 0b1001'0100'0000'1010, // DEC  - Decrement                     - 1001'010d'dddd'1010
			EOR  = 0b0010'0100'0000'0000, // EOR  - Logical Exclusive OR          - 0010'01rd'dddd'rrrr
			INC  = 0b1001'0100'0000'0011, // INC  - Increment                     - 1001'010d'dddd'0011
			NEG  = 0b1001'0100'0000'0001, // NEG  - Two's Complement              - 1001'010d'dddd'0001
			OR   = 0b0010'1000'0000'0000, // OR   - Logical OR                    - 0010'10rd'dddd'rrrr
			ORI  = 0b0110'0000'0000'0000, // ORI  - Logical OR with Immediate     - 0110'KKKK'dddd'KKKK
			SBC  = 0b0000'1000'0000'0000, // SBC  - Subtract with Carry           - 0000'10rd'dddd'rrrr
			SBCI = 0b0100'0000'0000'0000, // SBCI - Subtract Immediate with Carry - 0100'KKKK'dddd'KKKK
			SBI  = 0b1001'1010'0000'0000, // SBI  - Set Bit in I/O Register       - 1001'1010'AAAA'Abbb
			                              // SBR  - ORI Rd,K (Handled by assembler)
			                              // SER  - LDI Rd,0xFF (Handled by assembler)
			SUB  = 0b0001'1000'0000'0000, // SBC  - Subtract without Carry        - 0001'10rd'dddd'rrrr
			SUBI = 0b0101'0000'0000'0000; // SUBI - Subtract Immediate            - 0101'KKKK'dddd'KKKK
			                              // TST  - Test for Zero or Minus - AND Rd,Rd (Handled by assembler)

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

		// ASR - Arithmetic Shift Right
		void Handle_ASR(Word instruction, CPU* cpu);

		// BCLR - Bit Clear in SREG
		void Handle_BCLR(Word instruction, CPU* cpu);

		// BLD - Bit Load from the T Flag
		void Handle_BLD(Word instruction, CPU* cpu);

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

		// ORI - Logical OR with Immediate
		void Handle_ORI(Word instruction, CPU* cpu);

		// SBC - Subtract with Carry
		void Handle_SBC(Word instruction, CPU* cpu);

		// SBCI - Subtract Immediate with Carry SBI
		void Handle_SBCI(Word instruction, CPU* cpu);

		// SBI - Set Bit in I/O Register
		void Handle_SBI(Word instruction, CPU* cpu);

		// SUBI - Subtract Immediate
		void Handle_SUBI(Word instruction, CPU* cpu);

		// SUB - Subtract without Carry
		void Handle_SUB(Word instruction, CPU* cpu);
		
	}
}
