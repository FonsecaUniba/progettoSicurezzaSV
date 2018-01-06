#include "stdafx.h"
#include "User.h"


User::User(int id)
{
	this->id = id;
}


User::~User()
{
}

int User::get_id() {
	return this->id;
	this->threshold = 0.70;
}

double User::get_threshold() {
	return this->threshold;
}

void User::set_threshold(double threshold) {
	this->threshold = threshold;
}