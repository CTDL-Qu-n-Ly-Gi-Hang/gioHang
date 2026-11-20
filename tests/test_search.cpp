
#include "../gioHangdemo.h"
#include <iostream>
#include <cassert>

using namespace std;

int main() {
    cout << "===== KIEM THU TV4 =====\n";

    // Tao danh sach san pham da sap xep theo ID
    Product a[MAX_CATALOG + 1] = {
        {101, "Op lung POCO", 50000, 50, 0},
        {102, "Tai nghe Bluetooth", 150000, 20, 0},
        {103, "Dien thoai POCO X8", 15000000, 10, 0},
        {104, "Sac du phong", 250000, 15, 0}
    };

    int n = 4;

    // 1. Kiem thu Binary Search
    cout << "\n1. Binary Search:\n";
    assert(binarySearchByID(a, n, 103) == 2);
    assert(binarySearchByID(a, n, 999) == -1);
    cout << "Binary Search: PASS\n";

    // 2. Kiem thu Sentinel Search
    cout << "\n2. Sentinel Search:\n";
    assert(sentinelSearchByID(a, n, 101) == 0);
    assert(sentinelSearchByID(a, n, 104) == 3);
    assert(sentinelSearchByID(a, n, 999) == -1);
    cout << "Sentinel Search: PASS\n";

    // 3. Kiem thu Sequential Search
    cout << "\n3. Sequential Search:\n";
    int result[MAX_CATALOG];

    int count = sequentialSearchByName(
        a, n, "POCO", result, MAX_CATALOG
    );

    assert(count == 2);
    assert(result[0] == 0);
    assert(result[1] == 2);
    cout << "Sequential Search: PASS\n";

    // 4. Kiem thu cay AVL
    cout << "\n4. AVL Tree:\n";

    AVLTree tree;
    initAVL(tree);

    tree.root = addAVLNode(tree.root, a[0]);
    tree.root = addAVLNode(tree.root, a[1]);
    tree.root = addAVLNode(tree.root, a[2]);

    assert(searchAVL(tree.root, 101) != nullptr);
    assert(searchAVL(tree.root, 999) == nullptr);

    // Kiem tra cay tu can bang
    assert(tree.root->data.id == 102);

    cout << "AVL Search: PASS\n";
    cout << "AVL Balance: PASS\n";

    // Giai phong bo nho
    destroyAVLNode(tree.root);
    tree.root = nullptr;

    cout << "\n===== TAT CA KIEM THU THANH CONG =====\n";

    return 0;
}
