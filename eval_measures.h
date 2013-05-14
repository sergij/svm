#ifndef SVM_EVAL
#define SVM_EVAL

#include <vector>
#include "problem.h"

namespace svm_learning{
	
	class EvalMeasures {
	private:
		std::vector<int> predicted;
	public:
		
		Problem problem;
		EvalMeasures(Problem p, std::vector<int>);
		double accuracy();
	};
}
#endif
