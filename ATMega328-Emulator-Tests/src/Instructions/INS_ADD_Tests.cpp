#include "TestHardware.h"

#include "ATMega328Emulator/Instructions.h"

TEST_F(ATMega328, Test_INS_ADD)
{
	CPU cpuCopy = cpu;

	cpu.R01 = 0x01;
	cpu.R02 = 0x02;

	// add r1,r2 ; Add R2 to R1 (R1=R1+R2)
	constexpr Word instruction =
		Instruction::ADD
		| 0b1'0000  // Rd = R1
		| 0b0'0010; // Rr = R2

	int dummyCycles = 0;
	memory.WriteWord(instruction, 0x0, dummyCycles);

	// Act
	cpu.Execute(1, memory); // ADD takes 1 cycle

	// Assert
	EXPECT_EQ(cpu.R01, 0x03);

	EXPECT_EQ(cpu.IO.SREG.I, cpuCopy.IO.SREG.I);
	EXPECT_EQ(cpu.IO.SREG.T, cpuCopy.IO.SREG.T);

	EXPECT_FALSE(cpu.IO.SREG.H);
	EXPECT_FALSE(cpu.IO.SREG.S);
	EXPECT_FALSE(cpu.IO.SREG.V);
	EXPECT_FALSE(cpu.IO.SREG.N);
	EXPECT_FALSE(cpu.IO.SREG.Z);
	EXPECT_FALSE(cpu.IO.SREG.C);
}

TEST_F(ATMega328, Test_INS_ADD_AddRegToSelf)
{
	CPU cpuCopy = cpu;

	cpu.R28 = 0x10;

	// add R28,R28 ; Add R28 to itself (R28=R28+R28)
	constexpr Word instruction = Instruction::ADD
		| 0b01'1100'0000  // Rd = R28
		| 0b10'0000'1100; // Rr = R28

	int dummyCycles = 0;
	memory.WriteWord(instruction, 0x0, dummyCycles);

	// Act
	cpu.Execute(1, memory); // ADD takes 1 cycle

	// Assert
	EXPECT_EQ(cpu.R28, 0x20);

	EXPECT_EQ(cpu.IO.SREG.I, cpuCopy.IO.SREG.I);
	EXPECT_EQ(cpu.IO.SREG.T, cpuCopy.IO.SREG.T);

	EXPECT_FALSE(cpu.IO.SREG.H);
	EXPECT_FALSE(cpu.IO.SREG.S);
	EXPECT_FALSE(cpu.IO.SREG.V);
	EXPECT_FALSE(cpu.IO.SREG.N);
	EXPECT_FALSE(cpu.IO.SREG.Z);
	EXPECT_FALSE(cpu.IO.SREG.C);
}

TEST_F(ATMega328, Test_INS_ADD_CarryFlag)
{
	CPU cpuCopy = cpu;

	cpu.R01 = 0xFE;
	cpu.R02 = 0x03;

	// add r1,r2 ; Add R2 to R1 (R1=R1+R2)
	constexpr Word instruction =
		Instruction::ADD
		| 0b1'0000  // Rd = R1
		| 0b0'0010; // Rr = R2

	int dummyCycles = 0;
	memory.WriteWord(instruction, 0x0, dummyCycles);

	// Act
	cpu.Execute(1, memory); // ADD takes 1 cycle

	// Assert
	EXPECT_EQ(cpu.R01, 0x1);

	EXPECT_EQ(cpu.IO.SREG.I, cpuCopy.IO.SREG.I);
	EXPECT_EQ(cpu.IO.SREG.T, cpuCopy.IO.SREG.T);

	EXPECT_TRUE(cpu.IO.SREG.H);
	EXPECT_FALSE(cpu.IO.SREG.S);
	EXPECT_FALSE(cpu.IO.SREG.V);
	EXPECT_FALSE(cpu.IO.SREG.N);
	EXPECT_FALSE(cpu.IO.SREG.Z);
	EXPECT_TRUE(cpu.IO.SREG.C);
}
