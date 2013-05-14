#ifndef SVM_L
#define SVM_L

#include <vector>
#include "model.h"
#include "kernel_params.h"
#include "feature_node.h"
#include "problem.h"

namespace svm_learning {
    
class SVM {
    private:
        double C;
        double tol, tol2;
        double Ei, Ej;

        double ai_old, aj_old, b_old;
        double eta;
		double L, H;

        std::vector<double> E;

        int psmo_take_step(int i, int j);
        int psmo_examine_example(int j);
        void smo(Problem, KernelParams);
        double smo_obj(double, int, int, double, double, double, double, double);

        double compute_param_L(int, int);
        double compute_param_H(int, int);
        void compute_param_bias(double, double, int, int, double, double, double);
        double kernel(std::vector<FeatureNode*>, std::vector<FeatureNode*>);
        double kernel(int i, int j);

        double svm_test_one(std::vector<FeatureNode*>);

    public:
        Model model;

        SVM();
        std::vector<int> test(Problem);
        void train(Problem);
    };  
}
#endif
