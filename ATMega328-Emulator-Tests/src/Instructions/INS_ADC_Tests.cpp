#include "TestHardware.h"

#include "ATMega328Emulator/Instructions.h"

TEST_F(ATMega328, Test_INS_ADC)
{
	CPU cpuCopy = cpu;

	cpu.R01 = 0x9;
	cpu.R02 = 0x2;
	cpu.SREG.C = 1;

	constexpr Byte a = 0x9 + 0x2 + 1;

	// adc r1,r2 ; Add r2 to r1 with carry high byte
	constexpr Word instruction =
		Instruction::ADC
		| 0b1'0000 // Rd = R1
		| 0b10; // Rr = R2

	int dummyCycles = 0;
	memory.WriteWord(instruction, 0x0, dummyCycles);

	// Act
	cpu.Execute(1, memory); // ADC takes 1 cycle

	// Assert
	EXPECT_EQ(cpu.R01, 0xC);

	EXPECT_EQ(cpu.SREG.I, cpuCopy.SREG.I);
	EXPECT_EQ(cpu.SREG.T, cpuCopy.SREG.T);

	EXPECT_FALSE(cpu.SREG.I);
	EXPECT_FALSE(cpu.SREG.S);
	EXPECT_FALSE(cpu.SREG.V);
	EXPECT_FALSE(cpu.SREG.N);
	EXPECT_FALSE(cpu.SREG.Z);
	EXPECT_FALSE(cpu.SREG.C);
}