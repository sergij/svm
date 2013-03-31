#ifndef SVM_KERNEL
#define SVM_KERNEL

#include "feature_node"

namespace svm_learning {
	
	class Kernel {
	public:
		static double euclidean_dist2(FeatureNode [], FeatureNode[]);
		static double dot_product(FeatureNode [], FeatureNode[]);
		static double k_linear(FeatureNode [], FeatureNode []);
		static double k_poly(FeatureNode [], FeatureNode[], double, double, double);
		static double k_gaussian(FeatureNode [], FeatureNode[], double);
		static double k_tanh(FeatureNode[], FeatureNode[], double, double);
	};
}
#endif
