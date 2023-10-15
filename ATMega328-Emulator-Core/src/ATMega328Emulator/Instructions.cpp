#include "ATMega328Emulator/Instructions.h"

namespace ATMega328Emulator {
	
	namespace Instruction {
	
		namespace StatusFlag {
	
			// Set if there was a carry from bit 3; cleared otherwise.
			static inline void CarryBit3(CPU* cpu, Byte R, Byte Rd, Byte Rr)
			{
				cpu->SREG.H = (
					((Rd & 0x8) & (Rr & 0x8)) |
					((Rr & 0x8) & (~R & 0x8)) |
					((~R & 0x8) & (Rd & 0x8))
				) >> 3;
			}

			// Set if there was a borrow from bit 3; cleared otherwise.
			static inline void BorrowBit3(CPU* cpu, Byte R, Byte Rd)
			{
				cpu->SREG.H = ((R & 0b1000) | (Rd & 0b1000)) >> 3;
			}
			
			// N XOR V, for signed tests.
			static inline void SignedTest(CPU* cpu)
			{
				cpu->SREG.S = cpu->SREG.N ^ cpu->SREG.V;
			}
		
			// Set if two's complement overflow resulted from the operation; cleared otherwise.
			static inline void ByteTwosComplementOverflow(CPU* cpu, Byte R, Byte Rd, Byte Rr)
			{
				cpu->SREG.V =(
					((Rd & 0x80) & (Rr & 0x80) & (~R & 0x80)) |
					((~Rd & 0x80) & (~Rr & 0x80) & (R & 0x80))
				) >> 7;
			}

			static inline void WordTwosComplementOverflow(CPU* cpu, Word R, Word Rdh)
			{
				cpu->SREG.V = ((~Rdh & 0x80) >> 7) & ((R & 0x8000) >> 15);
			}
		
			// Set if Most Significant Bit(MSB) of the result is set; cleared otherwise.
			static inline void MSBSet(CPU* cpu, Byte result)
			{
				cpu->SREG.N = (result & 0x80) >> 7;
			}

			// Set if Zero; cleared otherwise.
			static inline void ByteNullRes(CPU* cpu, Byte result)
			{
				cpu->SREG.Z = result == 0;
			}

			// Set if Zero; cleared otherwise.
			static inline void WordNullRes(CPU* cpu, Word result)
			{
				cpu->SREG.Z = result == 0;
			}

			// Set if there was a carry from the Most Significant Bit(MSB) of the result; cleared otherwise.
			static inline void ByteCarryMSB(CPU* cpu, Byte R, Byte Rd, Byte Rr)
			{
				cpu->SREG.C = (
					((Rd & 0x80) & (Rr & 0x80)) |
					((Rr & 0x80) & (~R & 0x80)) |
					((~R & 0x80) & (Rd & 0x80))
				) >> 7;
			}

			// Set if there was a carry from the Most Significant Bit(MSB) of the result; cleared otherwise.
			static inline void WordCarryMSB(CPU* cpu, Word R, Word Rdh)
			{
				cpu->SREG.C = ((~R & 0x8000) >> 15) & ((Rdh & 0x80) >> 7);
			}
		
		}
		
		void Handle_ADC(Word instruction, CPU* cpu)
		{
			Byte d = (instruction & 0b1'1111'0000) >> 4;
			Byte r = (instruction & 0b1111) | ((instruction & 0b10'0000'0000) >> 5);
			
			Byte* Rd = &cpu->R00 + d;
			Byte* Rr = &cpu->R00 + r;
		
			Byte R = *Rd + *Rr + cpu->SREG.C;

			StatusFlag::CarryBit3(cpu, R, *Rd, *Rr);
			StatusFlag::SignedTest(cpu);
			StatusFlag::ByteTwosComplementOverflow(cpu, R, *Rd, *Rr);
			StatusFlag::MSBSet(cpu, R);
			StatusFlag::ByteNullRes(cpu, R);
			StatusFlag::ByteCarryMSB(cpu, R, *Rd, *Rr);
		
			*Rd = R;
		}

		void Handle_ADD(Word instruction, CPU* cpu)
		{
			Byte d = (instruction & 0b1'1111'0000) >> 4;
			Byte r = (instruction & 0b1111) | ((instruction & 0b10'0000'0000) >> 5);
			
			Byte* Rd = &cpu->R00 + d;
			Byte* Rr = &cpu->R00 + r;
		
			Byte R = *Rd + *Rr;
		
			StatusFlag::CarryBit3(cpu, R, *Rd, *Rr);
			StatusFlag::SignedTest(cpu);
			StatusFlag::ByteTwosComplementOverflow(cpu, R, *Rd, *Rr);
			StatusFlag::MSBSet(cpu, R);
			StatusFlag::ByteNullRes(cpu, R);
			StatusFlag::ByteCarryMSB(cpu, R, *Rd, *Rr);
		
			*Rd = R;
		}

		void Handle_ADIW(Word instruction, int& cycles, CPU* cpu)
		{
			Byte d = (instruction & 0b11'0000) >> 4; // 2 bits describing the offset from R24 in steps of 2.
			Byte K = instruction & 0b1111 | ((instruction & 0b1100'0000) >> 2); // Value in the range of [0-63]

			Word* Rdh = ((Word*)&cpu->R24) + d;
			Word R = *Rdh + K;
			
			StatusFlag::SignedTest(cpu);
			StatusFlag::WordTwosComplementOverflow(cpu, R, *Rdh);
			StatusFlag::MSBSet(cpu, (R & 0xFF00) >> 8);
			StatusFlag::WordNullRes(cpu, R);
			StatusFlag::WordCarryMSB(cpu, R, *Rdh);
		
			*Rdh = R;
			--cycles;
		}
	
		void Handle_AND(Word instruction, CPU* cpu)
		{
			Byte d = (instruction & 0b1'1111'0000) >> 4;
			Byte r = (instruction & 0b1111) | ((instruction & 0b10'0000'0000) >> 5);

			Byte* Rd = &cpu->R00 + d;
			Byte* Rr = &cpu->R00 + r;
		
			Byte R = *Rd & *Rr;
	
			StatusFlag::SignedTest(cpu);
			cpu->SREG.V = 0;
			StatusFlag::MSBSet(cpu, R);
			StatusFlag::ByteNullRes(cpu, R);
	
			*Rd = R;
		}
	
		void Handle_ANDI(Word instruction, CPU* cpu)
		{
			Byte d = (instruction & 0b1111'0000) >> 4;
			Byte K = (instruction & 0xF) | ((instruction & 0xF00) >> 4);
		
			Byte* Rd = &cpu->R16 + d;
			Byte R = *Rd & K;
		
			StatusFlag::SignedTest(cpu);
			cpu->SREG.V = 0;
			StatusFlag::MSBSet(cpu, R);
			StatusFlag::ByteNullRes(cpu, R);
		
			*Rd = R;
		}
		
		void Handle_COM(Word instruction, CPU* cpu)
		{
			Byte d = (instruction & 0b1'1111'0000) >> 4;

			Byte* Rd = &cpu->R00 + d;
			Byte R = ~*Rd;
			
			StatusFlag::SignedTest(cpu);
			cpu->SREG.V = 0;
			StatusFlag::MSBSet(cpu, R);
			StatusFlag::ByteNullRes(cpu, R);
			cpu->SREG.C = 1;
			
			*Rd = R;
		}

		void Handle_DEC(Word instruction, CPU* cpu)
		{
			Byte d = (instruction & 0b1'1111'0000) >> 4;

			Byte* Rd = &cpu->R00 + d;
			Byte R = *Rd - 1;

			StatusFlag::SignedTest(cpu);
			cpu->SREG.V = R == 0x7F;
			StatusFlag::MSBSet(cpu, R);
			StatusFlag::ByteNullRes(cpu, R);

			*Rd = R;
		}

		void Handle_EOR(Word instruction, CPU* cpu)
		{
			Byte r = (instruction & 0b1111) | ((instruction & 0b10'0000'0000) >> 5);
			Byte d = (instruction & 0b1'1111'0000) >> 4;

			Byte* Rd = &cpu->R00 + d;
			Byte* Rr = &cpu->R00 + r;

			Byte R = *Rd ^ *Rr;

			StatusFlag::SignedTest(cpu);
			cpu->SREG.V = 0;
			StatusFlag::MSBSet(cpu, R);
			StatusFlag::ByteNullRes(cpu, R);

			*Rd = R;
		}

		void Handle_INC(Word instruction, CPU* cpu)
		{
			Byte d = (instruction & 0b1'1111'0000) >> 4;

			Byte* Rd = &cpu->R00 + d;
			Byte R = *Rd + 1;

			StatusFlag::SignedTest(cpu);
			cpu->SREG.V = R == 0x80;
			StatusFlag::MSBSet(cpu, R);
			StatusFlag::ByteNullRes(cpu, R);

			*Rd = R;
		}

		void Handle_NEG(Word instruction, CPU* cpu)
		{
			Byte d = (instruction & 0b1'1111'0000) >> 4;

			Byte* Rd = &cpu->R00 + d;
			Byte R = (~*Rd) + 1;

			StatusFlag::BorrowBit3(cpu, R, *Rd);
			StatusFlag::SignedTest(cpu);
			cpu->SREG.V = R == 0x80;
			StatusFlag::MSBSet(cpu, R);
			StatusFlag::ByteNullRes(cpu, R);
			cpu->SREG.C = R != 0x0;

			*Rd = R;
		}

		void Handle_OR(Word instruction, CPU* cpu)
		{
			Byte d = (instruction & 0b1'1111'0000) >> 4;
			Byte r = (instruction & 0b1111) | ((instruction & 0b10'0000'0000) >> 5);
			
			Byte* Rd = &cpu->R00 + d;
			Byte* Rr = &cpu->R00 + r;
			
			Byte R = *Rd | *Rr;

			StatusFlag::SignedTest(cpu);
			cpu->SREG.V = 0;
			StatusFlag::MSBSet(cpu, R);
			StatusFlag::ByteNullRes(cpu, R);

			*Rd = R;
		}
		
	}
}
