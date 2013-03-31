#include <string>
#include <vector>
#include "category_map.h"
#include "feature_node.h"
namespace svm_learning {
    
    void Problem::load_binary_problem(std::string) {
        return;
    }
    vector<FeatureNode> Problem::parse_row(std::vector<std::string> row) {
        std::vector<FeatureNode> example = std::vector<FeatureNode>(row.size()-1);
        int max_index = 0;
        for (int i=1; i<row.size(); i++) {
            std::vector<std::string> iv;
            /*
            feel iv iv = row[i].split(":")
            */
            int index = atoi(iv[0]);
            max_index = index;
            double value = atof(iv[1]);
            example[i - 1] = new FeatureNode(index, value);
        }
        if (this.n < max_index)
            this.n = max_index;
        return example;
    }
}
