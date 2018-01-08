#pragma once

#include <fstream>
#include <string>
#include <iostream>
#include <vector>
#include <sstream>
#include <iterator>
#include <iomanip>
#include <math.h>

#include "../Signature/Instant.h"
#include "../Signature/Signature.h"
#include "../User.h"

/*
	Functions used to read .sig files

	Author: Nicola Oreste Fonseca
	Created on: 26 December 2017
*/

/*
	Splits a string when a whitespace occurs
	The resulting substrings are stored in a vector

	str - String to split in substrings

	Example:
	"This is a test string"
	{"This", "is", "a", "test", "string"}
*/
std::vector<std::string> split_string(std::string str);

/*
	Reads a single .sig file and returns it as a Signature Object

	file_to_read - Complete path to file .sig (Ex: "C:\Signatures\File.sig" or "Files\File.sig")
	is_genuine - Is the Signature Genuine or Forgery?
*/
Signature read_signature(std::string file_to_read, bool is_genuine);

/*
	Reads all signatures of a given user and returns it as vector of Signature
	The files are stored as userID_isGenuine_signatureID.sig
	Example: 001_g_5.sig means User 001, Genuine, Fifth Signature

	path - Path to where Signatures are stored
	user - UserID to read (Will only read user_* files)
*/
std::vector<Signature> read_all_signatures(std::string path, std::string user);

/*
	Reads all the Signatures of all the Users and returns a vector of User
	Applying two fold cross-validation, the path to the signatures
	will chance depending on which phase it's currently undergoing

	isTraining - Is it Training or Testing phase?
*/
std::vector<User> read_all_users(bool isTraining);

/*
	Creates a [Threshold].csv file in path containing the results
	of a test executed at a given threshold value

	path - complete path to [Threshold].csv
	threshold - threshold at which the test was executed
	results - vector of results containing the following informations separated by comma
		- UserID
		- SignatureID
		- Genuine/Forgery
		- Accepted/Rejected
		- OK/FR/FA
		Ex: "99,1,Genuine,Accepted,OK"
*/
void print_results(std::string path, std::vector<std::string> results);

/*
	Analizes all [Threshold].csv in threshold_path in order to
	determine the FAR (False Acceptance Rate) and FRR (False Rejection Rate)
	and will store the results in a [UserID].csv in results_path

	[UserID].csv will contain:
		- Threshold, the current threshold value;
		- FAR, the false acceptance rate at that threshold value;
		- FRR, the false rejection rate at that thrashold value.

	threshold_path - complete path to [Threshold].csv
	result_path - complete path to [UserID].csv in which results are to be stored
	first - first Threshold value used
	step - step at which threshold value increses
	stop - last threshold value used
*/
void print_optimization(std::string threshold_path, std::string results_path, double first, double step, double stop);

/*
	Reads the [UserID].csv in path, determines the threshold value at which
	FAR (False Acceptance Rate) and FRR (False Rejection Rate) are minimal
	and returns that threshold value.

	path - complete path to [UserID].csv
	max_far - maximum False Acceptance Rate allowed
*/
double read_user_optimal(std::string path, double max_far);