#pragma once

#ifndef DB_TEST
#define DB_TEST

#include <iostream>
#include <math.h>
#include <string>
#include <cppconn/driver.h>
#include <cppconn/connection.h>
#include <cppconn/exception.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/resultset.h>

#include "../InputOutput/DBConnector.h"

/*
	Tests model save by saving a model,
	loading it (using custom code),
	checking the returned values and
	deleting the model
*/
void test_save_model();

/*
	Tests model load by saving a model,
	loading it (using library code),
	checking the returned values and
	deleting the model
*/
void test_load_model();

/*
	Tests the threshold return by
	saving a model,
	returning the threshold and
	comparing it to the default value (0.70)
*/
void test_get_threshold();

/*
	Tests the threshold return by
	saving a model, updating the threshold
	returning the threshold and
	comparing it to the new value
*/
void test_update_threshold();

/*
	Executes all previous tests
*/
void test_dbconnector_module();

#endif //DB_TEST