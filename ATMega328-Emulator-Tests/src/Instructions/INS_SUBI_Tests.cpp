#include "TestHardware.h"

#include "ATMega328Emulator/Instructions.h"

TEST_F(ATMega328, Test_INS_SUBI)
{
	CPU cpuCopy = cpu;

	cpu.R22 = 0xF1;

	//subi r22,0x0F ; Subtract 0x0F from r22
	constexpr Word instruction =
		Instruction::SUBI
		| 0b110'0000 // Rd = R22
		| 0b1111;    // K = 0x0F

	int dummyCycles = 0;
	memory.WriteWord(instruction, 0x0, dummyCycles);

	// Act
	cpu.Execute(1, memory); // SUBI takes 1 cycle
	
	// Assert
	EXPECT_EQ(cpu.R22, 0xF1 - 0x0F);

	EXPECT_EQ(cpu.SREG.I, cpuCopy.SREG.I);
	EXPECT_EQ(cpu.SREG.T, cpuCopy.SREG.T);

	EXPECT_TRUE(cpu.SREG.H);
	EXPECT_FALSE(cpu.SREG.S);
	EXPECT_FALSE(cpu.SREG.V);
	EXPECT_TRUE(cpu.SREG.N);
	EXPECT_FALSE(cpu.SREG.Z);
	EXPECT_FALSE(cpu.SREG.C);
}
