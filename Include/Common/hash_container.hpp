#ifndef __HASH_CONTAINER_HEADER_FILE
#define __HASH_CONTAINER_HEADER_FILE

/* omp::common::hash_map,omp::common::hash_set 对自定义类型有以下要求：
Windows 上的 VS2005,VS2008 要求提供
operator size_t() const
bool operator < (const T & o) const

在 Linux 上 gcc ，要求提供
operator size_t() const
bool operator == (const T & o) const
*/

#if defined _MSC_VER
#include <hash_map>
#include <hash_set>

namespace SgsStl
{
	using stdext::hash_set;
	using stdext::hash_map;

	template <class _Key>
	struct hash_traits
	{
		static const size_t bucket_size = 4;
		static const size_t min_buckets = 8;
		size_t operator () (const _Key & o) const
		{
			return static_cast<size_t>(o);
		}
		bool operator () (const _Key & a,const _Key & b) const
		{
			return a < b;
		}
	};
}

#elif defined __GNUC__

#include <ext/hash_map>
#include <ext/hash_set>
#include <string>
#include <boost/static_assert.hpp>
using namespace std;
namespace SgsStl
{
	template <class T>
	struct hash
	{
		size_t operator()(const T & o) const
		{
			return static_cast<size_t>(o);
		}
	};

	template <>
	struct hash<char *>
	{
		size_t operator()(const char* __s) const
		{
			return __gnu_cxx::__stl_hash_string(__s);
		}
	};

	template <>
	struct hash<const char *>
	{
		size_t operator()(const char* __s) const
		{
			return __gnu_cxx::__stl_hash_string(__s);
		}
	};

	template <>
	struct hash<std::string>
	{
		size_t operator()(const std::string & __s) const
		{
			return __gnu_cxx::__stl_hash_string(__s.c_str());
		}
	};

#if defined(OMP_UNICODE)

	inline size_t __stl_hash_wstring(const wchar_t* __s)
	{
		BOOST_STATIC_ASSERT(sizeof(wchar_t)==2);
		size_t __h = 0; 
		for ( ; *__s; ++__s)
			__h = 5*__h + *__s;

		return size_t(__h);
	};

	template <>
	struct hash<wchar_t *>
	{
		size_t operator()(const wchar_t* __s) const
		{
			return __stl_hash_wstring(__s);
		}
	};

	template <>
	struct hash<const wchar_t *>
	{
		size_t operator()(const wchar_t* __s) const
		{
			return __stl_hash_wstring(__s);
		}
	};

	template <>
	struct hash<std::wstring>
	{
		size_t operator()(const std::wstring & __s) const
		{
			return __stl_hash_wstring(__s.c_str());
		}
	};
#endif

	template <class _Key>
	struct hash_traits
	{
		size_t operator () (const _Key & o) const
		{
			hash<_Key> h;
			return h(o);
		}
		bool equal(const _Key & a,const _Key & b) const
		{
			return a == b;
		}
	};

	template <class _Key,class _Traits>
	struct equal_from_traits
	{
		bool operator()(const _Key & a,const _Key & b) const
		{
			return _Traits().equal(a,b);
		}
	};

	template<class _Key, class _Tp, class _Traits  = hash_traits<_Key>,class _Alloc = std::allocator<std::pair<_Key,_Tp> > >
	class hash_map:
		public __gnu_cxx::hash_map<_Key,_Tp,_Traits,equal_from_traits<_Key,_Traits> , typename _Alloc::template rebind<_Tp>::other >
	{

	};
	template <class _Value, class _Traits  = hash_traits<_Value>,class _Alloc = std::allocator<_Value>  >
	class hash_set:
		public __gnu_cxx::hash_set<_Value,_Traits,equal_from_traits<_Value,_Traits>,_Alloc>
	{

	};
}

#else

# error "Unknown compiler, only support for msvc and gcc."

#endif

#endif
