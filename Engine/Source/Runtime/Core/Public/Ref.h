// Copyright 2024 Insight Interactive. All Rights Reserved.
#pragma once

//#include "CoreFwd.h"
#include <memory>
//using Ref = std::shared_ptr<T>;
template <typename T>
class CORE_API Ref
{
public:
	template <typename ... Args>
	Ref( Args ... args )
	{
		m_Ptr = std::make_shared<T>( args... );
	}

	inline void Reset(T* pOther)
	{
		m_Ptr.reset( pOther );
	}
	
	inline void Reset( )
	{
		m_Ptr.reset(  );
	}

	inline T* Get()
	{
		return m_Ptr.get();
	}

	inline const T* Get() const
	{
		return m_Ptr.get();
	}

	inline bool operator != ( const Ref<T>& rhs )
	{
		return m_Ptr != rhs.m_Ptr;
	}

	inline T* operator ->()
	{
		return Get();
	}

private:
	std::shared_ptr<T> m_Ptr;
};

//
//template <typename DataType>
//class CORE_API Ref
//{
//public:
//	Ref(const Ref& ref);
//	Ref(DataType* tex = nullptr);
//	~Ref();
//	Ref<DataType>& operator=(const Ref& Other)
//	{
//		this->m_Data = Other.m_Data;
//		return *this;
//	}
//
//	void operator= (std::nullptr_t);
//	void operator= (Ref& rhs);
//
//	// Check that this points to a valid texture (which loaded successfully)
//	bool IsValid() const;
//
//	// Get the texture pointer.  Client is responsible to not dereference
//	// null pointers.
//	DataType* Get();
//
//	const DataType* operator->() const;
//
//	void Reset();
//
//	template <typename ... InitArgs>
//	inline static Ref<DataType> MakeRef(InitArgs ... Args);
//
//private:
//	DataType* m_Data;
//	uint64 m_ReferenceCount;
//};

#include "Ref.inl"
