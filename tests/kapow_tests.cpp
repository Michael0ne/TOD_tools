#include "Kapow.Main.h"

bool test_main()
{
	#pragma message("[MAIN] Checking CRC32 implementation...");
	static_assert(check_crc() == true, "[MAIN] CRC32 has FAILED!");
	#pragma message("[MAIN] Checking CRC32 success!");
	
	return true;
}

bool check_crc()
{
	Utils::generic_crc32()
}

int main(int argc, char** argv)
{
	static_assert(test_main());

	return 0;
}