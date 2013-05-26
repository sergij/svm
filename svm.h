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

        double svm_tester(int i, std::vector<FeatureNode*>& x) {
            return model.alpha[i] * model.y[i] * kernel(x, model.x[i]);
        }

        double parallel_svm_test_one(std::vector<FeatureNode*>& x) {
            return 0.0;
        }
        double sequentual_test_one(std::vector<FeatureNode*>& x) {

            double f = 0;
            for(int i=0;i<model.l; i++) {
                // f += model.alpha[i] * model.y[i] * kernel(x, model.x[i]);
                f += svm_tester(i, x);
            }
            return f + model.b;
        }

        double svm_test_one(std::vector<FeatureNode*>& x) {

            TestReducer agregate(this, x);
            tbb::parallel_reduce( 
                tbb::blocked_range<size_t>(0, model.l, 6),
                agregate);

            // if (agregate.value != f) 
            // {
            //     std::cout << agregate.value << std::endl;
            // }
            // else {
            //     std::cout << "OK!\n";
            // }
            return agregate.value + model.b;

        }
    
        struct TestReducer {
                float value;
                SVM* w_;
                std::vector<FeatureNode*>& x_;
                TestReducer(SVM* w, std::vector<FeatureNode*>& x) : value(0.0), w_(w), x_(x){}
                TestReducer(TestReducer& s, tbb::split):
                    value(0.0),
                    x_(s.x_),
                    w_(s.w_) {}
    
            void operator() (const tbb::blocked_range<size_t>& r) {
                float temp = value;
                for (size_t i = r.begin(); i!=r.end(); ++i) {
                    temp = w_->svm_tester(i, x_);
                    // temp += w_->runComputeHeavyOperation(i); 
                }
                value = temp;
            }
            void join(TestReducer& rhs) {
                value += rhs.value;
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
