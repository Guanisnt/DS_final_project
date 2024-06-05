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
        if (this->expirationDate < other.expirationDate) {
            return true;
        } else if (this->expirationDate == other.expirationDate) {
            if (this->strikePrice < other.strikePrice) {
                return true;
            } else if (this->strikePrice == other.strikePrice) {
                return this->optionType < other.optionType;
            }
        }
        return false;
    }

    bool operator>(const DataSet& other) const {
        if (this->expirationDate > other.expirationDate) {
            return true;
        } else if (this->expirationDate == other.expirationDate) {
            if (this->strikePrice > other.strikePrice) {
                return true;
            } else if (this->strikePrice == other.strikePrice) {
                return this->optionType > other.optionType;
            }
        }
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
    TreeNode** arr;
    int capacity;
    int size;
    MinHeap() {this->arr = NULL;}
};

void insert(MinHeap* heap, TreeNode* node) {
    ++heap->size;
    int i = heap->size - 1;  // 最後一個節點
    while (i && node->data < heap->arr[(i - 1) / 2]->data) {  // 如果新節點比父節點小
        heap->arr[i] = heap->arr[(i - 1) / 2];
        i = (i - 1) / 2;
    }
    heap->arr[i] = node;
}

MinHeap* newMinHeap(int capacity) {
    MinHeap* tmp = new MinHeap;
    tmp->capacity = capacity;
    tmp->size = 0;
    tmp->arr = new TreeNode * [capacity];
    return tmp;
}

void Swap(struct TreeNode** a, struct TreeNode** b) {
    struct TreeNode* tmp = *a;
    *a = *b;
    *b = tmp;
}
// 維護最小堆
void heapify(MinHeap* heap, int idx) {
    int minn = idx;
    int left = 2 * idx + 1;  // 因為是 0-index 左子節點是 2*idx+1 
    int right = 2 * idx + 2;  // 右子節點是 2*idx+2
    // 如果左子節點比父節點小，就把左子節點設為最小
    if (left < heap->size && heap->arr[left]->data < heap->arr[minn]->data) minn = left;
    // 如果右子節點比父節點小，就把右子節點設為最小
    if (right < heap->size && heap->arr[right]->data < heap->arr[minn]->data) minn = right;
    // 如果最小的不是父節點，就交換
    if (minn != idx) {
        Swap(&heap->arr[minn], &heap->arr[idx]);
        heapify(heap, minn);
    }
}

// 取得最小的節點
TreeNode* getMin(MinHeap* heap) {
    TreeNode* minn = heap->arr[0];
    heap->arr[0] = heap->arr[heap->size - 1];
    heap->size--;
    heapify(heap, 0);
    return minn;
}

Vector<string> row;
long long dataSize = 0;
void readCSV(const string& filename) {
    ifstream file(filename);
    string line;

    while (getline(file, line)) {
        stringstream ss(line);
        string field;

        while (getline(ss, field, ',')) {
            row.push_back(field);
            dataSize++;
        }
    }
    file.close();
}

int main() {
    const string filename = "DStest.txt";
    readCSV(filename);
    // for(int i = 0; i < dataSize; i++) {
    //     cout << row.get(i);
    //     if((i+1) % 8 == 0) {cout << endl;}
    // }
    MinHeap* heap = newMinHeap(dataSize / 8);
    //創建 DataSet 和 TreeNode, 並插入到 MinHeap
    for (int i = 0; i < dataSize; i += 8) {
        string dDate = row.get(i);
        string pCode = row.get(i + 1);
        double sPrice = stod(row.get(i + 2));
        string eDate = row.get(i + 3);
        string oType = row.get(i + 4);
        string dTime = row.get(i + 5);
        double dPrice = stod(row.get(i + 6));
        int dVolume = stoi(row.get(i + 7));

        DataSet* dataSet = new DataSet(dDate, pCode, sPrice, eDate, oType, dTime, dPrice, dVolume);
        TreeNode* node = new TreeNode(dataSet);
        insert(heap, node);
    }

    while (heap->size > 0) {
        TreeNode* minNode = getMin(heap);
        minNode->data->print();
    }

    return 0;
}