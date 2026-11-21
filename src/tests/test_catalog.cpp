#include "../core/gioHang.h"
#include <iostream>
#include <cassert>

using namespace std;

// Khai bao tam cac ham de test (tranh loi bien dich neu header chua update)
void initSampleCSV(const string& filename);
int loadCatalogFromCSV(const string& filename, Product catalog[], int& n, AVLNode*& root);
bool saveCatalogToCSV(const string& filename, Product catalog[], int n);
void printCatalog(Product catalog[], int n);

int main() {
    cout << "===== TEST TINH NANG CATALOG CUA TV2 =====\n";
    
    Product* catalog = new Product[MAX_CATALOG];
    int n = 0;
    AVLNode* root = nullptr;

    // 1. Test tao file mau
    string testFile = "test_products_small.csv";
    initSampleCSV(testFile);
    cout << "[PASS] Tao file CSV mau thanh cong.\n";

    // 2. Test doc file vao mang
    int loaded = loadCatalogFromCSV(testFile, catalog, n, root);
    assert(loaded > 0 && n > 0);
    cout << "[PASS] Doc thanh cong " << n << " san pham tu file.\n";

    // 3. Test in danh sach
    cout << "\n--- Ban xem truoc bang in danh sach ---\n";
    printCatalog(catalog, n);
    cout << "---------------------------------------\n";
    cout << "[PASS] In danh sach san pham hien thi on dinh.\n";

    // 4. Test luu file moi
    string saveFile = "test_products_saved.csv";
    bool saved = saveCatalogToCSV(saveFile, catalog, n);
    assert(saved == true);
    cout << "[PASS] Ghi mang du lieu ra file CSV moi thanh cong.\n";

    delete[] catalog; // Don dep bo nho
    cout << "\n===== HOAN THANH TOAN BO TEST TV2 =====\n";
    return 0;
}
