#ifndef TREE_STRUCTURE
#define TREE_STRUCTURE

#include <vector>

struct TreeNode;
struct TreeBranch;

struct TreeNode
{
	char label;		//determines the node's attribute or '+' or '-'
	vector<TreeBranch*> branch;
	//vector<TreeNode *> branch;
	//int labelint;			//determines the branch's value
	//TreeNode *child;
};

struct TreeBranch
{
	int label;			//determines the branch's value
	TreeNode* child;	//node to which this branch points
};

struct RFTree
{
	TreeNode* root;
	vector<Attribute> continous_attributes;
	vector<unordered_map<string, int>> continous_attributes_map;
};

struct ABTree
{
	TreeNode* root;
	int index;
	float alpha;
	float accuracy;
	vector<Attribute> continous_attributes;
	vector<unordered_map<string, int>> continous_attributes_map;
};

#endif