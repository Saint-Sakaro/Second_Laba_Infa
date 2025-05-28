#include <gtest/gtest.h>
#include <utility>
#include "Exceptions.hpp"
#include "Option.hpp"
#include "DynamicArray.hpp"
#include "Sequence.hpp"
#include "ArraySequence.hpp"
#include "LinkedList.hpp"
#include "ListSequence.hpp"
#include "ImmutableArraySequence.hpp"
#include "ImmutableListSequence.hpp"
#include "SequencePairOperations.hpp"

template <typename T>
class MockSequence : public Sequence<T> {
private:
    DynamicArray<T> data;
public:
    MockSequence(const T* items, int size) : data(items, size) {}
    T Get(int index) const override { return data.Get(index); }
    T GetFirst() const override {
        if (data.GetSize() == 0) throw EmptySequenceException();
        return data.Get(0);
    }
    T GetLast() const override {
        if (data.GetSize() == 0) throw EmptySequenceException();
        return data.Get(data.GetSize() - 1);
    }
    Option<T> TryGet(int index) const override {
        if (index < 0 || index >= data.GetSize()) return Option<T>::None();
        return Option<T>::Some(data.Get(index));
    }
    Option<T> TryGetFirst() const override {
        if (data.GetSize() == 0) return Option<T>::None();
        return Option<T>::Some(data.Get(0));
    }
    Option<T> TryGetLast() const override {
        if (data.GetSize() == 0) return Option<T>::None();
        return Option<T>::Some(data.Get(data.GetSize() - 1));
    }
    int GetLength() const override { return data.GetSize(); }
    Sequence<T>* GetSubsequence(int startIndex, int endIndex) const override { return nullptr; }
    void Append(const T& item) override {}
    void Prepend(const T& item) override {}
    void InsertAt(const T& item, int index) override {}
    Sequence<T>* Map(T (*func)(const T&)) const override { return nullptr; }
    Sequence<T>* Where(bool (*predicate)(const T&)) const override { return nullptr; }
    T Reduce(T (*func)(const T&, const T&), const T& initial) const override { return initial; }
    
    // Реализация Slice для MockSequence
    Sequence<T>* Slice(int startIndex, int endIndex, const Sequence<T>* other) const override {
        return nullptr; // Заглушка для тестов
    }

    // Реализация новых методов
    Sequence<T>* FlatMap(Sequence<T>* (*func)(const T&)) const override {
        return nullptr; // Заглушка для тестов
    }

    Option<T> Find(bool (*predicate)(const T&)) const override {
        return Option<T>::None(); // Заглушка для тестов
    }

    std::pair<Sequence<T>*, Sequence<T>*> Split(bool (*predicate)(const T&)) const override {
        return std::make_pair(nullptr, nullptr); // Заглушка для тестов
    }
};

// Тест для исключений
TEST(ExceptionTest, IndexOutOfRangeException) {
    const std::string message = "Custom index error";
    IndexOutOfRangeException ex(message);
    EXPECT_STREQ(ex.what(), message.c_str());
    bool caught = false;
    try {
        throw IndexOutOfRangeException(message);
    } catch (const IndexOutOfRangeException& ex) {
        caught = true;
    }
    EXPECT_TRUE(caught);
}

// Тест для Option<T>
TEST(OptionTest, CreateSomeAndNone) {
    Option<int> some = Option<int>::Some(42);
    EXPECT_TRUE(some.isSome());
    EXPECT_EQ(some.getValue(), 42);
    Option<int> none = Option<int>::None();
    EXPECT_TRUE(none.isNone());
    EXPECT_THROW(none.getValue(), InvalidArgumentException);
}

// Тест для DynamicArray<T>
TEST(DynamicArrayTest, ConstructorFromArray) {
    int data[] = {1, 2, 3};
    DynamicArray<int> arr(data, 3);
    EXPECT_EQ(arr.GetSize(), 3);
    EXPECT_EQ(arr.Get(0), 1);
    EXPECT_EQ(arr.Get(1), 2);
    EXPECT_EQ(arr.Get(2), 3);
    EXPECT_THROW(DynamicArray<int>(data, -1), InvalidSizeException);
}

// Тест для Sequence<T>
TEST(SequenceTest, Get) {
    int data[] = {1, 2, 3};
    MockSequence<int> seq(data, 3);
    EXPECT_EQ(seq.Get(0), 1);
    EXPECT_EQ(seq.Get(1), 2);
    EXPECT_EQ(seq.Get(2), 3);
    EXPECT_THROW(seq.Get(3), IndexOutOfRangeException);
}

// Тесты для ArraySequence<T>
TEST(ArraySequenceTest, ConstructorAndGet) {
    int data[] = {1, 2, 3};
    ArraySequence<int> seq(data, 3);
    EXPECT_EQ(seq.GetLength(), 3);
    EXPECT_EQ(seq.Get(0), 1);
    EXPECT_EQ(seq.Get(1), 2);
    EXPECT_EQ(seq.Get(2), 3);
    EXPECT_THROW(seq.Get(3), IndexOutOfRangeException);
}

TEST(ArraySequenceTest, AppendPrependInsert) {
    ArraySequence<int> seq;
    seq.Append(1);
    EXPECT_EQ(seq.GetLength(), 1);
    EXPECT_EQ(seq.Get(0), 1);
    seq.Prepend(0);
    EXPECT_EQ(seq.GetLength(), 2);
    EXPECT_EQ(seq.Get(0), 0);
    seq.InsertAt(2, 1);
    EXPECT_EQ(seq.GetLength(), 3);
    EXPECT_EQ(seq.Get(1), 2);
    EXPECT_THROW(seq.InsertAt(3, 4), IndexOutOfRangeException);
}

TEST(ArraySequenceTest, ZipTest) {
    int data1[] = {1, 2, 3};
    int data2[] = {4, 5, 6};
    ArraySequence<int> seq1(data1, 3);
    ArraySequence<int> seq2(data2, 3);
    
    auto* result = Zip(seq1, seq2);
    
    EXPECT_EQ(result->GetLength(), 3);
    EXPECT_EQ(result->Get(0).first, 1);
    EXPECT_EQ(result->Get(0).second, 4);
    EXPECT_EQ(result->Get(1).first, 2);
    EXPECT_EQ(result->Get(1).second, 5);
    EXPECT_EQ(result->Get(2).first, 3);
    EXPECT_EQ(result->Get(2).second, 6);
    
    delete result;
}

// Тесты для LinkedList<T>
TEST(LinkedListTest, ConstructorAndGet) {
    int data[] = {1, 2, 3};
    LinkedList<int> list(data, 3);
    EXPECT_EQ(list.GetSize(), 3);
    EXPECT_EQ(list.Get(0), 1);
    EXPECT_EQ(list.Get(1), 2);
    EXPECT_EQ(list.Get(2), 3);
    EXPECT_THROW(list.Get(3), IndexOutOfRangeException);
}

TEST(LinkedListTest, AppendPrependInsert) {
    LinkedList<int> list;
    list.Append(1);
    EXPECT_EQ(list.GetSize(), 1);
    EXPECT_EQ(list.Get(0), 1);
    list.Prepend(0);
    EXPECT_EQ(list.GetSize(), 2);
    EXPECT_EQ(list.Get(0), 0);
    list.InsertAt(2, 1);
    EXPECT_EQ(list.GetSize(), 3);
    EXPECT_EQ(list.Get(1), 2);
    EXPECT_THROW(list.InsertAt(3, 4), IndexOutOfRangeException);
}

// Тесты для ListSequence<T>
TEST(ListSequenceTest, ConstructorAndGet) {
    int data[] = {1, 2, 3};
    ListSequence<int> seq(data, 3);
    EXPECT_EQ(seq.GetLength(), 3);
    EXPECT_EQ(seq.Get(0), 1);
    EXPECT_EQ(seq.Get(1), 2);
    EXPECT_EQ(seq.Get(2), 3);
    EXPECT_THROW(seq.Get(3), IndexOutOfRangeException);
}

TEST(ListSequenceTest, GetFirstAndLast) {
    int data[] = {1, 2, 3};
    ListSequence<int> seq(data, 3);
    EXPECT_EQ(seq.GetFirst(), 1);
    EXPECT_EQ(seq.GetLast(), 3);
    ListSequence<int> empty_seq;
    EXPECT_THROW(empty_seq.GetFirst(), EmptySequenceException);
    EXPECT_THROW(empty_seq.GetLast(), EmptySequenceException);
}

TEST(ListSequenceTest, TryGetMethods) {
    int data[] = {1, 2, 3};
    ListSequence<int> seq(data, 3);
    EXPECT_TRUE(seq.TryGet(1).isSome());
    EXPECT_EQ(seq.TryGet(1).getValue(), 2);
    EXPECT_TRUE(seq.TryGet(3).isNone());
    EXPECT_TRUE(seq.TryGetFirst().isSome());
    EXPECT_EQ(seq.TryGetFirst().getValue(), 1);
    EXPECT_TRUE(seq.TryGetLast().isSome());
    EXPECT_EQ(seq.TryGetLast().getValue(), 3);
    ListSequence<int> empty_seq;
    EXPECT_TRUE(empty_seq.TryGetFirst().isNone());
}

// Функции для тестирования Map, Where, Reduce
int square(const int& x) { return x * x; }
bool isPositive(const int& x) { return x > 0; }
int add(const int& a, const int& b) { return a + b; }

TEST(ListSequenceTest, Map) {
    int data[] = {1, 2, 3};
    ListSequence<int> seq(data, 3);
    Sequence<int>* result = seq.Map(square);
    EXPECT_EQ(result->GetLength(), 3);
    EXPECT_EQ(result->Get(0), 1);
    EXPECT_EQ(result->Get(1), 4);
    EXPECT_EQ(result->Get(2), 9);
    delete result;
}

TEST(ListSequenceTest, Where) {
    int data[] = {-1, 2, -3, 4};
    ListSequence<int> seq(data, 4);
    Sequence<int>* result = seq.Where(isPositive);
    EXPECT_EQ(result->GetLength(), 2);
    EXPECT_EQ(result->Get(0), 2);
    EXPECT_EQ(result->Get(1), 4);
    delete result;
}

TEST(ListSequenceTest, Reduce) {
    int data[] = {1, 2, 3};
    ListSequence<int> seq(data, 3);
    int result = seq.Reduce(add, 0);
    EXPECT_EQ(result, 6); // 0 + 1 + 2 + 3
}

TEST(ListSequenceTest, ZipTest) {
    int data1[] = {1, 2, 3};
    int data2[] = {4, 5, 6};
    ListSequence<int> seq1(data1, 3);
    ListSequence<int> seq2(data2, 3);
    
    auto* result = Zip(seq1, seq2);
    
    EXPECT_EQ(result->GetLength(), 3);
    EXPECT_EQ(result->Get(0).first, 1);
    EXPECT_EQ(result->Get(0).second, 4);
    EXPECT_EQ(result->Get(1).first, 2);
    EXPECT_EQ(result->Get(1).second, 5);
    EXPECT_EQ(result->Get(2).first, 3);
    EXPECT_EQ(result->Get(2).second, 6);
    
    delete result;
}

// Тесты для ImmutableArraySequence
TEST(ImmutableArraySequenceTest, BasicOperations) {
    int data[] = {1, 2, 3, 4, 5};
    ImmutableArraySequence<int> seq(data, 5);
    
    // Проверка базовых операций
    EXPECT_EQ(seq.GetLength(), 5);
    EXPECT_EQ(seq.Get(0), 1);
    EXPECT_EQ(seq.GetFirst(), 1);
    EXPECT_EQ(seq.GetLast(), 5);
    
    // Проверка иммутабельности при попытке модификации
    EXPECT_THROW(seq.Append(6), InvalidOperationException);
    EXPECT_THROW(seq.Prepend(0), InvalidOperationException);
    EXPECT_THROW(seq.InsertAt(6, 2), InvalidOperationException);
    
    // Проверка создания новых последовательностей
    ImmutableArraySequence<int>* newSeq = seq.AppendNew(6);
    EXPECT_EQ(newSeq->GetLength(), 6);
    EXPECT_EQ(newSeq->GetLast(), 6);
    EXPECT_EQ(seq.GetLength(), 5); // Оригинальная последовательность не изменилась
    delete newSeq;
}

// Тесты для ImmutableListSequence
TEST(ImmutableListSequenceTest, BasicOperations) {
    int data[] = {1, 2};
    ImmutableListSequence<int> seq(data, 2);
    
    // Проверка базовых операций
    EXPECT_EQ(seq.GetLength(), 2);
    EXPECT_EQ(seq.Get(0), 1);
    EXPECT_EQ(seq.GetFirst(), 1);
    EXPECT_EQ(seq.GetLast(), 2);
    
    // Проверка иммутабельности при попытке модификации
    EXPECT_THROW(seq.Append(3), InvalidOperationException);
    EXPECT_THROW(seq.Prepend(0), InvalidOperationException);
    EXPECT_THROW(seq.InsertAt(3, 1), InvalidOperationException);
    
    // Проверка создания новых последовательностей
    ImmutableListSequence<int>* newSeq = seq.AppendNew(3);
    EXPECT_EQ(newSeq->GetLength(), 3);
    EXPECT_EQ(newSeq->GetLast(), 3);
    EXPECT_EQ(seq.GetLength(), 2); // Оригинальная последовательность не изменилась
    delete newSeq;
}

// Вспомогательные функции для тестов
Sequence<int>* doubleSequence(const int& x) {
    auto* seq = new ImmutableArraySequence<int>();
    return seq->AppendNew(x)->AppendNew(x);
}

bool isEven(const int& x) {
    return x % 2 == 0;
}

// Тесты для новых методов ImmutableArraySequence
TEST(ImmutableArraySequenceTest, Constructor) {
    int data[] = {1, 2, 3};
    ImmutableArraySequence<int> seq(data, 3);
    EXPECT_EQ(seq.GetLength(), 3);
    EXPECT_EQ(seq.Get(0), 1);
    EXPECT_EQ(seq.Get(1), 2);
    EXPECT_EQ(seq.Get(2), 3);
}

TEST(ImmutableArraySequenceTest, FlatMap) {
    int data[] = {1, 2};
    ImmutableArraySequence<int> seq(data, 2);
    Sequence<int>* result = seq.FlatMap(doubleSequence);
    EXPECT_EQ(result->GetLength(), 4);
    EXPECT_EQ(result->Get(0), 1);
    EXPECT_EQ(result->Get(1), 1);
    EXPECT_EQ(result->Get(2), 2);
    EXPECT_EQ(result->Get(3), 2);
    delete result;
}

TEST(ImmutableArraySequenceTest, Find) {
    int data[] = {1, 2, 3, 4};
    ImmutableArraySequence<int> seq(data, 4);
    Option<int> result = seq.Find(isEven);
    EXPECT_TRUE(result.isSome());
    EXPECT_EQ(result.getValue(), 2);
    
    int oddData[] = {1, 3, 5};
    ImmutableArraySequence<int> oddSeq(oddData, 3);
    Option<int> noResult = oddSeq.Find(isEven);
    EXPECT_TRUE(noResult.isNone());
}

TEST(ImmutableArraySequenceTest, Split) {
    int data[] = {1, 2, 3, 4};
    ImmutableArraySequence<int> seq(data, 4);
    auto [even, odd] = seq.Split(isEven);
    
    EXPECT_EQ(even->GetLength(), 2);
    EXPECT_EQ(odd->GetLength(), 2);
    EXPECT_EQ(even->Get(0), 2);
    EXPECT_EQ(even->Get(1), 4);
    EXPECT_EQ(odd->Get(0), 1);
    EXPECT_EQ(odd->Get(1), 3);
    
    delete even;
    delete odd;
}

TEST(ImmutableArraySequenceTest, UnzipPairs) {
    std::pair<int, std::string> data[] = {{1, "one"}, {2, "two"}};
    ImmutableArraySequence<std::pair<int, std::string>> seq(data, 2);
    auto [numbers, strings] = Unzip(seq);
    
    EXPECT_EQ(numbers->GetLength(), 2);
    EXPECT_EQ(strings->GetLength(), 2);
    EXPECT_EQ(numbers->Get(0), 1);
    EXPECT_EQ(numbers->Get(1), 2);
    EXPECT_EQ(strings->Get(0), "one");
    EXPECT_EQ(strings->Get(1), "two");
    
    delete numbers;
    delete strings;
}

// Тесты для новых методов ImmutableListSequence
TEST(ImmutableListSequenceTest, Constructor) {
    int data[] = {1, 2, 3};
    ImmutableListSequence<int> seq(data, 3);
    EXPECT_EQ(seq.GetLength(), 3);
    EXPECT_EQ(seq.Get(0), 1);
    EXPECT_EQ(seq.Get(1), 2);
    EXPECT_EQ(seq.Get(2), 3);
}

TEST(ImmutableListSequenceTest, FlatMap) {
    int data[] = {1, 2};
    ImmutableListSequence<int> seq(data, 2);
    Sequence<int>* result = seq.FlatMap(doubleSequence);
    EXPECT_EQ(result->GetLength(), 4);
    EXPECT_EQ(result->Get(0), 1);
    EXPECT_EQ(result->Get(1), 1);
    EXPECT_EQ(result->Get(2), 2);
    EXPECT_EQ(result->Get(3), 2);
    delete result;
}

TEST(ImmutableListSequenceTest, Find) {
    int data[] = {1, 2, 3, 4};
    ImmutableListSequence<int> seq(data, 4);
    Option<int> result = seq.Find(isEven);
    EXPECT_TRUE(result.isSome());
    EXPECT_EQ(result.getValue(), 2);
    
    int oddData[] = {1, 3, 5};
    ImmutableListSequence<int> oddSeq(oddData, 3);
    Option<int> noResult = oddSeq.Find(isEven);
    EXPECT_TRUE(noResult.isNone());
}

TEST(ImmutableListSequenceTest, Split) {
    int data[] = {1, 2, 3, 4};
    ImmutableListSequence<int> seq(data, 4);
    auto [even, odd] = seq.Split(isEven);
    
    EXPECT_EQ(even->GetLength(), 2);
    EXPECT_EQ(odd->GetLength(), 2);
    EXPECT_EQ(even->Get(0), 2);
    EXPECT_EQ(even->Get(1), 4);
    EXPECT_EQ(odd->Get(0), 1);
    EXPECT_EQ(odd->Get(1), 3);
    
    delete even;
    delete odd;
}

TEST(ImmutableListSequenceTest, UnzipPairs) {
    std::pair<int, std::string> data[] = {{1, "one"}, {2, "two"}};
    ImmutableListSequence<std::pair<int, std::string>> seq(data, 2);
    auto [numbers, strings] = Unzip(seq);
    
    EXPECT_EQ(numbers->GetLength(), 2);
    EXPECT_EQ(strings->GetLength(), 2);
    EXPECT_EQ(numbers->Get(0), 1);
    EXPECT_EQ(numbers->Get(1), 2);
    EXPECT_EQ(strings->Get(0), "one");
    EXPECT_EQ(strings->Get(1), "two");
    
    delete numbers;
    delete strings;
}

// Дополнительные тесты для операций с парами
TEST(SequencePairOperationsTest, ZipEmptySequences) {
    ArraySequence<int> seq1;
    ArraySequence<int> seq2;
    auto* result = Zip(seq1, seq2);
    EXPECT_EQ(result->GetLength(), 0);
    delete result;
}

TEST(SequencePairOperationsTest, ZipDifferentLengths) {
    int data1[] = {1, 2, 3};
    int data2[] = {4, 5};
    ArraySequence<int> seq1(data1, 3);
    ArraySequence<int> seq2(data2, 2);
    
    auto* result = Zip(seq1, seq2);
    EXPECT_EQ(result->GetLength(), 2); // Должен использовать минимальную длину
    EXPECT_EQ(result->Get(0).first, 1);
    EXPECT_EQ(result->Get(0).second, 4);
    EXPECT_EQ(result->Get(1).first, 2);
    EXPECT_EQ(result->Get(1).second, 5);
    delete result;
}

TEST(SequencePairOperationsTest, UnzipEmptySequence) {
    ArraySequence<std::pair<int, std::string>> seq;
    auto [numbers, strings] = Unzip(seq);
    
    EXPECT_EQ(numbers->GetLength(), 0);
    EXPECT_EQ(strings->GetLength(), 0);
    
    delete numbers;
    delete strings;
}

// Тесты для Slice
TEST(ArraySequenceTest, SliceBasic) {
    int data[] = {1, 2, 3, 4, 5};
    ArraySequence<int> seq(data, 5);
    
    // Удаление 2 элементов с индекса 1 без замены
    Sequence<int>* result1 = seq.Slice(1, 2);
    EXPECT_EQ(result1->GetLength(), 3);
    EXPECT_EQ(result1->Get(0), 1);
    EXPECT_EQ(result1->Get(1), 4);
    EXPECT_EQ(result1->Get(2), 5);
    delete result1;
    
    // Удаление с отрицательным индексом
    result1 = seq.Slice(-2, 1);
    EXPECT_EQ(result1->GetLength(), 4);
    EXPECT_EQ(result1->Get(0), 1);
    EXPECT_EQ(result1->Get(1), 2);
    EXPECT_EQ(result1->Get(2), 3);
    EXPECT_EQ(result1->Get(3), 5);
    delete result1;
}

TEST(ArraySequenceTest, SliceWithReplacement) {
    int data[] = {1, 2, 3, 4, 5};
    ArraySequence<int> seq(data, 5);
    
    // Создаем последовательность для замены
    int replaceData[] = {10, 20};
    ArraySequence<int> replacement(replaceData, 2);
    
    // Заменяем 2 элемента начиная с индекса 1
    Sequence<int>* result = seq.Slice(1, 2, &replacement);
    EXPECT_EQ(result->GetLength(), 5);
    EXPECT_EQ(result->Get(0), 1);
    EXPECT_EQ(result->Get(1), 10);
    EXPECT_EQ(result->Get(2), 20);
    EXPECT_EQ(result->Get(3), 4);
    EXPECT_EQ(result->Get(4), 5);
    delete result;
}

TEST(ListSequenceTest, SliceBasic) {
    int data[] = {1, 2, 3, 4, 5};
    ListSequence<int> seq(data, 5);
    
    // Удаление 2 элементов с индекса 1 без замены
    Sequence<int>* result1 = seq.Slice(1, 2);
    EXPECT_EQ(result1->GetLength(), 3);
    EXPECT_EQ(result1->Get(0), 1);
    EXPECT_EQ(result1->Get(1), 4);
    EXPECT_EQ(result1->Get(2), 5);
    delete result1;
    
    // Удаление с отрицательным индексом
    result1 = seq.Slice(-2, 1);
    EXPECT_EQ(result1->GetLength(), 4);
    EXPECT_EQ(result1->Get(0), 1);
    EXPECT_EQ(result1->Get(1), 2);
    EXPECT_EQ(result1->Get(2), 3);
    EXPECT_EQ(result1->Get(3), 5);
    delete result1;
}

TEST(ListSequenceTest, SliceWithReplacement) {
    int data[] = {1, 2, 3, 4, 5};
    ListSequence<int> seq(data, 5);
    
    // Создаем последовательность для замены
    int replaceData[] = {10, 20};
    ListSequence<int> replacement(replaceData, 2);
    
    // Заменяем 2 элемента начиная с индекса 1
    Sequence<int>* result = seq.Slice(1, 2, &replacement);
    EXPECT_EQ(result->GetLength(), 5);
    EXPECT_EQ(result->Get(0), 1);
    EXPECT_EQ(result->Get(1), 10);
    EXPECT_EQ(result->Get(2), 20);
    EXPECT_EQ(result->Get(3), 4);
    EXPECT_EQ(result->Get(4), 5);
    delete result;
}

TEST(ImmutableArraySequenceTest, SliceBasic) {
    int data[] = {1, 2, 3, 4, 5};
    ImmutableArraySequence<int> seq(data, 5);
    
    // Удаление 2 элементов с индекса 1 без замены
    Sequence<int>* result1 = seq.Slice(1, 2);
    EXPECT_EQ(result1->GetLength(), 3);
    EXPECT_EQ(result1->Get(0), 1);
    EXPECT_EQ(result1->Get(1), 4);
    EXPECT_EQ(result1->Get(2), 5);
    delete result1;
    
    // Проверяем, что оригинальная последовательность не изменилась
    EXPECT_EQ(seq.GetLength(), 5);
    EXPECT_EQ(seq.Get(1), 2);
    EXPECT_EQ(seq.Get(2), 3);
}

TEST(ImmutableListSequenceTest, SliceBasic) {
    int data[] = {1, 2, 3, 4, 5};
    ImmutableListSequence<int> seq(data, 5);
    
    // Удаление 2 элементов с индекса 1 без замены
    Sequence<int>* result1 = seq.Slice(1, 2);
    EXPECT_EQ(result1->GetLength(), 3);
    EXPECT_EQ(result1->Get(0), 1);
    EXPECT_EQ(result1->Get(1), 4);
    EXPECT_EQ(result1->Get(2), 5);
    delete result1;
    
    // Проверяем, что оригинальная последовательность не изменилась
    EXPECT_EQ(seq.GetLength(), 5);
    EXPECT_EQ(seq.Get(1), 2);
    EXPECT_EQ(seq.Get(2), 3);
}

// Тесты для граничных случаев и исключений
TEST(ArraySequenceTest, EdgeCases) {
    ArraySequence<int> seq;
    
    // Тест пустой последовательности
    EXPECT_EQ(seq.GetLength(), 0);
    EXPECT_THROW(seq.Get(0), IndexOutOfRangeException);
    EXPECT_THROW(seq.GetFirst(), EmptySequenceException);
    EXPECT_THROW(seq.GetLast(), EmptySequenceException);
    
    // Тест TryGet для пустой последовательности
    EXPECT_TRUE(seq.TryGet(0).isNone());
    EXPECT_TRUE(seq.TryGetFirst().isNone());
    EXPECT_TRUE(seq.TryGetLast().isNone());
    
    // Тест некорректных индексов
    seq.Append(1);
    EXPECT_THROW(seq.Get(-1), IndexOutOfRangeException);
    EXPECT_THROW(seq.Get(1), IndexOutOfRangeException);
    EXPECT_THROW(seq.InsertAt(1, 2), IndexOutOfRangeException);
}

TEST(ListSequenceTest, EdgeCases) {
    ListSequence<int> seq;
    
    // Тест пустой последовательности
    EXPECT_EQ(seq.GetLength(), 0);
    EXPECT_THROW(seq.Get(0), IndexOutOfRangeException);
    EXPECT_THROW(seq.GetFirst(), EmptySequenceException);
    EXPECT_THROW(seq.GetLast(), EmptySequenceException);
    
    // Тест TryGet для пустой последовательности
    EXPECT_TRUE(seq.TryGet(0).isNone());
    EXPECT_TRUE(seq.TryGetFirst().isNone());
    EXPECT_TRUE(seq.TryGetLast().isNone());
    
    // Тест некорректных индексов
    seq.Append(1);
    EXPECT_THROW(seq.Get(-1), IndexOutOfRangeException);
    EXPECT_THROW(seq.Get(1), IndexOutOfRangeException);
    EXPECT_THROW(seq.InsertAt(1, 2), IndexOutOfRangeException);
}

// Тесты для методов Map, Where, Reduce с разными функциями
int multiply2(const int& x) { return x * 2; }
bool isNegative(const int& x) { return x < 0; }
int subtract(const int& a, const int& b) { return a - b; }

TEST(ArraySequenceTest, FunctionalOperations) {
    int data[] = {-2, -1, 0, 1, 2};
    ArraySequence<int> seq(data, 5);
    
    // Тест Map с разными функциями
    auto* mapped = seq.Map(multiply2);
    EXPECT_EQ(mapped->Get(0), -4);
    EXPECT_EQ(mapped->Get(4), 4);
    delete mapped;
    
    // Тест Where с разными предикатами
    auto* filtered = seq.Where(isNegative);
    EXPECT_EQ(filtered->GetLength(), 2);
    EXPECT_EQ(filtered->Get(0), -2);
    EXPECT_EQ(filtered->Get(1), -1);
    delete filtered;
    
    // Тест Reduce с разными функциями
    int result = seq.Reduce(subtract, 0);
    EXPECT_EQ(result, 0 - (-2) - (-1) - 0 - 1 - 2);
}

// Тесты для новых методов
TEST(ArraySequenceTest, SliceEdgeCases) {
    int data[] = {1, 2, 3, 4, 5};
    ArraySequence<int> seq(data, 5);
    
    // Тест с отрицательным индексом
    Sequence<int>* result = seq.Slice(-1, 1);
    EXPECT_EQ(result->GetLength(), 4);
    EXPECT_EQ(result->Get(3), 4);
    delete result;
    
    // Тест с N больше оставшейся длины
    result = seq.Slice(3, 10);
    EXPECT_EQ(result->GetLength(), 3);
    EXPECT_EQ(result->Get(0), 1);
    EXPECT_EQ(result->Get(1), 2);
    EXPECT_EQ(result->Get(2), 3);
    delete result;
    
    // Тест с некорректным индексом
    EXPECT_THROW(seq.Slice(10, 1), IndexOutOfRangeException);
    EXPECT_THROW(seq.Slice(-10, 1), IndexOutOfRangeException);
}

TEST(ArraySequenceTest, FlatMapEdgeCases) {
    int data[] = {1};
    ArraySequence<int> seq(data, 1);
    
    // Тест с пустой последовательностью
    Sequence<int>* result = seq.FlatMap([](const int& x) -> Sequence<int>* {
        return new ArraySequence<int>();
    });
    EXPECT_EQ(result->GetLength(), 0);
    delete result;
    
    // Тест с функцией, возвращающей несколько элементов
    result = seq.FlatMap([](const int& x) -> Sequence<int>* {
        auto* seq = new ArraySequence<int>();
        for (int i = 0; i < x; ++i) {
            seq->Append(x);
        }
        return seq;
    });
    EXPECT_EQ(result->GetLength(), 1);
    EXPECT_EQ(result->Get(0), 1);
    delete result;
}

TEST(ArraySequenceTest, FindAndSplitEdgeCases) {
    ArraySequence<int> seq;
    
    // Тест Find на пустой последовательности
    EXPECT_TRUE(seq.Find(isEven).isNone());
    
    // Тест Split на пустой последовательности
    auto [empty1, empty2] = seq.Split(isEven);
    EXPECT_EQ(empty1->GetLength(), 0);
    EXPECT_EQ(empty2->GetLength(), 0);
    delete empty1;
    delete empty2;
    
    // Тест Split когда все элементы удовлетворяют предикату
    int data[] = {2, 4, 6};
    ArraySequence<int> evenSeq(data, 3);
    auto [all, none] = evenSeq.Split(isEven);
    EXPECT_EQ(all->GetLength(), 3);
    EXPECT_EQ(none->GetLength(), 0);
    delete all;
    delete none;
}

// Тесты для иммутабельных последовательностей
TEST(ImmutableArraySequenceTest, ImmutabilityCheck) {
    int data[] = {1, 2, 3};
    ImmutableArraySequence<int> seq(data, 3);
    
    // Проверка, что методы модификации выбрасывают исключения
    EXPECT_THROW(seq.Append(4), InvalidOperationException);
    EXPECT_THROW(seq.Prepend(0), InvalidOperationException);
    EXPECT_THROW(seq.InsertAt(4, 1), InvalidOperationException);
    
    // Проверка, что New-методы создают новые последовательности
    auto* newSeq = seq.AppendNew(4);
    EXPECT_EQ(newSeq->GetLength(), 4);
    EXPECT_EQ(seq.GetLength(), 3); // Оригинальная не изменилась
    EXPECT_EQ(newSeq->Get(3), 4);
    delete newSeq;
    
    newSeq = seq.PrependNew(0);
    EXPECT_EQ(newSeq->GetLength(), 4);
    EXPECT_EQ(seq.GetLength(), 3); // Оригинальная не изменилась
    EXPECT_EQ(newSeq->Get(0), 0);
    delete newSeq;
}

// Тесты для операций с парами
TEST(SequencePairOperationsTest, ComplexOperations) {
    // Тест Zip с последовательностями разных типов
    ArraySequence<int> numbers;
    numbers.Append(1);
    numbers.Append(2);
    
    ArraySequence<std::string> strings;
    strings.Append("one");
    strings.Append("two");
    
    auto* pairs = Zip(numbers, strings);
    EXPECT_EQ(pairs->GetLength(), 2);
    EXPECT_EQ(pairs->Get(0).first, 1);
    EXPECT_EQ(pairs->Get(0).second, "one");
    EXPECT_EQ(pairs->Get(1).first, 2);
    EXPECT_EQ(pairs->Get(1).second, "two");
    
    // Тест Unzip
    auto [nums, strs] = Unzip(*pairs);
    EXPECT_EQ(nums->GetLength(), 2);
    EXPECT_EQ(strs->GetLength(), 2);
    EXPECT_EQ(nums->Get(0), 1);
    EXPECT_EQ(strs->Get(0), "one");
    
    delete pairs;
    delete nums;
    delete strs;
}

// Тесты для конструкторов и копирования
TEST(SequenceTest, ConstructorsAndCopy) {
    // Тест конструктора из массива
    int data[] = {1, 2, 3};
    ArraySequence<int> arraySeq(data, 3);
    ListSequence<int> listSeq(data, 3);
    
    // Тест копирующего конструктора
    ArraySequence<int> arraySeqCopy(arraySeq);
    ListSequence<int> listSeqCopy(listSeq);
    
    EXPECT_EQ(arraySeqCopy.GetLength(), 3);
    EXPECT_EQ(listSeqCopy.GetLength(), 3);
    
    // Проверка независимости копий
    arraySeq.Append(4);
    listSeq.Append(4);
    
    EXPECT_EQ(arraySeqCopy.GetLength(), 3);
    EXPECT_EQ(listSeqCopy.GetLength(), 3);
}

// Тесты для GetSubsequence
TEST(SequenceTest, GetSubsequenceEdgeCases) {
    int data[] = {1, 2, 3, 4, 5};
    ArraySequence<int> seq(data, 5);
    
    // Тест корректных границ
    auto* sub1 = seq.GetSubsequence(1, 3);
    EXPECT_EQ(sub1->GetLength(), 3);
    EXPECT_EQ(sub1->Get(0), 2);
    EXPECT_EQ(sub1->Get(2), 4);
    delete sub1;
    
    // Тест некорректных границ
    EXPECT_THROW(seq.GetSubsequence(-1, 2), IndexOutOfRangeException);
    EXPECT_THROW(seq.GetSubsequence(3, 2), IndexOutOfRangeException);
    EXPECT_THROW(seq.GetSubsequence(0, 5), IndexOutOfRangeException);
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}