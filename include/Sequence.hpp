#pragma once
#include <utility>
#include "Option.hpp"
#include "IEnumerable.hpp"

template<typename T>
class Sequence : public IEnumerable<T> {
public:
    virtual ~Sequence() = default;
    
    virtual T Get(int index) const = 0;
    virtual T GetFirst() const = 0;
    virtual T GetLast() const = 0;
    
    virtual Option<T> TryGet(int index) const = 0;
    virtual Option<T> TryGetFirst() const = 0;
    virtual Option<T> TryGetLast() const = 0;
    
    virtual int GetLength() const = 0;
    virtual Sequence<T>* GetSubsequence(int startIndex, int endIndex) const = 0;
    
    virtual void Append(const T& item) = 0;
    virtual void Prepend(const T& item) = 0;
    virtual void InsertAt(const T& item, int index) = 0;
    
    virtual Sequence<T>* Map(T (*func)(const T&)) const = 0;
    virtual Sequence<T>* Where(bool (*predicate)(const T&)) const = 0;
    virtual T Reduce(T (*func)(const T&, const T&), const T& initialValue) const = 0;
    virtual Sequence<T>* FlatMap(Sequence<T>* (*func)(const T&)) const = 0;
    virtual Option<T> Find(bool (*predicate)(const T&)) const = 0;
    virtual std::pair<Sequence<T>*, Sequence<T>*> Split(bool (*predicate)(const T&)) const = 0;
    virtual Sequence<T>* Concat(const Sequence<T>* other) const = 0;
    virtual Sequence<T>* Slice(int i, int N, const Sequence<T>* s = nullptr) const = 0;
};