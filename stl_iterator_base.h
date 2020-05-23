#ifndef F00l_TINY_STL_ITERATOR_BASE_H
#define F00l_TINY_STL_ITERATOR_BASE_H

#define STL_CLASS_PARTIAL_SPECIALIZATION

#include<cstddef>

#include "stl_config.h"

__STL_BEGIN_NAMESPACE
    struct input_iterator_tag {};
    struct output_iterator_tag {};
    struct forward_iterator_tag : public input_iterator_tag {};
    struct bidirectional_iterator_tag : public forward_iterator_tag {};
    struct random_access_iterator_tag : public bidirectional_iterator_tag {};

    template<class _Tp, class _Distance>
    struct input_iterator {
        typedef input_iterator_tag iterator_category;
        typedef _Tp         value_type;
        typedef _Tp*        pointer;
        typedef _Tp&        reference;
        typedef _Distance   difference_type;
    };

    struct output_iterator {
        typedef output_iterator_tag iterator_category;
        typedef void            value_type;
        typedef void            pointer;
        typedef void            reference;
        typedef void            difference_type;
    };

    template<class _Tp, class _Distance>
    struct forward_iterator {
        typedef forward_iterator_tag iterator_category;
        typedef _Tp         value_type;
        typedef _Tp*        pointer;
        typedef _Tp&        reference;
        typedef _Distance   difference_type;
    };

    template<class _Tp, class _Distance>
    struct bidirectional_iterator {
        typedef bidirectional_iterator_tag iterator_category;
        typedef _Tp         value_type;
        typedef _Tp*        pointer;
        typedef _Tp&        reference;
        typedef _Distance   difference_type;
    };

    template<class _Tp, class _Distance>
    struct random_access_iterator {
        typedef random_access_iterator_tag iterator_category;
        typedef _Tp         value_type;
        typedef _Tp*        pointer;
        typedef _Tp&        reference;
        typedef _Distance   difference_type;
    };

    template<class _Category, class _Tp, class _Distance = ptrdiff_t, class _Pointer = _Tp*, class _Reference = _Tp&>
    struct iterator {
        typedef _Category  iterator_category;
        typedef _Tp        value_type;
        typedef _Distance  difference_type;
        typedef _Pointer   pointer;
        typedef _Reference reference;
    };

    //对迭代器进行萃取

    template<class _Iterator>
    struct iterator_traits {
        typedef typename _Iterator::iterator_category   iterator_category;
        typedef typename _Iterator::value_type          value_type;
        typedef typename _Iterator::pointer             pointer;
        typedef typename _Iterator::reference           reference;
        typedef typename _Iterator::difference          difference_type;
    };

    //针对指针偏特化

    template<class _Tp>
    struct iterator_traits<_Tp*> {
        typedef random_access_iterator_tag iterator_category;
        typedef _Tp                        value_type;
        typedef _Tp*                       pointer;
        typedef _Tp&                       reference;
        typedef ptrdiff_t                  difference_type;
    };

    template<class _Tp>
    struct iterator_traits<const _Tp*> {
        typedef random_access_iterator_tag iterator_category;
        typedef _Tp                        value_type;
        typedef const _Tp*                       pointer;
        typedef const _Tp&                       reference;
        typedef ptrdiff_t                  difference_type;
    };

    template<class _Iterator>
    inline typename iterator_traits<_Iterator>::iterator_category __iterator_category(const _Iterator&) {
        return iterator_traits<_Iterator>::iterator_category();
    }

    template<class _Iterator>
    inline typename iterator_traits<_Iterator>::iterator_category iterator_category(const _Iterator &__i) {
        return __iterator_category(__i);
    }

    template<class _Iterator>
    inline typename iterator_traits<_Iterator>::difference_type * __iterator_distance_type(const _Iterator&) {
        return static_cast<typename iterator_traits<_Iterator>::difference_type *>(0);
    }

    template <class _Iterator>
    inline typename iterator_traits<_Iterator>::difference_type * iterator_distance_type(const _Iterator& __i) {
        return __iterator_distance_type(__i);
    }

    template<class _Iterator>
    inline typename iterator_traits<_Iterator>::value_type * __iterator_value_type(const _Iterator&) {
        return static_cast<typename iterator_traits<_Iterator>::value_type *>(0);
    }

    template <class _Iterator>
    inline typename iterator_traits<_Iterator>::value_type * iterator_value_type(const _Iterator& __i) {
        return __iterator_value_type(__i);
    }

#define __ITERATOR_CATEGORY(__i) __iterator_category(__i)
#define __DISTANCE_TYPE(__i)     __distance_type(__i)
#define __VALUE_TYPE(__i)        __value_type(__i)


    template<class _InputIter, class _Distance>
    inline void __distance(_InputIter __first, _InputIter __last, _Distance& __n, input_iterator_tag) {
        while(__first != __last) {
            ++__first;
            ++__n;
        }
    }

    template<class _RandomAccessIter, class _Distance>
    inline void __distance(_RandomAccessIter __first, _RandomAccessIter __last, _Distance &__n, random_access_iterator_tag) {
        __n += (__last - __first);
    }

    template<class _InputIter, class _Distance>
    inline void distance(_InputIter __first, _InputIter __last, _Distance &__n) {
        __distance(__first, __last, __n,iterator_category(__first));
    }

#ifdef __STL_CLASS_PARTIAL_SPECIALIZATION

    template<class _InputIter>
    inline typename iterator_traits<_InputIter>::difference_type
    __distance(_InputIter first, _InputIter last, input_iterator_tag) {
        typename iterator_traits<_InputIter>::difference_type __n = 0;
        while(__first != __last) {
            __first++;
            __n++
        }
        return __n;
    }

    template<class _RandomAccessIter>
    inline typename iterator_traits<_RandomAccessIter>::difference_type
    __distance(_RandomAccessIter first, _RandomAccessIter last, random_access_iterator_tag) {
        typename iterator_traits<_RandomAccessIter>::difference_type __n = 0;
        __n = __last - __first;
        return __n;
    }

    template<class _InputIter>
    inline typename iterator_traits<_InputIter>::difference_type
    distance(_InputIter __first, _InputIter __last) {
        typename iterator_traits<_InputIter>::iterator_category __Category;
        typename iterator_traits<_InputIter>::difference_type __n = 0;
        __n = __distance(__first, __last, __Category());
        return __n;
    }
#endif

    template<class _InputIter, class _Distance>
    inline void __advance(_InputIter &__i, _Distance __n, input_iterator_tag) {
        while(__n--) {
            __i++;
        }
    }

    template<class _BidirectionalIter, class _Distance>
    inline void __advance(_BidirectionalIter &__i, _Distance __n, bidirectional_iterator_tag) {
        if(__n > 0) {
            while(__n--) {
                ++__i;
            }
        }
       else {
           while(__n++) {
               --__i;
           }
       }
    }

    template<class _RandomAccessIter, class _Distance>
    inline void __advance(_RandomAccessIter &__i, _Distance __n, random_access_iterator_tag) {
        __i += __n;
    }

    template<class _InputIter, class _Distance>
    inline void advance(_InputIter &__i, _Distance __n) {
        typedef typename iterator_traits<_InputIter>::iterator_category Category;
        __advance(__i, __n, Category());
    }
__STL_END_NAMESPACE
#endif //F00l_TINY_STL_ITERATOR_BASE_H