#ifndef SVM_MODEL
#define SVM_MODEL

#include <string>
#include <vector>
#include "kernel_params.h"

namespace svm_learning {
	
	class Model {
	public:
		std::vector<double> alpha;
		double b;
		KernelParams params;
		void save_model(std::string);
		void load_model(std::string);
	};
}
#endif
