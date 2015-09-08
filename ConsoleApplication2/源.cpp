#include<iostream>
#include "my_vector.h"
#include <numeric>
#include <utility>
using namespace my_stl;
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
	vector<int> vec5{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
	vector<int> vec6 = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };


	cout << vec.size() << " " << vec.capcity() << endl;
	cout << vec1.size() << " " << vec1.capcity() << endl;
//	cout << vec2.size() << " " << vec2.capcity() << endl;
	
	print<int>(vec1, vec1.size());
	print<int>(vec2, vec2.size());
	vec2[1] = 2;
	print<int>(vec4, vec4.size());
	print<int>(vec5, vec5.size());


	for (auto iter = vec4.begin(); iter != vec4.end(); ++iter)
		*iter += 1;
	cout << endl;
	int sum = std::accumulate(vec4.begin(), vec4.end(), 0);
	cout << "sum:" << sum << endl;  
	cout << vec2.end() - vec2.begin() << endl;
	vec2.~vector();
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