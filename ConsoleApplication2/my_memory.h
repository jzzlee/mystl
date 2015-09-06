#ifndef __MY_MEMORY_H_
#define __MY_MEMORY_H_
#include <new.h>
#include "my_construct.h"
#include "my_alloc.h"

namespace my_stl
{
	template<typename T, typename Alloc>
	class allocater
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
		static T * allocate(size_t n)
		{
			return n == 0 ? nullptr : (T*)Alloc::allocate(n * sizeof(T));
		}
		
		static T * allocate(void)
		{
			return (T*)Alloc::allocate(sizeof(T));
		}

		static void deallocate(T *p, size_t n)
		{
			if (p != 0)
				Alloc::deallocate(p, n * sizeof(T));
		}

		static void deallocate(T *p)
		{
			if (p != 0)
				Alloc::deallocate(p, sizeof(T));
		}
	};
	
}



#endif