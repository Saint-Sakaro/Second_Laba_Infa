#pragma once
#include <utility>
#include "Sequence.hpp"
#include "LinkedList.hpp"
#include "Exceptions.hpp"

template <typename T>
class ListSequence : public Sequence<T> {
protected:
    LinkedList<T> list;

private:
    class LinkedListEnumerator : public IEnumerator<T> {
    private:
        const LinkedList<T>& list;
        int currentIndex;
        T currentValue;
        bool isBeforeFirst;

    public:
        explicit LinkedListEnumerator(const LinkedList<T>& list) 
            : list(list), currentIndex(-1), isBeforeFirst(true) {}

        bool MoveNext() override {
            if (currentIndex + 1 < list.GetSize()) {
                currentIndex++;
                currentValue = list.Get(currentIndex);
                isBeforeFirst = false;
                return true;
            }
            return false;
        }

        const T& Current() const override {
            if (isBeforeFirst || currentIndex >= list.GetSize()) {
                throw InvalidStateException("Enumerator is not in a valid position");
            }
            return currentValue;
        }

        void Reset() override {
            currentIndex = -1;
            isBeforeFirst = true;
        }
    };

public:
    ListSequence() = default;
    ListSequence(T* items, int count) : list(items, count) {}
    ListSequence(const T* items, int count) {
        T* nonConstItems = new T[count];
        for (int i = 0; i < count; ++i) {
            nonConstItems[i] = items[i];
        }
        list = LinkedList<T>(nonConstItems, count);
        delete[] nonConstItems;
    }
    ListSequence(const LinkedList<T>& other) : list(other) {}
    // from
    ListSequence(const ListSequence<T>& other) : list(other.list) {}

    T Get(int index) const override {
        return list.Get(index);
    }

    T GetFirst() const override {
        return list.GetFirst();
    }

    T GetLast() const override {
        return list.GetLast();
    }

    Option<T> TryGet(int index) const override {
        try {
            return Option<T>::Some(list.Get(index));
        } catch (const IndexOutOfRangeException&) {
            return Option<T>::None();
        }
    }

    Option<T> TryGetFirst() const override {
        try {
            return Option<T>::Some(list.GetFirst());
        } catch (const EmptySequenceException&) {
            return Option<T>::None();
        }
    }

    Option<T> TryGetLast() const override {
        try {
            return Option<T>::Some(list.GetLast());
        } catch (const EmptySequenceException&) {
            return Option<T>::None();
        }
    }

    Sequence<T>* GetSubsequence(int startIndex, int endIndex) const override {
        if (startIndex < 0 || endIndex >= list.GetSize() || startIndex > endIndex) {
            throw IndexOutOfRangeException("Invalid subsequence indices");
        }
        int length = endIndex - startIndex + 1;
        T* items = new T[length];
        for (int i = 0; i < length; ++i) {
            items[i] = list.Get(startIndex + i);
        }
        ListSequence<T>* result = new ListSequence<T>(items, length);
        delete[] items;
        return result;
    }

    int GetLength() const override {
        return list.GetSize();
    }

    void Append(const T& item) override {
        list.Append(item);
    }

    void Prepend(const T& item) override {
        list.Prepend(item);
    }

    void InsertAt(const T& item, int index) override {
        list.InsertAt(item, index);
    }

    Sequence<T>* Map(T (*func)(const T&)) const override {
        ListSequence<T>* result = new ListSequence<T>();
        for (int i = 0; i < list.GetSize(); ++i) {
            result->Append(func(list.Get(i)));
        }
        return result;
    }

    Sequence<T>* Where(bool (*predicate)(const T&)) const override {
        ListSequence<T>* result = new ListSequence<T>();
        for (int i = 0; i < list.GetSize(); ++i) {
            T item = list.Get(i);
            if (predicate(item)) {
                result->Append(item);
            }
        }
        return result;
    }

    T Reduce(T (*func)(const T&, const T&), const T& initial) const override {
        T result = initial;
        for (int i = 0; i < list.GetSize(); ++i) {
            result = func(result, list.Get(i));
        }
        return result;
    }

    Sequence<T>* Slice(int i, int N, const Sequence<T>* s = nullptr) const override {
        int length = list.GetSize();
        
        if (i < 0) {
            i = length + i;
        }
        if (i < 0 || i >= length) {
            throw IndexOutOfRangeException("Invalid slice index");
        }
        if (i + N > length) {
            N = length - i;
        }
        
        ListSequence<T>* result = new ListSequence<T>();
        
        for (int j = 0; j < i; ++j) {
            result->Append(list.Get(j));
        }
        
        if (s != nullptr) {
            for (int j = 0; j < s->GetLength(); ++j) {
                result->Append(s->Get(j));
            }
        }

        for (int j = i + N; j < length; ++j) {
            result->Append(list.Get(j));
        }
        
        return result;
    }

    Sequence<T>* FlatMap(Sequence<T>* (*func)(const T&)) const override {
        ListSequence<T>* result = new ListSequence<T>();
        for (int i = 0; i < list.GetSize(); ++i) {
            Sequence<T>* subseq = func(list.Get(i));
            for (int j = 0; j < subseq->GetLength(); ++j) {
                result->Append(subseq->Get(j));
            }
            delete subseq;
        }
        return result;
    }

    Option<T> Find(bool (*predicate)(const T&)) const override {
        for (int i = 0; i < list.GetSize(); ++i) {
            if (predicate(list.Get(i))) {
                return Option<T>::Some(list.Get(i));
            }
        }
        return Option<T>::None();
    }

    std::pair<Sequence<T>*, Sequence<T>*> Split(bool (*predicate)(const T&)) const override {
        ListSequence<T>* matching = new ListSequence<T>();
        ListSequence<T>* notMatching = new ListSequence<T>();

        for (int i = 0; i < list.GetSize(); ++i) {
            T current = list.Get(i);
            if (predicate(current)) {
                matching->Append(current);
            } else {
                notMatching->Append(current);
            }
        }

        return std::make_pair(matching, notMatching);
    }

    Sequence<T>* Concat(const Sequence<T>* other) const override {
        ListSequence<T>* result = new ListSequence<T>();
        
        for (int i = 0; i < list.GetSize(); ++i) {
            result->Append(list.Get(i));
        }
        
        for (int i = 0; i < other->GetLength(); ++i) {
            result->Append(other->Get(i));
        }
        
        return result;
    }

    IEnumerator<T>* GetEnumerator() const override {
        return new LinkedListEnumerator(list);
    }
};