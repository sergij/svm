#include <vector>
#include "feature_node.h"
#include "kernel.h"

namespace svm_learning {
	
	double
	Kernel::euclidean_dist2(
		std::vector<FeatureNode> x, std::vector<FeatureNode> z
	) {
		double sum=0;
		int i,j;
		size_t x_size = x.size();
		size_t z_size = z.size();
		for (i=0, j=0; i < x_size && j < z_size;) {
			if (x[i].index < z[j].index) {
				sum += x[i].value * x[i].value;
				i++;
			}
			else if (z[j].index<x[i].index) {
				sum += z[j].value * z[j].value;
				j++;
			}
			else {
				sum+=(x[i].value-z[j].value)*(x[i].value-z[j].value);
				i++;
				j++;
			}
		}
		for (;x_size && i < x_size; i++) {
			sum+=x[i].value*x[i].value;
		}
		for (;z_size && j<z_size; j++) {
			sum += z[j].value * z[j].value;
		}
		return sum;
	}
		static double dot_product(FeatureNode [], FeatureNode[]);
		static double k_linear(FeatureNode [], FeatureNode []);
		static double k_poly(FeatureNode [], FeatureNode[], double, double, double);
		static double k_gaussian(FeatureNode [], FeatureNode[], double);
		static double k_tanh(FeatureNode[], FeatureNode[], double, double);
}
