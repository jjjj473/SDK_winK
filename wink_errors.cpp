// wink_errors.cpp - enhanced error handling utilities
#include <string>
#include <vector>
#include <mutex>
#include <fstream>
#include <stdexcept>

extern "C" {
    const char* wink_last_error();
    int wink_last_error_code();
    int wink_error_count();
    const char* wink_error_at(int index);
    void wink_clear_errors();
    int wink_run_catch(int (*func)(void));
}

enum WinkErrorCode {
    WINK_ERR_NONE = 0,
    WINK_ERR_EXCEPTION = 1,
};

static thread_local std::string last_error;
static thread_local WinkErrorCode last_code = WINK_ERR_NONE;
static thread_local std::vector<std::string> error_stack;
static std::mutex log_mutex;

static void push_error(const std::string& msg, WinkErrorCode code)
{
    last_error = msg;
    last_code = code;
    if (error_stack.size() >= 32)
        error_stack.erase(error_stack.begin());
    error_stack.push_back(msg);
    std::lock_guard<std::mutex> lock(log_mutex);
    std::ofstream out("wink_errors.log", std::ios::app);
    out << msg << "\n";
}

extern "C" const char* wink_last_error()
{
    return last_error.c_str();
}

extern "C" int wink_last_error_code()
{
    return static_cast<int>(last_code);
}

extern "C" int wink_error_count()
{
    return static_cast<int>(error_stack.size());
}

extern "C" const char* wink_error_at(int index)
{
    if (index < 0 || index >= static_cast<int>(error_stack.size()))
        return "";
    return error_stack[index].c_str();
}

extern "C" void wink_clear_errors()
{
    last_error.clear();
    last_code = WINK_ERR_NONE;
    error_stack.clear();
}

extern "C" int wink_run_catch(int (*func)(void))
{
    if (!func)
        return 0;
    try {
        return func();
    } catch (const std::exception& e) {
        push_error(e.what(), WINK_ERR_EXCEPTION);
        return -1;
    } catch (...) {
        push_error("unknown exception", WINK_ERR_EXCEPTION);
        return -1;
    }
}

// Example function demonstrating usage
static int throw_example_inner()
{
    throw std::runtime_error("wink example error");
    return 0;
}

extern "C" int wink_throw_example()
{
    return wink_run_catch(throw_example_inner);
}
