#include "TestHardware.h"

#include "ATMega328Emulator/Instructions.h"

TEST_F(ATMega328, Test_INS_SUB)
{
	CPU cpuCopy = cpu;

	cpu.R01 = 0xF;
	cpu.R02 = 0x2;

	// sub r1,r2 ; Subtract R2 from R1 (R1=R1-R2)
	constexpr Word instruction =
		Instruction::SUB
		| 0b1'0000  // Rd = R1
		| 0b0'0010; // Rr = R2

	int dummyCycles = 0;
	memory.WriteWord(instruction, 0x0, dummyCycles);
	
	// Act
	cpu.Execute(1, memory); // SUB takes 1 cycle

	// Assert
	EXPECT_EQ(cpu.R01, 0xD);

	EXPECT_EQ(cpu.IO.SREG.I, cpuCopy.IO.SREG.I);
	EXPECT_EQ(cpu.IO.SREG.T, cpuCopy.IO.SREG.T);

	EXPECT_FALSE(cpu.IO.SREG.H);
	EXPECT_FALSE(cpu.IO.SREG.S);
	EXPECT_FALSE(cpu.IO.SREG.V);
	EXPECT_FALSE(cpu.IO.SREG.N);
	EXPECT_FALSE(cpu.IO.SREG.Z);
	EXPECT_FALSE(cpu.IO.SREG.C);
}

TEST_F(ATMega328, Test_INS_SUB_CarryFlag)
{
	CPU cpuCopy = cpu;

	cpu.R01 = 0x2;
	cpu.R02 = 0x3;

	// sub r1,r2 ; Subtract R2 from R1 (R1=R1-R2)
	constexpr Word instruction =
		Instruction::SUB
		| 0b1'0000  // Rd = R1
		| 0b0'0010; // Rr = R2

	int dummyCycles = 0;
	memory.WriteWord(instruction, 0x0, dummyCycles);

	// Act
	cpu.Execute(1, memory); // SUB takes 1 cycle

	// Assert
	EXPECT_EQ(cpu.R01, 0xFF);

	EXPECT_EQ(cpu.IO.SREG.I, cpuCopy.IO.SREG.I);
	EXPECT_EQ(cpu.IO.SREG.T, cpuCopy.IO.SREG.T);

	EXPECT_TRUE(cpu.IO.SREG.H);
	EXPECT_FALSE(cpu.IO.SREG.S);
	EXPECT_FALSE(cpu.IO.SREG.V);
	EXPECT_TRUE(cpu.IO.SREG.N);
	EXPECT_FALSE(cpu.IO.SREG.Z);
	EXPECT_TRUE(cpu.IO.SREG.C);
}
