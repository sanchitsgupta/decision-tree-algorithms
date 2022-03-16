#include <string>

#include "id3_e2e.h"
#include "random_forest.h"
#include "ada_boost.h"

using namespace std;

int main(int argc, char* argv[])
{
	string usage_msg = "Usage: ./decision_tree <ALGORITHM>\n"
		"where ALGORITHM must be one of 'id3', 'random_forest' or 'adaboost'";

	if (argc != 2)
	{
		cout << usage_msg << endl;
		return 0;
	}

	string algo = argv[1];

	if (algo == "id3") id3E2E();
	else if (algo == "random_forest") randomForestE2E();
	else if (algo == "adaboost") adaboostE2E();
	else cout << usage_msg << endl;

	return 0;
}
