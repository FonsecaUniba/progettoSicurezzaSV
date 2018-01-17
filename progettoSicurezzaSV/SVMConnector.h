#pragma once

#ifndef SVM_CONNECTOR
#define SVM_CONNECTOR

#include <vector>

#include "InputOutput/DBConnector.h"

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
std::vector<double> predict_value(int userID, Signature to_check, float threshold);

/*
	Predicts if a given Signature is Genuine or Forgery
	The System will read the Threshold from the DB

	userID - User to check Signature of
	to_check - Signature to predict
*/
std::vector<double> predict_value(int userID, Signature to_check);

#endif //SVM_CONNECTOR