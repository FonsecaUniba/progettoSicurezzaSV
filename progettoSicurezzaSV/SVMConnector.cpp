#include "stdafx.h"
#include "SVMConnector.h"

template <typename T>
cv::Mat_<T> vec2cvMat_2D(std::vector< std::vector<T> > &inVec) {
	int rows = static_cast<int>(inVec.size());
	int cols = static_cast<int>(inVec[0].size());

	cv::Mat_<T> resmat(rows, cols);
	for (int i = 0; i < rows; i++)
	{
		resmat.row(i) = cv::Mat(inVec[i]).t();
	}
	return resmat;
}

void train_svm(User to_train) {
	//Define dimensionality constants
	const int LABEL_DIMENSION = 1;
	const int PARAMETERS_COUNT = 4;

	//Stores User ID
	int id = to_train.get_id();

	//Instant Counter
	int instant_count = 0;
	//Label and Training Vector
	std::vector<int> label_vector;
	std::vector<std::vector<double>> training_vector;

	//For each Instant in each Signature
	for (int i = 0; i < to_train.user_signatures.size(); i++) {
		for (int j = 0; j < to_train.user_signatures.at(i).time_sequence.size(); j++) {
			//Counts the elements
			instant_count++;

			//Add 1 if Signature is genuine, -1 otherwise
			int to_add = (to_train.user_signatures.at(i).is_genuine) ? 1 : -1;
			label_vector.push_back(to_add);

			//Temporary vector to push data to Bidimensional Vector
			std::vector<double> to_push;

			//Adds parameters to vector
			to_push.push_back(to_train.user_signatures.at(i).time_sequence.at(j).get_displacement());
			to_push.push_back(to_train.user_signatures.at(i).time_sequence.at(j).get_velocity());
			to_push.push_back(to_train.user_signatures.at(i).time_sequence.at(j).get_acceleration());
			to_push.push_back(to_train.user_signatures.at(i).time_sequence.at(j).get_pressure());

			//Adds vector to bidimensional vector
			training_vector.push_back(to_push);
			//Clears temporary vector
			to_push.clear();
		}
	}

	std::cout << label_vector.size() << std::endl;
	std::cout << training_vector.size() << std::endl;

	try{
		//Declare TrainingLabels and TrainingData
		cv::Mat labels_mat(label_vector, true);
		cv::Mat training_mat = vec2cvMat_2D(training_vector);
		cv::Ptr<cv::ml::TrainData> td = cv::ml::TrainData::create(training_mat, cv::ml::ROW_SAMPLE, labels_mat);

		//Sets up the SVM
		//cv::Ptr<cv::ml::SVM> svm;
		//Longer Training times, better optimization
		//svm->trainAuto(td);

		
		//Worse parameter optimization but better performances
		// Set up SVM for OpenCV 3
		cv::Ptr<cv::ml::SVM> svm = cv::ml::SVM::create();
		// Set SVM type
		svm->setType(cv::ml::SVM::C_SVC);
		// Set SVM Kernel to Radial Basis Function (RBF)
		svm->setKernel(cv::ml::SVM::RBF);
		// Set parameter C
		svm->setC(12.5);
		// Set parameter Gamma
		svm->setGamma(0.50625);

		// Train SVM on training data
		svm->train(td);

		//Sets svm save path
		std::string path = "SVMs/" + std::to_string(id) + ".xml";
		//Save svm to file
		svm->save(path);
		//Store svm path on DB Currently disabled
		//save_model(id, path);

		label_vector.clear();
		training_vector.clear();
		delete svm;
		delete td;
	}
	catch (cv::Exception &e) {
		std::cout << e.what() << std::endl;
	}
}

std::vector<double> compute_distances(int userID, Signature to_check) {
	const int PARAMETERS_COUNT = 4;
	std::vector<double> results;
	
	//Get SVM model corresponding to userID from DB
	std::string path = load_model(userID);
	
	//Sets up the SVM
	cv::Ptr<cv::ml::SVM> svm;
	svm->load(path);

	//For each instant in the time sequence
	for (int i = 0; to_check.time_sequence.size(); i++) {
		std::vector<double> instant_vector(PARAMETERS_COUNT);

		instant_vector.push_back(to_check.time_sequence.at(i).get_displacement());
		instant_vector.push_back(to_check.time_sequence.at(i).get_velocity());
		instant_vector.push_back(to_check.time_sequence.at(i).get_acceleration());
		instant_vector.push_back(to_check.time_sequence.at(i).get_pressure());
		
		//Creates Mat to test
		cv::Mat test_mat(PARAMETERS_COUNT, 1, CV_32FC1, instant_vector.data());

		//Predict distance
		double distance = svm->predict(test_mat);
		
		//Adds Distance to results vector
		results.push_back(distance);
		//Clears Vector
		instant_vector.clear();
	}

	return results;
}

std::vector<double> compute_distances(int userID, Signature to_check, std::string path) {
	const int PARAMETERS_COUNT = 4;
	std::vector<double> results;

	//Sets up the SVM
	cv::Ptr<cv::ml::SVM> svm;
	svm->load(path);

	//For each instant in the time sequence
	for (int i = 0; to_check.time_sequence.size(); i++) {
		std::vector<double> instant_vector(PARAMETERS_COUNT);

		instant_vector.push_back(to_check.time_sequence.at(i).get_displacement());
		instant_vector.push_back(to_check.time_sequence.at(i).get_velocity());
		instant_vector.push_back(to_check.time_sequence.at(i).get_acceleration());
		instant_vector.push_back(to_check.time_sequence.at(i).get_pressure());

		//Creates Mat to test
		cv::Mat test_mat(PARAMETERS_COUNT, 1, CV_32FC1, instant_vector.data());

		//Predict distance
		double distance = svm->predict(test_mat);

		//Adds Distance to results vector
		results.push_back(distance);
		//Clears Vector
		instant_vector.clear();
	}

	return results;
}

bool test_signature(int userID, Signature to_check, double threshold) {
	std::vector<double> distances = compute_distances(userID, to_check);

	int accepted_count = 0;
	int rejected_count = 0;

	for (int i = 0; i < distances.size(); i++) {
		if (distances.at(i) > threshold) {
			accepted_count++;
			//Possible plotting
		}
		else {
			rejected_count++;
			//Possible plotting
		}
	}

	return (accepted_count > rejected_count) ? true : false;
}

bool test_signature(int userID, Signature to_check) {
	double threshold = get_threshold(userID);
	return test_signature(userID, to_check, threshold);
}