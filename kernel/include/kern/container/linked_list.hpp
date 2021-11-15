#pragma once

#include <kern/mem/allocator.hpp>

namespace os::kern::cont {

    inline void test() {
        u32 x = 0;

        x += 5 % 7;

        asm volatile("" : : "r,m"(x) : "memory");
    }

    template<typename T>
    struct ListNode {
        T value;

        ListNode<T> *next, *prev;
    };

    template<typename T>
    struct ListIterator {
        ListIterator(ListNode<T> *node) : m_node(node) { }

        constexpr auto& operator++() {
            this->m_node = this->m_node->next;

            return *this;
        }

        constexpr auto& operator--() {
            this->m_node = this->m_node->prev;

            return *this;
        }

        constexpr auto& operator*() {
            return this->m_node->value;
        }

        constexpr auto& operator->() {
            return this->m_node->value;
        }

        constexpr bool operator==(const ListIterator<T> &other) const {
            return this->m_node == other.m_node;
        }

        constexpr bool operator!=(const ListIterator<T> &other) const {
            return this->m_node != other.m_node;
        }

    private:
        ListNode<T> *m_node;
    };

    template<typename T>
    struct List {
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

        constexpr ListIterator<T> begin() {
            return ListIterator<T>(front);
        }

        constexpr ListIterator<T> end() {
            return ListIterator<T>(nullptr);
        }


        ListNode<T> *front = nullptr, *back = nullptr;
        mem::Allocator<ListNode<T>, 128> allocator;
    };

}