#ifndef AVLTREE_H
#define AVLTREE_H
#include <iostream>
#include<string>
#include"Tuple.hpp"
using namespace std;

class AVLTree {
private:
    struct Node {
        Tuple<string> key;
        Node* left;
        Node* right;
        int height;

        Node(Tuple<string> k) : key(k), left(nullptr), right(nullptr), height(1) {}
    };

    int size;

    Node* root;

    int height(Node* n) {
        return n == nullptr ? 0 : n->height;
    }

    int max(int a, int b) {
        return (a > b) ? a : b;
    }

    Node* rightRotate(Node* y) {
        Node* x = y->left;
        Node* T2 = x->right;

        x->right = y;
        y->left = T2;

        y->height = max(height(y->left), height(y->right)) + 1;
        x->height = max(height(x->left), height(x->right)) + 1;

        return x;
    }

    Node* leftRotate(Node* x) {
        Node* y = x->right;
        Node* T2 = y->left;

        y->left = x;
        x->right = T2;

        x->height = max(height(x->left), height(x->right)) + 1;
        y->height = max(height(y->left), height(y->right)) + 1;

        return y;
    }

    int getBalance(Node* n) {
        return n == nullptr ? 0 : height(n->left) - height(n->right);
    }

    Node* insert(Node* node, Tuple<string> key) {
        if (node == nullptr)
            return new Node(key);

        if (key < node->key)
            node->left = insert(node->left, key);
        else if (key > node->key)
            node->right = insert(node->right, key);
        else
            return node; // 重複的鍵不允許

        node->height = 1 + max(height(node->left), height(node->right));

        int balance = getBalance(node);

        // 左左情況
        if (balance > 1 && key < node->left->key)
            return rightRotate(node);

        // 右右情況
        if (balance < -1 && key > node->right->key)
            return leftRotate(node);

        // 左右情況
        if (balance > 1 && key > node->left->key) {
            node->left = leftRotate(node->left);
            return rightRotate(node);
        }

        // 右左情況
        if (balance < -1 && key < node->right->key) {
            node->right = rightRotate(node->right);
            return leftRotate(node);
        }

        return node;
    }

    Node* search(Node* root, const Tuple<string>& key) {
        if (root == nullptr || root->key == key)
            return root;

        if (key < root->key)
            return search(root->left, key);

        return search(root->right, key);
    }

    void preOrder(Node* root) {
        if (root != nullptr) {
            cout << root->key.data[0] << "_" << root->key.data[1] << "_" << root->key.data[2] << "_" << root->key.data[3]<<endl;
            preOrder(root->left);
            preOrder(root->right);
        }
    }

public:
    AVLTree() : root(nullptr),size(0) {}

    void insert(Tuple<string> key) {
        size++;
        root = insert(root, key);
    }

    bool search(Tuple<string> key){
        return search(root,key) != nullptr;
    }

    void preOrder() {
        preOrder(root);
        cout << endl;
    }
};

#endif