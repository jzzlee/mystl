#include <iostream>
#include <functional>
#include "my_map.h"
#include <cassert>
#include <string>
#include "my_vector.h"
#include "my_deque.h"
using namespace my_stl;
using std::cout;
using std::endl;
using std::pair;
using std::string;
typedef pair<int, int> PNN;

void display_sizes(const map<int, int> &nums1,
	const map<int, int> &nums2,
	const map<int, int> &nums3)
{
	std::cout << "nums1: " << nums1.size()
		<< " nums2: " << nums2.size()
		<< " nums3: " << nums3.size() << '\n';
}

struct A
{
	int first;
	std::string second;
	A(int a, std::string b) :first(a), second(b){}
};

int main()
{
	map<int, int> nums1{ { 3, 1 }, { 4, 1 }, { 5, 9 },
	{ 6, 1 }, { 7, 1 }, { 8, 9 } };
	map<int, int> nums2;
	map<int, int> nums3;

	std::cout << "Initially:\n";
	display_sizes(nums1, nums2, nums3);

	// copy assignment copies data from nums1 to nums2
	nums2 = nums1;

	std::cout << "After assigment:\n";
	display_sizes(nums1, nums2, nums3);

	// move assignment moves data from nums1 to nums3,
	// modifying both nums1 and nums3
	nums3 = std::move(nums1);

	std::cout << "After move assigment:\n";
	display_sizes(nums1, nums2, nums3);

	cout << "*****************" << endl;
	map<int, std::string> c = { { 1, "one" }, { 2, "two" }, { 3, "three" },
	{ 4, "four" }, { 5, "five" }, { 6, "six" } };
	// erase all odd numbers from c
	for (auto it = c.begin(); it != c.end();)
	{
		cout << it->first << endl;
		if (it->first % 2 == 1)
			it = c.erase(it);
		else
			++it;
	}

	for (auto& p : c)
		std::cout << p.second << ' ';
	cout << "************************" << endl;
	map<string, int> smap;
	smap[string("jjzhou")] = 1;
	smap[string("jerry")] = 2;
	smap[string("jason")] = 3;
	smap[string("jimmy")] = 4;

	pair<string, int> value(string("david"), 5);
	smap.insert(value);
	map<string, int>::iterator siter = smap.begin();
	for (; siter != smap.end(); ++siter)
		cout << siter->first << "  " << siter->second << endl;
	int number = smap[string("jason")];
	cout << number << endl;

	siter = smap.find(string("mchen"));
	if (siter == smap.end())
		cout << "mchan not found" << endl;

	siter = smap.find(string("jerry"));
	if (siter != smap.end())
		cout << "jerry found" << endl;

	siter->second = 9;
	cout << smap[string("jerry")] << endl;
	return 0;
}
//
//int main()
//{
//	int a[5] = { 1, 2, 3, 4, 5 };
//	vector<PNN> vec;
//	for (auto x : a)
//	{
//		auto p = pair<int, int>(x, x);
//		vec.push_back(p);
//	}
//	map<int, int> map0;
//	map<int, int> map1(vec.begin(), vec.end());
//	map<std::string, int> init{
//			{ "this", 100 },
//			{ "can", 100 },
//			{ "be", 100 },
//			{ "const", 100 },
//	};
////	map<int, int> map2{ PNN(1, 2), PNN(2, 2), PNN(3, 2), PNN(4, 2), PNN(5, 2) };
//	map<int, int> map3(std::move(map1));
//
//
//	return 0;
//}