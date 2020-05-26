#ifndef F00l_TINY_STL_ALGO_H
#define F00l_TINY_STL_ALGO_H

#include "stl_config.h"
#include "stl_iterator_base.h"

__STL_BEGIN_NAMESPACE
    //return the first iterator which is >= the value,if its not exist, return the last
    template<class _ForwardIter, class _Tp, class _Distance>
    _ForwardIter __lower_bound(_ForwardIter __first, _ForwardIter __last, const _Tp &__value, _Distance *) {
        _Distance __n = 0;
        distance(__first, __last, __n);
        _Distance __half;
        _ForwardIter __middle;

        while(__n > 0) {
            __half = __n >> 1;
            __middle = __first;
            advance(__middle, __half);
            if(*__middle < __value) {
                __first = __middle;
                ++__first;
                __n = __n - __half - 1;
            }
            else {
                __n = __half;
            }
        }

        return __first;
    }

    template<class _ForwardIter, class _Tp>
    inline _ForwardIter lower_bound(_ForwardIter __first, _ForwardIter __last, const _Tp &__value) {
        return __lower_bound(__first, __last, __value, __DISTANCE_TYPE(__first));
    }

    template<class _ForwardIter, class _Tp, class _Comp, class _Distance>
    inline _ForwardIter __lower_bound(_ForwardIter __first, _ForwardIter __last, const _Tp &__value, _Comp __compare, _Distance *) {
        _Distance __n = 0;
        _Distance __half;
        _ForwardIter __middle;
        distance(__first, __last, __n);
        while(__n > 0) {
            __middle = __first;
            __half = __n >> 1;
            advance(__middle, __half);
            if(__compare(*__middle, __value)) {
                __first = __middle;
                ++__first;
                __n = __n - __half - 1;
            }
            else {
                __n = __half;
            }
        }
        return __first;
    }

    template<class _ForwardIter, class _Tp, class _Comp>
    inline _ForwardIter lower_bound(_ForwardIter __first, _ForwardIter __last, const _Tp &__value, _Comp __compare) {
        return __lower_bound(__first, __last, __value, __compare, __DISTANCE_TYPE(__first));
    }

__STL_END_NAMESPACE
#endif //F00l_TINY_STL_ALGO_H