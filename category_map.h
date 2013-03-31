#ifndef SVM_CAT_MAP
#define SVM_CAT_MAP

#include <unordered_map>

namespace svm_learning {
	template <class T>
	class CategoryMap() {
	private:
		std::unordered_map<T, int> old_new;
		std::unordered_map<int, T> new_old;
		size_t last_index;
	public:
		size_t size();
		bool is_empty();
		void add_category(T);
		T get_old_category(int);
		int get_new_category(T);

	}
}
#endif