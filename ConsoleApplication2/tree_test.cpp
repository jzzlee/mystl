#include <iostream>
#include <functional>
#include "my_rb_tree.h"

using namespace my_stl;
using std::cout;
using std::endl;

//template<typename T> void print_queue(T& q) {
//	while (!q.empty()) {
//		std::cout << q.top() << " ";
//		q.pop();
//	}
//	std::cout << '\n';
//}

int main() {

	int cx = 3;
	const auto &ax = cx;
	int mx = 4;
	//	auto &bx = std::move(mx);

	cout << ax << endl;

	rb_tree<int, int, std::identity<int>, std::less<int> > itree;
	cout << itree.size() << endl;

	itree.insert_unique(10);
	itree.insert_unique(7);
	itree.insert_unique(8);

	rb_tree<int, int, std::identity<int>, std::less<int> >::iterator iter1 = itree.begin();
	rb_tree<int, int, std::identity<int>, std::less<int> >::iterator iter2 = itree.end();

	//	iter1++;
	__rb_tree_iterator_base rbtite;

	for (; iter1 != iter2; ++iter1)
	{
		rbtite = __rb_tree_iterator_base(iter1);
		cout << *iter1 << "(" << rbtite.node->color << ")  ";
	}
	cout << endl;

	itree.insert_unique(15);
	itree.insert_unique(5);
	itree.insert_unique(6);

	iter1 = itree.begin();


	for (; iter1 != iter2; ++iter1)
	{
		rbtite = __rb_tree_iterator_base(iter1);
		cout << *iter1 << "(" << rbtite.node->color << ")  ";
	}
	cout << endl;

	itree.insert_unique(11);
	itree.insert_unique(13);
	itree.insert_unique(12);

	cout << itree.size() << endl;

	iter1 = itree.begin();


	for (; iter1 != iter2; ++iter1)
	{
		rbtite = __rb_tree_iterator_base(iter1);
		cout << *iter1 << "(" << rbtite.node->color << ")  ";
	}
	cout << endl;

	itree.inprint();
	cout << endl;
	auto iter = itree.find(10);
	cout << *iter << endl;
	itree.erase(iter);

	iter1 = itree.begin();


	for (; iter1 != iter2; ++iter1)
	{
		rbtite = __rb_tree_iterator_base(iter1);
		cout << *iter1 << "(" << rbtite.node->color << ")  ";
	}
	cout << endl;
	return 0;

}