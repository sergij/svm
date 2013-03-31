#ifndef SVM_FEATURE
#define SVM_FEATURE

namespace svm_learning {
	class FeatureNode {
		int index;
		double value;
	public:
		FeatureNode(int, double);
	};
}
#endif
