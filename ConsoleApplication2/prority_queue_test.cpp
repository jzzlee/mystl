#include <iostream>
#include "my_algorithm.h"
#include "my_vector.h"

using namespace my_stl;
using std::cout;
using std::endl;

int main()
{
	int ia[9] = { 0, 1, 2, 3, 4, 8, 9, 3, 5 };
	vector<int> v{ia, ia + 9};

	make_heap(v.begin(), v.end());
	std::cout << "after make: ";
	for (auto i : v) std::cout << i << ' ';
	std::cout << '\n';

	v.push_back(7);
	push_heap(v.begin(), v.end());
	for (auto i : v) std::cout << i << ' ';
	std::cout << '\n';

	pop_heap(v.begin(), v.end());
	cout << v.back() << endl;
	v.pop_back();
	for (const auto &i : v) {
		std::cout << i << ' ';
	}
	std::cout << '\n';


	sort_heap(v.begin(), v.end());
	std::cout << "\nsorted:\t";
	for (const auto &i : v) {
		std::cout << i << ' ';
	}
	std::cout << '\n';
	return 0;
}