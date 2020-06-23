#include "pch.h"
#include "Keccak.h"
#include <fstream>
#include <clocale>

int main(int argc, char* argv[])
{
	setlocale(LC_ALL, "russian");

	std::ifstream encode_file("encode.txt"); //file with text to encode
	std::ifstream length_file("length.txt"); //file with hash length
	std::ofstream hash_file("hash.txt"); //file with resulting hash

	if (!encode_file || !hash_file || !length_file)
	{
		cout << "Программа не смогла открыть один из файлов!";
		cout << "Возможно, файл имеет неправильное название.";
		system("pause");
		return -1;
	}

	int hash_length = 512;
	std::string message;

	length_file >> hash_length;

	getline(encode_file, message);

	try
	{
		Keccak hash(hash_length);
		std::string result = hash.createHashOf(message);
		hash_file << result;
	}
	catch (std::domain_error err)
	{
		cout << err.what();
		system("pause");
		return -1;
	}

	encode_file.close();
	length_file.close();
	hash_file.close();
	return 0;
}


