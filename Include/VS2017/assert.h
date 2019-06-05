/*!
@copyright

	Copyright (c) 2019, MinnowWare. All rights reserved.
	This program and the accompanying materials are licensed and made
	available under the terms and conditions of the BSD License
	which accompanies this distribution.  The full text of the license
	may be found at

	http://opensource.org/licenses/bsd-license.php

	THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
	WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

@file
	assert.h

@brief
	standard C header file for CdePkg
@todo
*/
//#pragma warning( disable : 4996 4311 4312 4101)
#ifndef _CDE_ASSERT_H_
#define _CDE_ASSERT_H_
#include <stddef.h>

#undef assert

#ifdef NDEBUG

	#define assert(exp) ((void)0)

#else
	#define _CDE_CRT_WIDE_(s) L ## s
	#define _CDE_CRT_WIDE(s) _CDE_CRT_WIDE_(s)

	void __cdecl _wassert(wchar_t const* _Message, wchar_t const* _File,unsigned _Line);
	#define assert(exp) (void)((!!(exp)) || (_wassert(_CDE_CRT_WIDE(#exp), _CDE_CRT_WIDE(__FILE__), (unsigned)(__LINE__)), 0))

	//removed _assert() used in older CRT versions, support _wassert() only
	//#define assert(exp) (void)( (!!(exp)) || (_assert(#exp, __FILE__, __LINE__), 0) )

#endif

#endif//_CDE_ASSERT_H_
