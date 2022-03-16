# Decision Tree Algorithms in C++

![C++](https://img.shields.io/badge/C%2B%2B-v14-blue.svg?style=flat&logo=c%2B%2B)
![Linux](https://svgshare.com/i/Zhy.svg)
![PyPI license](https://img.shields.io/github/license/sanchitsgupta/decision_tree)

Implementation of three decision tree algorithms in C++. Namely [id3](https://en.wikipedia.org/wiki/ID3_algorithm), [RandomForest](https://en.wikipedia.org/wiki/Random_forest) and [AdaBoost(with id3)](https://en.wikipedia.org/wiki/AdaBoost).

This implementation can handle continous and missing value attributes.

## Running
------
1. Compile the code
    ```shell
    $ g++ -w -o decision_tree decision_tree.cpp
    ```
2. Run any one of the algorithms
    ```shell
    $ ./decision_tree <ALGORITHM>
    ```
    where ALGORITHM can be one of `id3`, `random_forest`, or `adaboost`.

## Dataset Info
------
[Adult Dataset](http://archive.ics.uci.edu/ml/datasets/Adult) has been used.

I split the data beforehand. [datafiles/data.txt](./datafiles/data.txt) contains the training instances, and
[testfiles/test.txt](./testfiles/test.txt) contains the testing instances.

> NOTE: Code was written specifically for this dataset and will require significant changes before running on some other dataset.

## Results
------

| Algorithm     | Accuracy (%)  | Runtime  |
| ------------- |:-------------:|:--------:|
| id3           | 80.5356       | ~1min    |
| RandomForest  | **82.4335**       | ~3mins   |
| AdaBoost      | 81.3218       | ~3mins   |

## Additional Notes
------
I did this project to get a better understanding of the said algorithms, and thus, performance optimizations were not a priority. Coming back to the code after several years, I can see that there are several oppurtunities for improvement.
