#include "gioHang.h"

void initHashTable(VoucherHashTable& ht) {
    for (int i = 0; i < HASH_SIZE; i++) {
        ht.buckets[i] = nullptr;
    }
}

int hashFunctionDivision(int key) {
    return key % HASH_SIZE;
}

void insertVoucher(VoucherHashTable& ht, int key, string codeName, double value) {
    int index = hashFunctionDivision(key);
    VoucherNode* newNode = new VoucherNode{key, codeName, value, ht.buckets[index]};
    ht.buckets[index] = newNode;
}

VoucherNode* searchVoucher(VoucherHashTable& ht, int key) {
    int index = hashFunctionDivision(key);
    VoucherNode* cur = ht.buckets[index];
    while (cur != nullptr) {
        if (cur->voucherKey == key) {
            return cur;
        }
        cur = cur->next;
    }
    return nullptr;
}

void displayVouchers(VoucherHashTable& ht) {
    cout << "\n--- BANG BAM MA KHUYEN MAI (SEPARATE CHAINING, m = " << HASH_SIZE << ") ---\n";
    for (int i = 0; i < HASH_SIZE; i++) {
        if (ht.buckets[i] != nullptr) {
            cout << "Bucket [" << setw(2) << i << "]: ";
            VoucherNode* cur = ht.buckets[i];
            while (cur != nullptr) {
                cout << "[Ma: " << cur->voucherKey << " (" << cur->codeName
                     << " - Giam " << fixed << setprecision(0) << cur->discountValue << "d)] -> ";
                cur = cur->next;
            }
            cout << "NULL\n";
        }
    }
}

void destroyHashTable(VoucherHashTable& ht) {
    for (int i = 0; i < HASH_SIZE; i++) {
        VoucherNode* cur = ht.buckets[i];
        while (cur != nullptr) {
            VoucherNode* tmp = cur;
            cur = cur->next;
            delete tmp;
        }
        ht.buckets[i] = nullptr;
    }
}