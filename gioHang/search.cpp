#include "gioHang.h"
#include <cctype>

// ============================================================================
// HAM HO TRO NOI BO (CHUAN HOA CHUOI KHONG PHAN BIET HOA THUONG)
// ============================================================================

static string toLowerStr(const string& s) {
    string res;
    res.reserve(s.size());
    for (size_t i = 0; i < s.size(); ++i) {
        res.push_back(static_cast<char>(tolower(static_cast<unsigned char>(s[i]))));
    }
    return res;
}

static string trimAndLower(const string& s) {
    size_t start = 0;
    while (start < s.size() && isspace(static_cast<unsigned char>(s[start]))) {
        ++start;
    }
    size_t end = s.size();
    while (end > start && isspace(static_cast<unsigned char>(s[end - 1]))) {
        --end;
    }

    string res;
    res.reserve(end - start);
    for (size_t i = start; i < end; ++i) {
        res.push_back(static_cast<char>(tolower(static_cast<unsigned char>(s[i]))));
    }
    return res;
}

// ============================================================================
// 1. BINARY SEARCH THEO ID
// Yeu cau tien quyet: Mang a[0..n-1] PHAI duoc sap xep tang dan theo Product::id.
// Do phuc tap thoi gian: O(log n) | Khong gian phu: O(1)
// ============================================================================

int binarySearchByID(Product a[], int n, int id) {
    if (a == nullptr || n <= 0) {
        return -1;
    }

    int low = 0;
    int high = n - 1;

    while (low <= high) {
        int mid = low + (high - low) / 2; // Tranh tran so nguyen (low + high)
        if (a[mid].id == id) {
            return mid;
        } else if (a[mid].id < id) {
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }

    return -1;
}

// ============================================================================
// 2. SENTINEL SEARCH THEO ID (TIM KIEM LINH CANH)
// Khong yeu cau mang da sap xep. Dat linh canh tai a[n] khi n < MAX_CATALOG
// va khoi phuc lai gia tri cu cua o linh canh sau khi tim xong.
// Do phuc tap thoi gian: O(n) | Khong gian phu: O(1)
// ============================================================================

int sentinelSearchByID(Product a[], int n, int id) {
    if (a == nullptr || n <= 0) {
        return -1;
    }

    // Truong hop chuan: mang con o du a[n] (n < MAX_CATALOG)
    if (n < MAX_CATALOG) {
        Product backup = a[n];
        a[n].id = id;

        int i = 0;
        while (a[i].id != id) {
            ++i;
        }

        a[n] = backup; // Tra lai gia tri cu cho o a[n]
        return (i < n) ? i : -1;
    }

    // Truong hop mang day (n == MAX_CATALOG): dung o cuoi a[n - 1] lam linh canh de khong tran mang
    if (a[n - 1].id == id) {
        return n - 1;
    }

    Product lastBackup = a[n - 1];
    a[n - 1].id = id;

    int i = 0;
    while (a[i].id != id) {
        ++i;
    }

    a[n - 1] = lastBackup;
    return (i < n - 1) ? i : -1;
}

// ============================================================================
// 3. SEQUENTIAL SEARCH THEO TEN (KHOP CHUOI CON, KHONG PHAN BIET HOA THUONG)
// Ghi toi da maxResult chi so vao mang result[], tra ve tong so san pham khop.
// Tu khoa rong hoac chi toan dau cach tra ve 0.
// Do phuc tap thoi gian: O(n * m) voi m la do dai ten san pham | Khong gian: O(m)
// ============================================================================

int sequentialSearchByName(Product a[], int n, const string& keyword, int result[], int maxResult) {
    if (a == nullptr || n <= 0 || result == nullptr || maxResult <= 0) {
        return 0;
    }

    string cleanKey = trimAndLower(keyword);
    if (cleanKey.empty()) {
        return 0;
    }

    int totalMatches = 0;
    for (int i = 0; i < n; ++i) {
        string lowerName = toLowerStr(a[i].name);
        if (lowerName.find(cleanKey) != string::npos) {
            if (totalMatches < maxResult) {
                result[totalMatches] = i;
            }
            ++totalMatches;
        }
    }

    return totalMatches;
}