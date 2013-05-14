#include <iostream>
#include <vector>
#include <ctime>
#include "problem.h"
#include "svm.h"
#include "eval_measures.h"

using namespace svm_learning;
int main (int argc, char *argv[]) {
    if (argc < 3) {
        std::cerr << "Error while starting: no training or test file given";
        return 0;
    }
    SVM s;
    Problem train;
    train.load_data(argv[1]);
    clock_t start = clock();
    s.train(train);
    clock_t end = clock();
    /*Time consuming code*/
    std::cout << "Time taken for learning: " << ((double)(end-start)/(double)CLOCKS_PER_SEC)<< "s\n";

    Problem test;
    test.load_data(argv[2]);
    start = clock();
    std::vector<int> r = s.test(test);
    end = clock();
    /*Time consuming code*/
    std::cout << "Time taken for recognition: " << ((double)(end-start)/(double)CLOCKS_PER_SEC)<< "s\n";

    EvalMeasures e(test, r);
    std::cout << e.accuracy() << std::endl;
    return 0;
}
