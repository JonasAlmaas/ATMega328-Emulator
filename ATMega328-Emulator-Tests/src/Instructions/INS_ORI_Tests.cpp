#include "TestHardware.h"

#include "ATMega328Emulator/Instructions.h"

TEST_F(ATMega328, Test_INS_ORI)
{
	CPU cpuCopy = cpu;

	cpu.R20 = 0b1010;
	
	// ori r20,0b1110 ; Do bitwise or between registers R20 and immediate 0b1110
	constexpr Word instruction =
		Instruction::ORI
		| 0b100'0000 // Rd = R20
		| 0b1110;    // K = 0b1110 (14)
	
	int dummyCycles = 0;
	memory.WriteWord(instruction, 0x0, dummyCycles);
	
	// Act
	cpu.Execute(1, memory); // ORI takes 1 cycle

	// Assert
	EXPECT_EQ(cpu.R20, 0b1110);

	EXPECT_EQ(cpu.IO.SREG.I, cpuCopy.IO.SREG.I);
	EXPECT_EQ(cpu.IO.SREG.T, cpuCopy.IO.SREG.T);
	EXPECT_EQ(cpu.IO.SREG.H, cpuCopy.IO.SREG.H);

	EXPECT_FALSE(cpu.IO.SREG.S);
	EXPECT_FALSE(cpu.IO.SREG.V);
	EXPECT_FALSE(cpu.IO.SREG.N);
	EXPECT_FALSE(cpu.IO.SREG.Z);

	EXPECT_EQ(cpu.IO.SREG.C, cpuCopy.IO.SREG.C);
}

TEST_F(ATMega328, Test_INS_ORI_Zero)
{
	CPU cpuCopy = cpu;

	cpu.R20 = 0x0;

	// ori r20,0b0 ; Do bitwise or between registers R20 and immediate 0b0
	constexpr Word instruction =
		Instruction::ORI
		| 0b100'0000 // Rd = R20
		| 0b0;       // K = 0

	int dummyCycles = 0;
	memory.WriteWord(instruction, 0x0, dummyCycles);

	// Act
	cpu.Execute(1, memory); // ORI takes 1 cycle

	// Assert
	EXPECT_EQ(cpu.R20, 0x0);

	EXPECT_EQ(cpu.IO.SREG.I, cpuCopy.IO.SREG.I);
	EXPECT_EQ(cpu.IO.SREG.T, cpuCopy.IO.SREG.T);
	EXPECT_EQ(cpu.IO.SREG.H, cpuCopy.IO.SREG.H);

	EXPECT_FALSE(cpu.IO.SREG.S);
	EXPECT_FALSE(cpu.IO.SREG.V);
	EXPECT_FALSE(cpu.IO.SREG.N);
	EXPECT_TRUE(cpu.IO.SREG.Z);

	EXPECT_EQ(cpu.IO.SREG.C, cpuCopy.IO.SREG.C);
}

TEST_F(ATMega328, Test_INS_ORI_MSB)
{
	CPU cpuCopy = cpu;

	cpu.R20 = 0x80;
	
	// ori r20,0b1111 ; Do bitwise or between registers R20 and immediate 0b1111
	constexpr Word instruction =
		Instruction::ORI
		| 0b100'0000 // Rd = R20
		| 0b1111;    // K = 0b1111 (15)

	int dummyCycles = 0;
	memory.WriteWord(instruction, 0x0, dummyCycles);

	// Act
	cpu.Execute(1, memory); // OR takes 1 cycle

	// Assert
	EXPECT_EQ(cpu.R20, 0x8F);

	EXPECT_EQ(cpu.IO.SREG.I, cpuCopy.IO.SREG.I);
	EXPECT_EQ(cpu.IO.SREG.T, cpuCopy.IO.SREG.T);
	EXPECT_EQ(cpu.IO.SREG.H, cpuCopy.IO.SREG.H);

	EXPECT_FALSE(cpu.IO.SREG.S);
	EXPECT_FALSE(cpu.IO.SREG.V);
	EXPECT_TRUE(cpu.IO.SREG.N);
	EXPECT_FALSE(cpu.IO.SREG.Z);

	EXPECT_EQ(cpu.IO.SREG.C, cpuCopy.IO.SREG.C);
}
