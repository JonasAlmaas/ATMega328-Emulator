#pragma once

#include "ATMega328Emulator/CPU.h"

/*
 * Instructions w/o testing
 *
 * ASR
 * BCLR
 * BLD
 * BRBC
 * BRBS
 * BREAK (Not sure how to implement)
 * BSET
 * BST
 * CALL (Not sure how to implement)
 * CBI
 * CP
 * CPC
 * CPI
 * CPSE
 * DES (Not sure how to implement)
 * EICALL (Not sure how to implement)
 * EIJMP (Not sure how to implement)
 * ELPM (Not sure how to implement) (This one is strange)
 * FMUL
 * FMULS
 * FMULSU
 * ICALL (Not implemented)
 * IN (Not implemented)
 * JMP (Not implemented)
 * LAC
 * LAS
 * LAT
 * LD (Not sure how to implement) (This one is strange)
 * LDI
 * LDS
 * LPM (Not sure how to implement) (This one is strange)
 * LSL
 * LSR
 * MOV
 * MOVW
 * MUL
 * MULS
 * MULSU
 * NOP
 * OUT
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
			ADC    = 0b0001'1100'0000'0000, // ADC    - Add with Carry                           - 0001'11rd'dddd'rrrr
			ADD    = 0b0000'1100'0000'0000, // ADD    - Add without Carry                        - 0000'11rd'dddd'rrrr
			ADIW   = 0b1001'0110'0000'0000, // ADIW   - Add Immediate to Word                    - 1001'0110'KKdd'KKKK
			AND    = 0b0010'0000'0000'0000, // AND    - Logical AND                              - 0010'00rd'dddd'rrrr
			ANDI   = 0b0111'0000'0000'0000, // ANDI   - Logical AND with Immediate               - 0111'KKKK'dddd'KKKK
			ASR    = 0b1001'0100'0000'0101, // ASR    - Arithmetic Shift Right                   - 1001'010d'dddd'0101
			BCLR   = 0b1001'0100'1000'1000, // BCLR   - Bit Clear in SREG                        - 1001'0100'1sss'1000
			BLD    = 0b1111'1000'0000'0000, // BLD    - Bit Load from the T Flag                 - 1111'100d'dddd'0bbb
			BRBC   = 0b1111'0100'0000'0000, // BRBC   - Branch if Bit in SREG is Clear           - 1111'01kk'kkkk'ksss
			BRBS   = 0b1111'0000'0000'0000, // BRBS   - Branch if Bit in SREG is Set             - 1111'00kk'kkkk'ksss
			                                // BRCC   - Branch if Carry Clear                    - BRBC 0,k
			                                // BRBS   - Branch if Carry Set                      - BRBS 0,k
			BREAK  = 0b1001'0101'1001'1000, // BREAK  - Break                                    - 1001'0101'1001'1000
			                                // BREQ   - Branch if Equal                          - BRBS 1,k
			                                // BRGE   - Branch if Greater or Equal (Signed)      - BRBC 4,k
			                                // BRHC   - Branch if Half Carry Clear               - BRBC 5,k
			                                // BRHS   - Branch if Half Carry Set                 - BRBS 5,k
			                                // BRID   - Branch if Global Interrupt is Disabled   - BRBC 7,k
			                                // BRIE   - Branch if Global Interrupt is Enabled    - BRBS 7,k
			                                // BRLO   - Branch if Lower                          - BRBS 0,k
			                                // BRLT   - Branch if Less Than (Signed)             - BRBS 4,k
			                                // BRMI   - Branch if Minus                          - BRBS 2,k
			                                // BRNE   - Branch if Not Equal                      - BRBC 1,k
			                                // BRPL   - Branch if Plus                           - BRBC 2,k
			                                // BRSH   - Branch if Same or Higher (Unsigned)      - BRBC 0,k
			                                // BRTC   - Branch if the T Flag is Cleared          - BRBC 6,k
			                                // BRTS   - Branch if the T Flag is Set              - BRBS 6,k
			                                // BRVC   - Branch if Overflow Cleared               - BRBC 3,k
			                                // BRVS   - Branch if Overflow Set                   - BRBS 3,k
			BSET =  0b1001'0100'0000'1000,  // BSET   - Bit Set in SREG                          - 1001'0100'0sss'1000
			BST  =  0b1111'1010'0000'0000,  // BST    - Bit Store from Bit in Register to T Flag - 1111'101d'dddd'0bbb
			CALL =  0b1001'0100'0000'1110,  // CALL   - Long Call to a Subroutine                - 1001'010k'kkkk'111k'kkkk'kkkk'kkkk'kkkk
			CBI  =  0b1001'1000'0000'0000,  // CBI    - Clear Bit in I/O Register                - 1001'1000'AAAA'Abbb
			                                // CBR    - Clear Bits in Register                   - ANDI Rd,(0xFF-K)
			                                // CLC    - Clear Carry Flag                         - BCLR 0
			                                // CLH    - Clear Half Carry Flag                    - BCLR 5
			                                // CLI    - Clear Global Interrupt Flag              - BCLR 7
			                                // CLN    - Clear Negative Flag                      - BCLR 2
			                                // CLR    - Clear Register                           - EOR Rd,Rd
			                                // CLS    - Clear Signed Flag                        - BCLR 4
			                                // CLT    - Clear T Flag                             - BCLR 6
			                                // CLV    - Clear Overflow Flag                      - BCLR 3
			                                // CLZ    - Clear Zero Flag                          - BCLR 1
			COM    = 0b1001'0100'0000'0000, // COM    - One's Complement                         - 1001'010d'dddd'0000
			CP     = 0b0001'0100'0000'0000, // CP     - Compare                                  - 0001'01rd'dddd'rrrr
			CPC    = 0b0000'0100'0000'0000, // CPC    - Compare with Carry                       - 0000'01rd'dddd'rrrr
			CPI    = 0b0011'0000'0000'0000, // CPI    - Compare with Immediate                   - 0011'KKKK'dddd'KKKK
			CPSE   = 0b0001'0000'0000'0000, // CPSE   - Compare Skip if Equal                    - 0001'00rd'dddd'rrrr
			DEC    = 0b1001'0100'0000'1010, // DEC    - Decrement                                - 1001'010d'dddd'1010
			DES    = 0b1001'0100'0000'1011, // DES    - Data Encryption Standard                 - 1001'0100'KKKK'1011
			EICALL = 0b1001'0101'0001'1001, // EICALL - Extended Indirect Call to Subroutine     - 1001'0101'0001'1001
			EIJMP  = 0b1001'0100'0001'1001, // EIJMP  - Extended Indirect Jump                   - 1001'0100'0001'1001
			ELPM   = 0, // TODO             // ELPM   - Extended Load Program Memory             - 1001'0101'1101'1000
			EOR    = 0b0010'0100'0000'0000, // EOR    - Logical Exclusive OR                     - 0010'01rd'dddd'rrrr
			FMUL   = 0b0000'0011'0000'1000, // FMUL   - Fractional Multiply (Unsigned)           - 0000'0011'0ddd'1rrr
			FMULS  = 0b0000'0011'1000'0000, // FMULS  - Fractional Multiply (Signed)             - 0000'0011'1ddd'0rrr
			FMULSU = 0b0000'0011'1000'1000, // FMULSU - Fractional Multiply Signed with Unsigned - 0000'0011'1ddd'1rrr
			ICALL  = 0b1001'0101'0000'1001, // ICALL  - Indirect Call to Subroutine              - 1001'0101'0000'1001
			IJMP   = 0b1001'0100'0000'1001, // IJMP   - Indirect Jump                            - 1001'0100'0000'1001
			IN     = 0b1011'0000'0000'0000, // IN     - Load an I/O Location to Register         - 1011'0AAd'dddd'AAAA
			INC    = 0b1001'0100'0000'0011, // INC    - Increment                                - 1001'010d'dddd'0011
			JMP    = 0b1001'0100'0000'1100, // JMP    - Jump                                     - 1001'010k'kkkk'110k'kkkk'kkkk'kkkk'kkkk
			LAC    = 0b1001'0010'0000'0110, // LAC    - Load and Clear                           - 1001'001r'rrrr'0110
			LAS    = 0b1001'0010'0000'0101, // LAS    - Load and Set                             - 1001'001r'rrrr'0101
			LAT    = 0b1001'0010'0000'0111, // LAT    - Load and Toggle                          - 1001'001r'rrrr'0111
			LD     = 0, // TODO             // LD     - Load Indirect from Data Space to Register using X
			LDI    = 0b1110'0000'0000'0000, // LDI    - Load Immediate                           - 1110'KKKK'dddd'KKKK
			LDS    = 0b1001'0000'0000'0000, // LDS    - Load Direct from Data Space              - 1001'000d'dddd'0000'kkkk'kkkk'kkkk'kkkk
			LPM    = 0, // TODO             // LPM    - Load Program Memory
			LSL    = 0b0000'1100'0000'0000, // LSL    - Logical Shift Left                       - ADD Rd,Rd
			LSR    = 0b1001'0100'0000'0110, // LSR    - Logical Shift Right                      - 1001'010d'dddd'0110
			MOV    = 0b0010'1100'0000'0000, // MOV    - Copy Register                            - 0010'11rr'dddd'rrrr
			MOVW   = 0b0000'0001'0000'0000, // MOVW   - Copy Register Word                       - 0000'0001'dddd'rrrr
			MUL    = 0b1001'1100'0000'0000, // MUL    - Multiply Unsigned                        - 1001'11rd'dddd'rrrr
			MULS   = 0b0000'0010'0000'0000, // MULS   - Multiply Signed                          - 0000'0010'dddd'rrrr
			MULSU  = 0b0000'0011'0000'0000, // MULSU  - Multiply Signed with Unsigned            - 0000'0011'0ddd'0rrr
			NEG    = 0b1001'0100'0000'0001, // NEG    - Two's Complement                         - 1001'010d'dddd'0001
			NOP    = 0b0000'0000'0000'0000, // NOP    - No Operation                             - 0000'0000'0000'0000
			OR     = 0b0010'1000'0000'0000, // OR     - Logical OR                               - 0010'10rd'dddd'rrrr
			ORI    = 0b0110'0000'0000'0000, // ORI    - Logical OR with Immediate                - 0110'KKKK'dddd'KKKK
			OUT    = 0b1011'1000'0000'0000, // OUT    - Store Register to I/O Location           - 1011'1AAr'rrrr'AAAA
			SBC    = 0b0000'1000'0000'0000, // SBC    - Subtract with Carry                      - 0000'10rd'dddd'rrrr
			SBCI   = 0b0100'0000'0000'0000, // SBCI   - Subtract Immediate with Carry            - 0100'KKKK'dddd'KKKK
			SBI    = 0b1001'1010'0000'0000, // SBI    - Set Bit in I/O Register                  - 1001'1010'AAAA'Abbb
			                                // SBR    - Set Bits in Register                     - ORI Rd,K
			                                // SER    - Set all Bits in Register                 - LDI Rd,0xFF
			SUB    = 0b0001'1000'0000'0000, // SBC    - Subtract without Carry                   - 0001'10rd'dddd'rrrr
			SUBI   = 0b0101'0000'0000'0000; // SUBI   - Subtract Immediate                       - 0101'KKKK'dddd'KKKK
			                                // TST    - Test for Zero or Minus                   - AND Rd,Rd

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

		// BRBC - Branch if Bit in SREG is Clear
		void Handle_BRBC(Word instruction, int& cycles, CPU* cpu);

		// BRBS - Branch if Bit in SREG is Set
		void Handle_BRBS(Word instruction, int& cycles, CPU* cpu);

		// BSET - Bit Set in SREG
		void Handle_BSET(Word instruction, CPU* cpu);

		// BST - Bit Store from Bit in Register to T Flag
		void Handle_BST(Word instruction, CPU* cpu);

		// CBI - Clear Bit in I/O Register
		void Handle_CBI(Word instruction, CPU* cpu);

		// COM - One's Complement
		void Handle_COM(Word instruction, CPU* cpu);

		// CP - Compare
		void Handle_CP(Word instruction, CPU* cpu);

		// CPC - Compare with Carry
		void Handle_CPC(Word instruction, CPU* cpu);

		// CPI - Compare with Immediate
		void Handle_CPI(Word instruction, CPU* cpu);

		// CPSE - Compare Skip if Equal
		void Handle_CPSE(Word instruction, int& cycles, CPU* cpu);

		// DEC - Decrement
		void Handle_DEC(Word instruction, CPU* cpu);

		// EOR - Logical Exclusive OR
		void Handle_EOR(Word instruction, CPU* cpu);

		// FMUL - Fractional Multiply Unsigned
		void Handle_FMUL(Word instruction, int& cycles, CPU* cpu);

		// FMULS - Fractional Multiply Signed
		void Handle_FMULS(Word instruction, int& cycles, CPU* cpu);

		// FMULSU - Fractional Multiply Signed with Unsigned
		void Handle_FMULSU(Word instruction, int& cycles, CPU* cpu);

		// INC - Increment
		void Handle_INC(Word instruction, CPU* cpu);

		// LAC - Load and Clear
		void Handle_LAC(Word instruction, int& cycles, CPU* cpu);

		// LAS - Load and Set
		void Handle_LAS(Word instruction, int& cycles, CPU* cpu);

		// LAT - Load and Toggle
		void Handle_LAT(Word instruction, int& cycles, CPU* cpu);

		// LDI - Load Immediate
		void Handle_LDI(Word instruction, CPU* cpu);

		// LDS - Load Direct from Data Space
		void Handle_LDS(Word instruction, int& cycles, CPU* cpu, Memory& memory);

		// LSR - Logical Shift Right
		void Handle_LSR(Word instruction, CPU* cpu);

		// MOV - Copy Register
		void Handle_MOV(Word instruction, CPU* cpu);

		// MOVW - Copy Register Word
		void Handle_MOVW(Word instruction, CPU* cpu);

		// MUL - Multiply Unsigned
		void Handle_MUL(Word instruction, int& cycles, CPU* cpu);

		// MULS - Multiply Signed
		void Handle_MULS(Word instruction, int& cycles, CPU* cpu);

		// MULSU - Multiply Signed with Unsigned
		void Handle_MULSU(Word instruction, int& cycles, CPU* cpu);

		// NEG - Two's Complement
		void Handle_NEG(Word instruction, CPU* cpu);
		
		// OR - Logical OR
		void Handle_OR(Word instruction, CPU* cpu);

		// ORI - Logical OR with Immediate
		void Handle_ORI(Word instruction, CPU* cpu);

		// OUT - Store Register to I/O Location
		void Handle_OUT(Word instruction, CPU* cpu);

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
