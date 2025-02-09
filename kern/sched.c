#include <inc/assert.h>
#include <inc/x86.h>
#include <kern/spinlock.h>
#include <kern/monitor.h>
#include "inc/env.h"
#include "inc/stdio.h"
#include "inc/types.h"
#include "kern/cpu.h"
#include "kern/env.h"
#include "inc/log.h"
#include "kern/pmap.h"

void sched_halt(void);

// Choose a user environment to run and run it.
void
sched_yield(void)
{
	DEBUG("CPU %d enter scheduler\n", thiscpu->cpu_id);
	struct Env *idle;

	// Implement simple round-robin scheduling.
	//
	// Search through 'envs' for an ENV_RUNNABLE environment in
	// circular fashion starting just after the env this CPU was
	// last running.  Switch to the first such environment found.
	//
	// If no envs are runnable, but the environment previously
	// running on this CPU is still ENV_RUNNING, it's okay to
	// choose that environment.
	//
	// Never choose an environment that's currently running on
	// another CPU (env_status == ENV_RUNNING). If there are
	// no runnable environments, simply drop through to the code
	// below to halt the cpu.

	// LAB 4: Your code here.
	struct Env *env, *orig_env;
	size_t orig_idx, next_env_idx;

	env = NULL;
	orig_idx = next_env_idx = 0;
	if (thiscpu->cpu_env && !envid2env(thiscpu->cpu_env->env_id, &orig_env, true) && orig_env) {
		orig_idx = orig_env - envs;
		next_env_idx = orig_idx + 1;
	}

	do {
		env = &envs[next_env_idx];
		if (env->env_status == ENV_RUNNABLE) 
			break;
		next_env_idx = (next_env_idx + 1) % NENV;
	} while (next_env_idx != orig_idx);

	DEBUG("env = %08x, status = %d\n", env->env_id, env->env_status);
	if (env && env->env_status == ENV_RUNNABLE)  {
		DEBUG("CPU %d exit scheduler\n", thiscpu->cpu_id);
		env_run(env);
	} else if (orig_env && orig_env->env_status == ENV_RUNNING) {
		DEBUG("scheduler run last env %08x\n", orig_env->env_id);
		DEBUG("CPU %d exit scheduler\n", thiscpu->cpu_id);
		env_run(orig_env);
	}
	// sched_halt never returns
	DEBUG("cpu %d halt\n", thiscpu->cpu_id);
	DEBUG("CPU %d exit scheduler\n", thiscpu->cpu_id);
	sched_halt();
}

// Halt this CPU when there is nothing to do. Wait until the
// timer interrupt wakes it up. This function never returns.
//
void
sched_halt(void)
{
	int i;

	// For debugging and testing purposes, if there are no runnable
	// environments in the system, then drop into the kernel monitor.
	for (i = 0; i < NENV; i++) {
		if ((envs[i].env_status == ENV_RUNNABLE ||
		     envs[i].env_status == ENV_RUNNING ||
		     envs[i].env_status == ENV_DYING))
			break;
	}
	if (i == NENV) {
		cprintf("No runnable environments in the system!\n");
		while (1)
			monitor(NULL);
	}

	// Mark that no environment is running on this CPU
	curenv = NULL;
	lcr3(PADDR(kern_pgdir));

	// Mark that this CPU is in the HALT state, so that when
	// timer interupts come in, we know we should re-acquire the
	// big kernel lock
	xchg(&thiscpu->cpu_status, CPU_HALTED);

	// Release the big kernel lock as if we were "leaving" the kernel
	unlock_kernel();

	// Reset stack pointer, enable interrupts and then halt.
	asm volatile (
		"movl $0, %%ebp\n"
		"movl %0, %%esp\n"
		"pushl $0\n"
		"pushl $0\n"
		// Uncomment the following line after completing exercise 13
		"sti\n"
		"1:\n"
		"hlt\n"
		"jmp 1b\n"
	: : "a" (thiscpu->cpu_ts.ts_esp0));
}

