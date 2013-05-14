#include <string>
#include <vector>
#include <fstream>

#include <algorithm>
#include "category_map.h"
#include "feature_node.h"
#include "problem.h"

namespace svm_learning {
	Problem::Problem() {
		l = 0;
		n = 0;
	}
    void Problem::load_data(char* file_name) {
		std::vector<std::string> vs;
		std::vector<int> classes;
		std::vector<std::vector<FeatureNode*> > examples;
		std::ifstream in_file(file_name);
		std::string line;
		int cat;
		char ch = '\0';
		while(in_file >> cat) {
			while(ch != '\n') {
				in_file >> line;
				vs.push_back(line);
				in_file.get(ch);
			}
			ch = '\0';
			x.push_back(parse_row(vs));
			vs.clear();
			cat_map.add_category(cat);
			y.push_back(((cat_map.get_new_category(cat)-1)<<1)-1);
		}
		l = x.size();
    }
    std::vector<FeatureNode*> Problem::parse_row(std::vector<std::string> row) {
        std::vector<FeatureNode*> example = std::vector<FeatureNode*>(row.size());
		std::string tmp;
        int max_index = 0;
		int index;
		double value;
        for (int i=0; i<(int)row.size(); i++) {
			tmp = row[i];
			int semi_index = tmp.find(':');

			index = atoi(tmp.substr(0, semi_index).c_str());
			value = atof(tmp.substr(semi_index + 1, row[i].length()).c_str());

			max_index = std::max(index, max_index);
            example[i] = new FeatureNode(index, value);
        }
        if (this->n < max_index)
            this->n = max_index;
        return example;
    }
}
