#ifndef F00l_TINY_STL_ITERATOR_H
#define F00l_TINY_STL_ITERATOR_H

#include "stl_config.h"
#include "stl_iterator_base.h"

__STL_BEGIN_NAMESPACE
    template<class _Container>
    class back_insert_iterator {
    protected:
        _Container *container;
    public:
        typedef _Container container_type;
        typedef output_iterator_tag iterator_category;
        typedef void value_type;
        typedef void difference_type;
        typedef void reference;
        typedef void pointer;

        explicit back_insert_iterator(_Container &__c) : container(&__c) {}

        back_insert_iterator<_Container> &operator=(typename _Container::value_type &__value) {
            container->push_back(__value);
            return *this;
        }

        back_insert_iterator<_Container> &operator*() { return *this;}

        back_insert_iterator<_Container> &operator++() {return *this;}

        back_insert_iterator<_Container> &operator++(int) { return *this;}
    };

    template<class _Container>
    inline back_insert_iterator<_Container>back_insert(_Container &__c) {
        return back_insert_iterator<_Container>(__c);
    }

    template <class _Container>
    class front_insert_iterator {
    protected:
         _Container *container;
    public:
         typedef _Container container_type;
         typedef output_iterator_tag iterator_category;
         typedef void value_type;
         typedef void difference_type;
         typedef void pointer;
         typedef void reference;

         explicit front_insert_iterator(_Container &__c) : container(&__c) {}

         front_insert_iterator<_Container> &operator=(const typename _Container::value_type &__value) {
             container->push_back(__value);
             return *this;
         }

         front_insert_iterator<_Container> &operator*() {
             return *this;
         }

         front_insert_iterator<_Container> &operator++() {
             return *this;
         }

         front_insert_iterator<_Container> &operator++(int) {
             return *this;
         }
    };

    template <class _Container>
    inline front_insert_iterator<_Container>front_insert(_Container __c) {
        return front_insert_iterator<_Container>(__c);
    }

    template<class _Container>
    class insert_iterator {
    protected:
        _Container *container;
        typename _Container::iterator iter;
    public:
        typedef _Container container_type;
        typedef output_iterator_tag iterator_category;
        typedef void value_type;
        typedef void difference_type;
        typedef void pointer;
        typedef void reference;

        insert_iterator(_Container &__c, typename _Container::iterator __i) : container(&__c), iter(__i) {}

        insert_iterator<_Container> &operator=(const typename _Container::value_type &__value) {
            iter = container->insert(iter,__value);
            iter++;
            return *this;
        }

        insert_iterator<_Container> &operator*() {return *this;}

        insert_iterator<_Container> &operator++() {return *this;}

        insert_iterator<_Container> &operator++(int) {return *this;}
    };

    template<class _Container>
    inline insert_iterator<_Container> insert(_Container &__c, typename _Container::iterator __i) {
        return insert_iterator<_Container>(__c, __i);
    }

    template<class _Iterator>
    class reverse_iterator {
    protected:
       _Iterator current;
    public:
       typedef typename iterator_traits<_Iterator>::iterator_category iterator_category;
       typedef typename iterator_traits<_Iterator>::value_type        value_type;
       typedef typename iterator_traits<_Iterator>::difference_type   difference_type;
       typedef typename iterator_traits<_Iterator>::pointer           pointer;
       typedef typename iterator_traits<_Iterator>::reference         reference;
       typedef _Iterator iterator_type;
       typedef reverse_iterator<_Iterator> _Self;

       reverse_iterator() {}

       explicit reverse_iterator(iterator_type __i) : current(__i) {}

       reverse_iterator(_Self &s) : current(s.current) {}

       template <class _Iter>
       reverse_iterator(const reverse_iterator<_Iter> &other) : current(other.current) {}

       iterator_type base() const {
           return current;
       }

       reference operator*() const {
           _Iterator tmp = current;
           return *--tmp;
       }

       pointer operator->() const {
           return &(operator*());
       }

       _Self& operator++() {
           --current;
           return *this;
       }

       _Self operator++(int) {
           _Self tmp = *this;
           --current;
           return tmp;
       }

        _Self& operator--() {
            ++current;
            return *this;
        }

        _Self operator--(int) {
            _Self tmp = *this;
            ++current;
            return tmp;
        }

        _Self operator+(difference_type __n) const {
            return _Self(current - __n);
       }

       _Self operator-(difference_type __n) const {
           return _Self (current + __n);
       }

       _Self operator+=(difference_type __n) const {
           current -= __n;
           return *this;
       }

       _Self operator-=(difference_type __n) const {
           current += __n;
           return *this;
       }

        reference operator[] (difference_type __n) const {
            return *(*this+__n);
        }
    };

    template <class _Iterator>
    inline bool operator==(const reverse_iterator<_Iterator> &__lhs, const reverse_iterator<_Iterator> &__rhs) {
        return __lhs.base() == __rhs.base();
    }

    template <class _Iterator>
    inline bool operator!=(const reverse_iterator<_Iterator> &__lhs, const reverse_iterator<_Iterator> &__rhs) {
        return !(__lhs.base() == __rhs.base());
    }

    template <class _Iterator>
    inline bool operator<(const reverse_iterator<_Iterator> &__lhs, const reverse_iterator<_Iterator> &__rhs) {
        return __lhs.base() < __rhs.base();
    }

    template <class _Iterator>
    inline bool operator>(const reverse_iterator<_Iterator> &__lhs, const reverse_iterator<_Iterator> &__rhs) {
        return __lhs.base() > __rhs.base();
    }

    template <class _Iterator>
    inline bool operator<=(const reverse_iterator<_Iterator> &__lhs, const reverse_iterator<_Iterator> &__rhs) {
        return !(__rhs < __lhs);
    }

    template <class _Iterator>
    inline bool operator>=(const reverse_iterator<_Iterator> &__lhs, const reverse_iterator<_Iterator> &__rhs) {
        return !(__lhs < __rhs);
    }

    template <class _Iterator>
    reverse_iterator<_Iterator> operator+(const reverse_iterator<_Iterator>&__p,
                                          typename reverse_iterator<_Iterator>::difference_type __n) {
        return reverse_iterator<_Iterator>(__p.base() - __n);
    }

    template <class _Iterator>
    typename reverse_iterator<_Iterator>::difference_type operator-(const reverse_iterator<_Iterator>&__lhs,
                                          const reverse_iterator<_Iterator>&__rhs) {
        return __lhs.base() - __rhs.base();
    }
__STL_END_NAMESPACE
#endif /*F00l_TINY_STL_ITERATOR_H*/