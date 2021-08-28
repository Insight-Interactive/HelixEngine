#pragma once

//#include "CoreFwd.h"
#include <memory>
template <typename T>
using Ref = std::shared_ptr<T>;
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
