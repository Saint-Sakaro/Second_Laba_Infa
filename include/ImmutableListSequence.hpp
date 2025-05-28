#pragma once
#include <utility>
#include "Sequence.hpp"
#include "LinkedList.hpp"
#include "ListSequence.hpp"

template <typename T>
class ImmutableListSequence : public ListSequence<T> {
private:
    using ListSequence<T>::list;

public:
    ImmutableListSequence() = default;
    ImmutableListSequence(const T* items, int count) : ListSequence<T>(items, count) {}
    ImmutableListSequence(const LinkedList<T>& other) : ListSequence<T>(other) {}
    ImmutableListSequence(const ListSequence<T>& other) : ListSequence<T>(other) {}
    ImmutableListSequence(const ImmutableListSequence<T>& other) : ListSequence<T>(other) {}

    void Append(const T& item) override {
        throw InvalidOperationException("Cannot modify immutable sequence");
    }

    void Prepend(const T& item) override {
        throw InvalidOperationException("Cannot modify immutable sequence");
    }

    void InsertAt(const T& item, int index) override {
        throw InvalidOperationException("Cannot modify immutable sequence");
    }

    ImmutableListSequence<T>* AppendNew(const T& item) const {
        ImmutableListSequence<T>* result = new ImmutableListSequence<T>(*this);
        result->list.Append(item);
        return result;
    }

    ImmutableListSequence<T>* PrependNew(const T& item) const {
        ImmutableListSequence<T>* result = new ImmutableListSequence<T>(*this);
        result->list.Prepend(item);
        return result;
    }

    ImmutableListSequence<T>* InsertAtNew(const T& item, int index) const {
        if (index < 0 || index > list.GetSize()) {
            throw IndexOutOfRangeException("Invalid insert index");
        }
        ImmutableListSequence<T>* result = new ImmutableListSequence<T>(*this);
        result->list.InsertAt(item, index);
        return result;
    }

    Sequence<T>* Map(T (*func)(const T&)) const override {
        ImmutableListSequence<T>* result = new ImmutableListSequence<T>();
        for (int i = 0; i < list.GetSize(); ++i) {
            result = static_cast<ImmutableListSequence<T>*>(result->AppendNew(func(list.Get(i))));
        }
        return result;
    }

    Sequence<T>* Where(bool (*predicate)(const T&)) const override {
        ImmutableListSequence<T>* result = new ImmutableListSequence<T>();
        for (int i = 0; i < list.GetSize(); ++i) {
            T current = list.Get(i);
            if (predicate(current)) {
                result = static_cast<ImmutableListSequence<T>*>(result->AppendNew(current));
            }
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
        
        ImmutableListSequence<T>* result = new ImmutableListSequence<T>();
        
        for (int j = 0; j < i; ++j) {
            result = static_cast<ImmutableListSequence<T>*>(result->AppendNew(list.Get(j)));
        }
        
        if (s != nullptr) {
            for (int j = 0; j < s->GetLength(); ++j) {
                result = static_cast<ImmutableListSequence<T>*>(result->AppendNew(s->Get(j)));
            }
        }

        for (int j = i + N; j < length; ++j) {
            result = static_cast<ImmutableListSequence<T>*>(result->AppendNew(list.Get(j)));
        }
        
        return result;
    }

    Sequence<T>* FlatMap(Sequence<T>* (*func)(const T&)) const override {
        ImmutableListSequence<T>* result = new ImmutableListSequence<T>();
        for (int i = 0; i < list.GetSize(); ++i) {
            Sequence<T>* subseq = func(list.Get(i));
            for (int j = 0; j < subseq->GetLength(); ++j) {
                result = static_cast<ImmutableListSequence<T>*>(result->AppendNew(subseq->Get(j)));
            }
            delete subseq;
        }
        return result;
    }

    std::pair<Sequence<T>*, Sequence<T>*> Split(bool (*predicate)(const T&)) const override {
        ImmutableListSequence<T>* matching = new ImmutableListSequence<T>();
        ImmutableListSequence<T>* notMatching = new ImmutableListSequence<T>();

        for (int i = 0; i < list.GetSize(); ++i) {
            T current = list.Get(i);
            if (predicate(current)) {
                matching = static_cast<ImmutableListSequence<T>*>(matching->AppendNew(current));
            } else {
                notMatching = static_cast<ImmutableListSequence<T>*>(notMatching->AppendNew(current));
            }
        }

        return std::make_pair(matching, notMatching);
    }

    Sequence<T>* Concat(const Sequence<T>* other) const override {
        ImmutableListSequence<T>* result = new ImmutableListSequence<T>();
        
        for (int i = 0; i < list.GetSize(); ++i) {
            result = static_cast<ImmutableListSequence<T>*>(result->AppendNew(list.Get(i)));
        }
        
        for (int i = 0; i < other->GetLength(); ++i) {
            result = static_cast<ImmutableListSequence<T>*>(result->AppendNew(other->Get(i)));
        }
        
        return result;
    }
};
