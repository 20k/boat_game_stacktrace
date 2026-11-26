#ifndef STACKTRACING_HPP_INCLUDED
#define STACKTRACING_HPP_INCLUDED

#include <cpptrace/from_current.hpp>

void stacktrace_start();
void stacktrace_dump();
void stacktrace_exception_dump();
void stacktrace_set_on_crash(void(*ptr)());

#endif // STACKTRACING_HPP_INCLUDED
