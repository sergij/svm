#ifndef SVM_PROBLEM
#define SVM_PROBLEM

#include <string>
#include <vector>
#include "category_map.h"
#include "feature_node.h"
namespace svm_learning {
	
	class Problem {
	public:
		int l;
		int n;
		int y[];
		FeatureNode x[][];
		CategoryMap<int> cat_map;
		Problem();
		void load_binary_problem(std::string);
		vector<FeatureNode> parse_row(std::vector<std::string>);
	};
}
#endif
