// Copyright 2024 Insight Interactive. All Rights Reserved.
#pragma once
//
//template <typename DataType>
//Ref<DataType>::Ref(const Ref<DataType>& ref)
//	: m_Data(ref.m_Data)
//{
//	if (m_Data != nullptr)
//		++m_ReferenceCount;
//}
//
//template <typename DataType>
//Ref<DataType>::Ref(DataType* tex) : m_Data(tex)
//{
//	if (m_Data != nullptr)
//		++m_ReferenceCount;
//}
//
//template <typename DataType>
//Ref<DataType>::~Ref()
//{
//	if (m_Data != nullptr && --m_ReferenceCount == 0)
//		m_Data->Unload();
//}
//
//template <typename DataType>
//void Ref<DataType>::operator= (std::nullptr_t)
//{
//	if (m_Data != nullptr)
//		--m_ReferenceCount;
//
//	m_Data = nullptr;
//}
//
//template <typename DataType>
//void Ref<DataType>::operator= (Ref<DataType>& rhs)
//{
//	if (m_Data != nullptr)
//		--m_ReferenceCount;
//
//	m_Data = rhs.m_Data;
//
//	if (m_Data != nullptr)
//		++m_ReferenceCount;
//}
//
//template <typename DataType>
//bool Ref<DataType>::IsValid() const
//{
//	return m_Data != NULL;
//}
//
//template <typename DataType>
//DataType* Ref<DataType>::Get()
//{
//	return m_Data;
//}
//
//template <typename DataType>
//const DataType* Ref<DataType>::operator->() const
//{
//	HE_ASSERT(m_Data != nullptr);
//	return m_Data;
//}
//
//template <typename DataType>
//void Ref<DataType>::Reset()
//{
//	if (IsValid())
//	{
//		delete m_Data;
//		m_ReferenceCount = 0;
//	}
//}
//
//template <typename DataType>
//template <typename ... InitArgs>
//inline static Ref<DataType> Ref<DataType>::MakeRef(InitArgs ... Args)
//{
//	Ref<DataType> NewRef;
//	NewRef.m_Data = new DataType(Args...);
//	return NewRef;
//}
