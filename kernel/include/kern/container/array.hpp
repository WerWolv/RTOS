#pragma once

#include <kern.hpp>

namespace os::kern::cont {

    template<typename T, size_t Size>
    class Array {
    public:
        constexpr T& operator[](size_t index) {
            return this->m_storage[index];
        }

        constexpr const T& operator[](size_t index) const {
            return this->m_storage[index];
        }

        constexpr size_t size() const {
            return Size;
        }

        constexpr T* begin() {
            return &this->m_storage[0];
        }

        constexpr T* end() {
            return &this->m_storage[Size];
        }

    private:
        T m_storage[Size];
    };

}