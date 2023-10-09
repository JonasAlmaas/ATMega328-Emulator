#include "ATMega328Emulator/Memory.h"

#include <cstring>
#include <cstdlib>

namespace ATMega328Emulator {
	
	Memory::Memory()
	{
		Data = (Byte*)std::malloc(MAX_MEM);
	}

	Memory::~Memory()
	{
		delete[] Data;
	}

	void Memory::Initialize()
	{
		std::memset(Data, 0, MAX_MEM);
	}

}
