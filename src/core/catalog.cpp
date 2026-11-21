#include "gioHang.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <string>
#include <stdexcept>

using namespace std;

string trim(const string& str) {
    size_t first = str.find_first_not_of(" \t\r\n");
    if (string::npos == first) return "";
    size_t last = str.find_last_not_of(" \t\r\n");
    return str.substr(first, (last - first + 1));
}

int findProductIndexByID(Product catalog[], int n, int id) {
    if (n <= 0) return -1;
    for (int i = 0; i < n; ++i) {
        if (catalog[i].id == id) {
            return i;
        }
    }
    return -1;
}

void initSampleCSV(const string& filename) {
    ifstream fileCheck(filename);
    if (fileCheck.good()) {
        fileCheck.close();
        return; 
    }
    fileCheck.close();
    
    ofstream file(filename);
    if (!file) {
        cout << "Khong the tao file CSV mau!\n";
        return;
    }
    file << "ProductID,Name,Price,Stock,Discount\n";
    file << "101,Ban Phim Co,1500000,50,100000\n";
    file << "102,Chuot Khong Day,500000,100,50000\n";
    file << "103,Man Hinh 24 Inch,3500000,30,200000\n";
    file << "104,Tai Nghe Bluetooth,800000,0,0\n"; // San pham co ton kho = 0 de test
    file << "105,Lot Chuot,150000,200,10000\n";
    file.close();
}

int loadCatalogFromCSV(const string& filename, Product catalog[], int& n, AVLNode*& root) {
    ifstream file(filename);
    if (!file) {
        cout << "Loi: Khong doc duoc file " << filename << ". Vui long thoat va kiem tra lai.\n";
        return 0;
    }

    string line;
    // Bo qua dong tieu de
    if (!getline(file, line)) {
        return 0;
    }

    n = 0;
    int errorCount = 0;
    int duplicateCount = 0;

    while (getline(file, line)) {
        if (n >= MAX_CATALOG) {
            cout << "Canh bao: Da dat gioi han " << MAX_CATALOG << " san pham. Ngung doc them.\n";
            break;
        }

        line = trim(line);
        if (line.empty()) continue;

        stringstream ss(line);
        string idStr, nameStr, priceStr, stockStr, discountStr;

        getline(ss, idStr, ',');
        getline(ss, nameStr, ',');
        getline(ss, priceStr, ',');
        getline(ss, stockStr, ',');
        getline(ss, discountStr, ',');

        discountStr = trim(discountStr); 

        try {
            int id = stoi(idStr);
            double price = stod(priceStr);
            int stock = stoi(stockStr);
            double discount = stod(discountStr);

            if (findProductIndexByID(catalog, n, id) != -1) {
                duplicateCount++;
                cout << "Canh bao: Trung ID " << id << ". Bo qua dong nay de tranh lech du lieu voi AVL.\n";
                continue;
            }

            catalog[n].id = id;
            catalog[n].name = nameStr;
            catalog[n].price = price;
            catalog[n].stock = stock;
            catalog[n].discount = discount;
            
            root = addAVLNode(root, catalog[n]);
            
            n++;
        } catch (const exception& e) {
            errorCount++; // Neu loi chuyen doi so (chuoi rong, ky tu la) thi bo qua dong nay
        }
    }
    file.close();
    
    if (errorCount > 0) cout << "Canh bao: Bo qua " << errorCount << " dong bi loi format (khong phai so).\n";
    return n;
}

bool saveCatalogToCSV(const string& filename, Product catalog[], int n) {
    ofstream file(filename);
    if (!file) {
        cout << "Loi: Ghi file that bai tai " << filename << "\n";
        return false;
    }

    file << "ProductID,Name,Price,Stock,Discount\n";
    for (int i = 0; i < n; ++i) {
        file << catalog[i].id << ","
             << catalog[i].name << ","
             << fixed << setprecision(0) << catalog[i].price << ","
             << catalog[i].stock << ","
             << catalog[i].discount << "\n";
    }
    file.close();
    return true;
}

void printCatalog(Product catalog[], int n) {
    if (n == 0) {
        cout << "Kho hang hien dang trong!\n";
        return;
    }

    cout << string(85, '-') << "\n";
    cout << left << setw(10) << "ID"
         << setw(35) << "Ten San Pham"
         << right << setw(15) << "Gia (VND)"
         << setw(10) << "Ton Kho"
         << setw(15) << "Giam Gia\n";
    cout << string(85, '-') << "\n";

    int previewLimit = 15;
    
    if (n <= previewLimit + 5) {
        for (int i = 0; i < n; ++i) {
            string name = catalog[i].name;
            if (name.length() > 32) name = name.substr(0, 29) + "...";
            
            cout << left << setw(10) << catalog[i].id
                 << setw(35) << name
                 << right << fixed << setprecision(0) << setw(15) << catalog[i].price
                 << setw(10) << catalog[i].stock
                 << setw(15) << catalog[i].discount << "\n";
        }
    } else {
        for (int i = 0; i < previewLimit; ++i) {
            string name = catalog[i].name;
            if (name.length() > 32) name = name.substr(0, 29) + "...";
            
            cout << left << setw(10) << catalog[i].id
                 << setw(35) << name
                 << right << fixed << setprecision(0) << setw(15) << catalog[i].price
                 << setw(10) << catalog[i].stock
                 << setw(15) << catalog[i].discount << "\n";
        }
        
        cout << left << setw(10) << "..." << setw(35) << "..." << right << setw(15) << "..." << setw(10) << "..." << setw(15) << "...\n";
      
        for (int i = n - 5; i < n; ++i) {
            string name = catalog[i].name;
            if (name.length() > 32) name = name.substr(0, 29) + "...";
            
            cout << left << setw(10) << catalog[i].id
                 << setw(35) << name
                 << right << fixed << setprecision(0) << setw(15) << catalog[i].price
                 << setw(10) << catalog[i].stock
                 << setw(15) << catalog[i].discount << "\n";
        }
    }
    cout << string(85, '-') << "\n";
    cout << "Tong so san pham: " << n << "\n";
}
