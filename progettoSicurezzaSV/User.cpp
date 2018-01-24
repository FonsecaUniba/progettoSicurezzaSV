#include "stdafx.h"
#include "User.h"


User::User(int id)
{
	this->id = id;
	this->threshold = 0.70;
}


User::~User()
{
	this->user_signatures.clear();
}

int User::get_id() {
	return this->id;
}

double User::get_threshold() {
	return this->threshold;
}

void User::set_threshold(double threshold) {
	this->threshold = threshold;
}