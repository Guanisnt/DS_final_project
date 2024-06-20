#ifndef MINHEAP_HPP
#define MINHEAP_HPP

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>
#include <iomanip>
#include "Tuple.hpp"
#include "Set.hpp"
#include "Pair.hpp"
#include "Vector.hpp"
#include "Pair3.hpp"

using namespace std;

/*split 和 compareDecimals 是處理字串數字比大小的*/
Vector<string> split(const string& str, char delimiter) {
    Vector<string> parts;
    stringstream ss(str);
    string part;
    while (getline(ss, part, delimiter)) {
        parts.push_back(part);
    }
    return parts;
}

bool compareDecimals(const string& a, const string& b) {
    auto aParts = split(a, '.');
    auto bParts = split(b, '.');

    string aIntPart = aParts.at(0);
    string aFracPart = (aParts.size() > 1) ? aParts.at(1) : "0";
    string bIntPart = bParts.at(0);
    string bFracPart = (bParts.size() > 1) ? bParts.at(1) : "0";

    if (aIntPart.length() != bIntPart.length()) {
        return aIntPart.length() < bIntPart.length();
    }
    if (aIntPart != bIntPart) {
        return aIntPart < bIntPart;
    }

    return aFracPart < bFracPart;
}

class DataSet {
private:
    string dealDate;
    string productCode;
    string strikePrice;
    string expirationDate;
    string optionType;
    string dealTime;
    string dealPrice;
    string dealVolume;
public:
    DataSet(string ddate, string pc, string sp, string ed, string ot, string dt, string dp, string dv)
        : dealDate(ddate), productCode(pc), strikePrice(sp), expirationDate(ed),
        optionType(ot), dealTime(dt), dealPrice(dp), dealVolume(dv) {}
    
    DataSet() : dealDate(""), productCode(""), strikePrice(""), expirationDate(""), optionType(""), dealTime(""), dealPrice(""), dealVolume("") {}
    string getDealDate() const {return dealDate;}
    string getProductCode() const {return productCode;}
    string getStrikePrice() const {return strikePrice;}
    string getExpirationDate() const {return expirationDate;}
    string getOptionType() const {return optionType;}
    string getDealTime() const {return dealTime;}
    string getDealPrice() const {return dealPrice;}
    string getDealVolume() const {return dealVolume;}
    bool operator<(const DataSet& other) const {
        return compareDecimals(this->dealPrice, other.dealPrice);
    }

    bool operator>(const DataSet& other) const {
        return compareDecimals(other.dealPrice, this->dealPrice);
    }

    void print() {
        cout << dealDate << ", " << productCode << ", " << strikePrice << ", "
            << expirationDate << ", " << optionType << ", " << dealTime << ", "
            << dealPrice << ", " << dealVolume << endl;
    }
};

class MinHeap {
private:
    Vector<DataSet> heap;  // 用 vector 作 heap

    void heapifyUp(int index) {
        while (index > 0 && heap.at((index - 1) / 2) > heap.at(index)) {  // 父節點比子節點大
            swap(heap.at(index), heap.at((index - 1) / 2));  // 交換
            index = (index - 1) / 2;  // 更新富節點
        }
    }

    void heapifyDown(int index) {
        int size = heap.size();
        while (true) {
            int left = 2 * index + 1;
            int right = 2 * index + 2;
            int smallest = index;

            if (left < size && heap.at(left) < heap.at(smallest)) {  // 左子節點比較小
                smallest = left;
            }

            if (right < size && heap.at(right) < heap.at(smallest)) {  // 右子節點比較小
                smallest = right;
            }

            if (smallest != index) {
                swap(heap.at(index), heap.at(smallest));
                index = smallest;
            }
            else {
                break;
            }
        }
    }

public:
    void insert(const DataSet& data) {
        heap.push_back(data);  // 每次都插入到最後一個
        heapifyUp(heap.size() - 1); // 所以從最後一個開始 heapifyUp
    }

    DataSet extractMin() {
        if (heap.empty()) {
            throw runtime_error("Heap is empty");
        }

        DataSet minData = heap.front();  // 把頭拿出來
        heap.at(0) = heap.back();  // 把最後一個放到頭
        heap.pop_back();  // 把最後一個刪掉
        heapifyDown(0);  // 從頭開始 heapifyDown

        return minData;
    }

    bool isEmpty() const {
        return heap.empty();
    }
};

#endif