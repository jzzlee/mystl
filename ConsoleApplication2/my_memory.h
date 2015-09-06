#ifndef __MY_MEMORY_H_
#define __MY_MEMORY_H_
#include <new.h>
#include <limits>
#include "my_alloc.h"

namespace my_stl
{
	template<typename T, typename Alloc = malloc_alloc>
	class allocator
	{
	public:
		typedef typename T value_type;
		typedef typename T* pointer;
		typedef typename const T* const_pointer;
		typedef typename T& reference;
		typedef typename const T& const_reference;
		typedef typename size_t size_type;
		typedef typename ptrdiff_t difference_type;


		//四个函数只是简单的转调用，调用传递给配置器（可能是一级或二级配置器）的成员函数。
		T * allocate(size_t n)
		{
			return n == 0 ? nullptr : (T*)Alloc::allocate(n * sizeof(T));
		}
		
		T * allocate(void)
		{
			return (T*)Alloc::allocate(sizeof(T));
		}

		void deallocate(T *p, size_t n)
		{
			if (p != 0)
				Alloc::deallocate(p, n * sizeof(T));
		}

		void deallocate(T *p)
		{
			if (p != 0)
				Alloc::deallocate(p, sizeof(T));
		}

		//obtains the address of an object
		pointer address(reference x)
		{
			return (pointer)&x;
		}

		const_pointer const_address(const_reference x)
		{
			return (const_pointer)&x;
		}

		//Returns the maximum theoretically possible value of n, for which the call allocate(n, 0) could succeed.
		size_type max_size() const
		{
			return std::numeric_limits<size_type>::max()
		}

		void construct(pointer p, const_reference value)
		{
			new((void *)p) T(value);    //placement new
		}

		void destroy(pointer p)
		{
			((T*)p)->~T();  //call destructor of class T
		}
	};
	
}



#endif