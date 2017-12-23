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
}