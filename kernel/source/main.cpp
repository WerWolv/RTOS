#include <kern.hpp>

#include <kern/container/linked_list.hpp>

#include <hw/stm32f7/uart.hpp>

extern "C" void default_handler() {
    while (true);
}

extern "C" void kern_main() {
    os::kern::cont::List<u32> list;

    list.push_back(1);
    list.push_back(2);
    list.push_back(3);
    list.push_back(4);

    u32 x = 0;

    hw::uart::init();

    for (u32 value : list) {
        hw::uart::write('0' + value);
        x += value;
    }

    asm volatile("" : : "r,m"(x) : "memory");

    while (true);
}