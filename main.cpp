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

long long cnt = 0;
// 每 8 個 col 一組，建成 DataSet 的物件，把物件差到 minHeap
void buildMinHeapFromVector(const Vector<string>& row, MinHeap& minHeap) {
    Set<Tuple<string>> uniqueData;  // 第一題
    for (int i = 0; i < row.size(); i += 8) {
        if (i + 7 < row.size()) {
            cnt++;
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
            uniqueData.insert(Tuple<string>(row.get(i + 1), row.get(i + 2), row.get(i + 3), row.get(i + 4)));
        }
    }
    cout << "Total data count: " << cnt << endl;
    cout << "Unique data count: " << uniqueData.size() << endl;
}

// row 存放所有資料，每個 col 分開存
Vector<string> row;  // 存 data 的 vector
void readCSV(const string& filename) {
    ifstream file(filename);
    string line;

    while (getline(file, line)) {
        stringstream ss(line);
        string field;

        while (getline(ss, field, ',')) {  // 逗號分隔
            row.push_back(field);
        }
    }
    file.close();
}

Vector<DataSet> dataSets;  // 存 DataSet 的 vector
bool productExists(string productCode, string strikePrice, string expirationDate, string optionType) {
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
            dataSets.push_back(data);
        }
    }
    for (int i = 0; i < dataSets.size(); ++i) {
        const DataSet& data = dataSets.at(i);
        if (data.getProductCode() == productCode && data.getStrikePrice() == strikePrice && data.getExpirationDate() == expirationDate && data.getOptionType() == optionType) {
            return true;
        }
    }
    return false;
}

Vector<DataSet> dataSetForTick;  // 存 DataSet 的 vector
void filterDataSets(const string& productCode, const string& strikePrice, const string& expirationDate, const string& optionType) {
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

            if (data.getProductCode() == productCode && data.getStrikePrice() == strikePrice && data.getExpirationDate() == expirationDate && data.getOptionType() == optionType) {
                dataSetForTick.push_back(data);
            }
        }
    }
}

Vector<DataSet> heapSort(MinHeap& minHeap) {
    Vector<DataSet> sortedVector;
    Set<Pair3<string, string, string>> seen;
    while (!minHeap.isEmpty()) {
        DataSet data = minHeap.extractMin();
        if (data.getProductCode() == "    TXO     " && data.getStrikePrice() == "9900" && data.getExpirationDate() == "201705" && data.getOptionType() == "    C     ") {
            // 檢查這個 dealDate 和 dealTime 是否已經輸出過
            if (seen.find({data.getDealDate(), data.getDealTime(), data.getDealPrice()}) == seen.end()) {
                // 如果沒有輸出過，加到 set 和 heap
                seen.insert({data.getDealDate(), data.getDealTime(), data.getDealPrice()});
                sortedVector.push_back(data);
            }
        }
    }
    return sortedVector;
}

void computeTick(Vector<DataSet>& dataSets) {
    double maxReturn = -1.0;
    double minReturn = 1.0;
    DataSet maxReturnData;
    DataSet minReturnData;

    for (size_t i = 1; i < dataSets.size(); ++i) {
        double previousPrice = stod(dataSets.at(i - 1).getDealPrice());
        double currentPrice = stod(dataSets.at(i).getDealPrice());
        double tickReturn = (currentPrice - previousPrice) / previousPrice;

        if (tickReturn > maxReturn) {
            maxReturn = tickReturn;
            maxReturnData = dataSets.at(i);
        }

        if (tickReturn < minReturn) {
            minReturn = tickReturn;
            minReturnData = dataSets.at(i);
        }
    }

    cout << "Max Return: " << maxReturn << " at ";
    maxReturnData.print();
    cout << "Min Return: " << minReturn << " at ";
    minReturnData.print();
}


int main() {
    for(int i=5; i<=9; i++){
        string file = "OptionsDaily_2017_05_1" + to_string(i) + ".csv";
        cout << file << endl;
        readCSV(file);
    }
    /*第 2 3 4 題*/
    string product1 = "TXO_1000_201706_P";
    string product2 = "TXO_9500_201706_C";
    string product3 = "GIO_5500_201706_C";

    if (productExists("    TXO     ", "1000", "201706", "    P     ")) {
        cout << product1 << " exists in the datasets." << endl;
    } else {
        cout << product1 << " does not exist in the datasets." << endl;
    }

    if (productExists("    TXO     ", "9500", "201706", "    C     ")) {
        cout << product2 << " exists in the datasets." << endl;
    } else {
        cout << product2 << " does not exist in the datasets." << endl;
    }

    if (productExists("    GIO     ", "5500", "201706", "    C     ")) {
        cout << product3 << " exists in the datasets." << endl;
    } else {
        cout << product3 << " does not exist in the datasets." << endl;
    }
    /*************************************************************** */

    /*第 5 題 a*/
    MinHeap minHeap;
    buildMinHeapFromVector(row, minHeap);

    Vector<DataSet> sortedData = heapSort(minHeap);
    cout << "10 samllest prices for TXO_9900_201705_C" << endl;
    for (int i = 0; i < 10; i++) {
        sortedData.at(i).print();
    }

    /*第 5 題 b*/
    cout << "10 largest prices for TXO_9900_201705_C" << endl;
    for (int i = sortedData.size() - 1; i >= sortedData.size() - 10; i--) {
        sortedData.at(i).print();
    }

    /*第 5 題 c*/
    cout << "meidan price for TXO_9900_201705_C: ";
    string medianPrice;
    if (sortedData.size() % 2 == 0) {
        medianPrice = sortedData.at(sortedData.size() / 2).getDealPrice();
    } else {
        medianPrice = sortedData.at(sortedData.size() / 2).getDealPrice();
    }
    cout << medianPrice << endl;

    /*第 5 題 d*/
    filterDataSets("    TXO     ", "9900", "201705", "    C     ");
    computeTick(dataSetForTick);

    return 0;
}
