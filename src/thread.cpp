#include <thread/thread>
#include <sys/mman.h>
#include <unistd.h>
#include <sched.h>
#include <sys/signal.h>
#include <sys/wait.h>

// 8M
#define STACK_SIZE 1024 * 1024 * 8

thread::thread_exception::thread_exception(std::string message) noexcept: message(message) {
}

const char *thread::thread_exception::what() const noexcept {
    return message.c_str();
}

thread::thread::thread(std::function<int(void *)> action) {
    void *stack = (void *)mmap(NULL, STACK_SIZE,
                               PROT_READ | PROT_WRITE,
                               MAP_PRIVATE | MAP_ANONYMOUS | MAP_ANON, //| MAP_GROWSDOWN ,
                               -1,
                               0);

    if (MAP_FAILED == stack) {
        throw ::thread::thread_exception("Fail to call 'mmap'! ");
    }

    pid_t pid = clone(action.target<int(void *)>(),
                             (void *)((unsigned char *)stack + STACK_SIZE),
                             CLONE_VM | CLONE_FS | CLONE_THREAD | CLONE_FILES | CLONE_SIGHAND | SIGCHLD,
                             (void *)NULL);

    if (pid == -1) {
        throw ::thread::thread_exception("Failed to call 'clone'! ");
    }

    this->pid = pid;
    this->state = ::thread::thread_state::RUNNING;
}

void thread::thread::join() {
    if (get_state() != ::thread::thread_state::RUNNING) {
        return;
    }

    int status;
    pid_t result = waitpid(pid, &status, 0);
    if (result == -1) {
        throw ::thread::thread_exception("Fail to call 'waitpid'! ");
    } else if (result != 0) {
        if (WIFEXITED(status)) {
            this->status = WEXITSTATUS(status);
            this->state = ::thread::thread_state::EXITED;
        } else if (WIFSIGNALED(status)) {
            this->status = WTERMSIG(status);
            this->state = ::thread::thread_state::TERMINATED;
        }
    }
}

thread::thread_state thread::thread::get_state() {
    if (state == ::thread::thread_state::RUNNING) {
        int status;
        pid_t result = waitpid(pid, &status, WNOHANG);
        if (result == -1) {
            throw ::thread::thread_exception("Fail to call 'waitpid'! ");
        } else if (result != 0) {
            if (WIFEXITED(status)) {
                this->status = WEXITSTATUS(status);
                this->state = ::thread::thread_state::EXITED;
            } else if (WIFSIGNALED(status)) {
                this->status = WTERMSIG(status);
                this->state = ::thread::thread_state::TERMINATED;
            }
        }
    }

    return state;
}

void thread::thread::kill() {
    if (get_state() != ::thread::thread_state::RUNNING) {
        throw ::thread::thread_exception("Thread is not running! ");
    }

    int ret = ::kill(pid, SIGKILL);
    if (ret == -1) {
        throw ::thread::thread_exception("Fail to call 'kill'! ");
    }
}

thread::thread::~thread() {
    if (get_state() == ::thread::thread_state::RUNNING) {
        int ret = ::kill(pid, SIGKILL);
        if (ret == -1) {
            throw ::thread::thread_exception("Fail to call 'kill'! ");
        }
    }
}