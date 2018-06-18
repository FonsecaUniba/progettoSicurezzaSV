#include "stdafx.h"
#include "SVMConnector.h"
#include <boost/filesystem.hpp>

#include <fstream>

namespace svm_connector {
	/*
		Pads the Signature to get WANTED_LENGTH Points
	*/
	std::vector<std::vector<float>> padding(Signature to_pad) {
		const int MAX_LENGTH = 16384;
		std::vector<std::vector<float>> result;
		result.reserve(MAX_LENGTH);
		std::vector<float> to_add;
		to_add.reserve(5);

		for (Instant elem : to_pad.time_sequence) {
			to_add.clear();
			to_add.push_back(elem.get_displacement());
			to_add.push_back(elem.get_velocity());
			to_add.push_back(elem.get_acceleration());
			to_add.push_back(elem.get_pressure());
			to_add.push_back(elem.get_timestamp());
			result.push_back(to_add);
		}

		while (result.size() < MAX_LENGTH) {
			to_add.clear();
			to_add.push_back(0.0);
			to_add.push_back(0.0);
			to_add.push_back(0.0);
			to_add.push_back(0.0);
			to_add.push_back(0.0);
			result.push_back(to_add);
		}

		return result;
	}

	void train_classifier(User to_train) {
		//Label and Training Vector
		std::vector<int> label_vector;
		std::vector<std::vector<float>> training_vector;

		//For each Signature
		for (int i = 0; i < to_train.user_signatures.size(); i++) {

			//Pad the signature to a given size and stores data
			std::vector<std::vector<float>> signature_data = padding(to_train.user_signatures.at(i));

			//For every element in the signature
			for (int j = 0; j < signature_data.size(); j++) {
				//Add 1 if Signature is genuine, -1 otherwise
				int to_add = (to_train.user_signatures.at(i).is_genuine) ? 1 : -1;
				label_vector.push_back(to_add);

				//Add element to training_vector
				training_vector.push_back(signature_data.at(j));
			}
		}

		try {
			//Declares labels_mat and training_mat
			cv::Mat labels_mat(label_vector, true);
			cv::Mat_<float> training_mat;

			//Copies data from training_vector to trainig_mat
			for (const auto & v : training_vector)
			{
				cv::Mat_<float> r(v), t = r.t(); // you need to do this
				training_mat.push_back(t); // because push_back(Mat_<float>(v).t()) does not work
			}

			//Converts the two mats to the type supported by the SVM trainer
			labels_mat.convertTo(labels_mat, CV_32S);
			training_mat.convertTo(training_mat, CV_32F);
			//Declares Training Data using training_mat and labels_mat
			cv::Ptr<cv::ml::TrainData> td = cv::ml::TrainData::create(training_mat, cv::ml::ROW_SAMPLE, labels_mat);

			//Sets up the SVM
			cv::Ptr<cv::ml::SVM> svm = cv::ml::SVM::create();
			//Longer Training times, better optimization
			//svm->trainAuto(td);

			
			//Worse parameter optimization but better performances
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
			std::string path = "Classifiers/SVMs/" + std::to_string(to_train.id) + ".xml";
			//Save svm to file
			svm->save(path);
			//Store svm path on DB Currently disabled
			//save_model(id, path);
		}
		catch (cv::Exception &e) {
			std::cout << e.what() << std::endl;
		}
	}

	float compute_distances(int userID, Signature to_check, std::string path) {
		//Vector to store distances
		float result;
		//Vector to store instant
		std::vector<float> instant_vector;

		//Sets up the RandomTree
		cv::Ptr<cv::ml::SVM> classifier = cv::ml::SVM::load(path);

		try {
			cv::Mat_<float> test_mat;
			std::vector<std::vector<float>> test_vector;

			//Pad the signature to a given size and stores data
			std::vector<std::vector<float>> signature_data = padding(to_check);
			//For every element in the signature
			for (int j = 0; j < signature_data.size(); j++) {
				//Add element to training_vector
				test_vector.push_back(signature_data.at(j));
			}

			//Copies data from training_vector to trainig_mat
			for (const auto & v : test_vector)
			{
				cv::Mat_<float> r(v), t = r.t(); // you need to do this
				test_mat.push_back(t); // because push_back(Mat_<float>(v).t()) does not work
			}

			test_mat.convertTo(test_mat, CV_32F);
			cv::Mat receivedResponses;
			result = classifier->predict(test_mat, receivedResponses); //Returns distance from plane
																		  //float distance = classifier->predict(trans_mat); //Returns decision

			float result = cv::mean(receivedResponses).val[0];
			
			result = (result + 1) / 2;
		}
		catch (cv::Exception &e) {
			std::cout << e.what() << std::endl;
		}

		return result;
	}

	bool test_signature(int userID, Signature to_check, float threshold) {
		//Loads path where Tree are stored
		std::string path = "Classifiers/SVMs/" + std::to_string(userID) + ".xml";
		//std::string path = load_model(userID);

		//Computes confidence for each instant
		float confidence = svm_connector::compute_distances(userID, to_check, path);

		return (confidence > threshold) ? true : false;
	}

	bool test_signature(int userID, Signature to_check) {
		//Get Threshold from DB
		float threshold = get_threshold(userID);
		//Call test_signature
		return svm_connector::test_signature(userID, to_check, threshold);
	}
}