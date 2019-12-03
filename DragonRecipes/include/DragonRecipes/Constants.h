/*--------------------------------------------------------------------------
   Copyright (c) 2019 Mark Elrod. All rights reserved. This file is
   distributed under the MIT License. See LICENSE.TXT for details.
  --------------------------------------------------------------------------*/

#ifndef DR_CONSTANTS_H
#define DR_CONSTANTS_H

#ifdef DRAGON_RECIPES_LIB_BUILD
    #ifdef _WIN32
        #define DRAGON_EXPORT __declspec(dllexport)
        #define DRAGON_LOCAL
    #else
        #define DRAGON_EXPORT __attribute__((visibility("default")))
        #define DRAGON_LOCAL __attribute__((visibility("hidden")))
    #endif
#else
    #ifdef _WIN32
        #define DRAGON_EXPORT __declspec(dllimport)
        #define DRAGON_LOCAL
    #else
        #define DRAGON_EXPORT
        #define DRAGON_LOCAL
    #endif
#endif

namespace dragon {
	template <class T>
class DRAGON_EXPORT UniquePtr {
	public:
	UniquePtr(T* ptr) : ptr(ptr) {
	}

	/*
	UniquePtr(UniquePtr<T> &p) {
		T *tmp = p.ptr;
		p.ptr = ptr;
		ptr = tmp;
	}
*/
	
	
	~UniquePtr();
	
	T *&get() {
		return ptr;
	}

	T * const&get() const {
		return ptr;
	}
	
		T *operator->() {
		return ptr;
	}

	T * const operator->() const {
		return ptr;
	}
	
	
	private:
	T *ptr;
};


#define UNIQUE_PTR_IMPL(a)\
			template<>\
	UniquePtr<a>::~UniquePtr() {\
		delete ptr;\
		ptr = nullptr;\
	}

	
	
}

#endif
