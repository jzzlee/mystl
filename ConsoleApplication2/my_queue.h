#ifndef MY_QUEUE_H_
#define MY_QUEUE_H_

#include "my_list.h"

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

}

#endif