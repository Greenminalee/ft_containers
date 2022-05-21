#ifndef ITERATOR_TRAITS
#define ITERATOR_TRAITS

#include "../util/util.hpp"
#include "tree.hpp"

namespace ft {
    template<class Key, class value, class Compare = std::less<Key>, class Allocator = std::allocator<pair<const Key, value> > >
    class map {
    public:
        typedef Key key_type;
        typedef value mapped_type;

        typedef pair<const Key, value> value_type;
        typedef Compare key_compare;

        typedef typename Allocator::template rebind<Node<value> >::other node_allocator;
        typedef typename node_allocator::pointer node_pointer;

        typedef Allocator allocator_type;
        typedef typename allocator_type::reference reference;
        typedef typename allocator_type::const_reference const_reference;
        typedef typename allocator_type::pointer pointer;
        typedef typename allocator_type::const_pointer const_pointer;

        typedef std::ptrdiff_t difference_type;
        typedef std::size_t size_type;
        class pair_compare {
            friend class map;
        protected:
            Compare comp;
            pair_compare(Compare c) : comp(c) {}
        public:
            pair_compare() {}
            typedef bool result_type;
            typedef value_type first_argument_type;
            typedef value_type second_argument_type;
            bool operator()(const value_type &x, const value_type &y) const {
                return comp(x.first, y.first);
            }
        };

        typedef pair_compare value_compare;
        typedef tree<value_type, pair_compare, allocator_type> tree_type;
        typedef typename tree_type::iterator iterator;
        typedef typename tree_type::const_iterator const_iterator;
        typedef typename tree_type::reverse_iterator reverse_iterator;
        typedef typename tree_type::const_reverse_iterator const_reverse_iterator;

    private:
        key_compare _comp;
        allocator_type _allocator;
        tree_type _tree;
        
    public:
        explicit map(const key_compare &comp = key_compare(),
                     const allocator_type &alloc = allocator_type()) : _comp(comp), _allocator(alloc),
                                                                       _tree(tree_type(comp, alloc)) {}

        template<class InputIterator>
        map(InputIterator first, InputIterator last,
            const key_compare &comp = key_compare(),
            const allocator_type &alloc = allocator_type())
                :_comp(comp), _allocator(alloc), _tree(first, last, comp, alloc) {}

        map(const map &x) {
            this->_comp = x._comp;
            this->_allocator = x._allocator;
            this->_tree = x._tree;
        }

        map &operator=(const map &x) {
            if (this != &x) {
                this->_allocator = x._allocator;
                this->_comp = x._comp;
                this->_tree = x._tree;
            }
            return (*this);
        }

        ~map(){}

        iterator begin() {
            return (this->_tree.begin());
        }

        const_iterator begin() const {
            return (this->_tree.begin());
        }

        iterator end() {
            return (this->_tree.end());
        }

        const_iterator end() const {
            return (this->_tree.end());
        }

        reverse_iterator rbegin() {
            return (this->_tree.rbegin());
        }

        const_reverse_iterator rbegin() const {
            return (this->_tree.rbegin());
        }

        reverse_iterator rend() {
            return (this->_tree.rend());
        }

        const_reverse_iterator rend() const {
            return (this->_tree.rend());
        }

        bool empty() const {
            return (this->_tree.empty());
        }

        size_type size() const {
            return (this->_tree.size());
        }

        size_type max_size() const {
            return (this->_tree.max_size());
        }

        mapped_type &operator[](const key_type &k) {
            return (*((this->insert(ft::make_pair(k, mapped_type()))).first)).second;
        }

        pair<iterator, bool> insert(const value_type &val) {
            return (this->_tree.insert(val));
        }

        iterator insert(iterator position, const value_type &val) {
            return (this->_tree.insert(position, val));
        }

        template<class InputIterator>
        void insert(InputIterator first, InputIterator last) {
            return (this->_tree.insert(first, last));
        }

        void erase(iterator position) {
            this->_tree.erase(position);
        }

        size_type erase(const key_type &k) {

            return (this->_tree.erase(ft::make_pair(k, mapped_type())));
        }

        void erase(iterator first, iterator last) {
            this->_tree.erase(first, last);
        }

        void swap(map &x) {
            this->_tree.swap(x._tree);
        }

        void clear() {
            this->_tree.clear();
        }

        key_compare key_comp() const {
            return (this->_comp);
        }

        value_compare value_comp() const {
            return (this->_tree.value_comp());
        }

        iterator find(const key_type &k) {
            return (this->_tree.find(ft::make_pair(k, mapped_type())));
        }

        const_iterator find(const key_type &k) const {
            return (this->_tree.find(ft::make_pair(k, mapped_type())));
        }

        size_type count(const key_type &k) const {
            return (this->_tree.count(ft::make_pair(k, mapped_type())));
        }

        iterator lower_bound(const key_type &k) {
            return (this->_tree.lower_bound(ft::make_pair(k, mapped_type())));
        }

        const_iterator lower_bound(const key_type &k) const {
            return (this->_tree.lower_bound(ft::make_pair(k, mapped_type())));
        }

        iterator upper_bound(const key_type &k) {
            return (this->_tree.upper_bound(ft::make_pair(k, mapped_type())));
        }

        const_iterator upper_bound(const key_type &k) const {
            return (this->_tree.upper_bound(ft::make_pair(k, mapped_type())));
        }

        pair<iterator, iterator> equal_range(const key_type &k) {
            return (this->_tree.equal_range(ft::make_pair(k, mapped_type())));
        }

        pair<const_iterator, const_iterator> equal_range(const key_type &k) const {
            return (this->_tree.equal_range(ft::make_pair(k, mapped_type())));
        }


        allocator_type get_allocator() const {
            return (this->_allocator);
        }

        template<class _Key, class _T, class _Compare, class _Alloc>
        friend bool operator==(const map<_Key, _T, _Compare, _Alloc> &lhs,
                               const map<_Key, _T, _Compare, _Alloc> &rhs);

        template<class _Key, class _T, class _Compare, class _Alloc>
        friend bool operator<(const map<_Key, _T, _Compare, _Alloc> &lhs,
                              const map<_Key, _T, _Compare, _Alloc> &rhs);

    };

    template<class Key, class T, class Compare, class Alloc>
    bool operator==(const map<Key, T, Compare, Alloc> &lhs, const map<Key, T, Compare, Alloc> &rhs) {
        return (lhs._tree == rhs._tree);
    }

    template<class Key, class T, class Compare, class Alloc>
    bool operator!=(const map<Key, T, Compare, Alloc> &lhs, const map<Key, T, Compare, Alloc> &rhs) {
        return !(lhs == rhs);
    }

    template<class Key, class T, class Compare, class Alloc>
    bool operator<(const map<Key, T, Compare, Alloc> &lhs, const map<Key, T, Compare, Alloc> &rhs) {
        return (lhs._tree < rhs._tree);
    }

    template<class Key, class T, class Compare, class Alloc>
    bool operator>(const map<Key, T, Compare, Alloc> &lhs, const map<Key, T, Compare, Alloc> &rhs) {
        return (rhs < lhs);
    }

    template<class Key, class T, class Compare, class Alloc>
    bool operator<=(const map<Key, T, Compare, Alloc> &lhs, const map<Key, T, Compare, Alloc> &rhs) {
        return !(lhs > rhs);
    }

    template<class Key, class T, class Compare, class Alloc>
    bool operator>=(const map<Key, T, Compare, Alloc> &lhs, const map<Key, T, Compare, Alloc> &rhs) {
        return !(lhs < rhs);
    }

    template<class Key, class T, class Compare, class Alloc>
    void swap(const map<Key, T, Compare, Alloc> &lhs, const map<Key, T, Compare, Alloc> &rhs) {
        lhs.swap(rhs);
    }
}

#endif