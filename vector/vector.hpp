#ifndef VECTOR
#define VECTOR

#include <iostream>
#include "vector_iterator.hpp"
#include "../iterator/reverse_iterator.hpp"
#include "../util/util.hpp"

namespace ft {
    template<class T, class Allocator = std::allocator<T> >
    class vector {
    public:
        typedef T value_type;
        typedef Allocator allocator_type;
        typedef typename allocator_type::reference reference;
        typedef typename allocator_type::const_reference const_reference;
        typedef typename allocator_type::size_type size_type;
        typedef typename allocator_type::difference_type difference_type;
        typedef typename allocator_type::pointer pointer;
        typedef typename allocator_type::const_pointer const_pointer;

        typedef vector_iterator<value_type> iterator;
        typedef vector_iterator<const value_type> const_iterator;

        typedef ft::reverse_iterator<iterator> reverse_iterator;
        typedef ft::reverse_iterator<const_iterator> const_reverse_iterator;

    private:
        pointer _begin;
        size_type _size;
        size_type _capacity;
        allocator_type _allocator;

    public:
        explicit vector(const allocator_type &alloc = allocator_type()) {
            this->_allocator = alloc;
            this->_begin = NULL;
            this->_size = 0;
            this->_capacity = 0;
        }

        explicit vector(size_type n, const value_type &val = value_type(),
                        const allocator_type &alloc = allocator_type()) {
            this->_allocator = alloc;
            this->_size = n;
            this->_capacity = n;
            this->_begin = this->_allocator.allocate(n);
            for (size_type i = 0; i < n; i++)
                this->_allocator.construct(this->_begin + i, val);
        }

        template<class InputIterator>
        vector(InputIterator first, InputIterator last,
               const allocator_type &alloc = allocator_type(),
               typename enable_if<!is_integral<InputIterator>::value>::type * = 0) :_allocator(alloc) {
            difference_type tmp_size = std::distance(first, last);

            if (tmp_size < 0)
                throw std::length_error("vector");
            this->_size = static_cast<size_type>(tmp_size);
            this->_capacity = this->_size;
            this->_begin = this->_allocator.allocate(this->_capacity);
            std::uninitialized_copy(first, last, this->_begin);
        }

        vector(const vector &x) {
            this->_size = 0;
            this->_capacity = 0;
            *this = x;
        }

        vector &operator=(const vector &x) {
            if (this == &x)
                return (*this);

            for (size_type i = 0; i < this->_size; i++)
                this->_allocator.destroy(this->_begin + i);
            this->_size = x._size;
            if (this->_capacity < this->_size) {
                if (this->_capacity != 0)
                    this->_allocator.deallocate(this->_begin, this->_capacity);
                this->_capacity = this->_size;
                this->_begin = this->_allocator.allocate(this->_capacity);
            }
            for (size_type i = 0; i < this->_size; i++)
                this->_allocator.construct(this->_begin + i, x._begin[i]);
            return (*this);
        }

        ~vector() {
            for (size_type i = 0; i < this->_size; i++)
                this->_allocator.destroy(this->_begin + i);
            if (this->_capacity != 0)
                this->_allocator.deallocate(this->_begin, this->_capacity);
        }

        iterator begin() {
            return (iterator(this->_begin));
        }

        const_iterator begin() const {
            return (const_iterator(this->_begin));
        }

        iterator end() {
            return (iterator(this->_begin + this->_size));
        }

        const_iterator end() const {
            return (const_iterator(this->_begin + this->_size));
        }

        reverse_iterator rbegin() {
            return (reverse_iterator(this->end()));
        }

        const_reverse_iterator rbegin() const {
            return (reverse_iterator(this->end()));
        };

        reverse_iterator rend() {
            return (reverse_iterator(this->begin()));
        }

        const_reverse_iterator rend() const {
            return (reverse_iterator(this->begin()));
        }

        size_type size() const {
            return (this->_size);
        }

        size_type max_size() const {
            return (this->_allocator.max_size());
        }

        void resize(size_type n, value_type val = value_type()) {
            if (n < this->_size) {
                for (size_type i = n; i < this->_size; i++)
                    this->_allocator.destroy(this->_begin + i);
                this->_size = n;
            } else {
                if (n > this->_capacity) {
                    this->reserve(n);
                    for (size_type i = this->_size; i < n; i++) {
                        this->_allocator.construct(this->_begin + i, val);
                        this->_size++;
                    }
                } else {
                    this->reserve(this->_capacity);
                    for (size_type i = this->_size; i < n; i++) {
                        this->_allocator.construct(this->_begin + i, val);
                        this->_size++;
                    }
                }
            }
        }

        size_type capacity() const {
            return (this->_capacity);
        }

        bool empty() const {
            if (this->_size == 0)
                return (true);
            else
                return (false);
        }

        void reserve(size_type n) {
            if (n < this->_capacity)
                return;
            pointer new_first = this->_allocator.allocate(n);
            for (size_type i = 0; i < this->_size; i++)
                this->_allocator.construct(new_first + i, *(this->_begin + i));
            for (size_type i = 0; i < this->_size; i++)
                this->_allocator.destroy(this->_begin + i);
            this->_allocator.deallocate(this->_begin, this->_capacity);
            this->_capacity = n;
            this->_begin = new_first;
        }

        reference operator[](size_type n) {
            return (this->_begin[n]);
        }

        const_reference operator[](size_type n) const {
            return (this->_begin[n]);
        }

        reference at(size_type n) {
            if (n > this->_capacity)
                throw std::out_of_range("vector out of range");
            return (this->_begin[n]);
        }

        const_reference at(size_type n) const {
            if (n > this->_capacity)
                throw std::out_of_range("vector out of range");
            return (this->_begin[n]);
        }

        reference front() {
            return (*(this->_begin));
        }

        const_reference front() const {
            return (*(this->_begin));
        }

        reference back() {
            return (this->_begin[this->_size - 1]);
        }

        const_reference back() const {
            return (this->_begin[this->_size - 1]);
        }

        void assign(size_type n, const value_type &u) {
            clear();
            resize(n, u);
        }

        template<class InputIterator>
        void assign(InputIterator first, InputIterator last,
                    typename enable_if<!is_integral<InputIterator>::value>::type * = 0) {
            size_type size = static_cast<size_type>(std::distance(first, last));
            if (size < 0)
                throw std::length_error("vector");
            clear();
            if (this->_capacity < size)
                reserve(size);

            std::uninitialized_copy(first, last, this->_begin);
            this->_size = size;
        }

        void push_back(const value_type &x) {
            if (this->_size >= this->_capacity)
                reserve(this->_capacity == 0 ? 1 : this->_capacity * 2);

            this->_allocator.construct(this->_begin + this->_size, x);
            this->_size++;
        }

        void pop_back() {
            value_type tmp;

            tmp = this->back();
            this->_allocator.destroy(this->_begin + this->_size - 1);
            this->_size -= 1;

        }

        iterator insert(iterator position, const value_type &val) {
            if (position < begin() || position > end())
				throw std::logic_error("vector");
            difference_type d_size = position - this->begin();
            if (this->_size == this->_capacity){
                if (this->_capacity == 0)
                    this->_capacity += 1;
                this->_capacity *= 2;
                pointer new_begin = this->_allocator.allocate(this->_capacity);
                std::uninitialized_copy(this->begin(), position, iterator(new_begin));
                this->_allocator.construct(new_begin + d_size, val);
                std::uninitialized_copy(position, this->end(), iterator(new_begin + d_size + 1));
                for (size_type i = 0; i < this->_size; i++)
                    this->_allocator.destroy(this->_begin + i);
                if (this->_size)
                    this->_allocator.deallocate(this->_begin, this->_size);
                this->_size += 1;
                this->_begin = new_begin;
            }
            else
            {
                for (size_type i = this->_size; i > static_cast<size_type>(d_size); i--)
                {
                    this->_allocator.destroy(this->_begin + i);
                    this->_allocator.construct(this->_begin + i, *(this->_begin + i - 1));
                }
                    this->_allocator.destroy(&(*position));
                    this->_allocator.construct(&(*position), val);
                    this->_size += 1;
            }
            return (this->begin() + d_size);
        }

        void insert(iterator position, size_type n, const value_type &val) {
            difference_type d_size = position - this->begin();
            
            if (this->_size + n > this->_capacity){
                if (this->_capacity * 2 >= this->_size + n)
                    this->_capacity *= 2;
                else
                    this->_size += n;
                pointer new_begin = this->_allocator.allocate(this->_capacity);
                std::uninitialized_copy(this->begin(), position, new_begin);
                std::uninitialized_fill_n(new_begin + d_size, n, val);
                std::uninitialized_copy(position, this->end(), new_begin + d_size + n);
                for (size_type i = 0; i < this->_size; i++)
                    this->_allocator.destroy(this->_begin + i);
                this->_allocator.deallocate(this->_begin, this->_capacity);
                this->_size += n;
                this->_begin = new_begin;
            }
            else {
                for (size_type i = this->_size; i > static_cast<size_type>(d_size); i--){
                    this->_allocator.destroy(this->_begin + i + n - 1);
                    this->_allocator.construct(this->_begin + i + n - 1, *(this->_begin + i - 1));
                }
                for (size_type i = 0; i < n; i++){
                    this->_allocator.destroy(this->_begin + i + d_size);
                    this->_allocator.construct(this->_begin + i + d_size, val);
                }
                    this->_size += n;
            }
        }

        template<class InputIterator>
        void insert(iterator position, InputIterator first, InputIterator last,
                    typename enable_if<!is_integral<InputIterator>::value>::type * = 0) {
            difference_type d_size = position - this->begin();
            size_type n = static_cast<size_type>(std::distance(first, last));
            
            if (this->_size + n > this->_capacity){
                if (this->_capacity * 2 >= this->_size + n)
                        this->_capacity *= 2;
                    else
                        this->_capacity = this->_size + n;
                pointer new_begin = this->_allocator.allocate(this->_capacity);
                std::uninitialized_copy(this->begin(), position, new_begin);
                std::uninitialized_copy(first, last, new_begin + d_size);
                std::uninitialized_copy(position, this->end(), new_begin + d_size + n);
                
                for (size_type i = 0; i < this->_size; i++)
                    this->_allocator.destroy(this->_begin + i);
                this->_allocator.deallocate(this->_begin, this->_capacity);
                this->_size += n;
                this->_begin = new_begin;
            } else {
                for (size_type i = this->_size; i > static_cast<size_type>(d_size); i--){
                    this->_allocator.destroy(this->_begin + i + n - 1);
                    this->_allocator.construct(this->_begin + i + n - 1, *(this->_begin + i - 1));
                }
                for (size_type i = 0; i < n; i++, this->_begin++){
                    this->_allocator.destroy(this->_begin + i + n);
                    this->_allocator.construct(this->_begin + + d_size + i, *this->_begin);
                }
                this->_size += n;
            }
        }

        iterator erase(const_iterator position) {
            size_type d_size = static_cast<size_type>(position - this->begin());

            for (size_type i = d_size; i < this->_size - 1; i++) {
                this->_allocator.destroy(this->_begin + i);
                this->_allocator.construct(this->_begin + i, *(this->_begin + i + 1));
            }
            this->_size--;
            this->_allocator.destroy(this->_begin + this->_size - 1);
            return (this->begin() + d_size);
        }

        iterator erase(iterator first, iterator last) {
            difference_type begin_to_first_size = first - this->begin();
            difference_type first_to_last_size = last - first;
            difference_type last_to_end_size = this->end() - last;

            for (difference_type i = 0; i < first_to_last_size; i++) {
                this->_allocator.destroy(this->_begin + begin_to_first_size + i);
            }

            for (difference_type i = 0; i < last_to_end_size; i++) {
                this->_allocator.construct(this->_begin + begin_to_first_size + i,
                                           *(this->_begin + begin_to_first_size + first_to_last_size + i));
                this->_allocator.destroy(this->_begin + begin_to_first_size + first_to_last_size + i);
            }

            this->_size -= first_to_last_size;

            return (first);
        }

        void swap(vector &x) {
            pointer tmp_begin;
            size_type tmp_size;
            size_type tmp_capacity;
            allocator_type tmp_allocator;

            tmp_begin = this->_begin;
            tmp_size = this->_size;
            tmp_capacity = this->_capacity;
            tmp_allocator = this->_allocator;

            this->_begin = x._begin;
            this->_size = x._size;
            this->_capacity = x._capacity;
            this->_allocator = x._allocator;

            x._begin = tmp_begin;
            x._size = tmp_size;
            x._capacity = tmp_capacity;
            x._allocator = tmp_allocator;
        }

        void clear() {
            for (size_type i = 0; i < this->_size; i++)
                this->_allocator.destroy(this->_begin + i);
            this->_size = 0;
        }

        allocator_type get_allocator() const {
            return (this->_allocator);
        }
    };

    template<class T, class Allocator>
    bool operator==(const vector<T, Allocator> &x, const vector<T, Allocator> &y) {
        if (x.size() != y.size())
            return (false);
        return (equal(x.begin(), x.end(), y.begin()));
    }

    template<class T, class Allocator>
    bool operator<(const vector<T, Allocator> &x, const vector<T, Allocator> &y) {

        return (lexicographical_compare(x.begin(), x.end(), y.begin(), y.end()));
    }

    template<class T, class Allocator>
    bool operator!=(const vector<T, Allocator> &x, const vector<T, Allocator> &y) {
        return (!(x == y));
    }

    template<class T, class Allocator>
    bool operator>(const vector<T, Allocator> &x, const vector<T, Allocator> &y) {
        return (lexicographical_compare(y.begin(), y.end(), x.begin(), x.end()));
    }

    template<class T, class Allocator>
    bool operator>=(const vector<T, Allocator> &x, const vector<T, Allocator> &y) {
        return (!(x < y));
    }

    template<class T, class Allocator>
    bool operator<=(const vector<T, Allocator> &x, const vector<T, Allocator> &y) {
        return (!(x > y));
    }

    template<class T, class Alloc>
    void swap(vector<T, Alloc> &x, vector<T, Alloc> &y) {
        x.swap(y);
    }
};

#endif