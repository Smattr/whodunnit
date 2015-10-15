#include <signal.h>
#include <stdio.h>
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

    /* Install a signal handler for division by zero. */
    struct sigaction sa = {
        .sa_sigaction = debug_msg,
        .sa_flags = SA_SIGINFO,
    };
    sigaction(SIGFPE, &sa, NULL);

}
