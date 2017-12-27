#pragma once

#include <vector>
#include "Instant.h"

class Signature
{

	/*
		Class representing a Signature

		Author: Nicola Oreste Fonseca
		Created on: 18 December 2017
	*/

public:
	// Signature Constructor
	Signature();
	// Signature Deconstructor
	~Signature();

	// Vector of Instant representing the time function of the Signature
	std::vector<Instant> time_sequence;
	
	/*
		Flag used to determine known Signatures
		When running the system at runtime
	*/
	bool is_known;
	/*
		Flag used on known Signatures when training and testing FAR/FRR
		True if the known Signature is genuine
		False if the known Signature is forgery
	*/
	bool is_genuine;
};

