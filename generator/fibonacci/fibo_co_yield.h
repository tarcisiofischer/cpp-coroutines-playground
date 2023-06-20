#ifndef COROUTINES_FIBO_YIELD_H
#define COROUTINES_FIBO_YIELD_H

#include <coroutine>
#include <iostream>
#include <optional>

// Based on https://en.cppreference.com/w/cpp/coroutine/coroutine_handle#Example

namespace fibo_co_yield {

namespace detail {

template<typename T>
class Generator {
public:
    struct promise_type {
        Generator get_return_object() {
            return Generator{Handle::from_promise(*this)};
        }

        static std::suspend_always initial_suspend() noexcept {
            return {};
        }

        static std::suspend_always final_suspend() noexcept {
            return {};
        }

        std::suspend_always yield_value(T& value) noexcept {
            current_value = std::move(value);
            return {};
        }

        // Disallow co_await in generator coroutines.
        void await_transform() = delete;

        [[noreturn]]
        static void unhandled_exception() { throw; }

        T current_value;
    };

    using Handle = std::coroutine_handle<promise_type>;

    Generator() = default;
    Generator(const Generator &) = delete;
    Generator &operator=(const Generator &) = delete;
    Generator &operator=(Generator &&other) noexcept = delete;

    explicit Generator(const Handle coroutine)
            : m_coroutine{coroutine}
    {}

    Generator(Generator &&other) noexcept
            : m_coroutine{other.m_coroutine}
    {
        other.m_coroutine = {};
    }

    ~Generator()
    {
        if (m_coroutine) {
            m_coroutine.destroy();
        }
    }

    // Range-based for loop support.
    class Iter
    {
    public:
        void operator++()
        {
            m_coroutine.resume();
        }
        const int& operator*() const
        {
            return m_coroutine.promise().current_value;
        }
        bool operator==(std::default_sentinel_t) const
        {
            return !m_coroutine || m_coroutine.done();
        }

        explicit Iter(const Handle coroutine) :
                m_coroutine{coroutine}
        {}

    private:
        Handle m_coroutine;
    };

    Iter begin()
    {
        if (m_coroutine)
            m_coroutine.resume();
        return Iter{m_coroutine};
    }

    std::default_sentinel_t end() { return {}; }

private:
    Handle m_coroutine;
};

}

detail::Generator<int> fibo(int last) {
    auto fcurr = 0;
    auto fnext = 1;
    auto n = 0;
    while (n < last) {
        co_yield fcurr;
        auto tmp = fcurr;
        fcurr = fnext;
        fnext = tmp + fnext;
        n++;
    }
}

auto run_example()
{
    for (auto i : fibo(10)) {
        std::cout << i << ", ";
    }
    std::cout << '\n';
}

}

#endif //COROUTINES_FIBO_YIELD_H
