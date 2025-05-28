#pragma once
#include "Exceptions.hpp"

template <typename T>
struct Node {
    T data;
    Node* next;
    Node(const T& item) : data(item), next(nullptr) {}
};

template <typename T>
class LinkedList {
private:
    Node<T>* head;
    int size;

public:
    LinkedList() : head(nullptr), size(0) {}
    LinkedList(const T* items, int count) : head(nullptr), size(0) {
        if (count < 0) {
            throw InvalidSizeException("Count cannot be negative");
        }
        for (int i = 0; i < count; i++) {
            Append(items[i]);
        }
    }
    // from
    LinkedList(const LinkedList<T>& other) : head(nullptr), size(0) {
        Node<T>* current = other.head;
        while (current) {
            Append(current->data);
            current = current->next;
        }
    }

    LinkedList& operator=(const LinkedList<T>& other) {
        if (this != &other) {
            Clear();
            Node<T>* current = other.head;
            while (current) {
                Append(current->data);
                current = current->next;
            }
        }
        return *this;
    }

    ~LinkedList() {
        Clear();
    }

    T Get(int index) const {
        if (index < 0 || index >= size) {
            throw IndexOutOfRangeException("Index out of range");
        }
        Node<T>* current = head;
        for (int i = 0; i < index; ++i) {
            current = current->next;
        }
        return current->data;
    }

    T GetFirst() const {
        if (size == 0) {
            throw EmptySequenceException();
        }
        return head->data;
    }

    T GetLast() const {
        if (size == 0) {
            throw EmptySequenceException();
        }
        Node<T>* current = head;
        while (current->next) {
            current = current->next;
        }
        return current->data;
    }

    int GetSize() const {
        return size;
    }

    void Append(const T& item) {
        Node<T>* newNode = new Node<T>(item);
        if (!head) {
            head = newNode;
        } else {
            Node<T>* current = head;
            while (current->next) {
                current = current->next;
            }
            current->next = newNode;
        }
        ++size;
    }

    void Prepend(const T& item) {
        Node<T>* newNode = new Node<T>(item);
        newNode->next = head;
        head = newNode;
        ++size;
    }

    void InsertAt(const T& item, int index) {
        if (index < 0 || index > size) {
            throw IndexOutOfRangeException("Invalid insert index");
        }
        if (index == 0) {
            Prepend(item);
            return;
        }
        Node<T>* newNode = new Node<T>(item);
        Node<T>* current = head;
        for (int i = 0; i < index - 1; ++i) {
            current = current->next;
        }
        newNode->next = current->next;
        current->next = newNode;
        ++size;
    }

    void Clear() {
        while (head) {
            Node<T>* temp = head;
            head = head->next;
            delete temp;
        }
        size = 0;
    }
};