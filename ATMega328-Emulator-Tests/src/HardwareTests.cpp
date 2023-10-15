#include <gtest/gtest.h>

#include "ATMega328Emulator/CPU.h"

using namespace ATMega328Emulator;

TEST(HardwareTests, CPUMemoryLayout)
{
	EXPECT_EQ(offsetof(CPU, CPU::R00), 0x0);
	EXPECT_EQ(offsetof(CPU, CPU::R31), 0x1F);

	EXPECT_EQ(offsetof(CPU, CPU::IO), 0x20);

	// https://www.arnabkumardas.com/arduino-tutorial/gpio-register-description
	EXPECT_EQ(offsetof(CPU::IORegisters, CPU::IORegisters::PINB), 0x03);
	EXPECT_EQ(offsetof(CPU::IORegisters, CPU::IORegisters::DDRB), 0x04);
	EXPECT_EQ(offsetof(CPU::IORegisters, CPU::IORegisters::PORTB), 0x05);
	EXPECT_EQ(offsetof(CPU::IORegisters, CPU::IORegisters::PINC), 0x06);
	EXPECT_EQ(offsetof(CPU::IORegisters, CPU::IORegisters::DDRC), 0x07);
	EXPECT_EQ(offsetof(CPU::IORegisters, CPU::IORegisters::PORTC), 0x08);
	EXPECT_EQ(offsetof(CPU::IORegisters, CPU::IORegisters::PIND), 0x09);
	EXPECT_EQ(offsetof(CPU::IORegisters, CPU::IORegisters::DDRD), 0x0A);
	EXPECT_EQ(offsetof(CPU::IORegisters, CPU::IORegisters::PORTD), 0x0B);
}
