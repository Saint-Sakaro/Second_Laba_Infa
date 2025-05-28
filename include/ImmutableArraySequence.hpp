#pragma once
#include <utility>
#include "Sequence.hpp"
#include "DynamicArray.hpp"
#include "ArraySequence.hpp"

template <typename T>
class ImmutableArraySequence : public ArraySequence<T> {
private:
    using ArraySequence<T>::array;

public:
    ImmutableArraySequence() = default;
    ImmutableArraySequence(const T* items, int count) : ArraySequence<T>(items, count) {}
    ImmutableArraySequence(const DynamicArray<T>& other) : ArraySequence<T>(other) {}
    ImmutableArraySequence(const ArraySequence<T>& other) : ArraySequence<T>(other) {}
    ImmutableArraySequence(const ImmutableArraySequence<T>& other) : ArraySequence<T>(other) {}

    void Append(const T& item) override {
        throw InvalidOperationException("Cannot modify immutable sequence");
    }

    void Prepend(const T& item) override {
        throw InvalidOperationException("Cannot modify immutable sequence");
    }

    void InsertAt(const T& item, int index) override {
        throw InvalidOperationException("Cannot modify immutable sequence");
    }

    ImmutableArraySequence<T>* AppendNew(const T& item) const {
        ImmutableArraySequence<T>* result = new ImmutableArraySequence<T>(*this);
        result->array.Resize(array.GetSize() + 1);
        result->array.Set(array.GetSize(), item);
        return result;
    }

    ImmutableArraySequence<T>* PrependNew(const T& item) const {
        ImmutableArraySequence<T>* result = new ImmutableArraySequence<T>();
        result->array.Resize(array.GetSize() + 1);
        result->array.Set(0, item);
        for (int i = 0; i < array.GetSize(); ++i) {
            result->array.Set(i + 1, array.Get(i));
        }
        return result;
    }

    ImmutableArraySequence<T>* InsertAtNew(const T& item, int index) const {
        if (index < 0 || index > array.GetSize()) {
            throw IndexOutOfRangeException("Invalid insert index");
        }
        ImmutableArraySequence<T>* result = new ImmutableArraySequence<T>();
        result->array.Resize(array.GetSize() + 1);
        
        for (int i = 0; i < index; ++i) {
            result->array.Set(i, array.Get(i));
        }
        result->array.Set(index, item);
        for (int i = index; i < array.GetSize(); ++i) {
            result->array.Set(i + 1, array.Get(i));
        }
        return result;
    }

    Sequence<T>* Map(T (*func)(const T&)) const override {
        ImmutableArraySequence<T>* result = new ImmutableArraySequence<T>();
        for (int i = 0; i < array.GetSize(); ++i) {
            result = static_cast<ImmutableArraySequence<T>*>(result->AppendNew(func(array.Get(i))));
        }
        return result;
    }

    Sequence<T>* Where(bool (*predicate)(const T&)) const override {
        ImmutableArraySequence<T>* result = new ImmutableArraySequence<T>();
        for (int i = 0; i < array.GetSize(); ++i) {
            T current = array.Get(i);
            if (predicate(current)) {
                result = static_cast<ImmutableArraySequence<T>*>(result->AppendNew(current));
            }
        }
        return result;
    }

    Sequence<T>* Slice(int i, int N, const Sequence<T>* s = nullptr) const override {
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
        
        ImmutableArraySequence<T>* result = new ImmutableArraySequence<T>();
        
        for (int j = 0; j < i; ++j) {
            result = static_cast<ImmutableArraySequence<T>*>(result->AppendNew(array.Get(j)));
        }
        
        if (s != nullptr) {
            for (int j = 0; j < s->GetLength(); ++j) {
                result = static_cast<ImmutableArraySequence<T>*>(result->AppendNew(s->Get(j)));
            }
        }
        
        for (int j = i + N; j < length; ++j) {
            result = static_cast<ImmutableArraySequence<T>*>(result->AppendNew(array.Get(j)));
        }
        
        return result;
    }

    Sequence<T>* FlatMap(Sequence<T>* (*func)(const T&)) const override {
        ImmutableArraySequence<T>* result = new ImmutableArraySequence<T>();
        for (int i = 0; i < array.GetSize(); ++i) {
            Sequence<T>* subseq = func(array.Get(i));
            for (int j = 0; j < subseq->GetLength(); ++j) {
                result = static_cast<ImmutableArraySequence<T>*>(result->AppendNew(subseq->Get(j)));
            }
            delete subseq;
        }
        return result;
    }

    std::pair<Sequence<T>*, Sequence<T>*> Split(bool (*predicate)(const T&)) const override {
        auto* trueSeq = new ImmutableArraySequence<T>();
        auto* falseSeq = new ImmutableArraySequence<T>();
        
        for (int i = 0; i < array.GetSize(); i++) {
            if (predicate(array.Get(i))) {
                trueSeq = static_cast<ImmutableArraySequence<T>*>(trueSeq->AppendNew(array.Get(i)));
            } else {
                falseSeq = static_cast<ImmutableArraySequence<T>*>(falseSeq->AppendNew(array.Get(i)));
            }
        }
        return std::make_pair(trueSeq, falseSeq);
    }

    Sequence<T>* Concat(const Sequence<T>* other) const override {
        ImmutableArraySequence<T>* result = new ImmutableArraySequence<T>();
        
        for (int i = 0; i < array.GetSize(); ++i) {
            result = static_cast<ImmutableArraySequence<T>*>(result->AppendNew(array.Get(i)));
        }
    
        for (int i = 0; i < other->GetLength(); ++i) {
            result = static_cast<ImmutableArraySequence<T>*>(result->AppendNew(other->Get(i)));
        }
        
        return result;
    }
};

