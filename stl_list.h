#ifndef F00l_TINY_STL_LIST_H
#define F00l_TINY_STL_LIST_H

#include<cstddef>

#include "stl_config.h"
#include "stl_iterator.h"
#include "stl_iterator_base.h"
#include "stl_algobase.h"
#include "stl_alloc.h"

__STL_BEGIN_NAMESPACE
    struct _List_node_base {
        _List_node_base *_M_prev;
        _List_node_base *_M_next;
    };

    template<class _Tp>
    struct _List_node : public _List_node_base {
        _Tp _M_data;
    };

    struct _List_iterator_base {
        typedef size_t                      size_type;
        typedef ptrdiff_t                   difference_type;
        typedef bidirectional_iterator_tag  iterator_category;
        _List_node_base *_M_node;

        _List_iterator_base(_List_node_base *__x) : _M_node(__x) {}

        _List_iterator_base()  {}

        void _M_inc() {_M_node = _M_node->_M_next;}

        void _M_dec() {_M_node = _M_node->_M_prev;}

        bool operator==(const _List_iterator_base &__x) const {
            return _M_node == __x._M_node;
        }

        bool operator!=(const _List_iterator_base &__x) const {
            return _M_node != __x._M_node;
        }
    };

    template<class _Tp, class _Ref, class _Ptr>
    struct _List_iterator : public _List_iterator_base {
        typedef _List_iterator<_Tp, _Tp&, _Tp*> iterator;
        typedef _List_iterator<const _Tp, const _Tp&, const _Tp*> const_iterator;
        typedef _List_iterator<_Tp, _Ref, _Ptr> _Self;

        typedef _Tp value_type;
        typedef _Ref reference;
        typedef _Ptr pointer;
        typedef _List_node<_Tp> _Node;

        _List_iterator(_Node *__x) : _List_iterator_base(__x) {};
        _List_iterator() {}
        _List_iterator(const iterator &__x) : _List_iterator_base(__x._M_node) {};

        reference operator*() const {return ((_Node*)_M_node)->_M_data;};
        pointer operator->() const {return &(operator*());}

        _Self& operator++() {
            this->_M_inc();
            return *this;
        }

        _Self& operator--() {
            this->_M_dec();
            return *this;
        }

        _Self& operator++(int) {
            _Self &__tmp = *this;
            this->_M_inc();
            return __tmp;
        }

        _Self& operator--(int) {
            _Self &__tmp = *this;
            this->_M_dec();
            return __tmp;
        }

        inline bidirectional_iterator_tag
        iterator_category(const _List_iterator_base&) {
            return bidirectional_iterator_tag();
        }
    };

    inline bidirectional_iterator_tag
    iterator_category(const _List_iterator_base&) {
        return bidirectional_iterator_tag();
    }

    template<class _Tp, class _Ref, class _Ptr>
    inline _Tp*
    value_type(const _List_iterator<_Tp, _Ref, _Ptr>) {
        return 0;
    }

    inline ptrdiff_t*
    distance_type(const _List_iterator_base& ) {
        return 0;
    }

    template<class _Tp, class _Alloc>
    class _List_base {
    public:
        typedef _Alloc allocator_type;
        allocator_type get_allocator() const {return allocator_type ();}

        _List_base(const allocator_type &) {
            _M_node = _M_get_node();
            _M_node->_M_prev = _M_node;
            _M_node->_M_next = _M_node;
        }
        ~_List_base() {
            clear();
            _M_put_node(_M_node);
        }

        void clear();

    protected:
        typedef simple_alloc<_List_node<_Tp>,_Alloc> _Alloc_type;
        _List_node<_Tp>* _M_get_node() {return _Alloc_type :: allocate(1);}
        void _M_put_node(_List_node<_Tp>* __ptr) {_Alloc_type :: deallocate(__ptr, 1);}

    protected:
        _List_node<_Tp>* _M_node;
    };

    template<class _Tp, class _Alloc>
    void _List_base<_Tp,_Alloc>::clear() {
        _List_node<_Tp>* __cur = (_List_node<_Tp>*)_M_node->_M_next;
        while(__cur != _M_node) {
            _List_node<_Tp>* __tmp = __cur;
            __cur = (_List_node<_Tp>*)__cur->_M_next;
            _Destroy(__tmp);
            _M_put_node(__tmp);
        }
        _M_node->_M_next = _M_node;
        _M_node->_M_prev = _M_node;
    }

    template<class _Tp, class _Alloc = alloc>
    class list : protected _List_base<_Tp, _Alloc> {
        typedef _List_base<_Tp,_Alloc> _Base;
    protected:
        typedef void* void_pointer;
    public:
        typedef _Tp value_type;
        typedef value_type* pointer;
        typedef value_type& reference;
        typedef const value_type* const_pointer;
        typedef const value_type& const_reference;
        typedef _List_node<_Tp> _Node;
        typedef size_t size_type;
        typedef ptrdiff_t difference_type;
        typedef typename _Base::allocator_type allocator_type;

        allocator_type get_allocator() const {return _Base::get_allocator();}

    public:
        typedef _List_iterator<_Tp, _Tp&, _Tp*> iterator;
        typedef _List_iterator<_Tp, const _Tp&, const _Tp*> const_iterator;

        typedef reverse_iterator<const iterator> const_reverse_iterator;
        typedef reverse_iterator<iterator> reverse_iterator;
    protected:
        using _Base::_M_node;
        using _Base::_M_get_node;
        using _Base::_M_put_node;
    protected:
        _Node* _M_create_node(const _Tp& __x) {
            _Node* __p = _M_get_node();
            try{
                _Construct(__p,__x);
            }
            catch (...) {
                _M_put_node(__p);
                throw;
            }
            return __p;
        }

        _Node* _M_create_node() {
            _Node* __p = _M_get_node();
            try {
                _Construct(__p);
            }
            catch (...) {
                _M_put_node(__p);
                throw;
            }
        }

    public:
        explicit list(const allocator_type& __a = allocator_type()) : _Base(__a) {}

        iterator begin() {return (_Node*)_M_node->_M_next;}
        const_iterator begin() const {return (_Node*)_M_node->_M_next;}

        iterator end() {return _M_node;}
        const_iterator end() const {return _M_node;}

        reverse_iterator rbegin() {
            return reverse_iterator(end());
        }
        const_reverse_iterator rbegin() const {
            return const_reverse_iterator(end());
        }

        reverse_iterator rend() {
            return reverse_iterator(begin());
        }
        const_reverse_iterator rend() const {
            return const_reverse_iterator(begin);
        }

        bool empty() const {return _M_node->_M_next == _M_node;}

        size_type size() const {
            size_type  __result = 0;
            distance(begin(), end(), __result);
            return __result;
        }

        size_type max_size() const {return size_type(-1);}

        reference front() {return *begin();};
        const_reference front() const {return *begin();}

        reference back() {return *(--end());}
        const_reference back() const {return *(--end());}

        void swap(list<_Tp, _Alloc> &__x) {
            f00l_stl::swap(_M_node, __x._M_node);
        }

        iterator insert(iterator __position, const _Tp& __x) {
            _Node* __tmp = _M_create_node(__x);
            __tmp->_M_next = __position._M_node;
            __tmp->_M_prev = __position;
            __position._M_node->_M_prev->_M_next = __tmp;
            __position._M_node->_M_prev = __tmp;
            return __tmp;
        }

        iterator insert(iterator __position) {return insert(__position, _Tp());}

        //its the common type
        template<class _Integer>
        void _M_insert_dispatch(iterator __pos, _Integer __n, _Integer __x, __true_type) {
            _M_fill_insert(__pos, (size_type) __n, (_Tp) __x);
        }

        //unknown type
        template<class _InputIter>
        void _M_insert_dispatch(iterator __pos, _InputIter __first, _InputIter __last, __false_type);

        template<class _InputIter>
        void insert(iterator __pos, _InputIter __first, _InputIter __last) {
            typedef typename _Is_integer<_InputIter>::_Integral _Integral;
            _M_insert_dispatch(__pos, __first, __last, _Integral());
        }

        void insert(iterator __pos, size_type __n, const _Tp &__x) {
            _M_fill_insert(__pos, __n, __x);
        }

        void _M_fill_insert(iterator __pos, size_type __n, const _Tp &__x);

        void push_front(const _Tp &__x) {insert(begin(),__x);}
        void push_front() {insert(begin());}

        void push_back(const _Tp &__x) {insert(end(),__x);}
        void push_back() {insert(end());}

        iterator erase(iterator __pos) {
            _List_node_base* __next_node = __pos._M_node->_M_next;
            _List_node_base* __prev_node = __pos._M_node->_M_prev;
            _Node* __n = (_Node*) __pos._M_node;
            __prev_node->_M_next = __next_node;
            __next_node->_M_prev = __prev_node;
            _Destroy(&__n->_M_data);
            _M_put_node(__n);
            return iterator((_Node*) __next_node);
        }
        iterator erase(iterator __first, iterator __last);
        void clear() { _Base::clear(); }

        void resize(size_type __new_size, const _Tp &__x);
        void resize(size_type __new_size) {this->resize(__new_size, _Tp());}

        void pop_front() {erase(begin());}

        void pop_back() {
            iterator __tmp = end();
            erase(--__tmp);
        }

        list(size_type __n, const _Tp &__value, const allocator_type &__a = allocator_type()) : _Base(__a) {
            insert(begin(), __n, __value);
        }

        explicit list(size_type __n) : _Base(allocator_type()) {
            insert(begin(), __n, _Tp());
        }

        template<class _InputIter>
        list(_InputIter __first, _InputIter __last, const allocator_type &__a = allocator_type()) : _Base(__a){
            insert(begin(),__first,__last);
        }

        list(const list<_Tp,_Alloc> &__x) {
            insert(begin(), __x.begin(), __x.end());
        }

        ~list() {}

        list<_Tp, _Alloc> &operator=(const list<_Tp, _Alloc> &__x);

    public:

        void assign(size_type __n, const _Tp &__val) { _M_fill_assign(__n, __val); }

        void _M_fill_assign(size_type __n, const _Tp &__val);

        template<class _InputIter>
        void assign(_InputIter __first, _InputIter __last) {
            typedef typename _Is_integer<_InputIter>::_Integral _Integral;
            _M_assign_dispatch(__first, __last, _Integral());
        }

        template<class _Integer>
        void _M_assign_dispatch(_Integer __n, _Integer __val, __true_type){
              _M_fill_assign((size_type) __n, (_Tp)__val);
        }

        template<class _InputIter>
        void _M_assign_dispatch(_InputIter __first, _InputIter __last, __false_type);

    protected:
        //transfer [first,last) to insert the postion's prev
        void transfer(iterator __position, iterator __first, iterator __last) {
            if(__position != __last) {
                __last._M_node->_M_prev->_M_next = __position._M_node;
                __first._M_node->_M_prev->_M_next = __last._M_node;
                __position._M_node->_M_prev->_M_next = __first._M_node;
                _List_node_base *__tmp = __position._M_node->_M_prev;
                __position._M_node->_M_prev = __last._M_node->_M_prev;
                __last._M_node->_M_prev = __first._M_node->_M_prev;
                __first._M_node->_M_prev = __tmp;
            }
        }

    public:
         void splice(iterator __pos, list &__x) {
            transfer(__pos,__x.begin(), __x.end());
        }

        void splice(iterator __pos, list &, iterator __i) {
            iterator __j = __i;
            ++__j;
            if(__pos == __i || __pos == __j) return;
            this->transfer(__pos, __i, __j);
        }

        void splice(iterator __pos, list &, iterator __first, iterator __last) {
            if(__first != __last)
                this->transfer(__pos, __first, __last);
        }

        void remove(const _Tp &__value);
        void unique();
        void merge(list &__x);
        void reverse();
        void sort();
    };

    template <class _Tp, class _Alloc>
    inline bool operator==(const list<_Tp, _Alloc> &__a, const list<_Tp, _Alloc> &__b) {
        typedef typename list<_Tp, _Alloc>::const_iterator const_iterator;
        const_iterator __begin1 = __a.begin();
        const_iterator __begin2 = __b.begin();

        const_iterator __end1 = __a.end();
        const_iterator __end2 = __b.end();

        while(__begin1 != __end1 && __begin2 != __end2 && *__begin1 == *__begin2){
            ++__begin1;
            ++__begin2;
        }

        return __begin1 == __end1 && __begin2 == __end2;
    }

    template<class _Tp, class _Alloc>
    inline bool operator < (const list<_Tp,_Alloc> &__a, const list<_Tp,_Alloc> &__b) {
        return lexicographical_compare(__a.begin(), __a.end(), __b.begin(), __b.end());
    }

    template<class _Tp, class _Alloc>
    inline bool operator != (const list<_Tp,_Alloc> &__a, const list<_Tp,_Alloc> &__b) {
        return !(__a == __b);
    }

    template<class _Tp, class _Alloc>
    inline bool operator > (const list<_Tp,_Alloc> &__a, const list<_Tp,_Alloc> &__b) {
        return __b < __a;
    }

    template<class _Tp, class _Alloc>
    inline bool operator <= (const list<_Tp,_Alloc> &__a, const list<_Tp,_Alloc> &__b) {
        return !(__b < __a);
    }

    template<class _Tp, class _Alloc>
    inline bool operator >= (const list<_Tp,_Alloc> &__a, const list<_Tp,_Alloc> &__b) {
        return !(__a < __b);
    }

    template<class _Tp, class _Alloc>
    inline void swap(list<_Tp,_Alloc> &__a, list<_Tp,_Alloc> &__b) {
        __a.swap(__b);
    }

    //insert from another iterator
    template<class _Tp, class _Alloc>
    template<class _InputIter>
    void list<_Tp,_Alloc>::_M_insert_dispatch(iterator __pos, _InputIter __first, _InputIter __last, __false_type) {
        for(; __first != __last ; ++__first) {
            insert(__pos, *__first);
        }
    }

    //fill the list with value(x) for n times
    template<class _Tp, class _Alloc>
    void list<_Tp, _Alloc>::_M_fill_insert(iterator __pos, size_type __n, const _Tp &__x) {
        for(; __n > 0 ; --__n)
            insert(__pos, __x);
    }

    //erase in a range
    template<class _Tp, class _Alloc>
    typename list<_Tp,_Alloc>::iterator list<_Tp,_Alloc>::erase(iterator __first, iterator __last) {
        while(__first != __last) {
            erase(__first);
            ++__first;
        }
        return __last;
    }

    template<class _Tp, class _Alloc>
    void list<_Tp, _Alloc>::resize(size_type __new_size, const _Tp &__x) {
        iterator __i = begin();
        iterator __e = end();
        size_type __len = 0;

        for(;__i != __e && __len < __new_size ; ++__i,++__len)
            ;
        if(__len == __new_size)
            erase(__i,__e);
        else
            insert(__e,__new_size - __len,__x);
    }

    template<class _Tp, class _Alloc>
    list<_Tp,_Alloc>& list<_Tp,_Alloc>::operator=(const list<_Tp, _Alloc> &__x) {
        iterator __first1 = begin();
        iterator __last1 = end();
        iterator __first2 = begin();
        iterator __last2 = end();
        while(__first1 != __last1 && __first2 != __last2)
            *__first1++ = *__first2++;
        if(__first1 == __last1)
            insert(__first1, __first2, __last2);
        else
            erase(__first1,__last1);
    }

    template<class _Tp, class _Alloc>
    void list<_Tp, _Alloc>::_M_fill_assign(size_type __n, const _Tp &__val) {
        iterator __first = begin();
        iterator __last = end();

        for(; __n > 0 && __first != __last ; --__n, ++__first){
            insert(__first, __val);
        }
        if(__n > 0)
            insert(__last, __n, __val);
        else
            erase(__first, __last);
    }

    template<class _Tp, class _Alloc>
    template<class _InputIter>
    void list<_Tp,_Alloc>::_M_assign_dispatch(_InputIter __first, _InputIter __last, __false_type) {
        iterator __first1 = begin();
        iterator __last1 = end();
        for( ; __first1 != __last1 && __first != __last ; ++__first1,++__first) {
            *__first1 = *__first;
        }
        if(__first1 == __last1)
            insert(__first1, __first, __last);
        else
            erase(__first1, __last1);
    }

    template<class _Tp, class _Alloc>
    void list<_Tp, _Alloc>::remove(const _Tp &__value) {
        iterator __first = begin();
        iterator __last = end();
        while(__first != __last) {
            iterator __next = __first;
            ++__next;
            if(*__first == __value)
                erase(__first);
            __first = __next;
        }
    }

    //删除连续相同的元素
    template<class _Tp, class _Alloc>
    void list<_Tp, _Alloc>::unique() {
        iterator __first = begin();
        iterator __last = end();
        iterator __next = __first;
        while(++__next != __last) {
            if(*__first == *__next)
                erase(__next);
            else
                __first = __next;
            __next = __first;
        }
    }

    template<class _Tp, class _Alloc>
    void list<_Tp, _Alloc>::merge(list<_Tp, _Alloc> &__x) {

    }

    inline void _List_base_reverse(_List_node_base *__p) {
        _List_node_base *__tmp = __p;
        do {
            f00l_stl::swap(__tmp->_M_prev, __tmp->_M_next);
            __tmp = __tmp->_M_prev;
        }while (__tmp != __p);
    }

    template<class _Tp, class _Alloc>
    void list<_Tp,_Alloc>::reverse() {
        _List_base_reverse(this->_M_node);
    }

    template<class _Tp, class _Alloc>
    void list<_Tp,_Alloc>::sort() {

    }
__STL_END_NAMESPACE
#endif //F00l_TINY_STL_LIST_H