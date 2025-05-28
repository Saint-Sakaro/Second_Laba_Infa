#pragma once
#include <utility>
#include "Sequence.hpp"
#include "DynamicArray.hpp"
#include "Exceptions.hpp"

template <typename T>
class ArraySequence : public Sequence<T> {
protected:
    DynamicArray<T> array;

private:
    class ArraySequenceEnumerator : public IEnumerator<T> {
    private:
        const DynamicArray<T>& array;
        int currentIndex;
        T currentValue;

    public:
        explicit ArraySequenceEnumerator(const DynamicArray<T>& array) 
            : array(array), currentIndex(-1) {}

        bool MoveNext() override {
            if (currentIndex + 1 < array.GetSize()) {
                currentIndex++;
                currentValue = array.Get(currentIndex);
                return true;
            }
            return false;
        }

        const T& Current() const override {
            if (currentIndex < 0 || currentIndex >= array.GetSize()) {
                throw InvalidStateException("Enumerator is not in a valid position");
            }
            return currentValue;
        }

        void Reset() override {
            currentIndex = -1;
        }
    };

public:
    ArraySequence() = default;
    ArraySequence(T* items, int count) : array(items, count) {}
    ArraySequence(const T* items, int count) {
        T* nonConstItems = new T[count];
        for (int i = 0; i < count; ++i) {
            nonConstItems[i] = items[i];
        }
        array = DynamicArray<T>(nonConstItems, count);
        delete[] nonConstItems;
    }
    ArraySequence(const DynamicArray<T>& other) : array(other) {}
    // from
    ArraySequence(const ArraySequence<T>& other) : array(other.array) {}

    T Get(int index) const override {
        return array.Get(index);
    }

    T GetFirst() const override {
        if (array.GetSize() == 0) {
            throw EmptySequenceException();
        }
        return array.Get(0);
    }

    T GetLast() const override {
        if (array.GetSize() == 0) {
            throw EmptySequenceException();
        }
        return array.Get(array.GetSize() - 1);
    }

    Option<T> TryGet(int index) const override {
        if (index < 0 || index >= array.GetSize()) {
            return Option<T>::None();
        }
        return Option<T>::Some(array.Get(index));
    }

    Option<T> TryGetFirst() const override {
        if (array.GetSize() == 0) {
            return Option<T>::None();
        }
        return Option<T>::Some(array.Get(0));
    }

    Option<T> TryGetLast() const override {
        if (array.GetSize() == 0) {
            return Option<T>::None();
        }
        return Option<T>::Some(array.Get(array.GetSize() - 1));
    }

    Sequence<T>* GetSubsequence(int startIndex, int endIndex) const override {
        if (startIndex < 0 || endIndex >= array.GetSize() || startIndex > endIndex) {
            throw IndexOutOfRangeException("Invalid subsequence indices");
        }
        int length = endIndex - startIndex + 1;
        T* items = new T[length];
        for (int i = 0; i < length; ++i) {
            items[i] = array.Get(startIndex + i);
        }
        ArraySequence<T>* result = new ArraySequence<T>(items, length);
        delete[] items;
        return result;
    }

    int GetLength() const override {
        return array.GetSize();
    }

    void Append(const T& item) override {
        int oldSize = array.GetSize();
        array.Resize(oldSize + 1);
        array.Set(oldSize, item);
    }

    void Prepend(const T& item) override {
        int oldSize = array.GetSize();
        array.Resize(oldSize + 1);
        for (int i = oldSize; i > 0; --i) {
            array.Set(i, array.Get(i - 1));
        }
        array.Set(0, item);
    }

    void InsertAt(const T& item, int index) override {
        if (index < 0 || index > array.GetSize()) {
            throw IndexOutOfRangeException("Invalid insert index");
        }
        int oldSize = array.GetSize();
        array.Resize(oldSize + 1);
        for (int i = oldSize; i > index; --i) {
            array.Set(i, array.Get(i - 1));
        }
        array.Set(index, item);
    }

    Sequence<T>* Map(T (*func)(const T&)) const override {
        ArraySequence<T>* result = new ArraySequence<T>();
        for (int i = 0; i < array.GetSize(); ++i) {
            result->Append(func(array.Get(i)));
        }
        return result;
    }

    Sequence<T>* Where(bool (*predicate)(const T&)) const override {
        ArraySequence<T>* result = new ArraySequence<T>();
        for (int i = 0; i < array.GetSize(); ++i) {
            T current = array.Get(i);
            if (predicate(current)) {
                result->Append(current);
            }
        }
        return result;
    }

    T Reduce(T (*func)(const T&, const T&), const T& initial) const override {
        T result = initial;
        for (int i = 0; i < array.GetSize(); ++i) {
            result = func(result, array.Get(i));
        }
        return result;
    }

    Sequence<T>* Slice(int i, int N, const Sequence<T>* s = nullptr) const override { // Slice: удаляет N элементов начиная с позиции i и вставляет элементы из последовательности s
        int length = array.GetSize();
    
        if (i < 0) {
            i = length + i;
        }
        if (i < 0 || i >= length) {
            throw IndexOutOfRangeException("Invalid slice index");
        }
        if (i + N > length) {
            N = length - i;
        }
        
        ArraySequence<T>* result = new ArraySequence<T>();
        
        for (int j = 0; j < i; ++j) {
            result->Append(array.Get(j));
        }
        
        if (s != nullptr) {
            for (int j = 0; j < s->GetLength(); ++j) {
                result->Append(s->Get(j));
            }
        }

        for (int j = i + N; j < length; ++j) {
            result->Append(array.Get(j));
        }
        
        return result;
    }

    Sequence<T>* FlatMap(Sequence<T>* (*func)(const T&)) const override {
        ArraySequence<T>* result = new ArraySequence<T>();
        for (int i = 0; i < array.GetSize(); ++i) {
            Sequence<T>* subseq = func(array.Get(i));
            for (int j = 0; j < subseq->GetLength(); ++j) {
                result->Append(subseq->Get(j));
            }
            delete subseq;
        }
        return result;
    }

    Option<T> Find(bool (*predicate)(const T&)) const override {
        for (int i = 0; i < array.GetSize(); ++i) {
            if (predicate(array.Get(i))) {
                return Option<T>::Some(array.Get(i));
            }
        }
        return Option<T>::None();
    }

    std::pair<Sequence<T>*, Sequence<T>*> Split(bool (*predicate)(const T&)) const override {
        ArraySequence<T>* matching = new ArraySequence<T>();
        ArraySequence<T>* notMatching = new ArraySequence<T>();

        for (int i = 0; i < array.GetSize(); ++i) {
            T current = array.Get(i);
            if (predicate(current)) {
                matching->Append(current);
            } else {
                notMatching->Append(current);
            }
        }

        return std::make_pair(matching, notMatching);
    }

    Sequence<T>* Concat(const Sequence<T>* other) const override {
        ArraySequence<T>* result = new ArraySequence<T>();
        
        for (int i = 0; i < array.GetSize(); ++i) {
            result->Append(array.Get(i));
        }
        
        for (int i = 0; i < other->GetLength(); ++i) {
            result->Append(other->Get(i));
        }
        
        return result;
    }

    IEnumerator<T>* GetEnumerator() const override {
        return new ArraySequenceEnumerator(array);
    }
};