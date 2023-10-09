#include <gtest/gtest.h>

#include "ATMega328Emulator/CPU.h"

using namespace ATMega328Emulator;

TEST(HardwareTests, CPUMemoryLayout)
{
	EXPECT_EQ(offsetof(CPU, CPU::R00), 0x0);
	EXPECT_EQ(offsetof(CPU, CPU::R31), 0x1F);

	// TODO: Figure out memmory layout
	//EXPECT_EQ(offsetof(CPU, CPU::SPL), 0x5D);
	//EXPECT_EQ(offsetof(CPU, CPU::SPL), 0x5E);
	//EXPECT_EQ(offsetof(CPU, CPU::SREG), 0x5F);
	
	//EXPECT_EQ(offsetof(CPU, CPU::SRAM), 0x100);
}
