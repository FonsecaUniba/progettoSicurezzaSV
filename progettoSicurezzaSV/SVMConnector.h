#pragma once

#ifndef SVM_CONNECTOR
#define SVM_CONNECTOR

#include <vector>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/ml/ml.hpp>
#include <boost/array.hpp>

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
	Trains the System to recognize an User
	The SVM will be saved in /SVMs/[UserID].xml

	to_train - User to train the system for
*/
void train_svm(User to_train);

/*
	Given a Signature and a user id,
	loads the SVM from the DB
	and returns a vector
	containing the distances between
	each Instant and the hyperplane

	userID - User to check Signature of
	to_check - Signature to predict
*/
std::vector<double> compute_distances(int userID, Signature to_check);

/*
	Given a Signature and a user id,
	loads the SVM from file path
	and returns a vector
	containing the distances between
	each Instant and the hyperplane

	userID - User to check Signature of
	to_check - Signature to predict
	path - Path where SVM model is stored
*/
std::vector<double> compute_distances(int userID, Signature to_check, std::string path);

/*
	Tests a user signature at a given
	threshold

	userID - User to check Signature of
	to_check - Signature to predict
	threshold - Threshold at which the signature is accepted
*/
bool test_signature(int userID, Signature to_check, double threshold);

/*
	Tests a user signature, the
	threshold value will be taken from
	the database

	userID - User to check Signature of
	to_check - Signature to predict
*/
bool test_signature(int userID, Signature to_check);

#endif //SVM_CONNECTOR