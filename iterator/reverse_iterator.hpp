#ifndef REVERSE_ITERATOR
#define REVERSE_ITERATOR

#include "iterator_traits.hpp"

namespace ft {
    template<class Iterator>
    class reverse_iterator {
    public:
        typedef Iterator iterator_type;
        typedef typename iterator_traits<iterator_type>::iterator_category iterator_category;
        typedef typename iterator_traits<iterator_type>::value_type value_type;
        typedef typename iterator_traits<iterator_type>::difference_type difference_type;
        typedef typename iterator_traits<iterator_type>::pointer pointer;
        typedef typename iterator_traits<iterator_type>::reference reference;

    private:
        iterator_type _it;

    public:
        reverse_iterator() : _it() {}

        explicit reverse_iterator(iterator_type it) : _it(it) {}

        template<class Iter>
        reverse_iterator(const reverse_iterator<Iter> &rev_it) {
            this->_it = rev_it.base();
        }

        iterator_type base() const {
            return (this->_it);
        }

        reference operator*() const {
            iterator_type tmp = this->_it;

            return (*--tmp);
        }

        reverse_iterator operator+(difference_type n) const {
            return (reverse_iterator(this->_it - n));
        }

        reverse_iterator &operator++() {
            --(this->_it);
            return (*this);
        }

        reverse_iterator operator++(int) {
            reverse_iterator tmp = *this;

            ++(*this);
            return (tmp);
        }

        reverse_iterator &operator+=(difference_type n) {
            this->_it -= n;
            return (*this);
        }

        reverse_iterator operator-(difference_type n) const {
            return (reverse_iterator(this->_it + n));
        }

        reverse_iterator &operator--() {
            ++(this->_it);

            return (*this);
        }

        reverse_iterator operator--(int) {
            reverse_iterator tmp = *this;

            --(*this);
            return (tmp);
        }

        reverse_iterator &operator-=(difference_type n) {
            this->_it += n;

            return (*this);
        }

        pointer operator->() const {
            return &(operator*());
        }

        reference operator[](difference_type n) const {
            return (_it[-n - 1]);
        }
    };

    template<typename A, typename B>
    bool operator==(const reverse_iterator<A> &lhs,
                    const reverse_iterator<B> &rhs) {
        return (lhs.base() == rhs.base());
    }

    template<typename A, typename B>
    bool operator!=(const reverse_iterator<A> &lhs,
                    const reverse_iterator<B> &rhs) {
        return (lhs.base() != rhs.base());
    }

    template<typename A, typename B>
    bool operator<(const reverse_iterator<A> &lhs,
                   const reverse_iterator<B> &rhs) {
        return (lhs.base() > rhs.base());
    }

    template<typename A, typename B>
    bool operator<=(const reverse_iterator<A> &lhs,
                    const reverse_iterator<B> &rhs) {
        return (lhs.base() >= rhs.base());
    }

    template<typename A, typename B>
    bool operator>(const reverse_iterator<A> &lhs,
                   const reverse_iterator<B> &rhs) {
        return (lhs.base() < rhs.base());
    }

    template<typename A, typename B>
    bool operator>=(const reverse_iterator<A> &lhs,
                    const reverse_iterator<B> &rhs) {
        return (lhs.base() <= rhs.base());
    }

    template<class Iterator>
    reverse_iterator<Iterator> operator+(
            typename reverse_iterator<Iterator>::difference_type n,
            const reverse_iterator<Iterator> &rev_it) {
        return (rev_it + n);
    }

    template<class Iterator>
    reverse_iterator<Iterator> operator-(
            typename reverse_iterator<Iterator>::difference_type n,
            const reverse_iterator<Iterator> &rev_it) {
        return (rev_it - n);
    }

    template<typename A, typename B>
    typename reverse_iterator<A>::difference_type operator-(
            const reverse_iterator<A> &lhs,
            const reverse_iterator<B> &rhs) {
        return (rhs.base() - lhs.base());
    }
}

#endif