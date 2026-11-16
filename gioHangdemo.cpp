#include "gioHangdemo.h"

// ============================================================================
// PHAN 1: HE THONG MENU DIEU HUONG (MENU UI)
// ============================================================================

void showMainMenu(const string& csvSortedFile) {
    cout << "\n================ HE THONG QUAN LY GIO HANG (DSA 124002) ================\n";
    cout << " 1. Xem kho hang tu file CSV & Duyet cay chi muc AVL (LNR)\n";
    cout << " 2. Sap xep 3000 san pham & Xuat ra file '" << csvSortedFile << "'\n";
    cout << " 3. Tim kiem san pham (Binary Search / Sentinel Search / Sequential)\n";
    cout << " 4. Them san pham vao gio hang (DSLK Don + Kiem tra ton kho)\n";
    cout << " 5. Cap nhat so luong / Xoa 1 mon / Xoa toan bo gio hang\n";
    cout << " 6. Xem gio hang & Sap xep gio hang theo Thanh tien (Merge Sort DSLK)\n";
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

// ============================================================================
// PHAN 2: NHAP LIEU & DOC/GHI FILE (INPUT & FILE CSV I/O)
// ============================================================================

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

void initSampleCSV(const string& filename) {
    ifstream checkFile(filename);
    if (checkFile.good()) return;
    checkFile.close();

    ofstream outFile(filename);
    outFile << "ProductID,Name,Price,Stock,Discount\n";
    outFile << "105,Ban phim co AKKO,1250000,15,50000\n";
    outFile << "102,Chuot Logitech G102,400000,30,20000\n";
    outFile << "108,Tai nghe Sony WH,3500000,8,200000\n";
    outFile << "101,Op lung POCO X8 Pro,150000,50,10000\n";
    outFile << "104,Gia do Laptop Nhom,250000,25,0\n";
    outFile << "107,Sac du phong Anker,850000,12,50000\n";
    outFile << "103,Lot chuot Gaming XL,120000,40,0\n";
    outFile << "106,Hub USB-C Baseus,550000,20,30000\n";
    outFile.close();
}

int loadCatalogFromCSV(const string& filename, Product catalog[], int maxCap, AVLTree& avlIndex) {
    ifstream file(filename);
    if (!file.is_open()) return 0;

    string line;
    getline(file, line); // Bo qua dong tieu de (Header)
    int count = 0;

    while (getline(file, line) && count < maxCap) {
        if (line.empty()) continue;
        stringstream ss(line);
        string idStr, name, priceStr, stockStr, discStr;

        getline(ss, idStr, ',');
        getline(ss, name, ',');
        getline(ss, priceStr, ',');
        getline(ss, stockStr, ',');
        getline(ss, discStr, ',');

        Product p;
        p.id = stoi(idStr);
        p.name = name;
        p.price = stod(priceStr);
        p.stock = stoi(stockStr);
        p.discount = stod(discStr);

        catalog[count++] = p;
        avlIndex.root = addAVLNode(avlIndex.root, p);
    }
    file.close();
    return count;
}

bool saveCatalogToCSV(const string& filename, Product catalog[], int n) {
    ofstream outFile(filename);
    if (!outFile.is_open()) return false;

    outFile << "ProductID,Name,Price,Stock,Discount\n";
    for (int i = 0; i < n; i++) {
        outFile << catalog[i].id << ","
                << catalog[i].name << ","
                << fixed << setprecision(0) << catalog[i].price << ","
                << catalog[i].stock << ","
                << fixed << setprecision(0) << catalog[i].discount << "\n";
    }
    outFile.close();
    return true;
}

// ============================================================================
// PHAN 3: CAC HAM DUNG CHUNG (SHARED / UTILITY FUNCTIONS)
// ============================================================================

// --- 3.1. Cac ham Hien thi dung chung (Display Helpers) ---
void printCatalog(Product catalog[], int n, int previewLimit) {
    cout << "\n+-------+----------------------------------+-------------+---------+-------------+\n";
    cout << "| Ma SP | Ten San Pham                     | Don Gia (d) | Ton Kho | Giam Gia(d) |\n";
    cout << "+-------+----------------------------------+-------------+---------+-------------+\n";
    int limit = (n > previewLimit) ? previewLimit : n;
    for (int i = 0; i < limit; i++) {
        cout << "| " << setw(5) << catalog[i].id << " | "
             << left << setw(32) << catalog[i].name << right << " | "
             << setw(11) << fixed << setprecision(0) << catalog[i].price << " | "
             << setw(7) << catalog[i].stock << " | "
             << setw(11) << catalog[i].discount << " |\n";
    }
    if (n > previewLimit) {
        cout << "|  ...  | ... (An " << setw(4) << (n - previewLimit - 5) << " dong o giua) ...        |     ...     |   ...   |     ...     |\n";
        for (int i = n - 5; i < n; i++) {
            cout << "| " << setw(5) << catalog[i].id << " | "
                 << left << setw(32) << catalog[i].name << right << " | "
                 << setw(11) << fixed << setprecision(0) << catalog[i].price << " | "
                 << setw(7) << catalog[i].stock << " | "
                 << setw(11) << catalog[i].discount << " |\n";
        }
    }
    cout << "+-------+----------------------------------+-------------+---------+-------------+\n";
    cout << "Tong so san pham trong danh sach: " << n << " ban ghi.\n";
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

// --- 3.2. Ham dung chung quan ly Danh sach lien ket don (Gio hang) ---
void initCart(CartLinkedList& list) {
    list.head = nullptr;
}

CartNode* createCartNode(CartItem x) {
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
    while (last->next != nullptr)
        last = last->next;
    last->next = newNode;
}

CartNode* findCartNode(CartLinkedList& list, int productId) {
    CartNode* cur = list.head;
    while (cur != nullptr) {
        if (cur->data.productId == productId)
            return cur;
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

// --- 3.3. Ham dung chung quan ly Stack (UNDO) ---
void initStack(UndoStack& s) { s.top = nullptr; }
bool isEmptyStack(UndoStack& s) { return s.top == nullptr; }

void pushStack(UndoStack& s, UndoAction val) {
    StackNode* newNode = new StackNode;
    newNode->data = val;
    newNode->next = s.top;
    s.top = newNode;
}

UndoAction topStack(UndoStack& s) { return s.top->data; }

void popStack(UndoStack& s) {
    if (isEmptyStack(s)) return;
    StackNode* temp = s.top;
    s.top = s.top->next;
    delete temp;
}

void destroyStack(UndoStack& s) {
    while (!isEmptyStack(s)) popStack(s);
}

// --- 3.4. Ham dung chung quan ly Queue (Don hang) ---
void initQueue(OrderQueue& q) {
    q.front = nullptr;
    q.rear = nullptr;
}

bool isEmptyQueue(OrderQueue& q) { return q.front == nullptr; }

void enqueueOrder(OrderQueue& q, Order val) {
    QueueNode* newNode = new QueueNode{val, nullptr};
    if (isEmptyQueue(q))
        q.front = newNode;
    else
        q.rear->next = newNode;
    q.rear = newNode;
}

Order dequeueOrder(OrderQueue& q) {
    QueueNode* tmp = q.front;
    Order data = tmp->data;
    q.front = q.front->next;
    if (q.front == nullptr)
        q.rear = nullptr;
    delete tmp;
    return data;
}

void destroyQueue(OrderQueue& q) {
    while (!isEmptyQueue(q)) dequeueOrder(q);
}

// --- 3.5. Ham dung chung quan ly Cay AVL (Chi muc san pham) ---
void initAVL(AVLTree& t) { t.root = nullptr; }

int heightAVL(AVLNode* node) {
    if (node == nullptr) return -1;
    return node->height;
}

int balanceFactor(AVLNode* node) {
    return heightAVL(node->left) - heightAVL(node->right);
}

void updateHeight(AVLNode* node) {
    int lh = heightAVL(node->left);
    int rh = heightAVL(node->right);
    node->height = 1 + (lh > rh ? lh : rh);
}

AVLNode* createAVLNode(Product val) {
    AVLNode* node = new AVLNode;
    node->data = val;
    node->left = node->right = nullptr;
    node->height = 0;
    return node;
}

AVLNode* rotateRight(AVLNode* node) {
    AVLNode* child = node->left;
    AVLNode* grandChild = child->right;
    child->right = node;
    node->left = grandChild;
    updateHeight(node);
    updateHeight(child);
    return child;
}

AVLNode* rotateLeft(AVLNode* node) {
    AVLNode* child = node->right;
    AVLNode* grandChild = child->left;
    child->left = node;
    node->right = grandChild;
    updateHeight(node);
    updateHeight(child);
    return child;
}

AVLNode* rotateLR(AVLNode* node) {
    node->left = rotateLeft(node->left);
    return rotateRight(node);
}

AVLNode* rotateRL(AVLNode* node) {
    node->right = rotateRight(node->right);
    return rotateLeft(node);
}

AVLNode* rebalance(AVLNode* node) {
    updateHeight(node);
    int bf = balanceFactor(node);
    if (bf > 1) {
        if (balanceFactor(node->left) >= 0)
            return rotateRight(node);
        else
            return rotateLR(node);
    }
    if (bf < -1) {
        if (balanceFactor(node->right) <= 0)
            return rotateLeft(node);
        else
            return rotateRL(node);
    }
    return node;
}

AVLNode* addAVLNode(AVLNode* node, Product val) {
    if (node == nullptr) return createAVLNode(val);
    if (val.id < node->data.id)
        node->left = addAVLNode(node->left, val);
    else if (val.id > node->data.id)
        node->right = addAVLNode(node->right, val);
    else {
        node->data = val;
        return node;
    }
    return rebalance(node);
}

AVLNode* searchAVL(AVLNode* node, int productId) {
    while (node != nullptr) {
        if (productId == node->data.id) return node;
        if (productId < node->data.id)
            node = node->left;
        else
            node = node->right;
    }
    return nullptr;
}

void inOrderAVL(AVLNode* root, int& printedCount, int maxPrint) {
    if (root == nullptr || printedCount >= maxPrint) return;
    inOrderAVL(root->left, printedCount, maxPrint);
    if (printedCount < maxPrint) {
        cout << "  [AVL LNR #" << setw(4) << (printedCount + 1) << "] ID: " << root->data.id 
             << " | " << left << setw(32) << root->data.name << right
             << " | Gia: " << setw(9) << fixed << setprecision(0) << root->data.price 
             << " | Height nut: " << setw(2) << root->height << "\n";
        printedCount++;
    }
    inOrderAVL(root->right, printedCount, maxPrint);
}

void destroyAVLNode(AVLNode* node) {
    if (node == nullptr) return;
    destroyAVLNode(node->left);
    destroyAVLNode(node->right);
    delete node;
}

// --- 3.6. Ham dung chung quan ly Bang bam (Voucher) ---
void initHashTable(VoucherHashTable& ht) {
    for (int i = 0; i < HASH_SIZE; i++)
        ht.buckets[i] = nullptr;
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
        if (cur->voucherKey == key)
            return cur;
        cur = cur->next;
        }
    return nullptr;
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

// --- 3.7. Ham tra cuu mang dung chung (Tim chi so san pham theo ID) ---
int findProductIndexByID(Product catalog[], int n, int productId) {
    for (int i = 0; i < n; i++) {
        if (catalog[i].id == productId) return i;
    }
    return -1;
}