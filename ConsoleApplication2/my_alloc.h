#ifndef __MY_ALLOC_H_
#define __MY_ALLOC_H_

#include<iostream>

namespace my_stl
{
	inline void throw_bad_alloc()
	{
		std::cerr << "out of memory" << std::endl; exit(1);
	}

	//一级内存配置器
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
				throw_bad_alloc();
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
				throw_bad_alloc();
			(*my_malloc_handler)();
			result = realloc(p, n);
			if (result)
				return result;
		}
	}

	typedef __malloc_alloc_template<0> malloc_alloc;


	//二级内存配置器
	enum {__ALIGN = 8}; //raised boundry of the size of small block
	enum {__MAX_BYTES = 128}; //limit to the size of small block
	enum {__NFREELISTS = __MAX_BYTES / __ALIGN}; //the number of free-lists

	template<bool threads, int inst>
	class __default_alloc_template
	{
	private:
		//raise to a mutiple of 8
		static size_t ROUND_UP(size_t bytes)
		{
			return ((bytes)+__ALIGN - 1) & ~(__ALIGN - 1);
		}

		//node of free-list, using union calss to save memory
		union obj
		{
			union obj * free_list_link;
			char client_data[1];
		};

		//free-lists
		static obj * volatile free_list[__NFREELISTS];
		
		//根据区块大小选择使用第n号free-list
		static size_t FREELIST_INDEX(size_t bytes)
		{
			return (((bytes)+__ALIGN - 1) / __ALIGN - 1);
		}
		
		//返回一个大小为n的对象，并可能加入大小为n的其他区块到free-list
		static void *refill(size_t n)
		{
			int nobjs = 20;
			char *chunk = chunk_alloc(n, nobjs);
			obj * volatile *my_free_list;
			obj *result;
			obj * current_obj, *next_obj;
			int i;

			if (nobjs == 1)
				return chunk;
			my_free_list = free_list + FREELIST_INDEX(n);

			//create free list in chunk space
			result = (obj*)chunk;
			*my_free_list = next_obj = (obj *)(chunk + n);
			for (i = 1;; ++i)
			{
				current_obj = next_obj;
				next_obj = (obj *)((char *)next_obj + n);
				if (nobjs - 1 == i)
				{
					current_obj->free_list_link = 0;
					break;
				}
				else
				{
					current_obj->free_list_link = next_obj;
				}
			}
			return result;
		}

		//配置一大块空间，可容纳nobjs个大小为”size"的区块
		static char *chunk_alloc(size_t size, int &nobjs)
		{
			char *result;
			size_t total_bytes = size * nobjs;
			size_t bytes_left = end_free - start_free;

			if (bytes_left >= total_bytes)
			{
				//内存池剩余空间完全满足需求
				result = start_free;
				start_free += total_bytes;
				return result;
			}
			else if (bytes_left >= size)
			{
				//内存池剩余空间不能完全满足需求，但足够供应一个以上的区块
				nobjs = bytes_left / size;
				total_bytes = size * nobjs;
				result = start_free;
				start_free += total_bytes;
				return result;
			}
			else
			{
				//内存池剩余空间完全不能完全满足需求，不足够供应一个区块
				size_t bytes_to_get = 2 * total_bytes + ROUND_UP(heap_size >> 4);
				if (bytes_left > 0)
				{
					obj * volatile * my_free_list = free_list + FREELIST_INDEX(bytes_left);
					((obj *)start_free)->free_list_link = *my_free_list;
					*my_free_list = (obj *)start_free;
				}
				start_free = (char *)malloc(bytes_to_get);
				if (start_free == 0)
				{
					start_free = (char *)malloc_alloc::allocate(bytes_to_get);
				}
				heap_size += bytes_to_get;
				end_free = start_free + bytes_to_get;
				return chunk_alloc(size, nobjs);
			}
		}

		//Chunk allocation state
		static char *start_free; //内存池起始位置
		static char *end_free; //内存池结束位置
		static size_t heap_size;

	public:
		static void *allocate(size_t n)
		{
			obj * volatile *my_free_list;
			obj * result;

			//if size is bigger than 128
			if (n > static_cast<size_t>(__MAX_BYTES))
				return malloc_alloc::allocate(n);

			my_free_list = free_list + FREELIST_INDEX(n);
			result = *my_free_list;
			if (result == 0)
			{
				void *r = refill(ROUND_UP(n));
				return r;
			}

			*my_free_list = result->free_list_link;
			return result;
		}

		static void deallocate(void *p, size_t n)
		{
			obj *q = (obj *)p;
			obj * volatile *my_free_list;
			
			//if n is bigger than __MAX_BYTES
			if (n > static_cast<size_t>(__MAX_BYTES))
			{
				malloc_alloc::deallocate(p, n);
				return;
			}

			my_free_list = free_list + FREELIST_INDEX(n);
			q->free_list_link = *my_free_list;
			*my_free_list = q;
		}
		static void *reallocate(void *p, size_t old_sz, size_t new_sz);


	};

	template<bool threads, int inst>
	char *__default_alloc_template<threads, inst>::start_free = 0;

	template<bool threads, int inst>
	char *__default_alloc_template<threads, inst>::end_free = 0;

	template<bool threads, int inst>
	size_t __default_alloc_template<threads, inst>::heap_size = 0;

	template<bool threads, int inst>
	typename __default_alloc_template<threads, inst>::obj * volatile __default_alloc_template<threads, inst>::free_list[__NFREELISTS] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, };

	typedef __default_alloc_template<0, 0> default_alloc;
}


#endif
