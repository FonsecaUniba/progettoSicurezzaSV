#include "stdafx.h"
#include "GradientBoostedTree.h"
#include <boost/filesystem.hpp>

#include <fstream>

namespace gradientboostedtree_connector {
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

			//Converts the two mats to the type supported by the DecisionTree trainer
			labels_mat.convertTo(labels_mat, CV_32S);
			training_mat.convertTo(training_mat, CV_32F);
			//Declares Training Data using training_mat and labels_mat
			cv::Ptr<cv::ml::TrainData> td = cv::ml::TrainData::create(training_mat, cv::ml::ROW_SAMPLE, labels_mat);

			//Sets up GradientBoostedTree Classifier
			cv::Ptr<cv::ml::Boost> tree = cv::ml::Boost::create();
			//Trains GradientBoostedTree Classifier
			tree->train(td);

			//Sets RandomTree save path
			std::string path = "Classifiers/GBTrees/" + std::to_string(to_train.id) + ".xml";
			//Save Tree to file
			tree->save(path);
			//Store Tree path on DB Currently disabled
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
		cv::Ptr<cv::ml::Boost> tree = cv::ml::Boost::load(path);

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
			cv::Mat trans_mat = test_mat.t(); //1 row - 4 cols
			cv::Mat receivedResponses;
			result = tree->predict(trans_mat, receivedResponses, true); //Returns distance from plane
																	//float distance = tree->predict(trans_mat); //Returns decision
			float result = cv::mean(receivedResponses).val[0];
		}
		catch (cv::Exception &e) {
			std::cout << e.what() << std::endl;
		}

		return result;
	}

	bool test_signature(int userID, Signature to_check, float threshold) {
		//Loads path where Tree are stored
		std::string path = "Classifiers/GBTrees/" + std::to_string(userID) + ".xml";
		//std::string path = load_model(userID);

		//Computes confidence for each instant
		float confidence = gradientboostedtree_connector::compute_distances(userID, to_check, path);

		return (confidence > threshold) ? true : false;
	}

	bool test_signature(int userID, Signature to_check) {
		//Get Threshold from DB
		float threshold = get_threshold(userID);
		//Call test_signature
		return gradientboostedtree_connector::test_signature(userID, to_check, threshold);
	}
}