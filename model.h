#ifndef SVM_MODEL
#define SVM_MODEL

#include <string>
#include <vector>
#include "kernel_params.h"
#include "problem.h"

namespace svm_learning {
	
	class Model{
	public:
		int l;
		int n;
		std::vector<int> y;
		std::vector<std::vector<FeatureNode*> > x;
		std::vector<double> alpha;
		double b;
		KernelParams params;
		void load_model(std::string);
	};
}
#endif
