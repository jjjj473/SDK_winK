// wink_errors.cpp - C++ helper for error messages
#include <string>
#include <stdexcept>

extern "C" {
    const char* wink_last_error();
}

static thread_local std::string last_error;

void set_error(const std::string& msg) {
    last_error = msg;
}

extern "C" const char* wink_last_error()
{
    return last_error.c_str();
}

// Example throwing function
extern "C" int wink_throw_example()
{
    try {
        throw std::runtime_error("wink example error");
    } catch (const std::exception& e) {
        set_error(e.what());
        return 1;
    }
    return 0;
}
