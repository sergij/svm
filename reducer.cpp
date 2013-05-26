#include <vector>
#include <cmath>
#include <iostream>
#include <cstdlib>

#include <tbb/blocked_range.h>
#include <tbb/parallel_for.h>
#include <tbb/parallel_reduce.h>
#include <tbb/task_scheduler_init.h>
#include <tbb/tick_count.h>

#define N 4 * 10e6
#define nThreads 10
int grain_size;
int parts;

class Worker 
{
public:
    Worker() {
        // mpl.resize(N);
        mpl = new float[int(N)];
        for(int i=0;i<N;i++) 
            mpl[i] = (0.5 * i);
        std::cout << "done\n";
    }
    float linear() {
        float suma = 0;
        for(int i=0; i<N; i++) {
            suma += sqrt(this->mpl[i]) / 100.;
        }
        return suma;
    }

    float computeHeavyMethod() {
        tbb::task_scheduler_init init(nThreads);
        TbbExecutor tbbExec;

        tbb::parallel_reduce( 
            tbb::blocked_range<float*>(mpl, mpl + size_t(N), int(N / parts)),
            tbbExec);

        return tbbExec.value;
    }


private:
    float* mpl;

    struct TbbExecutor 
    {
        float value;
        Worker* w_;
        TbbExecutor() : value(0.) {}
        TbbExecutor(TbbExecutor& s, tbb::split): value(0.0) {}

        void operator() (const tbb::blocked_range<float*>& r) {
            std::cout << '.';
            float temp = value;
            for (float* i = r.begin(); i!=r.end(); ++i) {
                temp += sqrt(*i) / 100.; 
            }
            value = temp;
        }
        void join(TbbExecutor& rhs) {
            value += rhs.value;
        }
    };
};

int main(int argc, char * argv[]) {
    parts = atoi(argv[1]);
    std::cout << parts << std::endl;
    grain_size = 3; // atoi(argv[2]);
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
