#include "gioHangdemo.h"

void initCart(CartLinkedList& list) {
    list.head = nullptr;
}

CartNode* createCartNode(CartItem x) {
    if (x.unitPrice < 0) {
        x.unitPrice = 0;
    }
    CartNode* newNode = new CartNode;
    newNode->data = x;
    newNode->next = nullptr;
    return newNode;
}

void insertTailCart(CartLinkedList& list, CartItem x) {
    CartNode* newNode = createCartNode(x);
    if (list.head == nullptr) {
        list.head = newNode;
        return;
    }
    CartNode* last = list.head;
    while (last->next != nullptr) {
        last = last->next;
    }
    last->next = newNode;
}

CartNode* findCartNode(CartLinkedList& list, int productId) {
    CartNode* cur = list.head;
    while (cur != nullptr) {
        if (cur->data.productId == productId) {
            return cur;
        }
        cur = cur->next;
    }
    return nullptr;
}

void deleteHeadCart(CartLinkedList& list) {
    if (list.head == nullptr) return;
    CartNode* temp = list.head;
    list.head = list.head->next;
    delete temp;
}

bool removeProductFromCart(CartLinkedList& list, int productId, CartItem& removedItem) {
    if (list.head == nullptr) return false;

    if (list.head->data.productId == productId) {
        removedItem = list.head->data;
        deleteHeadCart(list);
        return true;
    }

    CartNode* prev = list.head;
    while (prev->next != nullptr && prev->next->data.productId != productId) {
        prev = prev->next;
    }
    if (prev->next == nullptr) return false;

    CartNode* temp = prev->next;
    removedItem = temp->data;
    prev->next = temp->next;
    delete temp;
    return true;
}

void clearCart(CartLinkedList& list) {
    while (list.head != nullptr) {
        deleteHeadCart(list);
    }
}

void viewCart(CartLinkedList& cart, double voucherDiscount) {
    cout << "\n==================== GIO HANG HIEN TAI (DSLK DON) ====================\n";
    if (cart.head == nullptr) {
        cout << "  (Gio hang dang trong!)\n";
        cout << "======================================================================\n";
        return;
    }
    cout << left << setw(8) << "Ma SP" << setw(32) << "Ten San Pham"
         << right << setw(12) << "Gia Sau KM" << setw(10) << "So Luong" << setw(15) << "Thanh Tien\n";
    cout << "-----------------------------------------------------------------------------\n";

    double rawTotal = 0;
    CartNode* cur = cart.head;
    while (cur != nullptr) {
        cout << left << setw(8) << cur->data.productId
             << setw(32) << cur->data.name
             << right << setw(12) << fixed << setprecision(0) << cur->data.unitPrice
             << setw(10) << cur->data.quantity
             << setw(14) << cur->data.subtotal() << "d\n";
        rawTotal += cur->data.subtotal();
        cur = cur->next;
    }

    double vat = rawTotal * 0.08;
    double finalAmount = rawTotal + vat - voucherDiscount;
    if (finalAmount < 0) finalAmount = 0;

    cout << "-----------------------------------------------------------------------------\n";
    cout << "Tam tinh: " << setw(66) << rawTotal << "d\n";
    cout << "Thue VAT (8%): " << setw(61) << vat << "d\n";
    cout << "Giam gia Voucher (Hash Table): -" << setw(44) << voucherDiscount << "d\n";
    cout << "TONG CONG THANH TOAN: " << setw(54) << finalAmount << "d\n";
    cout << "=============================================================================\n";
}

static CartNode* getMiddleNode(CartNode* head) {
    if (head == nullptr) return head;
    CartNode* slow = head;
    CartNode* fast = head->next;

    while (fast != nullptr && fast->next != nullptr) {
        slow = slow->next;
        fast = fast->next->next;
    }
    return slow;
}

static CartNode* mergeSortedLists(CartNode* left, CartNode* right) {
    if (left == nullptr) return right;
    if (right == nullptr) return left;

    CartNode* result = nullptr;

    if (left->data.subtotal() <= right->data.subtotal()) {
        result = left;
        result->next = mergeSortedLists(left->next, right);
    } else {
        result = right;
        result->next = mergeSortedLists(left, right->next);
    }
    return result;
}

static CartNode* mergeSortRec(CartNode* head) {
    if (head == nullptr || head->next == nullptr) {
        return head;
    }

    CartNode* mid = getMiddleNode(head);
    CartNode* rightHalf = mid->next;
    mid->next = nullptr;

    CartNode* leftSorted = mergeSortRec(head);
    CartNode* rightSorted = mergeSortRec(rightHalf);

    return mergeSortedLists(leftSorted, rightSorted);
}

void sortCartBySubtotal(CartLinkedList& cart) {
    if (cart.head == nullptr || cart.head->next == nullptr) {
        return;
    }
    cart.head = mergeSortRec(cart.head);
}