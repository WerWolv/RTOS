#include <kern.hpp>

#include <kern/container/linked_list.hpp>
#include <kern/scheduler/scheduler.hpp>

#include <hw/stm32f7/uart.hpp>
#include <hw/stm32f7/stm32f769xx.h>


extern "C" void default_handler() {
    while (true);
}

extern "C" void nmi_handler() {
    while (true);
}

extern "C" void hard_fault_handler() {
    while (true);
}

extern "C" void mem_manage_handler() {
    while (true);
}

extern "C" void bus_fault_handler() {
    while (true);
}

extern "C" void usage_fault_handler() {
    while (true);
}

extern "C" void svc_handler() {
    while (true);
}

alignas(8) static u8 stack1[0x200 * 8];
alignas(8) static u8 stack2[0x200 * 8];

extern "C" void kern_main() {
	hw::uart::init();

	os::kern::sched::add_task([] {
			hw::uart::write("Executing Task 1\n\r");

			os::kern::sched::yield();
	}, stack1, sizeof(stack1));

	os::kern::sched::add_task([] {
			hw::uart::write("Executing Task 2\n\r");
	}, stack2, sizeof(stack1));

	os::kern::sched::start();

    while (true);
}
