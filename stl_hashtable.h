#ifndef F00l_TINY_STL_HASHTABLE_H
#define F00l_TINY_STL_HASHTABLE_H

#include "stl_config.h"
#include "stl_iterator.h"
#include "stl_alloc.h"
#include "stl_algo.h"
#include "stl_vector.h"
#include "stl_pair.h"

__STL_BEGIN_NAMESPACE
    template<class _Val>
    struct _Hashtable_node {
        _Hashtable_node *_M_next;
        _Val __M_val;
    };

    template<class _Val, class _Key, class _HashFcn, class _ExtractKey, class _EqualKey, class _Alloc = alloc>
    class hashtable;

    template<class _Val, class _Key, class _HashFcn, class _ExtractKey, class _EqualKey, class _Alloc>
    class _Hashtable_iterator;

    template<class _Val, class _Key, class _HashFcn, class _ExtractKey, class _EqualKey, class _Alloc>
    class _Hashtable_const_iterator;

    template<class _Val, class _Key, class _HashFcn, class _ExtractKey, class _EqualKey, class _Alloc>
    struct _Hashtable_iterator {
        typedef hashtable<_Val, _Key, _HashFcn, _ExtractKey, _EqualKey, _Alloc>  _Hashtable;
        typedef _Hashtable_iterator<_Val, _Key, _HashFcn, _ExtractKey, _EqualKey, _Alloc>  iterator;
        typedef _Hashtable_const_iterator<_Val, _Key, _HashFcn, _ExtractKey, _EqualKey, _Alloc> const_iterator;
        typedef _Hashtable_node<_Val> _Node;

        typedef forward_iterator_tag iterator_category;
        typedef _Val value_type;
        typedef ptrdiff_t difference_type;
        typedef size_t size_type;
        typedef _Val & reference;
        typedef _Val * pointer;

        _Node* _M_cur;
        _Hashtable* _M_ht;

        _Hashtable_iterator(_Node* __n, _Hashtable * __table)
         : _M_cur(__n), _M_ht(__table) {}
        _Hashtable_iterator() {}

        reference operator*() const {return _M_cur->__M_val;}

        pointer operator->() const {return &(operator*());}

        iterator & operator++();

        iterator operator++(int);

        bool operator==(const iterator& __it) const {
            return _M_cur == __it._M_cur;
        }

        bool operator!=(const iterator& __it) const {
            return _M_cur != __it._M_cur;
        }
    };

    template<class _Val, class _Key, class _HashFcn, class _ExtractKey, class _EqualKey, class _Alloc>
    struct _Hashtable_const_iterator {
        typedef hashtable<_Val, _Key, _HashFcn, _ExtractKey, _EqualKey, _Alloc>  _Hashtable;
        typedef _Hashtable_iterator<_Val, _Key, _HashFcn, _ExtractKey, _EqualKey, _Alloc>  iterator;
        typedef _Hashtable_const_iterator<_Val, _Key, _HashFcn, _ExtractKey, _EqualKey, _Alloc> const_iterator;
        typedef _Hashtable_node<_Val> _Node;

        typedef forward_iterator_tag iterator_category;
        typedef _Val value_type;
        typedef ptrdiff_t difference_type;
        typedef size_t size_type;
        typedef const _Val& reference;
        typedef const _Val* pointer;

        const _Node* _M_cur;
        const _Hashtable* _M_ht;

        _Hashtable_const_iterator(const _Node* __n, const _Hashtable * __table)
                : _M_cur(__n), _M_ht(__table) {}
        _Hashtable_const_iterator() {}
        _Hashtable_const_iterator(const iterator& __it)
            :   _M_cur(__it._M_cur), _M_ht(__it._M_ht) {}

        reference operator*() const {
            return _M_cur->__M_val;
        }

        pointer operator->() const {
            return &(operator*());
        }

        const_iterator& operator++();

        const_iterator operator++(int);

        bool operator==(const iterator& __it) const {
            return _M_cur == __it._M_cur;
        }

        bool operator!=(const iterator& __it) const {
            return _M_cur != __it._M_cur;
        }
    };

    //assume long is at least 32 bits
    enum {
        __stl_num_primes = 28
    };

    static const unsigned long __stl_prime_list[__stl_num_primes] =
            {
                    53ul, 97ul, 193ul, 389ul, 769ul,
                    1543ul, 3079ul, 6151ul, 12289ul, 24593ul,
                    49157ul, 98317ul, 196613ul, 393241ul, 786433ul,
                    1572869ul, 3145739ul, 6291469ul, 12582917ul, 25165843ul,
                    50331653ul, 100663319ul, 201326611ul, 402653189ul, 805306457ul,
                    1610612741ul, 3221225473ul, 4294967291ul
            };

    //find the iterator which's value lower than __n
    inline unsigned long __stl_next_prime(unsigned long __n) {
        const unsigned long* __first = __stl_prime_list;
        const unsigned long* __last = __stl_prime_list + (int)__stl_num_primes;
        const unsigned long* pos = lower_bound(__first, __last, __n);
        return pos == __last ? *(__last - 1) : *pos;
    }

    template<class _Val, class _Key, class _HashFcn, class _ExtractKey, class _EqualKey, class _Alloc>
    bool operator == (const hashtable<_Val, _Key, _HashFcn, _EqualKey, _EqualKey, _Alloc>& __h1,
                      const hashtable<_Val, _Key, _HashFcn, _EqualKey, _EqualKey, _Alloc>& __h2);

    template<class _Val, class _Key, class _HashFcn, class _ExtractKey, class _EqualKey, class _Alloc>
    class hashtable {
    public:
        typedef _Val value_type;
        typedef _Key key_type;
        typedef _HashFcn hasher;
        typedef _EqualKey key_equal;

        typedef size_t size_type;
        typedef ptrdiff_t difference_type;
        typedef value_type* pointer;
        typedef const value_type* const_pointer;
        typedef value_type& reference;
        typedef const value_type& const_reference;

        hasher hash_funct() const {return _M_hash;}

        key_equal key_eq() const {return _M_equals;}

    private:
        typedef _Hashtable_node<_Val> _Node;
        typedef _Alloc allocator_type;

        allocator_type get_allocator() {return allocator_type ();}

    private:
        typedef simple_alloc<_Node, _Alloc> _M_node_allocator_type;

        _Node* _M_get_node() {
            return _M_node_allocator_type::allocate(1);
        }

        _Node* _M_put_node(_Node *__p) {
            return _M_node_allocator_type::deallocate(__p, 1);
        }

    #define __HASH_ALLOC_INIT(__a)
    private:
        hasher _M_hash;
        key_equal _M_equals;
        _ExtractKey _M_get_key;
        vector<_Node*, _Alloc> _M_buckets;
        size_type _M_num_elements;

    public:
        typedef _Hashtable_iterator<_Val, _Key, _HashFcn, _EqualKey, _EqualKey, _Alloc>  iterator;
        typedef _Hashtable_const_iterator<_Val, _Key, _HashFcn, _EqualKey, _EqualKey, _Alloc> const_iterator;

        friend struct _Hashtable_iterator<_Val, _Key, _HashFcn, _EqualKey, _EqualKey, _Alloc>;
        friend struct _Hashtable_const_iterator<_Val, _Key, _HashFcn, _EqualKey, _EqualKey, _Alloc>;

    public:
        hashtable(size_type __n, const _HashFcn& __hf,
                  const _EqualKey& __eq1, const _ExtractKey& __ext,
                  const allocator_type& __a = allocator_type())
           : __HASH_ALLOC_INIT(__a)
             _M_hash(__hf),
             _M_equals(__eq1),
             _M_get_key(__ext),
             _M_buckets(__a),
             _M_num_elements(0){
             _M_initialize_buckets(__n);
        }

        hashtable(size_type __n, const _HashFcn& __hf,
                  const _EqualKey& __eq1,const allocator_type& __a = allocator_type())
            : __HASH_ALLOC_INIT(__a)
              _M_hash(__hf),
              _M_equals(__eq1),
              _M_get_key(_ExtractKey()),
              _M_buckets(__a),
              _M_num_elements(0){
            _M_initialize_buckets(__n);
        }

        hashtable(const hashtable& __ht)
            : __HASH_ALLOC_INIT(__ht.get_allocators())
              _M_hash(__ht._M_hash),
              _M_equals(__ht._M_equals),
              _M_get_key(__ht._M_get_key),
              _M_buckets(__ht.get_allocators()),
              _M_num_elements(0) {
              _M_copy_from(__ht);
        }

#undef __HASH_ALLOC_INIT

        hashtable& operator=(const hashtable& __ht) {
            if(this != &__ht) {
                clear();
                _M_hash = __ht._M_hash;
                _M_equals = __ht._M_equals;
                _M_get_key = __ht._M_get_key;
                _M_copy_from(__ht);
            }
            return *this;
        }

        ~hashtable() {clear();}

        size_type size() const {return _M_num_elements;}

        size_type max_size() const {return size_type (-1);}

        bool empty() const {return size() == 0;}

        void swap(hashtable& __ht) {
            f00l_stl::swap(_M_hash, __ht._M_hash);
            f00l_stl::swap(_M_equals, __ht._M_equals);
            f00l_stl::swap(_M_get_key, __ht._M_get_key);
            _M_buckets.swap(__ht._M_buckets);
            f00l_stl::swap(_M_num_elements,__ht._M_num_elements);
        }

        iterator begin() {
            for(size_type __n = 0; __n < _M_buckets.size(); ++__n)
                if(_M_buckets[__n])
                    return iterator(_M_buckets[__n],this);
                return end();
        }

        iterator end() {return iterator(0,this);}

        const_iterator begin() const {
            for(size_type __n = 0; __n < _M_buckets.size(); ++__n)
                if(_M_buckets[__n])
                    return const_iterator(__n, this);
                return end();
        }

        const_iterator end() const {return const_iterator(0, this);}

        template<class _V, class _K, class _Hf, class _Ex, class _Eq, class _Al>
        friend bool operator == (const hashtable<_V,_K,_Hf,_Ex,_Eq,_Al>&,
                                 const hashtable<_V,_K,_Hf,_Ex,_Eq,_Al>&);

    public:
        size_type bucket_count(){
            return _M_buckets.size();
        }

        size_type max_bucket_count() {
            return __stl_prime_list[(int)__stl_num_primes - 1];
        }

        //compute the hashtable[__bucket]'s elements number
        size_type elems_in_bucket(size_type __bucket) {
            size_type __result = 0;
            for(_Node* __cur = _M_buckets[__bucket]; __cur; __cur = __cur->_M_next){
                ++__result;
            }
            return __result;
        }

        pair<iterator, bool> insert_unique(const value_type& __obj) {
            resize(_M_num_elements + 1);
            return insert_unique_noresize(__obj);
        }

        iterator insert_equal(const value_type& __obj) {
            resize(_M_num_elements + 1);
            return insert_equal_noresize(__obj);
        }

        pair<iterator, bool> insert_unique_noresize(const value_type& __obj);

        iterator insert_equal_noresize(const value_type& __obj);

        template<class _InputIter>
        void insert_unique(_InputIter __f, _InputIter __l) {
            insert_unique(__f, __l, __ITERATOR_CATEGORY(__f));
        }

        template<class _InputIter>
        void insert_equal(_InputIter __f, _InputIter __l) {
            insert_equal(__f, __l, __ITERATOR_CATEGORY(__f));
        }

        template<class _InputIter>
        void insert_unique(_InputIter __f, _InputIter __l, input_iterator_tag) {
            for(; __f != __l ; ++__f) {
                insert_unique(*__f);
            }
        }

        template<class _InputIter>
        void insert_equal(_InputIter __f, _InputIter __l, input_iterator_tag) {
            for(; __f != __l ; ++__f) {
                insert_equal(*__f);
            }
        }

        template<class _ForwardIter>
        void insert_unique(_ForwardIter __f, _ForwardIter __l, forward_iterator_tag) {
            size_type __n = 0;
            distance(__f, __l, __n);
            resize(_M_num_elements + __n);
            for(; __n > 0 ; --__n, ++__f) {
                insert_unique_noresize(*__f);
            }
        }

        template<class _ForwardIter>
        void insert_equal(_ForwardIter __f, _ForwardIter __l, forward_iterator_tag) {
            size_type __n = 0;
            distance(__f, __l, __n);
            resize(_M_num_elements + __n);
            for(; __n > 0 ; --__n, ++__f) {
                insert_equal_noresize(*__f);
            }
        }

        reference find_or_insert(const value_type& __obj);

        iterator find(const key_type& __key) {
            size_type __n = _M_bkt_num_key(__key);
            _Node* __first;
            for(__first = _M_buckets[__n]; __first && !_M_equals(_M_get_key(__first->__M_val), __key);
                __first = __first->_M_next) {
            }

            return iterator(__first, this);
        }

        const_iterator find(const key_type& __key) const {
            size_type __n = _M_bkt_num_key(__key);
            _Node* __first;
            for(__first = _M_buckets[__n]; __first && !_M_equals(_M_get_key(__first->__M_val), __key);
                __first = __first->_M_next) {
            }

            return const_iterator(__first, this);
        }

        size_type count(key_type& __key) {
            size_type __n = _M_bkt_num_key(__key);
            size_type __count = 0;
            _Node* __first;
            for(__first = _M_buckets[__n]; __first && !_M_equals(_M_get_key(__first->__M_val), __key);
                __first = __first->_M_next) {
                ++__count;
            }

            return __count;
        }

        pair<iterator, iterator>
        equal_range(const key_type& __key);

        pair<iterator, iterator>
        equal_range(const key_type& __key) const;

        size_type erase(const key_type& __key);

        void erase(const iterator& __it);

        void erase(iterator __first, iterator __last);

        void erase(const const_iterator& __it);

        void erase(const_iterator __first, const_iterator __last);

        void resize(size_type __num_element_hint);

        void clear();

    private:
        size_type _M_next_size(size_type __n) {
            return __stl_prime_list[__n];
        }

        void _M_initialize_buckets(size_type __n) {
            const size_type __n_buckets = _M_next_size(__n);
            _M_buckets.reserve(__n_buckets);
            _M_buckets.insert(_M_buckets.end(), __n_buckets, (_Node*)0);
            _M_num_elements = 0;
        }

        size_type _M_bkt_num_key(const key_type& __key) const {
            return _M_bkt_num_key(__key, _M_buckets.size());
        }

        size_type _M_bkt_num_key(const value_type& __obj) const {
            return _M_bkt_num_key(_M_get_key(__obj));
        }

        size_type _M_bkt_num_key(const key_type& __key, size_type __n) const {
            return _M_hash(__key) % __n;
        }

        size_type _M_bkt_num(const value_type& __obj, size_type __n) const {
            return _M_bkt_num_key(_M_get_key(__obj), __n);
        }

        _Node* _M_new_node(const value_type& __obj) {
            _Node* __n = _M_get_node();
            __n->_M_next = 0;
            try{
                construct(&__n->__M_val, __obj);
                return __n;
            }
            catch (...) {
                _M_put_node(__n);
                throw;
            }
        }

        _Node* _M_delete_node(_Node* __p) {
            destroy(&__p->__M_val);
            _M_put_node(__p);
        }

        void _M_erase_bucket(const size_type __n, _Node *__first, _Node *__last);

        void _M_erase_bucket(const size_type __n, _Node *__last);

        void _M_copy_from(const hashtable& __ht);
    };

    template<class _Val, class _Key, class _HF, class _ExK, class _Eqk, class _All>
    _Hashtable_iterator<_Val,_Key,_HF,_ExK,_Eqk,_All>&
    _Hashtable_iterator<_Val,_Key,_HF,_ExK,_Eqk,_All>::operator++() {
        const _Node* __old = _M_cur;
        _M_cur  = _M_cur->_M_next;
        if(!_M_cur) {
            size_type __bucket = _M_ht->_M_bkt_num(__old->__M_val);
            while(!_M_cur && ++__bucket < _M_ht->_M_buckets.size()) {
                _M_cur = _M_ht->_M_buckets[__bucket];
            }
        }
        return *this;
    }

    template<class _Val, class _Key, class _HF, class _ExK, class _Eqk, class _All>
    inline _Hashtable_iterator<_Val,_Key,_HF,_ExK,_Eqk,_All>
    _Hashtable_iterator<_Val,_Key,_HF,_ExK,_Eqk,_All>::operator++(int) {
        iterator __tmp = *this;
        ++*this;
        return __tmp;
    }

    template<class _Val, class _Key, class _HF, class _ExK, class _Eqk, class _All>
    _Hashtable_const_iterator<_Val,_Key,_HF,_ExK,_Eqk,_All>&
    _Hashtable_const_iterator<_Val,_Key,_HF,_ExK,_Eqk,_All>::operator++() {
        const _Node* __old = _M_cur;
        _M_cur  = _M_cur->_M_next;
        if(!_M_cur) {
            size_type __bucket = _M_ht->_M_bkt_num(__old->__M_val);
            while(!_M_cur && ++__bucket < _M_ht->_M_buckets.size()) {
                _M_cur = _M_ht->_M_buckets[__bucket];
            }
        }
        return *this;
    }

    template<class _Val, class _Key, class _HF, class _ExK, class _Eqk, class _All>
    inline _Hashtable_const_iterator<_Val,_Key,_HF,_ExK,_Eqk,_All>
    _Hashtable_const_iterator<_Val,_Key,_HF,_ExK,_Eqk,_All>::operator++(int) {
        const_iterator __tmp = *this;
        ++*this;
        return __tmp;
    }

    template<class _Val, class _Key, class _HF, class _ExK, class _Eqk, class _All>
    inline bool operator==(const hashtable<_Val,_Key,_HF,_ExK,_Eqk,_All>& __a,
                    const hashtable<_Val,_Key,_HF,_ExK,_Eqk,_All>& __b) {
        typedef typename hashtable<_Val,_Key,_HF,_ExK,_Eqk,_All>::_Node _Node;
        if(__a._M_buckets.size() != __b._M_buckets.size())
            return false;
        for(int __n = 0 ; __n < __a._M_buckets.size() ; ++__n) {
            _Node* __cur1 = __a._M_buckets[__n];
            _Node* __cur2 = __a._M_buckets[__n];
            for(; __cur1 && __cur2 && __cur1->_M_val == __cur2->_M_val
                ; __cur1 = __cur1->_M_next,__cur2 = __cur2->_M_next)
            {}
            if(__cur2 || __cur1)
                return false;
        }
        return true;
    }

    template<class _Val, class _Key, class _HF, class _ExK, class _Eqk, class _All>
    inline bool operator!=(const hashtable<_Val,_Key,_HF,_ExK,_Eqk,_All>& __a,
                    const hashtable<_Val,_Key,_HF,_ExK,_Eqk,_All>& __b) {
        return !(__a == __b);
    }

    template<class _Val, class _Key, class _HF, class _ExK, class _Eqk, class _All>
    inline void swap(const hashtable<_Val,_Key,_HF,_ExK,_Eqk,_All>& __a,
                    const hashtable<_Val,_Key,_HF,_ExK,_Eqk,_All>& __b) {
        __a.swap(__b);
    }

    template<class _Val, class _Key, class _HF, class _ExK, class _Eqk, class _All>
    pair<typename hashtable<_Val,_Key,_HF,_ExK,_Eqk,_All>::iterator, bool>
    hashtable<_Val,_Key,_HF,_ExK,_Eqk,_All>::insert_unique_noresize(const value_type &__obj) {
        const size_type __n = _M_bkt_num(__obj);
        _Node* __first = _M_buckets[__n];
        for(_Node* __cur = __first ; __cur ; __cur = __cur->_M_next){
            if(_M_equals(_M_get_key(__cur->__M_val),_M_get_key(__obj)))
                return pair<iterator,bool>(__cur, false);
        }

        _Node* __p = _M_new_node(__obj);
        __p->_M_next = __first;
        _M_buckets[__n] = __p;
        ++_M_num_elements;
        return pair<iterator,bool>(__p,true);
    }

    template<class _Val, class _Key, class _HF, class _ExK, class _Eqk, class _All>
    typename hashtable<_Val,_Key,_HF,_ExK,_Eqk,_All>::iterator
    hashtable<_Val,_Key,_HF,_ExK,_Eqk,_All>::insert_equal_noresize(const value_type &__obj) {
        const size_type __n = _M_bkt_num(__obj);
        _Node* __first = _M_buckets[__n];
        for(_Node* __cur = __first ; __cur ; __cur = __cur->_M_next){
            if(_M_equals(_M_get_key(__cur->__M_val),_M_get_key(__obj))) {
                _Node *__p = _M_new_node(__obj);
                __p->_M_next = __cur->_M_next;
                __cur->_M_next = __p;
                ++_M_num_elements;
                return iterator(__p,this);
            }
        }

        _Node* __p = _M_new_node(__obj);
        __p->_M_next = __first;
        _M_buckets[__n] = __p;
        ++_M_num_elements;
        return iterator(__p,this);
    }

    //find the obj in the hashtable  if not found insert a new node into the hashtable
    template<class _Val, class _Key, class _HF, class _ExK, class _Eqk, class _All>
    typename hashtable<_Val,_Key,_HF,_ExK,_Eqk,_All>::reference
    hashtable<_Val,_Key,_HF,_ExK,_Eqk,_All>::find_or_insert(const value_type &__obj) {
        resize(_M_num_elements + 1);
        size_type __n = _M_bkt_num(__obj);
        _Node* __first = _M_buckets[__n];
        for(_Node* __cur = __first ; __cur ; ++__cur) {
            if(_M_equals(_M_get_node(__cur->__M_val), _M_get_key(__obj)))
                return __cur->__M_val;
        }

        _Node* __p = _M_new_node(__obj);
        __p->_M_next = __first;
        _M_buckets[__n] = __p;
        ++_M_num_elements;
        return iterator(__p,this);
    }

    template<class _Val, class _Key, class _HF, class _ExK, class _Eqk, class _All>
    typename hashtable<_Val,_Key,_HF,_ExK,_Eqk,_All>::size_type
    hashtable<_Val,_Key,_HF,_ExK,_Eqk,_All>::erase(const key_type& __key) {
        const size_type __n = _M_bkt_num_key(__key);
        _Node* __first = _M_buckets[__n];
        size_type __erased = 0;
        if(__first) {
            _Node* __cur = __first;
            _Node* __next = __cur->_M_next;
            while(__next) {
                if(_M_equals(_M_get_key(__next->__M_val),__key)) {
                    __cur->_M_next = __next->_M_next;
                    _M_delete_node(__next);
                    ++__erased;
                    __next = __cur->_M_next;
                    --_M_num_elements;
                }
                else {
                    __cur = __next;
                    __next = __cur->_M_next;
                }
            }
            if(_M_equals(_M_get_key(__first->__M_val),__key)) {
                _M_buckets[__n] = __first->_M_next;
                _M_delete_node(__first);
                ++__erased;
                --_M_num_elements;
            }
        }
        return __erased;
    }

    template<class _Val, class _Key, class _HF, class _ExK, class _Eqk, class _All>
    void hashtable<_Val,_Key,_HF,_ExK,_Eqk,_All>::erase(const iterator &__it) {
        _Node *__p = __it._M_cur;
        if(__p) {
            const size_type __n = _M_bkt_num(__p->__M_val);
            _Node *__cur = _M_buckets[__n];

            if(__cur == __p) {
                _M_buckets[__n] = __cur->_M_next;
                _M_delete_node(__cur);
                --_M_num_elements;
            }
            else {
                _Node *__next = __cur->_M_next;
                while(__next) {
                    if(__next == __p) {
                        __cur->_M_next = __next->_M_next;
                        _M_delete_node(__next);
                        __next = __cur->_M_next;
                    }
                    else{
                        __cur = __next;
                        __next = __cur->_M_next;
                    }
                }
            }
        }
    }

    template<class _Val, class _Key, class _HF, class _ExK, class _Eqk, class _All>
    void hashtable<_Val,_Key,_HF,_ExK,_Eqk,_All>::erase(iterator __first, iterator __last) {
        size_type __f_bucket = __first._M_cur ? _M_bkt_num(__first._M_cur->_M_val) : _M_buckets.size();
        size_type __l_bucket = __last._M_cur ? _M_bkt_num(__last._M_cur->_M_val) : _M_buckets.size();

        if(__first._M_cur == __last._M_cur)
            return;
        else if(__f_bucket == __l_bucket)
            _M_erase_bucket(__f_bucket,__first._M_cur,__last._M_cur);
        else {
            _M_erase_bucket(__f_bucket, __first._M_cur, 0);
            for(size_type __n = __f_bucket + 1 ; __n < 1 ; ++__n)
                _M_erase_bucket(__n,0);
            if(__l_bucket != _M_buckets.size())
                _M_erase_bucket(__l_bucket, __last._M_cur);
        }
    }

    template<class _Val, class _Key, class _HF, class _ExK, class _Eqk, class _All>
    inline void
    hashtable<_Val,_Key,_HF,_ExK,_Eqk,_All>::erase(const_iterator __first, const_iterator __last) {
        erase(iterator(const_cast<_Node*>(__first._M_cur),
                       const_cast<hashtable*>(__first._M_ht)),
               iterator(const_cast<_Node*>(__last._M_cur),
                        const_cast<hashtable*>(__last._M_ht)));
    }

    template<class _Val, class _Key, class _HF, class _ExK, class _Eqk, class _All>
    inline void
    hashtable<_Val,_Key,_HF,_ExK,_Eqk,_All>::erase(const const_iterator &__it) {
        erase(iterator(const_cast<_Node*>(__it._M_cur),
                       const_cast<hashtable*>(__it._M_ht)));
    }

    template<class _Val, class _Key, class _HF, class _ExK, class _Eqk, class _All>
    void hashtable<_Val,_Key,_HF,_ExK,_Eqk,_All>::_M_erase_bucket(const size_type __n, _Node *__first, _Node *__last) {
        _Node* __cur = _M_buckets[__n];
        if(__first == __cur)
            _M_erase_bucket(__n,__last);
        else{
            _Node *__next = __cur->_M_next;
            for( ; __next != __first ; __cur = __next, __next = __cur->_M_next)
                while(__next != __last) {
                    __cur->_M_next = __next->_M_next;
                    _M_delete_node(__next);
                    __next = __cur->_M_next;
                    --_M_num_elements;
                }
        }
    }

    template<class _Val, class _Key, class _HF, class _ExK, class _Eqk, class _All>
    void hashtable<_Val,_Key,_HF,_ExK,_Eqk,_All>::_M_erase_bucket(const size_type __n, _Node *__last) {
        _Node* __cur = _M_buckets[__n];
        while(__cur != __last) {
            _Node *__next = __cur->_M_next;
            _M_delete_node(__cur);
            --_M_num_elements;
            __cur = __next;
            _M_buckets[__n] = __cur;
        }
    }

    template<class _Val, class _Key, class _HF, class _ExK, class _Eqk, class _All>
    pair<typename hashtable<_Val,_Key,_HF,_ExK,_Eqk,_All>::iterator,
         typename hashtable<_Val,_Key,_HF,_ExK,_Eqk,_All>::iterator>
    hashtable<_Val,_Key,_HF,_ExK,_Eqk,_All>::equal_range(const key_type &__key) {
        typedef pair<iterator,iterator> _Pil;
        const size_type __n = _M_bkt_num_key(__key);

        for(_Node* __first = _M_buckets[__n] ; __first ; __first = __first->_M_next) {
            if(_M_equals(_M_get_key(__first->__M_val),__key)) {
                for(_Node* __next = __first->_M_next ; __next ; __next = __next->_M_next)
                    if(!_M_equals(_M_get_key(__next),__key))
                        return _Pil(iterator(__first,this), iterator(__next,this));
                for(size_type __m = __n + 1 ; __m < _M_buckets.size() ; ++__m)
                    if(_M_buckets[__m])
                        return _Pil(iterator(__first,this),
                                    iterator(_M_buckets[__m],this));
                return _Pil(iterator(__first,this),end());
            }
        }
        return _Pil(end(),end());
    }

    template<class _Val, class _Key, class _HF, class _ExK, class _Eqk, class _All>
    void hashtable<_Val,_Key,_HF,_ExK,_Eqk,_All>::resize(size_type __num_element_hint) {
        const size_type __old_size = _M_buckets.size();
        if(__num_element_hint > __old_size) {
            const size_type __n = _M_next_size(__num_element_hint);
            if(__n > __old_size) {
                vector<_Node*, _All> __tmp(__n, (_Node*)(0), _M_buckets.get_allocator());
                try{
                    for(size_type __bucket = 0 ; __bucket < __n ; ++__bucket) {
                        _Node* __first = _M_buckets[__bucket];
                        while(__first) {
                            size_type __new_bucket = _M_bkt_num(__first->__M_val,__n);
                            _M_buckets[__bucket] = __first->_M_next;
                            __first->_M_next = __tmp[__new_bucket];
                            __tmp[__new_bucket] = __first;
                            __first = _M_buckets[__bucket];
                        }
                    }
                    _M_buckets.swap(__tmp);
                }
                catch (...) {
                    for(size_type __bucket = 0 ; __bucket < _M_buckets.size() ; ++__bucket) {
                        while(__tmp[__bucket]) {
                            _Node* __p = __tmp[__bucket]->_M_next;
                            _M_delete_node(__tmp[__bucket]);
                            __tmp[__bucket] = __p;
                        }
                    }
                }
            }
        }
    }

    //clear all the elements
    template<class _Val, class _Key, class _HF, class _ExK, class _Eqk, class _All>
    void hashtable<_Val,_Key,_HF,_ExK,_Eqk,_All>::clear() {
        for(size_type __i = 0 ; __i < _M_buckets.size() ; ++__i) {
            _Node* __first = _M_buckets[__i];
            while(__first) {
                _Node *__next = __first->_M_next;
                _M_delete_node(__first);
                __first = __next;
            }
            _M_buckets[__i] = 0;
        }
        _M_num_elements = 0;
    }

    template<class _Val, class _Key, class _HF, class _ExK, class _Eqk, class _All>
    void hashtable<_Val,_Key,_HF,_ExK,_Eqk,_All>
     ::_M_copy_from(const hashtable<_Val,_Key,_HF,_ExK,_Eqk,_All> &__ht) {
         _M_buckets.clear();
         _M_buckets.reserve(__ht._M_buckets.size());
         _M_buckets.insert(_M_buckets.end(),_M_buckets.size(),(_Node*)0);
         try{
             for(size_type __i = 0 ; __i < _M_buckets.size() ; ++__i) {
                 const _Node *__first = __ht._M_buckets[__i];
                 if(__first) {
                     _Node *__copy = _M_new_node(__first->__M_val);
                     _M_buckets[__i] = __copy;
                     for(_Node *__next = __first->_M_next ; __next ; __first = __next,__next = __first->_M_next) {
                         __copy ->_M_next = _M_new_node(__next->__M_val);
                         __copy = __copy->_M_next;
                     }
                 }
             }
             _M_num_elements = __ht._M_num_elements;
         }
         catch (...) {
            clear();
            throw;
         }
     }
__STL_END_NAMESPACE
#endif //F00l_TINY_STL_HASHTABLE_H