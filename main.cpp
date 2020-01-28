#include "pch.h"
#include "Keccak.h"
#include <fstream>
#include <clocale>

int main(int argc, char* argv[])
{
	setlocale(LC_ALL, "russian");

	std::ifstream file("..//hash test.txt");
	std::string message;

	getline(file, message);

	Keccak hash(512);
	cout << "Message: " << message << endl << endl;
	std::string h = hash.createHashOf(message);
	cout << "Hash: " << h << endl;
	
	system("pause");
	return 0;
}


