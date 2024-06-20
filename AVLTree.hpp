#ifndef AVLTREE_HPP
#define AVLTREE_HPP

#include <iostream>
#include <sstream>
#include <string>
#include <fstream>
#include <algorithm>
#include "Vector.hpp"
#include "Pair3.hpp"
#include "Pair.hpp"
#include "Set.hpp"

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

    bool operator==(const DataSet& other) const {
        return this->dealDate == other.dealDate && this->productCode == other.productCode && this->strikePrice == other.strikePrice &&
               this->expirationDate == other.expirationDate && this->optionType == other.optionType && this->dealTime == other.dealTime &&
               this->dealPrice == other.dealPrice && this->dealVolume == other.dealVolume;
    }

    bool isEqualTo(const DataSet& other) const {
        return this->productCode == other.productCode &&
               this->strikePrice == other.strikePrice &&
               this->expirationDate == other.expirationDate &&
               this->optionType == other.optionType;
    }

    void print() {
        cout << dealDate << ", " << productCode << ", " << strikePrice << ", "
             << expirationDate << ", " << optionType << ", " << dealTime << ", "
             << dealPrice << ", " << dealVolume << endl;
    }
};

class AVLTreeNode {
public:
    DataSet data;
    AVLTreeNode *left;
    AVLTreeNode *right;
    int count;
    int height;

    AVLTreeNode() : data(), left(nullptr), right(nullptr), count(1), height(1) {}
};

int height(AVLTreeNode *node) {
    if (node == nullptr) return 0;
    return node->height;
}

AVLTreeNode* newNode(DataSet data) {
    AVLTreeNode* node = new AVLTreeNode();
    node->data = data;
    node->left = nullptr;
    node->right = nullptr;
    node->height = 1;  // 新節點加到 leaf
    return node;
}

AVLTreeNode *LL(AVLTreeNode *y) {
    AVLTreeNode *x = y->left;  // x 是 y 的 left child
    AVLTreeNode *T2 = x->right;  // T2 是 x 的右子樹

    x->right = y;
    y->left = T2;

    y->height = max(height(y->left), height(y->right)) + 1;  // +1 是因為自己本身也算一層
    x->height = max(height(x->left), height(x->right)) + 1;

    return x;  // 新的跟
}

AVLTreeNode *RR(AVLTreeNode *x) {
    AVLTreeNode *y = x->right;
    AVLTreeNode *T2 = y->left;

    y->left = x;
    x->right = T2;

    x->height = max(height(x->left), height(x->right)) + 1;
    y->height = max(height(y->left), height(y->right)) + 1;

    return y;
}

int getBalance(AVLTreeNode *node) {
    if (node == nullptr) return 0;
    return height(node->left) - height(node->right);
}

AVLTreeNode* insert(AVLTreeNode* node, DataSet data) {
    if (node == nullptr) return newNode(data);

    if (data < node->data) {
        node->left = insert(node->left, data);
    } else if (data > node->data) {
        node->right = insert(node->right, data);
    } else {
        return node;
    }

    node->height = max(height(node->left), height(node->right)) + 1;
    int bf = getBalance(node);

    if (bf > 1 && data < node->left->data) { return LL(node); }
    if (bf < -1 && data > node->right->data) { return RR(node); }
    if (bf > 1 && data > node->left->data) {
        node->left = RR(node->left);
        return LL(node);
    }
    if (bf < -1 && data < node->right->data) {
        node->right = LL(node->right);
        return RR(node);
    }

    return node;
}

void inOrder(AVLTreeNode* root, Vector<DataSet>& vec) {
    if (root != nullptr) {
        inOrder(root->left, vec);
        vec.push_back(root->data);
        inOrder(root->right, vec);
    }
}



#endif
