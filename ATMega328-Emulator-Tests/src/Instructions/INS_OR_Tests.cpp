#include "TestHardware.h"

#include "ATMega328Emulator/Instructions.h"

TEST_F(ATMega328, Test_INS_OR)
{
	CPU cpuCopy = cpu;

	cpu.R15 = 0b1010;
	cpu.R16 = 0b1110;

	// or r15,r16 ; Do bitwise or between registers R15 and R16
	constexpr Word instruction =
		Instruction::OR
		| 0b1111'0000     // Rd = R15
		| 0b10'0000'0000; // Rr = R16

	int dummyCycles = 0;
	memory.WriteWord(instruction, 0x0, dummyCycles);

	// Act
	cpu.Execute(1, memory); // OR takes 1 cycle

	// Assert
	EXPECT_EQ(cpu.R15, 0b1110);

	EXPECT_EQ(cpu.IO.SREG.I, cpuCopy.IO.SREG.I);
	EXPECT_EQ(cpu.IO.SREG.T, cpuCopy.IO.SREG.T);
	EXPECT_EQ(cpu.IO.SREG.H, cpuCopy.IO.SREG.H);
	
	EXPECT_FALSE(cpu.IO.SREG.S);
	EXPECT_FALSE(cpu.IO.SREG.V);
	EXPECT_FALSE(cpu.IO.SREG.N);
	EXPECT_FALSE(cpu.IO.SREG.Z);

	EXPECT_EQ(cpu.IO.SREG.C, cpuCopy.IO.SREG.C);
}

TEST_F(ATMega328, Test_INS_OR_Zero)
{
	CPU cpuCopy = cpu;

	cpu.R15 = 0x0;
	cpu.R16 = 0x0;

	// or r15,r16 ; Do bitwise or between registers R15 and R16
	constexpr Word instruction =
		Instruction::OR
		| 0b1111'0000     // Rd = R15
		| 0b10'0000'0000; // Rr = R16

	int dummyCycles = 0;
	memory.WriteWord(instruction, 0x0, dummyCycles);

	// Act
	cpu.Execute(1, memory); // OR takes 1 cycle

	// Assert
	EXPECT_EQ(cpu.R15, 0x0);

	EXPECT_EQ(cpu.IO.SREG.I, cpuCopy.IO.SREG.I);
	EXPECT_EQ(cpu.IO.SREG.T, cpuCopy.IO.SREG.T);
	EXPECT_EQ(cpu.IO.SREG.H, cpuCopy.IO.SREG.H);

	EXPECT_FALSE(cpu.IO.SREG.S);
	EXPECT_FALSE(cpu.IO.SREG.V);
	EXPECT_FALSE(cpu.IO.SREG.N);
	EXPECT_TRUE(cpu.IO.SREG.Z);

	EXPECT_EQ(cpu.IO.SREG.C, cpuCopy.IO.SREG.C);
}

TEST_F(ATMega328, Test_INS_OR_MSB)
{
	CPU cpuCopy = cpu;

	cpu.R15 = 0x80;
	cpu.R16 = 0x0F;

	// or r15,r16 ; Do bitwise or between registers R15 and R16
	constexpr Word instruction =
		Instruction::OR
		| 0b1111'0000     // Rd = R15
		| 0b10'0000'0000; // Rr = R16

	int dummyCycles = 0;
	memory.WriteWord(instruction, 0x0, dummyCycles);

	// Act
	cpu.Execute(1, memory); // OR takes 1 cycle

	// Assert
	EXPECT_EQ(cpu.R15, 0x8F);

	EXPECT_EQ(cpu.IO.SREG.I, cpuCopy.IO.SREG.I);
	EXPECT_EQ(cpu.IO.SREG.T, cpuCopy.IO.SREG.T);
	EXPECT_EQ(cpu.IO.SREG.H, cpuCopy.IO.SREG.H);

	EXPECT_FALSE(cpu.IO.SREG.S);
	EXPECT_FALSE(cpu.IO.SREG.V);
	EXPECT_TRUE(cpu.IO.SREG.N);
	EXPECT_FALSE(cpu.IO.SREG.Z);

	EXPECT_EQ(cpu.IO.SREG.C, cpuCopy.IO.SREG.C);
}
