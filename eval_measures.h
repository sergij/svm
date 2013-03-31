#ifndef SVM_EVAL
#define SVM_EVAL

#include "problem.h"
namespace {
	
	class EvalMeasures {
	private:
		double tp[], fp[], fn[];
		Problem problem;
		int predicted[];
		int cat_num;
		int computed;
	public:
		EvalMeasures(Problem p, int[], int);
		double accuracy();
	};
}
#endif
