/*
 * Copyright (c) 2024-present Henri Michelon
 * 
 * This software is released under the MIT License.
 * https://opensource.org/licenses/MIT
*/
export module lysa.log;
#include <cstdio>

import std;
import lysa.types;

export namespace lysa {

    /**
     * Where to log messages
     */
    enum LoggingMode : uint32 {
        //! Disable logging
        LOGGING_MODE_NONE = 0,
        /**
        * Open an external Window (on the first screen if you have multiple screen) to display the log messages.
        * Log message appearance in the Window can be deferred to the next frame if the log message is sent from a thread different from the main thread
        */
        LOGGING_MODE_WINDOW = 0x001,
        /**
         * Log the messages into a file named 'log.txt'
         */
        LOGGING_MODE_FILE = 0x010,
        /**
         * Log the messages to std::cout. WIN32 applications needs to be linked with `-mconsole`
         */
        LOGGING_MODE_STDOUT = 0x100
    };

    /**
     * Log levels
     */
    enum class LogLevel : int32 {
        TRACE       = -1,
        DEBUG       = 0,
        INFO        = 1,
        GAME1       = 2,
        GAME2       = 3,
        GAME3       = 4,
        WARNING     = 5,
        ERROR       = 6,
        CRITICAL    = 7,
        INTERNAL    = 100,
    };

    struct LoggingConfiguration {
        //! Where to log a message using Logger
        int loggingMode{LOGGING_MODE_NONE};
        //! Minimum level for the log messages
        LogLevel logLevelMin{LogLevel::INFO};
    };

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

    class LogStreamBuf : public std::streambuf {
    public:
        std::streamsize xsputn(const char* s, std::streamsize n) override;

        int_type overflow(int_type c) override;

        void setLevel(const LogLevel level) { this->level = level; }

    private:
        LogLevel level{LogLevel::ERROR};
        bool newLine{true};
        void log(const std::string& message);
    };

    class LogStream : public std::ostream {
    public:
        LogStream(LogLevel level);
    private:
        LogStreamBuf logStreamBuf;
    };

#endif

    /**
     * @brief Global manager exposing one stream per log level.
     *
     * The effective outputs (stdout/file) are controlled by the first Lysa instance configuration
     */
    class Log {
    public:
        static void _init(const LoggingConfiguration &loggingConfiguration);

        static void _shutdown();

        /**
         * @brief Compile-time switch indicating whether logging produces output.
         * @return true when logging helpers emit; false when compiled out.
         */
        static consteval bool isLoggingEnabled() {
            return ENABLE_LOG;
        }

        /**
         * @brief Internal helper to write to the INTERNAL stream.
         * @param args Elements to write.
         */
        template <typename... Args>
        static void log(Args... args) { if constexpr (isLoggingEnabled()) { (loggingStreams->_internal << ... << args) << std::endl; } }

        /**
         * @brief Emit a short trace with the calling function name and line.
         * @param location Automatically provided via std::source_location.
         */
        static void trace(const std::source_location& location = std::source_location::current()) {
            if constexpr (isLoggingEnabled()) {
                loggingStreams->_trace << location.function_name() << " line " << location.line() << std::endl;
            }
        }

        /// @name Log helpers per level
        /// @{
        template <typename... Args>
        static void debug(Args... args) { if constexpr (isLoggingEnabled()) { (loggingStreams->_debug << ... << args) << std::endl; } }

        template <typename... Args>
        static void info(Args... args) { if constexpr (isLoggingEnabled()) { (loggingStreams->_info << ... << args) << std::endl; } }

        template <typename... Args>
        static void game1(Args... args) { if constexpr (isLoggingEnabled()) { (loggingStreams->_game1 << ... << args) << std::endl; } }

        template <typename... Args>
        static void game2(Args... args) { if constexpr (isLoggingEnabled()) { (loggingStreams->_game2 << ... << args) << std::endl; } }

        template <typename... Args>
        static void game3(Args... args) { if constexpr (isLoggingEnabled()) { (loggingStreams->_game3 << ... << args) << std::endl; } }

        template <typename... Args>
        static void warning(Args... args) { if constexpr (isLoggingEnabled()) { (loggingStreams->_warning << ... << args) << std::endl; } }

        template <typename... Args>
        static void error(Args... args) { if constexpr (isLoggingEnabled()) { (loggingStreams->_error << ... << args) << std::endl; } }

        template <typename... Args>
        static void critical(Args... args) { if constexpr (isLoggingEnabled()) { (loggingStreams->_critical << ... << args) << std::endl; } }
        /// @}

        Log(const LoggingConfiguration& loggingConfiguration) : loggingConfiguration{loggingConfiguration} {}

    private:
        friend class LogStreamBuf;

        LogStream _trace{LogLevel::TRACE};
        LogStream _internal{LogLevel::INTERNAL};
        LogStream _debug{LogLevel::DEBUG};
        LogStream _info{LogLevel::INFO};
        LogStream _game1{LogLevel::GAME1};
        LogStream _game2{LogLevel::GAME2};
        LogStream _game3{LogLevel::GAME3};
        LogStream _warning{LogLevel::WARNING};
        LogStream _error{LogLevel::ERROR};
        LogStream _critical{LogLevel::CRITICAL};

        //Log file handle when file output is active.
        FILE* logFile{nullptr};

        // Actual configuration
        LoggingConfiguration loggingConfiguration;

        // Active global @ref Log instance.
        static std::unique_ptr<Log> loggingStreams;

    };

}
