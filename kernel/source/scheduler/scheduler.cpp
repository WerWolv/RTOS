#include <kern/scheduler/scheduler.hpp>

#include <kern/container/linked_list.hpp>


#include <hw/stm32f7/stm32f769xx.h>
#include <hw/stm32f7/uart.hpp>

os::kern::cont::List<os::kern::sched::Task> tasks;
os::kern::cont::ListIterator<os::kern::sched::Task> curr_task(nullptr);

namespace os::kern::sched {

	void do_context_switch();

	struct StackFrame {
		u32 R0, R1, R2, R3, R12, LR, PC, XPSR;
	};

	struct StackSettings {
		u32 lr, control;
	};

	extern "C" u32 next_task_stack(u32 curr_sp) {
		curr_task->stack_pointer = curr_sp;
		++curr_task;

		return reinterpret_cast<u32>(curr_task->stack_pointer);
	}

	extern "C"
	[[gnu::naked]]
	void pend_sv_handler() {
		do_context_switch();
	}

	void task_exit_handler() {
		hw::uart::write("Task ended\n\r");
		while (true);
	}


	[[gnu::naked]]
	void do_context_switch() {
		__asm volatile
		        (
				"mrs      r1, msp\n"						// Move Stack Pointer into R1
				"mov      r2, lr\n"							// Move Link Register into R2
				"mrs      r3, control\n"					// Move control core register into R3
				"stmdb    r1!, { r2, r3, r4 - r11 }\n" 		// Push LR, control and other non-saved register to old stack
				"mov      r0, r1\n"							// Move Link Register value to R0
				"switch:  bl next_task_stack\n"				// Replace stack pointer
				"ldmia    r0!, { r2, r3, r4 - r11 }\n"		// Pop LR, control and other non-saved registers from new stack
				"msr      control, r3\n"					// Restore control register
				"isb\n"										// Instruction barrier, make sure there's nothing in the pipeline anymore
				"mov      lr, r2\n"							// Restore LR
				"msr      msp, r0\n"						// Restore SP
				"bx       lr\n"								// Jump back to where we left off in new task

		        ::
		    );
	}

	void add_task(TaskFunction func, u8 *stack, size_t stack_size) {
		Task task;

		task.stack_pointer = reinterpret_cast<u32>(stack) + stack_size;

		// Initialize stack frame of entry function
		task.stack_pointer -= sizeof(StackFrame);
		auto &stack_frame = *reinterpret_cast<StackFrame*>(task.stack_pointer);

		// Set PC to point to entry function and LR to point to exit handler
		stack_frame.PC = reinterpret_cast<u32>(func);
		stack_frame.LR = reinterpret_cast<u32>(task_exit_handler);
		stack_frame.XPSR = xPSR_T_Msk;	// Make sure Thumb mode is enabled

		// Make room for auxilary registers
		task.stack_pointer -= 8 * sizeof(u32);

		// Put exception return values onto stack
		task.stack_pointer -= sizeof(StackSettings);
		auto &stack_settings = *reinterpret_cast<StackSettings*>(task.stack_pointer);

		// Switch back to Handler mode after exception return
		stack_settings.lr = 0xFFFFFFF9;

		// Disable FPU
		stack_settings.control = 0x00;

		tasks.push_back(task);
	}

	void start() {
		curr_task = tasks.begin();
		auto stack_pointer = curr_task->stack_pointer;

		__asm volatile (
			"mov r0, %0\n"					// Move SP of first task to R0
			"ldmia r0!, { r2, r3 }\n"		// Load exception LR and control register from stack into R2 and R3
			"msr   msp, r0\n"           	// Set current SP to SP of first task
			"msr   control, r3\n"        	// Switch to Handler or Thread Mode
			"isb\n"							// Instruction barrier
			"pop   { r4 - r11 }\n"			// Pop auxiliary registers from stack
			"pop   { r0 - r3, r12, lr }\n"  // Pop standard registers from stack
			"pop   { pc }\n"             	// Pop PC from stack to jump to entry of first task
			: "=r"(stack_pointer)
		);

	}

	void yield() {
		hw::uart::write("Yield\n\r");
		SCB->ICSR = SCB_ICSR_PENDSVSET_Msk;
	}

}
