#include "../core/gioHang.h"

#include <cassert>
#include <cmath>
#include <iostream>

using namespace std;

static Product makeProduct(
    int id,
    const string& name,
    double price,
    int stock,
    double discount
) {
    Product product;

    product.id = id;
    product.name = name;
    product.price = price;
    product.stock = stock;
    product.discount = discount;

    return product;
}

static bool isSortedByID(Product catalog[], int n) {
    for (int i = 1; i < n; ++i) {
        if (catalog[i - 1].id > catalog[i].id) {
            return false;
        }
    }

    return true;
}

static bool isSortedByPrice(Product catalog[], int n) {
    for (int i = 1; i < n; ++i) {
        if (catalog[i - 1].price > catalog[i].price) {
            return false;
        }
    }

    return true;
}

static bool isSortedByStock(Product catalog[], int n) {
    for (int i = 1; i < n; ++i) {
        if (catalog[i - 1].stock > catalog[i].stock) {
            return false;
        }
    }

    return true;
}

void testQuickSortByID() {
    Product catalog[] = {
        makeProduct(105, "Product E", 50000, 10, 0),
        makeProduct(101, "Product A", 10000, 30, 0),
        makeProduct(104, "Product D", 40000, 20, 0),
        makeProduct(102, "Product B", 20000, 50, 0),
        makeProduct(103, "Product C", 30000, 40, 0)
    };

    int n = sizeof(catalog) / sizeof(catalog[0]);

    quickSortByID(catalog, 0, n - 1);

    assert(isSortedByID(catalog, n));
    assert(catalog[0].id == 101);
    assert(catalog[1].id == 102);
    assert(catalog[2].id == 103);
    assert(catalog[3].id == 104);
    assert(catalog[4].id == 105);

    cout << "[PASS] Quick Sort theo ID\n";
}

void testHeapSortByPrice() {
    Product catalog[] = {
        makeProduct(1, "Product 1", 90000, 10, 0),
        makeProduct(2, "Product 2", 20000, 20, 0),
        makeProduct(3, "Product 3", 70000, 30, 0),
        makeProduct(4, "Product 4", 10000, 40, 0),
        makeProduct(5, "Product 5", 50000, 50, 0)
    };

    int n = sizeof(catalog) / sizeof(catalog[0]);

    heapSortByPrice(catalog, n);

    assert(isSortedByPrice(catalog, n));
    assert(fabs(catalog[0].price - 10000) < 0.001);
    assert(fabs(catalog[1].price - 20000) < 0.001);
    assert(fabs(catalog[2].price - 50000) < 0.001);
    assert(fabs(catalog[3].price - 70000) < 0.001);
    assert(fabs(catalog[4].price - 90000) < 0.001);

    cout << "[PASS] Heap Sort theo Price\n";
}

void testInsertionSortByStock() {
    Product catalog[] = {
        makeProduct(1, "Product 1", 10000, 80, 0),
        makeProduct(2, "Product 2", 20000, 10, 0),
        makeProduct(3, "Product 3", 30000, 50, 0),
        makeProduct(4, "Product 4", 40000, 20, 0),
        makeProduct(5, "Product 5", 50000, 30, 0)
    };

    int n = sizeof(catalog) / sizeof(catalog[0]);

    insertionSortByStock(catalog, n);

    assert(isSortedByStock(catalog, n));
    assert(catalog[0].stock == 10);
    assert(catalog[1].stock == 20);
    assert(catalog[2].stock == 30);
    assert(catalog[3].stock == 50);
    assert(catalog[4].stock == 80);

    cout << "[PASS] Insertion Sort theo Stock\n";
}

void testEmptyAndOneElementArray() {
    Product catalog[] = {
        makeProduct(10, "Only Product", 50000, 10, 0)
    };

    quickSortByID(catalog, 0, 0);
    heapSortByPrice(catalog, 1);
    insertionSortByStock(catalog, 1);

    assert(catalog[0].id == 10);
    assert(catalog[0].price == 50000);
    assert(catalog[0].stock == 10);

    cout << "[PASS] Mang rong/mot phan tu\n";
}

int main() {
    cout << "===== TEST CAC THUAT TOAN SAP XEP =====\n";

    testQuickSortByID();
    testHeapSortByPrice();
    testInsertionSortByStock();
    testEmptyAndOneElementArray();

    cout << "===== TAT CA TEST SORT DEU PASS =====\n";

    return 0;
}
