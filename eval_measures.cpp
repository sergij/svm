#include <vector>
#include "problem.h"
#include "eval_measures.h"

namespace svm_learning{

    EvalMeasures::EvalMeasures(
        Problem *p,
        std::vector<int> predicted,
        int cat_num) 
	{
        this->problem = p;
        this->predicted = predicted;
        this->cat_num = cat_num;
        this->computed = 0;
    }
    double EvalMeasures::accuracy() {
        int ret = 0;
        for(int i=0; i<this->problem->l; i++) {
            if(this->problem->y[i] == this->predicted[i]) {
                ret++;
            }
        }
        return (double)ret/this->problem->l;
    }
}
