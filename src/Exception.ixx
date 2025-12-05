/*
* Copyright (c) 2025-present Henri Michelon
 *
 * This software is released under the MIT License.
 * https://opensource.org/licenses/MIT
*/
module;
#ifdef _WIN32
#include <windows.h>
#endif
export module lysa.exception;

import std;

export namespace lysa {

    /**
     * @brief Lightweight exception type.
     *
     * This class is a small convenience wrapper around std::exception that
     * stores a formatted message assembled from any streamable arguments.
     * In debug builds on Windows, if a debugger is attached, the message is
     * also sent to the debugger output and a debug break is triggered to help
     * you stop exactly at the throw site.
     */
    class Exception final : public std::exception {
    public:
        /**
         * @brief Construct an Exception from streamable parts.
         *
         * The message is created by streaming all provided arguments into an
         * internal std::ostringstream, in order. Each argument must be
         * stream-insertable (operator<< available).
         *
         * In debug builds, the resulting message may be emitted to the
         * debugger output and a debug trap/break may be triggered.
         *
         * @param args Variadic list of streamable argument types.
         */
        template <typename... Args>
        explicit Exception(Args&&... args) {
            std::ostringstream oss;
            (oss << ... << std::forward<Args>(args));
#ifdef _MSC_VER
            message = oss.str();
#endif
#ifdef _DEBUG
#ifdef _WIN32
            if (IsDebuggerPresent()) {
                OutputDebugStringA(message.c_str());
#endif
#ifdef __has_builtin
    __builtin_debugtrap();
#endif
#ifdef _MSC_VER
                __debugbreak();
#endif
#ifdef _WIN32
            }
#endif
#endif
        }

        /**
         * @brief Get a C-string describing the error.
         * @return Null-terminated string containing the stored message.
         */
        const char* what() const noexcept override {
            return message.c_str();
        }

    private:
        std::string message;
    };


#ifdef _DEBUG

    /**
     * @brief Debug-only assertion helper.
     *
     * Evaluates the provided callable expression. If it returns false, throws
     * lysa::Exception with a detailed message including the file and line.
     * This function is available only in debug builds.
     *
     * @param expression Callable evaluated for the condition.
     * @param message Short human-readable description of the expectation.
     * @param loc Source location automatically filled at call site.
     */
    template<typename Expr>
    constexpr void assert(
        Expr&& expression,
        const std::string message,
        const std::source_location& loc = std::source_location::current()) {
        if (!expression()) {
            throw Exception("Assertion failed: ", message, ", file ", loc.file_name(), ", line ", loc.line());
        }
    }

#else

    /**
     * @brief No-op placeholder for release builds.
     */
    template<typename Expr>
    constexpr void assert_expr(
        Expr&&,
        const std::string_view,
        const std::source_location& = std::source_location::current()) noexcept { }

#endif
}

