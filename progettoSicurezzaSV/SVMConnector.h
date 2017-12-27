#pragma once

#include "User.h"
#include "Signature/Signature.h"
#include "Signature/Instant.h"

/*
	Functions used to interact with OpenCV SVM Library
	
	Author: Nicola Oreste Fonseca
	Created on: 26 December 2017
*/

/*
	Generates the SVM model for an User
	Stores the model on the Database
*/
void generate_model(/*TODO Identify Parameters*/);

/*
	Saves the testing results on a .csv file
	The File will be stored as [Threshold].csv in Results/CustomTest/

	userID - ID of the user tested, used to name the .csv as UserID.csv
	is_genuine - Was the Signature tested Genuine or Forgery?
	threshold - Threshold at which the Signature is considered genuine
	prediction - Similarity value predicted by the SVM
*/
void print_custom_results(std::string userID, bool is_genuine, float threshold, float prediction);

/*
	Saves the testing results on a .csv file
	The File will be stored as [Threshold].csv in Results/AllThresholdsTest/[UserID]

	userID - ID of the user tested, used to name the .csv as UserID.csv
	is_genuine - Was the Signature tested Genuine or Forgery?
	threshold - Threshold at which the Signature is considered genuine
	prediction - Similarity value predicted by the SVM
*/
void print_results(std::string userID, bool is_genuine, float threshold, float prediction);

/*
	Trains the System to recognize an User
	The SVM will be saved in /SVM/[UserID]

	to_train - User to train the system for
*/
void train_svm(User to_train);

/*
	Predicts if a given Signature is Genuine or Forgery

	userID - User to check Signature of
	to_check - Signature to predict
	threshold - Threshold at which the system will consider the Signature Genuine
*/
void predict_value(std::string userID, Signature to_check, float threshold);

/*
	Predicts if a given Signature is Genuine or Forgery
	The System will read the Threshold from the DB

	userID - User to check Signature of
	to_check - Signature to predict
*/
void predict_value(std::string userID, Signature to_check);