#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "queue.hpp"
#include "vector.hpp"
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
    int dealVolume;
public:
    DataSet(string ddate, string pc, double sp, string ed, string ot, string dt, double dp, int dv) {
        this->dealDate = ddate;
        this->productCode = pc;
        this->strikePrice = sp;
        this->expirationDate = ed;
        this->optionType = ot;
        this->dealTime = dt;
        this->dealVolume = dv;
        this->dealPrice = dp;
    }
    bool operator<(const DataSet& other) const {
        if(this->productCode != other.productCode) {return this->productCode < other.productCode;}
        if(this->strikePrice != other.strikePrice) {return this->strikePrice < other.strikePrice;}
        if(this->expirationDate != other.expirationDate) {return this->expirationDate < other.expirationDate;}
        if(this->optionType != other.optionType) {return this->optionType < other.optionType;}
        return false;
    }
    bool operator>(const DataSet& other) const {
        if(this->productCode != other.productCode) {return this->productCode > other.productCode;}
        if(this->strikePrice != other.strikePrice) {return this->strikePrice > other.strikePrice;}
        if(this->expirationDate != other.expirationDate) {return this->expirationDate > other.expirationDate;}
        if(this->optionType != other.optionType) {return this->optionType > other.optionType;}
        return false;
    }
    void print() {
        cout << dealDate << ", " << productCode << ", " << strikePrice << ", " << expirationDate << ", " << optionType << ", " << dealTime << ", " << dealPrice << ", " << dealVolume << endl;
    }
};

class TreeNode {
public:
    DataSet* data;
    TreeNode* left;
    TreeNode* right;
    TreeNode* parent;
    TreeNode(DataSet* data) {
        this->parent = NULL;
        this->data = data;
        this->left = NULL;
        this->right = NULL;
    }
};

class MinHeap {
public:
    TreeNode* root;
    MinHeap() {
        this->root = NULL;
    }
    bool isEmpty() {
        return this->root == NULL;
    }
};
/*-------------------------------------------------------*/
void heapify(MinHeap* heap, TreeNode* node);
TreeNode* removeMin(MinHeap& heap, TreeNode* node);

void insert(MinHeap* heap, DataSet* data) {
    TreeNode* newNode = new TreeNode(data);
    if (heap->root == NULL) {
        heap->root = newNode;
        return;
    }

    TreeNode* current = heap->root;
    TreeNode* parent = NULL;

    while (current != NULL) {
        parent = current;
        if (*newNode->data < *current->data) {
            current = current->left;
        } else {
            current = current->right;
        }
    }

    if (*newNode->data < *parent->data) {
        parent->left = newNode;
    } else {
        parent->right = newNode;
    }
    newNode->parent = parent;
    heapify(heap, newNode);
}

DataSet* extractMin(MinHeap& heap) {
    if (heap.root == NULL) {
        return NULL;
    }

    DataSet* minData = heap.root->data;
    TreeNode* minNode = heap.root;

    if (heap.root->left == NULL && heap.root->right == NULL) {
        heap.root = NULL;
        delete minNode;
        return minData;
    }

    heap.root = removeMin(heap, heap.root);
    return minData;
}

TreeNode* removeMin(MinHeap& heap, TreeNode* node) {
    if (node->left == NULL) {
        TreeNode* temp = node->right;
        delete node;
        return temp;
    }

    if (node->left->left == NULL && node->left->right == NULL) {
        TreeNode* temp = node->left;
        delete node->left;
        node->left = NULL;
        heapify(&heap, node);
        return node;
    }

    node->left = removeMin(heap, node->left);
    heapify(&heap, node);
    return node;
}

void heapify(MinHeap* heap, TreeNode* node) {
    if (node == NULL || (node->left == NULL && node->right == NULL)) {
        return;
    }

    TreeNode* smallest = node;
    if (node->left != NULL && *node->left->data < *smallest->data) {
        smallest = node->left;
    }
    if (node->right != NULL && *node->right->data < *smallest->data) {
        smallest = node->right;
    }

    if (smallest != node) {
        swap(node, smallest);
        heapify(heap, smallest);
    }
}

void readCSV(const string& filename, MinHeap& heap) {
    ifstream file(filename);
    string line;
    getline(file, line); // Skip the header row
    while (getline(file, line)) {
        istringstream iss(line);
        string field;
        Vector<string> fields;
        while (getline(iss, field, ',')) {
            fields.push_back(field);
        }
        DataSet* data = new DataSet(fields.get(0), fields.get(1), stod(fields.get(2)), fields.get(3), fields.get(4), fields.get(5), stod(fields.get(6)), stoi(fields.get(7)));
        insert(&heap, data);
    }
    file.close();
}
// void readCSV(const string& filename, MinHeap& heap) {
//     ifstream file(filename);
//     if (!file.is_open()) {
//         cerr << "Error opening file: " << filename << endl;
//         return;
//     }

//     string line;
//     while (getline(file, line)) {
//         stringstream ss(line);
//         string dealDate;
//         string productCode;
//         double strikePrice;
//         string expirationDate;
//         string optionType;
//         string dealTime;
//         double dealPrice;
//         int dealVolume;

//         //cout << "Line: " << line << endl; // 輸出木前讀取的整行

//         getline(ss, dealDate, ',');
//         getline(ss, productCode, ',');
//         ss >> strikePrice;
//         ss.ignore();
//         getline(ss, expirationDate, ',');
//         getline(ss, optionType, ',');
//         getline(ss, dealTime, ',');
//         ss >> dealPrice;
//         ss.ignore();
//         ss >> dealVolume;
//         ss.ignore();
//         DataSet* data = new DataSet(dealDate, productCode, strikePrice, expirationDate, optionType, dealTime, dealPrice, dealVolume);
//         insert(&heap, data);
//     }
//     cout << "Read done\n";
//     file.close();
// }

int main() {
    MinHeap heap;
    const string filename = "DStest.txt";
    // const string filename = "OptionsDaily_2017_05_15.csv";
    readCSV(filename, heap);
    cout << "Sorted records are: \n";
    int cnt = 10;
    while (cnt--) {
        DataSet* data = extractMin(heap);
        data->print();
    }
    return 0;
}