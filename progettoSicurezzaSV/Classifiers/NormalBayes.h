#pragma once

#ifndef NORMALBAYES_CONNECTOR
#define NORMALBAYES_CONNECTOR

#include <vector>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/ml/ml.hpp>
#include <boost/array.hpp>

#include "../InputOutput/DBConnector.h"

#include "../User.h"
#include "../Signature/Signature.h"
#include "../Signature/Instant.h"

namespace normalbayes_connector {
	/*
	Functions used to interact with OpenCV NormalBayes Library

	Author: Nicola Oreste Fonseca
	Created on: 26 December 2017
	*/

	/*
	Trains the System to recognize an User
	The NormalBayes will be saved in /NBayes/[UserID].xml

	to_train - User to train the system for
	*/
	void train_classifier(User to_train);

	/*
	Given a Signature and a user id,
	loads the Tree from file path
	and returns a vector
	containing the distances between
	each Instant and the hyperplane

	userID - User to check Signature of
	to_check - Signature to predict
	path - Path where Tree model is stored
	*/
	float compute_distances(int userID, Signature to_check, std::string path);

	/*
	Tests a user signature at a given
	threshold

	userID - User to check Signature of
	to_check - Signature to predict
	threshold - Threshold at which the signature is accepted
	*/
	bool test_signature(int userID, Signature to_check, float threshold);

	/*
	Tests a user signature, the
	threshold value will be taken from
	the database

	userID - User to check Signature of
	to_check - Signature to predict
	*/
	bool test_signature(int userID, Signature to_check);
}

#endif //NORMALBAYES_CONNECTOR