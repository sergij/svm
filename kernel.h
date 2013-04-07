#ifndef SVM_KERNEL
#define SVM_KERNEL

#include <vector>
#include "feature_node.h"

namespace svm_learning {
	
	class Kernel {
	public:
		static double euclidean_dist2(std::vector<FeatureNode>, std::vector<FeatureNode>);
		static double dot_product(FeatureNode [], FeatureNode[]);
		static double k_linear(FeatureNode [], FeatureNode []);
		static double k_poly(FeatureNode [], FeatureNode[], double, double, double);
		static double k_gaussian(FeatureNode [], FeatureNode[], double);
		static double k_tanh(FeatureNode[], FeatureNode[], double, double);
	};
}
#endif
