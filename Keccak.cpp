#include "pch.h"
#include "round_constant.h"
#include "Keccak.h"
#include <iomanip>
#define XOR ^

Keccak::Keccak(UInt16 exit_length)
{
	//initializing r and c depending on exit hash length in bits
	switch (exit_length)
	{
	case 128: r = 1344; c = 256; break;
	case 224: r = 1156; c = 448; break;
	case 256: r = 1088; c = 512; break;
	case 384: r = 832; c = 768; break;
	case 512: r = 576; c = 1024; break;
	default: throw std::domain_error("Wrong exit length"); break;
	}

	//converting bites in bytes
	r /= 8; 
	c /= 8;
	d = exit_length/8;

}

void Keccak::PRINT_STATE_STRING()
{
	/*prints state string as its 
	double dimension hex representation*/

	cout.unsetf(std::ios::dec);
	cout.setf(std::ios::hex);

	for (size_t i = 0; i < state_size; ++i)
	{
		for (size_t j = 0; j < state_size; ++j)
			cout << std::setfill('0') << std::setw(16) << S[i][j] << "| |";
		cout << endl;
	}
	cout << endl << endl;

	cout.unsetf(std::ios::hex);
	cout.setf(std::ios::dec); 
}

void Keccak::PRINT_ARRAY(UInt64* array, size_t size, int field_width)
{
	cout.unsetf(std::ios::dec);
	cout.setf(std::ios::hex);

	for (size_t i = 0; i < size; ++i)
		cout << std::setfill('0') << std::setw(field_width) << array[i] << "| |";

	cout.unsetf(std::ios::hex);
	cout.setf(std::ios::dec);
}

void Keccak::PRINT_ARRAY(unsigned char* array, size_t size, int field_width)
{
	if (field_width)
	{
		cout.unsetf(std::ios::dec);
		cout.setf(std::ios::hex);
		for (size_t k = 0; k < size; ++k)
			cout << std::setfill('0') << std::setw(field_width) << (int)array[k] << " ";

		cout.unsetf(std::ios::hex);
		cout.setf(std::ios::dec);
	}
	else
		for (size_t i = 0; i < d; ++i)
			cout << array[i];
		
	cout << endl << endl;
}

Keccak::~Keccak()
{
}

void Keccak::Pad(Queue<unsigned char> &source_message)
{
	if (source_message.getSize() % r == r - 1)
		source_message.enqueue(0x81);
	else
	{
		size_t zeroes_amount = r - (source_message.getSize() % r) - 2;

		source_message.enqueue(0x01);

		for (size_t i = 0; i < zeroes_amount; ++i)
			source_message.enqueue(0);

		source_message.enqueue(0x80);
	}
}

void Keccak::Absorb(UInt64* Pi)
{
	//XORing Pi with state string
	for (size_t i = 0; i < state_size; ++i)
		for (size_t j = 0; j < state_size; ++j)
			S[i][j] = S[i][j] XOR Pi[i + state_size * j];
			
	f(); //permutate state string
}

void Keccak::f()
{
	for (size_t round_number = 0; round_number < rounds_amount; ++round_number)
	{
		//arrays initialization
		UInt64 *C = new UInt64[state_size];
		for (size_t i = 0; i < state_size; ++i)
			C[i] = 0;

		UInt64 *D = new UInt64[state_size];
		for (size_t i = 0; i < state_size; ++i)
			D[i] = 0;

		UInt64 **F = new UInt64*[state_size];
		for (size_t i = 0; i < state_size; ++i)
		{
			F[i] = new UInt64[state_size];
			for (size_t j = 0; j < state_size; ++j)
				F[i][j] = 0;
		}

		///*---TEST---*///
		cout << endl << "Round " << round_number + 1 << " /24" << endl;
		PRINT_STATE_STRING();
		///*---END TEST---*///


		///*---THETA STEP---*///
		{
			for (size_t i = 0; i < state_size; ++i)
				C[i] = S[i][0] XOR S[i][1] XOR S[i][2] XOR S[i][3] XOR S[i][4];

			for (size_t i = 0; i < state_size; ++i)
				D[i] = C[(i + 4) % 5] XOR cyclicShift(C[(i + 1) % 5], 1);

			for (size_t i = 0; i < state_size; ++i)
				for (size_t j = 0; j < state_size; ++j)
					S[i][j] = S[i][j] XOR D[i];

			///*---TEST---*///
			//cout << endl << "C: ";
			//PRINT_ARRAY(C, 16);

			//cout << endl << "D: ";
			//PRINT_ARRAY(D, 16);
		
			//cout << endl << endl << "S[i][j] Theta:" << endl;
			//PRINT_STATE_STRING();		
			///*---END TEST---*///
		}

		///*---PI STEP---*///
		{
			for (size_t i = 0; i < state_size; ++i)
				for (size_t j = 0; j < state_size; ++j)
					F[j][(2 * i + 3 * j) % 5] = cyclicShift(S[i][j], shift[i][j]);
		}


		///*---CHI STEP---*///
		{
			for (size_t i = 0; i < state_size; ++i)
				for (size_t j = 0; j < state_size; ++j)
					S[i][j] = F[i][j] XOR((~F[(i + 1) % 5][j]) & F[(i + 2) % 5][j]);
		}


		///*---IOTA STEP---*///
		{
			S[0][0] = S[0][0] XOR RC[round_number];
		}

		for (size_t i = 0; i < state_size; ++i)
			delete F[i];

		delete[] C;
		delete[] D;
		delete[] F;
	}
}

UInt64 Keccak::cyclicShift(UInt64 word, size_t n)
{
	n = n % (8*w);
	return ((word << n) | (word >> (8*w - n)));
}

std::string Keccak::createHashOf(std::string string_message)
{
	///*---PADDING AND INITIALIZATION---*///
	Queue<unsigned char> M; //source message that will be padded
	for (int i = 0; i < string_message.length(); ++i)
		M.enqueue(string_message[i]);

	Pad(M); 

	for (size_t i = 0; i < state_size; ++i)
		for (size_t j = 0; j < state_size; ++j)
			S[i][j] = 0;



	///*---ABSORBING---*///
	size_t blocks_amount = M.getSize() / r;
	unsigned char* block = new unsigned char[b]; 

	for (size_t i = 0; i < blocks_amount; ++i)
	{
		//expanding the block length up to 200 bytes with zeroes
		for (size_t byte = 0; byte < b; ++byte)
		{
			if (byte < r)
				block[byte] = M.dequeue();
			else
			{
				///*---TEST---*///
				if (byte == r)
				{
					PRINT_ARRAY(block, r, 2);
				}
				///*---END TEST---*///

				block[byte] = 0x00;
			}
		}

		UInt64* P_i = (UInt64*)block;

		PRINT_ARRAY(P_i, b/w ,2);

		Absorb(P_i); //block permutations

	}

	///*---SQUEEZING---*///
	std::string hash;
	unsigned char* Z = new unsigned char[d];
	size_t output_byte_length = 0;

	for (size_t i = 0; i < state_size; ++i)
		for (size_t j = i; j < state_size; ++j)
			std::swap(S[i][j], S[j][i]);

	///*---TEST---*///
	//cout << "\n\n\n\n SQUEEZING!!!!!!" << endl;
	//PRINT_STATE_STRING();
	///*---END TEST---*///

	memcpy(Z, S, d); //copying first 'd' bytes of state string into messsage Z (hash bytes)

	const char *digits = "0123456789abcdef"; // all hex digits

	///*---TEST---*///
	//PRINT_ARRAY(Z, 0);
	///*---END TEST---*///

	//converting symbolic message to hex message
	while (hash.length() < 2*d)
	{
		for (int byte = 0; byte < d; byte++)
		{
			hash += digits[Z[byte] >> 4];
			hash += digits[Z[byte] & 15];
		}
	}

	delete[] block;
	delete[] Z;
	return hash;
}
