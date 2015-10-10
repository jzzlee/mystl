#include "my_vector.h"
#include "my_hash_tables.h"
#include <unordered_map>

using namespace my_stl;
using std::cout;
using std::endl;

struct hashnode
{
	hashnode *next;
	int v;
};

int main()
{
	vector<hashnode *> vec(10);
	auto alloc = allocator<int>();
	hash_table<int, int, std::hash<int>, std::identity<int>, std::equal_to<int> > hash(alloc);
	cout << hash.max_load_factor() << endl;

	auto p = hash.begin();
	for (auto x : { 1, 2, 3, 4,  5, 1, 2, 3, 4, 5 })
	{
		p = hash.insert_unique(p, x);
		
		cout << p.node->value << endl;
	}
	cout << endl;
	for (auto x : { 1, 2, 3, 4, 5, 1, 2, 3, 4, 5 })
	{
		auto sp = hash.insert_equal(int(x));
	}
	auto it1 = hash.insert_equal(1);
	auto it2 = hash.insert_equal(2);

	for (auto it = hash.begin(); it != hash.end(); ++it)
		cout << it.node << " " << *it << endl;
	cout << endl;
	cout << it1.node << " " << it2.node << endl;
	hash.erase(it1, it2);
	cout << hash.find(5).node << endl;
	cout << hash.equal_range(1).first.node << endl;
	cout << hash.equal_range(1).second.node << endl;
	cout << hash.equal_range(5).first.node << endl;
	cout << hash.equal_range(5).second.node << endl;
	cout << hash.erase(5) << endl;
	for (auto it = hash.begin(); it != hash.end(); ++it)
		cout << *it << " ";
	cout << endl;
	cout << hash.erase(1) << endl;
	for (auto it = hash.begin(); it != hash.end(); ++it)
		cout << *it << " ";
	const auto hash1(hash);
	cout << hash1.equal_range(1).first.node << endl;
	cout << hash1.equal_range(1).second.node << endl;
	cout << hash1.equal_range(5).first.node << endl;
	cout << hash1.equal_range(5).second.node << endl;
	cout << endl;
	
	return 0;
}