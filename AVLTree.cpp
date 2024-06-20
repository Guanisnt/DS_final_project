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
// #include "Pair3.hpp"
#include "AVLTree.hpp"
using namespace std;

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

Set<Pair<string, double>> seen;
void bulidDataTree(const string& filename, AVLTree<DataSet>& Tree) {
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
                // 如果沒有輸出過，加到 set 和 tree
                seen.insert({dealTime, dealPrice});
                Tree.insert(data);
            }
        }
    }
    file.close();
}

void BuildAVLTree(AVLTree<Tuple<string>>& Tree){
    int size=0;
    for(int i=0; i < row.size(); i+=8){
        if(i + 7 < row.size()){
            Tuple<string> item(row.get(i+1),row.get(i+2),row.get(i+3),row.get(i+4));
            if(!Tree.search(item)){
                size++;
                Tree.insert(item);
            }
        }
    }
    cout << "Unique data count: " << size << endl;
}

int main() {
    for(int i=5; i<=9; i++){
        string file = "OptionsDaily_2017_05_1" + to_string(i) + ".csv";
        cout << file << endl;
        readCSV(file);
    }
    AVLTree<Tuple<string>> uniqueData;
    BuildAVLTree(uniqueData);

    //uniqueData.preOrder();
    // /*第 2 3 4 題*/
    string product1 = "TXO_1000_201706_P";
    string product2 = "TXO_9500_201706_C";
    string product3 = "GIO_5500_201706_C";

    if(uniqueData.search({"    TXO     ","1000","201706","    P     "}))
        cout << product1 << " exists\n";
    else   
        cout << product1 << " not exists\n";

    if(uniqueData.search({"    TXO     ","9500","201706","    C     "}))
        cout << product2 << " exists\n";
    else   
        cout << product2 << " not exists\n";

    if(uniqueData.search({"    GIO     ","5500","201706","    C     "}))
        cout << product3 << " exists\n";
    else   
        cout << product3 << " not exists\n";
        
    AVLTree<DataSet> Tree;
    for(int i=5; i<=5; i++){
        string file = "OptionsDaily_2017_05_1" + to_string(i) + ".csv";
        cout << file << endl;
        bulidDataTree(file, Tree);
    }
    Tree.printTopAndBottom10(Tree.DataRoot);
    return 0;
}