#include<iostream>
#include <numeric>
#include <string>
#include "my_deque.h"
#include "my_vector.h"
#include "my_list.h"

using namespace my_stl;
using std::cout;
using std::endl;
using std::string;

template<typename T>
void print(deque<T> dq)
{
	for (const auto &x : dq)
		cout << x << " ";
	cout << endl;
}
int main()
{
	deque<int> dq1;
	deque<int> dq2(10, 1);
	deque<int> dq3(10);

	print(dq1);
	print(dq2);
	print(dq3);
	auto dq33(dq3);
	print(dq33);
	cout << "************" << endl;
	list<int> lst1{ 1, 2, 3, 4 };
	vector<int> vec(lst1.begin(), lst1.end());
	for (auto x : vec)
		cout << x << endl;
	deque<int> dq4(vec.begin(), vec.end());
	print(dq4);
	auto dq5(dq4);
	print(dq5);
	auto dq6(std::move(dq5));
	print(dq5);
	print(dq6);
	deque<int> dq7(130, 1);
	print(dq7);
	cout << "************" << endl;
	cout << dq6.size() << endl;
	dq6 = { 1, 2, 3, 4, 5, 6 };
	print(dq6);
	dq6.assign(dq7.begin(), dq7.begin() + 4);
	print(dq6);
	dq6.assign(dq7.begin(), dq7.begin() + 20);
	print(dq6);
	dq6.assign(dq7.begin(), dq7.end());
	print(dq6);
	dq6.assign({ 1, 2, 3, 4, 5 });
	print(dq6);
	cout << dq6.at(1) << " " << dq6[2] << " " << dq6.front() << " " << dq6.back() << endl;
	dq6.shrink_to_fit();
	print(dq6);
	dq1.shrink_to_fit();
	dq2.shrink_to_fit();
	dq3.shrink_to_fit();
	dq4.shrink_to_fit();
	dq5.shrink_to_fit();
	dq6.shrink_to_fit();
	dq7.shrink_to_fit();
	dq6.clear();
	dq6.shrink_to_fit();
	print(dq6);
	return 0;
}