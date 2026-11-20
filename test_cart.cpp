#include "gioHangdemo.h"
#include <cassert>
#include <cmath>

void sortCartBySubtotal(CartLinkedList& cart);

int countCartNodes(const CartLinkedList& cart) {
    int count = 0;
    CartNode* cur = cart.head;
    while (cur != nullptr) {
        count++;
        cur = cur->next;
    }
    return count;
}

bool isCartSortedBySubtotal(const CartLinkedList& cart) {
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

int main() {
    cout << "=== BAT DAU KIEM THU MODULE GIO HANG & MERGE SORT (TV5) ===\n\n";

    CartLinkedList cart;
    initCart(cart);
    assert(cart.head == nullptr);

    cout << "[TEST 1] Gio hang rong (xoa, tim, sap xep, in)...\n";
    viewCart(cart, 0);
    sortCartBySubtotal(cart);
    assert(cart.head == nullptr);
    assert(findCartNode(cart, 101) == nullptr);
    CartItem dummy;
    assert(removeProductFromCart(cart, 101, dummy) == false);
    cout << "-> PASS Test 1!\n\n";

    cout << "[TEST 2] Them 1 phan tu & sap xep 1 phan tu...\n";
    insertTailCart(cart, {101, "Op lung POCO X8 Pro", 140000, 1});
    sortCartBySubtotal(cart);
    assert(countCartNodes(cart) == 1);
    assert(cart.head->data.productId == 101);
    cout << "-> PASS Test 2!\n\n";

    cout << "[TEST 3] Them 5 mon (co cap trung thanh tien) & Merge Sort...\n";
    insertTailCart(cart, {105, "Ban phim co AKKO", 1200000, 1});
    insertTailCart(cart, {102, "Chuot Logitech G102", 380000, 2});
    insertTailCart(cart, {103, "Lot chuot Gaming XL", 140000, 1});
    insertTailCart(cart, {104, "Gia do Laptop Nhom", 250000, 2});

    assert(countCartNodes(cart) == 5);
    sortCartBySubtotal(cart);

    assert(countCartNodes(cart) == 5);
    assert(isCartSortedBySubtotal(cart) == true);
    assert(cart.head->data.productId == 101);
    assert(cart.head->next->data.productId == 103);
    assert(cart.head->next->next->data.productId == 104);
    assert(cart.head->next->next->next->data.productId == 102);
    assert(cart.head->next->next->next->next->data.productId == 105);
    cout << "-> PASS Test 3 (Da xac minh thu tu & tinh on dinh Stable)!\n\n";

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
    assert(removeProductFromCart(cart, 102, dummy) == true && dummy.productId == 102);
    assert(removeProductFromCart(cart, 105, dummy) == true && dummy.productId == 105);
    assert(countCartNodes(cart) == 2);
    cout << "-> PASS Test 6!\n\n";

    cout << "[TEST 7] Chan duoi don gia am khi discount > price...\n";
    insertTailCart(cart, {108, "San pham loi gia", -50000, 2});
    CartNode* badNode = findCartNode(cart, 108);
    assert(badNode != nullptr && badNode->data.unitPrice == 0 && badNode->data.subtotal() == 0);
    cout << "-> PASS Test 7!\n\n";

    cout << "[TEST 8] Xoa toan bo gio hang (clearCart 2 lan lien tiep)...\n";
    clearCart(cart);
    assert(cart.head == nullptr);
    clearCart(cart);
    assert(cart.head == nullptr);
    cout << "-> PASS Test 8!\n\n";

    cout << "=== TAT CA 8 BAI TEST CUA TV5 DEU PASS THANH CONG! ===\n";
    return 0;
}