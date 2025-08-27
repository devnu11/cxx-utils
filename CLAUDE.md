# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Build and Test Commands

This is a C++ utility library. The code uses GoogleTest for testing, and the test should be stored in a different folder, and built to a different target.  The code should use CMake, Ninja, CTest and clang.  

### Building and Running Tests

```bash
# To compile just the library
cmake . --preset debug
cmake --build .

# Compile with GoogleTest and enable unit tests
#clang++ -std=c++20 -DENABLE_UNIT_TESTS -pthread pnext_range.cpp -lgtest -lgtest_main -o test_runner

# Run the tests
./test_runner
```

Alternative using clang++:
```bash
```

## Code Architecture

Currently there is only a single utility, we will add more together

### PNextRange Template Class

The codebase contains a single C++ template class `PNextRange` that provides STL-compatible iteration over linked list structures that use a `pNext` pointer pattern (commonly found in Vulkan API structures).

**Key Components:**

This is just for PNext, we will need to add more as we go

1. **PNextRangeTraits** - A traits class that defines how to access the next pointer in a chain
   - Default implementation expects a `pNext` member
   - Can be specialized for custom structures with different member names

2. **PNextRange** - Main template class that wraps pointer chains into C++20 ranges
   - Template parameters: `PtrType` (pointer type), `Traits` (optional traits customization)
   - Provides forward iterator semantics
   - Compatible with STL algorithms and C++20 range views

3. **Iterator** - Forward iterator implementation
   - Supports standard iterator operations (`++`, `*`, `->`, comparisons)
   - Uses C++20 three-way comparison operator (`<=>`)

### Design Patterns

- **Template Specialization**: Custom traits can be provided for structures with non-standard member names
- **SFINAE/Concepts**: Uses `std::is_pointer_v` requirement and static assertions for type safety
- **Range-based Design**: Full compatibility with C++20 ranges and STL algorithms
- **Header-only**: Single file containing both interface and comprehensive unit tests

The code follows modern C++ practices with C++20 features including concepts, ranges, and three-way comparison operators.

Please use SOLID principles.  Functions ideally should be short, and self documenting.  Ideally no more than 7 lines of code