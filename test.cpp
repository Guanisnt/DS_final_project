#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>
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
        return productCode < other.productCode;
        if(this->productCode == other.productCode) {return this->strikePrice < other.strikePrice;}
        if(this->strikePrice == other.strikePrice) {return this->expirationDate < other.expirationDate;}
        if(this->expirationDate == other.expirationDate) {return this->optionType < other.optionType;}
        return true;
    }
    bool operator>(const DataSet& other) const {
        return productCode > other.productCode;
        if(this->productCode == other.productCode) {return this->strikePrice > other.strikePrice;}
        if(this->strikePrice == other.strikePrice) {return this->expirationDate > other.expirationDate;}
        if(this->expirationDate == other.expirationDate) {return this->optionType > other.optionType;}
        return true;
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
private:
    Vector<DataSet> heap;

    void heapifyUp(int index) {
        while (index > 0 && heap.get((index - 1) / 2) > heap.get(index)) {
            swap(heap.at(index), heap.at((index - 1) / 2));
            index = (index - 1) / 2;
        }
    }

    void heapifyDown(int index) {
        int size = heap.size();
        while (true) {
            int left = 2 * index + 1;
            int right = 2 * index + 2;
            int smallest = index;

            if (left < size && heap.get(left) < heap.get(smallest)) {
                smallest = left;
            }

            if (right < size && heap.get(right) < heap.get(smallest)) {
                smallest = right;
            }

            if (smallest != index) {
                swap(heap.at(index), heap.at(smallest));
                index = smallest;
            } else {
                break;
            }
        }
    }

public:
    void insert(DataSet data) {
        heap.push_back(data);
        heapifyUp(heap.size() - 1);
    }

    DataSet extractMin() {
        if (heap.empty()) {
            throw runtime_error("Heap is empty");
        }

        DataSet minData = heap.front();
        heap.get(0) = heap.back();
        heap.pop_back();
        heapifyDown(0);

        return minData;
    }

    bool isEmpty() const {
        return heap.empty();
    }
};

void buildMinHeapFromVector(const Vector<string>& row, MinHeap& minHeap) {
    for (int i = 0; i < row.size(); i += 8) {
        if (i + 7 < row.size()) {
            DataSet data(
                row.get(i),
                row.get(i + 1),
                stod(row.get(i + 2)),
                row.get(i + 3),
                row.get(i + 4),
                row.get(i + 5),
                stod(row.get(i + 6)),
                stoi(row.get(i + 7))
            );
            minHeap.insert(data);
        }
    }
}

void insert(Vector<string>& heap, string value)
{
    // Add the new element to the end of the heap
    heap.push_back(value);
    // Get the index of the last element
    int index = heap.size() - 1;
    // Compare the new element with its parent and swap if
    // necessary
    while (index > 0 && heap.get((index - 1) / 2) > heap.get(index)) {
        swap(heap.at(index), heap.at((index - 1) / 2));
        // Move up the tree to the parent of the current
        // element
        index = (index - 1) / 2;
    }
}

// Function to delete a node from the min-heap
void Delete(Vector<string>& heap, string value)
{
    // Find the index of the element to be deleted
    int index = -1;
    for (int i = 0; i < heap.size(); i++) {
        if (heap.get(i) == value) {
            index = i;
            break;
        }
    }
    // If the element is not found, return
    if (index == -1) {
        return;
    }
    // Replace the element to be deleted with the last
    // element
    heap.get(index) = heap.get(heap.size() - 1);
    // Remove the last element
    heap.pop_back();
    // Heapify the tree starting from the element at the
    // deleted index
    while (true) {
        int left_child = 2 * index + 1;
        int right_child = 2 * index + 2;
        int smallest = index;
        if (left_child < heap.size()
            && heap.get(left_child) < heap.get(smallest)) {
            smallest = left_child;
        }
        if (right_child < heap.size()
            && heap.get(right_child) < heap.get(smallest)) {
            smallest = right_child;
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

    MinHeap minHeap;
    buildMinHeapFromVector(row, minHeap);

    while (!minHeap.isEmpty()) {
        DataSet minData = minHeap.extractMin();
        minData.print();
    }

    return 0;
}