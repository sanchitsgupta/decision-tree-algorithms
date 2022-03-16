#ifndef ID3_E2E
#define ID3_E2E

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>			//std::distance
#include <ctime>

#include "attribute_maps.h"
#include "attributes_structure.h"
#include "attributes.h"
#include "tree_structure.h"

#include "threshold.h"			//for calculating the threshold of continous attributes
#include "replace_unknowns.h"	//for replacing unknowns in the set
#include "perform_id3.h"		//to perform id3
#include "classify.h"			//to classify the test set
#include "utils.h"				//general utils such as to load data

using namespace std;

// perform id3 end-to-end including both training and testing
void id3E2E()
{
	clock_t begin = clock();
	string training_filepath = "datafiles/data.txt";
	string test_filepath = "testfiles/test.txt";

	//load training data
	vector<vector<string>> data = loadData(training_filepath);
	cout << "Dataset size : "<< data.size() << "\n\n";

	//vector containing continous attributes
	vector<Attribute> cont_attr = { age, fnlwgt, education_num, capital_gain, capital_loss, hours_per_week };
	cont_attr_map.resize(cont_attr.size());

	//assigning threshold to each continous attributes
	clock_t t1 = clock();
	cout << "Finding Thresholds...\n";
	for (int i = 0; i < cont_attr.size(); i++)
	{
		cont_attr[i].threshold = threshold(data, cont_attr[i]);
		cout << "Threshold of continous attribute " << i + 1 << " : " << cont_attr[i].threshold << "\n";
	}
	cout << "\nAll thresholds founded in " << double(clock() - t1) / CLOCKS_PER_SEC << " secs.\n\n";

	//assigning map to each continous attribute
	fillContAttrMaps(cont_attr, data);
	age.map = cont_attr[0].map;
	fnlwgt.map = cont_attr[1].map;
	education_num.map = cont_attr[2].map;
	capital_gain.map = cont_attr[3].map;
	capital_loss.map = cont_attr[4].map;
	hours_per_week.map = cont_attr[5].map;

	//replace unknown values in data set
	vector<Attribute> attributes = {
		age, workclass, fnlwgt, education, education_num,
		marital_status, occupation, relationship, race, sex,
		capital_gain, capital_loss, hours_per_week, native_country
	};
	replaceUnknowns(data, attributes);

	//perform id3 and form decision tree
	clock_t t3 = clock();
	cout << "Training started..." << "\n";
	TreeNode *root = id3(data, attributes);
	cout << "\nTraining completed in " << double(clock() - t3) / CLOCKS_PER_SEC << " secs.\n\n";

	//testing
	cout << string(50, '-') << "\n";
	vector<vector<string>> test = loadData("testfiles/test.txt");
	//add one more column to test vector for accomodating the output predicted by our learned decision tree
	for (int i = 0; i < test.size(); i++)
	{
		test[i].resize(16);
	}
	cout << "\nTestset size : " << test.size() << "\n";

	//modify attributes according to test set
	//first fill map of each continous attribute with values that these attributes take in test set
	fillContAttrMaps(cont_attr, test);
	age.map = cont_attr[0].map;
	fnlwgt.map = cont_attr[1].map;
	education_num.map = cont_attr[2].map;
	capital_gain.map = cont_attr[3].map;
	capital_loss.map = cont_attr[4].map;
	hours_per_week.map = cont_attr[5].map;

	vector<Attribute> attributes_modified = {
		age, workclass, fnlwgt, education, education_num, marital_status,
		occupation, relationship, race, sex, capital_gain,
		capital_loss, hours_per_week, native_country
	};
	replaceUnknowns(test, attributes);

	cout << "\nTesting started...\n\n";
	clock_t t5 = clock();
	classifyInstances(root, attributes_modified, test);
	clock_t end = clock();

	cout << "Testing done in " << double(end - t5) / CLOCKS_PER_SEC << " secs.\n\n";
	cout << "Total running time : " << double(end - begin) / CLOCKS_PER_SEC << " secs.\n\n";
}

#endif
