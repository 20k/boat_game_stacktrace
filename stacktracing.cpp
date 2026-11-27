#include "stacktracing.hpp"

#include <fstream>
#include <iostream>
#include <cpptrace/cpptrace.hpp>
#include <cpptrace/from_current.hpp>
#include <signal.h>
#include <assert.h>
#include <atomic>

static std::atomic_bool crashed{false};

void (*on_crash)(bool) = nullptr;

std::atomic_bool& crashed_by_exception()
{
    static std::atomic_bool d{false};
    return d;
}

#ifdef _WIN32
#include <windows.h>

LONG WINAPI
VectoredHandle(
    struct _EXCEPTION_POINTERS *ExceptionInfo
)
{
    if(crashed)
        return EXCEPTION_CONTINUE_SEARCH;

    stacktrace_dump();
    crashed = true;

    if(on_crash)
        on_crash(crashed_by_exception());

    exit(0);
    return EXCEPTION_CONTINUE_SEARCH;
};
#endif

void signal_handler(int signum)
{
    ::signal(signum, SIG_DFL);

    stacktrace_dump();
    crashed = true;

    if(on_crash)
        on_crash(crashed_by_exception());

    ::raise(SIGABRT);

    exit(0);
}

void stacktrace_start()
{
    crashed_by_exception() = false;

    #ifdef DEBUG_SYMBOLS
    std::ofstream* crash = new std::ofstream("crash_really_bad.txt");
    std::cerr.rdbuf(crash->rdbuf());

    ::signal(SIGSEGV, &signal_handler);
    ::signal(SIGABRT, &signal_handler);
    ::signal(SIGINT, &signal_handler);
    ::signal(SIGILL, &signal_handler);
    ::signal(SIGFPE, &signal_handler);

    cpptrace::register_terminate_handler();

    #ifdef _WIN32
    #ifdef VECTOR
    AddVectoredExceptionHandler(1, VectoredHandle);
    #endif
    #endif
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

void stacktrace_set_on_crash(void(*ptr)(bool))
{
    on_crash = ptr;
}

void stacktrace_set_crashed_by_exception(bool status)
{
    crashed_by_exception() = status;
}
