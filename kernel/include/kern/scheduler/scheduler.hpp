#pragma once

#include <kern.hpp>

namespace os::kern::sched {

	struct Task {
		u32 stack_pointer;
	};

	using TaskFunction = void(*)();

	void add_task(TaskFunction func, u8 *stack, size_t stack_size);

	void start();

	void yield();
}
