#include "gioHang.h"

void initSampleCSV(const string& filename) {
    ifstream checkFile(filename);
    if (checkFile.good()) return;
    checkFile.close();

    ofstream outFile(filename);
    outFile << "ProductID,Name,Price,Stock,Discount\n";
    outFile << "105,Ban phim co AKKO,1250000,15,50000\n";
    outFile << "102,Chuot Logitech G102,400000,30,20000\n";
    outFile << "108,Tai nghe Sony WH,3500000,8,200000\n";
    outFile << "101,Op lung POCO X8 Pro,150000,50,10000\n";
    outFile << "104,Gia do Laptop Nhom,250000,25,0\n";
    outFile << "107,Sac du phong Anker,850000,12,50000\n";
    outFile << "103,Lot chuot Gaming XL,120000,40,0\n";
    outFile << "106,Hub USB-C Baseus,550000,20,30000\n";
    outFile.close();
}

int loadCatalogFromCSV(const string& filename, Product catalog[], int maxCap, AVLTree& avlIndex) {
    ifstream file(filename);
    if (!file.is_open()) return 0;

    string line;
    getline(file, line); // Bo qua dong tieu de (Header)
    int count = 0;

    while (getline(file, line) && count < maxCap) {
        if (line.empty()) continue;
        stringstream ss(line);
        string idStr, name, priceStr, stockStr, discStr;

        getline(ss, idStr, ',');
        getline(ss, name, ',');
        getline(ss, priceStr, ',');
        getline(ss, stockStr, ',');
        getline(ss, discStr, ',');

        Product p;
        p.id = stoi(idStr);
        p.name = name;
        p.price = stod(priceStr);
        p.stock = stoi(stockStr);
        p.discount = stod(discStr);

        catalog[count++] = p;
        avlIndex.root = addAVLNode(avlIndex.root, p);
    }
    file.close();
    return count;
}

bool saveCatalogToCSV(const string& filename, Product catalog[], int n) {
    ofstream outFile(filename);
    if (!outFile.is_open()) return false;

    outFile << "ProductID,Name,Price,Stock,Discount\n";
    for (int i = 0; i < n; i++) {
        outFile << catalog[i].id << ","
                << catalog[i].name << ","
                << fixed << setprecision(0) << catalog[i].price << ","
                << catalog[i].stock << ","
                << fixed << setprecision(0) << catalog[i].discount << "\n";
    }
    outFile.close();
    return true;
}

void printCatalog(Product catalog[], int n, int previewLimit) {
    cout << "\n+-------+----------------------------------+-------------+---------+-------------+\n";
    cout << "| Ma SP | Ten San Pham                     | Don Gia (d) | Ton Kho | Giam Gia(d) |\n";
    cout << "+-------+----------------------------------+-------------+---------+-------------+\n";

    int limit = (n > previewLimit) ? previewLimit : n;
    for (int i = 0; i < limit; i++) {
        cout << "| " << setw(5) << catalog[i].id << " | "
             << left << setw(32) << catalog[i].name << right << " | "
             << setw(11) << fixed << setprecision(0) << catalog[i].price << " | "
             << setw(7) << catalog[i].stock << " | "
             << setw(11) << catalog[i].discount << " |\n";
    }

    if (n > previewLimit) {
        cout << "|  ...  | ... (An " << setw(4) << (n - previewLimit - 5)
             << " dong o giua) ...        |     ...     |   ...   |     ...     |\n";
        for (int i = n - 5; i < n; i++) {
            cout << "| " << setw(5) << catalog[i].id << " | "
                 << left << setw(32) << catalog[i].name << right << " | "
                 << setw(11) << fixed << setprecision(0) << catalog[i].price << " | "
                 << setw(7) << catalog[i].stock << " | "
                 << setw(11) << catalog[i].discount << " |\n";
        }
    }

    cout << "+-------+----------------------------------+-------------+---------+-------------+\n";
    cout << "Tong so san pham trong danh sach: " << n << " ban ghi.\n";
}

int findProductIndexByID(Product catalog[], int n, int productId) {
    for (int i = 0; i < n; i++) {
        if (catalog[i].id == productId) return i;
    }
    return -1;
}
