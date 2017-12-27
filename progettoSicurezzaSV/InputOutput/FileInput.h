#pragma once

#include <fstream>
#include <string>
#include <iostream>
#include <vector>
#include <sstream>
#include <iterator>

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