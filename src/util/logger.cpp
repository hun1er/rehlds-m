/*
 * SPDX-FileCopyrightText: Valve Corporation.
 * SPDX-License-Identifier: GPL-3.0
 */

#include "util/logger.hpp"
#include "abstract/view.hpp"
#include "util/string.hpp"
#include <memory>
#include <mutex>
#include <spdlog/common.h>
#include <spdlog/formatter.h>
#include <spdlog/logger.h>
#include <spdlog/sinks/base_sink.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <unordered_map>
#include <vector>

namespace spdlog::sinks
{
    class sink;
}

namespace
{
    util::LogSettings logger_settings{};
    std::shared_ptr<spdlog::logger> logger{};
    std::shared_ptr<spdlog::sinks::sink> view_sink{};
    std::shared_ptr<spdlog::sinks::sink> file_sink{};

    class Formatter final : public spdlog::formatter
    {
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
            const auto& prefix = (it == prefixes.end()) ? "" : util::str::format("[{}] ", it->second);
            const auto& formatted_msg = util::str::format("{}{}", prefix, msg.payload);
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

    template <typename Mutex>
    class ViewSink final : public spdlog::sinks::base_sink<Mutex>
    {
      public:
        explicit ViewSink(std::shared_ptr<abstract::View> view) : view_(std::move(view))
        {
        }

      protected:
        void sink_it_(const spdlog::details::log_msg& msg) override
        {
            spdlog::memory_buf_t formatted;
            spdlog::sinks::base_sink<Mutex>::formatter_->format(msg, formatted);
            view_->print(fmt::to_string(formatted));
        }

        void flush_() override
        {
            // The view must implement this
        }

      private:
        std::shared_ptr<abstract::View> view_;
    };
}

namespace util
{
    void log_init(const LogSettings& settings)
    {
        logger_settings = settings;
        spdlog::drop_all();

        std::vector<spdlog::sink_ptr> sinks{};
        constexpr std::size_t max_sinks = 2;
        sinks.reserve(max_sinks);

        if ((settings.output & LogOutput::view) == LogOutput::view) {
            view_sink = std::make_shared<ViewSink<std::mutex>>(settings.view);
            sinks.emplace_back(view_sink);
        }

        if ((settings.output & LogOutput::file) == LogOutput::file) {
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

    void log_init(const bool con_debug, std::string logfile, std::shared_ptr<abstract::View> view)
    {
        LogSettings settings{};
        settings.view = std::move(view);

        if (con_debug) {
            settings.logfile = std::move(logfile);
            settings.output = LogOutput::view_and_file;
        }

        log_init(settings);
    }

    const LogSettings& log_settings()
    {
        return logger_settings;
    }
}
