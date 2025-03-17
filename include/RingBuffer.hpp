#pragma once
#include <array>
#include <atomic>
#include <cstddef>

namespace market {

template<typename T, size_t Size>
class RingBuffer {
    static_assert((Size & (Size - 1)) == 0, "Size must be a power of 2");

public:
    bool push(const T& item) {
        const auto current_write = write_index.load(std::memory_order_relaxed);
        const auto next_write = (current_write + 1) & (Size - 1);
        
        if (next_write == read_index.load(std::memory_order_acquire)) {
            return false; // Buffer is full
        }
        
        buffer[current_write] = item;
        write_index.store(next_write, std::memory_order_release);
        return true;
    }

    bool pop(T& item) {
        const auto current_read = read_index.load(std::memory_order_relaxed);
        
        if (current_read == write_index.load(std::memory_order_acquire)) {
            return false; // Buffer is empty
        }
        
        item = buffer[current_read];
        read_index.store((current_read + 1) & (Size - 1), std::memory_order_release);
        return true;
    }

private:
    std::array<T, Size> buffer;
    std::atomic<size_t> write_index{0};
    std::atomic<size_t> read_index{0};
};

} // namespace market