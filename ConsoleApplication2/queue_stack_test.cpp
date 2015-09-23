#include <iostream>
#include "my_queue.h"
#include "my_stack.h"
using std::cout;
using std::endl;
using namespace my_stl;

int main()
{
	vector<int> vec{ 1, 2, 3 };
	stack<int> s1(vec);
	stack<int> s2;
	cout << (s2 < s1) << endl;

	for (int i = 0; i != 9; ++i)
		s1.push(i);
	for (int i = 0; i != 9; ++i)
	{
		cout << s1.top() << endl;
		s1.pop();
	}
	list<int> lst1{ 1, 2, 3, 4 };
	queue<int> q1(lst1);
	queue<int> q2(q1);
	cout << (q2 < q1) << endl;

	return 0;
}