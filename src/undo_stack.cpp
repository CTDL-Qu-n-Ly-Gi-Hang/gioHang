#include "gioHang.h"

void initStack(UndoStack& s) {
    s.top = nullptr;
}

bool isEmptyStack(UndoStack& s) {
    return s.top == nullptr;
}

void pushStack(UndoStack& s, UndoAction val) {
    StackNode* newNode = new StackNode;
    newNode->data = val;
    newNode->next = s.top;
    s.top = newNode;
}

UndoAction topStack(UndoStack& s) {
    return s.top->data;
}

void popStack(UndoStack& s) {
    if (isEmptyStack(s)) return;
    StackNode* temp = s.top;
    s.top = s.top->next;
    delete temp;
}

void destroyStack(UndoStack& s) {
    while (!isEmptyStack(s)) {
        popStack(s);
    }
}
