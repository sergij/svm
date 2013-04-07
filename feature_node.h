#ifndef SVM_FEATURE
#define SVM_FEATURE

namespace svm_learning {
	class FeatureNode {
	public:
		FeatureNode(int, double);
		int index;
		double value;
	};
}
#endif
