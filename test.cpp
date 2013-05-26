#include <vector>
#include <cmath>
#include <iostream>

#include <tbb/blocked_range.h>
#include <tbb/parallel_for.h>
#include <tbb/parallel_reduce.h>
#include <tbb/task_scheduler_init.h>
#include <tbb/tick_count.h>

#define N 4 * 10e6
#define nThreads 3
int grain_size;

class Worker 
{
public:
    Worker() {
        mpl.resize(N);
        for(int i=0;i<N;i++) 
            mpl[i] = (0.5 * i);
        std::cout << "done\n";
    }
    float runComputeHeavyOperation(size_t i) {
        return sqrt(this->mpl[i]);
    }
    float linear() {
        float suma = 0;
        for(int i=0; i<N; i++) {
            suma += sqrt(this->mpl[i]);
        }
        return suma;
    }

    float computeHeavyMethod() {
        tbb::task_scheduler_init init(3);
        TbbExecutor tbbExec(this);

        tbb::parallel_reduce( 
            tbb::blocked_range<size_t>(0, N),
            tbbExec);

        return tbbExec.value;
    }


private:
    std::vector<float> mpl;

    struct TbbExecutor 
    {
        float value;
        Worker* w_;
        TbbExecutor(Worker* w) : value(0.), w_(w) {}
        TbbExecutor(TbbExecutor& s, tbb::split) {value=0.0; w_ = s.w_;}

        void operator() (const tbb::blocked_range<size_t>& r) {
            float temp = value;
            const size_t end = r.end();
            for (size_t i = r.begin(); i!=end; ++i) {
                temp += w_->runComputeHeavyOperation(i); 
            }
            value = temp;
        }
        void join(TbbExecutor& rhs) {
            value += rhs.value;
        }
    };
};

int main(int argc, char * argv[]) {
    grain_size = int(argv[2]);
    Worker w;
    std::cout << "worker created\n";
    // w.print();
    tbb::tick_count start, end;

    start = tbb::tick_count::now();
    if (argc == 1) {
        w.linear();
    }
    else {
        w.computeHeavyMethod();
    }
    end = tbb::tick_count::now();
    double time_ = (end - start).seconds();

    std::cout << std::endl;
    std::cout << "Seconds: " << time_ << std::endl;
    // w.print_square();
    std::cout << std::endl;
    return 0;
}