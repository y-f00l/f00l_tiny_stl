#ifndef F00l_TINY_STL_CONSTRUC_H
#define F00l_TINY_STL_CONSTRUC_H

#include <new>

#include "stl_config.h"
#include "type_traits.h"

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
    void destroy_one(_Tp *, __true_type) {};

    template<class _Tp>
    void destroy_one(_Tp *__p, __false_type) {
        if(__p != nullptr) {
            __p->~Tp();
        }
    }

    template<class _Tp>
    void _Destroy(_Tp *__p) {
        destroy_one(*__p, typename __type_traits<_Tp>::has_trival_destructor());
    }

    template<class _ForwardIterator>
    void _destroy_aux(_ForwardIterator __first, _ForwardIterator __last, __false_type) {
        for(;__first != __last;__first++)
            _Destroy(&*__first);
    }

    template<class _ForwardIterator>
    inline void _destroy_aux(_ForwardIterator, _ForwardIterator, __true_type) {}

    template<class _ForwardIterator, class _Tp>
    inline void _destroy(_ForwardIterator __first, _ForwardIterator __last,_Tp *) {
        _destroy_aux(__first, __last,typename __type_traits<_Tp>::has_trival_destructor());
    }

    template<class _ForwardIterator>
    inline void _Destroy(_ForwardIterator __first,_ForwardIterator __last) {
        _destroy(__first, __last, __VALUE_TYPE(__first));
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