#pragma once
#include <type_traits>
#include <ranges>
#include <cstddef>

template<typename T>
struct PNextRangeTraits {
    static inline const void* getNext(const T* node) {
        return node ? node->pNext : nullptr;
    }
};

template<typename PtrType, typename Traits = PNextRangeTraits<std::remove_pointer_t<PtrType>>>
    requires std::is_pointer_v<PtrType>
class PNextRange {
private:
    using T = std::remove_pointer_t<PtrType>;
    static_assert(std::is_const_v<T>, "PNextRange should use const pointer types");
    
public:
    class Iterator {
    public:
        using iterator_category = std::forward_iterator_tag;
        using value_type = PtrType;
        using difference_type = std::ptrdiff_t;
        using pointer = PtrType*;
        using reference = PtrType&;

        explicit Iterator(PtrType ptr) : current(ptr) {}

        PtrType operator*() const { 
            return current; 
        }
        
        PtrType operator->() const { 
            return current; 
        }

        Iterator& operator++() {
            advance();
            return *this;
        }

        Iterator operator++(int) {
            Iterator tmp = *this;
            advance();
            return tmp;
        }

        auto operator<=>(const Iterator&) const = default;

    private:
        void advance() {
            if (current) {
                current = static_cast<PtrType>(Traits::getNext(current));
            }
        }
        
        PtrType current;
    };

    explicit PNextRange(const void* start) 
        : head(static_cast<PtrType>(start)) {}
    
    explicit PNextRange(PtrType start) 
        : head(start) {}

    Iterator begin() const { 
        return Iterator(head); 
    }
    
    Iterator end() const { 
        return Iterator(nullptr); 
    }

    Iterator cbegin() const { 
        return begin(); 
    }
    
    Iterator cend() const { 
        return end(); 
    }

    [[nodiscard]] bool empty() const { 
        return head == nullptr; 
    }
    
    [[nodiscard]] std::size_t size() const {
        return countNodes();
    }

    [[nodiscard]] PtrType front() const { 
        return head; 
    }

private:
    std::size_t countNodes() const {
        std::size_t count = 0;
        for ([[maybe_unused]] auto* _ : *this) {
            ++count;
        }
        return count;
    }
    
    PtrType head;
};