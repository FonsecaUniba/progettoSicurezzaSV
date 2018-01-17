#pragma once

#ifndef USER_TEST
#define USER_TEST

#include <iostream>
#include <math.h>

#include "../User.h"

/*
	Testes User Constructor by creating an User
	and checking if attributes correspond to
	expected values
*/
void test_user();

/*
	Testes threshold setter by setting a threshold
	and checking if it has been memorized
*/
void test_set_threshold();

/*
	Executes all previous tests
*/
void test_user_module();

#endif //USER_TEST