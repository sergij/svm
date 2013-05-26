#include <vector>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <iostream>

#include "model.h"
#include "problem.h"
#include "kernel_params.h"
#include "kernels.h"
#include "svm.h"

#include <tbb/blocked_range.h>
#include <tbb/parallel_for.h>
#include <tbb/parallel_reduce.h>
#include <tbb/task_scheduler_init.h>
#include <tbb/tick_count.h>

namespace svm_learning {
    
	SVM::SVM(){
        C = 1;

        tol = 10e-3;
        tol2 = 10e-5;
    }

	void SVM::train(Problem train) {
        KernelParams p = KernelParams();
        smo(train, p);
    }

    void SVM::smo(Problem train, KernelParams p) {
        model = Model();
        model.alpha = std::vector<double>(train.l, 0);
        model.b = 0;
        model.params = p;
        model.x = train.x;
        model.y = train.y;
        model.l = train.l;
        model.n = train.n;

        int model_l = model.l;
        E = std::vector<double>(model_l, 0.0);

        
        tbb::tick_count time_s, time_e;
        time_s = tbb::tick_count::now();
        ParallelVectorize parallel_calculation(this, &model);
        tbb::parallel_for(
            tbb::blocked_range<size_t>(0, model_l, model_l >> 1),
            parallel_calculation);
        // for(int i=0; i<model_l; i++) {
        //     E[i] = svm_test_one(model.x[i]) - model.y[i];
        // }
        time_e = tbb::tick_count::now();
        std::cout << "First E_i calculation: " << (double)(time_e - time_s).seconds() << "s\n";

        int num_changes = 0;
        int examine_all = 1;

        while (num_changes > 0 || examine_all == 1) {
            num_changes = 0;

            if(examine_all == 1) {
                for(int i=0;i<model_l; i++) {
                    num_changes += psmo_examine_example(i);
                }
            }
            else {
                for (int i=0;i<model_l; i++) {
                    if (model.alpha[i] == 0) {
                        num_changes += psmo_examine_example(i);
                    }
                }
            }

            if (examine_all == 1)
                examine_all = 0;
            else if (num_changes == 0)
                examine_all = 1;
        }
    }

    int SVM::psmo_take_step(int i, int j) {
        double ai, aj;
        if(i == j) return 0;
        b_old = model.b;
        ai_old = model.alpha[i];
        int yi = model.y[i];
        int yj = model.y[j];
        Ei = E[i];
        double s = yi * yj;
        L = compute_param_L(yi, yj);
        H = compute_param_H(yi, yj);
        if (L == H) return 0;
        double kii = kernel(i, i);
        double kjj = kernel(j, j);
        double kij = kernel(i, j);
        eta = 2 * kij - kii - kjj;
        if (eta < 0) {
            aj = aj_old - yj * (Ei - Ej) / eta;
            if (aj < L)aj = L;
            else if (aj > H) aj = H;
        } else {
            double vi, vj;
            vi = svm_test_one(model.x[i]) - yi * ai_old * kii - yj * aj_old * kij;
            vj = svm_test_one(model.x[j]) - yi * ai_old * kij - yj * aj_old * kjj;
            double lobj = smo_obj(L, yi, yj, kij, kii, kjj, vi, vj);
            double hobj = smo_obj(H, yi, yj, kij, kii, kjj, vi, vj);
            if(lobj > (hobj + tol)) aj = L;
            else if (lobj < (hobj - tol)) aj = H;
            else aj = aj_old;
        }
        if (aj < tol2)
            aj = 0;
        else if (aj > (C - tol2))
            aj = C;

        if(fabs(aj - aj_old) < tol * (aj + aj_old + tol))
            return 0;
        ai = ai_old + s * (aj_old - aj);
        compute_param_bias(ai, aj, yi, yj, kii, kjj, kij);
        model.alpha[i] = ai;
        model.alpha[j] = aj;
        for (int k=0; k<model.l; k++) {
            double kik = kernel(model.x[i], model.x[k]);
            double kjk = kernel(model.x[j], model.x[k]);
            E[k] += (ai - ai_old) * yi*kik + (aj - aj_old) * yj * kjk - b_old + model.b;
        }
        return 1;
    }

    int SVM::psmo_examine_example(int j) {
        if(model.alpha[j] != 0) return 0;
        int i = 0;
        int randpos;
        int yj = model.y[j];
        aj_old = model.alpha[j];
        Ej = E[j];
        double rj = Ej * yj;
        if ((rj < -tol && aj_old<C) || (rj>tol && aj_old > 0)) {
            bool exists = false;
            for(int k=0;k<model.l; k++) {
                if(model.alpha[k]>0 && model.alpha[k]<C) {
                    exists = true;
                    break;
                }
            }
            if (exists) {
                int max_ind = 0;
                double max_val = (E[0]>Ej)?E[0]-Ej:Ej-E[0];
                for(int k=1; k<model.l; k++) {
                    if(fabs(E[k]-Ej) > max_val) {
                        max_val = fabs(E[k]-Ej);
                        max_ind = k;
                    }
                }
                if (psmo_take_step(max_ind, j) == 1)
                    return 1;
            }

            randpos = rand() % model.l;
            int a_size = model.alpha.size();
            for (int k=0; k<a_size; k++) {
                i = (randpos + k) % model.l;
                if (psmo_take_step(i, j) == 1)
                    return 1;
            }
        }
        return 0;
    }

    double SVM::smo_obj(double aj,
                        int yi,
                        int yj,
                        double kij,
                        double kii,
                        double kjj,
                        double vi,
                        double vj) {
        double s = yi * yj;
        double gamma = ai_old + s*aj_old;
        double result = (gamma + (1-s)*aj - 0.5*kii*(gamma-s*aj)*(gamma-s*aj) - 0.5*kjj*aj*aj +
                - s*kij*(gamma-s*aj)*aj - yi*(gamma-s*aj)*vi - yj*aj*vj);
        return result;
    }

    double SVM::compute_param_L(int yi, int yj) {
        double L = 0;
        if (yi != yj) {
            L = std::max(0., aj_old - ai_old);
        } else {
            L = std::max(0., ai_old + aj_old - C);
        }
        return L;
    }
    double SVM::compute_param_H(int yi, int yj) {
        double H = 0;
        if(yi != yj) {
            H = std::min(C, aj_old + C - ai_old);
        } else {
            H = std::min(C, ai_old + aj_old);
        }
        return H;
    }
    void SVM::compute_param_bias(double ai,
                                 double aj,
                                 int yi,
                                 int yj,
                                 double kii,
                                 double kjj,
                                 double kij) {
        double b1 = model.b - Ei - yi*(ai-ai_old)*kii - yj*(aj-aj_old) * kij;
        double b2 = model.b - Ej - yi*(ai-ai_old)*kij - yj*(aj-aj_old) * kjj;
        if (0 < ai && ai<C)
            model.b = b1;
        else if (0 < aj && aj < C)
            model.b = b2;
        else
            model.b = (b1+b2)/2.;
    }
    std::vector<int> SVM::test(Problem test) {
        std::vector<int> pred = std::vector<int>(test.l, 0);
        for( int i=0;i<test.l; i++) {
            pred[i] = (test_one(test.x[i])<0?-1:1);
        }
        return pred;
    }
    // double SVM::svm_test_one(std::vector<FeatureNode*> x) {
    //     double f = 0.;
    //     for(int i=0;i<model.l; i++) {
    //         f += model.alpha[i] * model.y[i] * kernel(x, model.x[i]);
    //     }
    //     return f + model.b;
    // }
    double SVM::kernel(std::vector<FeatureNode*> x,
                       std::vector<FeatureNode*> z) {
        double ret = 0;
		Kernel ker = Kernel();
        switch (model.params.kernel) {
        case 1:
            ret = ker.k_linear(x, z);
            break;
        case 2:
            ret = ker.k_poly(x, z, model.params.a, model.params.b, model.params.c);
            break;
        case 3:
            ret = ker.k_gaussian(x, z, model.params.a);
            break;
        case 4:
            ret = ker.k_tanh(x, z, model.params.a, model.params.b);
            break;
        }
        return ret;
    }
    double SVM::kernel(int i, int j) {
        double ret = 0;
		Kernel ker = Kernel();
        switch (model.params.kernel) {
        case 1:
            ret = ker.k_linear(model.x[i], model.x[j]);
            break;
        case 2:
            ret = ker.k_poly(model.x[i], model.x[j], model.params.a, model.params.b, model.params.c);
            break;
        case 3:
            ret = ker.k_gaussian(model.x[i], model.x[j], model.params.a);
            break;
        case 4:
            ret = ker.k_tanh(model.x[i], model.x[j], model.params.a, model.params.b);
            break;
        }
        return ret;
    }
}

