#include "stdafx.h"
#include "SVMConnector.h"

void generate_model(/*TODO Identify Parameters*/) {

}


void train_svm(User to_train) {
	//Store User ID
	//Declare TrainingLabels and TrainingData

	//For each Signature
		//For each Instant in Signature
			//Determine if Signature is Genuine or Forgery
			//Add 1 if Genuine or -1 if Forgery to TrainingLabels

		//Get Signature representation as string of array of Instants
		//Example: S1 = "{d1, v1, a1, p1}, {d2, v2, a2, p2}, {d3, ...}, {...}"
		//Add Signature representation to TrainingData

		//Train SVM
}

std::vector<double> predict_value(int userID, Signature to_check, double threshold) {
	std::vector<double> results;
	
	//Get SVM model corresponding to userID from DB
	
	//For each Instant in Signature
		//Predict Distance
		//Add Distance to Array
		
	//Return Array of distance

	return results;
}

std::vector<double> predict_value(int userID, Signature to_check) {
	double threshold = get_threshold(userID);
	return predict_value(userID, to_check, threshold);

	//Get Threshold from DB
	//Call predict_value using obtained threshold
}