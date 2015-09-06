#ifndef __vector_H_
#define __vector_H_
//#include <memory>
#include "my_memory.h"
#include <iterator>
#include <initializer_list>

namespace my_stl
{
	using  std::random_access_iterator_tag;
	template<typename T, typename Allocator = allocator<T>> class vector
	{
	public:
		class iterator;
		typedef typename T value_type;
		typedef typename Allocator alloc_type;

		vector();//Default constructor
		vector(const vector &vec); //Copy constructor
		vector(vector &&vec); //Move constructor
		vector & operator=(vector &&vec); //Move assign operator
		vector(const int num, const T &node = T()); //Create a vector with n nodes
		vector(iterator beg, iterator end); //Create a vector with the elements of range [beg, end)
		vector(std::initializer_list<T> lst); //List constructor
		~vector();//Destructor
		
		
		std::size_t  size() const;
		std::size_t  capcity() const;

		T & operator[](size_t i);

		iterator begin() const
		{
			return iterator(head_pointer); 
		}
		iterator end() const
		{
			return iterator(end_pointer);  
		}

		//const_iterator cbegin() const 
		//{
		//	return const_cast<const_iterator>(iterator(head_pointer));
		//}
		//const_iterator cend() const
		//{
		//	return iterator(end_pointer);
		//}

	private:
		enum MORE_SIZE { EXTRA_SPACE = 5 };
		Allocator alloc;
		T *head_pointer, *end_pointer;
		std::size_t vec_size, vec_capcity;
		void initialize();
	};

	//***********************************************************************************************
	//This part is class iterator
	template<typename T, typename Allocator>
	class vector<T, Allocator>::iterator
	{
	public:
		typedef typename T                     value_type;
		typedef typename ptrdiff_t			   difference_type;
		typedef typename T&                    reference;
		typedef typename T*                    pointer;
		typedef typename std::random_access_iterator_tag  iterator_category;
	protected:
		T*      item;
	public:
		iterator() :item(nullptr){}
		iterator(T* input_item) :item(input_item){}
		iterator(iterator& iter) :item(iter.item){}

		template<typename Iterator>
		Iterator& operator=(Iterator iter)
		{
			item = iter.item;
			return *this;
		}

		~iterator(){}

		iterator& operator++()
		{
			++item;
			return *this;
		}

		iterator operator++(int)
		{
			iterator temp = *this;
			++(*this);
			return temp;
		}

		iterator operator--()
		{
			--item;
			return *this;
		}

		iterator operator--(int)
		{
			iterator temp = *this;
			--(*this);
			return temp;
		}

		iterator operator-(int n)
		{
			iterator temp = *this;
			temp.item -= n;
			return temp;
		}

		iterator operator+(int n)
		{
			iterator temp = *this;
			temp.item += n;
			return temp;
		}

		difference_type operator-(iterator other_iter)
		{
			return  item - other_iter.item;
		}

		T & operator*()
		{
			return *item;
		}

		bool operator== (const iterator &rv) const
		{
			return this->item == rv.item;
		}

		bool operator != (const iterator &rv) const
		{
			return this->item != rv.item;
		}

		bool operator < (const iterator &rv) const
		{
			return this->item < rv.item;
		}
	};

	//*********************************************************************************************

	//********************************************************************************************
	//This part is constructor and destructor
	//
	//Default Constructor 
	template<typename T, typename Allocator >
	vector<T, Allocator>::vector()
		:head_pointer(nullptr), end_pointer(nullptr), vec_size(0), vec_capcity(0) 
	{
		head_pointer = alloc.allocate(vec_capcity);
		end_pointer = head_pointer;
	}

	//Copy Constrictor
	template<typename T, typename Allocator>
	vector<T, Allocator>::vector(const vector &vec)
		: vec_size(vec.size()), vec_capcity(vec_size + EXTRA_SPACE)
	{
		head_pointer = alloc.allocate(vec_capcity);
		end_pointer = std::uninitialized_copy(vec.begin(), vec.end(), head_pointer);
	}

	//Move constructor
	template<typename T, typename Allocator>
	vector<T, Allocator>::vector(vector &&vec)
		: vec_size(vec.size()), vec_capcity(vec.capcity()), head_pointer(vec.head_pointer), end_pointer(vec.end_pointer)
	{
		vec.head_pointer = vec.end_pointer = nullptr;
	}

	//Move assignment operator
	template<typename T, typename Allocator>
	vector<T, Allocator> &vector<T, Allocator>::operator=(vector &&vec)
	{
		//check if vec is the vector itself
		if (this != &vec)
		{
			vec_size = vec.size();
			vec_capcity = vec.capcity();
			head_pointer = vec.head_pointer;
			end_pointer = vec.end_pointer;
			vec.head_pointer = vec.end_pointer = nullptr;
		}
		
	}

	//Create a vector with n nodes
	template<typename T, typename Allocator>
	vector<T, Allocator>::vector(const int num, const T &node = T())
		: vec_size(num), vec_capcity(num + EXTRA_SPACE)
	{
		head_pointer = alloc.allocate(vec_capcity);
		end_pointer = head_pointer;
		for (int i = 0; i != num; ++i)
		{
			alloc.construct(end_pointer++, node);
		}
	}

	//Create a vector with the elements of the range [beg, end)
	template<typename T, typename Allocator>
	vector<T, Allocator>::vector(iterator beg, iterator end)
		: vec_size(end - beg), vec_capcity(vec_size + EXTRA_SPACE)
	{
		head_pointer = alloc.allocate(vec_capcity);
		std::uninitialized_copy(beg, end, head_pointer);
		end_pointer = head_pointer + (end - beg);
	}

	//List constructor
	template<typename T, typename Allocator>
	vector<T, Allocator>::vector(std::initializer_list<T> lst)
		: vec_size(lst.end() - lst.begin()), vec_capcity(vec_size + EXTRA_SPACE)
	{
		head_pointer = alloc.allocate(vec_capcity);
		std::uninitialized_copy(lst.begin(), lst.end(), head_pointer);
		end_pointer = head_pointer + vec_size;
	}

	//Destructor
	template<typename T, typename Allocator>
	vector<T, Allocator>::~vector()
	{
		//Release the elements
		while (end_pointer != head_pointer)
			alloc.destroy(--end_pointer);
		//Release the memory
		alloc.deallocate(head_pointer, vec_capcity);
		//others
		head_pointer = end_pointer = nullptr;
		vec_size = vec_capcity = 0;
	}
	//
	//************************************************************************************************







	//returns the number of elements 
	template<typename T, typename Allocator>
	std::size_t vector<T, Allocator>::size() const
	{
		return vec_size;
	}

	//return the capcity of elements
	template<typename T, typename Allocator>
	std::size_t vector<T, Allocator>::capcity() const
	{
		return vec_capcity;
	}

	template<typename T, typename Allocator>
	T & vector<T, Allocator>::operator[](size_t i)
	{
		return *(head_pointer + i);
	}
	


}
#endif