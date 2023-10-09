#pragma once

#include <gtest/gtest.h>

#include "ATMega328Emulator/CPU.h"

using namespace ATMega328Emulator;

class ATMega328 : public testing::Test
{
public:
	void SetUp() override
	{
		cpu.Reset(memory);
	}

	void TearDown() override
	{
	}

public:
	Memory memory;
	CPU cpu;
};
