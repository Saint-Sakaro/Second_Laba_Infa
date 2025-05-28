#pragma once
#include <stdexcept>
#include <string>

class SequenceException : public std::runtime_error {
public:
    SequenceException(const std::string& message)
        : std::runtime_error(message) {}
};

class IndexOutOfRangeException : public SequenceException {
public:
    IndexOutOfRangeException(const std::string& message = "Index out of range")
        : SequenceException(message) {}
};

class EmptySequenceException : public SequenceException {
public:
    EmptySequenceException(const std::string& message = "Sequence is empty")
        : SequenceException(message) {}
};

class InvalidSizeException : public SequenceException {
public:
    InvalidSizeException(const std::string& message = "Invalid size")
        : SequenceException(message) {}
};

class InvalidArgumentException : public SequenceException {
public:
    InvalidArgumentException(const std::string& message = "Invalid argument")
        : SequenceException(message) {}
};

class InvalidOperationException : public SequenceException {
public:
    InvalidOperationException(const std::string& message = "Invalid operation")
        : SequenceException(message) {}
};

class InvalidStateException : public SequenceException {
public:
    explicit InvalidStateException(const std::string& message) : SequenceException(message) {}
};