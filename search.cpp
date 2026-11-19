
#include "gioHangdemo.h"
#include <cctype>

// 1. Binary Search theo ma san pham
// Mang a phai duoc sap xep tang dan theo id
int binarySearchByID(Product a[], int n, int id) {
    int left = 0;
    int right = n - 1;

    while (left <= right) {
        int mid = (left + right) / 2;

        if (a[mid].id == id) {
            return mid;
        }
        else if (a[mid].id < id) {
            left = mid + 1;
        }
        else {
            right = mid - 1;
        }
    }

    return -1;
}

// 2. Sentinel Search theo ma san pham
int sentinelSearchByID(Product a[], int n, int id) {
    if (n <= 0 || n >= MAX_CATALOG) {
        return -1;
    }

    int oldID = a[n].id;
    a[n].id = id;

    int i = 0;
    while (a[i].id != id) {
        i++;
    }

    a[n].id = oldID;

    if (i == n) {
        return -1;
    }

    return i;
}

// Ham chuyen chuoi thanh chu thuong
string toLowerCase(string s) {
    for (int i = 0; i < (int)s.length(); i++) {
        s[i] = (char)tolower((unsigned char)s[i]);
    }
    return s;
}

// 3. Sequential Search theo ten san pham
// Tim ten co chua tu khoa, khong phan biet chu hoa va chu thuong
int sequentialSearchByName(
    Product a[],
    int n,
    const string& keyword,
    int result[],
    int maxResult
) {
    if (n <= 0 || maxResult <= 0) {
        return 0;
    }

    string key = toLowerCase(keyword);
    int count = 0;

    for (int i = 0; i < n; i++) {
        string name = toLowerCase(a[i].name);

        if (name.find(key) != string::npos) {
            if (count < maxResult) {
                result[count] = i;
                count++;
            }
        }
    }

    return count;
}
