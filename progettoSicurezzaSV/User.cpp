#include "stdafx.h"
#include "User.h"


User::User(int id)
{
	this->id = id;
	this->threshold = (float) 0.70;
}


User::~User()
{
	this->user_signatures.clear();
}

int User::get_id() {
	return this->id;
}

float User::get_threshold() {
	return this->threshold;
}

void User::set_threshold(float threshold) {
	this->threshold = threshold;
}