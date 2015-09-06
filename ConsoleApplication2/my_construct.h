#ifndef __MY_CONSTRUCT_H_
#define __MY_CONSTRUCT_H_
namespace my_stl
{
	//global construct function
	template<typename T1, typename T2>
	inline void construct(T1 *p, const T2 &value)
	{
		new (p)T1(value);    //placement new
	}

	//global destroy function
	template<typename T>
	inline void destroy(T *p)
	{
		p->~T();    //call destructor of class T
	}


}

#endif
