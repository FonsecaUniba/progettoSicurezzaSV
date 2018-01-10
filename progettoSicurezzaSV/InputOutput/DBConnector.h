#pragma once

#include <string>
#include <cppconn/driver.h>
#include <cppconn/connection.h>
#include <cppconn/exception.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/resultset.h>
#include <cppconn/resultset_metadata.h>

/*
	Interface to connect the software to a mySQL Database
	
	The UserID is represented by an integer value for
	simplicity sake, but in an actual system it should
	be substituted by a generated value

	Author: Nicola Oreste Fonseca
	Created on: 27 December 2017
*/

/*
	Saves the path to the svm model on the Database

	user_id - ID for the user
	model_path - Complete path to the file containing the model
*/
void save_model(int user_id, std::string model_path);

/*
	Returns the path to the svm model

	user_id - ID for the user
*/
std::string load_model(int user_id);

/*
	Returns the threshold for the specified user

	user_id - ID for the user
*/
double get_threshold(int user_id);

/*
	Changes the threshold for the specified user

	user_id - ID for the user
	threshold - value to set as new threshold
*/
void update_threshold(int user_id, double threshold);