#include "gioHang.h"
// Hoan doi hai san pham
void swapProduct(Product& a, Product& b) {
    Product temp = a;
    a = b;
    b = temp;
}

// QUICK SORT - Sap xep tang dan theo Ma san pham (id)

int partitionByID(Product catalog[], int low, int high) {
    int pivot = catalog[high].id;
    int i = low - 1;

    for (int j = low; j < high; ++j) {
        if (catalog[j].id <= pivot) {
            ++i;
            swapProduct(catalog[i], catalog[j]);
        }
    }

    swapProduct(catalog[i + 1], catalog[high]);
    return i + 1;
}

void quickSortByID(Product catalog[], int low, int high) {
    if (catalog == nullptr || low >= high) {
        return;
    }

    int pivotIndex = partitionByID(catalog, low, high);

    quickSortByID(catalog, low, pivotIndex - 1);
    quickSortByID(catalog, pivotIndex + 1, high);
}

// HEAP SORT - Sap xep tang dan theo Don gia (price)

void heapifyByPrice(Product catalog[], int n, int root) {
    while (true) {
        int largest = root;
        int left = 2 * root + 1;
        int right = 2 * root + 2;
    
        if (left < n && catalog[left].price > catalog[largest].price) {
            largest = left;
        }
    
        if (right < n && catalog[right].price > catalog[largest].price) {
            largest = right;
        }
    
        if (largest == root) {
            return;
        }
        
        swapProduct(catalog[root], catalog[largest]);
        root = largest;
    }
}

void heapSortByPrice(Product catalog[], int n) {
    if (catalog == nullptr || n <= 1) {
        return;
    }

    // Tao max heap
    for (int i = n / 2 - 1; i >= 0; --i) {
        heapifyByPrice(catalog, n, i);
    }

    // Dua phan tu lon nhat ve cuoi mang
    for (int i = n - 1; i > 0; --i) {
        swapProduct(catalog[0], catalog[i]);
        heapifyByPrice(catalog, i, 0);
    }
}

// INSERTION SORT - Sap xep tang dan theo Ton kho (stock)

void insertionSortByStock(Product catalog[], int n) {
    if (catalog == nullptr || n <= 1) {
        return;
    }

    for (int i = 1; i < n; ++i) {
        Product key = catalog[i];
        int j = i - 1;

        while (j >= 0 && catalog[j].stock > key.stock) {
            catalog[j + 1] = catalog[j];
            --j;
        }

        catalog[j + 1] = key;
    }
}
