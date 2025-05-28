#pragma once
#include "Sequence.hpp"
#include "ImmutableArraySequence.hpp"
#include "ImmutableListSequence.hpp"
#include <utility>

template<typename T, typename U>
Sequence<std::pair<T, U>>* Zip(const Sequence<T>& first, const Sequence<U>& second) {
    int minLength = std::min(first.GetLength(), second.GetLength());
    std::pair<T, U>* pairs = new std::pair<T, U>[minLength];
    
    for (int i = 0; i < minLength; ++i) {
        pairs[i] = std::make_pair(first.Get(i), second.Get(i));
    }
    
    auto* result = new ImmutableArraySequence<std::pair<T, U>>(pairs, minLength);
    delete[] pairs;
    return result;
}

template<typename T, typename U>
std::pair<Sequence<T>*, Sequence<U>*> Unzip(const Sequence<std::pair<T, U>>& sequence) {
    int length = sequence.GetLength();
    T* firstItems = new T[length];
    U* secondItems = new U[length];
    
    for (int i = 0; i < length; ++i) {
        const auto& pair = sequence.Get(i);
        firstItems[i] = pair.first;
        secondItems[i] = pair.second;
    }
    
    auto* firstSeq = new ImmutableArraySequence<T>(firstItems, length);
    auto* secondSeq = new ImmutableArraySequence<U>(secondItems, length);
    
    delete[] firstItems;
    delete[] secondItems;
    
    return std::make_pair(firstSeq, secondSeq);
} 