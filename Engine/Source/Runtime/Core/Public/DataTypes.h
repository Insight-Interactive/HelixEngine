// Copyright 2021 Insight Interactive. All Rights Reserved.
/*
	File: DataTypes.h
	Source: None

	Author: Garrett Courtney

	Description: Contains core numeric types and defines used engine-wide.
*/
#pragma once



//-----------------------------------
//		Compiler Macros
//-----------------------------------
//
// 
#ifdef _MSC_VER
#	define HE_COMPILE_MSVC 1
#else 
#	define HE_COMPILE_MSVC 0
#endif

//-----------------------------------
//		Common Macros
//-----------------------------------
//
// 
// 
// Safely checks a pointer and deletes it if it is non-null.
#define HE_SAFE_DELETE_PTR( Ptr )			if( (Ptr) != NULL ) { delete	(Ptr); (Ptr) = NULL; }
#define HE_SAFE_DELETE_PTR_ARRAY( Ptr )		if( (Ptr) != NULL ) { delete[]	(Ptr); }
// Safely checks a COM pointer and deletes it if it is non-null.
#define HE_COM_SAFE_RELEASE( ComObject )	if( (ComObject) != nullptr ) { (ComObject)->Release(); (ComObject) = nullptr; }
// Turn characters into a string of text.
#define HE_STRINGIFY( Value )			#Value
// Force the compiler to inline a piece of code.
#if __clang__
#	define FORCEINLINE					inline
#else
#	define FORCEINLINE					__forceinline
#endif
// Hint the compiler to inline a piece of code
#define INLINE							inline 
// Expression that evaluates at compile time.
#define CEXPR							constexpr
// Value of zero.
#ifndef NULL
#	define NULL							0
#endif
// Alias for nullptr
#define null							nullptr
#if HE_COMPILE_MSVC
#	define HE_PRAGMA( X )					__pragma (X)
#else
#	define HE_PRAGMA( X )					_Pragma (X)
#endif
// Force the client to store the output of a function.
#define HE_NO_DISCARD					[[nodiscard]]
// Text unicode complience helper.
#define LONG_STR_( x )					L ## x
// Make a piece of text unicode complient.
#define LONG_STR( x )					LONG_STR_(x)
// Aligns a structure to a specified number of bytes.
#define HE_ALIGN( InBytes )				__declspec( align(InBytes) )
#define DLL_EXPORT						__declspec( dllexport )
#define DLL_IMPORT						__declspec( dllimport )
// Returns the size of an array.
#define HE_ARRAYSIZE(Arr)				( sizeof(Arr) / sizeof(Arr[0]) )
// The max path for a string of characters (analogous to microsoft's MAX_PATH).
#define HE_MAX_PATH						260
// Returns the required memory size in bytes for a given kilobyte value.
#define HE_KILOBYTES(Value)				( Value * 1024 )
// Returns the required memory size in bytes for a given megabyte value.
#define HE_MEGABYTES(Value)				( Value * HE_KILOBYTES(1024) )
// Returns the required memory size in bytes for a given gigabyte value.
#define HE_GIGABYTES(Value)				( Value * HE_MEGABYTES(1024) )
// Declare a class to be non-copyable and non-movable.
#define HE_DECL_NON_COPYABLE( Class )					\
		Class(const Class&)					= delete;	\
		Class(Class&&)						= delete;	\
		Class& operator = (const Class&)	= delete;	
// The max path for a string of characters (analogous to microsoft's MAX_PATH).
#define HE_MAX_PATH						260
#define HE_MAX(a, b)					( ((a) > (b)) ? (a) : (b) )
#define HE_MIN(a, b)					( ((a) > (b)) ? (b) : (a) )
#define HE_PRAGMA_DISABLE(PragmaCode, ...)			\
		HE_PRAGMA (warning (push))					\
		HE_PRAGMA (warning (disable : PragmaCode))	\
		__VA_ARGS__									\
		HE_PRAGMA (warning (pop))						
#define HE_StackAlloc(Size)				_malloca(Size)
#define HE_HeapAlloc(Size)				::malloc(Size)
#define HE_HeapFree(HeapPtr)			::free(HeapPtr)
#define HE_ReAlloc(HeapPtr, Size)		::realloc(HeapPtr, Size)
#define HE_UNUSED_PARAM(Value)			(void)Value;
#define HE_TERNARY(Expression, IfTrue, IfFalse) (Expression) ? (IfTrue) : (IfFalse)
// Creates a buffer and zeros the memory.
#define HE_CREATE_BUFFER(Type, Name, ArrayElements) Type Name[ArrayElements]; ZeroMemory(Name, sizeof(Name))

//-----------------------------------
//		Container Debug Levels
//-----------------------------------
//
#if HE_DEBUG_EDITOR
#	define HE_TARRAY_DEBUG_LEVEL	2
#	define HE_TDYNARRAY_DEBUG_LEVEL 2
#endif
#if HE_DEVELOPMENT || HE_DEBUG_GAME
#	define HE_TARRAY_DEBUG_LEVEL	1
#	define HE_TDYNARRAY_DEBUG_LEVEL 1
#endif
#if HE_SHIPPING
#	define HE_TARRAY_DEBUG_LEVEL	0
#	define HE_TDYNARRAY_DEBUG_LEVEL 0
#endif


//-----------------------------------
//		Numeric Values
//-----------------------------------
//
/*
	8-bit signed integer.
*/
typedef signed char int8;

/*
	8-bit unsigned integer.
*/
typedef unsigned char uint8;

/*
	16-bit signed integer.
*/
typedef short int16;

/*
	16-bit unsigned integer.
*/
typedef unsigned short uint16;

/*
	32-bit signed integer.
*/
typedef long int32;

/*
	32-bit unsigned integer.
*/
typedef unsigned long uint32;

/*
	64-bit signed integer.
*/
typedef long long int64;

/*
	64-bit unsigned integer.
*/
typedef unsigned long long uint64;



//-----------------------------------
//		Strings and Characters
//-----------------------------------
//
/*
	A UTF - 16 character.
*/
typedef wchar_t WChar;

/*
	A UTF - 8 character.
*/
typedef char	Char;

/*
	String using UTF-16 encoding.
*/
typedef std::wstring WString;

/*
	String using UTF-8 encoding.
*/
typedef std::string String;

/*
	View into a UTF-8 string.
*/
typedef std::string_view StringView;

/*
	View into a UTF-16 string.
*/
typedef std::wstring_view WStringView;



//-----------------------------------
//		System Text Manipulation
//-----------------------------------
//
//
#if _UNICODE
// A character that could be ASCII or Unicode depending on
// if unicode is supported in the build configuration.
typedef WChar TChar;
// A client facing string composed of TChar.
typedef WString HName;
typedef WStringView HNameView;
typedef std::wstringstream TStringStream;

#	define Printf(Fmt, ...)									::wprintf( Fmt, __VA_ARGS__)
#	define PrintBuffer(Buffer, Fmt, ...)					::swprintf_s( Buffer, Fmt, __VA_ARGS__)
#	define VSPrintBuffer(Buffer, BufferSize, Fmt, ArgList)	::_vsnwprintf_s( Buffer, BufferSize, Fmt, ArgList)
#	define TCharStrCpy( Destination, Source )				HE_PRAGMA_DISABLE( 4996, ::wcscpy( Destination, Source ) );
#	define TCharStrlen( Str )								::wcslen( Str )
#	define TCharStrCat( Destination, Source )				::lstrcatW( Destination, Source )
#	define TCharStrCmp( Str1, Str2 )						::wcscmp( Str1, Str2 )
#	define CharToTChar( Str )								StringHelper::UTF8ToUTF16( Str ).c_str()
#	define TCharToChar( Str )								StringHelper::UTF16ToUTF8( Str ).c_str()
#	define WCharToTChar( Str )								Str
#	define HE_FILE											LONG_STR( __FILE__ )
#	define HE_FUNCTION										LONG_STR( __FUNCTION__ )
#	define HE_TIME											LONG_STR( __TIME__ )
#	define HE_DATA											LONG_STR( __DATE__ )

#else
typedef Char TChar;
// A client facing UTF-8 string.
typedef String HName;
typedef StringView HNameView;
typedef std::stringstream TStringStream;

#	define Printf(Fmt, ...)									::printf( Fmt, __VA_ARGS__ )
#	define PrintBuffer(Buffer, Fmt, ...)					::sprintf_s( Buffer, Fmt, __VA_ARGS__ )
#	define VSPrintBuffer(Buffer, BufferSize, Fmt, ArgList)	::vsnprintf_s( Buffer, BufferSize, Fmt, ArgList )
#	define TCharStringCopy( Destination, Source )			::strcpy( Destination, Source )
#	define TCharStrLen( Str )								::strlen( Str )
#	define TCharStrCat( Destination, Source )				::strcat( Destination, Source )
#	define TCharStrCmp( Str1, Str2 )						::strcmp( Str1, Str2 )
#	define CharToTChar( Str )								StringHelper::UTF16ToUTF8( Str ).c_str()
#	define TCharToChar( Str )								( Str )
#	define WCharToTChar( Str )								StringHelper::UTF16ToUTF8( Str ).c_str()
#	define FILE												( __FILE__ )
#	define FUNCTION											( __FUNCTION__ )
#	define HE_TIME											( __TIME__ )
#	define HE_DATA											( __DATE__ )

#endif // _UNICODE

#if HE_WINDOWS
/*
	If the HRESULT fails a message box is presented to the user indicating the problem.
*/
#	define ThrowIfFailedMsg(Hr, Message)																					\
	if( FAILED(Hr) )																										\
	{																														\
		using namespace System;																								\
		WChar MsgBuffer[128];																								\
		ZeroMemory(MsgBuffer, sizeof(MsgBuffer));																			\
		::swprintf_s( MsgBuffer, L"%u - %s", Hr, LONG_STR( Message ) );														\
		CreateMessageBox( MsgBuffer, L"HRESULT Failed!", MessageDialogInput::MDI_Ok, MessageDialogIcon::MDIcon_Critical );	\
		HE_ASSERT( false );																									\
	}

/*
	Assert that an HRESULT succeeded with S_OK.
*/
#	define ASSERT_SUCCEEDED(Hr)																								\
	if( !SUCCEEDED(hr) )																									\
	{																														\
		using namespace System;																								\
		WChar MsgBuffer[128];																								\
		ZeroMemory(MsgBuffer, sizeof(MsgBuffer));																			\
		::swprintf_s( MsgBuffer, L"%u", Hr );																				\
		CreateMessageBox( MsgBuffer, L"HRESULT Failed!", MessageDialogInput::MDI_Ok, MessageDialogIcon::MDIcon_Critical );	\
		HE_ASSERT( false );																									\
	}

/*
	Reset an HRESULT to the default value.
*/
#	define ResetHr(Hr) (Hr = S_OK)

#endif


//---------------------------------------
//		Compiler Warning Supression
//---------------------------------------
//
//
#if HE_COMPILE_MSVC
#	define HE_DISABLE_MSVC_WARNINGS				\
	HE_PRAGMA ( warning ( push )	)			\
	HE_PRAGMA ( warning ( disable : 26812) )	 // The enum type '<SomeType>' is unscoped.Prefer 'enum class' over 'enum'


#	define HE_RESTORE_MSVC_WARNINGS \
	HE_PRAGMA ( warning ( pop ) )		
#else
#	define HE_DISABLE_MSVC_WARNINGS
#	define HE_RESTORE_MSVC_WARNINGS

#endif


#include "EnumHelper.h"
