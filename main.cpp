#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>
#include <iomanip>
#include <windows.h>
#include "Tuple.hpp"
#include "Set.hpp"
#include "Pair.hpp"
#include "Vector.hpp"
#include "Pair3.hpp"
#include "MinHeap.hpp"
using namespace std;



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

    for (int i = 1; i < dataSets.size(); ++i) {
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
    LARGE_INTEGER cpuFreq;
	LARGE_INTEGER startTime;
	LARGE_INTEGER endTime;
	QueryPerformanceFrequency(&cpuFreq);

    for(int i=5; i<=9; i++){
        string file = "OptionsDaily_2017_05_1" + to_string(i) + ".csv";
        cout << file << endl;
        readCSV(file);
    }
    /*第 2 3 4 題*/
    string product1 = "TXO_1000_201706_P";
    string product2 = "TXO_9500_201706_C";
    string product3 = "GIO_5500_201706_C";

    QueryPerformanceCounter(&startTime);
    if (productExists("    TXO     ", "1000", "201706", "    P     ")) {
        cout << product1 << " exists in the datasets." << endl;
    } else {
        cout << product1 << " does not exist in the datasets." << endl;
    }
	QueryPerformanceCounter(&endTime);
    cout << "Search "<<product1<<" cost : "<< (((endTime.QuadPart - startTime.QuadPart) * 1000.0f) / cpuFreq.QuadPart) <<" ms\n\n";

    QueryPerformanceCounter(&startTime);
    if (productExists("    TXO     ", "9500", "201706", "    C     ")) {
        cout << product2 << " exists in the datasets." << endl;
    } else {
        cout << product2 << " does not exist in the datasets." << endl;
    }
    QueryPerformanceCounter(&endTime);
    cout << "Search "<<product1<<" cost : "<< (((endTime.QuadPart - startTime.QuadPart) * 1000.0f) / cpuFreq.QuadPart) <<" ms\n\n";

    QueryPerformanceCounter(&startTime);
    if (productExists("    GIO     ", "5500", "201706", "    C     ")) {
        cout << product3 << " exists in the datasets." << endl;
    } else {
        cout << product3 << " does not exist in the datasets." << endl;
    }
    QueryPerformanceCounter(&endTime);
    cout << "Search "<<product1<<" cost : "<< (((endTime.QuadPart - startTime.QuadPart) * 1000.0f) / cpuFreq.QuadPart) <<" ms\n\n";
    /*************************************************************** */
    
    /*第 5 題 a*/
    QueryPerformanceCounter(&startTime);
    MinHeap minHeap;
    buildMinHeapFromVector(row, minHeap);
    Vector<DataSet> sortedData = heapSort(minHeap);
    QueryPerformanceCounter(&endTime);
    cout << "Build minHeap Tree cost : "<< (((endTime.QuadPart - startTime.QuadPart) * 1000.0f) / cpuFreq.QuadPart) <<" ms\n\n";

    QueryPerformanceCounter(&startTime);
    cout << "10 samllest prices for TXO_9900_201705_C" << endl;
    for (int i = 0; i < 10; i++) {
        sortedData.at(i).print();
    }
    QueryPerformanceCounter(&endTime);
    cout << "cost : "<< (((endTime.QuadPart - startTime.QuadPart) * 1000.0f) / cpuFreq.QuadPart) <<" ms\n\n";

    /*第 5 題 b*/
    QueryPerformanceCounter(&startTime);
    cout << "10 largest prices for TXO_9900_201705_C" << endl;
    for (int i = sortedData.size() - 1; i >= sortedData.size() - 10; i--) {
        sortedData.at(i).print();
    }
    QueryPerformanceCounter(&endTime);
    cout << "cost : "<< (((endTime.QuadPart - startTime.QuadPart) * 1000.0f) / cpuFreq.QuadPart) <<" ms\n\n";

    /*第 5 題 c*/
    QueryPerformanceCounter(&startTime);
    cout << "meidan price for TXO_9900_201705_C: ";
    string medianPrice;
    if (sortedData.size() % 2 == 0) {
        medianPrice = sortedData.at(sortedData.size() / 2).getDealPrice();
    } else {
        medianPrice = sortedData.at(sortedData.size() / 2).getDealPrice();
    }
    cout << medianPrice << endl;
    QueryPerformanceCounter(&endTime);
    cout << "cost : "<< (((endTime.QuadPart - startTime.QuadPart) * 1000.0f) / cpuFreq.QuadPart) <<" ms\n\n";

    /*第 5 題 d*/
    QueryPerformanceCounter(&startTime);
    filterDataSets("    TXO     ", "9900", "201705", "    C     ");
    computeTick(dataSetForTick);
    QueryPerformanceCounter(&endTime);
    cout << "cost : "<< (((endTime.QuadPart - startTime.QuadPart) * 1000.0f) / cpuFreq.QuadPart) <<" ms\n\n";

    return 0;
}