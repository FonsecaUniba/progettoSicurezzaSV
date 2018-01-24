/* 
	SVM-based Signature Authentication Software
	Created on: 15/12/2017
	Author: Nicola Oreste Fonseca

	Entry Point for an Online Signature Authentication System based on Support Vector Machines

	This file represent the entry point for the application,
	it will display a selection menu used to select one of the following options:

	1) Train the system using signatures from the Signatures/TrainingSet subfolder
			This option will train the signature verification software using a
			subset of SUSig Blind SubCorpus and will store the svm_model on the Database,
			the system will also store a default threshold of 0.70 per user in order to
			comply to the NOT_NULL constraint
	
	2) Test the system using signatures from the Signatures/TestingSet subfolder at a selected threshold value
			This option will test the system by comparing the testing subset's signatures to the
			model stored for each user at a given input treshold value.
			After the testing phase, the results will be saved as [Threshold].csv in the Results/CustomTest/ subfolder
			The .csv will display the results as a table composed of:
				UserID, an ID used to identify different users and differentiate results;
				SignatureID, an ID used to identify each iterate on the current user;
				Genuine/Forgery, a flag used to identify if the tested signature is true or false, useful to check FRR and FAR;
				Accepted/Rejected, the actual test result, showing if the signature was accepted or not by the system ;
				OK/FR/FA, the system result, showing if the signature was correctly categorized or if an error was made.
	
	3) Test the system using signatures from the Signatures/TestingSet subfolder at different threshold values
			This option will test the system by comparing the testing subset's signatures to the
			model stored for each user at different threshold values.
			After the testing phase, the results will be saved as a [Threshold].csv file for each different
			user in the Results/AllThresholdsTest/[UserID] subfolder
			The .csv will display the results as a table composed of:
				UserID, an ID used to identify different users and differentiate results;
				SignatureID, an ID used to identify each iterate on the current user;
				Genuine/Forgery, a flag used to identify if the tested signature is true or false, useful to check FRR and FAR;
				Accepted/Rejected, the actual test result, showing if the signature was accepted or not by the system;
				OK/FR/FA, the system result, showing if the signature was correctly categorized or if an error was made.
	
	4) Optimize the system
			This option will update each user's optimal threshold by analizing each [Threshold].csv
			stored in Results/AllThresholdsTest/[UserID] subfolder.
			Each file will be used to determine the optimal value at which both the
			FRR (False Rejection Rate) and the FAR (False Acceptance Rate) are minimal.
			This option will first produce a [UserID].csv stored in the Results/Optimization subfolder,
			which will contain:
				Threshold, the current threshold value;
				FAR, the false acceptance rate at that threshold value;
				FRR, the false rejection rate at that thrashold value.
			After the file has been created, this option will determine the optimal threshold and store it in the database.

	5) Exit the program

	9) Unit Test Code
*/

#include "stdafx.h"
#include <iostream>
#include <boost/filesystem.hpp>
#include <fstream>

#include "Test/DBConnectorTest.h"
#include "Test/FileInputTest.h"
#include "Test/InstantTest.h"
#include "Test/UserTest.h"

#include "User.h"
#include "Signature/Signature.h"
#include "Signature/Instant.h"

#include "InputOutput/FileInput.h"

#include "SVMConnector.h"

using namespace std;

const int USER_SIZE = 100;
const std::string FOUR_NEWLINE = "\n\n\n\n";

std::string set_result_string(int userID, int signatureID, bool result, bool genuine);
void create_folder(std::string path);
void start_training();
void start_testing_at_threshold();
void start_testing_at_all_thresholds();
void start_optimize();
void test_all();

int main()
{
	bool keep_going = true;
	bool is_system_trained = false;
	bool is_system_tested = false;

	do {

		cout << "Application for online signature authentication" << endl;
		cout << endl;
		cout << "Choose an action: " << endl;
		cout << "1) Train the system using signatures from the TrainingSet subfolder" << endl;
		cout << "2) Test the system using signatures from the TestingSet subfolder at a selected threshold value" << endl;
		cout << "3) Test the system using signatures from the TestingSet subfolder at different threshold values" << endl;
		cout << "4) Optimize the system" << endl;
		cout << "5) Exit the program" << endl;
		cout << "9) Unit Test Code" << endl << endl;

		int choice;
		cout << "Insert option value: ";
		cin >> choice;
		cout << endl;

		switch (choice) {
		case 1:
			cout << "Training System" << FOUR_NEWLINE;
			is_system_trained = true;
			start_training();
			break;
		case 2:
			if (is_system_trained) {
				cout << "Testing System" << FOUR_NEWLINE;
				start_testing_at_threshold();
			}
			else {
				cout << "Option 1 has not been selected yet. Please select before using Option 2" << FOUR_NEWLINE;
			}
			break;
		case 3:
			if (is_system_trained) {
				cout << "Testing System" << FOUR_NEWLINE;
				is_system_tested = true;
				start_testing_at_all_thresholds();
			}
			else {
				cout << "Option 1 has not been selected yet. Please select before using Option 3" << FOUR_NEWLINE;
			}
			break;
		case 4:
			if (is_system_trained) {
				cout << "Optimizing System" << FOUR_NEWLINE;
				start_optimize();
			}
			else {
				cout << "Option 3 has not been selected yet. Please select before using Option 4" << FOUR_NEWLINE;
			}
			break;
		case 5:
			cout << "Terminating Program" << FOUR_NEWLINE;
			keep_going = false;
			break;
		case 9:
			cout << "Unit Testing System" << FOUR_NEWLINE;
			test_all();
			break;
		default:
			break;
		}

	} while (keep_going);

    return 0;
}

void start_training() {
	User to_train = NULL;
	
	for (int i = 1; i <= USER_SIZE; i++) {
		std::string path = "Signatures/TrainingSubset/";

		//Pads UserID to 3 characters
		if (i < 10) {
			path += "00" + std::to_string(i);
		}
		else if (100) {
			path += "0" + std::to_string(i);
		}
		else {
			path += std::to_string(i);
		}

		path += "_f_1.sig";

		if ( boost::filesystem::exists(path) ) {
			to_train = read_one_user(i, true);
			train_svm(to_train);
		}
	}
}

std::string set_result_string(int userID, int signatureID, bool result, bool genuine) {
	//Sets result string
	std::string to_print = std::to_string(userID) + ",";
	to_print += std::to_string(signatureID) + ",";
	to_print += (genuine) ? "Genuine," : "Forgery,";
	to_print += (result) ? "Accepted," : "Rejected,";

	if (result == genuine) {
		to_print += "OK\n";
	}
	else if ((!result) && genuine) {
		to_print += "FR\n";
	}
	else {
		to_print += "FA\n";
	}

	return to_print;
}

void start_testing_at_threshold() {
	//Threshold to use
	double threshold;
	//Vector to store Results
	std::vector<std::string> test_results;

	//Ask for Threshold
	cout << "Insert threshold at which data will be tested" << endl
		<< "(values accepted between 0.00 and 1.00): ";
	cin >> threshold;

	//Where to store results
	std::string path = "Results/CustomTest/" + std::to_string(threshold) + ".csv";

	for (int i = 1; i <= USER_SIZE; i++) {
		std::string path = "Signatures/TestingSubset/";

		//Pads UserID to 3 characters
		if (i < 10) {
			path += "00" + std::to_string(i);
		}
		else if (100) {
			path += "0" + std::to_string(i);
		}
		else {
			path += std::to_string(i);
		}

		path += "_f_1.sig";

		if (boost::filesystem::exists(path)) {
			User user_to_test = read_one_user(i, false);
			//Store userID for readability
			int& userID = i;

			//For each signature
			for (int j = 0; j < user_to_test.user_signatures.size(); j++) {
				//Alias Signature for readability
				Signature& to_test = user_to_test.user_signatures.at(j);

				//Test Signature
				bool result = test_signature(userID, to_test, threshold);

				//Creates result string
				std::string to_print = set_result_string(userID, (j + 1), result, to_test.is_genuine);
				test_results.push_back(to_print);
			}

			print_results(path, test_results);
			test_results.clear();
		}
	}
}

void create_folder(std::string path) {
	boost::filesystem::path dir(path);
	if (boost::filesystem::create_directory(dir))
	{
		std::cerr << "Directory Created" << std::endl;
	}
}

void start_testing_at_all_thresholds() {
	//Vector to store Results
	std::vector<std::string> test_results;

	double threshold = 0.10;
	double ending_threshold = 1.00;

	//For each threshold
	while (abs(threshold - ending_threshold) > 1e-6) {
		//For each user
		for (int i = 1; i <= USER_SIZE; i++) {
			for (int i = 1; i <= USER_SIZE; i++) {
				std::string path = "Signatures/TestingSubset/";

				//Pads UserID to 3 characters
				if (i < 10) {
					path += "00" + std::to_string(i);
				}
				else if (100) {
					path += "0" + std::to_string(i);
				}
				else {
					path += std::to_string(i);
				}

				path += "_f_1.sig";

				if (boost::filesystem::exists(path)) {
					//Read user from file
					User user_to_test = read_one_user(i, false);
					//Store userID for readability
					int& userID = i;

					std::string path = "Results/AllThresholdsTest/" + std::to_string(userID) + "/";
					create_folder(path);
					path += std::to_string(threshold) + ".csv";

					//For each signature
					for (int j = 0; j < user_to_test.user_signatures.size(); j++) {
						//Alias Signature for readability
						Signature& to_test = user_to_test.user_signatures.at(j);

						//Test Signature
						bool result = test_signature(userID, to_test, threshold);

						//Creates result string
						std::string to_print = set_result_string(userID, (j + 1), result, to_test.is_genuine);
						test_results.push_back(to_print);
					}

					print_results(path, test_results);
					test_results.clear();
				}
			}
		}
		threshold += 0.10;
	}
}

void start_optimize() {
	const double FIRST = 0.10;
	const double STEP = 0.10;
	const double LAST = 1.00;
	const double MAX_FAR = 0.60;

	std::ofstream file;
	file.open("Results/Optimization/OptimalValues.csv");

	file << "UserID,Optimal\n";

	for (int i = 0; i < USER_SIZE; i++) {
		std::string threshold_path = "Results/AllThresholdsTest/" + std::to_string(i) + "/";
		std::string result_path = "Results/Optimization/" + std::to_string(i) + ".csv";

		print_optimization(threshold_path, result_path, FIRST, STEP, LAST);
		double optimal = read_user_optimal(result_path, MAX_FAR);
		
		//update_threshold((i+1), optimal);
		file << std::to_string(i+1) << "," << std::to_string(optimal) << "\n";
	}

	file.close();
}

void test_all() {
	test_instant_module();
	test_user_module();
	test_fileinput_module();
	//test_dbconnector_module();
}