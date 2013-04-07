#ifndef KPARAMS
#define KPARAMS

namespace svm_learning {
	
	class KernelParams {
	public:
		int kernel;
		double a, b, c;
		KernelParams(int k, double a, double b, double c) {
			this->kernel = k;
			this->a = a;
			this->b = b;
			this->c = c;
		}
		KernelParams(){
			this->kernel = 1;
			this->a = this->b = this->c = 1.0;
		};
	};
}
#endif
