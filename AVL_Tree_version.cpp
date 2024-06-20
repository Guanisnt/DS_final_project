#include "AVLTree.hpp"
#include "Vector.hpp"
#include "Pair3.hpp"
#include "Pair.hpp"
#include "Set"
#include <iostream>
#include <sstream>
#include <string>
#include <fstream>
#include <algorithm>
using namespace std;

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

        if (productCode == "    TXO     " && strikePrice == 9900.0 && expirationDate == "201705" && optionType == "    C     ") {
            DataSet data(dealDate, productCode, strikePrice, expirationDate, optionType, dealTime, dealPrice, dealVolume);
            if (seen.find({dealTime, dealPrice}) == seen.end()) {
                seen.insert({dealTime, dealPrice});
                root = insert(root, data);
            }
        }
    }
    file.close();
}

void computeTick() {
    double maxReturn = -1.0;
    double minReturn = 1.0;

    for (int i = 1; i < seen.size(); i++) {
        double returnRate = (seen.get(i).second - seen.get(i - 1).second) / seen.get(i - 1).second;
        maxReturn = max(maxReturn, returnRate);
        minReturn = min(minReturn, returnRate);
    }

    cout << "Max return rate: " << maxReturn << endl;
    cout << "Min return rate: " << minReturn << endl;
}

void printTopAndBottom10(AVLTreeNode* root) {
    Vector<DataSet> vec;
    inOrder(root, vec);
    int n = vec.size();

    cout << "10 smallest prices for TXO_9900_201705_C:" << endl;
    for (int i = 0; i < 10 && i < n; i++) {
        vec.at(i).print();
    }
    cout << endl;

    cout << "10 largest prices for TXO_9900_201705_C:" << endl;
    for (int i = n - 1; i >= n - 10 && i >= 0; i--) {
        vec.at(i).print();
    }
    cout << endl;
}

void findMedian(AVLTreeNode* root) {
    Vector<DataSet> vec;
    inOrder(root, vec);
    int n = vec.size();
    if (n % 2 == 0) {
        cout << "Median price for TXO_9900_201705_C: " << (vec.at(n / 2).getDealPrice() + vec.at(n / 2 - 1).getDealPrice()) / 2 << endl;
    } else {
        cout << "Median price for TXO_9900_201705_C: " << vec.at(n / 2).getDealPrice() << endl;
    }
}

int main() {
    AVLTreeNode *root = nullptr;
    for (int i = 5; i <= 9; i++) {
        string file = "OptionsDaily_2017_05_1" + to_string(i) + ".csv";
        cout << file << endl;
        readCSV(file, root);
    }
    string product1 = "TXO_1000_201706_P";
    string product2 = "TXO_9500_201706_C";
    string product3 = "GIO_5500_201706_C";

    // DataSet key1("    TXO     ", 1000, "201706", "    P     ");
    // DataSet key2("    TXO     ", 9500, "201706", "    C     ");
    // DataSet key3("    GIO     ", 5500, "201706", "    C     ");

    // AVLTreeNode* result1 = search(root, key1);
    // AVLTreeNode* result2 = search(root, key2);
    // AVLTreeNode* result3 = search(root, key3);

    // if (result1)
    //     cout << key1.getProductCode() << " 存在於樹中。" << endl;
    // else
    //     cout << key1.getProductCode() << " 不存在於樹中。" << endl;

    // if (result2)
    //     cout << key2.getProductCode() << " 存在於樹中。" << endl;
    // else
    //     cout << key2.getProductCode() << " 不存在於樹中。" << endl;

    // if (result3)
    //     cout << key3.getProductCode() << " 存在於樹中。" << endl;
    // else
    //     cout << key3.getProductCode() << " 不存在於樹中。" << endl;

    printTopAndBottom10(root);
    findMedian(root);
    computeTick();
    return 0;
}
