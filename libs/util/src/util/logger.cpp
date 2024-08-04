/*
 * Half Life 1 SDK License
 * Copyright(c) Valve Corp
 *
 * DISCLAIMER OF WARRANTIES. THE HALF LIFE 1 SDK AND ANY OTHER MATERIAL
 * DOWNLOADED BY LICENSEE IS PROVIDED "AS IS". VALVE AND ITS SUPPLIERS
 * DISCLAIM ALL WARRANTIES WITH RESPECT TO THE SDK, EITHER EXPRESS OR IMPLIED,
 * INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF MERCHANTABILITY,
 * NON-INFRINGEMENT, TITLE AND FITNESS FOR A PARTICULAR PURPOSE.
 *
 * LIMITATION OF LIABILITY. IN NO EVENT SHALL VALVE OR ITS SUPPLIERS BE LIABLE
 * FOR ANY SPECIAL, INCIDENTAL, INDIRECT, OR CONSEQUENTIAL DAMAGES WHATSOEVER
 * (INCLUDING, WITHOUT LIMITATION, DAMAGES FOR LOSS OF BUSINESS PROFITS,
 * BUSINESS INTERRUPTION, LOSS OF BUSINESS INFORMATION, OR ANY OTHER PECUNIARY
 * LOSS) ARISING OUT OF THE USE OF OR INABILITY TO USE THE ENGINE AND/OR THE
 * SDK, EVEN IF VALVE HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
 *
 * For commercial use, contact: sourceengine@valvesoftware.com
 */

#include "util/logger.hpp"
#include "util/string.hpp"
#include <spdlog/common.h>
#include <spdlog/formatter.h>
#include <spdlog/logger.h>
#include <spdlog/sinks/base_sink.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <memory>
#include <mutex>
#include <unordered_map>
#include <vector>

namespace spdlog::sinks {
    class sink;
}

namespace {
    Util::LogSettings logger_settings{};
    std::shared_ptr<spdlog::logger> logger{};
    std::shared_ptr<spdlog::sinks::sink> log_sink{};
    std::shared_ptr<spdlog::sinks::sink> file_sink{};

    class Formatter final : public spdlog::formatter {
      public:
        void format(const spdlog::details::log_msg& msg, spdlog::memory_buf_t& dest) override
        {
            static const std::unordered_map<spdlog::level::level_enum, const char*> prefixes = {
              {spdlog::level::debug,    "DEBUG"      },
              {spdlog::level::warn,     "WARNING"    },
              {spdlog::level::err,      "ERROR"      },
              {spdlog::level::critical, "FATAL ERROR"}
            };

            const auto& it = prefixes.find(msg.level);
            const auto& prefix = (it == prefixes.end()) ? "" : Util::str::format("[{}] ", it->second);
            const auto& formatted_msg = Util::str::format("{}{}", prefix, msg.payload);
            dest.append(formatted_msg.data(), formatted_msg.data() + formatted_msg.size());

            if (msg.level != spdlog::level::info) {
                constexpr auto* newline = "\n";
                dest.append(newline, newline + 1);
            }

#ifdef _WIN32
            if (!prefix.empty()) {
                msg.color_range_start = 0;
                msg.color_range_end = prefix.size() - 1;
            }
#endif
        }

        [[nodiscard]] std::unique_ptr<formatter> clone() const override
        {
            return spdlog::details::make_unique<Formatter>();
        }
    };

    /**
     * @brief Custom sink for spdlog that uses a LogOutput for message handling.
     */
    template <typename Mutex>
    class LogOutputSink final : public spdlog::sinks::base_sink<Mutex> {
      public:
        /**
         * @brief Construct a new Custom Sink object.
         *
         * @param log_output The LogOutput instance to handle the actual logging.
         */
        explicit LogOutputSink(std::shared_ptr<Util::LogOutput> log_output) : log_output_(std::move(log_output))
        {
        }

      protected:
        /**
         * @brief Process a log message.
         *
         * Called by spdlog to process each log message.
         * It formats the message and passes it to the LogOutput instance.
         *
         * @param msg The log message to be processed.
         */
        void sink_it_(const spdlog::details::log_msg& msg) override
        {
            spdlog::memory_buf_t formatted;
            spdlog::sinks::base_sink<Mutex>::formatter_->format(msg, formatted);
            log_output_->write_log(fmt::to_string(formatted));
        }

        /**
         * @brief Flush the sink.
         *
         * Called by spdlog when the sink needs to be flushed.
         */
        void flush_() override
        {
            // The view must implement this
        }

      private:
        ///< The LogOutput instance handling the actual logging.
        std::shared_ptr<Util::LogOutput> log_output_;
    };
}

namespace Util {
    void log_init(const LogSettings& settings)
    {
        logger_settings = settings;
        spdlog::drop_all();

        std::vector<spdlog::sink_ptr> sinks{};
        constexpr std::size_t max_sinks = 2;
        sinks.reserve(max_sinks);

        if ((settings.output & LogDestination::view) == LogDestination::view) {
            log_sink = std::make_shared<LogOutputSink<std::mutex>>(settings.log_output);
            sinks.emplace_back(log_sink);
        }

        if ((settings.output & LogDestination::file) == LogDestination::file) {
            file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(settings.logfile);
            sinks.emplace_back(file_sink);
        }

        logger = std::make_shared<spdlog::logger>("main", sinks.cbegin(), sinks.cend());
        logger->flush_on(static_cast<spdlog::level::level_enum>(settings.flush_on));
        logger->set_level(static_cast<spdlog::level::level_enum>(settings.level));
        logger->set_formatter(std::make_unique<Formatter>());

        register_logger(logger);
        set_default_logger(logger);
    }

    void log_init(const bool con_debug, std::string logfile, std::shared_ptr<LogOutput> log_output)
    {
        LogSettings settings{};
        settings.log_output = std::move(log_output);

        if (con_debug) {
            settings.logfile = std::move(logfile);
            settings.output = LogDestination::view_and_file;
        }

        log_init(settings);
    }

    const LogSettings& log_settings()
    {
        return logger_settings;
    }
}
