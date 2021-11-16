#pragma once

#include <kern/mem/allocator.hpp>

namespace os::kern::cont {

    template<typename T>
    struct ListNode {
        T value;

        ListNode<T> *next, *prev;
    };

    template<typename T>
    struct ListIterator {
        using value_type = T;
        using difference_type = std::ptrdiff_t;

        constexpr auto& operator++() {
            this->m_node = this->m_node->next;

            return *this;
        }

        constexpr auto operator++(int) {
            const auto copy = *this;
            
            ++(*this);

            return copy;
        }

        constexpr auto& operator--() {
            this->m_node = this->m_node->prev;

            return *this;
        }

        constexpr auto operator--(int) {
            const auto copy = *this;

            --(*this);

            return copy;
        }

        constexpr auto& operator*() const {
            return this->m_node->value;
        }

        constexpr auto operator->() const {
            return std::addressof(this->m_node->value);
        }

        constexpr auto operator <=>(const ListIterator &other) const = default;

    private:
        ListNode<T> *m_node = nullptr;
    };

    template<typename T>
    struct List {
        using iterator = ListIterator<T>;

        constexpr T& push_back(const T &value) {
            auto node = allocator.allocate();

            node->value = value;

            if (back == nullptr) {
                node->next = nullptr;
                node->prev = nullptr;

                back = node;
            }
            else {
                back->next = node;
                node->prev = back;
                node->next = nullptr;
                back = node;
            }

            if (front == nullptr)
                front = node;

            return node->value;
        }

        constexpr void pop_back() {
            if (back != nullptr) {
                if (back->prev != nullptr) {
                    back->prev->next = nullptr;
                }

                auto old = back;
                back = back->prev;

                allocator.free(old);
            }
        }

        constexpr iterator begin() {
            return iterator{ front };
        }

        constexpr iterator end() {
            return iterator{ nullptr };
        }

        ListNode<T> *front = nullptr, *back = nullptr;
        Allocator<ListNode<T>, 128> allocator;
    };

}
