#ifndef F00l_TINY_STL_CONSTRUC_H
#define F00l_TINY_STL_CONSTRUC_H

#include <new>

#include "stl_config.h"
#include "type_traits.h"
#include "stl_iterator_base.h"

__STL_BEGIN_NAMESPACE
    template<class _T1, class _T2>
    inline void _Construct(_T1 *__p, _T2 &__value) {
        new ((void *) __p) _T1(__value);
    }

    template<class _T1>
    inline void _Construct(_T1 *__p) {
        new ((void *) __p) _T1();
    }

    template<class _Tp>
    inline void _Destroy(_Tp* __p) {
    __p->~_Tp();
    }

    template<class _ForwardIter>
    void __destroy_aux(_ForwardIter __first, _ForwardIter __last, __false_type){
        for(; __first != __last; ++__first)
            destroy(&*__first);
    }

    template<class _ForwardIter>
    void __destroy_aux(_ForwardIter __first, _ForwardIter __last, __true_type) {}

    template<class _ForwardIter, class _Tp>
    inline void
    __destroy(_ForwardIter __first, _ForwardIter __last, _Tp *)
    {
        typedef typename __type_traits<_Tp>::has_trival_destructor _Trival_destructor;
        __destroy_aux(__first, __last, _Trival_destructor());
    }

    template<class _ForwardIter>
    inline void _Destroy(_ForwardIter __first, _ForwardIter __last) {
        __destroy(__first, __last, __VALUE_TYPE(__first));
    }

    inline void _Destroy(char *, char *) {}

    inline void _Destroy(int *, int *) {}

    inline void _Destroy(float *, float *) {}

    inline void _Destroy(long *, long *) {}

    inline void _Destroy(double *, double *) {}

    template<class _Tp>
    inline void construct(_Tp *__p) {
        _Construct(__p);
    }

    template<class _T1, class _T2>
    inline void construct(_T1 *__p, const _T2 &__value) {
        _Construct(__p, __value);
    }

    template<class _Tp>
    inline void destroy(_Tp *__p) {
        _Destroy(__p);
    }

    template<class _ForwardIterator>
    inline void destroy(_ForwardIterator __first, _ForwardIterator __last) {
        _Destroy(__first, __last);
    }

__STL_END_NAMESPACE
#endif //F00l_TINY_STL_CONSTRUC_H