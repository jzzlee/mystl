#ifndef __MY_RBTREE_H_
#define __MY_RBTREE_H_

#include <iostream>
#include "my_memory.h"
#include "my_iterator.h"
#include "my_xutility.h"
#include <utility>
#include <initializer_list>


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
		T _value;
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
		__rb_tree_iterator(const const_iterator &it) { node = it.node; }

		reference operator*() const { return link_type(node)->_value; }
		pointer operator->() const { return &(operator*()); }

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

		//inline bidirectional_iterator_tag
		//	iterator_category(const __rb_tree_iterator_base&) {
		//	return bidirectional_iterator_tag();
		//}
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

	//用v为根节点的子树替代以u为根节点的子树
	inline void transplant(__rb_tree_node_base *u, __rb_tree_node_base *v, __rb_tree_node_base *&root)
	{
		if (!v)
			return;
		//先跟双亲节点脱离
		if (v == v->parent->left)
			v->parent->left = 0;
		else
			v->parent->right = 0;

		//替代u的位置
		if (u == root) //u是树根
			root->parent->parent = v;
		else if (u == u->parent->left) //u是父节点的左孩子
			u->parent->left = v;
		else
			u->parent->right = v;

		//改变parent
		v->parent = u->parent;

		//如果u是leftmost或者rightmost，更新为v
		if (root->parent->left == u)
			root->parent->left = v;
		if (root->parent->right == u)
			root->parent->right = v;
	}

	//删除节点的树调整
	inline void rb_delete_fixup(__rb_tree_node_base *x, __rb_tree_node_base *&root)
	{
		__rb_tree_node_base *w;
		while (x != root && x->color == __rb_tree_black)
		{
			if (x == x->parent->left) //x是父节点的左孩子
			{
				w = x->parent->right; //w是x的兄弟节点
				if (w->color == __rb_tree_red) //case1,w节点是红色，改变颜色后左旋到case2~4
				{
					w->color = __rb_tree_black;
					x->parent->color = __rb_tree_red;
					__rb_tree_rotate_left(x->parent, root);
					w = x->parent->right;
				}
				if (w->left->color == __rb_tree_black &&w->right->color == __rb_tree_black) //case2, w颜色是黑色，w左右孩子均为黑色
				{
					w->color = __rb_tree_red;
					x = x->parent;
				}
				else if (w->right->color == __rb_tree_black) //case3, w颜色是黑色，w孩子左红右黑
				{
					w->left->color = __rb_tree_black;
					w->color = __rb_tree_red;
					__rb_tree_rotate_right(w, root);
					w = x->parent->right;
				}
				else										//case4, w颜色是黑色，w右孩子是红色
				{
					w->color = x->parent->color;
					x->parent->color = __rb_tree_black;
					w->right->color = __rb_tree_black;
					__rb_tree_rotate_left(x->parent, root);
					x = root;
				}
			}
			else                            //如果x是父节点的右孩子，与以上对称处理
			{
				w = x->parent->left; //w是x的兄弟节点
				if (w->color == __rb_tree_red) //case1,w节点是红色，改变颜色后左旋到case2~4
				{
					w->color = __rb_tree_black;
					x->parent->color = __rb_tree_red;
					__rb_tree_rotate_right(x->parent, root);
					w = x->parent->left;
				}
				if (w->left->color == __rb_tree_black &&w->right->color == __rb_tree_black) //case2, w颜色是黑色，w左右孩子均为黑色
				{
					w->color = __rb_tree_red;
					x = x->parent;
				}
				else if (w->left->color == __rb_tree_black) //case3, w颜色是黑色，w孩子左黑右红
				{
					w->right->color = __rb_tree_black;
					w->color = __rb_tree_red;
					__rb_tree_rotate_left(w, root);
					w = x->parent->left;
				}
				else										//case4, w颜色是黑色，w左孩子是红色
				{
					w->color = x->parent->color;
					x->parent->color = __rb_tree_black;
					w->left->color = __rb_tree_black;
					__rb_tree_rotate_right(x->parent, root);
					x = root;
				}
			}
		}
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
		typedef rb_tree<Key, Value, KeyOfValue, Compare, Allocator> Myt;
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
			alloc.construct(&tmp->_value, x);
			return tmp;
		}

		link_type clone_node(link_type x)
		{
			link_type tmp = create_node(x->_value);
			tmp->color = x->color;
			tmp->left = 0;
			tmp->right = 0;
			return tmp;
		}

		void destroy_node(link_type p)
		{
			alloc.destroy(&p->_value);
			put_node(p);
		}

		//定义一些取成员函数，均返回引用，便于修改
		link_type &root() const { return (link_type &)header->parent; }
		link_type &leftmost() const { return (link_type &)header->left; }
		link_type &rightmost() const { return (link_type &)header->right; }

		static link_type &left(link_type x) { return (link_type&)(x->left); }
		static link_type &right(link_type x) { return (link_type&)(x->right); }
		static link_type &parent(link_type x) { return (link_type&)(x->parent); }
		static reference value(link_type x) { return x->_value; }
		static const Key key(link_type x){ return KeyOfValue()(value(x)); }
//		static const Key &key(link_type x) { return KeyOfValue()(value(x)); }
		static color_type &color(link_type x) { return (color_type&)(x->color); }

		static link_type &left(base_ptr x) { return (link_type&)(x->left); }
		static link_type &right(base_ptr x) { return (link_type&)(x->right); }
		static link_type &parent(base_ptr x) { return (link_type&)(x->parent); }
		static reference value(base_ptr x) { return ((link_type)x)->_value; }
		static const Key key(base_ptr x) { return KeyOfValue()(value(link_type(x))); }
		static color_type &color(base_ptr x) { return (color_type&)(link_type(x)->color); }

		//求极小极大值，使用node_base的方法
		static link_type minimum(link_type x) { return (link_type)__rb_tree_node_base::minimum(x); }
		static link_type maximum(link_type x) { return (link_type)__rb_tree_node_base::maximum(x); }

	private:
		iterator __insert(base_ptr x, base_ptr y, const value_type &v);
		void rb_erase(__rb_tree_node_base *z, __rb_tree_node_base *&root);
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


		//递归删除x为根节点的子树
		void erase_for_clear(link_type x)
		{
			while (x)
			{
				erase_for_clear((link_type)x->right);
				link_type y = (link_type)x->left;
				destroy_node(x);
				x = y;
			}
		}

		//清除树的所有节点
		void  clear()
		{
			if (node_count != 0) {
				erase_for_clear(root());
				leftmost() = header;
				root() = 0;
				rightmost() = header;
				node_count = 0;
			}
		}

		//复制以p为header，x为根节点的一棵树，返回新树的根节点
		link_type copy(link_type x, link_type p)
		{
			link_type top = clone_node(x);
			top->parent = p;
			if (x->right)
				top->right = copy(right(x), top);
			p = top;
			x = left(x);

			while (x)
			{
				link_type y = clone_node(x);
				p->left = y;
				y->parent = p;
				if (x->right)
					y->right = copy(right(x), y);
				p = y;
				x = left(x);
			}
			return top;
		}

	public:
		rb_tree() : node_count(0), key_compare() { init(); }
		rb_tree(const Compare &comp) : node_count(0), key_compare(comp) { init(); }
		rb_tree(const Compare &comp, const allocator_type &a) : node_count(0), key_compare(comp), alloc(a) { init(); }
		rb_tree(const allocator_type &a) : node_count(0), key_compare(), alloc(a){ init(); }
		
		rb_tree(const rb_tree<Key, Value, KeyOfValue, Compare, Allocator> &t)
			: node_count(t.node_count), key_compare(t.key_compare), alloc(t.alloc), alloc_node(t.alloc_node)
		{
			if (t.root() == 0)
				init();
			else
			{
				header = get_node(); //创建header节点
				color(header) = __rb_tree_red;
				root() = copy(t.root(), header);
				leftmost() = minimum(root());
				rightmost() = maximum(root());
			}
		}

		rb_tree(const rb_tree<Key, Value, KeyOfValue, Compare, Allocator> &t, const allocator_type &a)
			: node_count(t.node_count), key_compare(t.key_compare), alloc(a), alloc_node(t.alloc_node)
		{
			if (t.root() == 0)
				init();
			else
			{
				header = get_node(); //创建header节点
				color(header) = __rb_tree_red;
				root() = copy(t.root(), header);
				leftmost() = minimum(root());
				rightmost() = maximum(root());
			}
		}

		//move constructor
		rb_tree(rb_tree<Key, Value, KeyOfValue, Compare, Allocator> &&t)
			: node_count(t.node_count), key_compare(t.key_compare), alloc(t.alloc), alloc_node(t.alloc_node)
		{
			header = t.header;
			t.node_count = 0;
			t.header->parent = t.header->left = t.header->right = 0;
		}

		rb_tree(rb_tree<Key, Value, KeyOfValue, Compare, Allocator> &&t, const allocator_type &a)
			: node_count(t.node_count), key_compare(t.key_compare), alloc(a), alloc_node(t.alloc_node)
		{
			header = t.header;
			t.node_count = 0;
			t.header->parent = t.header->left = t.header->right = 0;
		}



		~rb_tree()
		{
			clear();
			put_node(header);
		}

		rb_tree<Key, Value, KeyOfValue, Compare, Allocator>&
			operator=(const rb_tree<Key, Value, KeyOfValue, Compare, Allocator> &x);
		rb_tree<Key, Value, KeyOfValue, Compare, Allocator>&
			operator=(rb_tree<Key, Value, KeyOfValue, Compare, Allocator> &&x);
		rb_tree<Key, Value, KeyOfValue, Compare, Allocator>&
			operator=(std::initializer_list<value_type> ilist);

		Compare key_comp() const { return key_compare; }
		allocator_type get_allocator() { return alloc; }
		iterator begin() { return leftmost(); }
		const_iterator begin() const { return leftmost(); }
		const_iterator cbegin() const { return ((const Myt*)this)->begin(); }

		iterator end() { return header; }
		const_iterator end() const { return header; }
		const_iterator cend() const { return ((const Myt*)this)->end(); }

		bool empty() const { return node_count == 0; }
		size_type size() const { return node_count; }
		size_type max_size() const { return std::numeric_limits<size_type>::max(); }

		void swap(rb_tree<Key, Value, KeyOfValue, Compare, Allocator> &t)
		{
			std::swap(header, t.header);
			std::swap(node_count, t.node_count);
			std::swap(key_compare, t.key_compare);
			std::swap(alloc, t.alloc);
			std::swap(alloc_node, t.alloc_node);
		}

		std::pair<iterator, bool> insert_unique(const value_type &x);
		iterator insert_equal(const value_type &x);

		iterator insert_equal(const_iterator hint, const value_type &x);
		iterator insert_unique(const_iterator hint, const value_type &x);

		template<typename ForwardIt>
		void insert_equal(ForwardIt first, ForwardIt last);

		template<typename ForwardIt>
		void insert_unique(ForwardIt first, ForwardIt last);

		iterator find(const key_type &k);
		const_iterator find(const key_type& k) const;

		iterator erase(iterator x);
		iterator erase(const_iterator x);

		size_type erase(const key_type&  key);
		iterator erase(const_iterator first, const_iterator last);

		iterator lower_bound(const key_type& k);
		const_iterator lower_bound(const key_type& k) const;
		iterator upper_bound(const key_type& k);
		const_iterator upper_bound(const key_type& k) const;

		size_type count(const key_type& k) const;

		std::pair<iterator, iterator> equal_range(const key_type& k);
		std::pair<const_iterator, const_iterator> equal_range(const key_type& k) const;

		void inprint() const
		{
			base_ptr x = header->parent;
			print(x);
		}
		void print(base_ptr z) const
		{
			if (!z->left && !z->right)
			{
				cout << "leftandright" << endl;
				cout << ((link_type)(z))->_value.first << endl;
				return;
			}
			else if (!z->right)
			{
				cout << ((link_type)(z))->_value.first << endl;
				print(z->left);
			}
			else if (!z->left)
			{
				cout << "left" << endl;
				cout << ((link_type)(z))->_value.first << endl;
				print(z->right);
			}
			else
			{
				cout << ((link_type)(z))->_value.first << endl;
				print(z->left);
				print(z->right);
			}
		}
	};

	template<typename Key, typename Value, typename KeyOfValue, typename Compare, typename Allocator >
	rb_tree<Key, Value, KeyOfValue, Compare, Allocator>&
		rb_tree<Key, Value, KeyOfValue, Compare, Allocator>::operator=(const rb_tree<Key, Value, KeyOfValue, Compare, Allocator> &x)
	{
		if (this != &x)
		{
			clear();
			node_count = 0;
			key_compare = x.key_compare;
			alloc = x.alloc;
			alloc_node = x.alloc_node;

			if (x.root() == 0)
			{
				root() = 0;
				leftmost() = 0;
				rightmost() = 0;
			}
			else
			{
				root() = copy(x.root(), header);
				leftmost() = minimum(root());
				rightmost() = maximum(root());
				node_count = x.node_count;
			}
		}
		return *this;
	}

	template<typename Key, typename Value, typename KeyOfValue, typename Compare, typename Allocator >
	rb_tree<Key, Value, KeyOfValue, Compare, Allocator>&
		rb_tree<Key, Value, KeyOfValue, Compare, Allocator>::operator=(rb_tree<Key, Value, KeyOfValue, Compare, Allocator> &&x)
	{
		if (this != &x)
		{
			if (alloc == x.alloc) //一样就直接指向x的内容
			{
				clear();
				node_count = x.node_count;
				header = x.header;
				key_compare = x.key_compare;
				alloc_node = x.alloc_node;
				x.node_count = 0;
				x.header->parent = x.header->left = x.header->right = 0;
			}
			else
			{
				*this = x;
			}
		}
		return *this;
	}


	template<typename Key, typename Value, typename KeyOfValue, typename Compare, typename Allocator >
	rb_tree<Key, Value, KeyOfValue, Compare, Allocator>&
		rb_tree<Key, Value, KeyOfValue, Compare, Allocator>::operator=(std::initializer_list<value_type> ilist)
	{
		clear();
		node_count = 0;
		insert_unique(ilist.begin(), ilist.end());
		return *this;
	}

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

	//带位置提示的equal insert
	template<typename Key, typename Value, typename KeyOfValue, typename Compare, typename Allocator>
	typename rb_tree<Key, Value, KeyOfValue, Compare, Allocator>::iterator
		rb_tree<Key, Value, KeyOfValue, Compare, Allocator>::insert_equal(const_iterator hint, const value_type &v)
	{
		if (hint.node == header->left) //最小值
		{
			if (node_count > 0 && !key_compare(key(hint.node), KeyOfValue()(v)))
				return __insert(hint.node, hint.node, v);
			else
				return insert_equal(v);
		}
		else if (hint.node == header) //end()
		{
			if (!key_compare(KeyOfValue()(v), key(rightmost())))
				return __insert(0, rightmost(), v);
			else
				return insert_equal(v);
		}
		else
		{
			iterator before = hint;
			--before;
			if (!key_compare(KeyOfValue()(v), key(before.node)) && !key_compare(key(hint.node), KeyOfValue()(v)))
				if (right(before.node) == 0)
					return __insert(0, before.node, v);
				else
					return __insert(hint.node, hint.node, v);
			else
				return insert_equal(v);
		}

	}

	//带位置提示的unique insert
	template<typename Key, typename Value, typename KeyOfValue, typename Compare, typename Allocator >
	typename rb_tree<Key, Value, KeyOfValue, Compare, Allocator>::iterator
		rb_tree<Key, Value, KeyOfValue, Compare, Allocator>::insert_unique(const_iterator hint, const Value &v)
	{
		if (hint.node == header->left) // begin()
		{
			if (size() > 0 && key_compare(KeyOfValue()(v), key(hint.node)))
				return __insert(hint.node, hint.node, v);
			else
				return insert_unique(v).first;
		}
		else if (hint.node == header) // end()
		{
			if (key_compare(key(rightmost()), KeyOfValue()(v)))
				return __insert(0, rightmost(), v);
			else
				return insert_unique(v).first;
		}
		else
		{
			iterator before = hint;
			--before;
			if (key_compare(key(before.node), KeyOfValue()(v)) && key_compare(KeyOfValue()(v), key(hint.node)))
			{
				if (right(before.node) == 0)
					return __insert(0, before.node, v);
				else
					return __insert(hint.node, hint.node, v);
			}
			else
				return insert_unique(v).first;
		}
	}

	//范围equal插入
	template<typename Key, typename Value, typename KeyOfValue, typename Compare, typename Allocator >
	template<typename ForwardIt>
	void rb_tree<Key, Value, KeyOfValue, Compare, Allocator>::insert_equal(ForwardIt first, ForwardIt last)
	{
		for (; first != last; ++first)
			insert_equal(*first);
	}

	//范围unique插入
	template<typename Key, typename Value, typename KeyOfValue, typename Compare, typename Allocator>
	template<typename ForwardIt>
	void rb_tree<Key, Value, KeyOfValue, Compare, Allocator>::insert_unique(ForwardIt first, ForwardIt last)
	{
		for (; first != last; ++first)
			insert_unique(*first);
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


	//删除root为根节点的树的z节点
	template<typename Key, typename Value, typename KeyOfValue, typename Compare, typename Allocator>
	void rb_tree<Key, Value, KeyOfValue, Compare, Allocator>::rb_erase(__rb_tree_node_base *z, __rb_tree_node_base *&root)
	{
		__rb_tree_node_base *y = z;
		__rb_tree_node_base *x;
		__rb_tree_color_type y_color = y->color; //保存z节点原本的颜色
		if (z->left == 0)//没有左孩子
		{
			x = z->right;
			transplant(z, z->right, root);
		}
		else if (z->right == 0) //没有右孩子
		{
			x = z->left;
			transplant(z, z->left, root);
		}
		else
		{
			y = __rb_tree_node_base::minimum(z->right); //要把这个节点移到z的位置
			y_color = y->color; //记录该节点原本颜色
			x = y->right;
			if (y->parent == z) //y是z的孩子，直接移上去
				x->parent = y; // ??????????????
			if (y->parent != z) //y不是z的孩子
			{
				transplant(y, y->right, root); //y是最小的，所以没有左孩子，直接让右孩子取代y
				//y替代z
				y->right = z->right;
				y->right->parent = y;
			}
			transplant(z, y, root);
			y->left = z->left;
			y->left->parent = y;
			y->color = z->color;
		}
		if (y_color == __rb_tree_black) //如果原来颜色是黑色，需要调整树
			rb_delete_fixup(x, root);
		--node_count;
	}

	template<typename Key, typename Value, typename KeyOfValue, typename Compare, typename Allocator>
	typename rb_tree<Key, Value, KeyOfValue, Compare, Allocator>::iterator
		rb_tree<Key, Value, KeyOfValue, Compare, Allocator>::erase(iterator x)
	{
		iterator y = x;
		++y;
		rb_erase(x.node, header->parent);
		destroy_node((link_type)x.node);
		return y;
	}

	template<typename Key, typename Value, typename KeyOfValue, typename Compare, typename Allocator>
	typename rb_tree<Key, Value, KeyOfValue, Compare, Allocator>::iterator
		rb_tree<Key, Value, KeyOfValue, Compare, Allocator>::erase(const_iterator x)
	{
		iterator y = x;
		++y;
		rb_erase(x.node, header->parent);
		destroy_node((link_type)x.node);
		return y;
	}

	template<typename Key, typename Value, typename KeyOfValue, typename Compare, typename Allocator>
	typename rb_tree<Key, Value, KeyOfValue, Compare, Allocator>::iterator
		rb_tree<Key, Value, KeyOfValue, Compare, Allocator>::erase(const_iterator first, const_iterator last)
	{
		for (; first != last;)
		{
			first = erase(first);
			//			destroy_node((link_type)first.node);
		}
		return (iterator)last;
	}

	template<typename Key, typename Value, typename KeyOfValue, typename Compare, typename Allocator>
	typename rb_tree<Key, Value, KeyOfValue, Compare, Allocator>::size_type
		rb_tree<Key, Value, KeyOfValue, Compare, Allocator>::erase(const key_type&  k)
	{
		std::pair<iterator, iterator> p = equal_range(k);
		size_type n = my_stl::distance(p.first, p.second);
		erase(p.first, p.second);
		return n;
	}

	template<typename Key, typename Value, typename KeyOfValue, typename Compare, typename Allocator>
	typename rb_tree<Key, Value, KeyOfValue, Compare, Allocator>::iterator
		rb_tree<Key, Value, KeyOfValue, Compare, Allocator>::lower_bound(const Key &k)
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
		return iterator(y);
		//iterator j = iterator(y);
		//return (j == end() || key_compare(k, key(j.node))) ? end() : j;
	}

	template<typename Key, typename Value, typename KeyOfValue, typename Compare, typename Allocator>
	typename rb_tree<Key, Value, KeyOfValue, Compare, Allocator>::const_iterator
		rb_tree<Key, Value, KeyOfValue, Compare, Allocator>::lower_bound(const key_type& k) const
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
		return const_iterator(y);
		//const_iterator j = iterator(y);
		//return (j == cend() || key_compare(k, key(j.node))) ? cend() : j;
	}


	template<typename Key, typename Value, typename KeyOfValue, typename Compare, typename Allocator>
	typename rb_tree<Key, Value, KeyOfValue, Compare, Allocator>::iterator
		rb_tree<Key, Value, KeyOfValue, Compare, Allocator>::upper_bound(const key_type& k)
	{
		link_type y = header;
		link_type x = root();
		while (x)
		{
			if (key_compare(k, key(x)))
			{
				y = x;
				x = left(x);
			}
			else
			{
				x = right(x);
			}
		}
		return iterator(y);
		//iterator j = iterator(y);
		//return (j == end() || key_compare(k, key(j.node))) ? end() : j;
	}

	template<typename Key, typename Value, typename KeyOfValue, typename Compare, typename Allocator>
	typename rb_tree<Key, Value, KeyOfValue, Compare, Allocator>::const_iterator
		rb_tree<Key, Value, KeyOfValue, Compare, Allocator>::upper_bound(const key_type& k) const
	{
		link_type y = header;
		link_type x = root();
		while (x)
		{
			if (key_compare(k, key(x)))
			{
				y = x;
				x = left(x);
			}
			else
			{
				x = right(x);
			}
		}
		return const_iterator(y);

	}

	template<typename Key, typename Value, typename KeyOfValue, typename Compare, typename Allocator>
	typename rb_tree<Key, Value, KeyOfValue, Compare, Allocator>::size_type
		rb_tree<Key, Value, KeyOfValue, Compare, Allocator>::count(const key_type &k) const
	{
		std::pair<const_iterator, const_iterator> p = equal_range(k);
		return (size_type)my_stl::distance(p.first, p.second);
	}

	template<typename Key, typename Value, typename KeyOfValue, typename Compare, typename Allocator>
	std::pair<typename rb_tree<Key, Value, KeyOfValue, Compare, Allocator>::iterator,
		typename rb_tree<Key, Value, KeyOfValue, Compare, Allocator>::iterator>
		rb_tree<Key, Value, KeyOfValue, Compare, Allocator>::equal_range(const key_type &k)
	{
		iterator upp = upper_bound(k);
		return std::pair<iterator, iterator>(lower_bound(k), upp);
	}

	template<typename Key, typename Value, typename KeyOfValue, typename Compare, typename Allocator>
	std::pair<typename rb_tree<Key, Value, KeyOfValue, Compare, Allocator>::const_iterator,
		typename rb_tree<Key, Value, KeyOfValue, Compare, Allocator>::const_iterator>
		rb_tree<Key, Value, KeyOfValue, Compare, Allocator>::equal_range(const key_type &k) const
	{
		return std::pair<const_iterator, const_iterator>(lower_bound(k), upper_bound(k));
	}

	template<typename Key, typename Value, typename KeyOfValue, typename Compare, typename Allocator>
	inline bool	operator==(const rb_tree<Key, Value, KeyOfValue, Compare, Allocator>& lhs,
		const rb_tree<Key, Value, KeyOfValue, Compare, Allocator>& rhs)
	{
		return (lhs.size() == rhs.size() && my_stl::equal(lhs.begin(), lhs.end(), rhs.begin()));
	}

	template<typename Key, typename Value, typename KeyOfValue, typename Compare, typename Allocator>
	inline bool	operator!=(const rb_tree<Key, Value, KeyOfValue, Compare, Allocator>& lhs,
		const rb_tree<Key, Value, KeyOfValue, Compare, Allocator>& rhs)
	{
		return !(lhs == rhs);
	}

	template<typename Key, typename Value, typename KeyOfValue, typename Compare, typename Allocator>
	inline bool	operator<(const rb_tree<Key, Value, KeyOfValue, Compare, Allocator>& lhs,
		const rb_tree<Key, Value, KeyOfValue, Compare, Allocator>& rhs)
	{
		return (lexicographical_compare(lhs.begin(), lhs.end(),
			rhs.begin(), rhs.end()));
	}

	template<typename Key, typename Value, typename KeyOfValue, typename Compare, typename Allocator>
	inline bool	operator>(const rb_tree<Key, Value, KeyOfValue, Compare, Allocator>& lhs,
		const rb_tree<Key, Value, KeyOfValue, Compare, Allocator>& rhs)
	{
		return rhs < lhs;
	}

	template<typename Key, typename Value, typename KeyOfValue, typename Compare, typename Allocator>
	inline bool	operator<=(const rb_tree<Key, Value, KeyOfValue, Compare, Allocator>& lhs,
		const rb_tree<Key, Value, KeyOfValue, Compare, Allocator>& rhs)
	{
		return !(rhs < lhs);
	}

	template<typename Key, typename Value, typename KeyOfValue, typename Compare, typename Allocator>
	inline bool	operator>=(const rb_tree<Key, Value, KeyOfValue, Compare, Allocator>& lhs,
		const rb_tree<Key, Value, KeyOfValue, Compare, Allocator>& rhs)
	{
		return !(lhs < rhs);
	}

	template<typename Key, typename Value, typename KeyOfValue, typename Compare, typename Allocator>
	inline void swap(const rb_tree<Key, Value, KeyOfValue, Compare, Allocator>& lhs,
		const rb_tree<Key, Value, KeyOfValue, Compare, Allocator>& rhs)
	{
		lhs.swap(rhs);
	}


}


#endif