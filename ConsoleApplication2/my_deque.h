#ifndef __MY_DEQUE_H_
#define __MY_DEQUE_H_

#include "my_memory.h"
#include "my_iterator.h"
#include "my_xutility.h"
#include <initializer_list>
#include <cstdio>
#include <iostream>
namespace my_stl
{
	//deque的迭代器
	template<typename T, typename Ref, typename Ptr>
	struct __deque_iterator
	{
		typedef	__deque_iterator<T, T&, T*>						iterator;
		typedef	__deque_iterator<T, const T&, const T*>			const_iterator;
		typedef	__deque_iterator<T, Ref, Ptr>					self;
		typedef	random_access_iterator_tag						iterator_category;
		typedef	T								value_type;
		typedef Ref								reference;
		typedef Ptr								pointer;
		typedef	std::size_t						size_type;
		typedef ptrdiff_t						difference_type;
		typedef T**								map_pointer;

		static std::size_t buffer_size(){ return __deque_buf_size(sizeof(T)); }

		T *cur;	//迭代器所指缓冲区内指向的元素
		T *first;//迭代器所指缓冲区的头
		T *last;//迭代器所指缓冲区的尾后元素
		map_pointer node;//指向中控器

		static inline size_type __deque_buf_size(size_type sz)
		{
			return sz < 512 ? size_type(512 / sz) : size_type(1);
		}

		void set_node(map_pointer new_node)
		{
			node = new_node;
			first = *new_node;
			last = first + difference_type(buffer_size());
		}

		//constructor
		__deque_iterator(T *cur, T *first, T *last, map_pointer nd) : cur(cur), first(first), last(last), node(nd){}
		__deque_iterator() : cur(nullptr), first(nullptr), last(nullptr), node(0) {}
		__deque_iterator(const iterator &x) : cur(x.cur), first(x.first), last(x.last), node(x.node) {}
		__deque_iterator(const const_iterator &x) : cur(x.cur), first(x.first), last(x.last), node(x.node) {}

		bool operator==(const self &x) const { return cur == x.cur; }
		bool operator!=(const self &x) const { return !(*this == x); }
		bool operator<(const self &x) const { return (node == x.node) ? (cur < x.cur) : (node < x.node); }
		reference operator*() const { return *cur; }
		pointer operator->() const { return &(operator*()); }

		self &operator=(const self &x)
		{
			cur = x.cur; first = x.first; last = x.last; node = x.node;
			return *this;
		}

		self operator++()
		{
			++cur;
			if (cur == last) //cur指向当前缓冲区末尾元素，需要调整到下一个缓冲区
			{
				set_node(node + 1);
				cur = first;
			}
			return *this;
		}
		self operator++(int)
		{
			self tmp = *this;
			++(*this);
			return tmp;
		}
		self operator--()
		{
			if (cur == first) //cur指向当前缓冲区起始位置，需要调整到上一个缓冲区
			{
				set_node(node - 1);
				cur = last;
			}
			--cur;
			return *this;
		}
		self operator--(int)
		{
			self tmp = *this;
			--(*this);
			return tmp;
		}

		difference_type operator-(const self &other) const
		{
			return (node - other.node - 1) * buffer_size() + (cur - first) + (other.last - other.cur);
		}

		self &operator+=(difference_type n)
		{
			difference_type offset = n + (cur - first);
			if (offset >= 0 && offset < difference_type(buffer_size()))
				cur += n;
			else
			{
				difference_type node_offset =
					offset > 0 ? offset / buffer_size() : -((-offset - 1) / buffer_size()) - 1;
				set_node(node + node_offset);
				cur = first + (offset - node_offset * buffer_size());
			}
			return *this;
		}
		self &operator-=(difference_type n)
		{
			*this += -n;
			return *this;
		}
		self operator+(difference_type n) const
		{
			self tmp = *this;
			tmp += n;
			return tmp;
		}
		self operator-(difference_type n) const
		{
			self tmp = *this;
			tmp -= n;
			return tmp;
		}

		reference operator[](difference_type n) const { return *(*this + n); }

	};

	//deque的反向迭代器
	template<typename T, typename Ref, typename Ptr>
	struct __deque_reverse_iterator
	{
		typedef	__deque_reverse_iterator<T, T&, T*>						reverse_iterator;
		typedef	__deque_reverse_iterator<T, const T&, const T*>			const_reverse_iterator;
		typedef	__deque_reverse_iterator<T, Ref, Ptr>					self;
		typedef	random_access_iterator_tag								reverse_iterator_category;
		typedef	T								value_type;
		typedef Ref								reference;
		typedef Ptr								pointer;
		typedef	std::size_t						size_type;
		typedef ptrdiff_t						difference_type;
		typedef T**								map_pointer;

		static std::size_t buffer_size(){ return __deque_buf_size(512, sizeof(T)); }

		T *cur;	//迭代器所指缓冲区内指向的元素
		T *rfirst;//迭代器所指缓冲区的尾
		T *rlast;//迭代器所指缓冲区的首前元素
		map_pointer node;//指向中控器

		inline size_type __deque_buf_size(size_type sz)
		{
			return sz < 512 ? size_type(512 / sz) : size_type(1);
		}

		void set_node(map_pointer new_node)
		{
			node = new_node;
			rlast = *new_node - 1;
			rfirst = rlast + difference_type(buffer_size());
		}

		//constructor
		__deque_reverse_iterator(T *cur, T *rfirst, T *rlast, map_pointer nd) : cur(cur), rfirst(rfirst), rlast(rlast), node(nd){}
		__deque_reverse_iterator() : cur(nullptr), rfirst(nullptr), rlast(nullptr), node(0) {}
		__deque_reverse_iterator(const reverse_iterator &x) : cur(x.cur), rfirst(x.rfirst), rlast(x.rlast), node(x.node) {}
		__deque_reverse_iterator(const const_reverse_iterator &x) : cur(x.cur), rfirst(x.rfirst), rlast(x.rlast), node(x.node) {}

		bool operator==(const self &x) const { return cur == x.cur; }
		bool operator!=(const self &x) const { return !(*this == x); }
		reference operator*() const { return *cur; }
		pointer operator->() const { return &(operator*()); }

		self &operator=(const self &x)
		{
			cur = x.cur; rfirst = x.rfirst; rlast = x.rlast; node = x.node;
			return *this;
		}

		self operator++()
		{
			--cur;
			if (cur == rlast) //cur指向当前缓冲区首前位置，需要调整到上一个缓冲区
			{
				set_node(node - 1);
				cur = rfirst;
			}
			return *this;
		}
		self operator++(int)
		{
			self tmp = *this;
			++(*this);
			return tmp;
		}
		self operator--()
		{
			if (cur == rfirst) //cur指向当前缓冲区起始位置，需要调整到上一个缓冲区
			{
				set_node(node + 1);
				cur = rlast;
			}
			++cur;
			return *this;
		}
		self operator--(int)
		{
			self tmp = *this;
			--(*this);
			return tmp;
		}

		difference_type operator-(const self &other) const
		{
			return (other.node - node - 1) * buffer_size() + (rfirst - cur) + (other.cur - other.rlast);
		}

		self &operator+=(difference_type n)
		{
			difference_type offset = n + (first - cur);
			if (offset >= 0 && offset < difference_type(buffer_size()))
				cur -= n;
			else
			{
				difference_type node_offset =
					offset > 0 ? offset / buffer_size() : -((-offset - 1) / buffer_size()) - 1;
				set_node(node + node_offset);
				cur = rlast + ((node_offset + 1) * buffer_size() - offset);
			}
			return *this;
		}
		self &operator-=(difference_type n)
		{
			*this += -n;
			return *this;
		}

		self operator+(difference_type n) const
		{
			self tmp = *this;
			tmp += n;
			return tmp;
		}
		self operator-(difference_type n) const
		{
			self tmp = *this;
			tmp -= n;
			return tmp;
		}

		reference operator[](difference_type n) const { return *(*this + n); }
	};

	template<class T, class Allocator = allocator<T> >
	class deque
	{
	public:
		typedef deque<T, Allocator>												Myt;
		typedef T																value_type;
		typedef Allocator														allocator_type;
		typedef ptrdiff_t														difference_type;
		typedef value_type&														reference;
		typedef const value_type&												const_reference;
		typedef typename Allocator::pointer										pointer;
		typedef typename Allocator::const_pointer								const_pointer;
		typedef __deque_iterator<value_type, reference, pointer>				iterator;
		typedef __deque_iterator<value_type, const_reference, const_pointer>	const_iterator;
		typedef __deque_reverse_iterator<value_type, reference, pointer>		reverse_iterator;
		typedef __deque_reverse_iterator<value_type, const_reference, const_pointer>	const_reverse_iterator;
		typedef std::size_t														size_type;
		typedef pointer*														map_pointer;
		typedef typename __type_traits<T>::is_POD_type							is_POD;

		//////////////////////////////////////////
		//Constructor
		/////////////////////////////////////////////

		explicit deque(const Allocator& alloc = Allocator()) : alloc(alloc), map(0), map_size(0), start(), finish(), rstart(), rfinish() {}

		deque(size_type count, const T& value, const Allocator& alloc = Allocator())
			:alloc(alloc)
		{
			__map_initialize(count);
			__fill_initialize(count, value);
		}
		deque(int count, const T& value, const Allocator& alloc = Allocator())
			:alloc(alloc)
		{
			__map_initialize(count);
			__fill_initialize(count, value);
		}
		deque(long count, const T& value, const Allocator& alloc = Allocator())
			:alloc(alloc)
		{
			__map_initialize(count);
			__fill_initialize(count, value);
		}

		explicit deque(size_type count)
		{
			__map_initialize(count);
			__fill_initialize(count, value_type());
		}

		template< class InputIt >
		deque(InputIt first, InputIt last, const Allocator& alloc = Allocator())
			:alloc(alloc)
		{
			__map_initialize(my_stl::distance(first, last));
			__copy_initialize(first, last);
		}

		//Copy constructor.
		deque(const deque& other)
			:alloc(other.alloc)
		{
			if (other.map_size != 0)
			{
				__map_initialize(my_stl::distance(other.begin(), other.end()));
				__copy_initialize(other.begin(), other.end());
			}
			else
			{
				map = map_pointer(); map_size = 0;
				start = finish = iterator();
				rstart = rfinish = reverse_iterator();
			}
		}
		deque(const deque& other, const Allocator& alloc)
			:alloc(alloc)
		{
			if (other.map_size != 0)
			{
				__map_initialize(my_stl::distance(other.begin(), other.end()));
				__copy_initialize(other.begin(), other.end());
			}
			else
			{
				map = map_pointer(); map_size = 0;
				start = finish = iterator();
				rstart = rfinish = reverse_iterator();
			}
		}

		//Move constructor
		deque(deque&& other)
		{
			if (alloc == other.get_allocator())
			{
				start = other.start; finish = other.finish;
				rstart = other.rstart; rfinish = other.rfinish;
				map = other.map; map_size = other.map_size;
			}
			else
			{
				//allocator类型不一致，需要逐个元素复制
				if (other.map_size != 0)
				{
					__map_initialize(my_stl::distance(other.begin(), other.end()));
					__copy_initialize(other.begin(), other.end());
				}
				else
				{
					map = map_pointer(); map_size = 0;
					start = finish = iterator();
					rstart = rfinish = reverse_iterator();
				}
				other.__destroy_and_destruct();
			}
			other.start = other.finish = iterator();
			other.rstart = other.rfinish = reverse_iterator();
			other.map = map_pointer(0); other.map_size = 0;
		}

		deque(deque&& other, const Allocator& allc)
			:alloc(allc)
		{
			if (alloc == other.get_allocator())
			{
				start = other.start; finish = other.finish;
				rstart = other.rstart; rfinish = other.rfinish;
				map = other.map; map_size = other.map_size;
			}
			else
			{
				//allocator类型不一致，需要逐个元素复制
				if (other.map_size != 0)
				{
					__map_initialize(my_stl::distance(other.begin(), other.end()));
					__copy_initialize(other.begin(), other.end());
				}
				else
				{
					map = map_pointer(); map_size = 0;
					start = finish = iterator();
					rstart = rfinish = reverse_iterator();
				}
				other.__destroy_and_destruct();
			}
			other.start = other.finish = nullptr;
			other.rstart = other.rfinish = nullptr;
			other.map = nullptr; other.map_size = 0;
		}

		deque(std::initializer_list<T> init,
			const Allocator& alloc = Allocator())
			:alloc(alloc)
		{
			//allocator类型不一致，需要逐个元素复制
			if (init.size() != 0)
			{
				__map_initialize(my_stl::distance(init.begin(), init.end()));
				__copy_initialize(init.begin(), init.end());
			}
			else
			{
				map = map_pointer(); map_size = 0;
				start = finish = iterator();
				rstart = rfinish = reverse_iterator();
			}
		}

		//Destructor
		~deque()
		{
			__destroy_and_destruct();
		}

		deque& operator=(const deque& other)
		{
			if (map_size < other.map_size)
			{
				;
			}
			return *this;
		}

		allocator_type get_allocator() { return alloc; }

		iterator begin() { return start; }
		iterator end() { return finish; }
		const_iterator begin() const { return start; }
		const_iterator end() const { return finish; }
		const_iterator cbegin() const { return ((const Myt *)this)->begin(); }
		const_iterator cend() const { return ((const Myt*)this)->end(); }
		reverse_iterator rbegin() { return rstart; }
		reverse_iterator rend() { return rfinish; }
		const_reverse_iterator rbegin() const { return rstart; }
		const_reverse_iterator rend() const { return rfinish; }
		const_reverse_iterator crbegin() const { return ((const Myt*)this)->rbegin(); }
		const_reverse_iterator crend() const { return ((const Myt*)this)->rend(); }
		reference operator[](size_type n){ return start[difference_type(n)]; }
		const_reference operator[](size_type n) const { return start[difference_type(n)]; }
		reference front(){ return *start; }
		const_reference front() const { return *start; }
		reference back()
		{
			iterator tmp = finish;
			--tmp;
			return *tmp;
		}
		const_reference back() const
		{
			const_iterator tmp = finish;
			--tmp;
			return *tmp;
		}
		size_type size() const { return (start == finish ? 0 : finish - start); }
		size_type max_size() const { return std::numeric_limits<size_type>::max(); }
		bool empty() { return start == finish; }

	private:
		iterator start, finish; // 首、尾迭代器
		reverse_iterator rstart, rfinish; //首尾反向迭代器
		map_pointer map;	//指向中控单元
		size_type map_size; //map的大小
		allocator_type alloc;    //内存配置器
		allocator<T*> alloc_map;

		void __map_initialize(size_type n)
		{
			//计算需要几个缓冲区
			size_type space_size = iterator::buffer_size();
			map_size = (n + space_size - 1) / space_size;
			//申请中控器空间
			map = alloc_map.allocate(map_size);
			//申请缓存区空间、构造中控器各元素
			for (int i = 0; i != map_size; ++i)
				alloc_map.construct(&(map[i]), alloc.allocate(space_size));
			//设置start迭代器的值
			start.cur = start.first = map[0];
			start.last = start.first + space_size;
			start.node = &(map[0]);
			//设置rfinish反向迭代器的值
			rfinish.cur = rfinish.rlast = start.first - 1;
			rfinish.rfirst = rfinish.rlast + space_size;
			rfinish.node = &(map[0]);

			difference_type offset = n % space_size;
			//设置finish迭代器的值
			finish.first = map[map_size - 1];
			finish.last = finish.first + space_size;
			finish.cur = finish.first + offset;
			finish.node = &(map[map_size - 1]);
			//设置rtart迭代器的值
			rstart.rlast = finish.first - 1;
			rstart.rfirst = rstart.rlast + space_size;
			rstart.cur = rstart.rlast + offset;
			rstart.node = &(map[map_size - 1]);
		}

		void __fill_initialize(size_type n, const value_type &value)
		{
			my_stl::uninitialized_fill_n(start, n, value);
		}

		template<typename ForwardIterator>
		void __copy_initialize(ForwardIterator first, ForwardIterator last)
		{
			my_stl::uninitialized_copy(first, last, start);
		}

		void __destroy_and_destruct()
		{
			difference_type space_size = iterator::buffer_size();
			for (size_type i = 0; i != map_size; ++i)
			{
				//Release the elements
				pointer fin = map[i];
				for (difference_type j = 0; j != space_size; ++j)
					alloc.destroy(fin++);
				//Release the memory
				if (map[i])
					alloc.deallocate(map[i], space_size);
				alloc_map.destroy(&(map[i]));
			}
			if (map)
				alloc_map.deallocate(map, map_size);
		}



	};
}
#endif