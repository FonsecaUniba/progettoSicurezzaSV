#pragma once

#ifndef INSTANT_TEST
#define INSTANT_TEST

#include <math.h>
#include <iostream>

#include "../Signature/Instant.h"


/*
	Testes Instant Contructor by initializing values
	and checking if they correspond to expected values
*/
void test_instant();

/*
	Executes all previous tests
	
	Useless, as there is only one test, but
	it keeps test naming consistency between
	files
*/
void test_instant_module();

#endif //INSTANT_TEST