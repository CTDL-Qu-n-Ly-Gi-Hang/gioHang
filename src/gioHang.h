#ifndef GIO_HANG_H
#define GIO_HANG_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <iomanip>
#include <utility>

using namespace std;

// ============================================================================
// PHAN 1: HANG SO & CAU TRUC DU LIEU (CONSTANTS & DATA STRUCTURES)
// ============================================================================

const int MAX_CATALOG = 5000;
const int HASH_SIZE = 11;

// 1.1. San pham trong kho & Mon hang trong gio
struct Product {
    int id;             // Ma san pham (ProductID)
    string name;        // Ten san pham
    double price;       // Don gia
    int stock;          // So luong ton kho
    double discount;    // Giam gia truc tiep tren san pham
};

struct CartItem {
    int productId;
    string name;
    double unitPrice;   // Don gia sau khi tru discount
    int quantity;
    double subtotal() const { return unitPrice * quantity; }
};

// 1.2. Danh sach lien ket doi (Quan ly Gio hang - TV5)
struct CartNode {
    CartItem data;
    CartNode* prev;     // Con tro toi nut dung truoc
    CartNode* next;     // Con tro toi nut dung sau
};

struct CartLinkedList {
    CartNode* head;     // Con tro dau danh sach
    CartNode* tail;     // Con tro cuoi danh sach
};

// 1.3. Ngan xep - Stack (Phuc vu thao tac UNDO - TV6)
enum ActionType { ACT_ADD, ACT_REMOVE, ACT_UPDATE, ACT_CLEAR };

struct UndoAction {
    ActionType type;
    CartItem itemState;
    int oldQuantity;
};

struct StackNode {
    UndoAction data;
    StackNode* next;
};

struct UndoStack {
    StackNode* top;
};

// 1.4. Hang doi - Queue (Quan ly Don hang Checkout - TV6)
struct Order {
    int orderId;
    string customerName;
    double finalTotal;
};

struct QueueNode {
    Order data;
    QueueNode* next;
};

struct OrderQueue {
    QueueNode* front;
    QueueNode* rear;
};

// 1.5. Cay nhi phan tim kiem can bang (AVL Tree - TV4)
struct AVLNode {
    Product data;
    AVLNode* left;
    AVLNode* right;
    int height;
};

struct AVLTree {
    AVLNode* root;
};

// 1.6. Bang bam - Hash Table (Quan ly Voucher - TV6)
struct VoucherNode {
    int voucherKey;
    string codeName;
    double discountValue;
    VoucherNode* next;
};

struct VoucherHashTable {
    VoucherNode* buckets[HASH_SIZE];
};

// ============================================================================
// PHAN 2: MENU & NHAP LIEU (main.cpp - TV1)
// ============================================================================

void showMainMenu(const string& csvSortedFile);
void showSortMenu();
void showSearchMenu();
void showCartSubMenu();
int inputInt(const string& prompt);
string inputString(const string& prompt);

// ============================================================================
// PHAN 3: KHO HANG & DOC/GHI CSV (catalog.cpp - TV2)
// ============================================================================

void initSampleCSV(const string& filename);
int loadCatalogFromCSV(const string& filename, Product catalog[], int maxCap, AVLTree& avlIndex);
bool saveCatalogToCSV(const string& filename, Product catalog[], int n);
void printCatalog(Product catalog[], int n, int previewLimit = 20);
int findProductIndexByID(Product catalog[], int n, int productId);

// ============================================================================
// PHAN 4: SAP XEP MANG (sort.cpp - TV3)
// ============================================================================

void quickSortByID(Product catalog[], int low, int high);
void heapSortByPrice(Product catalog[], int n);
void insertionSortByStock(Product catalog[], int n);

// ============================================================================
// PHAN 5: TIM KIEM & CAY AVL (search.cpp, avl.cpp - TV4)
// ============================================================================

// 5.1. Cac thuat toan tim kiem (search.cpp)
// Yeu cau tien quyet cho binarySearchByID: mang a[0..n-1] da sap tang dan theo id
int binarySearchByID(Product a[], int n, int id);
int sentinelSearchByID(Product a[], int n, int id);
int sequentialSearchByName(Product a[], int n, const string& keyword, int result[], int maxResult);

// 5.2. Cay AVL (avl.cpp)
void initAVL(AVLTree& t);
int heightAVL(AVLNode* node);
int balanceFactor(AVLNode* node);
void updateHeight(AVLNode* node);
AVLNode* createAVLNode(Product val);
AVLNode* rotateRight(AVLNode* node);
AVLNode* rotateLeft(AVLNode* node);
AVLNode* rotateLR(AVLNode* node);
AVLNode* rotateRL(AVLNode* node);
AVLNode* rebalance(AVLNode* node);
AVLNode* addAVLNode(AVLNode* node, Product val);
AVLNode* searchAVL(AVLNode* node, int productId);
void inOrderAVL(AVLNode* root, int& printedCount, int maxPrint);
void destroyAVLNode(AVLNode* node);

// ============================================================================
// PHAN 6: GIO HANG DSLK DOI & MERGE SORT (cart.cpp - TV5)
// ============================================================================

void initCart(CartLinkedList& list);
CartNode* createCartNode(CartItem x);
void insertTailCart(CartLinkedList& list, CartItem x);
CartNode* findCartNode(CartLinkedList& list, int productId);
void deleteHeadCart(CartLinkedList& list);
void deleteTailCart(CartLinkedList& list);
bool removeProductFromCart(CartLinkedList& list, int productId, CartItem& removedItem);
void clearCart(CartLinkedList& list);
void viewCart(CartLinkedList& cart, double voucherDiscount);
void sortCartBySubtotal(CartLinkedList& cart);

// ============================================================================
// PHAN 7: STACK, QUEUE & HASH TABLE (undo_stack.cpp, order_queue.cpp, voucher_hash.cpp - TV6)
// ============================================================================

// 7.1. Stack (UNDO)
void initStack(UndoStack& s);
bool isEmptyStack(UndoStack& s);
void pushStack(UndoStack& s, UndoAction val);
UndoAction topStack(UndoStack& s);
void popStack(UndoStack& s);
void destroyStack(UndoStack& s);

// 7.2. Queue (Checkout)
void initQueue(OrderQueue& q);
bool isEmptyQueue(OrderQueue& q);
void enqueueOrder(OrderQueue& q, Order val);
Order dequeueOrder(OrderQueue& q);
void destroyQueue(OrderQueue& q);

// 7.3. Hash Table (Voucher)
void initHashTable(VoucherHashTable& ht);
int hashFunctionDivision(int key);
void insertVoucher(VoucherHashTable& ht, int key, string codeName, double value);
VoucherNode* searchVoucher(VoucherHashTable& ht, int key);
void displayVouchers(VoucherHashTable& ht);
void destroyHashTable(VoucherHashTable& ht);

#endif // GIO_HANG_H
