#pragma once

#ifndef FILE_TEST
#define FILE_TEST

#include <string>
#include <vector>
#include <iostream>

#include "../User.h"
#include "../Signature/Signature.h"
#include "../InputOutput/FileInput.h"

/*
	Tests the splitting of a string when
	a whitespace occurs by creating a string
	of three words separated by a whitespace
	and checking if the returned array has
	three elements
*/
void test_split_string();

/*
	Tests signature reading by pointing
	trying to read a genuine and forged signature
	and checking if values match expected values
*/
void test_read_signature();

/*
	Tests user's signature reading by
	checking if returned vector size
	matches expected user's signature
	count
*/
void test_read_all_signatures();

/*
	Tests all user's reading by
	checking if returned vector size
	matches expected size
*/
void test_read_all_users();

/*
	Tests the result printing
	by trying to print static results
*/
void test_print_results();

/*
	Tests the splitting of a string when
	a comma occurs by creating a string
	of three words separated by a comma
	and checking if the returned array has
	three elements
*/
void test_split_by_comma();

/*
	Tests the average calculation by
	setting up a .csv file and checking
	if returned averages match expected values
*/
void test_calculate_averages();

void test_print_optimization();

void test_read_user_optimal();

/*
	Executes all previous tests
*/
void test_fileinput_module();

#endif //FILE_TEST