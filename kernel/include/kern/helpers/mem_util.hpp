#pragma once

#include <kern.hpp>

#include <kern/container/array.hpp>

namespace os::kern::hlp {

    constexpr u32 getAlignmentPadding(size_t size, size_t alignment) {
        const u32 remainder = size % alignment;

        return remainder != 0 ? alignment - remainder : 0; 
    }

    template<typename T>
    struct Aligned {
        T value;
        cont::Array<u8, getAlignmentPadding(sizeof(T), 8)> padding;
    };

}

