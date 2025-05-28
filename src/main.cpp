#include <iostream>
#include <string>
#include "Sequence.hpp"
#include "ArraySequence.hpp"
#include "ListSequence.hpp"
#include "ImmutableArraySequence.hpp"
#include "ImmutableListSequence.hpp"
#include "SequencePairOperations.hpp"

Sequence<int>* doubleSequence(const int& x) {
    auto* seq = new ImmutableArraySequence<int>();
    return seq->AppendNew(x)->AppendNew(x);
}

bool isEven(const int& x) {
    return x % 2 == 0;
}

// (взяты из Tests.cpp)
int square(const int& x) { return x * x; }
bool isPositive(const int& x) { return x > 0; }
int add(const int& a, const int& b) { return a + b; }

template<typename T>
void printSequence(const Sequence<T>* seq) {
    if (seq->GetLength() == 0) {
        std::cout << "Последовательность пуста." << std::endl;
        return;
    }
    std::cout << "[ ";
    for (int i = 0; i < seq->GetLength(); ++i) {
        std::cout << seq->Get(i);
        if (i < seq->GetLength() - 1) std::cout << ", ";
    }
    std::cout << " ]" << std::endl;
}

// Основное меню
void showMenu() {
    std::cout << "\n=== Меню операций с последовательностями ===" << std::endl;
    std::cout << "1. Создать изменяемую последовательность" << std::endl;
    std::cout << "2. Создать неизменяемую последовательность" << std::endl;
    std::cout << "3. Выход" << std::endl;
    std::cout << "Введите выбор: ";
}

// Меню для мутабельных последовательностей
void showMutableMenu() {
    std::cout << "\n=== Операции с изменяемой последовательностью ===" << std::endl;
    std::cout << "1. Добавить элемент в конец" << std::endl;
    std::cout << "2. Добавить элемент в начало" << std::endl;
    std::cout << "3. Вставить элемент по индексу" << std::endl;
    std::cout << "4. Получить элемент по индексу" << std::endl;
    std::cout << "5. Получить первый элемент" << std::endl;
    std::cout << "6. Получить последний элемент" << std::endl;
    std::cout << "7. Попробовать получить элемент по индексу" << std::endl;
    std::cout << "8. Попробовать получить первый элемент" << std::endl;
    std::cout << "9. Попробовать получить последний элемент" << std::endl;
    std::cout << "10. Получить подпоследовательность" << std::endl;
    std::cout << "11. Map (возвести в квадрат)" << std::endl;
    std::cout << "12. Where (оставить положительные)" << std::endl;
    std::cout << "13. Reduce (сумма элементов)" << std::endl;
    std::cout << "14. Zip с другой последовательностью" << std::endl;
    std::cout << "15. Unzip последовательности пар" << std::endl;
    std::cout << "16. Slice последовательности" << std::endl;
    std::cout << "17. FlatMap (удвоить каждый элемент)" << std::endl;
    std::cout << "18. Найти первое четное число (Find)" << std::endl;
    std::cout << "19. Разделить на четные/нечетные (Split)" << std::endl;
    std::cout << "20. Создать из массива" << std::endl;
    std::cout << "21. Вывести последовательность" << std::endl;
    std::cout << "22. Concat с другой последовательностью" << std::endl;
    std::cout << "23. Вернуться в главное меню" << std::endl;
    std::cout << "Введите выбор: ";
}

// Меню для иммутабельных последовательностей
void showImmutableMenu() {
    std::cout << "\n=== Операции с неизменяемой последовательностью ===" << std::endl;
    std::cout << "1. Создать новую с добавленным элементом в конец" << std::endl;
    std::cout << "2. Создать новую с добавленным элементом в начало" << std::endl;
    std::cout << "3. Создать новую с вставленным элементом" << std::endl;
    std::cout << "4. Получить элемент по индексу" << std::endl;
    std::cout << "5. Получить первый элемент" << std::endl;
    std::cout << "6. Получить последний элемент" << std::endl;
    std::cout << "7. Попробовать получить элемент по индексу" << std::endl;
    std::cout << "8. Попробовать получить первый элемент" << std::endl;
    std::cout << "9. Попробовать получить последний элемент" << std::endl;
    std::cout << "10. Получить подпоследовательность" << std::endl;
    std::cout << "11. Map (возвести в квадрат)" << std::endl;
    std::cout << "12. Where (оставить положительные)" << std::endl;
    std::cout << "13. Reduce (сумма элементов)" << std::endl;
    std::cout << "14. Zip с другой последовательностью" << std::endl;
    std::cout << "15. Slice последовательности" << std::endl;
    std::cout << "16. FlatMap (удвоить каждый элемент)" << std::endl;
    std::cout << "17. Найти первое четное число (Find)" << std::endl;
    std::cout << "18. Разделить на четные/нечетные (Split)" << std::endl;
    std::cout << "19. Unzip последовательности пар" << std::endl;
    std::cout << "20. Создать из массива" << std::endl;
    std::cout << "21. Вывести последовательность" << std::endl;
    std::cout << "22. Concat с другой последовательностью" << std::endl;
    std::cout << "23. Вернуться в главное меню" << std::endl;
    std::cout << "Введите выбор: ";
}

template<typename T>
Sequence<T>* createMutableSequence(bool isArray) {
    int count;
    std::cout << "Введите количество начальных элементов (0 для пустой последовательности): ";
    std::cin >> count;
    
    if (count < 0) {
        std::cout << "Некорректное количество. Создаём пустую последовательность." << std::endl;
        if (isArray) {
            return new ArraySequence<T>();
        } else {
            return new ListSequence<T>();
        }
    }
    
    if (count == 0) {
        if (isArray) {
            return new ArraySequence<T>();
        } else {
            return new ListSequence<T>();
        }
    }
    
    T* items = new T[count];
    std::cout << "Введите " << count << " элементов: ";
    for (int i = 0; i < count; ++i) {
        std::cin >> items[i];
    }
    
    Sequence<T>* seq;
    if (isArray) {
        seq = new ArraySequence<T>(items, count);
    } else {
        seq = new ListSequence<T>(items, count);
    }
    
    delete[] items;
    return seq;
}

template<typename T>
Sequence<T>* createImmutableSequence(bool isArray) {
    int count;
    std::cout << "Введите количество начальных элементов (0 для пустой последовательности): ";
    std::cin >> count;
    
    if (count < 0) {
        std::cout << "Некорректное количество. Создаём пустую последовательность." << std::endl;
        if (isArray) {
            return new ImmutableArraySequence<T>();
        } else {
            return new ImmutableListSequence<T>();
        }
    }
    
    if (count == 0) {
        if (isArray) {
            return new ImmutableArraySequence<T>();
        } else {
            return new ImmutableListSequence<T>();
        }
    }
    
    T* items = new T[count];
    std::cout << "Введите " << count << " элементов: ";
    for (int i = 0; i < count; ++i) {
        std::cin >> items[i];
    }
    
    Sequence<T>* seq;
    if (isArray) {
        seq = new ImmutableArraySequence<T>(items, count);
    } else {
        seq = new ImmutableListSequence<T>(items, count);
    }
    
    delete[] items;
    return seq;
}

void handleMutableSequence(Sequence<int>* seq) {
    while (true) {
        showMutableMenu();
        int choice;
        std::cin >> choice;

        try {
            if (choice == 1) { // Append
                int value;
                std::cout << "Введите значение для добавления: ";
                std::cin >> value;
                seq->Append(value);
                std::cout << "Элемент добавлен." << std::endl;
            }
            else if (choice == 2) { // Prepend
                int value;
                std::cout << "Введите значение для добавления: ";
                std::cin >> value;
                seq->Prepend(value);
                std::cout << "Элемент добавлен." << std::endl;
            }
            else if (choice == 3) { // InsertAt
                int value, index;
                std::cout << "Введите значение для вставки: ";
                std::cin >> value;
                std::cout << "Введите индекс: ";
                std::cin >> index;
                seq->InsertAt(value, index);
                std::cout << "Элемент вставлен." << std::endl;
            }
            else if (choice == 4) { // Get
                int index;
                std::cout << "Введите индекс: ";
                std::cin >> index;
                int value = seq->Get(index);
                std::cout << "Элемент по индексу " << index << ": " << value << std::endl;
            }
            else if (choice == 5) { // GetFirst
                int value = seq->GetFirst();
                std::cout << "Первый элемент: " << value << std::endl;
            }
            else if (choice == 6) { // GetLast
                int value = seq->GetLast();
                std::cout << "Последний элемент: " << value << std::endl;
            }
            else if (choice == 7) { // TryGet
                int index;
                std::cout << "Введите индекс: ";
                std::cin >> index;
                Option<int> result = seq->TryGet(index);
                if (result.isSome()) {
                    std::cout << "Элемент по индексу " << index << ": " << result.getValue() << std::endl;
                } else {
                    std::cout << "Нет элемента по индексу " << index << std::endl;
                }
            }
            else if (choice == 8) { // TryGetFirst
                Option<int> result = seq->TryGetFirst();
                if (result.isSome()) {
                    std::cout << "Первый элемент: " << result.getValue() << std::endl;
                } else {
                    std::cout << "Последовательность пуста" << std::endl;
                }
            }
            else if (choice == 9) { // TryGetLast
                Option<int> result = seq->TryGetLast();
                if (result.isSome()) {
                    std::cout << "Последний элемент: " << result.getValue() << std::endl;
                } else {
                    std::cout << "Последовательность пуста" << std::endl;
                }
            }
            else if (choice == 10) { // GetSubsequence
                int start, end;
                std::cout << "Введите начальный индекс: ";
                std::cin >> start;
                std::cout << "Введите конечный индекс: ";
                std::cin >> end;
                Sequence<int>* subseq = seq->GetSubsequence(start, end);
                std::cout << "Подпоследовательность: ";
                printSequence(subseq);
                delete subseq;
            }
            else if (choice == 11) { // Map
                Sequence<int>* result = seq->Map(square);
                std::cout << "Результат Map (квадраты): ";
                printSequence(result);
                delete result;
            }
            else if (choice == 12) { // Where
                Sequence<int>* result = seq->Where(isPositive);
                std::cout << "Результат Where (положительные): ";
                printSequence(result);
                delete result;
            }
            else if (choice == 13) { // Reduce
                int result = seq->Reduce(add, 0);
                std::cout << "Сумма элементов: " << result << std::endl;
            }
            else if (choice == 14) { // Zip
                std::cout << "Создайте вторую последовательность для операции Zip." << std::endl;
                Sequence<int>* other = createMutableSequence<int>(true);
                Sequence<std::pair<int, int>>* result = Zip(*seq, *other);
                std::cout << "Результат Zip: [ ";
                for (int i = 0; i < result->GetLength(); ++i) {
                    auto pair = result->Get(i);
                    std::cout << "(" << pair.first << ", " << pair.second << ")";
                    if (i < result->GetLength() - 1) std::cout << ", ";
                }
                std::cout << " ]" << std::endl;
                delete other;
                delete result;
            }
            else if (choice == 15) { // Unzip
                if (seq->GetLength() == 0) {
                    std::cout << "Sequence is empty." << std::endl;
                    continue;
                }
                
                std::cout << "\nДемонстрационный пример операции Unzip:" << std::endl;
                std::cout << "Будет создана тестовая последовательность пар (число, строка):" << std::endl;
                std::cout << "[(1, \"one\"), (2, \"two\"), (3, \"three\")]" << std::endl;
                std::cout << "и продемонстрирована операция Unzip над ней.\n" << std::endl;
                
                // Создаем тестовую последовательность пар
                std::pair<int, std::string> pairs[] = {{1, "one"}, {2, "two"}, {3, "three"}};
                auto* pairSeq = new ImmutableArraySequence<std::pair<int, std::string>>(pairs, 3);
                
                auto [first, second] = Unzip(*pairSeq);
                std::cout << "First sequence: ";
                printSequence(first);
                std::cout << "Second sequence: ";
                for (int i = 0; i < second->GetLength(); ++i) {
                    std::cout << second->Get(i);
                    if (i < second->GetLength() - 1) std::cout << ", ";
                }
                std::cout << std::endl;
                
                delete first;
                delete second;
                delete pairSeq;
            }
            else if (choice == 16) { // Slice
                int index, count;
                std::cout << "Введите индекс (может быть отрицательным для отсчета с конца): ";
                std::cin >> index;
                std::cout << "Введите количество элементов для удаления: ";
                std::cin >> count;
                
                char hasReplacement;
                std::cout << "Хотите вставить элементы вместо удаленных? (y/n): ";
                std::cin >> hasReplacement;
                
                Sequence<int>* replacement = nullptr;
                if (hasReplacement == 'y' || hasReplacement == 'Y') {
                    std::cout << "Создайте последовательность для вставки:" << std::endl;
                    replacement = createMutableSequence<int>(true);
                }
                
                Sequence<int>* result = seq->Slice(index, count, replacement);
                std::cout << "Результат Slice: ";
                printSequence(result);
                
                delete replacement;
                delete result;
            }
            else if (choice == 17) { // FlatMap
                Sequence<int>* result = seq->FlatMap(doubleSequence);
                std::cout << "Результат FlatMap (каждый элемент удвоен): ";
                printSequence(result);
                delete result;
            }
            else if (choice == 18) { // Find
                Option<int> result = seq->Find(isEven);
                if (result.isSome()) {
                    std::cout << "Найдено первое четное число: " << result.getValue() << std::endl;
                } else {
                    std::cout << "Четные числа не найдены." << std::endl;
                }
            }
            else if (choice == 19) { // Split
                auto [even, odd] = seq->Split(isEven);
                std::cout << "Четные числа: ";
                printSequence(even);
                std::cout << "Нечетные числа: ";
                printSequence(odd);
                delete even;
                delete odd;
            }
            else if (choice == 20) { // Create from array
                int count;
                std::cout << "Введите количество элементов: ";
                std::cin >> count;
                if (count <= 0) {
                    std::cout << "Некорректное количество элементов" << std::endl;
                    continue;
                }
                int* items = new int[count];
                std::cout << "Введите " << count << " элементов: ";
                for (int i = 0; i < count; ++i) {
                    std::cin >> items[i];
                }
    
                auto* immutableArraySeq = dynamic_cast<ImmutableArraySequence<int>*>(seq);
                delete seq;

                if (immutableArraySeq) {
                    seq = new ImmutableArraySequence<int>(items, count);
                } else {
                    seq = new ImmutableListSequence<int>(items, count);
                }
                
                std::cout << "Создана новая последовательность: ";
                printSequence(seq);
                
                delete[] items;
            }
            else if (choice == 21) { // Print
                std::cout << "Current sequence: ";
                printSequence(seq);
            }
            else if (choice == 22) { // Concat
                std::cout << "Создайте вторую последовательность для операции Concat." << std::endl;
                Sequence<int>* other = createMutableSequence<int>(true);
                Sequence<int>* result = seq->Concat(other);
                std::cout << "Результат Concat: ";
                printSequence(result);
                delete other;
                delete result;
            }
            else if (choice == 23) { // Back to main menu
                break;
            }
            else {
                std::cout << "Invalid choice. Try again." << std::endl;
            }
        }
        catch (const SequenceException& e) {
            std::cout << "Ошибка: " << e.what() << std::endl;
        }
    }
}

void handleImmutableSequence(Sequence<int>* seq) {
    while (true) {
        showImmutableMenu();
        int choice;
        std::cin >> choice;

        try {
            if (choice == 1) { // AppendNew
                int value;
                std::cout << "Введите значение для добавления: ";
                std::cin >> value;
                auto* immutableSeq = dynamic_cast<ImmutableArraySequence<int>*>(seq);
                if (immutableSeq) {
                    Sequence<int>* newSeq = immutableSeq->AppendNew(value);
                    std::cout << "Новая последовательность с добавленным элементом: ";
                    printSequence(newSeq);
                    delete newSeq;
                } else {
                    auto* immutableListSeq = dynamic_cast<ImmutableListSequence<int>*>(seq);
                    if (immutableListSeq) {
                        Sequence<int>* newSeq = immutableListSeq->AppendNew(value);
                        std::cout << "Новая последовательность с добавленным элементом: ";
                        printSequence(newSeq);
                        delete newSeq;
                    }
                }
            }
            else if (choice == 2) { // PrependNew
                int value;
                std::cout << "Введите значение для добавления: ";
                std::cin >> value;
                auto* immutableSeq = dynamic_cast<ImmutableArraySequence<int>*>(seq);
                if (immutableSeq) {
                    Sequence<int>* newSeq = immutableSeq->PrependNew(value);
                    std::cout << "Новая последовательность с добавленным элементом: ";
                    printSequence(newSeq);
                    delete newSeq;
                } else {
                    auto* immutableListSeq = dynamic_cast<ImmutableListSequence<int>*>(seq);
                    if (immutableListSeq) {
                        Sequence<int>* newSeq = immutableListSeq->PrependNew(value);
                        std::cout << "Новая последовательность с добавленным элементом: ";
                        printSequence(newSeq);
                        delete newSeq;
                    }
                }
            }
            else if (choice == 3) { // InsertAtNew
                int value, index;
                std::cout << "Введите значение для вставки: ";
                std::cin >> value;
                std::cout << "Введите индекс: ";
                std::cin >> index;
                auto* immutableSeq = dynamic_cast<ImmutableArraySequence<int>*>(seq);
                if (immutableSeq) {
                    Sequence<int>* newSeq = immutableSeq->InsertAtNew(value, index);
                    std::cout << "Новая последовательность с вставленным элементом: ";
                    printSequence(newSeq);
                    delete newSeq;
                } else {
                    auto* immutableListSeq = dynamic_cast<ImmutableListSequence<int>*>(seq);
                    if (immutableListSeq) {
                        Sequence<int>* newSeq = immutableListSeq->InsertAtNew(value, index);
                        std::cout << "Новая последовательность с вставленным элементом: ";
                        printSequence(newSeq);
                        delete newSeq;
                    }
                }
            }
            else if (choice == 4) { // Get
                int index;
                std::cout << "Введите индекс: ";
                std::cin >> index;
                int value = seq->Get(index);
                std::cout << "Элемент по индексу " << index << ": " << value << std::endl;
            }
            else if (choice == 5) { // GetFirst
                int value = seq->GetFirst();
                std::cout << "Первый элемент: " << value << std::endl;
            }
            else if (choice == 6) { // GetLast
                int value = seq->GetLast();
                std::cout << "Последний элемент: " << value << std::endl;
            }
            else if (choice == 7) { // TryGet
                int index;
                std::cout << "Введите индекс: ";
                std::cin >> index;
                Option<int> result = seq->TryGet(index);
                if (result.isSome()) {
                    std::cout << "Элемент по индексу " << index << ": " << result.getValue() << std::endl;
                } else {
                    std::cout << "Нет элемента по индексу " << index << std::endl;
                }
            }
            else if (choice == 8) { // TryGetFirst
                Option<int> result = seq->TryGetFirst();
                if (result.isSome()) {
                    std::cout << "Первый элемент: " << result.getValue() << std::endl;
                } else {
                    std::cout << "Последовательность пуста" << std::endl;
                }
            }
            else if (choice == 9) { // TryGetLast
                Option<int> result = seq->TryGetLast();
                if (result.isSome()) {
                    std::cout << "Последний элемент: " << result.getValue() << std::endl;
                } else {
                    std::cout << "Последовательность пуста" << std::endl;
                }
            }
            else if (choice == 10) { // GetSubsequence
                int start, end;
                std::cout << "Введите начальный индекс: ";
                std::cin >> start;
                std::cout << "Введите конечный индекс: ";
                std::cin >> end;
                Sequence<int>* subseq = seq->GetSubsequence(start, end);
                std::cout << "Подпоследовательность: ";
                printSequence(subseq);
                delete subseq;
            }
            else if (choice == 11) { // Map
                Sequence<int>* result = seq->Map(square);
                std::cout << "Результат Map (квадраты): ";
                printSequence(result);
                delete result;
            }
            else if (choice == 12) { // Where
                Sequence<int>* result = seq->Where(isPositive);
                std::cout << "Результат Where (положительные): ";
                printSequence(result);
                delete result;
            }
            else if (choice == 13) { // Reduce
                int result = seq->Reduce(add, 0);
                std::cout << "Сумма элементов: " << result << std::endl;
            }
            else if (choice == 14) { // Zip
                std::cout << "Создайте вторую последовательность для операции Zip." << std::endl;
                Sequence<int>* other = createImmutableSequence<int>(true);
                Sequence<std::pair<int, int>>* result = Zip(*seq, *other);
                std::cout << "Результат Zip: [ ";
                for (int i = 0; i < result->GetLength(); ++i) {
                    auto pair = result->Get(i);
                    std::cout << "(" << pair.first << ", " << pair.second << ")";
                    if (i < result->GetLength() - 1) std::cout << ", ";
                }
                std::cout << " ]" << std::endl;
                delete other;
                delete result;
            }
            else if (choice == 15) { // Slice
                int index, count;
                std::cout << "Введите индекс (может быть отрицательным для отсчета с конца): ";
                std::cin >> index;
                std::cout << "Введите количество элементов для удаления: ";
                std::cin >> count;
                
                char hasReplacement;
                std::cout << "Хотите вставить элементы вместо удаленных? (y/n): ";
                std::cin >> hasReplacement;
                
                Sequence<int>* replacement = nullptr;
                if (hasReplacement == 'y' || hasReplacement == 'Y') {
                    std::cout << "Создайте последовательность для вставки:" << std::endl;
                    replacement = createImmutableSequence<int>(true);
                }
                
                Sequence<int>* result = seq->Slice(index, count, replacement);
                std::cout << "Результат Slice: ";
                printSequence(result);
                
                delete replacement;
                delete result;
            }
            else if (choice == 16) { // FlatMap
                Sequence<int>* result = seq->FlatMap(doubleSequence);
                std::cout << "FlatMap result (each element doubled): ";
                printSequence(result);
                delete result;
            }
            else if (choice == 17) { // Find
                Option<int> result = seq->Find(isEven);
                if (result.isSome()) {
                    std::cout << "First even number found: " << result.getValue() << std::endl;
                } else {
                    std::cout << "No even numbers found." << std::endl;
                }
            }
            else if (choice == 18) { // Split
                auto [even, odd] = seq->Split(isEven);
                std::cout << "Even numbers: ";
                printSequence(even);
                std::cout << "Odd numbers: ";
                printSequence(odd);
                delete even;
                delete odd;
            }
            else if (choice == 19) { // Unzip
                if (seq->GetLength() == 0) {
                    std::cout << "Последовательность пуста." << std::endl;
                    continue;
                }
                
                std::cout << "\nДемонстрационный пример операции Unzip:" << std::endl;
                std::cout << "Будет создана тестовая последовательность пар (число, строка):" << std::endl;
                std::cout << "[(1, \"one\"), (2, \"two\"), (3, \"three\")]" << std::endl;
                std::cout << "и продемонстрирована операция Unzip над ней.\n" << std::endl;
                
                // Создаем тестовую последовательность пар
                std::pair<int, std::string> pairs[] = {{1, "one"}, {2, "two"}, {3, "three"}};
                auto* pairSeq = new ImmutableArraySequence<std::pair<int, std::string>>(pairs, 3);
                
                auto [first, second] = Unzip(*pairSeq);
                std::cout << "Первая последовательность: ";
                printSequence(first);
                std::cout << "Вторая последовательность: ";
                for (int i = 0; i < second->GetLength(); ++i) {
                    std::cout << second->Get(i);
                    if (i < second->GetLength() - 1) std::cout << ", ";
                }
                std::cout << std::endl;
                
                delete first;
                delete second;
                delete pairSeq;
            }
            else if (choice == 20) { // Create from array
                int count;
                std::cout << "Введите количество элементов: ";
                std::cin >> count;
                if (count <= 0) {
                    std::cout << "Некорректное количество элементов" << std::endl;
                    continue;
                }
                int* items = new int[count];
                std::cout << "Введите " << count << " элементов: ";
                for (int i = 0; i < count; ++i) {
                    std::cin >> items[i];
                }
                
                auto* immutableArraySeq = dynamic_cast<ImmutableArraySequence<int>*>(seq);
                
                delete seq;
                
                if (immutableArraySeq) {
                    seq = new ImmutableArraySequence<int>(items, count);
                } else {
                    seq = new ImmutableListSequence<int>(items, count);
                }
                
                std::cout << "Создана новая последовательность: ";
                printSequence(seq);
                
                delete[] items;
            }
            else if (choice == 21) { // Print
                std::cout << "Текущая последовательность: ";
                printSequence(seq);
            }
            else if (choice == 22) { // Concat
                std::cout << "Создайте вторую последовательность для операции Concat." << std::endl;
                Sequence<int>* other = createImmutableSequence<int>(true);
                Sequence<int>* result = seq->Concat(other);
                std::cout << "Результат Concat: ";
                printSequence(result);
                delete other;
                delete result;
            }
            else if (choice == 23) { // Back to main menu
                break;
            }
            else {
                std::cout << "Некорректный выбор. Попробуйте снова." << std::endl;
            }
        }
        catch (const SequenceException& e) {
            std::cout << "Ошибка: " << e.what() << std::endl;
        }
    }
}

int main() {
    while (true) {
        showMenu();
        int choice;
        std::cin >> choice;

        if (choice == 1) { // Create mutable sequence
            std::cout << "Выберите тип последовательности:" << std::endl;
            std::cout << "1. ArraySequence" << std::endl;
            std::cout << "2. ListSequence" << std::endl;
            std::cout << "Введите выбор: ";
            int typeChoice;
            std::cin >> typeChoice;
            
            bool isArray = (typeChoice == 1);
            if (typeChoice != 1 && typeChoice != 2) {
                std::cout << "Некорректный выбор. Используем ArraySequence по умолчанию." << std::endl;
                isArray = true;
            }

            Sequence<int>* seq = createMutableSequence<int>(isArray);
            std::cout << "Последовательность создана. Тип: " << (isArray ? "ArraySequence" : "ListSequence") << std::endl;
            handleMutableSequence(seq);
            delete seq;
        }
        else if (choice == 2) { // Create immutable sequence
            std::cout << "Выберите тип последовательности:" << std::endl;
            std::cout << "1. ImmutableArraySequence" << std::endl;
            std::cout << "2. ImmutableListSequence" << std::endl;
            std::cout << "Введите выбор: ";
            int typeChoice;
            std::cin >> typeChoice;
            
            bool isArray = (typeChoice == 1);
            if (typeChoice != 1 && typeChoice != 2) {
                std::cout << "Некорректный выбор. Используем ImmutableArraySequence по умолчанию." << std::endl;
                isArray = true;
            }

            Sequence<int>* seq = createImmutableSequence<int>(isArray);
            std::cout << "Последовательность создана. Тип: " << (isArray ? "ImmutableArraySequence" : "ImmutableListSequence") << std::endl;
            handleImmutableSequence(seq);
            delete seq;
        }
        else if (choice == 3) { // Exit
            break;
        }
        else {
            std::cout << "Некорректный выбор. Попробуйте снова." << std::endl;
        }
    }

    std::cout << "Программа завершена." << std::endl;
    return 0;
}