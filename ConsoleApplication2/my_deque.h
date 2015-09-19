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
	//deque�ĵ�����
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

		T *cur;	//��������ָ��������ָ���Ԫ��
		T *first;//��������ָ��������ͷ
		T *last;//��������ָ��������β��Ԫ��
		map_pointer node;//ָ���п���

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
			if (cur == last) //curָ��ǰ������ĩβԪ�أ���Ҫ��������һ��������
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
			if (cur == first) //curָ��ǰ��������ʼλ�ã���Ҫ��������һ��������
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
					offset > 0 ? offset / buffer_size() : -((-offset - 1) / difference_type(buffer_size())) - 1;
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

	//deque�ķ��������
	template<typename T, typename Ref, typename Ptr>
	struct __deque_reverse_iterator
	{
		
		typedef	__deque_reverse_iterator<T, T&, T*>						reverse_iterator;
		typedef	__deque_reverse_iterator<T, const T&, const T*>			const_reverse_iterator;
		typedef __deque_iterator<T, Ref, Ptr>							iterator;
		typedef	__deque_iterator<T, const T&, const T*>					const_iterator;
		typedef	__deque_reverse_iterator<T, Ref, Ptr>					self;
		typedef	random_access_iterator_tag								reverse_iterator_category;
		typedef	T								value_type;
		typedef Ref								reference;
		typedef Ptr								pointer;
		typedef	std::size_t						size_type;
		typedef ptrdiff_t						difference_type;
		typedef T**								map_pointer;

		static std::size_t buffer_size(){ return __deque_buf_size(sizeof(T)); }

		T *cur;	//��������ָ��������ָ���Ԫ��
		T *rfirst;//��������ָ��������β
		T *rlast;//��������ָ����������ǰԪ��
		map_pointer node;//ָ���п���

		static inline size_type __deque_buf_size(size_type sz)
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
//		__deque_reverse_iterator(const iterator &x) : cur(x.cur), rfirst(x.last - 1), rlast(x.first - 1), node(x.node) {}
		__deque_reverse_iterator(const const_iterator &x) : cur(x.cur), rfirst(x.last - 1), rlast(x.first - 1), node(x.node) {}

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
			if (cur == rlast) //curָ��ǰ��������ǰλ�ã���Ҫ��������һ��������
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
			if (cur == rfirst) //curָ��ǰ��������ʼλ�ã���Ҫ��������һ��������
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

		///////////////////////////////////////////////////////////////////////////////////////////////////////
		//Member functions
		///////////////////////////////////////////////////////////////////////////////////////////////////////

		//Constructor
		explicit deque(const Allocator& alloc = Allocator()) : alloc(alloc), map(0), map_size(0), start(), finish() {}

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
			}
		}

		//Move constructor
		deque(deque&& other)
		{
			if (alloc == other.get_allocator())
			{
				start = other.start; finish = other.finish;
				map = other.map; map_size = other.map_size;
			}
			else
			{
				//allocator���Ͳ�һ�£���Ҫ���Ԫ�ظ���
				if (other.map_size != 0)
				{
					__map_initialize(my_stl::distance(other.begin(), other.end()));
					__copy_initialize(other.begin(), other.end());
				}
				else
				{
					map = map_pointer(); map_size = 0;
					start = finish = iterator();
				}
				other.__destroy_and_destruct();
			}
			other.start = other.finish = iterator();
			other.map = map_pointer(0); other.map_size = 0;
		}

		deque(deque&& other, const Allocator& allc)
			:alloc(allc)
		{
			if (alloc == other.get_allocator())
			{
				start = other.start; finish = other.finish;
				map = other.map; map_size = other.map_size;
			}
			else
			{

				//allocator���Ͳ�һ�£���Ҫ���Ԫ�ظ���
				if (other.map_size != 0)
				{
					__map_initialize(my_stl::distance(other.begin(), other.end()));
					__copy_initialize(other.begin(), other.end());
				}
				else
				{
					map = map_pointer(); map_size = 0;
					start = finish = iterator();
				}
				other.__destroy_and_destruct();
			}
			other.start = other.finish = nullptr;
			other.map = nullptr; other.map_size = 0;
		}

		deque(std::initializer_list<T> init,
			const Allocator& alloc = Allocator())
			:alloc(alloc)
		{
			//allocator���Ͳ�һ�£���Ҫ���Ԫ�ظ���
			if (init.size() != 0)
			{
				__map_initialize(my_stl::distance(init.begin(), init.end()));
				__copy_initialize(init.begin(), init.end());
			}
			else
			{
				map = map_pointer(); map_size = 0;
				start = finish = iterator();
			}
		}

		//Destructor
		~deque()
		{
			__destroy_and_destruct();
		}

		deque& operator=(const deque& other)
		{
			if (this != &other)
			{
				size_type other_size = other.size();
				size_type space_size = iterator::buffer_size(); //һ��������������
				size_type capacity = map_size * space_size; // ��ǰdeque���������
				if (size() >= other_size)
				{
					my_stl::copy(other.begin(), other.end(), start);
					iterator new_finish = start + other_size;
					//ɾ������Ԫ��
					while (finish != new_finish)
						alloc.destroy((--finish).cur);
					finish = new_finish;
				}
				else if (capacity > other_size)
				{
					copy(other.begin(), other.begin() + size(), start);
					my_stl::uninitialized_copy(other.begin() + size(), other.end(), finish);
					finish = start + other_size;
				}
				else if (capacity <= other_size)
				{
					//��������Դ��other������Ԫ�أ�����Ҫ��������map
					__alloc_new_map(other_size);
					my_stl::uninitialized_copy(other.begin(), other.end(), start);
					finish = start + other_size;
				}
			}
			return *this;
		}

		deque& operator=(deque&& other)
		{
			if (this != &other)
			{
				if (alloc == other.get_allocator())//allocator����һ�£�ֱ�Ӹ��ƿ�����Ϣ
				{
					start = other.start; finish = other.finish;
					map = other.map; map_size = other.map_size;
				}
				else
				{
					//allocator���Ͳ�һ�£���Ҫ���Ԫ�ظ���
					*this = other;
					other.__destroy_and_destruct();
				}
				other.start = other.finish = iterator();
				other.map = map_pointer(0); other.map_size = 0;
			}
			return *this;
		}

		deque& operator=(std::initializer_list<T> ilist)
		{
			size_type ilist_size = ilist.size();
			size_type space_size = iterator::buffer_size(); //һ��������������
			size_type capacity = map_size * space_size; // ��ǰdeque���������
			if (size() >= ilist_size)
			{
				my_stl::copy(ilist.begin(), ilist.end(), start);
				iterator new_finish = start + ilist_size;
				//ɾ������Ԫ��
				while (finish != new_finish)
					alloc.destroy((--finish).cur);
				finish = new_finish;
			}
			else if (capacity > ilist_size)
			{
				copy(ilist.begin(), ilist.begin() + size(), start);
				my_stl::uninitialized_copy(ilist.begin() + size(), ilist.end(), finish);
				finish = start + ilist_size;
			}
			else if (capacity <= ilist_size)
			{
				//��������Դ��ilist������Ԫ�أ�����Ҫ��������map
				__alloc_new_map(ilist_size);
				my_stl::uninitialized_copy(ilist.begin(), ilist.end(), start);
				finish = start + ilist_size;
			}
			return *this;
		}

		void assign(size_type count, const T& value)
		{
			size_type space_size = iterator::buffer_size(); //һ��������������
			size_type capacity = map_size * space_size; // ��ǰdeque���������
			if (size() >= count)
			{
				iterator new_finish = my_stl::fill_n(start, count, value);
				//ɾ������Ԫ��
				while (finish != new_finish)
					alloc.destroy((--finish).cur);
				finish = new_finish;
			}
			else if (capacity > count)
			{
				my_stl::fill_n(start, size(), value);
				finish = my_stl::uninitialized_fill_n(finish, count - size(), value);
			}
			else if (capacity <= count)
			{
				//��������Դ��count��Ԫ�أ�����Ҫ��������map
				__alloc_new_map(count);
				finish = my_stl::uninitialized_fill_n(start, count, value);
			}
		}

		void assign(int count, const T& value) { assign(size_type(count), value); }
		void assign(long count, const T& value) { assign(size_type(count), value); }

		template< class InputIt >
		void assign(InputIt first, InputIt last)
		{
			size_type count = my_stl::distance(first, last); //Ԫ��������
			size_type space_size = iterator::buffer_size(); //һ��������������
			size_type capacity = map_size * space_size; // ��ǰdeque���������
			if (size() >= count)
			{
				iterator new_finish = my_stl::copy(first, last, start);
				//ɾ������Ԫ��
				while (finish != new_finish)
					alloc.destroy((--finish).cur);
				finish = new_finish;
			}
			else if (capacity > count)
			{
				my_stl::copy(first, first + size(), start);
				finish = my_stl::uninitialized_copy(first + size(), last, finish);
			}
			else if (capacity <= count)
			{
				//��������Դ��count��Ԫ�أ�����Ҫ��������map
				__alloc_new_map(count);
				finish = my_stl::uninitialized_copy(first, last, start);
			}
		}

		void assign(std::initializer_list<T> ilist)
		{
			*this = ilist;
		}

		allocator_type get_allocator() { return alloc; }

		reference at(size_type pos)
		{
			if (pos < 0 || pos >= size())
				throw std::out_of_range("out of the range of the deque\n");
			return start[difference_type(pos)];
		}
		const_reference at(size_type pos) const
		{
			if (pos < 0 || pos >= size())
				throw std::out_of_range("out of the range of the deque\n");
			return start[difference_type(n)];
		}
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
		iterator begin() { return start; }
		iterator end() { return finish; }
		const_iterator begin() const { return start; }
		const_iterator end() const { return finish; }
		const_iterator cbegin() const { return ((const Myt *)this)->begin(); }
		const_iterator cend() const { return ((const Myt*)this)->end(); }
		//reverse_iterator rbegin() { return reverse_iterator(finish - 1); }
		//const_reverse_iterator rbegin() const { return const_reverse_iterator(finish - 1); }
		//const_reverse_iterator crbegin() const { return ((const Myt*)this)->rbegin(); }
		/*reverse_iterator rend() 
		{ 
			reverse_iterator tmp(start);
			--tmp.cur;
			return tmp; 
		}
		const_reverse_iterator rend() const
		{
			reverse_iterator tmp(start);
			--tmp.cur;
			return tmp;
		}
		const_reverse_iterator crend() const { return ((const Myt*)this)->rend(); }*/
		bool empty() { return start == finish; }
		size_type size() const { return (start == finish ? 0 : finish - start); }
		size_type max_size() const { return std::numeric_limits<size_type>::max(); }

		void shrink_to_fit()
		{
			if (map)
			{
				size_type space_size = iterator::buffer_size();
				//�ͷ���Щ��finish.node֮����пؽڵ�ָ��Ŀջ�����
				for (map_pointer p = finish.node + 1; p && *p && p != map + map_size; ++p)
				{
					alloc.deallocate(*p, space_size);
					*p = pointer();
				}
			}
		}

		void clear()
		{
			difference_type space_size = iterator::buffer_size();
			//����Ԫ��
			while (finish != start)
				alloc.destroy((--finish).cur);
		}

		iterator insert(const_iterator pos, const T& value)
		{

		}

		iterator insert(const_iterator pos, T&& value)
		{

		}

		iterator insert(const_iterator pos, size_type count, const T& value)
		{

		}
		iterator insert(const_iterator pos, int count, const T& value){ return insert(pos, size_type(count), value); }
		iterator insert(const_iterator pos, long count, const T& value) { return insert(pos, size_type(count), value); }

		template< class InputIt >
		iterator insert(const_iterator pos, InputIt first, InputIt last)
		{

		}

		iterator insert(const_iterator pos, std::initializer_list<T> ilist)
		{

		}

		


	private:
		iterator start, finish; // �ס�β������
//		reverse_iterator rstart, rfinish; //��β���������
		map_pointer map;	//ָ���пص�Ԫ
		size_type map_size; //map�Ĵ�С
		allocator_type alloc;    //�ڴ�������
		allocator<T*> alloc_map;

		void __map_initialize(size_type n)
		{
			//������Ҫ����������
			size_type space_size = iterator::buffer_size();
			map_size = n / space_size + 1;
			//�����п����ռ�
			map = alloc_map.allocate(map_size);
			//���뻺�����ռ䡢�����п�����Ԫ��
			for (int i = 0; i != map_size; ++i)
				alloc_map.construct(&(map[i]), alloc.allocate(space_size));
			//����start��������ֵ
			start.set_node(map);
			start.cur = start.first;
			difference_type offset = n % space_size;
			//����finish��������ֵ
			finish.set_node(map + map_size - 1);
			finish.cur = finish.first + offset;

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
			//������Щ���õĻ�����
			shrink_to_fit();
			//���� [start.node, finish.node)ָ��ָ����п��������п���ָ��Ļ�����
			for (map_pointer p = start.node; p != finish.node; ++p)
			{
				pointer fin = *p;
				for (difference_type j = 0; j != space_size; ++j)
					alloc.destroy(fin++);
				//Release the memory
				if (*p)
					alloc.deallocate(*p, space_size);
				alloc_map.destroy(p);
			}
			//���� finish.nodeָ��ָ����п��������п���ָ��Ļ�����
			if (finish.node)
			{
				for (pointer fin = *(finish.node); fin != finish.cur; ++fin)
					alloc.destroy(fin);
				//Release the memory
				if (*(finish.node))
					alloc.deallocate(*(finish.node), space_size);
				alloc_map.destroy(finish.node);
			}

			if (map)
				alloc_map.deallocate(map, map_size);
		}

		void __alloc_new_map(size_type n)
		{
			//������Ҫ����������
			size_type space_size = iterator::buffer_size();
			size_type need_map_size = n / space_size + 1;
			size_type new_map_size = std::max(need_map_size + 2, 2 * map_size);
			//�ͷ�ԭ�пռ�
			__destroy_and_destruct();
			//�����п����ռ�
			map = alloc_map.allocate(new_map_size);
			map_size = new_map_size;
			//���뻺�����ռ䡢�����п�����Ԫ��
			for (int i = 0; i != need_map_size; ++i)
				alloc_map.construct(&(map[i]), alloc.allocate(space_size));
			//�����п�������Ϊ��ָ��
			for (int i = need_map_size; i != map_size; ++i)
				alloc_map.construct(&(map[i]), pointer());
			//����start��������ֵ
			start.set_node(map);
			start.cur = start.first;


			difference_type offset = n % space_size;
			//����finish��������ֵ
			finish.set_node(map + need_map_size - 1);
			finish.cur = finish.first + offset;

		}

		

	};
}
#endif