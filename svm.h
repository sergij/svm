#ifndef SVM_L
#define SVM_L

#define n_threads 4

#include <vector>
#include "model.h"
#include "kernel_params.h"
#include "feature_node.h"
#include "problem.h"

#include <tbb/blocked_range.h>
#include <tbb/parallel_for.h>
#include <tbb/parallel_reduce.h>
#include <tbb/task_scheduler_init.h>
#include <tbb/tick_count.h>

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

        double test_one(std::vector<FeatureNode*>& x);

        double svm_test_one(std::vector<FeatureNode*>& x);
    
        struct TestReducer {
                double value;
                SVM* w_;
                std::vector<FeatureNode*>& v;
                TestReducer(SVM* w, std::vector<FeatureNode*>& x) : value(0.0), w_(w), v(x) {}
                TestReducer(TestReducer& s, tbb::split):
                    value(0.0),
                    w_(s.w_), v(s.v) {}
    
            void operator() (const tbb::blocked_range<size_t>& r) {
                double temp = value;
                const size_t end = r.end();
                for (size_t i = r.begin(); i!=end; ++i) {
                    temp += (w_->model.alpha[i] * w_->model.y[i] * w_->kernel(v, w_->model.x[i]));
                }
                value = temp;
            }
            void join(TestReducer& rhs) {
                value += rhs.value;
            }
        };

        struct ParallelVectorize{
            SVM* svm;
            Model* model;
            ParallelVectorize(SVM* s, Model* m): svm(s), model(m) {}
            void operator() (const tbb::blocked_range<size_t>& r) const {
                const size_t end = r.end();
                for(size_t i = r.begin(); i!=end; ++i) {
                    svm->E[i] = svm->svm_test_one(model->x[i]) - model->y[i];
                }
            }
        };

    public:
        Model model;

        SVM();
        std::vector<int> test(Problem);
        void train(Problem);
    };  
}
#endif
