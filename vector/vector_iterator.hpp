#ifndef VECTOR_ITERATOR
#define VECTOR_ITERATOR

#include "../iterator/iterator_traits.hpp"

namespace ft {
    template<class T>
    class vector_iterator {
    public:
        typedef T iterator_type;
        typedef std::random_access_iterator_tag iterator_category;
        typedef typename iterator_traits<iterator_type *>::value_type value_type;
        typedef typename iterator_traits<iterator_type *>::pointer pointer;
        typedef typename iterator_traits<iterator_type *>::reference reference;
        typedef typename iterator_traits<iterator_type *>::difference_type difference_type;

    private:
        pointer _p;

    public:
        vector_iterator() {
            this->_p = 0;
        }

        vector_iterator(pointer p) {
            this->_p = p;
        }

        template<class Iter>
        vector_iterator(const vector_iterator<Iter> &it) {
            this->_p = it.base();
        }

        vector_iterator<iterator_type> &operator=(vector_iterator<iterator_type> const &copy) {
            this->_p = copy.base();
            return (*this);
        }

        virtual ~vector_iterator() {}

        pointer base() const {
            return (this->_p);
        }

        reference operator*() const {
            return (*this->_p);
        }

        pointer operator->() const {
            return &(operator*());
        }

        reference operator[](difference_type n) const {
            return *(this->_p + n);
        }

        vector_iterator &operator++() {
            ++this->_p;
            return (*this);
        }

        vector_iterator operator++(int) {
            vector_iterator tmp(*this);
            ++this->_p;
            return (tmp);
        }

        vector_iterator &operator--() {
            --this->_p;
            return (*this);
        }

        vector_iterator operator--(int) {
            vector_iterator tmp(*this);
            --this->_p;
            return (tmp);
        }

        vector_iterator operator+(const difference_type &n) const {
            return (vector_iterator(this->_p + n));
        }

        vector_iterator operator-(const difference_type &n) const {
            return (vector_iterator(this->_p - n));
        }

        vector_iterator &operator+=(const difference_type &n) {
            this->_p += n;
            return (*this);
        }

        vector_iterator &operator-=(const difference_type &n) {
            this->_p -= n;
            return (*this);
        }

        template<class Type>
        friend vector_iterator<Type> operator+(typename vector_iterator<Type>::difference_type n,
                                               const vector_iterator<Type> &it);

        template<class Type>
        friend vector_iterator<Type> operator-(typename vector_iterator<Type>::difference_type n,
                                               const vector_iterator<Type> &it);

        template<typename A, typename B>
        friend typename vector_iterator<A>::difference_type operator-(const vector_iterator<A> &lhs,
                                                                      const vector_iterator<B> &rhs);
    };

    template<typename A, typename B>
    bool operator==(const vector_iterator<A> &lhs,
                    const vector_iterator<B> &rhs) {
        return (lhs.base() == rhs.base());
    };

    template<typename A, typename B>
    bool operator!=(const vector_iterator<A> &lhs,
                    const vector_iterator<B> &rhs) {
        return (!(lhs == rhs));
    };

    template<typename A, typename B>
    bool operator<(const vector_iterator<A> &lhs,
                   const vector_iterator<B> &rhs) {
        return (lhs.base() < rhs.base());
    };

    template<typename A, typename B>
    bool operator<=(const vector_iterator<A> &lhs,
                    const vector_iterator<B> &rhs) {
        return (!(lhs > rhs));
    };

    template<typename A, typename B>
    bool operator>(const vector_iterator<A> &lhs,
                   const vector_iterator<B> &rhs) {
        return (lhs.base() > rhs.base());
    };

    template<typename A, typename B>
    bool operator>=(const vector_iterator<A> &lhs,
                    const vector_iterator<B> &rhs) {
        return (!(lhs < rhs));
    };

    template<class Type>
    vector_iterator<Type> operator+(typename vector_iterator<Type>::difference_type n,
                                    const vector_iterator<Type> &it) {
        return vector_iterator<Type>(it._p + n);
    };

    template<class Type>
    vector_iterator<Type> operator-(typename vector_iterator<Type>::difference_type n,
                                    const vector_iterator<Type> &it) {
        return vector_iterator<Type>(it._p - n);
    };

    template<typename A, typename B>
    typename vector_iterator<A>::difference_type
    operator-(const vector_iterator<A> &lhs, const vector_iterator<B> &rhs) {
        return (lhs.base() - rhs.base());
    }
}

#endif