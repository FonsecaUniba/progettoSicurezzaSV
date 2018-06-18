#include "stdafx.h"
#include "NeuralNetwork.h"
#include <boost/filesystem.hpp>

#include <fstream>

namespace neuralnetwork_connector {
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

			//Sets up NeuralNetwork Classifier
			cv::Ptr<cv::ml::ANN_MLP> classifier = cv::ml::ANN_MLP::create();

			cv::Mat_<int> layers(4, 1);
			int nfeatures = training_mat.cols;
			int nclasses = 2;	//Forgery or Genuine
			layers(0) = nfeatures;     // input
			layers(1) = nclasses * 8;  // hidden
			layers(2) = nclasses * 4;  // hidden
			layers(3) = nclasses;      // output, 1 pin per class.
			classifier->setLayerSizes(layers);
			classifier->setActivationFunction(cv::ml::ANN_MLP::SIGMOID_SYM, 0, 0);
			classifier->setTermCriteria(cv::TermCriteria(cv::TermCriteria::MAX_ITER + cv::TermCriteria::EPS, 300, 0.0001));
			classifier->setTrainMethod(cv::ml::ANN_MLP::BACKPROP, 0.0001);

			//Sets up training data
			// ann requires "one-hot" encoding of class labels:
			cv::Mat train_classes = cv::Mat::zeros(training_mat.rows, nclasses, CV_32FC1);
			for (int i = 0; i<train_classes.rows; i++)
			{
				train_classes.at<float>(i, labels_mat.at<int>(i)) = 1.f;
			}
			std::cerr << training_mat.size() << " " << train_classes.size() << std::endl;

			//Trains Neural Network Classifier
			classifier->train(training_mat, cv::ml::ROW_SAMPLE, train_classes);

			//Sets NeuralNetwork save path
			std::string path = "Classifiers/NNetwork/" + std::to_string(id) + ".xml";
			//Save NeuralNetwork to file
			classifier->save(path);
			//Store NeuralNetwork path on DB Currently disabled
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
		cv::Ptr<cv::ml::ANN_MLP> classifier = cv::ml::ANN_MLP::load(path);

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
			//cv::Mat trans_mat = test_mat.t(); //1 row - 4 cols
			cv::Mat receivedResponses;
			result = classifier->predict(test_mat, receivedResponses, true); //Returns distance from plane
																	//float distance = classifier->predict(trans_mat); //Returns decision
			float result = cv::mean(receivedResponses).val[0];
			result = 1.0 / (1.0 + exp(-result));
		}
		catch (cv::Exception &e) {
			std::cout << e.what() << std::endl;
		}

		return result;
	}

	bool test_signature(int userID, Signature to_check, float threshold) {
		//Loads path where Tree are stored
		std::string path = "Classifiers/NNetwork/" + std::to_string(userID) + ".xml";
		//std::string path = load_model(userID);

		//Computes confidence for each instant
		float confidence = neuralnetwork_connector::compute_distances(userID, to_check, path);

		return (confidence > threshold) ? true : false;
	}

	bool test_signature(int userID, Signature to_check) {
		//Get Threshold from DB
		float threshold = get_threshold(userID);
		//Call test_signature
		return neuralnetwork_connector::test_signature(userID, to_check, threshold);
	}
}