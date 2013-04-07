#ifndef SVM_PROBLEM
#define SVM_PROBLEM

#include <string>
#include <vector>
#include "category_map.h"
#include "feature_node.h"

namespace svm_learning {
	
	class Problem {
	public:
		Problem();
		int l;
		int n;
		std::vector<int> y;
		std::vector<std::vector<FeatureNode> > x;
		CategoryMap<int> cat_map;
		void load_binary_problem(std::string);
		std::vector<FeatureNode*> parse_row(std::vector<std::string>);
	};
}
#endif
