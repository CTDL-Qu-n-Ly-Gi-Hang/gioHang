#include "gioHang.h"

// ============================================================================
// PHAN 1: CAC HAM HIEN THI MENU & NHAP LIEU (TV1)
// ============================================================================

void showMainMenu(const string& csvSortedFile) {
    cout << "\n================ HE THONG QUAN LY GIO HANG (DSA 124002) ================\n";
    cout << " 1. Xem kho hang tu file CSV & Duyet cay chi muc AVL (LNR)\n";
    cout << " 2. Sap xep san pham & Xuat ra file '" << csvSortedFile << "'\n";
    cout << " 3. Tim kiem san pham (Binary Search / Sentinel Search / Sequential)\n";
    cout << " 4. Them san pham vao gio hang (DSLK Doi + Kiem tra ton kho)\n";
    cout << " 5. Cap nhat so luong / Xoa 1 mon / Xoa toan bo gio hang\n";
    cout << " 6. Xem gio hang & Sap xep gio hang theo Thanh tien (Merge Sort DSLK Doi)\n";
    cout << " 7. Ap dung ma giam gia Voucher (Tra cuu Bang bam O(1))\n";
    cout << " 8. UNDO thao tac gio hang gan nhat (Su dung Stack LIFO)\n";
    cout << " 9. Thanh toan (Checkout -> Dua vao Queue FIFO) & Xu ly don hang\n";
    cout << " 0. Thoat va giai phong bo nho\n";
    cout << "========================================================================\n";
}

void showSortMenu() {
    cout << "Chon kieu sap xep:\n";
    cout << "  (1) Quick Sort theo Ma SP (ProductID tang dan)\n";
    cout << "  (2) Heap Sort theo Don Gia (Price tang dan)\n";
    cout << "  (3) Insertion Sort theo Ton Kho (Stock tang dan - Stable)\n";
}

void showSearchMenu() {
    cout << "Chon kieu tim: (1) Tim theo ID (Binary/Sentinel) | (2) Tim theo Ten (Sequential)\n";
}

void showCartSubMenu() {
    cout << "(1) Cap nhat so luong | (2) Xoa 1 san pham | (3) Xoa toan bo (Clear)\n";
}

int inputInt(const string& prompt) {
    int value;
    while (true) {
        cout << prompt;
        if (cin >> value) {
            return value;
        }
        cout << "-> Loi nhap lieu! Vui long nhap mot so nguyen hop le.\n";
        cin.clear();
        cin.ignore(1000, '\n');
    }
}

string inputString(const string& prompt) {
    cout << prompt;
    string s;
    getline(cin >> ws, s);
    return s;
}

// ============================================================================
// PHAN 2: HAM MAIN CHINH CUA CHUONG TRINH
// ============================================================================

int main() {
    // ------------------------------------------------------------------------
    // 1. KHOI TAO CHUONG TRINH & NAP DU LIEU TU FILE CSV
    // ------------------------------------------------------------------------
    const string csvInputFile = "products.csv";
    const string csvSortedFile = "products_sorted.csv";

    initSampleCSV(csvInputFile);

    Product* catalog = new Product[MAX_CATALOG];

    AVLTree avlIndex;
    initAVL(avlIndex);

    int n = loadCatalogFromCSV(csvInputFile, catalog, MAX_CATALOG, avlIndex);
    cout << "[Khoi tao] Da doc thanh cong " << n << " san pham tu file '" << csvInputFile
         << "' vao RAM & Cay AVL (Chieu cao h = " << heightAVL(avlIndex.root) << ").\n";

    CartLinkedList cart;
    initCart(cart);

    UndoStack undoStack;
    initStack(undoStack);

    OrderQueue orderQueue;
    initQueue(orderQueue);
    int nextOrderId = 1001;

    VoucherHashTable voucherTable;
    initHashTable(voucherTable);
    insertVoucher(voucherTable, 10, "UTH50K", 50000);
    insertVoucher(voucherTable, 21, "SINHVIEN100K", 100000);
    insertVoucher(voucherTable, 15, "FREESHIP30K", 30000);

    double activeVoucherDiscount = 0;
    bool isSortedByID = false; // Co danh dau mang da sap tang theo ID chua

    // ------------------------------------------------------------------------
    // 2. VONG LAP DIEU KHIEN MENU CHINH
    // ------------------------------------------------------------------------
    int choice;
    do {
        showMainMenu(csvSortedFile);
        choice = inputInt("Nhap lua chon cua ban: ");

        if (choice == 1) {
            printCatalog(catalog, n, 15);
            cout << "\n--- DUYET CAY TIM KIEM CAN BANG AVL THEO THU TU GIUA (LNR - 15 NUT DAU) ---\n";
            int printed = 0;
            inOrderAVL(avlIndex.root, printed, 15);
            cout << "-> Chieu cao cay AVL hien tai voi " << n << " nut la h = "
                 << heightAVL(avlIndex.root) << " (O(log n)).\n";
        }
        else if (choice == 2) {
            showSortMenu();
            int sortOpt = inputInt("Lua chon: ");

            if (sortOpt == 1) {
                quickSortByID(catalog, 0, n - 1);
                isSortedByID = true;
                cout << "-> Da sap xep tang dan theo Ma SP (Quick Sort)!\n";
            } else if (sortOpt == 2) {
                heapSortByPrice(catalog, n);
                isSortedByID = false;
                cout << "-> Da sap xep tang dan theo Don Gia (Heap Sort)!\n";
            } else if (sortOpt == 3) {
                insertionSortByStock(catalog, n);
                isSortedByID = false;
                cout << "-> Da sap xep tang dan theo Ton Kho (Insertion Sort - Stable)!\n";
            } else {
                cout << "-> Lua chon sap xep khong hop le!\n";
                continue;
            }

            printCatalog(catalog, n, 15);
            if (saveCatalogToCSV(csvSortedFile, catalog, n)) {
                cout << "=> [XUAT FILE THANH CONG] Da ghi toan bo " << n
                     << " ban ghi ra file '" << csvSortedFile << "'!\n";
            } else {
                cout << "=> [LOI] Ghi file '" << csvSortedFile << "' that bai!\n";
            }
        }
        else if (choice == 3) {
            showSearchMenu();
            int sOpt = inputInt("Lua chon: ");

            if (sOpt == 1) {
                int targetId = inputInt("Nhap Ma SP can tim: ");
                int idx = -1;

                if (isSortedByID) {
                    idx = binarySearchByID(catalog, n, targetId);
                    cout << "[Dung Binary Search O(log n)]\n";
                } else {
                    idx = sentinelSearchByID(catalog, n, targetId);
                    cout << "[Dung Sentinel Search O(n)]\n";
                }

                if (idx != -1) {
                    cout << "Tim thay tai chi so [" << idx << "]: " << catalog[idx].name
                         << " | Gia: " << fixed << setprecision(0) << catalog[idx].price
                         << "d | Ton: " << catalog[idx].stock << "\n";
                } else {
                    cout << "Khong tim thay san pham co ID = " << targetId << "\n";
                }
            }
            else if (sOpt == 2) {
                string kw = inputString("Nhap tu khoa ten san pham (khong dau): ");
                const int MAX_DISPLAY = 20;
                int resultIdx[MAX_DISPLAY];

                int totalFound = sequentialSearchByName(catalog, n, kw, resultIdx, MAX_DISPLAY);
                if (totalFound == 0) {
                    cout << "Khong tim thay san pham nao (hoac tu khoa rong).\n";
                } else {
                    int showCount = (totalFound > MAX_DISPLAY) ? MAX_DISPLAY : totalFound;
                    cout << "\n--- KET QUA TIM KIEM THEO TEN (Hien thi toi da "
                         << showCount << " / " << totalFound << " ket qua) ---\n";
                    for (int i = 0; i < showCount; ++i) {
                        const Product& p = catalog[resultIdx[i]];
                        cout << "  [" << setw(4) << resultIdx[i] << "] ID: " << setw(5) << p.id
                             << " | " << left << setw(32) << p.name << right
                             << " | Gia: " << setw(10) << fixed << setprecision(0) << p.price
                             << "d | Ton: " << p.stock << "\n";
                    }
                }
            }
            else {
                cout << "-> Lua chon tim kiem khong hop le!\n";
            }
        }
        else if (choice == 4) {
            int pid = inputInt("Nhap Ma SP muon them: ");
            int qty = inputInt("Nhap so luong: ");

            if (qty <= 0) {
                cout << "-> Loi: So luong phai lon hon 0!\n";
                continue;
            }

            AVLNode* foundNode = searchAVL(avlIndex.root, pid);
            if (foundNode == nullptr) {
                cout << "-> Loi: Ma san pham khong ton tai!\n";
                continue;
            }

            Product p = foundNode->data;
            CartNode* existing = findCartNode(cart, pid);
            int currentInCart = (existing != nullptr) ? existing->data.quantity : 0;

            if (currentInCart + qty > p.stock) {
                cout << "-> Loi: So luong yeu cau vuot qua ton kho (" << p.stock << ")!\n";
                continue;
            }

            if (existing != nullptr) {
                pushStack(undoStack, {ACT_UPDATE, existing->data, existing->data.quantity});
                existing->data.quantity += qty;
                cout << "-> Da cong don so luong cho [" << p.name << "] trong gio!\n";
            } else {
                CartItem newItem{p.id, p.name, p.price - p.discount, qty};
                insertTailCart(cart, newItem);
                pushStack(undoStack, {ACT_ADD, newItem, 0});
                cout << "-> Da them [" << p.name << "] vao cuoi DSLK doi gio hang!\n";
            }
        }
        else if (choice == 5) {
            showCartSubMenu();
            int sub = inputInt("Lua chon: ");
            if (sub == 1) {
                int pid = inputInt("Nhap Ma SP trong gio: ");
                CartNode* node = findCartNode(cart, pid);
                if (!node) {
                    cout << "-> San pham khong co trong gio!\n";
                    continue;
                }
                int newQty = inputInt("Nhap so luong moi: ");
                if (newQty <= 0) {
                    cout << "-> Loi: So luong phai lon hon 0!\n";
                    continue;
                }
                AVLNode* pNode = searchAVL(avlIndex.root, pid);
                if (newQty > pNode->data.stock) {
                    cout << "-> Vuot qua ton kho (" << pNode->data.stock << ")!\n";
                    continue;
                }
                pushStack(undoStack, {ACT_UPDATE, node->data, node->data.quantity});
                node->data.quantity = newQty;
                cout << "-> Da cap nhat so luong!\n";
            } else if (sub == 2) {
                int pid = inputInt("Nhap Ma SP can xoa: ");
                CartItem removed;
                if (removeProductFromCart(cart, pid, removed)) {
                    pushStack(undoStack, {ACT_REMOVE, removed, removed.quantity});
                    cout << "-> Da xoa [" << removed.name << "] khoi gio hang!\n";
                } else {
                    cout << "-> Khong tim thay Ma SP trong gio!\n";
                }
            } else if (sub == 3) {
                if (cart.head == nullptr) {
                    cout << "-> Gio hang dang trong san!\n";
                    continue;
                }

                int count = 0;
                // Duyet nguoc tu tail -> head de khi UNDO pop ra se giu dung thu tu ban dau
                for (CartNode* cur = cart.tail; cur != nullptr; cur = cur->prev) {
                    pushStack(undoStack, {ACT_REMOVE, cur->data, cur->data.quantity});
                    count++;
                }
                CartItem emptyItem{0, "", 0.0, 0};
                pushStack(undoStack, {ACT_CLEAR, emptyItem, count});

                clearCart(cart);
                activeVoucherDiscount = 0;
                cout << "-> Da xoa rong toan bo gio hang! (Chon Menu 8 neu muon UNDO khoi phuc)\n";
            } else {
                cout << "-> Lua chon khong hop le!\n";
            }
        }
        else if (choice == 6) {
            sortCartBySubtotal(cart);
            cout << "-> Da sap xep gio hang tang dan theo Thanh tien (Merge Sort DSLK Doi)!\n";
            viewCart(cart, activeVoucherDiscount);
        }
        else if (choice == 7) {
            displayVouchers(voucherTable);
            int vKey = inputInt("Nhap Ma so Voucher (10, 21 hoac 15): ");
            VoucherNode* v = searchVoucher(voucherTable, vKey);
            if (v != nullptr) {
                activeVoucherDiscount = v->discountValue;
                cout << "-> Ap dung thanh cong Voucher [" << v->codeName
                     << "] giam " << fixed << setprecision(0) << activeVoucherDiscount << "d!\n";
            } else {
                cout << "-> Ma Voucher khong hop le!\n";
            }
        }
        else if (choice == 8) {
            if (isEmptyStack(undoStack)) {
                cout << "-> Khong co thao tac nao trong Stack de Undo!\n";
                continue;
            }
            UndoAction lastAct = topStack(undoStack);
            popStack(undoStack);

            if (lastAct.type == ACT_ADD) {
                CartItem dummy;
                removeProductFromCart(cart, lastAct.itemState.productId, dummy);
                cout << "-> [UNDO] Da hoan tac lenh Them: Xoa [" << lastAct.itemState.name << "] khoi gio!\n";
            } else if (lastAct.type == ACT_REMOVE) {
                insertTailCart(cart, lastAct.itemState);
                cout << "-> [UNDO] Da hoan tac lenh Xoa: Khoi phuc [" << lastAct.itemState.name << "] vao gio!\n";
            } else if (lastAct.type == ACT_UPDATE) {
                CartNode* node = findCartNode(cart, lastAct.itemState.productId);
                if (node) node->data.quantity = lastAct.oldQuantity;
                cout << "-> [UNDO] Da khoi phuc so luong cu (" << lastAct.oldQuantity
                     << ") cho [" << lastAct.itemState.name << "]!\n";
            } else if (lastAct.type == ACT_CLEAR) {
                int count = lastAct.oldQuantity;
                for (int i = 0; i < count && !isEmptyStack(undoStack); i++) {
                    UndoAction itemAct = topStack(undoStack);
                    popStack(undoStack);
                    insertTailCart(cart, itemAct.itemState);
                }
                cout << "-> [UNDO] Da khoi phuc lai toan bo " << count << " san pham vao gio hang!\n";
            }
        }
        else if (choice == 9) {
            if (cart.head == nullptr) {
                cout << "-> Gio hang rong, khong the Checkout!\n";
                continue;
            }

            bool stockOK = true;
            double rawTotal = 0;
            for (CartNode* cur = cart.head; cur != nullptr; cur = cur->next) {
                int idx = findProductIndexByID(catalog, n, cur->data.productId);
                if (idx == -1 || catalog[idx].stock < cur->data.quantity) {
                    cout << "-> Loi ton kho voi san pham: " << cur->data.name << "\n";
                    stockOK = false;
                    break;
                }
                rawTotal += cur->data.subtotal();
            }
            if (!stockOK) continue;

            for (CartNode* cur = cart.head; cur != nullptr; cur = cur->next) {
                int idx = findProductIndexByID(catalog, n, cur->data.productId);
                catalog[idx].stock -= cur->data.quantity;
                avlIndex.root = addAVLNode(avlIndex.root, catalog[idx]);
            }

            string cusName = inputString("Nhap ten khach hang thanh toan: ");
            double finalTotal = rawTotal * 1.08 - activeVoucherDiscount;
            if (finalTotal < 0) finalTotal = 0;

            enqueueOrder(orderQueue, {nextOrderId++, cusName, finalTotal});
            clearCart(cart);
            destroyStack(undoStack);
            activeVoucherDiscount = 0;

            cout << "-> Da tao don hang va dua vao Queue! Dang xu ly hang doi FIFO:\n";
            while (!isEmptyQueue(orderQueue)) {
                Order ord = dequeueOrder(orderQueue);
                cout << "   [QUEUE PROCESSED] Ma Don: #" << ord.orderId
                     << " | Khach: " << ord.customerName
                     << " | Tong thanh toan: " << fixed << setprecision(0) << ord.finalTotal << "d\n";
            }
        }
        else if (choice != 0) {
            cout << "-> Lua chon khong hop le! Vui long chon tu 0 den 9.\n";
        }
    } while (choice != 0);

    // ------------------------------------------------------------------------
    // 3. GIAI PHONG BO NHO DONG & KET THUC CHUONG TRINH
    // ------------------------------------------------------------------------
    clearCart(cart);
    destroyStack(undoStack);
    destroyQueue(orderQueue);
    destroyAVLNode(avlIndex.root);
    destroyHashTable(voucherTable);
    delete[] catalog;

    cout << "Da giai phong toan bo bo nho dong (Mang dong, DSLK Doi, Stack, Queue, AVL, Hash Table). Tam biet!\n";
    return 0;
}
