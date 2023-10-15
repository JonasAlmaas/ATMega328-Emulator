#include "ATMega328Emulator/Instructions.h"

namespace ATMega328Emulator {
	
	namespace Instruction {
	
		namespace StatusFlag {

			namespace H {
				
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
				static inline void BorrowBit3(CPU* cpu, Byte R, Byte Rd, Byte Rr)
				{
					cpu->SREG.H = (
						((~Rd & 0x8) & (Rr & 0x8)) |
						((Rr & 0x8) & (R & 0x8)) |
						((R & 0x8) & (~Rd & 0x8))
					) >> 3;
				}
			}

			namespace S {
				
				// N XOR V, for signed tests.
				static inline void SignedTest(CPU* cpu)
				{
					cpu->SREG.S = cpu->SREG.N ^ cpu->SREG.V;
				}
				
			}

			namespace V {

				static inline void Set(CPU* cpu, Byte value)
				{
					cpu->SREG.V = value;
				}
				
				// Set if two's complement overflow resulted from the operation; cleared otherwise.
				static inline void ByteAddTwosComplementOverflow(CPU* cpu, Byte R, Byte Rd, Byte Rr)
				{
					cpu->SREG.V = (
						((Rd & 0x80) & (Rr & 0x80) & (~R & 0x80)) |
						((~Rd & 0x80) & (~Rr & 0x80) & (R & 0x80))
					) >> 7;
				}

				static inline void WordAddTwosComplementOverflow(CPU* cpu, Word R, Word Rdh)
				{
					cpu->SREG.V = ((~Rdh & 0x80) >> 7) & ((R & 0x8000) >> 15);
				}

				// Set if two’s complement overflow resulted from the operation; cleared otherwise.
				static inline void ByteSubTwosComplementOverflow(CPU* cpu, Byte R, Byte Rd, Byte Rr)
				{
					cpu->SREG.V = (
						((Rd & 0x80) & (~Rr & 0x80) & (~R & 0x80)) |
						((~Rd & 0x80) & (Rr & 0x80) & (R & 0x80))
					) >> 7;
				}
				
			}

			namespace N {
				
				// Set if Most Significant Bit(MSB) of the result is set; cleared otherwise.
				static inline void MSBSet(CPU* cpu, Byte R)
				{
					cpu->SREG.N = (R & 0x80) >> 7;
				}
				
			}

			namespace Z {
				
				// Set if Zero; cleared otherwise.
				static inline void ByteNullRes(CPU* cpu, Byte R)
				{
					cpu->SREG.Z = R == 0;
				}

				// Set if Zero; cleared otherwise.
				static inline void WordNullRes(CPU* cpu, Word R)
				{
					cpu->SREG.Z = R == 0;
				}

				// Previous value remains unchanged when the result is zero; cleared otherwise.
				static inline void ByteNullResCarry(CPU* cpu, Byte R)
				{
					cpu->SREG.Z = (R == 0) & cpu->SREG.Z;
				}
				
			}

			namespace C {

				static inline void Set(CPU* cpu, Byte value)
				{
					cpu->SREG.C = value;
				}

				static inline void NotNull(CPU* cpu, Byte R)
				{
					cpu->SREG.C = R != 0x0;
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
				static inline void WordCarryMSB(CPU* cpu, Word R, Word Rd)
				{
					cpu->SREG.C = ((~R & 0x8000) >> 15) & ((Rd & 0x80) >> 7);
				}

				// Set if the absolute value of the contents of Rr plus previous carry is larger than the absolute value of the Rd; cleared otherwise.
				static inline void ByteGreater(CPU* cpu, Byte R, Byte Rd, Byte Rr)
				{
					cpu->SREG.C = (
						((~Rd & 0x80) & (Rr & 0x80)) |
						((Rr & 0x80) & (R & 0x80)) |
						((R & 0x80) & (~Rd & 0x80))
					) >> 7;
				}
			}
		}
		
		void Handle_ADC(Word instruction, CPU* cpu)
		{
			Byte d = (instruction & 0b1'1111'0000) >> 4;
			Byte r = (instruction & 0b1111) | ((instruction & 0b10'0000'0000) >> 5);
			
			Byte* Rd = &cpu->R00 + d;
			Byte* Rr = &cpu->R00 + r;
		
			Byte R = *Rd + *Rr + cpu->SREG.C;

			StatusFlag::H::CarryBit3(cpu, R, *Rd, *Rr);
			StatusFlag::S::SignedTest(cpu);
			StatusFlag::V::ByteAddTwosComplementOverflow(cpu, R, *Rd, *Rr);
			StatusFlag::N::MSBSet(cpu, R);
			StatusFlag::Z::ByteNullRes(cpu, R);
			StatusFlag::C::ByteCarryMSB(cpu, R, *Rd, *Rr);
		
			*Rd = R;
		}

		void Handle_ADD(Word instruction, CPU* cpu)
		{
			Byte d = (instruction & 0b1'1111'0000) >> 4;
			Byte r = (instruction & 0b1111) | ((instruction & 0b10'0000'0000) >> 5);
			
			Byte* Rd = &cpu->R00 + d;
			Byte* Rr = &cpu->R00 + r;
		
			Byte R = *Rd + *Rr;
		
			StatusFlag::H::CarryBit3(cpu, R, *Rd, *Rr);
			StatusFlag::S::SignedTest(cpu);
			StatusFlag::V::ByteAddTwosComplementOverflow(cpu, R, *Rd, *Rr);
			StatusFlag::N::MSBSet(cpu, R);
			StatusFlag::Z::ByteNullRes(cpu, R);
			StatusFlag::C::ByteCarryMSB(cpu, R, *Rd, *Rr);
		
			*Rd = R;
		}

		void Handle_ADIW(Word instruction, int& cycles, CPU* cpu)
		{
			Byte d = (instruction & 0b11'0000) >> 4; // 2 bits describing the offset from R24 in steps of 2.
			Byte K = instruction & 0b1111 | ((instruction & 0b1100'0000) >> 2); // Value in the range of [0-63]

			Word* Rdh = ((Word*)&cpu->R24) + d;
			Word R = *Rdh + K;
			
			StatusFlag::S::SignedTest(cpu);
			StatusFlag::V::WordAddTwosComplementOverflow(cpu, R, *Rdh);
			StatusFlag::N::MSBSet(cpu, (R & 0xFF00) >> 8);
			StatusFlag::Z::WordNullRes(cpu, R);
			StatusFlag::C::WordCarryMSB(cpu, R, *Rdh);
		
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
	
			StatusFlag::S::SignedTest(cpu);
			StatusFlag::V::Set(cpu, 0);
			StatusFlag::N::MSBSet(cpu, R);
			StatusFlag::Z::ByteNullRes(cpu, R);
	
			*Rd = R;
		}
	
		void Handle_ANDI(Word instruction, CPU* cpu)
		{
			Byte d = (instruction & 0b1111'0000) >> 4;
			Byte K = (instruction & 0xF) | ((instruction & 0xF00) >> 4);
		
			Byte* Rd = &cpu->R16 + d;
			Byte R = *Rd & K;
		
			StatusFlag::S::SignedTest(cpu);
			StatusFlag::V::Set(cpu, 0);
			StatusFlag::N::MSBSet(cpu, R);
			StatusFlag::Z::ByteNullRes(cpu, R);
		
			*Rd = R;
		}
		
		void Handle_COM(Word instruction, CPU* cpu)
		{
			Byte d = (instruction & 0b1'1111'0000) >> 4;

			Byte* Rd = &cpu->R00 + d;
			Byte R = ~*Rd;
			
			StatusFlag::S::SignedTest(cpu);
			StatusFlag::V::Set(cpu, 0);
			StatusFlag::N::MSBSet(cpu, R);
			StatusFlag::Z::ByteNullRes(cpu, R);
			StatusFlag::C::Set(cpu, 1);
			
			*Rd = R;
		}

		void Handle_DEC(Word instruction, CPU* cpu)
		{
			Byte d = (instruction & 0b1'1111'0000) >> 4;

			Byte* Rd = &cpu->R00 + d;
			Byte R = *Rd - 1;

			StatusFlag::S::SignedTest(cpu);
			cpu->SREG.V = R == 0x7F;
			StatusFlag::N::MSBSet(cpu, R);
			StatusFlag::Z::ByteNullRes(cpu, R);

			*Rd = R;
		}

		void Handle_EOR(Word instruction, CPU* cpu)
		{
			Byte r = (instruction & 0b1111) | ((instruction & 0b10'0000'0000) >> 5);
			Byte d = (instruction & 0b1'1111'0000) >> 4;

			Byte* Rd = &cpu->R00 + d;
			Byte* Rr = &cpu->R00 + r;

			Byte R = *Rd ^ *Rr;

			StatusFlag::S::SignedTest(cpu);
			StatusFlag::V::Set(cpu, 0);
			StatusFlag::N::MSBSet(cpu, R);
			StatusFlag::Z::ByteNullRes(cpu, R);

			*Rd = R;
		}

		void Handle_INC(Word instruction, CPU* cpu)
		{
			Byte d = (instruction & 0b1'1111'0000) >> 4;

			Byte* Rd = &cpu->R00 + d;
			Byte R = *Rd + 1;

			StatusFlag::S::SignedTest(cpu);
			cpu->SREG.V = R == 0x80;
			StatusFlag::N::MSBSet(cpu, R);
			StatusFlag::Z::ByteNullRes(cpu, R);

			*Rd = R;
		}

		void Handle_NEG(Word instruction, CPU* cpu)
		{
			Byte d = (instruction & 0b1'1111'0000) >> 4;

			Byte* Rd = &cpu->R00 + d;
			Byte R = (~*Rd) + 1;
			
			StatusFlag::H::BorrowBit3(cpu, R, *Rd, 0);
			StatusFlag::S::SignedTest(cpu);
			cpu->SREG.V = R == 0x80;
			StatusFlag::N::MSBSet(cpu, R);
			StatusFlag::Z::ByteNullRes(cpu, R);
			StatusFlag::C::NotNull(cpu, R);

			*Rd = R;
		}

		void Handle_OR(Word instruction, CPU* cpu)
		{
			Byte d = (instruction & 0b1'1111'0000) >> 4;
			Byte r = (instruction & 0b1111) | ((instruction & 0b10'0000'0000) >> 5);
			
			Byte* Rd = &cpu->R00 + d;
			Byte* Rr = &cpu->R00 + r;
			
			Byte R = *Rd | *Rr;

			StatusFlag::S::SignedTest(cpu);
			StatusFlag::V::Set(cpu, 0);
			StatusFlag::N::MSBSet(cpu, R);
			StatusFlag::Z::ByteNullRes(cpu, R);

			*Rd = R;
		}

		void Handle_ORI(Word instruction, CPU* cpu)
		{
			Byte d = (instruction & 0b1111'0000) >> 4;
			Byte K = (instruction & 0b1111) | (instruction & 0b1111'0000'0000) >> 4;

			Byte* Rd = &cpu->R16 + d;
			Byte R = *Rd | K;

			StatusFlag::S::SignedTest(cpu);
			StatusFlag::V::Set(cpu, 0);
			StatusFlag::N::MSBSet(cpu, R);
			StatusFlag::Z::ByteNullRes(cpu, R);
			
			*Rd = R;
		}

		void Handle_SBC(Word instruction, CPU* cpu)
		{
			Byte d = (instruction & 0b1'1111'0000) >> 4;
			Byte r = (instruction & 0b1111) | ((instruction & 0b10'0000'0000) >> 5);

			Byte* Rd = &cpu->R00 + d;
			Byte* Rr = &cpu->R00 + r;

			Byte R = *Rd - *Rr - cpu->SREG.C;

			StatusFlag::H::BorrowBit3(cpu, R, *Rd, *Rr);
			StatusFlag::S::SignedTest(cpu);
			StatusFlag::V::ByteSubTwosComplementOverflow(cpu, R, *Rd, *Rr);
			StatusFlag::N::MSBSet(cpu, R);
			StatusFlag::Z::ByteNullResCarry(cpu, R);
			StatusFlag::C::ByteGreater(cpu, R, *Rd, *Rr);

			*Rd = R;
		}

		void Handle_SUBI(Word instruction, CPU* cpu)
		{
			Byte d = (instruction & 0b1111'0000) >> 4;
			Byte K = (instruction & 0b1111) | (instruction & 0b1111'0000'0000) >> 4;

			Byte* Rd = &cpu->R16 + d;
			Byte R = *Rd - K;

			StatusFlag::H::BorrowBit3(cpu, R, *Rd, K);
			StatusFlag::S::SignedTest(cpu);
			StatusFlag::V::ByteSubTwosComplementOverflow(cpu, R, *Rd, K);
			StatusFlag::N::MSBSet(cpu, R);
			StatusFlag::Z::ByteNullRes(cpu, R);
			StatusFlag::C::ByteGreater(cpu, R, *Rd, K);

			*Rd = R;
		}

		void Handle_SUB(Word instruction, CPU* cpu)
		{
			Byte d = (instruction & 0b1'1111'0000) >> 4;
			Byte r = (instruction & 0b1111) | ((instruction & 0b10'0000'0000) >> 5);

			Byte* Rd = &cpu->R00 + d;
			Byte* Rr = &cpu->R00 + r;

			Byte R = *Rd - *Rr;

			StatusFlag::H::BorrowBit3(cpu, R, *Rd, *Rr);
			StatusFlag::S::SignedTest(cpu);
			StatusFlag::V::ByteSubTwosComplementOverflow(cpu, R, *Rd, *Rr);
			StatusFlag::N::MSBSet(cpu, R);
			StatusFlag::Z::ByteNullRes(cpu, R);
			StatusFlag::C::ByteGreater(cpu, R, *Rd, *Rr);

			*Rd = R;
		}
		
	}
}
