#ifndef STACKTRACING_HPP_INCLUDED
#define STACKTRACING_HPP_INCLUDED

#include <cpptrace/from_current.hpp>

void stacktrace_start();
void stacktrace_dump();
void stacktrace_exception_dump();

#endif // STACKTRACING_HPP_INCLUDED
