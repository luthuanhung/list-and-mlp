# 🧠 MLP Neural Network Project  

This project provides data structures and utilities to build and run a **Multi-Layer Perceptron (MLP)** neural network in C++17.  
It includes:  
- 📋 **Custom List Structures**: Array-based list (`XArrayList`) and Linked list (`DLinkedList`).  
- 🔢 **XTensor Integration**: For working with multi-dimensional arrays (tensors).  
- 🤖 **MLP Library**: Basic support for dataset creation/splitting and inference.  

---

## ⚙️ Build Instructions  

Make sure you have **g++ (C++17)** installed.  

### 🔨 Build
```bash
g++ -fsanitize=address -o main -Iinclude -Isrc -std=c++17 \
    main.cpp \
    test/unit_test/arrayList/unit_test.cpp \
    test/unit_test/linkedList/unit_test.cpp \
    test/unit_test/data/unit_test.cpp \
    test/random_test/random_test.cpp \
    src/ann/xtensor_lib.cpp src/util/Point.cpp
```

🚀 Run Instructions

You can run different test modes depending on the component you want to check:

✅ Unit Tests

Array List

```
./main test_unit_array
./main test_unit_array nameFunctionUnitTest
```

Linked List
```
./main test_unit_linkedList
./main test_unit_linkedList nameFunctionUnitTest
```

Dataset
```
./main test_unit_data
./main test_unit_data nameFunctionUnitTest
```

🎲 Randomized Tests
```
./main test_random number_1 number_2
./main test_random number
```

📸 Sample Results

Here you can add screenshots or terminal outputs to show example runs of the project:

# Example: Array List Test
./main test_unit_array


👉 (Insert your screenshot here)

✨ Features

Built with modern C++17

Uses xtensor for tensor operations

Implements both array-based and linked-list data structures

Provides a foundation for building and running a Multi-Layer Perceptron


Bạn có muốn tôi thêm luôn **badge đẹp (build passing, C++17, xtensor)