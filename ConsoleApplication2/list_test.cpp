#include<iostream>
#include <numeric>
#include <string>
#include "my_list.h"

using namespace my_stl;
using std::cout;
using std::endl;

void print(list<std::string> lst)
{
	for (auto each : lst)
		cout << each << " ";
	cout << endl;
}
template<typename T>
void print(list<T> lst)
{
	for (auto each : lst)
		cout << each << " ";
	cout << endl;
}
template<typename T>
void print_reverse(list<T> lst)
{
	for (auto iter = lst.rbegin(); iter != lst.rend(); ++iter)
		cout << *iter << " ";
	cout << endl;
}

int main()
{
	// c++11 initializer list syntax:
	list<std::string> words1{ "the", "frogurt", "is", "also", "cursed" };
	print(words1);
	print_reverse<std::string>(words1);
	// words2 == words1
	list<std::string> words2(words1.begin(), words1.end());
	print(words2);
	print_reverse<std::string>(words2);


	// words3 == words1
	list<std::string> words3(words1);
	print(words3);
	print_reverse<std::string>(words3);

	// words4 is {"Mo", "Mo", "Mo", "Mo", "Mo"}
	list<std::string> words4(std::size_t(5), "Mo");
	print(words4);
	print_reverse<std::string>(words4);

	auto words5 = std::move(words4);
	print(words5);
	print_reverse<std::string>(words5);

	cout << words5.max_size() << endl;

	list<int> lst1;
	list<int> lst2{ 1, 2, 3 };
	list<int> lst3{ 11, 22, 33, 44 };
	auto lst4 = lst2;

	lst1 = std::move(lst4);
	lst3 = std::move(lst2);

	print<int>(lst1);

	print<int>(lst3);

	lst1 = { 1, 2, 3, 4, 5, 6 };
	print<int>(lst1);
	lst1 = { 1, 2, 3 };
	print<int>(lst1);

	cout << lst1.back() << endl;
	const list<int> lst5{ 1, 2, 3, 4, 5 };
	cout << lst5.front() << " " << lst5.back() << endl;
	auto lst6 = lst1;
	lst1.assign(lst5.begin(), lst5.begin());
	lst3.assign(lst5.begin(), lst5.end());
	lst6.assign({ 1, 2, 3, 4, 5, 6, 7 });
	lst6.clear();
	cout << "**********" << endl;
	lst6.insert(lst6.begin(), 1);
	print<int>(lst6);
	int a = 2;
	lst6.insert(lst6.begin(), std::move(a));
	print<int>(lst6);
	lst6.insert(lst6.begin(), 3, 3);
	print<int>(lst6);
	lst6.insert(lst6.begin(), lst3.crbegin(), lst3.crend());
	print<int>(lst6);
	lst6.insert(lst6.begin(), { 4, 5, 6, 7 });
	print<int>(lst6);
	
	words2.emplace_back("hello");
	print(words2);
	words2.emplace(words2.begin(), "first");
	print(words2);

	auto p = lst6.erase(lst6.begin());
	print<int>(lst6);
	cout << *p << endl;
	p = lst6.end();
	--p;
	p = lst6.erase(lst6.begin(), p);
	print<int>(lst6);
	cout << *p << endl;

	return 0;
}