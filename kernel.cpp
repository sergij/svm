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
	static double
	Kernel::dot_product(std::vector<FeatureNode> x, std::vector<FeatureNode> z) {
		double sum=0;
		int i,j;
		int x_size = x.size();
		int z_size = z.size();
		for (i=0,j=0; x_size && z_size && i < x_size && j < z_size;) {
			if (x[i].index<z[j].index) {
				i++;
			}
			else if (z[j].index<x[i].index) {
				j++;
			}
			else {
				sum+=x[i].value*z[j].value;
				i++;
				j++;
			}
		}
		return sum;
	}
	static double 
	Kernel::k_linear(std::vector<FeatureNode> x, std::vector<FeatureNode> z) {
		return dot_product(x, z);
	}
	static double
	Kernel::k_poly(std::vector<FeatureNode> x, std::vector<FeatureNode> z, double a, double b, double c) {
		if (c == 1.0)
			return a * dot_product(x, z) + b;
		return pow(a * dot_product(x, z) + b, c);
	}
	static double 
	Kernel::k_gaussian(std::vector<FeatureNode> x, std::vector<FeatureNode> z, double sigma) {
		return (-0.5 / sigma * sigma) * euclidean_dist2(x, z);
	}
	static double
	Kernel::k_tanh(std::vector<FeatureNode> x, std::vector<FeatureNode> z, double a, double b) {
		return tanh(a * dot_product(x, z) + b);
	}
}
