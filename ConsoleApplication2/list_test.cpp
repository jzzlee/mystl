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
bool comp(std::string s1, std::string s2)
{
	return s1.size() < s2.size();
}
bool equal_len(std::string s1, std::string s2)
{
	return s1.size() == s2.size();
}
std::ostream& operator<<(std::ostream& ostr, const list<int>& list)
{
	for (auto &i : list) {
		ostr << " " << i;
	}
	return ostr;
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
	cout << "*************" << endl;
	list<std::string> str1{ "1", "22", "333", "4444" };
	list<std::string> str2{ "3", "33", "433", "5444" };

	print(lst1);
	for (int i = 0; i != 9; ++i)
	{
		lst1.push_front(i);
	}
	print(lst1);
	/*for (int i = 0; i != 9; ++i)
	{
		lst1.pop_front();
		print(lst1);

	}
	for (int i = 0; i != 9; ++i)
	{
		lst1.push_back(i);
	}
	print(lst1);
	for (int i = 0; i != 9; ++i)
	{
		lst1.pop_back();
		print(lst1);
	}*/
//	print(str2);
	lst1.resize(9);
	print(lst1);
	lst1.resize(3);
	print(lst1);
	lst1.resize(9);
	print(lst1);
	print(lst2);
	lst1.swap(lst2);
	print(lst1);
	print(lst2);
	list<int> list1 = { 1, 2, 3, 4, 5 };
	list<int> list2 = { 10, 20, 30, 40, 50 };

	list1.reverse();
	list2.reverse();
	print(list1);
	print(list2);
	auto it = list1.begin();
	advance(it, 2);

	list1.splice(it, list2);

	std::cout << "list1: " << list1 << "\n";
	std::cout << "list2: " << list2 << "\n";

	list2.splice(list2.begin(), list1, it, list1.end());

	std::cout << "list1: " << list1 << "\n";
	std::cout << "list2: " << list2 << "\n";

	list2.splice(list2.begin(), list1, list1.begin());

	//std::cout << "list1: " << list1 << "\n";
	//std::cout << "list2: " << list2 << "\n";
	//cout << "**************" << endl;
	//list<int> l = { 1, 100, 2, 3, 10, 1, 11, -1, 12 };

	//l.remove(1); // remove both elements equal to 1
	//l.remove_if([](int n){ return n > 10; }); // remove all elements greater than 10

	//for (int n : l) {
	//	std::cout << n << ' ';
	//}
	//std::cout << '\n';
	list<int> x = { 1,5, 2,9,8,7,6};
	print(x);
	x.sort();
	print(x);
	list<int> mylist;
	// allocate an array of 5 elements using mylist's allocator:
	int * pp = mylist.get_allocator().allocate(5);

	// assign some values to array
	for (int i = 0; i<5; ++i) pp[i] = i;

	std::cout << "The allocated array contains:";
	for (int i = 0; i<5; ++i) std::cout << ' ' << pp[i];
	std::cout << '\n';

	mylist.get_allocator().deallocate(pp, 5);

	cout << (lst1 < lst2) << endl;
	swap(lst1, lst2);
	cout << (lst1 < lst2) << endl;
	return 0;
}