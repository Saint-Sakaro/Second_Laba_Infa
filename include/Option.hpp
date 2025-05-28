#pragma once
#include "Exceptions.hpp"

template <typename T>
class Option {
private:
    bool hasValue;
    T value;

public:
    Option() : hasValue(false) {}
    Option(const T& val) : hasValue(true), value(val) {}
    // from
    Option(const Option& other) = default;

    Option& operator=(const Option& other) = default;

    bool isSome() const { return hasValue; }
    bool isNone() const { return !hasValue; }

    T getValue() const {
        if (!hasValue) {
            throw InvalidArgumentException("Attempt to get value from None");
        }
        return value;
    }

    static Option None() { return Option(); }
    static Option Some(const T& val) { return Option(val); }
};