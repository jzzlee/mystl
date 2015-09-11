#include<iostream>
#include "my_vector.h"
#include <numeric>

#include<list>

using namespace my_stl;
using std::list;
using std::cout;
using std::endl;

template<typename T>
void print(vector<T> vec, const int num)
{
	for (int i = 0; i != num; ++i)
		cout<<vec[i]<<" ";
	cout << endl;
}



int main()
{
	vector<int> vec;
	vector<int> vec1(10, 1);
	vector<int> vec2(vec1);
	vector<int> vec3 = vec2;
	print<int>(vec3, 10);
	vector<int> vec4(vec3.begin(), vec3.end());
	vector<int> vec5;
	vec5 = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
	vector<int> vec6;
	vec6 = std::move(vec5);

	vector<int> vec7(100);
	uninitialized_copy(vec1.begin(), vec1.end(), vec7.begin());
	uninitialized_copy_n(vec1.begin(), 10, vec7.begin() + 10);
	uninitialized_fill(vec7.begin() + 20, vec7.begin() + 30, 20);
	uninitialized_fill_n(vec7.begin() + 30, 10, 30);
	std::cout << "ok" << std::endl;
	print<int>(vec7, 40);


	cout << vec.size() << " " << vec.capacity() << endl;
	cout << vec1.size() << " " << vec1.capacity() << endl;
//	cout << vec2.size() << " " << vec2.capcity() << endl;
	
	print<int>(vec1, vec1.size());
	print<int>(vec2, vec2.size());
	try{
		vec2[10] = 2;
	}
	catch (std::out_of_range &x)
	{
		cout << x.what() << endl;
	}
	
	auto p = vec2.begin();
	++p;
	*p = 30;
	print<int>(vec1, vec1.size());
	print<int>(vec2, vec2.size());
	print<int>(vec4, vec4.size());
	std::cout << "vec5" << endl;
	print<int>(vec5, vec5.size());
	vec6.assign({});
	print<int>(vec6, vec6.size());
	vec6.assign({ 1, 2, 3 });
	print<int>(vec6, vec6.size());
	vec6.assign({ 1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 1, 2, 3, 4, 5 });
	print<int>(vec6, vec6.size());

	for (auto iter = vec4.begin(); iter != vec4.end(); ++iter)
		*iter += 1;
	cout << endl;
	int sum = std::accumulate(vec4.begin(), vec4.end(), 0);
	cout << "sum:" << sum << endl;  
	cout << vec2.end() - vec2.begin() << endl;
	vec2.~vector();

	const vector<int> vec8{ 1, 2, 3, 4 };
	vec8[1]; 
	cout << vec8.front() << " " << vec8.back() << " " << vec8.data() << endl;

	auto p7 = vec7.cbegin();
	++p7;
	cout << vec6.size() << " " << vec6.capacity() << endl;
	vec6.reserve(9);
	cout << vec6.size() << " " << vec6.capacity() << endl;
	print<int>(vec6, vec6.size());
	vec6.reserve(20);
	cout << vec6.size() << " " << vec6.capacity() << endl;
	print<int>(vec6, vec6.size());
	vec6.shrink_to_fit();
	cout << vec6.size() << " " << vec6.capacity() << endl;
	print<int>(vec6, vec6.size());
	auto p6 = vec6.insert(vec6.cbegin(), vec8.begin(), vec8.end());
	cout << *p6 << endl;
	print<int>(vec6, vec6.size());
	p6 = vec6.insert(vec6.cbegin() + 10, { 11, 12, 13, 14, 15 });
	cout << *p6 << endl;
	print<int>(vec6, vec6.size());
	p6 = vec6.insert(vec6.cend(), 2, 12);
	cout << *p6 << endl;
	print<int>(vec6, vec6.size());
	int a = 13;
	p6 = vec6.insert(vec6.cend(), std::move(a));
	cout << *p6 << endl;
	print<int>(vec6, vec6.size());




	return 0;
}


//#include <algorithm>
//#include <iostream>
//#include <string>
//#include <tuple>
//#include "my_vector.h"
//#include "my_memory.h"
//
//int main()
//{
//	my_stl::vector<std::string> v = { "This", "is", "an", "example" };
//	std::string* p;
//	std::size_t sz;
//	std::tie(p, sz) = std::get_temporary_buffer<std::string>(v.size());
//	sz = std::min(sz, v.size());
//	my_stl::uninitialized_copy(v.begin(), v.begin() + sz, p);
//	for (std::string* i = p; i != p + sz; ++i) {
//		std::cout << *i << ' ';
//		i->~basic_string<char>();
//	}
//	std::return_temporary_buffer(p);
//
//
//	std::tie(p, sz) = std::get_temporary_buffer<std::string>(v.size());
//	sz = std::min(sz, v.size());
//	my_stl::uninitialized_copy_n(v.begin(), sz, p);
//
//	for (std::string* i = p; i != p + sz; ++i) {
//		std::cout << *i << ' ';
//		i->~basic_string<char>();
//	}
//	std::return_temporary_buffer(p);
//
//
//	std::tie(p, sz) = std::get_temporary_buffer<std::string>(4);
//	my_stl::uninitialized_fill(p, p + sz, "Example");
//	for (std::string* i = p; i != p + sz; ++i) {
//		std::cout << *i << '\n';
//		i->~basic_string<char>();
//	}
//	std::return_temporary_buffer(p);
//
//
//	std::tie(p, sz) = std::get_temporary_buffer<std::string>(4);
//	my_stl::uninitialized_fill_n(p, sz, "Example");
//
//	for (std::string* i = p; i != p + sz; ++i) {
//		std::cout << *i << '\n';
//		i->~basic_string<char>();
//	}
//	std::return_temporary_buffer(p);
//
//
//
//}