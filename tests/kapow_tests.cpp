#include "Kapow.Main.h"

bool test_main()
{
	debug("test_main BEGIN\n");
	//	TODO: put tests here.
	debug("test_main ENDS\n");
	
	return true;
}

int main(int argc, char** argv)
{
	static_assert(test_main());

	return 0;
}