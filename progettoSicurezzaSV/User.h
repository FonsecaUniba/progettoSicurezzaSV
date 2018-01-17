#pragma once

#ifndef USER_CLASS
#define USER_CLASS

#include <vector>
#include "Signature/Signature.h"

class User
{

	/*
		Class representing an User

		Author: Nicola Oreste Fonseca
		Created on: 23 December 2017
	*/

public:
	/*
		Constuctor for Class User

		id - id to assign to user
	*/
	User(int id);
	
	/*
		Deconstructor for Class User
	*/
	~User();

	// Returns User's ID
	int get_id();

	// Returns this user's acceptance threshold
	double get_threshold();

	/*
		Sets this user's acceptance threshold

		threshold - threshold to set
	*/
	void set_threshold(double threshold);

	/*
		Vector containing all User's Signatures
	*/
	std::vector<Signature> user_signatures;

private:
	// User ID
	int id;
	// User's acceptance threshold (Default value 0.70)
	double threshold;
};

#endif //USER_CLASS