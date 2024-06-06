#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>
#include <vector>

using namespace std;
vector<string> split(const string& str, char delimiter) {
    vector<string> parts;
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

    string aIntPart = aParts[0];
    string aFracPart = (aParts.size() > 1) ? aParts[1] : "0";
    string bIntPart = bParts[0];
    string bFracPart = (bParts.size() > 1) ? bParts[1] : "0";

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
    vector<DataSet> heap;

    void heapifyUp(int index) {
        while (index > 0 && heap[(index - 1) / 2] > heap[index]) {
            swap(heap[index], heap[(index - 1) / 2]);
            index = (index - 1) / 2;
        }
    }

    void heapifyDown(int index) {
        int size = heap.size();
        while (true) {
            int left = 2 * index + 1;
            int right = 2 * index + 2;
            int smallest = index;

            if (left < size && heap[left] < heap[smallest]) {
                smallest = left;
            }

            if (right < size && heap[right] < heap[smallest]) {
                smallest = right;
            }

            if (smallest != index) {
                swap(heap[index], heap[smallest]);
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
        heap[0] = heap.back();
        heap.pop_back();
        heapifyDown(0);

        return minData;
    }

    bool isEmpty() const {
        return heap.empty();
    }
};

void buildMinHeapFromVector(const vector<string>& row, MinHeap& minHeap) {
    for (int i = 0; i < row.size(); i += 8) {
        if (i + 7 < row.size()) {
            DataSet data(
                row[i],
                row[i + 1],
                row[i + 2],
                row[i + 3],
                row[i + 4],
                row[i + 5],
                row[i + 6],
                row[i + 7]
            );
            minHeap.insert(data);
        }
    }
}

vector<string> row;
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
    //const string filename = "DStest.txt";
    const string filename = "OptionsDaily_2017_05_15.csv";
    readCSV(filename);

    MinHeap minHeap;
    buildMinHeapFromVector(row, minHeap);
    int n = 10;
    while (n--) {
        DataSet minData = minHeap.extractMin();
        minData.print();
    }

    return 0;
}
