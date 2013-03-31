#ifndef KPARAMS
#define KPARAMS

namespace svm_learning {
	
	class KernelParams {
	public:
		int kernel = 1;
		double a, b, c;
		KernelParams(int k, double a, double b, double c) {
			this.kernel = k;
			this.a = a;
			this.b = b;
			this.c = c;
		}
		KernelParams():a(1), b(1), c(1), kernel(1){};
	};
}
#endif
