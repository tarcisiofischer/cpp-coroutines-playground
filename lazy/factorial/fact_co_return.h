#ifndef COROUTINES_FACT_CO_RETURN_H
#define COROUTINES_FACT_CO_RETURN_H

#include <coroutine>
#include <iostream>

namespace fact_co_return {

namespace detail {

template<typename T>
class Lazy {
public:
    struct promise_type {
        Lazy get_return_object() {
            return Lazy{Handle::from_promise(*this)};
        }

        static std::suspend_always initial_suspend() noexcept {
            return {};
        }

        static std::suspend_always final_suspend() noexcept {
            return {};
        }

        void return_value(T value) {
            current_value = value;
        }

        // Disallow co_await
        void await_transform() = delete;

        [[noreturn]]
        static void unhandled_exception() { throw; }

        T current_value;
    };

    using Handle = std::coroutine_handle<promise_type>;

    Lazy() = default;
    Lazy(const Lazy &) = delete;
    Lazy &operator=(const Lazy &) = delete;
    Lazy &operator=(Lazy &&other) noexcept = delete;

    explicit Lazy(const Handle coroutine)
            : m_coroutine{coroutine}
    {}

    Lazy(Lazy &&other) noexcept
            : m_coroutine{other.m_coroutine}
    {
        other.m_coroutine = {};
    }

    T get()
    {
        m_coroutine.resume();
        return m_coroutine.promise().current_value;
    }

    ~Lazy()
    {
        if (m_coroutine) {
            m_coroutine.destroy();
        }
    }

private:
    Handle m_coroutine;
};

}

detail::Lazy<int> factorial(int n)
{
    co_return [=]() {
        auto res = 1;
        for (int i = 2; i <= n; ++i) {
            res *= i;
        }
        return res;
    }();
}

auto run_example()
{
    auto f = factorial(10);
    std::cout << "Will this be called before?\n";
    std::cout << f.get() << '\n';
}

}

#endif //COROUTINES_FACT_CO_RETURN_H
