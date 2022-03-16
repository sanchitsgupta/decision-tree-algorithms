#ifndef UTILS
#define UTILS

#include <vector>
#include <string>
#include <iostream>
#include <fstream>

#include "tree_structure.h"

using namespace std;

//function prototypes
void resize(vector<vector<string>>& data, ifstream& datafile);
vector<vector<string>> loadData(string filepath);
void fillContAttrMaps(vector<Attribute>& cont_attr, vector<vector<string>>& data);
void printTree(TreeNode* node);

void saveTree(TreeNode *root, string filename);
void storeTreeRecursive(TreeNode *node, fstream& trainedfile);

//resizing data and test file acording to inputted data.txt and test.txt
void resize(vector<vector<string>>& data, ifstream& datafile)
{
	string line;
	int size_data = 0;						//size of data (number of instances)
	if (datafile.is_open())
	{
		while (getline(datafile, line))
		{
			size_data++;
		}
		datafile.close();
	}
	else
	{
		cout << "Unable to open data file!" << "\n";
	}
	data.resize(size_data);
	for (int i = 0;i < data.size();i++)
	{
		data[i].resize(15);
	}
}

vector<vector<string>> loadData(string filepath)
{
	ifstream datafile(filepath);
	vector<vector<string>> data;

	//resize data vector
	resize(data, datafile);

    string line;

    datafile.open(filepath);
	if (datafile.is_open())
	{
		int i = 0;
		while (getline(datafile, line))
		{
			for (int j = 0, k = 0; j < line.length(); k++, j+=2)
			{
				while (line[j] != ',' && j < line.length())
				{
					data[i][k].push_back(line[j]);
					j++;
				}
			}
			i++;
		}
		datafile.close();
	}
	else
	{
		cout << "Unable to open file " << filepath << "\n";
	}
	return data;
}

//filling maps of each continous attribute according to test/data set
void fillContAttrMaps(vector<Attribute>& cont_attr, vector<vector<string>>& data)
{
	for (int i = 0; i < cont_attr.size(); i++)
	{
		float t = cont_attr[i].threshold;
		for (int j = 0; j < data.size(); j++)
		{
			if (stoi(data[j][cont_attr[i].index]) <= t)
			{
				cont_attr_map[i][(data[j][cont_attr[i].index])] = 1;
			}
			else if (stoi(data[j][cont_attr[i].index]) > t)
			{
				cont_attr_map[i][(data[j][cont_attr[i].index])] = 2;
			}
		}
		cont_attr[i].map = cont_attr_map[i];
	}
}

void printTree(TreeNode* node)
{
	if (!node->branch.empty())
	{
		cout << "N" << node->label << " ";
		for (int i = 0; i < (node->branch).size(); i++)
		{
			cout << node->label << "B" << (node->branch[i])->label;
			printTree((node->branch[i])->child);
		}
	}
	else
	{
		cout << node->label << " ";
	}
}

// NOTE: Below functions are untested

//store decision tree to a txt file
void saveTree(TreeNode *root, string filepath)
{
	fstream trainedfile;
	trainedfile.open(filepath, ios::out);
	if (trainedfile.is_open())
	{
		storeTreeRecursive(root, trainedfile);
		trainedfile.close();
	}
	else
	{
		cout << "Unable to open " << filepath << "\n";
	}
}

void storeTreeRecursive(TreeNode *node, fstream& trainedfile)
{
	if (!node->branch.empty())
	{
		trainedfile << "N" << node->label << " ";
		for (int i = 0; i < (node->branch).size(); i++)
		{
			trainedfile << node->label << "B" << (node->branch[i])->label;
			storeTreeRecursive((node->branch[i])->child, trainedfile);
		}
	}
	else
	{
		trainedfile << node->label << " ";
	}
}

void loadTree(string filepath)
{
	string tree;
	fstream trainedfile;
	trainedfile.open(filepath, ios::in);

	if (trainedfile.is_open())
	{
		char c;
		while (trainedfile.get(c))
		{
			tree.push_back(c);
		}
		trainedfile.close();
	}
	else
	{
		cout << "Unable to open " << filepath << "\n";
	}
}

#endif
