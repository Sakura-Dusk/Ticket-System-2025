#ifndef VECTOR_HPP
#define VECTOR_HPP

template<typename T>
class vector
{
public:
	/**
	 * TODO
	 * a type for actions of the elements of a vector, and you should write
	 *   a class named const_iterator with same interfaces.
	 */
	int Size, len;
	char* a;
	/**
	 * you can see RandomAccessIterator at CppReference for help.
	 */
	class const_iterator;
	class iterator
	{
	// The following code is written for the C++ type_traits library.
	// Type traits is a C++ feature for describing certain properties of a type.
	// For instance, for an iterator, iterator::value_type is the type that the
	// iterator points to.
	// STL algorithms and containers may use these type_traits (e.g. the following
	// typedef) to work properly. In particular, without the following code,
	// @code{std::sort(iter, iter1);} would not compile.
	// See these websites for more information:
	// https://en.cppreference.com/w/cpp/header/type_traits
	// About value_type: https://blog.csdn.net/u014299153/article/details/72419713
	// About iterator_category: https://en.cppreference.com/w/cpp/iterator
	public:
		using difference_type = std::ptrdiff_t;
		using value_type = T;
		using pointer = T*;
		using reference = T&;
		using iterator_category = std::output_iterator_tag;

	private:
		/**
		 * TODO add data members
		 *   just add whatever you want.
		 */
		 T* id;
		 vector* vectorid;
		friend class vector;
		iterator(vector* vectorid, T* id) : vectorid(vectorid), id(id) {}

	public:
		/**
		 * return a new iterator which pointer n-next elements
		 * as well as operator-
		 */
		iterator operator+(const int &n) const
		{
			//TODO
			iterator res = *this;
			res.id += n;
			return res;
		}
		iterator operator-(const int &n) const
		{
			//TODO
			iterator res = *this;
			res.id -= n;
			return res;
		}
		// return the distance between two iterators,
		// if these two iterators point to different vectors, throw invaild_iterator.
		int operator-(const iterator &rhs) const
		{
			//TODO
			// if (vectorid != rhs.vectorid) throw invalid_iterator();
			int res = id - rhs.id; if (res < 0) res = -res;
			return res;
		}
		iterator& operator+=(const int &n)
		{
			//TODO
			id += n;
			return *this;
		}
		iterator& operator-=(const int &n)
		{
			//TODO
			id -= n;
			return *this;
		}
		/**
		 * TODO iter++
		 */
		iterator operator++(int) {
			iterator res = *this;
			id++;
			return res;
		}
		/**
		 * TODO ++iter
		 */
		iterator& operator++() {
			id++;
			return *this;
		}
		/**
		 * TODO iter--
		 */
		iterator operator--(int) {
			iterator res = *this;
			id--;
			return res;
		}
		/**
		 * TODO --iter
		 */
		iterator& operator--() {
			id--;
			return this;
		}
		/**
		 * TODO *it
		 */
		T& operator*() const{
			return *id;
		}
		/**
		 * a operator to check whether two iterators are same (pointing to the same memory address).
		 */
		bool operator==(const iterator &rhs) const {
			return id == rhs.id;
		}
		bool operator==(const const_iterator &rhs) const {
			return id == rhs.id;
		}
		/**
		 * some other operator for iterator.
		 */
		bool operator!=(const iterator &rhs) const {
			return id != rhs.id;
		}
		bool operator!=(const const_iterator &rhs) const {
			return id != rhs.id;
		}
	};
	/**
	 * TODO
	 * has same function as iterator, just for a const object.
	 */
	class const_iterator
	{
	public:
		using difference_type = std::ptrdiff_t;
		using value_type = T;
		using pointer = T*;
		using reference = T&;
		using iterator_category = std::output_iterator_tag;

	private:
		/*TODO*/
		friend class vector;
		const T* id;
		const vector* vectorid;
		const_iterator(const vector* vectorid, const T* id) : vectorid(vectorid), id(id) {}

	public:
		/**
		 * return a new iterator which pointer n-next elements
		 * as well as operator-
		 */
		const_iterator operator+(const int &n) const
		{
			//TODO
			const_iterator res = *this;
			id += n;
			return res;
		}
		const_iterator operator-(const int &n) const
		{
			//TODO
			const_iterator res = *this;
			id -= n;
			return res;
		}
		// return the distance between two iterators,
		// if these two iterators point to different vectors, throw invaild_iterator.
		int operator-(const const_iterator &rhs) const
		{
			//TODO
			// if (vectorid != rhs.vectorid) throw invalid_iterator();
			int res = id - rhs.id; if (res < 0) res = -res;
			return res;
		}
		const_iterator& operator+=(const int &n)
		{
			//TODO
			id += sizeof(T) * n;
			return *this;
		}
		const_iterator& operator-=(const int &n)
		{
			//TODO
			id -= sizeof(T) * n;
			return *this;
		}
		/**
		 * TODO iter++
		 */
		const_iterator operator++(int) {
			const_iterator res = *this;
			id++;
			return res;
		}
		/**
		 * TODO ++iter
		 */
		const_iterator& operator++() {
			id++;
			return *this;
		}
		/**
		 * TODO iter--
		 */
		const_iterator operator--(int) {
			const_iterator res = *this;
			id--;
			return res;
		}
		/**
		 * TODO --iter
		 */
		const_iterator& operator--() {
			id--;
			return this;
		}
		/**
		* TODO *it
		*/
		const T& operator*() const{
			return *id;
		}
		/**
		* a operator to check whether two iterators are same (pointing to the same memory address).
		*/
		bool operator==(const iterator &rhs) const {
			return id == rhs.id;
		}
		bool operator==(const const_iterator &rhs) const {
			return id == rhs.id;
		}
		/**
		* some other operator for iterator.
		*/
		bool operator!=(const iterator &rhs) const {
			return id != rhs.id;
		}
		bool operator!=(const const_iterator &rhs) const {
			return id != rhs.id;
		}
	};
	/**
	 * TODO Constructs
	 * At least two: default constructor, copy constructor
	 */
	vector() {
		Size = 0; len = 10;
		a = new char[len * sizeof(T)];
	}
	vector(const vector &other) {
		Size = other.Size; len = other.len;
		a = new char[len * sizeof(T)];
		for (int i = 0; i < Size; i++) new(reinterpret_cast<T*>(a + i * sizeof(T)))T(other[i]);
	}
	/**
	 * TODO Destructor
	 */
	~vector() {
		for (int i = 0; i < Size; i++) reinterpret_cast<T*>(a + i * sizeof(T)) -> ~T(); Size = 0;
		delete[] a;
	}
	/**
	 * TODO Assignment operator
	 */
	vector &operator=(const vector &other) {
		if (&other == this) return *this;
		for (int i = 0; i < Size; i++) reinterpret_cast<T*>(a + i * sizeof(T)) -> ~T();
		delete[] a;
		Size = other.Size; len = other.len;
		a = new char[len * sizeof(T)];
		for (int i = 0; i < Size; i++) new(reinterpret_cast<T*>(a + i * sizeof(T)))T(other[i]);
		return *this;
	}
	/**
	 * assigns specified element with bounds checking
	 * throw index_out_of_bound if pos is not in [0, Size)
	 */
	T & at(const size_t &pos) {
		// if (pos < 0 || pos >= Size) throw index_out_of_bound();
		return reinterpret_cast<T*>(a)[pos];
	}
	const T & at(const size_t &pos) const {
		// if (pos < 0 || pos >= Size) throw index_out_of_bound();
		return reinterpret_cast<T*>(a)[pos];
	}
	/**
	 * assigns specified element with bounds checking
	 * throw index_out_of_bound if pos is not in [0, Size)
	 * !!! Pay attentions
	 *   In STL this operator does not check the boundary but I want you to do.
	 */
	T & operator[](const size_t &pos) {
		// if (pos < 0 || pos >= Size) throw index_out_of_bound();
		return reinterpret_cast<T*>(a)[pos];
	}
	const T & operator[](const size_t &pos) const {
		// if (pos < 0 || pos >= Size) throw index_out_of_bound();
		return reinterpret_cast<T*>(a)[pos];
	}
	/**
	 * access the first element.
	 * throw container_is_empty if Size == 0
	 */
	const T & front() const {
		// if (!Size) throw container_is_empty();
		return reinterpret_cast<T*>(a)[0];
	}
	/**
	 * access the last element.
	 * throw container_is_empty if Size == 0
	 */
	const T & back() const {
		// if (!Size) throw container_is_empty();
		return reinterpret_cast<T*>(a)[Size - 1];
	}
	/**
	 * returns an iterator to the beginning.
	 */
	iterator begin() {
		// if (!Size) throw container_is_empty();
		return iterator(this, &reinterpret_cast<T*>(a)[0]);
	}
	const_iterator begin() const {
		// if (!Size) throw container_is_empty();
		return const_iterator(this, &reinterpret_cast<T*>(a)[0]);
	}
	const_iterator cbegin() const {
		// if (!Size) throw container_is_empty();
		return const_iterator(this, &reinterpret_cast<T*>(a)[0]);
	}
	/**
	 * returns an iterator to the end.
	 */
	iterator end() {
		return iterator(this, reinterpret_cast<T*>(a + Size * sizeof(T)));
	}
	const_iterator end() const {
		return const_iterator(this, reinterpret_cast<T*>(a + Size * sizeof(T)));
	}
	const_iterator cend() const {
		return const_iterator(this, reinterpret_cast<T*>(a + Size * sizeof(T)));
	}
	/**
	 * checks whether the container is empty
	 */
	bool empty() const {
		return Size == 0;
	}
	/**
	 * returns the number of elements
	 */
	size_t size() const {
		return Size;
	}
	/**
	 * clears the contents
	 */
	void clear() {
		for (int i = 0; i < Size; i++) reinterpret_cast<T*>(a + i * sizeof(T)) -> ~T();
		delete[] a;
		Size = 0; len = 10;
		a = new char[len * sizeof(T)];
	}
	/**
	 * inserts value before pos
	 * returns an iterator pointing to the inserted value.
	 */
	iterator insert(iterator pos, const T &value) {
		int ind = pos.id - &reinterpret_cast<T*>(a)[0];
		if (Size == len) {
			char* tmp = a;
			a = new char[2 * len * sizeof(T)];
			memcpy(a, tmp, Size * sizeof(T));
			delete[] tmp;
			len *= 2;
		}
		new(reinterpret_cast<T*>(a + Size * sizeof(T)))T(value);
		Size++;
		for (int i = Size - 2; i >= ind; i--) at(i + 1) = at(i);
		at(ind) = value;
		return iterator(this, &reinterpret_cast<T*>(a)[ind]);
	}
	/**
	 * inserts value at index ind.
	 * after inserting, this->at(ind) == value
	 * returns an iterator pointing to the inserted value.
	 * throw index_out_of_bound if ind > Size (in this situation ind can be Size because after inserting the Size will increase 1.)
	 */
	iterator insert(const size_t &ind, const T &value) {
		// if (ind > Size) throw index_out_of_bound();
		if (Size == len) {
			char* tmp = a;
			a = new char[2 * len * sizeof(T)];
			memcpy(a, tmp, Size * sizeof(T));
			delete[] tmp;
			len *= 2;
		}
		new(reinterpret_cast<T*>(a + Size * sizeof(T)))T(value);
		Size++;
		for (int i = Size - 2; i >= ind; i--) at(i + 1) = at(i);
		at(ind) = value;
		return iterator(this, &reinterpret_cast<T*>(a)[ind]);
	}
	/**
	 * removes the element at pos.
	 * return an iterator pointing to the following element.
	 * If the iterator pos refers the last element, the end() iterator is returned.
	 */
	iterator erase(iterator pos) {
		int ind = pos.id - &reinterpret_cast<T*>(a)[0];
		reinterpret_cast<T*>(a + ind * sizeof(T)) -> ~T();
		for (int i = ind + 1; i < Size; i++) at(i - 1) = at(i);
		Size--;
		return iterator(this, &reinterpret_cast<T*>(a)[ind]);
	}
	/**
	 * removes the element with index ind.
	 * return an iterator pointing to the following element.
	 * throw index_out_of_bound if ind >= Size
	 */
	iterator erase(const size_t &ind) {
		reinterpret_cast<T*>(a + ind * sizeof(T)) -> ~T();
		for (int i = ind + 1; i < Size; i++) at(i - 1) = at(i);
		Size--;
		return iterator(this, &reinterpret_cast<T*>(a)[ind]);
	}
	/**
	 * adds an element to the end.
	 */
	void push_back(const T &value) {
		insert(end(), value);
	}
	/**
	 * remove the last element from the end.
	 * throw container_is_empty if Size() == 0
	 */
	void pop_back() {
		// if (empty()) throw container_is_empty();
		const size_t tmp = Size - 1;
		erase(tmp);
	}
};

#endif

// //
// // Created by Sakura on 25-5-14.
// //
//
// #ifndef VECTOR_HPP
// #define VECTOR_HPP
//
//
// template<typename T>
// class vector {
//     public:
//       int Size, capacity;
//       T* data;
//
//       vector() {
//         Size = 0; capacity = 10;
//         data = nullptr; data = new T[capacity];
//       }
//
//       ~vector() {
//         delete[] data; Size = 0; data = nullptr;
//       }
//
//       void push_back(T x) {
//         if (Size == capacity) {
//           T* newData = new T[capacity * 2];
//           for (int i = 0; i < Size; i++) newData[i] = data[i];
//           delete[] data;
//           capacity *= 2;
//           data = newData;
//         }
//         data[Size++] = x;
//       }
//
//       T& operator[](int index) {
//         return data[index];
//       }
//
//       int size() {
//          return Size;
//       }
//
//       void erase(const int &id) {
//           memmove(reinterpret_cast<T*>(data) + sizeof(T) * id, reinterpret_cast<T*>(data) + sizeof(T) * (id + 1), sizeof(T) * (Size - id - 1));
//           Size--;
//       }
//
//       void clear() {
//         Size = 0;
//       }
// };
//
//
//
// #endif //VECTOR_HPP
