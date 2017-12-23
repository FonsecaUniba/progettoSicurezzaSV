#pragma once
#include <vector>
#include "Signature/Signature.h"

class User
{

	/*
		Class representing an User
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

	/*
		Vector containing all User's Signatures
	*/
	std::vector<Signature> user_signatures;

private:
	// User ID
	int id;
};

