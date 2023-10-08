#include "AVREmulator/Memory.h"

#include <cstring>
#include <cstdlib>

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
