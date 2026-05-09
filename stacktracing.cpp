#include "stacktracing.hpp"

#include <fstream>
#include <iostream>
#include <cpptrace/cpptrace.hpp>
#include <cpptrace/from_current.hpp>
#include <signal.h>

void signal_handler(int signum)
{
    ::signal(signum, SIG_DFL);

    stacktrace_dump();

    ::raise(SIGABRT);

    exit(0);
}

void stacktrace_start()
{
    #ifdef DEBUG_SYMBOLS
    std::ofstream* crash = new std::ofstream("crash_really_bad.txt");
    std::cerr.rdbuf(crash->rdbuf());

    ::signal(SIGSEGV, &signal_handler);
    ::signal(SIGABRT, &signal_handler);

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

void stacktrace_exception_dump()
{
    #ifdef DEBUG_SYMBOLS
    std::ofstream out("crash_exception.txt");

    cpptrace::from_current_exception().print(out);
    cpptrace::from_current_exception().print();
    #endif
}
