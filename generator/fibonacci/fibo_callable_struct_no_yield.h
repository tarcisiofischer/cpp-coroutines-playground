#ifndef COROUTINES_CALLABLE_STRUCT_FIBO_NO_YIELD_H
#define COROUTINES_CALLABLE_STRUCT_FIBO_NO_YIELD_H

#include <iostream>

// This is *NOT* an example of C++20 coroutines

namespace fibo_callable_struct_no_yield {

struct fibo {
    auto operator()() {
        auto fcurr_ = fcurr;
        auto fnext_ = fnext;

        fcurr = fnext;
        fnext = fcurr_ + fnext_;

        return fcurr_;
    }

private:
    int fcurr = 0;
    int fnext = 1;
};

auto run_example()
{
    auto f = fibo{};
    for (auto i = 0; i < 10; ++i) {
        std::cout << f() << ", ";
    }
    std::cout << '\n';
}

}

#endif //COROUTINES_CALLABLE_STRUCT_FIBO_NO_YIELD_H
