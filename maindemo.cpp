#include "gioHangdemo.h"

int main() {
    // ========================================================================
    // 1. KHUNG KHOI TAO CHUONG TRINH & NAP DU LIEU TU FILE CSV
    // ========================================================================
    const string csvInputFile = "products.csv";
    const string csvSortedFile = "products_sorted.csv";
    initSampleCSV(csvInputFile);

    // Cap phat mang dong tren Heap de luu tru kho hang
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

    // ========================================================================
    // 2. VONG LAP DIEU KHIEN MENU CHINH & XU LY NHAP LIEU
    // ========================================================================
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
                // [HOOK] Goi thuat toan quickSortByID(catalog, 0, n - 1) tai day
                cout << "-> [DEMO KHUNG] Da nhan lenh sap xep theo Ma SP (Quick Sort)!\n";
            } else if (sortOpt == 2) {
                // [HOOK] Goi thuat toan heapSortByPrice(catalog, n) tai day
                cout << "-> [DEMO KHUNG] Da nhan lenh sap xep theo Don Gia (Heap Sort)!\n";
            } else if (sortOpt == 3) {
                // [HOOK] Goi thuat toan insertionSortByStock(catalog, n) tai day
                cout << "-> [DEMO KHUNG] Da nhan lenh sap xep theo Ton Kho (Insertion Sort)!\n";
            }
            printCatalog(catalog, n, 15);
            if (saveCatalogToCSV(csvSortedFile, catalog, n)) {
                cout << "=> [XUAT FILE THANH CONG] Da ghi toan bo " << n
                     << " ban ghi ra file '" << csvSortedFile << "'!\n";
            }
        }
        else if (choice == 3) {
            showSearchMenu();
            int sOpt = inputInt("Lua chon: ");
            if (sOpt == 1) {
                int targetId = inputInt("Nhap Ma SP can tim: ");
                // [HOOK] Goi BinarySearchByID hoac SentinelSearchByID tai day
                int idx = findProductIndexByID(catalog, n, targetId);
                if (idx != -1)
                    cout << "Tim thay tai chi so [" << idx << "]: " << catalog[idx].name
                         << " | Gia: " << fixed << setprecision(0) << catalog[idx].price
                         << "d | Ton: " << catalog[idx].stock << "\n";
                else
                    cout << "Khong tim thay san pham co ID = " << targetId << "\n";
            } else {
                string kw = inputString("Nhap tu khoa ten san pham: ");
                // [HOOK] Goi SequentialSearchByName(catalog, n, kw) tai day
                cout << "-> [DEMO KHUNG] Da nhan tu khoa tim kiem theo ten: \"" << kw << "\"\n";
            }
        }
        else if (choice == 4) {
            int pid = inputInt("Nhap Ma SP muon them: ");
            int qty = inputInt("Nhap so luong: ");

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
                cout << "-> Da them [" << p.name << "] vao cuoi DSLK gio hang!\n";
            }
        }
        else if (choice == 5) {
            showCartSubMenu();
            int sub = inputInt("Lua chon: ");
            if (sub == 1) {
                int pid = inputInt("Nhap Ma SP trong gio: ");
                CartNode* node = findCartNode(cart, pid);
                if (!node) { cout << "-> San pham khong co trong gio!\n"; continue; }
                int newQty = inputInt("Nhap so luong moi: ");
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
                clearCart(cart);
                destroyStack(undoStack);
                activeVoucherDiscount = 0;
                cout << "-> Da xoa rong toan bo gio hang!\n";
            }
        }
        else if (choice == 6) {
            // [HOOK] Goi sortCartBySubtotal(cart) (Merge Sort DSLK) tai day
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
    } while (choice != 0);

    // ========================================================================
    // 3. KHUNG KET THUC CHUONG TRINH & GIAI PHONG BO NHO DONG
    // ========================================================================
    clearCart(cart);
    destroyStack(undoStack);
    destroyQueue(orderQueue);
    destroyAVLNode(avlIndex.root);
    destroyHashTable(voucherTable);
    delete[] catalog;
    cout << "Da giai phong toan bo bo nho dong (Mang dong, DSLK, Stack, Queue, AVL, Hash Table). Tam biet!\n";
    return 0;
}