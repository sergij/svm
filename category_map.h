#ifndef SVM_CAT_MAP
#define SVM_CAT_MAP

#include <unordered_map>

namespace svm_learning {

	class CategoryMap {
	private:
		std::unordered_map<int, int> old_new;
		std::unordered_map<int, int> new_old;
        int last_index;
	public:
		CategoryMap() { last_index = 0;}
        int size() {
            return (int)old_new.size();
		}
		bool is_empty(){
			return old_new.empty();
		}
		void add_category(int cat) {
			if(!old_new[cat]) {
				this->last_index ++;
				this->old_new[cat] = last_index;
				this->new_old[last_index] = cat;
			}
		}
		int get_old_category(int cat) {
			return new_old[cat];
		}
		int get_new_category(int cat) {
			return (int)old_new[cat];
		}
	};
}
#endif
