#include <coroutine>
#include <iostream>

/**
 * This file is meant to be a starting point for studying coroutines. I suggest reading the links on the README.md file
 * in this repo while trying to read, understand and hack the code below.
 */

class MyFuture {
public:
    struct promise_type {
        MyFuture get_return_object() {
            std::cout << "MyFuture::promise_type::get_return_object()\n";
            return MyFuture{Handle::from_promise(*this)};
        }

        static std::suspend_always initial_suspend() noexcept {
            std::cout << "MyFuture::promise_type::initial_suspend()\n";
            return {};
        }

        static std::suspend_always final_suspend() noexcept {
            std::cout << "MyFuture::promise_type::final_suspend()\n";
            return {};
        }

        /* Only necessary if you use "co_yield" */
        std::suspend_always yield_value(int value) noexcept
        {
            std::cout << "MyFuture::promise_type::yield_value()\n";
            current_value = value;
            return {};
        }

        void return_value(int value) noexcept
        {
            std::cout << "MyFuture::promise_type::return_value()\n";
            current_value = value;
        }

        void await_transform() noexcept
        {
            std::cout << "MyFuture::promise_type::await_transform()\n";
        }

        [[noreturn]]
        static void unhandled_exception() { throw; }

        // Not really necessary, just an example.
        int current_value;
    };

    using Handle = std::coroutine_handle<promise_type>;

    /* Not used in this example. Doesn't mean they are not necessary for your case! */
    MyFuture() = delete;
    MyFuture(const MyFuture &) = delete;
    MyFuture &operator=(const MyFuture &) = delete;
    MyFuture &operator=(MyFuture &&other) noexcept = delete;

    explicit MyFuture(const Handle &coroutine)
            : m_coroutine{coroutine}
    {
        std::cout << "MyFuture::MyFuture(const Handle coroutine)\n";
    }

    MyFuture(MyFuture &&other) noexcept
            : m_coroutine{other.m_coroutine}
    {
        std::cout << "MyFuture::MyFuture(MyFuture&&)\n";
        other.m_coroutine = {};
    }

    ~MyFuture()
    {
        std::cout << "MyFuture::~MyFuture()\n";
        if (m_coroutine) {
            m_coroutine.destroy();
        }
    }

    /* User defined API to work with the coroutine */
    int get()
    {
        m_coroutine.resume();
        return m_coroutine.promise().current_value;
    }

private:
    Handle m_coroutine;
};

MyFuture my_coroutine()
{
    int a = 1;

    std::cout << "my_coroutine()> [" << a << "]\n";
    a++;

    // co_yield 7;
    co_yield []() -> int {
        std::cout << "my_coroutine().anon lambda 1\n";
        return 7;
    }();

    // Local stack kept between coroutine calls
    std::cout << "my_coroutine()> [" << a << "]\n";
    co_return []() -> int {
        std::cout << "my_coroutine().anon lambda 2\n";
        return 42;
    }();
}

int main()
{
    std::cout << "main()> [1]\n";
    auto f = my_coroutine();
    std::cout << "main()> [2]\n";
    std::cout << f.get() << '\n';
    std::cout << "main()> [3]\n";
    std::cout << f.get() << '\n';
    std::cout << "main()> [4]\n";

    return 0;
}
