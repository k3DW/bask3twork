#include <iostream>

int main(int argc, char** argv)
{
	for (int i = 0; i < argc; i++)
		std::cout << i << " -> " << argv[i] << "\n";

	if (argc == 0)
		std::cout << "argc is 0\n";
}
