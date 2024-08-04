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

#include "core/cmdline_processor.hpp"
#include "util/file.hpp"
#include "util/logger.hpp"
#include "util/signal.hpp"
#include "util/system.hpp"
#include <utility>

namespace {
    class SteamHandler final : public Core::CmdLineArgHandler {
      public:
        void handle(Core::CmdLineArgs& args) override
        {
            args.push_front("-steam");
        }
    };

    class ConsoleHandler final : public Core::CmdLineArgHandler {
      public:
        void handle([[maybe_unused]] Core::CmdLineArgs& args) override
        {
#ifdef _WIN32
            args.push_front("-console");
#endif
        }
    };

    class TargetFpsHandler final : public Core::CmdLineArgHandler {
      public:
        void handle(Core::CmdLineArgs& args) override
        {
            if (args.contains("targetfps")) {
                args.remove("pingboost");
                args.push_back("+sys_ticrate", "0");
            }
        }
    };

    class ConDebugHandler final : public Core::CmdLineArgHandler {
      public:
        void handle(Core::CmdLineArgs& args) override
        {
            // This argument is processed during logger initialization.
            args.remove("condebug");
        }
    };

    class ConClearLogHandler final : public Core::CmdLineArgHandler {
      public:
        void handle(Core::CmdLineArgs& args) override
        {
            const auto& logfile = Util::log_settings().logfile;

            if (logfile.empty()) {
                return;
            }

            if (args.contains("conclearlog")) {
                Util::try_file_write(logfile, std::string{}, false);
            }
        }
    };

    class PidFileHandler final : public Core::CmdLineArgHandler {
      public:
        void handle(Core::CmdLineArgs& args) override
        {
            const auto pidfile = args.get_argument_option("pidfile");

            if (!pidfile.has_value() || pidfile->empty()) {
                return;
            }

            const auto process_id = Util::get_process_id();
            Util::try_file_write(*pidfile, std::to_string(process_id) + "\n");
        }
    };

    class IgnoreSigIntHandler final : public Core::CmdLineArgHandler {
      public:
        void handle(Core::CmdLineArgs& args) override
        {
            if (args.contains("ignoresigint")) {
                Util::try_set_signal_handler(SIGINT, SIG_IGN);
            }
        }
    };

    class NoMiniDumpsHandler final : public Core::CmdLineArgHandler {
      public:
        void handle([[maybe_unused]] Core::CmdLineArgs& args) override
        {
#ifdef _WIN32
            if (!args.contains("nominidumps")) {
                Util::CrashDumper::init();
            }
#endif
        }
    };
}

namespace Core {
    CmdLineProcessor::CmdLineProcessor()
    {
        add_handler(std::make_unique<SteamHandler>());
        add_handler(std::make_unique<ConsoleHandler>());
        add_handler(std::make_unique<TargetFpsHandler>());
        add_handler(std::make_unique<ConDebugHandler>());
        add_handler(std::make_unique<ConClearLogHandler>());
        add_handler(std::make_unique<PidFileHandler>());
        add_handler(std::make_unique<IgnoreSigIntHandler>());
        add_handler(std::make_unique<NoMiniDumpsHandler>());
    }

    void CmdLineProcessor::process(CmdLineArgs& args) const
    {
        for (const auto& handler : handlers) {
            handler->handle(args);
        }
    }

    void CmdLineProcessor::add_handler(std::unique_ptr<CmdLineArgHandler> handler)
    {
        handlers.emplace_back(std::move(handler));
    }
}
