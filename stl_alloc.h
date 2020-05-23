#ifndef F00l_TINY_STL_ALLOC_H
#define F00l_TINY_STL_ALLOC_H

#include<cstddef>
#include<cstdlib>
#include<cstdio>

#include "stl_config.h"
#include "stl_construct.h"

__STL_BEGIN_NAMESPACE
    template<int __inst>
    class __malloc_alloc_template {
    public:
        static void *allocate(size_t __n) {
            void *__result = malloc(__n);
            if(nullptr == __result) {
                fprintf(stderr,"alloc failed\n");
                exit(1);
            }
            return __result;
        }
        static void deallocate(void *__ptr, size_t /* __n */) {
            free(__ptr);
        }
        static void *reallocate(void *__ptr, size_t /* __oid_size */, size_t __new_size) {
            __ptr = realloc(__ptr, __new_size);
            if(__ptr == 0) {
                fprintf(stderr,"alloc failed\n");
                exit(1);
            }
            return __ptr;
        }
    };

    typedef __malloc_alloc_template<0> malloc_alloc;

    template<class _Tp, class _Alloc>
    class simple_alloc {
    public:
        static _Tp *allocate(size_t __n) {
            return __n == 0 ? 0 : (_Tp *) _Alloc::allocate(__n * sizeof(_Tp));
        }

        static _Tp *allocate(void) {
            return (_Tp *) _Alloc::allocate(sizeof(_Tp));
        }

        static void deallocate(_Tp *__ptr, size_t __n) {
            if(__n != 0) {
                _Alloc::deallocate(__ptr, __n * sizeof(_Tp));
            }
        }

        static void deallocate(_Tp *__ptr) {
                _Alloc::deallocate(__ptr, sizeof(_Tp));
        }
    };
    typedef malloc_alloc alloc;

__STL_END_NAMESPACE
#endif //F00l_TINY_STL_ALLOC_H