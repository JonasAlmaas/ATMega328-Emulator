#include "TestHardware.h"

#include "ATMega328Emulator/Instructions.h"

TEST_F(ATMega328, Test_INS_SBC)
{
	CPU cpuCopy = cpu;

	cpu.R01 = 0x9;
	cpu.R02 = 0x2;
	cpu.IO.SREG.C = 1;

	// sbc r1,r2 ; Subtract r2 from r1 with carry high byte
	constexpr Word instruction =
		Instruction::SBC
		| 0b1'0000 // Rd = R1
		| 0b10; // Rr = R2

	int dummyCycles = 0;
	memory.WriteWord(instruction, 0x0, dummyCycles);

	// Act
	cpu.Execute(1, memory); // SBC takes 1 cycle

	// Assert
	EXPECT_EQ(cpu.R01, 0x6);
	
	EXPECT_EQ(cpu.IO.SREG.I, cpuCopy.IO.SREG.I);
	EXPECT_EQ(cpu.IO.SREG.T, cpuCopy.IO.SREG.T);

	EXPECT_FALSE(cpu.IO.SREG.I);
	EXPECT_FALSE(cpu.IO.SREG.S);
	EXPECT_FALSE(cpu.IO.SREG.V);
	EXPECT_FALSE(cpu.IO.SREG.N);
	EXPECT_FALSE(cpu.IO.SREG.Z);
	EXPECT_FALSE(cpu.IO.SREG.C);
}
