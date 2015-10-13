#include "my_vector.h"
#include "my_hash_tables.h"
#include "my_unordered_set.h"
#include "my_unordered_map.h"
#include <cstring>

using namespace my_stl;
using std::cout;
using std::endl;

struct eqstr
{
	bool operator()(const char* s1, const char *s2) const
	{
		return strcmp(s1, s2) == 0;
	}
};

int main()
{
//	vector<hashnode *> vec(10);
//	auto alloc = allocator<int>();
//	hash_table<int, int, std::hash<int>, std::identity<int>, std::equal_to<int> > hash(alloc);
//	cout << hash.max_load_factor() << endl;
//
//	auto p = hash.begin();
//	for (auto x : { 1, 2, 3, 4,  5, 1, 2, 3, 4, 5,6,7,8,9 })
//	{
//		p = hash.insert_unique(p, x);
//		
//		cout << p.node->value << endl;
//	}
//	cout << endl;
//
//	for (auto x : { 1, 2, 3, 4, 5, 1, 2, 3, 4, 5 })
//	{
//		hash.insert_equal(int(x));
//	}
//	
//	for (int i = 0; i != hash.bucket_count(); ++i)
//		cout << hash.bucket_size(i) << " ";
//	cout << endl;
//
//	cout << "*********************" << endl;
//
//	for (int i = 0; i != hash.bucket_count(); ++i)
//	{
//		cout << i << " has elements: ";
//		for (auto it = hash.cbegin(i); it != hash.cend(i); ++it)
//			cout << *it << " ";
//		cout << endl;
//	}
//	cout << "*********************" << endl;
//	cout << "buckets_size: " << hash.bucket_count() << endl;
//	auto it1 = hash.insert_equal(1);
//	auto it2 = hash.insert_equal(2);
//
//	for (auto it = hash.begin(); it != hash.end(); ++it)
//		cout << it.node << " " << *it << endl;
//	cout << endl;
//	cout << it1.node << " " << it2.node << endl;
//	hash.erase(it1, it2);
//	cout << hash.find(5).node << endl;
//	cout << hash.equal_range(1).first.node << endl;
//	cout << hash.equal_range(1).second.node << endl;
//	cout << hash.equal_range(5).first.node << endl;
//	cout << hash.equal_range(5).second.node << endl;
//	cout << hash.erase(5) << endl;
//	for (auto it = hash.begin(); it != hash.end(); ++it)
//		cout << *it << " ";
//	cout << endl;
//	cout << hash.erase(1) << endl;
//	for (auto it = hash.begin(); it != hash.end(); ++it)
//		cout << *it << " ";
//	auto hash1(hash);
//	cout << hash1.equal_range(1).first.node << endl;
//	cout << hash1.equal_range(1).second.node << endl;
//	cout << hash1.equal_range(5).first.node << endl;
//	cout << hash1.equal_range(5).second.node << endl;
//	cout << hash1.find(2).node << endl;
//	cout << endl;
//	for (auto it = hash.begin(); it != hash.end(); ++it)
//		cout << *it << " ";
//	cout << endl;
//	for (auto it = hash1.begin(); it != hash1.end(); ++it)
//		cout << *it << " ";
//	cout << endl;
//	cout << (hash == hash1) << endl;
//	hash1.erase(hash1.begin());
//	cout << (hash == hash1) << endl;
//	for (auto it = hash.begin(); it != hash.end(); ++it)
//		cout << *it << " ";
//	cout << endl;
//	for (auto it = hash1.begin(); it != hash1.end(); ++it)
//		cout << *it << " ";
//	cout << endl;
//	hash.insert_equal(1);
//	cout << (hash == hash1) << endl;
//
//	cout << "********************************" << endl;
//
//	hash_table<int, int, std::hash<int>, std::identity<int>, std::equal_to<int> > ht(50, std::hash<int>(), std::identity<int>(), std::equal_to<int>());
//	cout << ht.size() << endl;
//	cout << ht.bucket_count() << endl;
//	cout << ht.max_bucket_count() << endl;
//	ht.insert_unique(59);
//	ht.insert_unique(63);
//	ht.insert_unique(108);
//	ht.insert_unique(2);
//	ht.insert_unique(53);
//	ht.insert_unique(55);
//	cout << ht.size() << endl;
//	
//	cout << endl;
//
////	hash_table<int, int, std::hash<int>, std::identity<int>, std::equal_to<int> > hashtable(10);
//	unordered_multiset<int> set1;
//	unordered_multiset<int> set2({ 1, 2, 3, 4, 5 });
//	auto set3(set2);
//	auto set4(std::move(set3));
//	auto p1 = set1.begin();
//	for (auto x : { 1, 2, 3, 4,  5, 1, 2, 3, 4, 5,6,7,8,9 })
//	{
//		p1 = set1.insert(p1, x);
//		cout << p1.node->value << endl;
//	}
//	cout << endl;
//	for (auto it = set1.begin(); it != set1.end(); ++it)
//		cout << *it << " ";
//	cout << endl;
//	set1.erase(set1.begin());
//	cout << set1.erase(9) << endl;;
//	for (auto it = set1.begin(); it != set1.end(); ++it)
//		cout << *it << " ";
//	cout << endl;
//

	unordered_multimap<const char*, int, std::hash<const char*>, eqstr> days{ {"yiyue", 31} };
	days.insert({ "january", 31 });
	days.insert({ "january", 29 });
	days.insert({ "january", 30 });
	days.insert({ "june", 31 });
	days.insert({ "january", 29 });
	days.insert({ "january", 30 });	days.insert({ "january", 31 });
	days.insert({ "january", 29 });
	days.insert({ "january", 30 });
	//days["february"] = 28;
	//days["march"] = 31;
	//days["april"] = 30;
	//days["may"] = 31;
	//days["june"] = 30;
	//days["juny"] = 31;
	//days["august"] = 31;
	//days["september"] = 30;
	//days["october"] = 31;
	//days["november"] = 30;
	//days["december"] = 31;

	//cout << "september -> " << days["september"] << endl;
	//cout << "june -> " << days["june"] << endl;
	//cout << "february -> " << days["february"] << endl;
	//cout << "december -> " << days["december"] << endl;

	days.insert({ "shisan", 30 });
	for (auto it = days.begin(); it != days.end(); ++it)
		cout << it->first << " " << it->second << endl;
	return 0;
}