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
    Set<Tuple<string>> uniqueData;
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

class MaxHeap {
private:
    Vector<DataSet> heap;  // 用 vector 作 heap

    void heapifyUp(int index) {
        while (index > 0 && heap.at((index - 1) / 2) < heap.at(index)) {  // 父節點比子節點小
            swap(heap.at(index), heap.at((index - 1) / 2));  // 交換
            index = (index - 1) / 2;  // 更新富節點
        }
    }

    void heapifyDown(int index) {
        int size = heap.size();
        while (true) {
            int left = 2 * index + 1;
            int right = 2 * index + 2;
            int largest = index;

            if (left < size && heap.at(left) > heap.at(largest)) {  // 左子節點比較大
                largest = left;
            }

            if (right < size && heap.at(right) > heap.at(largest)) {  // 右子節點比較大
                largest = right;
            }

            if (largest != index) {
                swap(heap.at(index), heap.at(largest));
                index = largest;
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

    DataSet extractMax() {
        if (heap.empty()) {
            throw runtime_error("Heap is empty");
        }

        DataSet maxData = heap.front();  // 把頭拿出來
        heap.at(0) = heap.back();  // 把最後一個放到頭
        heap.pop_back();  // 把最後一個刪掉
        heapifyDown(0);  // 從頭開始 heapifyDown

        return maxData;
    }

    bool isEmpty() const {
        return heap.empty();
    }
};

// 每 8 個 col 一組，建成 DataSet 的物件，把物件差到 maxHeap
void buildMaxHeapFromVector(const Vector<string>& row, MaxHeap& maxHeap) {
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
            maxHeap.insert(data);
        }
    }
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

void sortVector(Vector<string>& vec) {
    for (int i = 0; i < vec.size(); ++i) {
        for (int j = 0; j < vec.size() - 1; ++j) {
            if (!compareDecimals(vec.at(j), vec.at(j + 1))) {  // 如果前面的比後面的大
                swap(vec.at(j), vec.at(j + 1));
            }
        }
    }
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

    // 建立一個新的 min heap 來存儲價格和時間
    MinHeap priceTimeHeap;
    // 建立一個 set 來存儲已經輸出過的 dealDate 和 dealTime
    Set<Pair<string, string>> seen;
    while (!minHeap.isEmpty()) {
        DataSet data = minHeap.extractMin();
        if (data.getProductCode() == "    TXO     " && data.getStrikePrice() == "9900" && data.getExpirationDate() == "201705" && data.getOptionType() == "    C     ") {
            // 檢查這個 dealDate 和 dealTime 是否已經輸出過
            if (seen.find({data.getDealDate(), data.getDealTime()}) == seen.end()) {
                // 如果沒有輸出過，加到 set 和 heap
                seen.insert({data.getDealDate(), data.getDealTime()});
                priceTimeHeap.insert(data);
            }
        }
    }

    int n = 10;
    while (n-- && !priceTimeHeap.isEmpty()) {
        DataSet minData = priceTimeHeap.extractMin();
        minData.print();
    }

    /*第 5 題 b*/
    cout << "MaxHeap" << endl;
    MaxHeap maxHeap;
    buildMaxHeapFromVector(row, maxHeap);
    MaxHeap MaxPriceTimeHeap;
    Set<Pair<string, string>> seen2;
    while (!maxHeap.isEmpty()) {
        DataSet data = maxHeap.extractMax();
        if (data.getProductCode() == "    TXO     " && data.getStrikePrice() == "9900" && data.getExpirationDate() == "201705" && data.getOptionType() == "    C     ") {
            if (seen2.find({data.getDealDate(), data.getDealTime()}) == seen2.end()) {
                seen2.insert({data.getDealDate(), data.getDealTime()});
                MaxPriceTimeHeap.insert(data);
            }
        }
    }

    n = 10;
    while (n-- && !MaxPriceTimeHeap.isEmpty()) {
        DataSet maxData = MaxPriceTimeHeap.extractMax();
        maxData.print();
    }

    /**第 5 題c */
    MaxHeap maxHeap2;
    buildMaxHeapFromVector(row, maxHeap2);
    Vector<string> prices;

    while (!maxHeap2.isEmpty()) {
        DataSet data = maxHeap2.extractMax();
        if (data.getProductCode() == "    TXO     " && data.getStrikePrice() == "9900" && data.getExpirationDate() == "201705" && data.getOptionType() == "    C     ") {
            prices.push_back(data.getDealPrice());
        }
    }

    sortVector(prices);

    int size = prices.size();
    string medianPrice;
    if (size % 2 == 0) {
        medianPrice = prices.at(size / 2 - 1);
    } else {
        medianPrice = prices.at(size / 2);
    }

    cout << "Median Price for TXO_9900_201705_C: " << medianPrice << endl;


    return 0;
}
