///////////////////////////////////////////////////////////////////
//实现了基本的vector类
///////////////////////////////////////////////////////////////////
#ifndef __vector_H_
#define __vector_H_
//#include <memory>
#include "my_memory.h"
//#include <iterator>
#include "my_iterator.h"
#include <initializer_list>

namespace my_stl
{
	template<typename T, typename Allocator = allocator<T>> class vector
	{
	public:
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
					end_of_storage = finish = start + (vec.end() - vec.begin());
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

		vector & operator=(std::initializer_list<T> lst)
		{
			if (lst.size() > capacity())
			{
				destroy_and_destruct();
				copy_initialize(lst.begin(), lst.end());
				end_of_storage = finish = start + lst.size();
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

		iterator begin() { return start; }
		const_iterator begin() const { return start; }
		iterator end() { return finish;  }
		const_iterator end() const { return finish; }
		size_type size() const { return size_type(end() - begin()); }
		size_type max_size() const { return size_type(-1) / sizeof(T); }
		size_type capacity() const { return size_type(end_of_storage - begin()); }
		bool empty() const { return begin() == end(); }
		reference operator[](size_type i) { return *(begin() + i); }
		const_reference operator[](size_type i) const { return *(begin() + i); }


	private:
		enum MORE_SIZE { EXTRA_SPACE = 5 };
		allocator_type alloc;
		iterator start, finish;  //表示目前使用空间的头和尾
		iterator end_of_storage;  //表示目前可用空间的尾

		void fill_initialize(size_type n, const T& x)
		{
			start = alloc.allocate(n);
			my_stl::uninitialized_fill_n(start, n, x);
			finish = start + n;
			end_of_storage = finish;
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
	};







}
#endif