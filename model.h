#ifndef SVM_MODEL
#define SVM_MODEL

#include <string>
#include <kernel_params>

namespace svm_learning {
	
	class Model {
	public:
		double alpha[];
		double b;
		KernelParams params;
		void save_model(std::string);
		void load_model(std::string);
	};
}
#endif
