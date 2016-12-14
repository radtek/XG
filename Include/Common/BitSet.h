#ifndef __BITSET_H__
#define __BITSET_H__

#include <boost/type_traits.hpp>
#include <boost/static_assert.hpp>
#include <boost/utility/enable_if.hpp>

template<typename A,typename Enable=void>class TBitSet
{ 
public:
	TBitSet()
	{
		/*BOOST_STATIC_ASSERT( 1>2 );*/
		int *p = 0;
		*p = 1;
	} 
};

template<typename A>class TBitSet<A,typename boost::enable_if_c< boost::is_unsigned<A>::value >::type >
{
private:
	A mbits;
public:
	TBitSet()                          { mbits = 0; }
	TBitSet(const TBitSet& in_rCopy)   { mbits = in_rCopy.mbits; }
	TBitSet(const A in_mask)           { mbits = in_mask; }

	operator A() const                 { return mbits; }
	A getMask() const                  { return mbits; }

	//设置所有位都为1
	void set()                         { mbits  = (A)0xFFFFFFFFUL; }

	/// 设置特殊位为1
	void set(const A m)                { mbits |= m; }

	void set(TBitSet s, bool b)        { mbits = (mbits&~(s.mbits))|(b?s.mbits:0); }

	//清除所有位
	void clear()                       { mbits  = 0; }

	//清除特殊位
	void clear(const A m)              { mbits &= ~m; }

	/// Toggle the specified bit(s).
	void toggle(const A m)             { mbits ^= m; }

	/// Are any of the specified bit(s) set?
	bool test(const A m) const         { return (mbits & m) != 0; }

	/// Are ALL the specified bit(s) set?
	bool testStrict(const A m) const   { return (mbits & m) == m; }

	/// @name Operator Overloads
	/// @{
	TBitSet& operator =(const A m)  { mbits  = m; return *this; }
	TBitSet& operator|=(const A m)  { mbits |= m; return *this; }
	TBitSet& operator&=(const A m)  { mbits &= m; return *this; }
	TBitSet& operator^=(const A m)  { mbits ^= m; return *this; }

	TBitSet operator|(const A m) const { return TBitSet(mbits | m); }
	TBitSet operator&(const A m) const { return TBitSet(mbits & m); }
	TBitSet operator^(const A m) const { return TBitSet(mbits ^ m); }
	/// @}
};

typedef unsigned long long	U64;
typedef unsigned int		U32;
typedef unsigned short		U16;
typedef unsigned char		U8;
typedef TBitSet<U64>		BitSet64;
typedef TBitSet<U32>		BitSet32;
typedef TBitSet<U16>		BitSet16;
typedef TBitSet<U8>			BitSet8;


//支持更多的位标识
template<unsigned int max_bit=80>
class TBigBitSet
{
private:
	unsigned char  m_buffer[max_bit/8+1];
	unsigned int   m_size;
public:
	TBigBitSet()
	{
		m_size = sizeof(m_buffer);
		memset(m_buffer,0,m_size);
	}
	TBigBitSet( unsigned char *buffer,unsigned int buffer_size )
	{
		m_size = sizeof(m_buffer);
		memset(m_buffer,0,m_size);
		if( m_size>=buffer_size )
		{
			memcpy(m_buffer,buffer,buffer_size );
		}
	}
	~TBigBitSet()
	{
	}

	unsigned char* get_buffer(){ return m_buffer; }
	unsigned int   get_buffer_size(){ return sizeof(m_buffer); }

	//设置所有位都为1
	void set(){ memset(m_buffer,0XFF,m_size); }

	/// 设置特殊位为1
	void set(unsigned int bitpos)      
	{
		unsigned int index = bitpos/8;
		if( index>=m_size )
		{
			//APPLOG_ERROR(FUN_FILE_LINE);
			return;
		}

		unsigned char    v  = 1<<(bitpos%8);
		unsigned char &  cv = m_buffer[index];
		cv |= v;
	}

	//清除所有位
	void clear(){ memset(m_buffer,0,m_size); }
	//清除特殊位
	void clear(unsigned int bitpos)    
	{
		unsigned int index = bitpos/8;
		if( index>=m_size )
		{
			//APPLOG_ERROR(FUN_FILE_LINE);
			return;
		}
		unsigned char    v  = 1<<(bitpos%8);
		unsigned char &  cv = m_buffer[index];
		cv &= ~v;
	}

	/// Are any of the specified bit(s) set?
	bool test( unsigned int bitpos) const  
	{
		unsigned int index = bitpos/8;
		if( index>=m_size )
		{
			//APPLOG_ERROR(FUN_FILE_LINE);
			return false;
		}
		const unsigned char cv = m_buffer[index];  
		unsigned char    v     = 1<<(bitpos%8);
		return (cv & v) != 0;
	}

	/********************************************************************
	created:	2010-9-8   14:38 author: Zhanghongtao
	purpose:	返回bitposbegin-->bitposend的值;包括bitposbegin,bitposend

	            如果失败, 返回 0XFFFFFFFF

	eg: 1011100110110111101;
	get_value(0,2);  返回 第 0,1,2位的值 101=5;
	get_value(6,10); 返回 第 6,7,8,9,10 位的值 01101=13;

	*********************************************************************/
	unsigned int get_value( unsigned int bitposbegin,unsigned int bitposend ) const  
	{
		if( bitposbegin>bitposend )
			return 0XFFFFFFFF;

		unsigned int uretv = 0;
		for( unsigned int t=0; (t+bitposbegin)<=bitposend; ++t )
		{
			uretv <<= 1;
			if( test(t+bitposend) )
			{
				++uretv;
			}
		}
		return uretv;
	}
};
/********************************************************************
	created:	2010-9-8   15:42 author: Zhanghongtao
	purpose:	example

	void example_TBigBitSet()
	{
		TBigBitSet<50> bitset;
		bitset.set( 3 );
		bitset.set( 5 );
		bitset.set( 7 );

		unsigned int v =0;
		v = bitset.get_value(0,1);
		v = bitset.get_value(1,1);
		v = bitset.get_value(1,5);
	}
*********************************************************************/

#endif