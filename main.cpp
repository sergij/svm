#include <iostream>
#include <vector>
#include <ctime>

#include "problem.h"
#include "svm.h"
#include "eval_measures.h"

#include <tbb/tick_count.h>


using namespace svm_learning;


int main (int argc, char *argv[]) {
    
    if (argc < 3) {
        std::cerr << "Error while starting: no training or test file given";
        return 0;
    }
    tbb::task_scheduler_init init(3);

    clock_t start, end;
    SVM s;
    Problem train;
    Problem test;

    start = clock();
    train.load_data(argv[1]);
    test.load_data(argv[2]);
    end = clock();
    std::cout << "Time taken for loading data: " << ((double)(end-start)/(double)CLOCKS_PER_SEC)<< "s\n";

    tbb::tick_count time_s, time_e;
    time_s = tbb::tick_count::now();
    s.train(train);
    time_e = tbb::tick_count::now();
    /*Time consuming code*/
    std::cout << "Time taken for learning: " << (double) (time_e - time_s).seconds() << "s\n";

    start = clock();
    std::vector<int> r = s.test(test);
    end = clock();
    /*Time consuming code*/
    std::cout << "Time taken for recognition: " << ((double)(end-start)/(double)CLOCKS_PER_SEC)<< "s\n";

    EvalMeasures e(test, r);
    std::cout << e.accuracy() << std::endl;
    return 0;
}
