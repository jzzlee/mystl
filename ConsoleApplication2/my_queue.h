#ifndef MY_QUEUE_H_
#define MY_QUEUE_H_

#include "my_list.h"
#include "my_vector.h"
#include "my_heap.h"

namespace my_stl
{

	template< class T, class Container > class queue;

	template< class T, class Container >
	bool operator==(queue<T, Container>& lhs, queue<T, Container>& rhs);

	template< class T, class Container >
	bool operator<(queue<T, Container>& lhs, queue<T, Container>& rhs);



	template< class T, class Container =list<T> >
	class queue
	{
	public:
		typedef typename Container::value_type value_type;
		typedef typename Container::size_type size_type;
		typedef typename Container::reference reference;
		typedef typename Container::const_reference const_reference;
		typedef Container container_type;

		template< class TT, class Cont >
		friend 	bool operator==(queue<TT, Cont>& lhs, queue<TT, Cont>& rhs);

		template< class TT, class Cont >
		friend 	bool operator<(queue<TT, Cont>& lhs, queue<TT, Cont>& rhs);

		explicit queue(const Container& cont) : c(cont) {}
		explicit queue(Container&& cont = Container()) : c(std::move(cont)) {}
		queue(const queue& other) : c(other.c) {}
		queue(queue&& other) : c(std::move(other.c)) {}

		template<typename Alloc>
		explicit queue(const Alloc& alloc) : c(alloc) {}

		template< class Alloc >
		queue(const Container& cont, const Alloc& alloc) : c(cont, alloc) {}

		template< class Alloc >
		queue(Container&& cont, const Alloc& alloc) : c(std::move(cont), alloc) {}

		template< class Alloc >
		queue(const queue& other, const Alloc& alloc) : c(other.c, alloc) {}

		template< class Alloc >
		queue(queue&& other, const Alloc& alloc) : c(std::move(other.c), alloc) {}

		~queue() { c.~Container(); }

		queue operator=(const queue& other) { c = other.c; }
		queue operator=(queue&& other) { c = std::move(other.c); }

		reference front() { return c.front(); }
		const_reference front() const { return c.front(); }

		reference back() { return c.back(); }
		const_reference back() const { return c.back(); }

		size_type size() { return c.size(); }
		bool empty() { return c.empty(); }

		void push(const T &value) { c.push_back(); }
		void push(T &&value) { c.push_back(std::move(value)); }

		template< class... Args >
		void emplace(Args&&... args) { c.emplace(std::forward<Args>(args)...); }
		void pop() { c.pop_front(); }

		void swap(queue& other) { c.swap(other.c); }

	protected:
		Container c;
	};

	template< class T, class Container >
	bool operator==(queue<T, Container>& lhs, queue<T, Container>& rhs)
	{
		return lhs.c == rhs.c;
	}

	template< class T, class Container >
	bool operator<(queue<T, Container>& lhs, queue<T, Container>& rhs)
	{
		return lhs.c < rhs.c;
	}

	template< class T, class Container >
	bool operator!=(queue<T, Container>& lhs, queue<T, Container>& rhs)
	{
		return !(lhs == rhs);
	}

	template< class T, class Container >
	bool operator>(queue<T, Container>& lhs, queue<T, Container>& rhs)
	{
		return rhs < lhs;
	}

	template< class T, class Container >
	bool operator>=(queue<T, Container>& lhs, queue<T, Container>& rhs)
	{
		return !(lhs < rhs);
	}

	template< class T, class Container >
	bool operator<=(queue<T, Container>& lhs, queue<T, Container>& rhs)
	{
		return !(rhs < lhs);
	}

	template< class T, class Container >
	void swap(queue<T, Container>& lhs, queue<T, Container>& rhs)
	{
		std::swap(lhs, rhs);
	}


	//priority_queue

	template<class T, class Container = my_stl::vector<T>, class Compare = std::less<typename Container::value_type>>
	class priority_queue
	{
	public:
		typedef typename Container container_type;
		typedef typename Container::value_type value_type;
		typedef typename Container::size_type size_type;
		typedef typename Container::reference reference;
		typedef typename Container::const_reference const_reference;

		priority_queue(const Compare& compare, const Container& cont) : c(cont), comp(compare) { my_stl::make_heap(c.begin(), c.end(), comp); }

		explicit priority_queue(const Compare& compare = Compare(),
			Container&& cont = Container()) : c(std::move(cont)), comp(std::move(compare)) {
			my_stl::make_heap(c.begin(), c.end(), comp);
		}

		priority_queue(const priority_queue& other) : c(other.c), comp(std::move(other.comp)) {}

		priority_queue(priority_queue&& other) : c(std::move(other.c)), comp(std::move(other.comp)) {}

		template< class Alloc >
		explicit priority_queue(const Alloc& alloc) : c(alloc), comp() {}

		template< class Alloc >
		priority_queue(const Compare& compare, const Alloc& alloc) : c(alloc), comp(compare) {}

		template< class Alloc >
		priority_queue(const Compare& compare, const Container& cont, const Alloc& alloc) : c(cont, alloc), comp(compare) { my_stl::make_heap(c.begin(), c.end(), comp); }

		template< class Alloc >
		priority_queue(const Compare& compare, Container&& cont, const Alloc& alloc) : c(std::move(cont), alloc), comp(compare) { my_stl::make_heap(c.begin(), c.end(), comp); }

		template< class Alloc >
		priority_queue(const priority_queue& other, const Alloc& alloc) : c(other.c, alloc), comp(other.comp) {}

		template< class Alloc >
		priority_queue(priority_queue&& other, const Alloc& alloc) : c(std::move(other.c), alloc), comp(std::move(other.comp)) {}

		template< class InputIt >
		priority_queue(InputIt first, InputIt last, const Compare& compare, const Container& cont) : c(cont), comp(compare)
		{
			c.insert(c.end(), first, last);
			my_stl::make_heap(c.begin(), c.end(), comp);
		}

		template< class InputIt >
		priority_queue(InputIt first, InputIt last, const Compare& compare = Compare(), Container&& cont = Container())
			:c(std::move(cont)), comp(std::move(compare))
		{
			c.insert(c.end(), first, last);
			my_stl::make_heap(c.begin(), c.end(), comp);
		}

		priority_queue operator=(const priority_queue& other){ c = other.c; return *this; }
		priority_queue operator=(priority_queue&& other) { c = std::move(other.c); return *this; }

		const_reference top() const { return c.front(); }
		bool empty() const { return c.empty(); }
		size_type size() const { return c.size(); }

		void push(const T& value)
		{
			c.push_back(value);
			my_stl::push_heap(c.begin(), c.end(), comp);
		}
		void push(T&& value)
		{
			c.push_back(std::move(value));
			my_stl::push_heap(c.begin(), c.end(), comp);
		}

		template< class... Args >
		void emplace(Args&&... args)
		{
			c.emplace_back(std::forward<Args>(args)...);
			my_stl::push_heap(c.begin(), c.end(), comp);
		}

		void pop()
		{
			my_stl::pop_heap(c.begin(), c.end(), comp);
			c.pop_back();
		}

		void swap(priority_queue& other)
		{
			my_stl::swap(c, other.c);
			std::swap(comp, other.comp);
		}
	protected:
		Container c;
		Compare comp;
	};

	template< class T, class Container, class Compare >
	void swap(priority_queue<T, Container, Compare>& lhs,
		priority_queue<T, Container, Compare>& rhs)
	{
		lhs.swap(rhs);
	}
}

#endif