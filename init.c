#include <signal.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>

/* A postponed function call. */
/* TODO: Not currently used, but this functionality might come in handy. */
struct callback {
    unsigned long options; /* 0 = immediate, 1 = deferred */
    void *arg;
    void (*fn)(void*);
};

/* Write a message to the given file and invoke the optional callback. */
void _debug_msg(int fd, const char *message, struct callback *cb) {

    /* Write the message to the file. */
    while (*message && -1 != write(fd, ++message, 1));

    /* Invoke the callback. */
    if (cb != NULL)
        cb->fn(cb->arg);
}

void debug_msg(int signum, siginfo_t *si, ucontext_t *context) {
    const char *error = "\ndivision by zero!\n";

    _debug_msg(STDERR_FILENO, error, NULL);

    exit(-1);
}

void init(void) __attribute__((constructor));
void init(void) {

    int pagesize = getpagesize();

    /* Setup a new stack. */
    // TODO: allocate a larger stack?
    void *st;
    posix_memalign(&st, pagesize, pagesize);
    mprotect(st, pagesize, 7); // set no-execute
    stack_t s = {
        .ss_sp = st,
        .ss_flags = 0,
        .ss_size = pagesize,
    };
    sigaltstack(&s, NULL);

    // quad word (on 64-bit) stack canary; future proof in case we move away from a 32-bit platform
    // TODO: check this value is not corrupted later
    uint64_t canary[] = {
        7526411282926321713,
        5828653450055004008,
        5584688988035320147,
        7346483829405260183,
    };
    int i;
    for (i = 0; i < 4; i++)
        ((uint64_t*)st)[i] = canary[i];

    /* Install a signal handler for division by zero. */
    struct sigaction sa = {
        .sa_sigaction = (void(*)(int,siginfo_t*,void*))_debug_msg,
        .sa_flags = SA_SIGINFO|SA_ONSTACK,
    };
    sigaction(SIGFPE, &sa, NULL);
    sigaction(SIGSEGV, &sa, NULL); //FIXME: will print division error on fail...

}
