#include <kern.hpp>

#include <kern/container/array.hpp>
#include <kern/helpers/mem_util.hpp>

namespace os::kern::mem {

    template<typename T, size_t Size>
    struct Allocator {
    public:

        constexpr Allocator() {
            for (u32 i = 0; i < Size; i++)
                this->m_freeObjects[i] = &this->m_storage[i].value;
        }

        constexpr T* allocate() {
            for (auto &obj : this->m_freeObjects) {
                if (obj != nullptr) {
                    auto result = obj;

                    obj = nullptr;
                    return result;
                }
            }

            return nullptr;
        }

        constexpr void free(T *&freeingObject) {
            if (freeingObject >= &this->m_storage.front().value && freeingObject <= &this->m_storage.back().value) {
                for (auto &obj : this->m_freeObjects) {
                    if (obj == nullptr) {
                        obj = freeingObject;
                        freeingObject = nullptr;
                        return;
                    }
                }
            }
        }

    private:
        cont::Array<hlp::Aligned<T>, Size> m_storage;
        cont::Array<T*, Size> m_freeObjects;
    };

}