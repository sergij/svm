#ifndef SVM_EVAL
#define SVM_EVAL

#include <vector>
#include "problem.h"

namespace svm_learning{
	
	class EvalMeasures {
	private:
		std::vector<double> tp, fp, fn;
		std::vector<int> predicted;
		int cat_num;
		int computed;
	public:
		
		Problem *problem;
		EvalMeasures(Problem *p, std::vector<int>, int);
		double accuracy();
	};
}
#endif
