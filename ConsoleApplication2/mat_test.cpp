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

void display_sizes(const multimap<int, int> &nums1,
	const multimap<int, int> &nums2,
	const multimap<int, int> &nums3)
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
	multimap<int, int> nums1{ { 3, 1 }, { 4, 1 }, { 5, 9 },
	{ 6, 1 }, { 7, 1 }, { 8, 9 } };
	multimap<int, int> nums2;
	multimap<int, int> nums3;

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
	c.print();
	// erase all odd numbers from c
	int counts = 0;
	for (auto it = c.begin(); it != c.end();)
	{
		cout << it->first << endl;
		if (it->first % 2 == 1)
			it = c.erase(it);
		else
			++it;
	}
	for (auto it = c.begin(); it != c.end(); ++it)
		cout << it->first << endl;
	for (auto& p : c)
		std::cout << p.second << ' ';
	cout << "************************" << endl;
	cout << "************************" << endl;
	multimap<string, int> smultimap;
	smultimap.insert(std::pair<std::string, int>(string("jjzhou"), 1));
	smultimap.insert(std::pair<std::string, int>(string("jjzhou"), 2));
	smultimap.insert(std::pair<std::string, int>(string("jjzhou"), 3));
	smultimap.insert(std::pair<std::string, int>(string("jjzhou"), 4));
	smultimap.insert(std::pair<std::string, int>(string("shenzhou"), 5));


	auto ps = smultimap.equal_range("jjzhou");
	cout << ps.first->second << " " << ps.second->second << endl;

	pair<string, int> value(string("david"), 5);
	smultimap.insert(value);
	multimap<string, int>::iterator siter = smultimap.begin();
	for (; siter != smultimap.end(); ++siter)
		cout << siter->first << "  " << siter->second << endl;


	siter = smultimap.find(string("mchen"));
	if (siter == smultimap.end())
		cout << "mchan not found" << endl;

	siter = smultimap.find(string("jerry"));
	if (siter != smultimap.end())
		cout << "jerry found" << endl;

	siter->second = 9;

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
//	multimap<int, int> multimap0;
//	multimap<int, int> multimap1(vec.begin(), vec.end());
//	multimap<std::string, int> init{
//			{ "this", 100 },
//			{ "can", 100 },
//			{ "be", 100 },
//			{ "const", 100 },
//	};
////	multimap<int, int> multimap2{ PNN(1, 2), PNN(2, 2), PNN(3, 2), PNN(4, 2), PNN(5, 2) };
//	multimap<int, int> multimap3(std::move(multimap1));
//
//
//	return 0;
//}