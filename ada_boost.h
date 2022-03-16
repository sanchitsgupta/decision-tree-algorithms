#ifndef ADABOOST
#define ADABOOST

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>	//std::distance
#include <ctime>

#include "attribute_maps.h"
#include "attributes_structure.h"
#include "tree_structure.h"
#include "attributes.h"

#include "threshold.h"			//for calculating the threshold of continous attributes
#include "replace_unknowns.h"	//for replacing unknowns in the set
#include "perform_id3.h"		//to perform id3
#include "classify.h"			//to classify the data/test set
#include "utils.h"				//general utils such as to load data

//this structure helps in selecting instances according to their weights
struct Data
{
	vector<string> training_example;
	float weight;
};

vector<float> weights;

//function prototypes
void adaboostE2E();
bool sortByWeight(Data lhs, Data rhs);
vector<vector<string> > dataByWeights(vector<vector<string>>& data, vector<float> weights);
TreeNode * formTree(vector<vector<string>>& data, vector<Attribute> attributes, ABTree& Tree);
void fillContAttrMaps(vector<Attribute>& cont_attr, vector<vector<string>>& test, ABTree& Tree);
float getABAccuracy(vector<vector<string>>& test, vector<ABTree>& Trees);

void adaboostE2E()
{
	clock_t begin = clock();

	string training_filepath = "datafiles/data.txt";
	string test_filepath = "testfiles/test.txt";
	int num_trees = 10;

	//load training data
	vector<vector<string>> data = loadData(training_filepath);
	cout << "Dataset size : " << data.size() << "\n\n";
	for (int i = 0; i < data.size(); i++)
	{
		data[i].resize(data[i].size() + num_trees);
	}

	vector<Attribute> attributes = {
		age, workclass, fnlwgt, education, education_num, marital_status,
		occupation, relationship, race, sex, capital_gain,
		capital_loss, hours_per_week, native_country
	};

	replaceUnknowns(data, attributes);		//replace unknowns in data file

	//initialize weights
	float n = data.size();					//n is the total number of instances
	weights.resize(n);
	for (int i = 0; i < weights.size(); i++)
	{
		weights[i] = (1 / n);
	}
	cout << "Weights Initialized !!\n";

	//train num_trees decision trees
	clock_t t1 = clock();
	// srand(time(NULL));
	vector<ABTree> Trees(num_trees);

	for (int i = 0; i < Trees.size(); i++)
	{
		if (i > 0)
		{
			if (Trees[i-1].accuracy > 95.0)
			{
				break;
			}
		}
		vector<Attribute> four_attributes;
		vector<Attribute> random_attr = attributes;
		random_shuffle(random_attr.begin(), random_attr.end());
		for (int i = 0; i < 4; i++)
		{
			four_attributes.push_back(random_attr[i]);
		}
		cout << "\n--------------------------- TREE " << i + 1 << " ----------------------------\n\n";
		for (int i = 0; i < 4; i++)
		{
			cout << "Attribute " << four_attributes[i].index << ", ";
		}
		cout << "\n\n";
		Trees[i].root = formTree(data, four_attributes, Trees[i]);

		//test on same dataset to find h(x) and alpha of each tree
		vector<Attribute> cont_attr = { age, fnlwgt, education_num, capital_gain, capital_loss, hours_per_week };

		fillContAttrMaps(Trees[i].continous_attributes, data, Trees[i]); //fill maps of continous attributes of tree

		for (int j = 0; j < cont_attr.size(); j++)
		{
			for (int k = 0; k < Trees[i].continous_attributes.size(); k++)
			{
				if (cont_attr[j].index == Trees[i].continous_attributes[k].index)
				{
					cont_attr[j].map = Trees[i].continous_attributes_map[k];
				}
			}
		}

		age.map = cont_attr[0].map;
		fnlwgt.map = cont_attr[1].map;
		education_num.map = cont_attr[2].map;
		capital_gain.map = cont_attr[3].map;
		capital_loss.map = cont_attr[4].map;
		hours_per_week.map = cont_attr[5].map;

		vector<Attribute> attributes_modified = { age , workclass, fnlwgt, education, education_num, marital_status, occupation, relationship, race, sex, capital_gain, capital_loss, hours_per_week, native_country };

		Trees[i].index = i;
		classifyInstances(Trees[i].root, attributes_modified, data, Trees[i]);

		//data[i][15 + Tree.index] contains the h(x) of Tree with index = Tree.index = i
		//update weights vector

		for (int j = 0; j < weights.size(); j++)
		{
			weights[j] = (weights[j] * exp((-Trees[i].alpha) * output_map[data[j][14]] * output_map[data[j][15 + Trees[i].index]]));
		}
		float sum = 0;
		for (int j = 0; j < weights.size(); j++)
		{
			sum = sum + weights[j];
		}
		for (int j = 0; j < weights.size(); j++)
		{
			weights[j] = weights[j] / sum;
		}
		if (Trees[i].accuracy < 95)
		{
			cout << "\nWeights Updated !!\n";
		}

	}
	double generaterf_secs = double(clock() - t1) / CLOCKS_PER_SEC;
	cout << string(51, '-') << "\n" << "Random Forest with weights generated in " << generaterf_secs << " secs.\n";
	cout << string(51, '-') << "\n";

	//testing
	vector<vector<string>> test = loadData("testfiles/test.txt");
	for (int i = 0;i < test.size();i++)
	{
		test[i].resize(15 + Trees.size());
	}
	cout << "\nTestset size : " << test.size() << "\n\n";

	replaceUnknowns(test, attributes);						//replace unknowns in test file

	//now classify
	cout << "Testing started...\n\n";
	clock_t t3 = clock();

	for (int i = 0; i < Trees.size(); i++)
	{
		//modify attributes according to test set
		//first fill map of each continous attribute that was present when forming this Tree, with values that this attribute takes in the test set
		vector<Attribute> cont_attr = { age, fnlwgt, education_num, capital_gain, capital_loss, hours_per_week };

		fillContAttrMaps(Trees[i].continous_attributes, test, Trees[i]); //fill maps of continous attributes of tree;

		for (int j = 0; j < cont_attr.size(); j++)
		{
			for (int k = 0; k < Trees[i].continous_attributes.size(); k++)
			{
				if (cont_attr[j].index == Trees[i].continous_attributes[k].index)
				{
					cont_attr[j].map = Trees[i].continous_attributes_map[k];
				}
			}
		}

		age.map = cont_attr[0].map;
		fnlwgt.map = cont_attr[1].map;
		education_num.map = cont_attr[2].map;
		capital_gain.map = cont_attr[3].map;
		capital_loss.map = cont_attr[4].map;
		hours_per_week.map = cont_attr[5].map;

		vector<Attribute> attributes_modified = { age , workclass, fnlwgt, education, education_num, marital_status, occupation, relationship, race, sex, capital_gain, capital_loss, hours_per_week, native_country };
		classifyInstances(Trees[i].root, attributes_modified, test, Trees[i].index);
	}

	float accuracy = getABAccuracy(test, Trees);
	cout << "\nAccuracy of Classifier with AdaBoost : " << accuracy << "%\n";

	clock_t end = clock();
	double classify_secs = double(end - t3) / CLOCKS_PER_SEC;
	double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
	cout << "\nTesting done in " << classify_secs << " secs." << "\n" << string(43, '-') << "\n";
	cout << "\nTotal running time : " << elapsed_secs << " secs.\n\n";
}

bool sortByWeight(Data lhs, Data rhs)
{
	return lhs.weight > rhs.weight;
}

vector<vector<string> > dataByWeights(vector<vector<string>>& data, vector<float> weights)
{
	vector<vector<string>> data_by_weights;
	vector<Data> weights_data;
	weights_data.resize(data.size());
	for (int i = 0; i < data.size(); i++)
	{
		weights_data[i].training_example = data[i];
		weights_data[i].weight = weights[i];
	}
	sort(weights_data.begin(), weights_data.end(), sortByWeight);
	//train on 60% of data
	int n = 0.6*data.size();
	for (int i = 0; i < n; i++)
	{
		data_by_weights.push_back(weights_data[i].training_example);
	}
	return data_by_weights;
}

//form decision tree on randomly selected instances and considering only 4 randomly selected attributes
TreeNode * formTree(vector<vector<string>>& data, vector<Attribute> attributes, ABTree& Tree)
{
	TreeNode *root = new TreeNode;
	vector<vector<string>> data_by_weights;
	vector<Attribute> cont_attr;
	for (int i = 0; i < attributes.size(); i++)
	{
		if (attributes[i].index == 0 || attributes[i].index == 2 || attributes[i].index == 4 || attributes[i].index == 10 || attributes[i].index == 11 || attributes[i].index == 12)
		{
			cont_attr.push_back(attributes[i]);
		}
	}
	cont_attr_map.resize(cont_attr.size());

	//select taining data in such a way that higher weights have more probability of showing in the training data
	data_by_weights = dataByWeights(data, weights);
	clock_t t1 = clock();
	cout << "Finding Thresholds...\n";
	for (int i = 0; i < cont_attr.size(); i++)
	{
		cont_attr[i].threshold = threshold(data, cont_attr[i]);
		cout << "Threshold of continous attribute " << cont_attr[i].index << " : " << cont_attr[i].threshold << "\n";
	}
	cout << "\nAll thresholds founded in " << double(clock() - t1) / CLOCKS_PER_SEC << " secs.\n\n";

	Tree.continous_attributes = cont_attr;
	fillContAttrMaps(cont_attr, data_by_weights);
	Tree.continous_attributes_map = cont_attr_map;
	for (int i = 0; i < attributes.size(); i++)
	{
		for (int j = 0; j < cont_attr.size(); j++)
		{
			if (attributes[i].index == cont_attr[j].index)
			{
				attributes[i].map = cont_attr[j].map;
			}
		}
	}

	clock_t t3 = clock();
	cout << "Training started...\n";

	root = id3(data_by_weights, attributes);
	for (int i = 0; i < cont_attr_map.size(); i++)
	{
		cont_attr_map[i].clear();
	}
	clock_t t4 = clock();
	double perform_id3_secs = double(t4 - t3) / CLOCKS_PER_SEC;
	cout << "\n" << "Training completed in " << perform_id3_secs << " secs.\n\n";
	return root;
}

void fillContAttrMaps(vector<Attribute>& cont_attr, vector<vector<string>>& test, ABTree& Tree)
{
	for (int i = 0; i < cont_attr.size(); i++)
	{
		float t = cont_attr[i].threshold;
		for (int j = 0; j < test.size(); j++)
		{
			if (stoi(test[j][cont_attr[i].index]) <= t)
			{
				Tree.continous_attributes_map[i][(test[j][cont_attr[i].index])] = 1;
			}
			else if (stoi(test[j][cont_attr[i].index]) > t)
			{
				Tree.continous_attributes_map[i][(test[j][cont_attr[i].index])] = 2;
			}
		}
		cont_attr[i].map = Tree.continous_attributes_map[i];
	}
}

float getABAccuracy(vector<vector<string>>& test, vector<ABTree>& Trees)
{
	for (int i = 0; i < test.size(); i++)
	{
		test[i].resize(test[i].size() + 1);
	}

	for (int i = 0; i < test.size(); i++)
	{
		float H_x = 0;
		for (int j = 15; j < test[i].size() - 1; j++)
		{
			H_x = H_x + (Trees[j-15].alpha*output_map[test[i][j]]);
		}
		if (H_x > 0)
		{
			test[i][test[i].size() - 1] = ">50K";
		}
		else if (H_x < 0)
		{
			test[i][test[i].size() - 1] = "<=50K";
		}
	}

	float p = 0;
	for (int i = 0; i < test.size(); i++)
	{
		if (output_map[test[i][14]] == output_map[test[i][test[i].size() - 1]])
		{
			p++;
		}
	}
	return (p / test.size()) * 100.0;
}

#endif
