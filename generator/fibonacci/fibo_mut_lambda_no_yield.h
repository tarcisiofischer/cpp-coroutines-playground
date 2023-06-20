#ifndef COROUTINES_FIBO_MUT_LAMBDA_NO_YIELD_H
#define COROUTINES_FIBO_MUT_LAMBDA_NO_YIELD_H

#include <iostream>

// This is *NOT* an example of C++20 coroutines

namespace fibo_mut_lambda_no_yield {

auto fibo()
{
    auto fcurr = 0;
    auto fnext = 1;
    return [=]() mutable {
        auto fcurr_ = fcurr;
        auto fnext_ = fnext;
        fcurr = fnext;
        fnext = fcurr_ + fnext_;
        return fcurr_;
    };
}

auto run_example()
{
    auto f = fibo();
    for (auto i = 0; i < 10; ++i) {
        std::cout << f() << ", ";
    }
    std::cout << '\n';
}

}

#endif //COROUTINES_FIBO_MUT_LAMBDA_NO_YIELD_H
