#ifndef MY_STACK_H_
#define MY_STACK_H_

#include "my_vector.h"

namespace my_stl
{
	template<typename T, typename Container>  class stack;

	template< class T, class Container >
	bool operator==(stack<T, Container>& lhs, stack<T, Container>& rhs);

	template< class T, class Container >
	bool operator<(stack<T, Container>& lhs, stack<T, Container>& rhs);

	template<typename T, typename Container = vector<T> >
	class stack
	{
	public:
		typedef typename Container::value_type	value_type;
		typedef typename Container::size_type	size_type;
		typedef typename Container::reference	reference;
		typedef typename Container::const_reference	const_reference;
		typedef	Container	container_type;

		template< class TT, class Cont >
		friend bool operator==(stack<TT, Cont>& lhs, stack<TT, Cont>& rhs);

		template< class TT, class Cont >
		friend bool operator<(stack<TT, Cont>& lhs, stack<TT, Cont>& rhs);



		explicit stack(const Container& cont) : c(cont) {}
		explicit stack(Container&& cont = Container()) : c(std::move(cont)) {}
		stack(stack&& other) : c(std::move(other.c)) {}

		template< class Alloc >
		explicit stack(const Alloc& alloc) : c(alloc) {}

		template< class Alloc >
		stack(const Container& cont, const Alloc& alloc) : c(cont, alloc) {}

		template< class Alloc >
		stack(Container&& cont, const Alloc& alloc) : c(std::move(cont), alloc) {}

		template< class Alloc >
		stack(const stack& other, const Alloc& alloc) : c(other.c, alloc) {}

		template< class Alloc >
		stack(stack&& other, const Alloc& alloc) : c(std::move(other.c), alloc) {}

		~stack() { c.~Container(); }

		stack operator=(const stack& other) { c = other.c; }
		stack operator=(stack&& other){ c = std::move(other.c); }

		reference top() { return c.back(); }
		const_reference top() const { return c.back(); }

		bool empty() const { return c.empty(); }
		size_type size() const { return c.size(); }

		void push(const T& value) { c.push_back(value); }
		void push(T&& value) { c.push_back(std::move(value)); }

		template< class... Args >
		void emplace(Args&&... args) { c.emplace_back(std::forward<Args>(args)...); }

		void pop(){ c.pop_back(); }
		void swap(stack& other) { c.swap(other.c); }

	protected:
		Container c;
	};

	template< class T, class Container >
	bool operator==(stack<T, Container>& lhs, stack<T, Container>& rhs)
	{
		return lhs.c == rhs.c;
	}
	template< class T, class Container >
	bool operator!=(stack<T, Container>& lhs, stack<T, Container>& rhs)
	{
		return !(lhs == rhs);
	}

	template< class T, class Container >
	bool operator<(stack<T, Container>& lhs, stack<T, Container>& rhs)
	{
		return lhs.c < rhs.c;
	}

	template< class T, class Container >
	bool operator>(stack<T, Container>& lhs, stack<T, Container>& rhs)
	{
		return rhs < lhs;
	}

	template< class T, class Container >
	bool operator<=(stack<T, Container>& lhs, stack<T, Container>& rhs)
	{
		return !(rhs < lhs);
	}

	template< class T, class Container >
	bool operator>=(stack<T, Container>& lhs, stack<T, Container>& rhs)
	{
		return !(lhs < rhs);
	}

	template< class T, class Container >
	void swap(stack<T, Container>& lhs, stack<T, Container>& rhs)
	{
		std::swap(lhs, rhs);
	}
}

#endif