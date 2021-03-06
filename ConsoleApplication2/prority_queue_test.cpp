#include <iostream>
#include <functional>
#include "my_queue.h"

using namespace my_stl;
using std::cout;
using std::endl;

template<typename T> void print_queue(T& q) {
	while (!q.empty()) {
		std::cout << q.top() << " ";
		q.pop();
	}
	std::cout << '\n';
}

int main()
{
	//int ia[9] = { 0, 1, 2, 3, 4, 8, 9, 3, 5 };
	//vector<int> v{ia, ia + 9};

	//make_heap(v.begin(), v.end());
	//std::cout << "after make: ";
	//for (auto i : v) std::cout << i << ' ';
	//std::cout << '\n';
	//
	//cout << is_heap(v.begin(), v.end()) << endl;

	//v.push_back(7);
	//push_heap(v.begin(), v.end());
	//for (auto i : v) std::cout << i << ' ';
	//std::cout << '\n';

	//pop_heap(v.begin(), v.end());
	//cout << v.back() << endl;
	//v.pop_back();
	//for (const auto &i : v) {
	//	std::cout << i << ' ';
	//}
	//std::cout << '\n';


	//sort_heap(v.begin(), v.end());
	//std::cout << "\nsorted:\t";
	//for (const auto &i : v) {
	//	std::cout << i << ' ';
	//}
	//std::cout << '\n';

	vector<int> v{ 3, 1, 4, 1, 5, 9 };

	my_stl::make_heap(v.begin(), v.end());

	// probably mess up the heap
	v.push_back(2);
	v.push_back(6);

	auto heap_end = is_heap_until(v.begin(), v.end());

	std::cout << "all of v: ";
	for (auto i : v) std::cout << i << ' ';
	std::cout << '\n';

	std::cout << "only heap: ";
	for (auto i = v.begin(); i != heap_end; ++i) std::cout << *i << ' ';
	std::cout << '\n';


	priority_queue<int> q;

	for (int n : {1, 8, 5, 6, 3, 4, 0, 9, 3, 2})
		q.push(n);

	print_queue(q);

	priority_queue<int, vector<int>, std::greater<int> > q2;

	for (int n : {1, 8, 5, 6, 3, 4, 0, 9, 3, 2})
		q2.push(n);

	print_queue(q2);

	return 0;
}