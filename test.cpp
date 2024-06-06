#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>
#include "Vector.hpp"

using namespace std;
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

    /*bool operator<(const DataSet& other) const {
        return productCode < other.productCode;
        if (this->productCode == other.productCode) { return this->strikePrice < other.strikePrice; }
        else if (this->strikePrice == other.strikePrice) { return this->expirationDate < other.expirationDate; }
        else if (this->expirationDate == other.expirationDate) { return this->optionType < other.optionType; }
        return true;
    }
    bool operator>(const DataSet& other) const {
        return productCode > other.productCode;
        if (this->productCode == other.productCode) { return this->strikePrice > other.strikePrice; }
        else if (this->strikePrice == other.strikePrice) { return this->expirationDate > other.expirationDate; }
        else if (this->expirationDate == other.expirationDate) { return this->optionType > other.optionType; }
        return true;
    }*/
    /*bool operator<(const DataSet& other) const {
        if (this->strikePrice.length() == other.strikePrice.length()) {
            return this->strikePrice <= other.strikePrice;
        }
        return this->strikePrice.length() <= other.strikePrice.length();
    }
    bool operator>(const DataSet& other) const {
        if (this->strikePrice.length() == other.strikePrice.length()) {
            return this->strikePrice > other.strikePrice;
        }
        return this->strikePrice.length() > other.strikePrice.length();
    }*/
    bool operator<(const DataSet& other) const {
        return compareDecimals(this->strikePrice, other.strikePrice);
    }

    bool operator>(const DataSet& other) const {
        return compareDecimals(other.strikePrice, this->strikePrice);
    }

    void print() {
        cout << dealDate << ", " << productCode << ", " << strikePrice << ", "
            << expirationDate << ", " << optionType << ", " << dealTime << ", "
            << dealPrice << ", " << dealVolume << endl;
    }
};

class MinHeap {
private:
    Vector<DataSet> heap;

    void heapifyUp(int index) {
        while (index > 0 && heap.at((index - 1) / 2) > heap.at(index)) {
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

            if (left < size && heap.at(left) < heap.at(smallest)) {
                smallest = left;
            }

            if (right < size && heap.at(right) < heap.at(smallest)) {
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
        heap.push_back(data);
        heapifyUp(heap.size() - 1);
    }

    DataSet extractMin() {
        if (heap.empty()) {
            throw runtime_error("Heap is empty");
        }

        DataSet minData = heap.front();
        heap.at(0) = heap.back();
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
                row.get(i + 2),
                row.get(i + 3),
                row.get(i + 4),
                row.get(i + 5),
                row.get(i + 6),
                row.get(i + 7)
            );
            minHeap.insert(data);
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
    // const string filename = "DStest.txt";
    // const string file1 = "OptionsDaily_2017_05_15.csv";
    // const string file2 = "OptionsDaily_2017_05_16.csv";
    // const string file3 = "OptionsDaily_2017_05_17.csv";
    // const string file4 = "OptionsDaily_2017_05_18.csv";
    // const string file5 = "OptionsDaily_2017_05_19.csv";
    for(int i=1; i<=5; i++){
        string file = "OptionsDaily_2017_05_1" + to_string(i) + ".csv";
        readCSV(file);
    }

    MinHeap minHeap;
    buildMinHeapFromVector(row, minHeap);
    int n = 10;
    while (n--) {
        DataSet minData = minHeap.extractMin();
        minData.print();
    }

    return 0;
}
