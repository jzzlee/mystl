#ifndef __MY_RBTREE_H_
#define __MY_RBTREE_H_

#include <iostream>
#include "my_memory.h"
#include "my_iterator.h"
#include "my_xutility.h"
#include <utility>

namespace my_stl
{
	typedef bool __rb_tree_color_type;
	const __rb_tree_color_type __rb_tree_red = false; //red is 0.
	const __rb_tree_color_type __rb_tree_black = true; //black is 1.

	struct __rb_tree_node_base
	{
		typedef __rb_tree_color_type color_type;
		typedef __rb_tree_node_base* base_ptr;

		color_type color;
		base_ptr parent;
		base_ptr left;
		base_ptr right;

		static base_ptr minimum(base_ptr x)
		{
			while (x->left != 0)
				x = x->left;
			return x;
		}

		static base_ptr maximum(base_ptr x)
		{
			while (x->right != 0)
				x = x->right;
			return x;
		}
	};

	template<typename T>
	struct __rb_tree_node : public __rb_tree_node_base
	{
		typedef __rb_tree_node<T>* link_type;
		T value;
	};


	//基层迭代器
	struct __rb_tree_iterator_base
	{
		typedef __rb_tree_node_base::base_ptr base_ptr;
		typedef bidirectional_iterator_tag iterator_category;
		typedef ptrdiff_t difference_type;
		base_ptr node;

		void increment()
		{
			if (node->right != 0)
			{
				node = node->right;
				while (node->left != 0)
					node = node->left;
			}
			else
			{
				base_ptr y = node->parent;
				while (node == y->right)
				{
					node = y;
					y = y->parent;
				}
				if (node->right != y)
					node = y;
			}

		}

		void decrement()
		{
			if (node->color == __rb_tree_red && node->parent->parent == node)
				node = node->right;
			else if (node->left != 0)
			{
				node = node->left;
				while (node->right != 0)
					node = node->right;
			}
			else
			{
				base_ptr y = node->parent;
				while (node == y->left)
				{
					node = y;
					y = y->parent;
				}
				node = y;
			}
		}
	};

	//RB-Tree的迭代器
	template<typename T, typename Ref, typename Ptr>
	struct __rb_tree_iterator : public __rb_tree_iterator_base
	{
		typedef T value_type;
		typedef Ref reference;
		typedef Ptr pointer;
		typedef __rb_tree_iterator<T, T&, T*> iterator;
		typedef __rb_tree_iterator<T, const T&, const T*> const_iterator;
		typedef __rb_tree_iterator<T, Ref, Ptr> self;
		typedef __rb_tree_node<T>* link_type;

		__rb_tree_iterator() {}
		__rb_tree_iterator(link_type x) { node = x; }
		__rb_tree_iterator(const iterator &it) { node = it.node; }

		reference operator*() const { return link_type(node)->value; }
		//		pointer opterator->() const { return &(operator*()); }

		self &operator++() { increment(); return *this; }
		self operator++(int)
		{
			self tmp = *this;
			increment();
			return tmp;
		}
		self &operator--() { decrement(); return *this; }
		self operator--(int)
		{
			slef tmp = *this;
			decrement();
			return tmp;
		}
	};

	inline bool operator==(const __rb_tree_iterator_base &x, const __rb_tree_iterator_base &y) {
		return x.node == y.node;
	}

	inline bool operator!=(const __rb_tree_iterator_base &x, const __rb_tree_iterator_base &y) {
		return x.node != y.node;
	}

	//左旋
	inline void __rb_tree_rotate_left(__rb_tree_node_base *x, __rb_tree_node_base *&root)
	{
		__rb_tree_node_base *y = x->right;
		x->right = y->left;
		if (y->left != 0)
			y->left->parent = x;
		y->parent = x->parent;
		if (x == root)
			root = y;
		else if (x == x->parent->left)
			x->parent->left = y;
		else
			x->parent->right = y;
		y->left = x;
		x->parent = y;
	}

	//右旋
	inline void __rb_tree_rotate_right(__rb_tree_node_base *x, __rb_tree_node_base *&root)
	{
		__rb_tree_node_base *y = x->left;
		x->left = y->right;
		if (y->right != 0)
			y->right->parent = x;
		y->parent = x->parent;
		if (x == root)
			root = y;
		else if (x == x->parent->left)
			x->parent->left = y;
		else
			x->parent->right = y;
		y->right = x;
		x->parent = y;
	}

	//全局函数，使树重新平衡
	inline void __rb_tree_rebalance(__rb_tree_node_base *x, __rb_tree_node_base *&root)
	{
		x->color = __rb_tree_red; //新节点颜色为红
		while (x != root && x->parent->color == __rb_tree_red)
		{
			if (x->parent == x->parent->parent->left) //如果父节点是组父节点的左孩子
			{
				__rb_tree_node_base *y = x->parent->parent->right; //y为伯父节点
				if (y && y->color == __rb_tree_red) // 伯父节点为红色, 情况1
				{
					x->parent->color = __rb_tree_black;
					y->color = __rb_tree_black;
					x->parent->parent->color = __rb_tree_red;
					x = x->parent->parent;
				}
				else //无伯父节点，或伯父节点为黑色
				{
					if (x == x->parent->right) //x是父节点的右孩子，情况2
					{
						x = x->parent;
						//左旋
						__rb_tree_rotate_left(x, root);
					}
					//情况3
					x->parent->color = __rb_tree_black;
					x->parent->parent->color = __rb_tree_red;
					//右旋
					__rb_tree_rotate_right(x->parent->parent, root);
				}
			}
			else //父节点是组父节点的右孩子
			{
				__rb_tree_node_base *y = x->parent->parent->left; //y为伯父节点
				if (y && y->color == __rb_tree_red) // 伯父节点为红色, 情况1
				{
					x->parent->color = __rb_tree_black;
					y->color = __rb_tree_black;
					x->parent->parent->color = __rb_tree_red;
					x = x->parent->parent;
				}
				else //无伯父节点，或伯父节点为黑色
				{
					if (x == x->parent->left) //x是父节点的右孩子，情况2
					{
						x = x->parent;
						//右旋
						//_rb_tree_rotate_right(x, root);
					}
					//情况3
					x->parent->color = __rb_tree_black;
					x->parent->parent->color = __rb_tree_red;
					//左旋
					//_rb_tree_rotate_left(x->parent->parent, root);
				}
			}
		}
		root->color = __rb_tree_black; //根节点为黑色
	}


	template<typename Key, typename Value, typename KeyOfValue, typename Compare, typename Allocator = allocator<Value> >
	class rb_tree
	{
	protected:
		typedef void* void_pointer;
		typedef __rb_tree_node_base* base_ptr;
		typedef __rb_tree_node<Value> rb_tree_node;
		typedef __rb_tree_color_type color_type;

	public:
		typedef Key key_type;
		typedef Value value_type;
		typedef Value* pointer;
		typedef const Value* const_pointer;
		typedef value_type& reference;
		typedef const value_type& const_reference;
		typedef rb_tree_node* link_type;
		typedef std::size_t size_type;
		typedef std::ptrdiff_t difference_type;
		typedef Allocator allocator_type;
		typedef __rb_tree_iterator<value_type, reference, pointer> iterator;
		typedef __rb_tree_iterator<value_type, const_reference, const_pointer> const_iterator;

	protected:
		Allocator alloc;
		allocator<rb_tree_node> alloc_node;
		size_type node_count;
		link_type header;
		Compare key_compare;

	protected:
		link_type get_node(){ return alloc_node.allocate(); }
		void put_node(link_type p) { alloc_node.deallocate(p); }

		link_type create_node(const value_type &x)
		{
			link_type tmp = get_node();
			alloc.construct(&tmp->value, x);
			return tmp;
		}

		link_type clone_node(link_type x)
		{
			link_type tmp = create_node(x->value);
			tmp->color = x->color;
			tmp->left = 0;
			tmp->right = 0;
			return tmp;
		}

		void destroy_node(link_type p)
		{
			alloc.destroy(&p->value);
			put_node(p);
		}

		//定义一些取成员函数，均返回引用，便于修改
		link_type &root() const { return (link_type &)header->parent; }
		link_type &leftmost() const { return (link_type &)header->left; }
		link_type &rightmost() const { return (link_type &)header->right; }

		static link_type &left(link_type x) { return (link_type&)(x->left); }
		static link_type &right(link_type x) { return (link_type&)(x->right); }
		static link_type &parent(link_type x) { return (link_type&)(x->parent); }
		static reference value(link_type x) { return x->value; }
		static const Key &key(link_type x) { return KeyOfValue()(value(x)); }
		static color_type &color(link_type x) { return (color_type&)(x->color); }

		static link_type &left(base_ptr x) { return (link_type&)(x->left); }
		static link_type &right(base_ptr x) { return (link_type&)(x->right); }
		static link_type &parent(base_ptr x) { return (link_type&)(x->parent); }
		static reference value(base_ptr x) { return ((link_type)x)->value; }
		static const Key &key(base_ptr x) { return KeyOfValue()(value(link_type(x))); }
		static color_type &color(base_ptr x) { return (color_type&)(link_type(x)->color); }

		//求极小极大值，使用node_base的方法
		static link_type minimum(link_type x) { return (link_type)__rb_tree_node_base::minimum(x); }
		static link_type maximum(link_type x) { return (link_type)__rb_tree_node_base::maximum(x); }

	private:
		iterator __insert(base_ptr x, base_ptr y, const value_type &v);
		link_type __copy(link_type x, link_type p);
		void __erase(link_type x);
		void init()
		{
			header = get_node(); //创建header节点
			color(header) = __rb_tree_red; //header为红色
			root() = 0;           //root节点为空
			leftmost() = header; //header左孩子为自己
			rightmost() = header; //header右孩子为自己
		}
		//void  clear()
		//{
		//	link_type x = root();
		//	if (node_count != 0) {
		//		leftmost() = header;
		//		root() = 0;
		//		rightmost() = header;
		//		node_count = 0;
		//	}
		//}

	public:

		rb_tree(const Compare &comp = Compare()) : node_count(0), key_compare(comp){ init(); }
		~rb_tree()
		{
			//clear();
			put_node(header);
		}

		rb_tree<Key, Value, KeyOfValue, Compare, Allocator>&
			operator=(const rb_tree<Key, Value, KeyOfValue, Compare, Allocator> &x);

		Compare key_comp() const { return key_compare; }
		iterator begin() { return leftmost(); }
		iterator end() { return header; }
		bool empty() const { return node_count == 0; }
		size_type size() const { return node_count; }
		size_type max_size() const { return std::numeric_limits<size_type>::max(); }

		std::pair<iterator, bool> insert_unique(const value_type &x);
		iterator insert_equal(const value_type &x);
		iterator find(const key_type &key);

		void inprint()
		{
			base_ptr x = header->parent;
			print(x);
		}
		void print(base_ptr z)
		{
			if (!z->left && !z->right)
			{
				cout << ((link_type)(z))->value << endl;
				return;
			}
			else if (!z->right)
			{
				cout << ((link_type)(z))->value << endl;
				print(z->left);
			}
			else if (!z->left)
			{
				cout << ((link_type)(z))->value << endl;
				print(z->right);
			}
			else
			{
				cout << ((link_type)(z))->value << endl;
				print(z->left);
				print(z->right);
			}
		}
	};

	//允许重复插入
	template<typename Key, typename Value, typename KeyOfValue, typename Compare, typename Allocator >
	typename rb_tree < Key, Value, KeyOfValue, Compare, Allocator>::iterator
		rb_tree<Key, Value, KeyOfValue, Compare, Allocator>::insert_equal(const Value &v)
	{
		link_type y = header;
		link_type x = root();
		while (x != 0)
		{
			y = x;
			x = key_compare(KeyOfValue()(v), key(x)) ? left(x) : right(x);
		}
		return __insert(x, y, v);
	}

	//unique insert
	template<typename Key, typename Value, typename KeyOfValue, typename Compare, typename Allocator >
	std::pair<typename rb_tree < Key, Value, KeyOfValue, Compare, Allocator>::iterator, bool>
		rb_tree<Key, Value, KeyOfValue, Compare, Allocator>::insert_unique(const Value &v)
	{
		link_type y = header;
		link_type x = root();
		bool comp = true;
		while (x != 0)
		{
			y = x;
			comp = key_compare(KeyOfValue()(v), key(x));
			x = comp ? left(x) : right(x);
		}
		iterator j = iterator(y);
		if (comp)
		{
			if (j == begin())
				return std::pair<iterator, bool>(__insert(x, y, v), true);
			else
				--j;
		}
		if (key_compare(key(j.node), KeyOfValue()(v)))
			return std::pair<iterator, bool>(__insert(x, y, v), true);
		return std::pair<iterator, bool>(j, false);
	}

	template<typename Key, typename Value, typename KeyOfValue, typename Compare, typename Allocator>
	typename rb_tree<Key, Value, KeyOfValue, Compare, Allocator>::iterator
		rb_tree<Key, Value, KeyOfValue, Compare, Allocator>::__insert(base_ptr x_, base_ptr y_, const Value &v)
	{
		link_type x = (link_type)x_;
		link_type y = (link_type)y_;
		link_type z;
		if (y == header || x != 0 || key_compare(KeyOfValue()(v), key(y)))
		{
			z = create_node(v);
			left(y) = z;
			if (y == header)
			{
				root() = z;
				rightmost() = z;
			}
			else if (y == leftmost())
				leftmost() = z;
		}
		else
		{
			z = create_node(v);
			right(y) = z;
			if (y == rightmost())
				rightmost() = z;
		}
		parent(z) = y;
		left(z) = 0;
		right(z) = 0;

		__rb_tree_rebalance(z, header->parent);
		++node_count;
		return iterator(z);
	}

	template<typename Key, typename Value, typename KeyOfValue, typename Compare, typename Allocator>
	typename rb_tree<Key, Value, KeyOfValue, Compare, Allocator>::iterator
		rb_tree<Key, Value, KeyOfValue, Compare, Allocator>::find(const Key &k)
	{
		link_type y = header;
		link_type x = root();
		while (x)
		{
			if (!key_compare(key(x), k))
			{
				y = x;
				x = left(x);
			}
			else
				x = right(x);
		}
		iterator j = iterator(y);
		return (j == end() || key_compare(k, key(j.node))) ? end() : j;
	}



}


#endif