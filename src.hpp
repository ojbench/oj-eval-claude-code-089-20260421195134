#ifndef SRC_HPP
#define SRC_HPP

#include <algorithm>

class AllocatorTester {
private:
    struct Node {
        int size;
        int addr;
        bool allocated;
        Node *left, *right, *parent;

        Node(int s, int a, Node* p = nullptr)
            : size(s), addr(a), allocated(false), left(nullptr), right(nullptr), parent(p) {}

        ~Node() {
            if (left) delete left;
            if (right) delete right;
        }
    };

    Node* root;
    int min_block_size;

    Node* find_free(Node* node, int size) {
        if (!node || node->size < size) return nullptr;
        if (node->allocated) return nullptr;

        if (!node->left && !node->right) {
            if (node->size == size) return node;
            int half = node->size / 2;
            if (half >= size && half >= min_block_size) {
                node->left = new Node(half, node->addr, node);
                node->right = new Node(half, node->addr + half, node);
                return find_free(node->left, size);
            }
            return nullptr;
        }

        Node* res = find_free(node->left, size);
        if (res) return res;
        return find_free(node->right, size);
    }

    Node* find_at(Node* node, int addr, int size) {
        if (!node || node->addr > addr || node->addr + node->size <= addr) return nullptr;

        if (node->size == size && node->addr == addr) {
            if (node->allocated || node->left || node->right) return nullptr;
            return node;
        }

        if (node->allocated) return nullptr;

        if (!node->left && !node->right) {
            int half = node->size / 2;
            if (half >= size && half >= min_block_size) {
                node->left = new Node(half, node->addr, node);
                node->right = new Node(half, node->addr + half, node);
            } else {
                return nullptr;
            }
        }

        Node* res = find_at(node->left, addr, size);
        if (res) return res;
        return find_at(node->right, addr, size);
    }

    void merge(Node* node) {
        if (!node || !node->parent) return;
        Node* p = node->parent;
        if (p->left && !p->left->allocated && !p->left->left && !p->left->right &&
            p->right && !p->right->allocated && !p->right->left && !p->right->right) {
            delete p->left;
            delete p->right;
            p->left = p->right = nullptr;
            merge(p);
        }
    }

    Node* find_node_to_free(Node* node, int addr, int size) {
        if (!node || node->addr > addr || node->addr + node->size <= addr) return nullptr;
        if (node->size == size && node->addr == addr) {
            if (node->allocated && !node->left && !node->right) return node;
            return nullptr;
        }

        Node* res = find_node_to_free(node->left, addr, size);
        if (res) return res;
        return find_node_to_free(node->right, addr, size);
    }

public:
    AllocatorTester(int ram_size, int min_block_sz) : min_block_size(min_block_sz) {
        root = new Node(ram_size, 0);
    }

    ~AllocatorTester() {
        delete root;
    }

    int malloc(int size) {
        Node* node = find_free(root, size);
        if (node) {
            node->allocated = true;
            return node->addr;
        }
        return -1;
    }

    int malloc_at(int addr, int size) {
        Node* node = find_at(root, addr, size);
        if (node) {
            node->allocated = true;
            return node->addr;
        }
        return -1;
    }

    void free_at(int addr, int size) {
        Node* node = find_node_to_free(root, addr, size);
        if (node) {
            node->allocated = false;
            merge(node);
        }
    }
};

#endif
