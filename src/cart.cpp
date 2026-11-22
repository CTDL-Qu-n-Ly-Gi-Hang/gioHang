#include "gioHang.h"

void initCart(CartLinkedList& list) {
    list.head = nullptr;
    list.tail = nullptr;
}

CartNode* createCartNode(CartItem x) {
    if (x.unitPrice < 0) {
        x.unitPrice = 0; // Chan duoi don gia am khi discount > price
    }
    CartNode* newNode = new CartNode;
    newNode->data = x;
    newNode->prev = nullptr;
    newNode->next = nullptr;
    return newNode;
}

// Them cuoi O(1) nho con tro tail cua DSLK doi
void insertTailCart(CartLinkedList& list, CartItem x) {
    CartNode* newNode = createCartNode(x);
    if (list.head == nullptr) {
        list.head = list.tail = newNode;
        return;
    }
    list.tail->next = newNode;
    newNode->prev = list.tail;
    list.tail = newNode;
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
    if (list.head != nullptr) {
        list.head->prev = nullptr;
    } else {
        list.tail = nullptr;
    }
    delete temp;
}

void deleteTailCart(CartLinkedList& list) {
    if (list.tail == nullptr) return;
    CartNode* temp = list.tail;
    list.tail = list.tail->prev;
    if (list.tail != nullptr) {
        list.tail->next = nullptr;
    } else {
        list.head = nullptr;
    }
    delete temp;
}

// Xoa truc tiep nut tim thay bang lien ket 2 chieu prev/next
bool removeProductFromCart(CartLinkedList& list, int productId, CartItem& removedItem) {
    CartNode* target = findCartNode(list, productId);
    if (target == nullptr) return false;

    removedItem = target->data;

    if (target->prev != nullptr) {
        target->prev->next = target->next;
    } else {
        list.head = target->next;
    }

    if (target->next != nullptr) {
        target->next->prev = target->prev;
    } else {
        list.tail = target->prev;
    }

    delete target;
    return true;
}

void clearCart(CartLinkedList& list) {
    while (list.head != nullptr) {
        deleteHeadCart(list);
    }
}

void viewCart(CartLinkedList& cart, double voucherDiscount) {
    cout << "\n==================== GIO HANG HIEN TAI (DSLK DOI) ====================\n";
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

// --- Merge Sort tren Danh sach lien ket doi (Stable, O(n log n)) ---
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

    if (left->data.subtotal() <= right->data.subtotal()) {
        left->next = mergeSortedLists(left->next, right);
        if (left->next != nullptr) {
            left->next->prev = left;
        }
        left->prev = nullptr;
        return left;
    } else {
        right->next = mergeSortedLists(left, right->next);
        if (right->next != nullptr) {
            right->next->prev = right;
        }
        right->prev = nullptr;
        return right;
    }
}

static CartNode* mergeSortRec(CartNode* head) {
    if (head == nullptr || head->next == nullptr) {
        return head;
    }

    CartNode* mid = getMiddleNode(head);
    CartNode* rightHalf = mid->next;

    // Ngat lien ket 2 chieu giua 2 nua
    mid->next = nullptr;
    if (rightHalf != nullptr) {
        rightHalf->prev = nullptr;
    }

    CartNode* leftSorted = mergeSortRec(head);
    CartNode* rightSorted = mergeSortRec(rightHalf);

    return mergeSortedLists(leftSorted, rightSorted);
}

void sortCartBySubtotal(CartLinkedList& cart) {
    if (cart.head == nullptr || cart.head->next == nullptr) {
        cart.tail = cart.head;
        return;
    }

    cart.head = mergeSortRec(cart.head);
    cart.head->prev = nullptr;

    // Cap nhat lai con tro tail sau khi sap xep xong
    CartNode* cur = cart.head;
    while (cur->next != nullptr) {
        cur = cur->next;
    }
    cart.tail = cur;
}
