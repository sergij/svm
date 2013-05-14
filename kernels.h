#ifndef SVM_KERNEL
#define SVM_KERNEL

#include <vector>
#include "feature_node.h"

namespace svm_learning {
	
	class Kernel {
	public:
		static double euclidean_dist2(std::vector<FeatureNode*>, std::vector<FeatureNode*>);
		static double dot_product(std::vector<FeatureNode*>, std::vector<FeatureNode*>);
		static double k_linear(std::vector<FeatureNode*>, std::vector<FeatureNode*>);
		static double k_poly(std::vector<FeatureNode*>, std::vector<FeatureNode*>, double, double, double);
		static double k_gaussian(std::vector<FeatureNode*>, std::vector<FeatureNode*>, double);
		static double k_tanh(std::vector<FeatureNode*>, std::vector<FeatureNode*>, double, double);
	};
}
#endif
