#include <thread/thread>
#include <atomic>
#include <optional>
#include <unistd.h>

class non_blocking_queue {
public:

    std::optional<int> poll() noexcept {
        node* head = this->head.load();
        if (head == nullptr) {
            return std::nullopt;
        }

        return head->value;
    }

    void push(int value) noexcept {
        node* n = new node();

        n->value = value;

        do {
            n->next = head.load();
        } while (!head.compare_exchange_strong(n->next, n));
    }

    bool empty() const noexcept {
        return head.load() == nullptr;
    }

    ~non_blocking_queue() noexcept {
        node* curr = this->head.load();
        while (curr != nullptr) {
            node *next = curr->next;
            delete curr;
            curr = next;
        }
    }

protected:
    struct node {
        int value;
        node *next;
    };

    std::atomic<node *> head = nullptr;
};

non_blocking_queue queue;

int producer(void* ignored) {
    for (int i = 0; i < 50; i++) {
        // sleep(1000);
        printf("[P] producing '%d'\n", i);
        queue.push(i);
    }

    printf("[P] producer exited! \n");

    return 0;
}

int consumer(void* ignored) {
    while (true) {
        std::optional<int> poll;
        do {
            poll = queue.poll();
        } while (!poll.has_value());

        printf("[C] consuming '%d'\n", poll.value());
    }

    return 0;
}

int main() {
    thread::thread consumer_thread(consumer);
    thread::thread producer_thread(producer);

    while (producer_thread.get_state() == thread::thread_state::RUNNING) {
        sleep(1000);
        printf("[M] waiting...\n");
    }
}