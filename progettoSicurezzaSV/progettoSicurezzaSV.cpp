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
*/

#include "stdafx.h"
#include <iostream>

using namespace std;

#define FOUR_NEWLINE "\n\n\n\n";

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
		cout << "5) Exit the program" << endl << endl;

		int choice;
		cout << "Insert option value: ";
		cin >> choice;
		cout << endl;

		switch (choice) {
		case 1:
			cout << "Training System" << FOUR_NEWLINE;
			is_system_trained = true;
			break;
		case 2:
			if (is_system_trained) {
				cout << "Testing System" << FOUR_NEWLINE;
			}
			else {
				cout << "Option 1 has not been selected yet. Please select before using Option 2" << FOUR_NEWLINE;
			}
			break;
		case 3:
			if (is_system_trained) {
				cout << "Testing System" << FOUR_NEWLINE;
				is_system_tested = true;
			}
			else {
				cout << "Option 1 has not been selected yet. Please select before using Option 3" << FOUR_NEWLINE;
			}
			break;
		case 4:
			if (is_system_trained) {
				cout << "Optimizing System" << FOUR_NEWLINE;
			}
			else {
				cout << "Option 3 has not been selected yet. Please select before using Option 4" << FOUR_NEWLINE;
			}
			break;
		case 5:
			cout << "Terminating Program" << FOUR_NEWLINE;
			keep_going = false;
			break;
		default:
			break;
		}

	} while (keep_going);

    return 0;
}