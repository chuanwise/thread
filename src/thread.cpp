#include <thread/thread>
#include <sys/mman.h>
#include <unistd.h>
#include <sched.h>
#include <sys/signal.h>
#include <sys/wait.h>
#include <errno.h>
#include <string>
#include <cstring>

// 8M
#define STACK_SIZE 1024 * 1024 * 8

thread::thread_exception::thread_exception(std::string message) noexcept: message(message) {
}

const char *thread::thread_exception::what() const noexcept {
    return message.c_str();
}

thread::thread::thread(int (*action)(void*)) {
    void *stack = (void *)mmap(NULL, STACK_SIZE,
                               PROT_READ | PROT_WRITE,
                               MAP_PRIVATE | MAP_ANONYMOUS | MAP_ANON, //| MAP_GROWSDOWN ,
                               -1,
                               0);

    if (MAP_FAILED == stack) {
        throw ::thread::thread_exception(std::string("Fail to call 'mmap': ") + std::strerror(errno));
    }

    pid_t pid = clone(action,
                      (void *)((unsigned char *)stack + STACK_SIZE),
                      CLONE_VM | CLONE_FS | CLONE_THREAD | CLONE_FILES | CLONE_SIGHAND | SIGCHLD,
                      (void *)NULL);

    if (pid == -1) {
        throw ::thread::thread_exception(std::string("Fail to call 'clone': ") + std::strerror(errno));
    }

    this->pid = pid;
    this->running = true;
}

bool thread::thread::is_running() {
    if (running) {
        running = kill(pid, 0) == 0;
    }

    return running;
}

thread::thread::~thread() {
}