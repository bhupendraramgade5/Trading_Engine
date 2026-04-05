## Content
1. [Overview](#overview)
2. [Architecture](#architecture)
3. [Features](#features)
4. [Project Structure](#project-structure)
5. [Requirements](#requirements)
6. [Installation](#installation)
7. [Build Instructions](#build-instructions)
8. [Running the Application](#running-the-application)
9. [Commands](#commands)
10. [Testing](#testing)
11. [Future Improvements](#future-improvements)
12. [Design Notes](#design-notes)

## [Overview](#content)

Multi-Symbol matching engine implemented in C++17.

This project simulates a simplified exchange matching engine supporting:

- Limit order matching
- Multi-symbol orderbooks
- Price-time priority
- Position & PnL tracking
- Command-driven interface

## [Architecture](#content)
    CommandParser
        ↓
    MatchingEngine
        ↓
    OrderBook (per symbol)
        ↓
    Trade Callback
        ↓
    PositionEngine
## [Features](#content)

```GO
    ✔ Multi-symbol matching
    ✔ Price-time priority
    ✔ Modify / Cancel support
    ✔ Position tracking
    ✔ Realized / Unrealized PnL
    ✔ Command driven interface
    ✔ Unit testing (GoogleTest)
    ✔ Integration testing (CTest)
```

## [Project Structure](#content)
    src
    ├── CMakeLists.txt
    ├── Matching_Engine
    │   ├── Command.cpp
    │   ├── Command_Parser.cpp
    │   ├── Design-Discussion.md
    │   ├── MatchingEngine.cpp
    │   └── OrderBook.cpp
    ├── Position_Engine
    │   └── PostionEngine.cpp
    └── main.cpp

    tests
    ├── CMakeLists.txt
    ├── README.md
    ├── basic_test.cpp
    ├── functional
    │   └── input1.txt
    └── unit
        └── orderbook_tests.cpp
## [Requirements](#content)
`The code was written and tested on a Linux environment. It should be portable to other platforms with minimal changes.`


* **C++17**
* **CMake 3.16+**
* **GCC / Clang**
* **GoogleTest**


## [Installation](#content)


```Shell
git clone https://github.com/bhupendraramgade5/Trading_Engine.git

cd matching-engine
```


## [Build Instructions](#content)
```shell
    mkdir build
    cd build
    cmake ..
    make -j
```


## [Running the Application](#content)




## [Commands](#content)
### **New Order**
```
NEW <symbol> <orderId> <userId> <BUY/SELL> <price> <qty>

Example:
    NEW 1 100 1 BUY 100 10
```
### **Cancel Order**
```
CANCEL <symbol> <orderId>

Example:
    CANCEL 1 100
```
### **Modify Order**
```
MODIFY <symbol> <orderId> <price> <qty>
Example:
    MODIFY 1 100 101 20
```
### **Print Orderbook**
```
PRINT <symbol>
```
### **Print All Books**
```
PRINTALL
```
### **Price Update**
```
PRICE <symbol> <price>
```

### **Position**
```
POSITION <userId> <symbol>
Example:
    POSITION 1 1
```
## [Testing](#content)
```shell
    cd build
    ctest
# or 
    make test
```

## [Future Improvements](#content)
## [Design Notes](#content)
Design Decisions

- Match before insert (aggressive order model)
- Price-time priority
- Separate orderbooks per symbol
- Trade callback architecture