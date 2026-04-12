#include "stacktracing.hpp"

#include <fstream>
#include <iostream>
#include <cpptrace/cpptrace.hpp>
#include <cpptrace/from_current.hpp>

void stacktrace_start()
{
    #ifdef DEBUG_SYMBOLS
    std::ofstream* crash = new std::ofstream("crash_really_bad.txt");

    std::cerr.rdbuf(crash->rdbuf());

    cpptrace::register_terminate_handler();
    #endif
}

void stacktrace_dump()
{
    #ifdef DEBUG_SYMBOLS
    std::ofstream out("crash.txt");

    cpptrace::generate_trace().print(out);
    cpptrace::generate_trace().print();
    #endif
}
