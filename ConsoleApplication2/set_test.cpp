#include <iostream>
#include <functional>
#include "my_set.h"
#include <cassert>

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

	set<int> set1;

	auto result_1 = set1.insert(3);
	assert(result_1.first != set1.end()); // it's a valid iterator
	assert(*result_1.first == 3);
	if (result_1.second)
		std::cout << "insert done\n";

	auto result_2 = set1.insert(3);
	assert(result_2.first == result_1.first); // same iterator
	assert(*result_2.first == 3);
	if (!result_2.second)
		std::cout << "no insertion\n";


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

	rb_tree<int, int, std::identity<int>, std::less<int> > itree2;
	for (int i = 0; i != 10; ++i)
		itree2.insert_equal(6);
	auto itree1(itree2);
	itree1 = itree2;
	itree1.inprint();
	auto it1 = itree1.lower_bound(6);
	cout << (it1 == itree1.begin()) << endl;
	auto it2 = itree1.upper_bound(6);
	cout << (it2 == itree1.end()) << endl;

	cout << "**************" << endl;
	for (; it1 != it2; ++it1)
		cout << *it1 << endl;
	cout << itree1.count(6) << endl;
	cout << itree2.erase(6) << endl;


	cout << "***************" << endl;

	int ia[5] = { 0, 1, 2, 3, 4 };
	rb_tree<int, int, std::identity<int>, std::less<int> > itree3;
	for (auto x : ia)
		itree3.insert_unique(x);
	itree3.inprint();

	set<int> iset(ia, ia + 5);
	iset.t.inprint();

	//	itree3.insert_unique(3);
	//	itree3.insert_unique(5);
	itree3.inprint();
	cout << "***" << endl;
	for (iter1 = itree3.begin(); iter1 != itree3.end(); ++iter1)
	{
		rbtite = __rb_tree_iterator_base(iter1);
		cout << *iter1 << "(" << rbtite.node->color << ")  ";
	}
	cout << endl;

	itree3.erase(1);
	itree3.inprint();


	cout << iset.size() << endl;
	cout << "3 count=" << iset.count(3) << endl;
	iset.insert(3);
	cout << iset.size() << endl;
	cout << "3 count=" << iset.count(3) << endl;
	iset.insert(5);
	cout << iset.size() << endl;
	cout << "3 count=" << iset.count(3) << endl;

	iset.erase(1);
	cout << iset.size() << endl;
	cout << "3 count=" << iset.count(3) << endl;
	cout << "1 count=" << iset.count(1) << endl;

	set<int>::iterator ite1 = iset.begin();
	auto ite2 = iset.end();
	for (; ite1 != ite2; ++ite1)
		cout << *ite1;
	cout << endl;

	ite1 = my_stl::find(iset.begin(), iset.end(), 3);
	if (ite1 != iset.end())
		cout << "3 found" << endl;

	ite1 = my_stl::find(iset.begin(), iset.end(), 1);
	if (ite1 != iset.end())
		cout << "1 found" << endl;


	ite1 = iset.find(3);
	if (ite1 != iset.end())
		cout << "3 found" << endl;
	ite1 = iset.find(1);
	if (ite1 != iset.end())
		cout << "1 found" << endl;

	return 0;

}