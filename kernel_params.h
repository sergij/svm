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
		KernelParams():kernel(1), a(1), b(1), c(1){};
	};
}
#endif
