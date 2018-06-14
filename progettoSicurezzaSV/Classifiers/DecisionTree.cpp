#include "stdafx.h"
#include "DecisionTree.h"
#include <boost/filesystem.hpp>

#include <fstream>

namespace decision_tree_connector {
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

	void train_svm(User to_train) {

		//Stores User ID
		int id = to_train.get_id();

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

			//Sets up DecisionTree Classifier
			cv::Ptr<cv::ml::DTrees> tree = cv::ml::DTrees::create();
			//Trains DecisionTree Classifier
			tree->train(td);

			//Sets DecisionTree save path
			std::string path = "Classifiers/Trees/" + std::to_string(id) + ".xml";
			if (boost::filesystem::exists(path)) {
				boost::filesystem::path dir(path);
				if (boost::filesystem::create_directory(dir))
				{
					std::cerr << "Directory Created" << std::endl;
				}
			}
			//Save Tree to file
			tree->save(path);
			//Store Tree path on DB Currently disabled
			//save_model(id, path);
		}
		catch (cv::Exception &e) {
			std::cout << e.what() << std::endl;
		}
	}

	std::vector<float> compute_distances(int userID, Signature to_check, std::string path) {
		//Vector to store distances
		std::vector<float> results;
		//Vector to store instant
		std::vector<float> instant_vector;

		//Sets up the Decision Tree
		cv::Ptr<cv::ml::DTrees> tree = cv::ml::DTrees::load(path);

		try {

			//Pad Signature to given length
			std::vector<std::vector<float>> signature_data = padding(to_check);
			//For each element in the signature
			for (int i = 0; i < signature_data.size(); i++) {
				//Creates Mat to test
				cv::Mat test_mat(signature_data.at(i), true); //4 rows - 1 col
				test_mat.convertTo(test_mat, CV_32F);
				cv::Mat trans_mat = test_mat.t(); //1 row - 4 cols

												  //Predict distance
				float distance = tree->predict(trans_mat, cv::noArray(), true); //Returns distance from plane
																			   //float distance = tree->predict(trans_mat); //Returns decision

																			   //Adds Distance to results vector
				results.push_back(distance);
			}
		}
		catch (cv::Exception &e) {
			std::cout << e.what() << std::endl;
		}

		return results;
	}

	bool test_signature(int userID, Signature to_check, float threshold) {
		//Loads path where Tree are stored
		std::string path = "Trees/" + std::to_string(userID) + ".xml";
		//std::string path = load_model(userID);

		//Computes distance for each instant
		std::vector<float> distances = decision_tree_connector::compute_distances(userID, to_check, path);

		int accepted_count = 0;
		int rejected_count = 0;

		/*
		Determine if Signature is accepted by checking
		if distance from hyperplane for each point is
		greater than threshold
		*/
		for (int i = 0; i < distances.size(); i++) {
			double confidence = 1.0 / (1.0 + exp(-distances.at(i)));

			//Instant is accepted 
			if (confidence > threshold) {
				accepted_count++;
				//Possible plotting
			}
			//Instant is rejected
			else {
				rejected_count++;
				//Possible plotting
			}
		}

		//Return decision based on majority
		return (accepted_count > rejected_count) ? true : false;
	}

	bool test_signature(int userID, Signature to_check) {
		//Get Threshold from DB
		float threshold = get_threshold(userID);
		//Call test_signature
		return decision_tree_connector::test_signature(userID, to_check, threshold);
	}
}