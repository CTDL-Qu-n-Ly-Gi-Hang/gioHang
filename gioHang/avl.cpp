#include "gioHang.h"

void initAVL(AVLTree& t) {
    t.root = nullptr;
}

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