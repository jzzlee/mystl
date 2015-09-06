#ifndef __MY_ALLOC_H_
#define __MY_ALLOC_H_

#include<iostream>

namespace my_stl
{
	//Ò»¼¶ÄÚ´æÅäÖÃÆ÷
	template<int inst>
	class __malloc_alloc_template
	{
	private:
		static void *oom_malloc(size_t); //alloc when out of memory
		static void *oom_realloc(void *, size_t);//realloc when out of memory
		static void(*__malloc_alloc_oom_handler)(); //handler when out of memory

	public:
		static void * allocate(size_t n) 
		{
			void *result = malloc(n); //use malloc directly
			if (result == 0)
				result = oom_malloc(n);
			return result;
		}

		static void deallocate(void *p, size_t n)
		{
			free(p); //use free directly
		}

		static void * reallocate(void *p, size_t n, size_t new_sz)
		{
			void result = realloc(p, new_sz); //call realloc directly
			if (result == 0)
				result = oom_realloc(p, new_sz);
			return result;
		}

		static void(*set_malloc_handler(void(*f) ()))()
		{
			void(*old)() = __malloc_alloc_oom_handler;
			__malloc_alloc_oom_handler = f;
			return (old);
		}
	};

	template<int inst>
	void(*__malloc_alloc_template<inst>::__malloc_alloc_oom_handler)() = 0;

	template<int inst>
	void * __malloc_alloc_template<inst>::oom_malloc(size_t n)
	{
		void(*my_malloc_handler)();
		void *result;
		for (;;)
		{
			my_malloc_handler = __malloc_alloc_oom_handler;
			if (my_malloc_handler == 0)
				_THROW_BAD_ALLOC;
			(*my_malloc_handler)();
			result = malloc(n);
			if (result)
				return result;
		}
	}

	template<int inst>
	void * __malloc_alloc_template<inst>::oom_realloc(void *p, size_t n)
	{
		void(*my_malloc_handler)();
		void *result;

		for (;;)
		{
			my_malloc_handler = __malloc_alloc_oom_handler;
			if (my_malloc_handler == 0)
				_THROW_BAD_ALLOC;
			(*my_malloc_handler)();
			result = realloc(p, n);
			if (result)
				return result;
		}
	}

	typedef typename __malloc_alloc_template<0> malloc_alloc;
}


#endif
