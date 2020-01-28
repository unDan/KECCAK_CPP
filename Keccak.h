#pragma once
#include <cmath>
#include <iostream>
#include <string>
#include <cstdlib>
#include <stdexcept>
#include <cmath>
#include "Queue.h"

using std::cin;
using std::cout;
using std::endl;

typedef unsigned short UInt16;
typedef unsigned long long UInt64;

class Keccak
{
private:
	const UInt16 b = 200; //state string length [bytes]
	const UInt16 w = 8; //word length [bytes]
	const UInt16 state_size = 5; //state string dimension size
	const UInt16 rounds_amount = 24;
	UInt16 d; //output string length [bytes]
	UInt16 r; //rate [bytes]
	UInt16 c; //capacity [bytes]
	UInt64 S[5][5]; //state string


	void Pad(Queue<unsigned char> &source_message); //extends source message to length multiple to r
	void Absorb(UInt64* Pi); //absorbs message block
	void f(); //Keccak-f permutations
	UInt64 cyclicShift(UInt64 word, size_t n); //cylicly bit-shifts the number to the left
	
	void PRINT_STATE_STRING(); //debugging function
    void PRINT_ARRAY(UInt64* array, size_t size, int field_width); //debugging function
	void PRINT_ARRAY(unsigned char* array, size_t size, int field_width); //debugging function

public:

	Keccak(UInt16 exit_length);

	std::string createHashOf(std::string message); //calculates hash and returns it as string

	~Keccak();
};

