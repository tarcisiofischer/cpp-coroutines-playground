#include <generator/fibonacci/fibo_mut_lambda_no_yield.h>
#include <generator/fibonacci/fibo_callable_struct_no_yield.h>
#include <generator/fibonacci/fibo_co_yield.h>

int main()
{
    fibo_mut_lambda_no_yield::run_example();
    fibo_callable_struct_no_yield::run_example();
    fibo_co_yield::run_example();

    return 0;
}
