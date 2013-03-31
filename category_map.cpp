#include <unordered_map>

namespace svm_learning {
	
	template <class T>
	CategoryMap<T>::CategoryMap() {
		old_new = new unordered_map<T, int>();
		new_old = new unordered_map<int, T>();
		last_index = 0;
	}

	template <class T>
	CategoryMap::size() {
		return old_new.size();
	}
	
	template <class T>
	CategoryMap<T>::is_empty(){
		return old_new.is_empty();
	}
	
	template <class T>
	CategoryMap<T>::add_category(T cat) {
		if(!old_new[cat]) {
			this.last_index ++;
			this.old_new[cat] = last_index;
			this.new_old[last_index] = cat;
		}
	}

	template <class T>
	T CategoryMap<class T>::get_old_category(int cat) {
		return new_old[cat];
	}
	
	template <class T>
	int CategoryMap<T>::get_new_category(T cat) {
		return (int)old_new[cat];
	}
}
