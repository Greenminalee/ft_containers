#ifndef TREE_ITERATOR
#define TREE_ITERATOR

#include "../util/util.hpp"
#include "../iterator/iterator_traits.hpp"

namespace ft {
    template<class Value>
    class Node {
    public:
        Value value;
        Node *parent;
        Node *left;
        Node *right;
        int height;

    public:
        explicit Node() : value(), parent(0), left(0), right(0), height(0) {}

        explicit Node(Value v) : value(v), parent(0), left(0), right(0), height(1) {}

        Node(Value *value, Node *parent, Node *left, Node *right, int height) {
            this->value = value;
            this->parent = parent;
            this->left = left;
            this->right = right;
            this->height = height;
        }

        Node(const Node &copy) {
            *this = copy;
        }

        Node &operator=(const Node &copy) {
            this->value = copy.value;
            this->parent = parent;
            this->left = copy.left;
            this->right = copy.right;
            this->height = copy.height;
            return (*this);
        }

        virtual ~Node() {}
    };

    template<class T>
    class tree_iterator {
    public:
        typedef T iterator_type;
        typedef std::bidirectional_iterator_tag iterator_category;
        typedef typename iterator_traits<iterator_type *>::value_type value_type;
        typedef typename iterator_traits<iterator_type *>::pointer pointer;
        typedef typename iterator_traits<iterator_type *>::reference reference;
        typedef typename iterator_traits<iterator_type *>::difference_type difference_type;
        typedef Node<typename std::remove_const<value_type>::type> *node_pointer;

    private:
        node_pointer _node_p;

    public:
        tree_iterator() {}

        tree_iterator(void *node_p) {
            this->_node_p = static_cast<node_pointer>(node_p);
        }

        tree_iterator(const tree_iterator<typename std::remove_const<value_type>::type> &other) {
            *this = other;
        }

        tree_iterator &operator=(const tree_iterator<typename std::remove_const<value_type>::type> &copy) {
            this->_node_p = copy.base();
            return (*this);
        }

        virtual ~tree_iterator() {}

        node_pointer base() const {
            return (this->_node_p);
        }

        reference operator*() const {
            return (_node_p->value);
        }

        pointer operator->() const {
            return &(operator*());
        }

        node_pointer find_min_value(node_pointer cur_node) {
            while (cur_node->left != 0)
                cur_node = cur_node->left;
            return (cur_node);
        }

        node_pointer find_max_value(node_pointer cur_node) {
            while (cur_node != 0 && cur_node->right != 0)
                cur_node = cur_node->right;
            return (cur_node);
        }

        tree_iterator &operator++() {
            node_pointer cur_node = this->_node_p;
            node_pointer res_node;

            if (cur_node->right != 0)
                this->_node_p = find_min_value(cur_node->right);
            else {
                res_node = cur_node;
                while (res_node != res_node->parent->left)
                    res_node = res_node->parent;
                this->_node_p = res_node->parent;
            }
            return (*this);
        }

        tree_iterator operator++(int) {
            tree_iterator tmp(*this);

            ++(*this);
            return (tmp);
        }

        tree_iterator &operator--() {
            node_pointer cur_node = this->_node_p;
            node_pointer res_node;

            if (cur_node->left != 0)
                this->_node_p = find_max_value(cur_node->left);
            else {
                res_node = cur_node;
                while (res_node != res_node->parent->right)
                    res_node = res_node->parent;
                this->_node_p = res_node->parent;
            }
            return (*this);
        }

        tree_iterator operator--(int) {
            tree_iterator tmp(*this);
            --(*this);
            return (tmp);
        }
    };

    template<typename A, typename B>
    bool operator==(const tree_iterator<A> &lhs,
                    const tree_iterator<B> &rhs) {
        return (lhs.base() == rhs.base());
    };

    template<typename A, typename B>
    bool operator!=(const tree_iterator<A> &lhs,
                    const tree_iterator<B> &rhs) {
        return (!(lhs.base() == rhs.base()));
    };
}

#endif