/*
 * Copyright (c) 2024-present Henri Michelon
 * 
 * This software is released under the MIT License.
 * https://opensource.org/licenses/MIT
*/
export module lysa.log;
#include <cstdio>

import std;
import lysa.enums;

export namespace lysa {

#ifdef DISABLE_LOG

    constexpr bool ENABLE_LOG = false;

    class LogStream {
    public:
        inline LogStream(LogLevel level) {};
    private:
    };

#else

    /// @brief Indicates at compile-time that logging is enabled.
    constexpr bool ENABLE_LOG = true;

    /**
     * @brief Stream buffer responsible for formatting and emitting messages.
     *
     * Prefixes the first write of a line with a timestamp and the log level,
     * then relays the content to stdout and/or a file depending on the
     * application configuration (see lysa::Application::getConfiguration).
     */
    class LogStreamBuf : public std::streambuf {
    public:
        /**
         * @brief Write a block of characters into the buffer.
         * @param s Pointer to the characters to write.
         * @param n Number of characters to write.
         * @return The number of characters consumed (n in practice).
         */
        std::streamsize xsputn(const char* s, std::streamsize n) override;

        /**
         * @brief Overload invoked to push a single character.
         * @param c Character to push.
         * @return The character pushed, or EOF on failure.
         */
        int_type overflow(int_type c) override;

        /// @brief Set the level associated with outgoing messages.
        void setLevel(const LogLevel level) { this->level = level; }

    private:
        /// @brief Current stream log level.
        LogLevel level{LogLevel::ERROR};
        /// @brief Whether the next write starts a new line.
        bool newLine{true};
        /**
         * @brief Finish formatting and dispatch the message to active sinks.
         */
        void log(const std::string& message);
    };

    /**
     * @brief Output stream for a given @ref LogLevel.
     */
    class LogStream : public std::ostream {
    public:
        /**
         * @brief Construct a stream bound to a given level.
         * @param level Log level of messages written to this stream.
         */
        LogStream(LogLevel level);
    private:
        /// @brief Internal buffer that performs formatting and output.
        LogStreamBuf logStreamBuf;
    };

#endif

    /**
     * @brief Global manager exposing one stream per log level.
     *
     * The effective outputs (stdout/file) are controlled by the application
     * configuration.
     */
    struct Log {
        /// @name Streams per level
        /// @{
        LogStream trace{LogLevel::TRACE};
        LogStream _internal{LogLevel::INTERNAL};
        LogStream debug{LogLevel::DEBUG};
        LogStream info{LogLevel::INFO};
        LogStream game1{LogLevel::GAME1};
        LogStream game2{LogLevel::GAME2};
        LogStream game3{LogLevel::GAME3};
        LogStream warning{LogLevel::WARNING};
        LogStream error{LogLevel::ERROR};
        LogStream critical{LogLevel::CRITICAL};
        /// @}

        /// @brief Log file handle when file output is active.
        FILE* logFile;

        /**
         * @brief Initialize logging infrastructure and open required sinks.
         * @param log Shared instance holding the per-level streams.
         * @note Writes a "START OF LOG" entry upon initialization.
         */
        static void open(const std::shared_ptr<Log>&log);
        /**
         * @brief Close the infrastructure and associated files.
         * @note Writes an "END OF LOG" entry before closing.
         */
        static void close();
        /// @brief Pointer to the active global @ref Log instance.
        static inline std::shared_ptr<Log> loggingStreams{nullptr};
    };

    /**
     * @brief Compile-time switch indicating whether logging produces output.
     * @return true when logging helpers emit; false when compiled out.
     */
    consteval bool isLoggingEnabled() {
        return ENABLE_LOG;
    }

    /**
     * @brief Internal helper to write to the INTERNAL stream.
     * @param args Elements to write.
     */
    template <typename... Args>
    void _LOG(Args... args) { if constexpr (isLoggingEnabled()) { (Log::loggingStreams->_internal << ... << args) << std::endl; } }

    /**
     * @brief Emit a short trace with the calling function name and line.
     * @param location Automatically provided via std::source_location.
     */
    inline void TRACE(const std::source_location& location = std::source_location::current()) {
        if constexpr (isLoggingEnabled()) {
            Log::loggingStreams->trace << location.function_name() << " line " << location.line() << std::endl;
        }
    }

    /// @name Log helpers per level
    /// @{
    template <typename... Args>
    void DEBUG(Args... args) { if constexpr (isLoggingEnabled()) { (Log::loggingStreams->debug << ... << args) << std::endl; } }

    template <typename... Args>
    void INFO(Args... args) { if constexpr (isLoggingEnabled()) { (Log::loggingStreams->info << ... << args) << std::endl; } }

    template <typename... Args>
    void GAME1(Args... args) { if constexpr (isLoggingEnabled()) { (Log::loggingStreams->game1 << ... << args) << std::endl; } }

    template <typename... Args>
    void GAME2(Args... args) { if constexpr (isLoggingEnabled()) { (Log::loggingStreams->game2 << ... << args) << std::endl; } }

    template <typename... Args>
    void GAME3(Args... args) { if constexpr (isLoggingEnabled()) { (Log::loggingStreams->game3 << ... << args) << std::endl; } }

    template <typename... Args>
    void WARNING(Args... args) { if constexpr (isLoggingEnabled()) { (Log::loggingStreams->warning << ... << args) << std::endl; } }

    template <typename... Args>
    void ERROR(Args... args) { if constexpr (isLoggingEnabled()) { (Log::loggingStreams->error << ... << args) << std::endl; } }

    template <typename... Args>
    void CRITICAL(Args... args) { if constexpr (isLoggingEnabled()) { (Log::loggingStreams->critical << ... << args) << std::endl; } }
    /// @}

}
