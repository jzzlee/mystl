///////////////////////////////////////////////////////////////////
//实现了基本的vector类
///////////////////////////////////////////////////////////////////
#ifndef __vector_H_
#define __vector_H_
//#include <memory>
#include "my_memory.h"
#include "my_iterator.h"
#include <initializer_list>

namespace my_stl
{
	template<typename T, typename Allocator = allocator<T>> class vector
	{
	public:
		typedef vector<T, Allocator>				Myt;
		typedef typename T							value_type;
		typedef typename Allocator					allocator_type;
		typedef typename ptrdiff_t					difference_type;
		typedef typename value_type&				reference;
		typedef typename const value_type&			const_reference;
		typedef typename Allocator::pointer			pointer;
		typedef typename Allocator::const_pointer	const_pointer;
		typedef typename pointer					iterator;
		typedef typename const_pointer				const_iterator;
		typedef typename std::size_t				size_type;

		//Default constructor
		explicit vector(const Allocator &alloc = Allocator()) : start(nullptr), finish(nullptr), end_of_storage(nullptr){}
		
		//Create a vector with n nodes
		vector(size_type num, const T &node, const Allocator &alloc = Allocator()) { fill_initialize(num, node); }
		//这两个为了避免与范围初始化重载冲突
		vector(int num, const T &node, const Allocator &alloc = Allocator()) { fill_initialize(num, node); }
		vector(long num, const T &node, const Allocator &alloc = Allocator()) { fill_initialize(num, node); }
		explicit vector(size_type n) { fill_initialize(n, T()); }

		//Create a vector with the elements of range [beg, end)
		template<typename InputIterator>
		vector(InputIterator first, InputIterator last) { copy_initialize(first, last); }

		//Copy constructor
		vector(const vector &vec){ copy_initialize(vec.begin(), vec.end()); }
		//Move constructor
		vector(vector &&vec) : start(vec.start), finish(vec.finish), end_of_storage(vec.end_of_storage)
		{ vec.start = vec.finish = vec.end_of_storage = nullptr; }

		//List constructor
		vector(std::initializer_list<T> lst) { copy_initialize(lst.begin(), lst.end()); }
		//Destructor
		~vector(){ destroy_and_destruct(); }
		
		//Assign operator
		vector & operator=(const vector &vec)
		{
			if (this != &vec)
			{
				if (vec.size() > capacity())
				{
					destroy_and_destruct();
					copy_initialize(vec.begin(), vec.end());
				}
				else if (size() >= vec.size())
				{
					iterator i = std::copy(vec.begin(), vec.end(), start);
					while (finish != i)
						alloc.destroy(--finish);
					finish = start + vec.size();
				}
				else
				{
					std::copy(vec.begin(), vec.begin() + size(), start);
					uninitialized_copy(vec.begin() + size(), vec.end(), finish);
					finish = start + vec.size();
				}
			}
			return *this;
		}
		//Move assign operator
		vector & operator=(vector &&vec)
		{
			//check if vec is the vector itself
			if (this != &vec)
			{
				start = vec.start;
				finish = vec.finish;
				end_of_storage = vec.end_of_storage;
				alloc = vec.alloc;
				vec.start = vec.finish = vec.end_of_storage = nullptr;
			}
			return *this;
		}
		
		//List operator =
		vector & operator=(std::initializer_list<T> lst)
		{
			if (lst.size() > capacity())
			{
				destroy_and_destruct();
				copy_initialize(lst.begin(), lst.end());
			}
			else if (size() >= lst.size())
			{
				iterator i = std::copy(lst.begin(), lst.end(), start);
				while (finish != i)
					alloc.destroy(--finish);
				finish = start + lst.size();
			}
			else
			{
				std::copy(lst.begin(), lst.begin() + size(), start);
				uninitialized_copy(lst.begin() + size(), lst.end(), finish);
				finish = start + lst.size();
			}
			return *this;
		}

		//Replaces the contents with count copies of value value
		void assign(size_type count, const T& value)
		{
			if (count > capacity())
			{
				destroy_and_destruct();
				fill_initialize(count, value);
			}
			else if (size() >= count)
			{
				iterator i = std::fill_n(start, count, value);
				while (finish != i)
					alloc.destroy(--finish);
				finish = start + count;
			}
			else
			{
				std::fill_n(start, size(), value);
				uninitialized_fill_n(start + size(), count - size(), value);
				finish = start + count;
			}
		}
		//避免与迭代器范围assign函数重载冲突
		void assign(int count, const T& value)
		{
			assign(size_type(count), value);
		}
		void assign(long count, const T& value)
		{
			assign(size_type(count), value);
		}

		//Replaces the contents with copies of those in the range [first, last)
		template< class InputIt >
		void assign(InputIt first, InputIt last)
		{
			size_type count = last - first;
			if (count > capacity())
			{
				destroy_and_destruct();
				copy_initialize(first, last);
			}
			else if (size() >= count)
			{
				iterator i = std::copy(first, last, start);
				while (finish != i)
					alloc.destroy(--finish);
				finish = start + count;
			}
			else
			{
				std::copy(first, first + size(), start);
				uninitialized_copy(first + size(), last, finish);
				finish = start +count;
			}
		}

		//Replaces the contents with the elements from the initializer list lst.
		void assign(std::initializer_list<T> lst)
		{
			size_type count = lst.end() - lst.begin();
			if (count > capacity())
			{
				destroy_and_destruct();
				copy_initialize(lst.begin(), lst.end());
			}
			else if (size() >= count)
			{
				iterator i = std::copy(lst.begin(), lst.end(), start);
				while (finish != i)
					alloc.destroy(--finish);
				finish = start + count;
			}
			else
			{
				std::copy(lst.begin(), lst.begin() + size(), start);
				uninitialized_copy(lst.begin() + size(), lst.end(), finish);
				finish = start + count;
			}
		}

		//Returns the allocator associated with the container. 
		allocator_type get_allocator() const { return alloc; }

		//Returns a reference to the element at specified location pos, with bounds checking.
		//If pos not within the range of the container, an exception of type std::out_of_range is thrown.
		reference at(size_type pos)
		{
			if (pos >= size())
				throw std::out_of_range("out of the range of the vector\n");
			return *(begin() + pos);
		}
		const_reference at(size_type pos) const
		{
			if (pos >= size())
				throw std::out_of_range("out of the range of the vector\n");
			return *(begin() + pos);
		}
		
		//Returns a reference to the element at specified location pos. No bounds checking is performed. 
		reference operator[](size_type i) {	return *(begin() + i); }
		const_reference operator[](size_type i) const { return *(begin() + i); }
		
		//Returns a reference to the first element in the container.
		reference front(){ return *begin(); }
		const_reference front() const { return *begin(); }

		//Returns reference to the last element in the container. 
		reference back() { auto tmp = end(); --tmp; return *tmp; }
		const_reference back() const { auto tmp = end(); --tmp; return *tmp; }

		//Returns pointer to the underlying array serving as element storage. 
		T* data() { if (empty()) return nullptr; return &front(); }
		const T* data() const { if (empty()) return nullptr; return &front(); }

		iterator begin() { return start; }
		const_iterator begin() const { return start; }
		const_iterator cbegin() const { return (((const Myt *)this)->begin()); }
		iterator end() { return finish;  }
		const_iterator end() const { return finish; }
		const_iterator cend() const { return (((const Myt *)this)->end()); }
	
		bool empty() const { return begin() == end(); }
		size_type size() const { return size_type(end() - begin()); }
		size_type max_size() const { return std::numeric_limits<size_type>::max(); }
		size_type capacity() const { return size_type(end_of_storage - begin()); }

		//Increase the capacity of the container to a value that's greater or equal to new_cap.
		//If new_cap is greater than the current capacity(), new storage is allocated, otherwise the method does nothing. 
		void reserve(size_type new_cap)
		{
			if (new_cap < capacity())
				return;
			if (new_cap > max_size())
				throw std::length_error("No enough memory.");
			size_type tmp_size = size();
			iterator tmp_start = alloc.allocate(new_cap);
			my_stl::uninitialized_copy(start, finish, tmp_start);
			destroy_and_destruct();
			start = tmp_start;
			finish = start + tmp_size;
			end_of_storage = start + new_cap;
		}

		//Requests the removal of unused capacity. 
		void shrink_to_fit()
		{
			alloc.deallocate(finish, end_of_storage - finish);
			end_of_storage = finish;
		}

		//Removes all elements from the container. 
		void clear()
		{
			while (finish != start)
				alloc.destroy(--finish);
		}


		// inserts value before pos
		iterator insert(iterator pos, const T& value)
		{
			return __insert_n(pos, 1, value);
		}
		iterator insert(const_iterator pos, const T& value)
		{
			return  __insert_n(pos, 1, value);
		}
		iterator insert(const_iterator pos, T&& value)
		{
			return __insert_n(pos, 1, value);
		}
		void insert(iterator pos, size_type count, const T& value)
		{
			__insert_n(pos, count, value);
		}
		iterator insert(const_iterator pos, size_type count, const T& value)
		{
			return __insert_n(pos, count, value);
		}
		void insert(iterator pos, int count, const T& value)
		{
			__insert_n(pos, count, value);
		}
		iterator insert(const_iterator pos, int count, const T& value)
		{
			return __insert_n(pos, count, value);
		}
		void insert(iterator pos, long count, const T& value)
		{
			__insert_n(pos, count, value);
		}
		iterator insert(const_iterator pos, long count, const T& value)
		{
			return __insert_n(pos, count, value);
		}
		template< class InputIt >
		void insert(iterator pos, InputIt first, InputIt last)
		{
			__insert(pos, first, last);
		}
		template< class InputIt >
		iterator insert(const_iterator pos, InputIt first, InputIt last)
		{
			return __insert(pos, first, last);
		}
		iterator insert(const_iterator pos, std::initializer_list<T> lst)
		{
			return __insert(pos, lst.begin(), lst.end());
		}

	private:
		enum MORE_SIZE { EXTRA_SPACE = 5 };
		allocator_type alloc;
		iterator start, finish;  //表示目前使用空间的头和尾
		iterator end_of_storage;  //表示目前可用空间的尾

		void fill_initialize(size_type n, const T& x)
		{
			start = alloc.allocate(n);
			my_stl::uninitialized_fill_n(start, n, x);
			end_of_storage = finish = start + n;
		}

		template<typename ForwardIterator>
		void copy_initialize(ForwardIterator first, ForwardIterator last)
		{
			size_type n = last - first;
			start = alloc.allocate(n);
			my_stl::uninitialized_copy(first, last, start);
			end_of_storage = finish = start + n;
		}

		void destroy_and_destruct()
		{
			//Release the elements
			while (finish != start)
				alloc.destroy(--finish);
			//Release the memory
			if (start)
				alloc.deallocate(start, end_of_storage - start);
		}

		iterator __insert_n(iterator pos, size_type n, const T &value)
		{
			if (size_type(end_of_storage - finish) >= n) //if there are enough empty spaces
			{
				size_type elem_count = finish - pos;
				iterator old_finish = finish;
				if (n <= elem_count) //插入点之后当前元素数量多于新增元素数量
				{
					my_stl::uninitialized_copy(finish - n, finish, finish);
					std::copy(pos, (iterator)(old_finish - n), pos + n);
					std::fill_n(pos, n, value);
					finish += n;
				}
				else                 //插入点之后当前元素数量小于新增元素数量
				{
					my_stl::uninitialized_copy(pos, finish, pos + n);
					my_stl::uninitialized_fill_n(finish, n - elem_count, value);
					std::fill_n(pos, elem_count, value);
					finish += n;
				}
				return pos;
			}
			else
			{
				size_type distance = pos - start;
				size_type old_size = size();
				size_type new_size = old_size > n ? 2 * old_size : 2 * n;
				iterator new_start = alloc.allocate(new_size);
				iterator new_finish = new_start;
				try
				{
					new_finish = my_stl::uninitialized_copy(start, pos, new_start);
					new_finish = my_stl::uninitialized_fill_n(new_finish, n, value);
					new_finish = my_stl::uninitialized_copy(pos, finish, new_finish);
				}
				catch (...)
				{
					while (new_finish != new_start)
						alloc.destroy(--new_finish);
					if (new_start)
						alloc.deallocate(new_start, new_size);
					throw;
				}
				//Release the elements
				while (finish != start)
					alloc.destroy(--finish);
				//Release the memory
				if (start)
					alloc.deallocate(start, end_of_storage - start);
				start = new_start;
				finish = new_finish;
				end_of_storage = new_start + new_size;
				return start + distance;
			}
		}
		iterator __insert_n(iterator pos, int n, const T &value)
		{
			return __insert_n(pos, size_type(n), value);
		}
		iterator __insert_n(iterator pos, long n, const T &value)
		{
			return __insert_n(pos, size_type(n), value);
		}
		iterator __insert_n(const_iterator pos, size_type n, const T &value)
		{
			iterator tmp_pos = start + (pos - start);
			return __insert_n(tmp_pos, n, value);
		}
		iterator __insert_n(const_iterator pos, int n, const T &value)
		{
			iterator tmp_pos = start + (pos - start);
			return __insert_n(tmp_pos, n, value);
		}
		iterator __insert_n(const_iterator pos, long n, const T &value)
		{
			iterator tmp_pos = start + (pos - start);
			return __insert_n(tmp_pos, n, value);
		}

		template<typename InputIt>
		iterator __insert(iterator pos, int n, InputIt last)
		{
			;
		}
		template<typename InputIt>
		iterator __insert(iterator pos, long n, InputIt last)
		{
			;
		}
		template< class InputIt >
		iterator __insert(iterator pos, InputIt first, InputIt last)
		{
			size_type n = distance(first, last);
			if (size_type(end_of_storage - finish) >= n) //if there are enough empty spaces
			{
				size_type elem_count = finish - pos;
				iterator old_finish = finish;
				if (n <= elem_count) //插入点之后当前元素数量多于新增元素数量
				{
					my_stl::uninitialized_copy(finish - n, finish, finish);
					std::copy(pos, (iterator)(old_finish - n), pos + n);
					std::copy(first, last, pos);
					finish += n;
				}
				else                 //插入点之后当前元素数量小于新增元素数量
				{
					my_stl::uninitialized_copy(pos, finish, pos + n);
					my_stl::uninitialized_copy_n(first + n, n - elem_count, finish);
					std::copy_n(first, elem_count, pos);
					finish += n;
				}
				return pos;
			}
			else
			{
				size_type diff = pos - start;
				size_type old_size = size();
				size_type new_size = old_size > n ? 2 * old_size : 2 * n;
				iterator new_start = alloc.allocate(new_size);
				iterator new_finish = new_start;
				try
				{
					new_finish = my_stl::uninitialized_copy(start, pos, new_start);
					new_finish = my_stl::uninitialized_copy(first, last, new_finish);
					new_finish = my_stl::uninitialized_copy(pos, finish, new_finish);
				}
				catch (...)
				{
					while (new_finish != new_start)
						alloc.destroy(--new_finish);
					if (new_start)
						alloc.deallocate(new_start, new_size);
					throw;
				}
				//Release the elements
				while (finish != start)
					alloc.destroy(--finish);
				//Release the memory
				if (start)
					alloc.deallocate(start, end_of_storage - start);
				start = new_start;
				finish = new_finish;
				end_of_storage = new_start + new_size;
				return start + diff;
			}
		}

		template< class InputIt >
		iterator __insert(const_iterator pos, InputIt first, InputIt last)
		{
			difference_type diff = pos - start;
			iterator tmp_pos = start + diff;
			return __insert(tmp_pos, first, last);
		}
	};







}
#endif