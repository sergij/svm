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
        double ai_old, aj_old, b_old;
        double max_up, min_low;
        double b_up, b_low;
        int i_up, i_low;
        double eta;
        std::vector<int> iv1, iv2, iv3, iv4;
        std::vector<double> E;

        int psmo_take_step(int i, int j);
        int psmo_examine_example(int j);
        void smo(Problem, KernelParams);
        void smo_simple(Problem, KernelParams);
        double smo_obj(double a_j, int y_i, int y_j, double k_ij, double k_ii, double k_jj, double v_i, double v_j);
        void smo_keerthi(Problem, KernelParams);
        int take_step(int i, int j);
        void update_sets(int i, int j);
        void add_to_set(int i, double a, int y);
        void compute_up_low();
        double compute_param_L(int y_i, int y_j);
        double compute_param_H(int y_i, int y_j);
        double compute_param_bias(double a_i, double a_j, int y_i, int y_j, double kii, double k_jj, double k_ij);
        double kernel(FeatureNode [], FeatureNode[]);
        double kernel(int i, int j);

    public:
        double C;
        double tol, tol2, tol3, tol4;
        int maxpass;
        double Ei, Ej;
        Model model;

        SVM();
        int * svm_test(Problem);
        void train(Problem);
        void train(Problem, KernelParams, int);
        double svm_test_one(FeatureNode);
    };  
};
#endif
