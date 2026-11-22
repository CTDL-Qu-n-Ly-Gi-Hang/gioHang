#include "gioHang.h"
#include <cassert>
#include <cmath>

// ============================================================================
// HAM HO TRO KIEM THU (TEST HELPERS)
// ============================================================================

static int countCartNodes(const CartLinkedList& cart) {
    int count = 0;
    CartNode* cur = cart.head;
    while (cur != nullptr) {
        count++;
        cur = cur->next;
    }
    return count;
}

// Kiem tra toan ven lien ket 2 chieu cua DSLK doi (head, tail, prev, next)
static void verifyDoublyLinkedList(const CartLinkedList& cart) {
    if (cart.head == nullptr) {
        assert(cart.tail == nullptr);
        return;
    }
    assert(cart.head->prev == nullptr);
    assert(cart.tail != nullptr);
    assert(cart.tail->next == nullptr);

    int forwardCount = 0;
    CartNode* cur = cart.head;
    CartNode* last = nullptr;
    while (cur != nullptr) {
        if (cur->next != nullptr) {
            assert(cur->next->prev == cur);
        }
        last = cur;
        cur = cur->next;
        forwardCount++;
    }
    assert(last == cart.tail);

    int backwardCount = 0;
    cur = cart.tail;
    CartNode* first = nullptr;
    while (cur != nullptr) {
        if (cur->prev != nullptr) {
            assert(cur->prev->next == cur);
        }
        first = cur;
        cur = cur->prev;
        backwardCount++;
    }
    assert(first == cart.head);
    assert(forwardCount == backwardCount);
}

static bool isCartSortedBySubtotal(const CartLinkedList& cart) {
    if (cart.head == nullptr || cart.head->next == nullptr) return true;
    CartNode* cur = cart.head;
    while (cur->next != nullptr) {
        if (cur->data.subtotal() > cur->next->data.subtotal()) {
            return false;
        }
        cur = cur->next;
    }
    return true;
}

static Product makeProduct(int id, const string& name, double price, int stock, double discount) {
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
        if (catalog[i - 1].id > catalog[i].id) return false;
    }
    return true;
}

static bool isSortedByPrice(Product catalog[], int n) {
    for (int i = 1; i < n; ++i) {
        if (catalog[i - 1].price > catalog[i].price) return false;
    }
    return true;
}

static bool isSortedByStock(Product catalog[], int n) {
    for (int i = 1; i < n; ++i) {
        if (catalog[i - 1].stock > catalog[i].stock) return false;
    }
    return true;
}

// ============================================================================
// BO KIEM THU 1: GIO HANG DSLK DOI & MERGE SORT
// ============================================================================

void runCartTests() {
    cout << "=== BAT DAU KIEM THU MODULE GIO HANG (DSLK DOI) & MERGE SORT ===\n\n";

    CartLinkedList cart;
    initCart(cart);
    verifyDoublyLinkedList(cart);

    cout << "[TEST 1] Gio hang rong (xoa, tim, sap xep, in)...\n";
    viewCart(cart, 0);
    sortCartBySubtotal(cart);
    verifyDoublyLinkedList(cart);
    assert(findCartNode(cart, 101) == nullptr);
    CartItem dummy;
    assert(removeProductFromCart(cart, 101, dummy) == false);
    cout << "-> PASS Test 1!\n\n";

    cout << "[TEST 2] Them 1 phan tu & sap xep 1 phan tu...\n";
    insertTailCart(cart, {101, "Op lung POCO X8 Pro", 140000, 1});
    sortCartBySubtotal(cart);
    verifyDoublyLinkedList(cart);
    assert(countCartNodes(cart) == 1);
    assert(cart.head->data.productId == 101 && cart.tail->data.productId == 101);
    cout << "-> PASS Test 2!\n\n";

    cout << "[TEST 3] Them 5 mon (co cap trung thanh tien) & Merge Sort DSLK Doi...\n";
    insertTailCart(cart, {105, "Ban phim co AKKO", 1200000, 1});
    insertTailCart(cart, {102, "Chuot Logitech G102", 380000, 2});
    insertTailCart(cart, {103, "Lot chuot Gaming XL", 140000, 1});
    insertTailCart(cart, {104, "Gia do Laptop Nhom", 250000, 2});
    verifyDoublyLinkedList(cart);

    assert(countCartNodes(cart) == 5);
    sortCartBySubtotal(cart);
    verifyDoublyLinkedList(cart);

    assert(countCartNodes(cart) == 5);
    assert(isCartSortedBySubtotal(cart) == true);
    assert(cart.head->data.productId == 101);
    assert(cart.head->next->data.productId == 103);
    assert(cart.head->next->next->data.productId == 104);
    assert(cart.head->next->next->next->data.productId == 102);
    assert(cart.tail->data.productId == 105);
    cout << "-> PASS Test 3 (Da xac minh thu tu, tinh on dinh Stable & lien ket 2 chieu)!\n\n";

    cout << "[TEST 4] Kiem tra hien thi viewCart voi VAT 8% va Voucher...\n";
    viewCart(cart, 50000);
    viewCart(cart, 5000000);
    cout << "-> PASS Test 4!\n\n";

    cout << "[TEST 5] Tim kiem & cong don so luong san pham da co...\n";
    CartNode* found = findCartNode(cart, 104);
    assert(found != nullptr);
    found->data.quantity += 3;
    assert(findCartNode(cart, 104)->data.subtotal() == 1250000);
    assert(findCartNode(cart, 999) == nullptr);
    cout << "-> PASS Test 5!\n\n";

    cout << "[TEST 6] Xoa nut dau (101), giua (102), cuoi (105), va ID ao (999)...\n";
    assert(removeProductFromCart(cart, 999, dummy) == false);
    assert(countCartNodes(cart) == 5);

    assert(removeProductFromCart(cart, 101, dummy) == true && dummy.productId == 101);
    verifyDoublyLinkedList(cart);
    assert(removeProductFromCart(cart, 102, dummy) == true && dummy.productId == 102);
    verifyDoublyLinkedList(cart);
    assert(removeProductFromCart(cart, 105, dummy) == true && dummy.productId == 105);
    verifyDoublyLinkedList(cart);
    assert(countCartNodes(cart) == 2);
    cout << "-> PASS Test 6!\n\n";

    cout << "[TEST 7] Chan duoi don gia am khi discount > price...\n";
    insertTailCart(cart, {108, "San pham loi gia", -50000, 2});
    verifyDoublyLinkedList(cart);
    CartNode* badNode = findCartNode(cart, 108);
    assert(badNode != nullptr && badNode->data.unitPrice == 0 && badNode->data.subtotal() == 0);
    cout << "-> PASS Test 7!\n\n";

    cout << "[TEST 8] Xoa toan bo gio hang (clearCart 2 lan lien tiep)...\n";
    clearCart(cart);
    verifyDoublyLinkedList(cart);
    clearCart(cart);
    verifyDoublyLinkedList(cart);
    cout << "-> PASS Test 8!\n\n";
}

// ============================================================================
// BO KIEM THU 2: CAC THUAT TOAN SAP XEP KHO HANG
// ============================================================================

void runSortTests() {
    cout << "=== BAT DAU KIEM THU CAC THUAT TOAN SAP XEP KHO HANG ===\n\n";

    // 1. Quick Sort theo ID
    {
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
        assert(catalog[0].id == 101 && catalog[4].id == 105);
        cout << "[PASS] Quick Sort theo ID\n";
    }

    // 2. Heap Sort theo Price
    {
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
        assert(fabs(catalog[4].price - 90000) < 0.001);
        cout << "[PASS] Heap Sort theo Price\n";
    }

    // 3. Insertion Sort theo Stock
    {
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
        assert(catalog[0].stock == 10 && catalog[4].stock == 80);
        cout << "[PASS] Insertion Sort theo Stock\n";
    }

    // 4. Mang rong / 1 phan tu
    {
        Product catalog[] = {
            makeProduct(10, "Only Product", 50000, 10, 0)
        };
        quickSortByID(catalog, 0, 0);
        heapSortByPrice(catalog, 1);
        insertionSortByStock(catalog, 1);
        assert(catalog[0].id == 10 && catalog[0].price == 50000 && catalog[0].stock == 10);
        cout << "[PASS] Mang rong / mot phan tu\n\n";
    }
}

int main() {
    runCartTests();
    runSortTests();
    cout << "=======================================================\n";
    cout << "=== TAT CA CAC BAI KIEM THU DEU PASS THANH CONG! ===\n";
    cout << "=======================================================\n";
    return 0;
}
