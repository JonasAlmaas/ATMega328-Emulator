#include "ATMega328Emulator/CPU.h"

using namespace ATMega328Emulator;

#include "gtest/gtest.h"

#if 0

int main()
{
	Memory memory;
	CPU cpu;

	cpu.Reset(memory);
	
	// TODO: Inject program into memory

	cpu.Execute(0, memory);
	
	return 0;
}
#endif

int main(int argc, char** argv)
{
	testing::InitGoogleTest(&argc, argv);

	return RUN_ALL_TESTS();
}
