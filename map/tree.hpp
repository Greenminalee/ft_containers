#ifndef TREE
#define TREE

#include "../util/util.hpp"
#include "../iterator/reverse_iterator.hpp"
#include "tree_iterator.hpp"

namespace ft {

    template<class Value, class Compare, class Allocator = std::allocator<Value> >
    class tree {
    public:
        typedef Value value_type;
        typedef Compare value_compare;

        typedef typename Allocator::template rebind<Node<Value> >::other node_allocator;
        typedef typename node_allocator::pointer node_pointer;

        typedef Allocator allocator_type;
        typedef typename allocator_type::reference reference;
        typedef typename allocator_type::const_reference const_reference;
        typedef typename allocator_type::pointer pointer;
        typedef typename allocator_type::const_pointer const_pointer;
        typedef typename allocator_type::difference_type difference_type;
        typedef typename allocator_type::size_type size_type;

        typedef tree_iterator<value_type> iterator;
        typedef tree_iterator<const value_type> const_iterator;

        typedef ft::reverse_iterator<iterator> reverse_iterator;
        typedef ft::reverse_iterator<const_iterator> const_reverse_iterator;


    private:
        value_compare _comp;
        allocator_type _allocator;
        node_allocator _node_alloc;
        node_pointer _super_root;
        node_pointer _root;
        size_type _size;

    public:
        tree() : _comp(value_compare()) {
            this->_root = 0;
            this->_size = 0;
            this->_super_root = this->_node_alloc.allocate(1);
            this->_node_alloc.construct(this->_super_root, Node<value_type>());
        }

        tree(const value_compare &comp,
             const allocator_type &alloc = allocator_type()) :
                _comp(comp), _allocator(alloc), _node_alloc(node_allocator()), _root(0), _size(0) {
            this->_super_root = this->_node_alloc.allocate(1);
            this->_node_alloc.construct(this->_super_root, Node<value_type>());
        }

        template<class InputIterator>
        tree(InputIterator first, InputIterator last,
             const value_compare &comp,
             const allocator_type &alloc = allocator_type()):
                _comp(comp), _allocator(alloc), _node_alloc(node_allocator()) {
            this->_size = 0;
            this->_root = 0;
            this->_super_root = this->_node_alloc.allocate(1);
            this->_node_alloc.construct(this->_super_root, Node<value_type>());

            insert(first, last);
        }

        tree(const value_type &copy) {
            *this = copy;
        }

        tree &operator=(const tree &copy) {
            if (this == &copy)
                return (*this);
            this->_comp = copy._comp;
            this->_node_alloc = copy._node_alloc;

            if (this->_root != 0)
                this->clear();
            if (this->_super_root == 0) {
                this->_super_root = this->_node_alloc.allocate(1);
                this->_node_alloc.construct(this->_super_root, Node<value_type>());
            }

            this->insert(copy.begin(), copy.end());
            return (*this);
        }

        ~tree()
        {
            this->clear();
            this->_node_alloc.destroy(this->_super_root);
            this->_node_alloc.deallocate(this->_super_root, 1);
        }
        iterator begin() {
            if (this->_root == 0)
                return (iterator(this->_super_root));
            node_pointer cur_node = this->_root;

            while (cur_node->left != 0)
                cur_node = cur_node->left;
            return (iterator(cur_node));
        }

        const_iterator begin() const {
            if (this->_root == 0)
                return (const_iterator(this->_super_root));
            node_pointer cur_node = this->_root;

            while (cur_node->left != 0)
                cur_node = cur_node->left;
            return (const_iterator(cur_node));
        }

        iterator end() {
            return (iterator(_super_root));
        }

        const_iterator end() const {
            return (const_iterator(_super_root));
        }

        reverse_iterator rbegin() {
            return (reverse_iterator(this->end()));
        }

        const_reverse_iterator rbegin() const {
            return (const_reverse_iterator(this->end()));
        }

        reverse_iterator rend() {
            return (reverse_iterator(begin()));
        }

        const_reverse_iterator rend() const {
            return (const_reverse_iterator(begin()));
        }

        node_pointer create_value(const value_type &v) {
            node_pointer tmp_node = this->_node_alloc.allocate(1);
            this->_node_alloc.construct(tmp_node, Node<value_type>(v));
            return (tmp_node);
        }

        ft::pair<iterator, bool> insert(const value_type &val) {
            node_pointer new_node = create_value(val);

            if (this->_root == 0) {
                this->_size++;
                this->_root = new_node;
                this->_super_root->left = this->_root;
                this->_root->parent = this->_super_root;
                return (ft::pair<iterator, bool>(iterator(new_node), true));
            }

            node_pointer cur_node = this->_root;

            while (cur_node != 0) {
                if (new_node->value.first == cur_node->value.first) {
                    return (ft::pair<iterator, bool>(iterator(cur_node), false));
                } else if (this->_comp(new_node->value, cur_node->value)) {
                    if (cur_node->left == 0) {
                        cur_node->left = new_node;
                        new_node->parent = cur_node;
                        break;
                    } else {
                        cur_node = cur_node->left;
                    }
                } else {
                    if (cur_node->right == 0) {
                        cur_node->right = new_node;
                        new_node->parent = cur_node;
                        break;
                    } else {
                        cur_node = cur_node->right;
                    }
                }
            }
            update_node_height(new_node);
            rebalance(find_rebalance_node(new_node));
            this->_size++;
            ft::pair<iterator, bool> res(iterator(new_node), true);
            return (res);
        }

        iterator insert(iterator position, const value_type &val) {
            iterator tmp = position;
            return (insert(val).first);
        }

        template<class InputIterator>
        void insert(InputIterator first, InputIterator last) {
            for (; first != last; ++first) {
                insert(*first);
            }
        }

        void erase(iterator position) {
            node_pointer cur_node = position.base();
            node_pointer p_node = cur_node->parent;

            if (cur_node == this->_root && cur_node->left == 0 && cur_node->right == 0) {
                this->_root = 0;
                this->_super_root->left = 0;
                this->_size--;
                this->_node_alloc.destroy(cur_node);
                this->_node_alloc.deallocate(cur_node, 1);
            } else if (cur_node->left == 0 && cur_node->right == 0) {
                if (cur_node == p_node->left)
                    p_node->left = 0;
                else
                    p_node->right = 0;
                p_node->height--;
                update_node_height(p_node);
                e_rebalance(p_node);
                this->_size--;
                this->_node_alloc.destroy(cur_node);
                this->_node_alloc.deallocate(cur_node, 1);
            } else if (cur_node->left != 0 && cur_node->right != 0) {
                node_pointer child_node = (--position).base();
                cur_node->value = child_node->value;

                if (child_node->parent->left == child_node) {
                    if (child_node->left != 0) {
                        child_node->parent->left = child_node->left;
                        child_node->left->parent = child_node->parent;
                    } else {
                        child_node->parent->left = 0;
                    }
                } else {
                    if (child_node->left != 0) {
                        child_node->parent->right = child_node->left;
                        child_node->left->parent = child_node->parent;
                    } else {
                        child_node->parent->right = 0;
                    }
                }
                update_node_height(child_node);
                e_rebalance(child_node);
                this->_size--;
                this->_node_alloc.destroy(child_node);
                this->_node_alloc.deallocate(child_node, 1);
            } else if (cur_node->left != 0 && cur_node->right == 0) {
                if (cur_node == p_node->left) {
                    p_node->left = cur_node->left;
                    cur_node->left->parent = p_node;
                } else if (cur_node == p_node->right) {
                    p_node->right = cur_node->left;
                    cur_node->left->parent = p_node;
                }
                if (cur_node == this->_root)
                    this->_root = cur_node->left;
                update_node_height(cur_node->left);
                e_rebalance(cur_node->left);
                this->_size--;
                this->_node_alloc.destroy(cur_node);
                this->_node_alloc.deallocate(cur_node, 1);
            } else if (cur_node->left == 0 && cur_node->right != 0) {
                if (cur_node == p_node->left) {
                    p_node->left = cur_node->right;
                    cur_node->right->parent = p_node;
                } else if (cur_node == p_node->right) {
                    p_node->right = cur_node->right;
                    cur_node->right->parent = p_node;
                }
                if (cur_node == this->_root)
                    this->_root = cur_node->right;
                update_node_height(cur_node->right);
                e_rebalance(cur_node->right);
                this->_size--;
                this->_node_alloc.destroy(cur_node);
                this->_node_alloc.deallocate(cur_node, 1);
            }
        }

        size_type erase(const value_type &v) {
            node_pointer item = search(v);
            if (item == this->_super_root) {
                return (0);
            } else {
                erase(iterator(item));
                return (1);
            }

        }

        void erase(iterator first, iterator last) {
            while (first != last)
                erase(first++);
        }

        node_pointer search(const value_type &v) const {
            const_iterator iter = this->begin();

            while (iter.base() != this->_super_root) {
                if (iter->first == v.first)
                    break;
                ++iter;
            }
            return (iter.base());
        }

        iterator find(const value_type &value) {
            node_pointer find_res = search(value);
            return (find_res == NULL ? end() : iterator(find_res));
        }

        const_iterator find(const value_type &value) const {
            node_pointer find_res = search(value);
            return (find_res == NULL ? end() : const_iterator(find_res));
        }

        size_type count(const value_type &value) const {
            return (find(value) != end());
        }

        void clear() {
            erase(this->begin(), this->end());
        }

        size_type max_size() const {
            return (std::min<size_type>(_node_alloc.max_size(),
                                        std::numeric_limits<difference_type>::max()));
        }

        value_compare value_comp() const {
            return (this->_comp);
        }

        int get_height_diff(node_pointer cur_node) {
            int lsh = 0;
            int rsh = 0;

            if (cur_node == NULL)
                return (0);
            if (cur_node->left != 0)
                lsh = cur_node->left->height;

            if (cur_node->right != 0)
                rsh = cur_node->right->height;

            return (lsh - rsh);
        }

        void update_node_height(node_pointer new_node) {
            node_pointer cur_node = new_node;
            node_pointer p_node = new_node->parent;
            int root_value = cur_node->height;
            root_value++;
            while (p_node != this->_super_root) {
                if (cur_node == p_node->left) {
                    if (p_node->right && p_node->right->height >= root_value)
                        break;
                } else if (cur_node == p_node->right) {
                    if (p_node->left && p_node->left->height >= root_value)
                        break;
                }
                p_node->height = root_value;
                cur_node = cur_node->parent;
                p_node = p_node->parent;
                root_value++;
            }
        }

        node_pointer find_rebalance_node(node_pointer new_node) {
            node_pointer cur_node = new_node;
            int balance_factor = 0;
            while (cur_node != this->_super_root) {
                balance_factor = get_height_diff(cur_node);
                if (balance_factor > 1 || balance_factor < -1) {
                    break;
                }
                cur_node = cur_node->parent;
            }
            return (cur_node);
        }

        node_pointer rebalance(node_pointer cur_node) {
            node_pointer result_node = 0;

            if (cur_node == this->_super_root || cur_node == 0)
                return 0;

            int h_diff = get_height_diff(cur_node);
            if (h_diff > 1) {
                if (get_height_diff(cur_node->left) > 0) {
                    result_node = rotateLL(cur_node);
                    update_node_height(result_node);
                } else {
                    result_node = rotateLR(cur_node);
                    update_node_height(result_node);
                }
            } else if (h_diff < -1) {
                if (get_height_diff(cur_node->right) < 0) {
                    result_node = rotateRR(cur_node);
                    update_node_height(result_node);
                } else {
                    result_node = rotateRL(cur_node);
                    update_node_height(result_node);
                }
            }
            this->_root = this->_super_root->left;
            return (result_node);
        }

        void e_rebalance(node_pointer cur_node) {
            node_pointer tmp_node = cur_node;

            while (tmp_node != this->_super_root && tmp_node != 0) {
                update_node_height(tmp_node);
                tmp_node = rebalance(find_rebalance_node(tmp_node));
                if (tmp_node != this->_super_root && tmp_node != 0)
                    tmp_node = tmp_node->parent;
            }
        }

        node_pointer rotateLL(node_pointer cur_node) {
            node_pointer p_node = cur_node;
            node_pointer c_node = p_node->left;

            p_node->height -= 2;
            if (c_node->right != 0) {
                p_node->left = c_node->right;
                c_node->right->parent = p_node;
            } else {
                p_node->left = 0;
            }

            if (p_node->parent->left == p_node)
                p_node->parent->left = c_node;
            else
                p_node->parent->right = c_node;
            c_node->parent = p_node->parent;

            c_node->right = p_node;
            p_node->parent = c_node;
            return (c_node);
        }

        node_pointer rotateRR(node_pointer cur_node) {
            node_pointer p_node = cur_node;
            node_pointer c_node = p_node->right;

            p_node->height -= 2;
            if (c_node->left != 0) {
                p_node->right = c_node->left;
                c_node->left->parent = p_node;
            } else {
                p_node->right = 0;
            }

            if (p_node->parent->left == p_node)
                p_node->parent->left = c_node;
            else
                p_node->parent->right = c_node;
            c_node->parent = p_node->parent;

            c_node->left = p_node;
            p_node->parent = c_node;
            return (c_node);
        }

        node_pointer rotateLR(node_pointer cur_node) {
            node_pointer p_node = cur_node;
            node_pointer c_node = p_node->left;

            c_node->height += 1;
            c_node->right->height += 1;
            p_node->left = rotateRR(c_node);
            return (rotateLL(p_node));
        }

        node_pointer rotateRL(node_pointer cur_node) {
            node_pointer p_node = cur_node;
            node_pointer c_node = p_node->right;

            c_node->height += 1;
            c_node->left->height += 1;
            p_node->right = rotateLL(c_node);
            return (rotateRR(p_node));
        }

        size_type size() const {
            return (this->_size);
        }

        bool empty() const {
            if (this->_size == 0)
                return (true);
            else
                return (false);
        }

        void swap(tree &x) {
            std::swap(this->_comp, x._comp);
            std::swap(this->_allocator, x._allocator);
            std::swap(this->_node_alloc, x._node_alloc);

            std::swap(this->_super_root, x._super_root);
            std::swap(this->_root, x._root);
            std::swap(this->_size, x._size);
        }

        iterator lower_bound(const value_type &v) {
            iterator it = begin();

            for (; it != end(); it++) {
                if (this->_comp(*it, v) == false)
                    break;
            }
            return (it);
        }

        const_iterator lower_bound(const value_type &v) const {
            const_iterator it = begin();

            for (; it != end(); it++) {
                if (this->_comp(*it, v) == false)
                    break;
            }
            return (it);
        }

        iterator upper_bound(const value_type &v) {
            iterator it = begin();

            for (; it != end(); it++) {
                if (this->_comp(v, *it) == true)
                    break;
            }
            return (it);
        }

        const_iterator upper_bound(const value_type &v) const {
            const_iterator it = begin();

            for (; it != end(); it++) {
                if (this->_comp(v, *it) == true)
                    break;
            }
            return (it);
        }

        pair<iterator, iterator> equal_range(const value_type &v) {
            return (ft::make_pair(this->lower_bound(v), this->upper_bound(v)));
        }

        pair<const_iterator, const_iterator> equal_range(const value_type &v) const {
            return (ft::make_pair(this->lower_bound(v), this->upper_bound(v)));
        }
    };

    template<class Content, class Compare, class Alloc>
    bool operator<(const tree<Content, Compare, Alloc> &lhs, const tree<Content, Compare, Alloc> &rhs) {
        return (ft::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end()));
    }

    template<class Content, class Compare, class Alloc>
    bool operator>(const tree<Content, Compare, Alloc> &lhs, const tree<Content, Compare, Alloc> &rhs) {
        return (lhs < rhs);
    }


    template<class Content, class Compare, class Alloc>
    bool operator==(const tree<Content, Compare, Alloc> &lhs, const tree<Content, Compare, Alloc> &rhs) {
        return (lhs.size() == rhs.size() && ft::equal(lhs.begin(), lhs.end(), rhs.begin()));
    }

    template<class Content, class Compare, class Alloc>
    void swap(const tree<Content, Compare, Alloc> &lhs, const tree<Content, Compare, Alloc> &rhs) {
        lhs.swap(rhs);
    }
}

#endif
