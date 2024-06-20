#ifndef AVLTREE_H
#define AVLTREE_H
#include <iostream>
#include<string>
#include"Tuple.hpp"
#include "Vector.hpp"
using namespace std;

class DataSet {
private:
    string dealDate;
    string productCode;
    double strikePrice;
    string expirationDate;
    string optionType;
    string dealTime;
    double dealPrice;
    string dealVolume;
public:
    DataSet(string ddate, string pc, double sp, string ed, string ot, string dt, double dp, string dv)
        : dealDate(ddate), productCode(pc), strikePrice(sp), expirationDate(ed),
        optionType(ot), dealTime(dt), dealPrice(dp), dealVolume(dv) {}

    DataSet() : dealDate(""), productCode(""), strikePrice(0.0), expirationDate(""), optionType(""), dealTime(""), dealPrice(0.0), dealVolume("") {}

    string getDealDate() const { return dealDate; }
    string getProductCode() const { return productCode; }
    double getStrikePrice() const { return strikePrice; }
    string getExpirationDate() const { return expirationDate; }
    string getOptionType() const { return optionType; }
    string getDealTime() const { return dealTime; }
    double getDealPrice() const { return dealPrice; }
    string getDealVolume() const { return dealVolume; }

    // 這裡要改成就算 dealPrice 一樣，只要 dealTime 不一樣也可以插入
    bool operator<(const DataSet& other) const {
        if (this->dealPrice < other.dealPrice) {
            return true;
        } else if (this->dealPrice == other.dealPrice) {
            return this->dealTime < other.dealTime;
        } else {
            return false;
        }
    }

    bool operator>(const DataSet& other) const {
        if (this->dealPrice > other.dealPrice) {
            return true;
        } else if (this->dealPrice == other.dealPrice) {
            return this->dealTime > other.dealTime;
        } else {
            return false;
        }
    }

    void print() {
        cout << dealDate << ", " << productCode << ", " << strikePrice << ", "
            << expirationDate << ", " << optionType << ", " << dealTime << ", "
            << dealPrice << ", " << dealVolume << endl;
    }
};

template <class T>
class AVLTree {
private:
    struct Node {
        T key;
        Node* left;
        Node* right;
        int height;
        // T data;
        // Node(Tuple<string> k) : key(k), left(nullptr), right(nullptr), height(1) {}
        // Node(DataSet d) : data(d), left(nullptr), right(nullptr) {}
        Node(T k) : key(k), left(nullptr), right(nullptr), height(1) {}  // 初始化 data 為 k
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

    Node* insert(Node* node, T& key) {
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

    void inOrder(Node* root, Vector<DataSet>& vec) {
        if (root != nullptr) {
            inOrder(root->left, vec);
            vec.push_back(root->key);
            inOrder(root->right, vec);
        }
    }

public:
    // Node* DataRoot;
    // AVLTree() : root(nullptr), size(0), DataRoot(nullptr) {}
    AVLTree() : root(nullptr) {}
    void insert(T key) {
        size++;
        root = insert(root, key);
    }

    void inOrderTraversal(Vector<DataSet>& vec) {
        inOrder(root, vec);
    }

    bool search(Tuple<string> key){
        return search(root,key) != nullptr;
    }

    void preOrder() {
        preOrder(root);
        cout << endl;
    }

    void printTopAndBottom10() {
        Vector<DataSet> vec;
        inOrderTraversal(vec);
        int n = vec.size();

        cout << "10 smallest prices for TXO_9900_201705_C:" << endl;
        for (int i = 0; i < 10 && i < n; i++) {
            // cout << vec.at(i).getDealPrice() << " " << vec.at(i).getDealTime() << endl;
            vec.at(i).print();
        }
        cout << endl;

        cout << "10 largest prices for TXO_9900_201705_C:" << endl;
        for (int i = n - 1; i >= n - 10 && i >= 0; i--) {
            // cout << vec.at(i).getDealPrice() << " " << vec.at(i).getDealTime() << endl;
            vec.at(i).print();
        }
        cout << endl;
    }

    void findMedian() {
        Vector<DataSet> vec;
        inOrderTraversal(vec);
        int n = vec.size();
        if (n % 2 == 0) {
            cout << "Median price for TXO_9900_201705_C: " << (vec.at(n / 2).getDealPrice() + vec.at(n / 2 - 1).getDealPrice()) / 2 << endl;
        } else {
            cout << "Median price for TXO_9900_201705_C: " << vec.at(n / 2).getDealPrice() << endl;
        }
    }
};

#endif