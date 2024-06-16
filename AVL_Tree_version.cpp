#include "Vector.hpp"
#include <iostream>
#include <sstream>
#include <string>
#include <fstream>
#include <algorithm>
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
        return this->dealPrice < other.dealPrice;
    }

    bool operator>(const DataSet& other) const {
        return this->dealPrice > other.dealPrice;
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

// 建立新節點
AVLTreeNode* newNode(DataSet data) {
    AVLTreeNode* node = new AVLTreeNode();
    node->data = data;
    node->left = nullptr;
    node->right = nullptr;
    node->height = 1;  // 新節點加到 leaf
    return(node);
}

// 順時針旋轉
AVLTreeNode *LL(AVLTreeNode *y) {
    AVLTreeNode *x = y->left;  // x 是 y 的 left child
    AVLTreeNode *T2 = x->right;  // T2 是 x 的右子樹

    x->right = y;
    y->left = T2;
//     y                          x
//    / \         LL (y)         / \ 
//   x   T3 --------------->   T1   y
//  / \                            / \ 
// T1  T2                         T2  T3

    // 更新高度
    y->height = max(height(y->left), height(y->right)) + 1;  // +1 是因為自己本身也算一層
    x->height = max(height(x->left), height(x->right)) + 1;

    return x;  // 新的跟
}

// 逆時針旋轉
AVLTreeNode *RR(AVLTreeNode *x) {
    AVLTreeNode *y = x->right;
    AVLTreeNode *T2 = y->left;

    y->left = x;
    x->right = T2;
//     x                          y
//    / \         RR (y)         / \ 
//   T1   y --------------->    x   T3
//       / \                   / \ 
//      T2  T3               T1   T2

    x->height = max(height(x->left), height(x->right)) + 1;
    y->height = max(height(y->left), height(y->right)) + 1;

    return y;
}

// 算 Balance factor
int getBalance(AVLTreeNode *node) {
    if (node == nullptr) return 0;
    return height(node->left) - height(node->right);
}

AVLTreeNode* insert(AVLTreeNode* node, DataSet data) {
    // if (node != nullptr && node->data.getDealTime() == data.getDealTime() && node->data.getDealPrice() == data.getDealPrice() && node->data.getDealDate() == data.getDealDate()){
    //     return node;  // 如果存在相同的 dealTime，直接返回
    // }
    // 先照 BST 的方式插入
    if (node == nullptr) return(newNode(data));
    // 遞迴找位置
    if (data < node->data) {
        node->left = insert(node->left, data);
    } else if (data > node->data) {
        node->right = insert(node->right, data);
    } else {  // 相等的話不插入
        node->count++;
        return node;
    }

    node->height = max(height(node->left), height(node->right)) + 1;  // 更新高度
    int bf = getBalance(node);

    // LL
    if (bf > 1 && data < node->left->data) {return LL(node);}

    // RR
    if (bf < -1 && data > node->right->data) {return RR(node);}

    // LR
    if (bf > 1 && data > node->left->data) {
        node->left = RR(node->left);
        return LL(node);
    }

    // RL
    if (bf < -1 && data < node->right->data) {
        node->right = LL(node->right);
        return RR(node);
    }

    return node;
}

// void inOrder(AVLTreeNode* root, Vector<DataSet>& vec) {
//     if (root != nullptr) {
//         inOrder(root->left, vec);
//         vec.push_back(root->data);
//         inOrder(root->right, vec);
//     }
// }
void inOrder(AVLTreeNode* root, Vector<DataSet>& vec) {
    if (root != nullptr) {
        inOrder(root->left, vec);
        for (int i = 0; i < root->count; ++i) {  // 重複的 price 也輸出
            vec.push_back(root->data);
        }
        inOrder(root->right, vec);
    }
}

Set<Pair<string, double>> seen;
void readCSV(const string& filename, AVLTreeNode*& root) {
    ifstream file(filename.c_str());
    string line;

    if (!file.is_open()) {
        cout << "open fail: " << filename << endl;
        return;
    }

    while (getline(file, line)) {
        string dealDate, productCode, expirationDate, optionType, dealTime, dealVolume;
        double strikePrice, dealPrice;
        char delimiter;

        stringstream ss(line);

        getline(ss, dealDate, ',');
        getline(ss, productCode, ',');
        ss >> strikePrice >> delimiter;
        getline(ss, expirationDate, ',');
        getline(ss, optionType, ',');
        getline(ss, dealTime, ',');
        ss >> dealPrice >> delimiter;
        getline(ss, dealVolume, ',');

        // DataSet data(dealDate, productCode, strikePrice, expirationDate, optionType, dealTime, dealPrice, dealVolume);
        // root = insert(root, data);
        if (productCode == "    TXO     " && strikePrice == 9900 && expirationDate == "201705" && optionType == "    C     ") {
            DataSet data(dealDate, productCode, strikePrice, expirationDate, optionType, dealTime, dealPrice, dealVolume);
            if (seen.find({data.getDealTime(), data.getDealPrice()}) == seen.end()) {
                // 如果沒有輸出過，加到 set 和 tree
                seen.insert({data.getDealTime(), data.getDealPrice()});
                root = insert(root, data);
            }
            // root = insert(root, data);
        }
    }

    file.close();
}

void printTopAndBottom10(AVLTreeNode* root) {
    Vector<DataSet> vec;
    inOrder(root, vec);
    int n = vec.size();

    cout << "10 samllest prices for TXO_9900_201705_C:" << endl;
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

int main() {
    AVLTreeNode *root = nullptr;
    for(int i=5; i<=9; i++){
        string file = "OptionsDaily_2017_05_1" + to_string(i) + ".csv";
        cout << file << endl;
        readCSV(file, root);
    }
    printTopAndBottom10(root);

    return 0;
}
