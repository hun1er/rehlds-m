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

#include "common/engine/engine_wrapper.hpp"
#include "core/cmdline_args.hpp"
#include "core/cmdline_processor.hpp"
#include "core/init.hpp"
#include "model/console_commands.hpp"
#include "model/server_loop.hpp"
#include "model/userinput_history.hpp"
#include "presenter/input_presenter.hpp"
#include "presenter/output_presenter.hpp"
#include "view/console_input.hpp"
#include "view/console_view.hpp"
#include <memory>

namespace {
    int run_server_loop(const Core::CmdLineArgs& cmdline_args, const std::shared_ptr<View::BaseView>& view)
    {
        auto& engine = Common::EngineWrapper::get_instance();
        auto* system_interface = engine.get_interface_system();
        auto& serverapi_interface = *engine.get_interface_or_abort<Common::EngineInterface::server_api>();

        const auto server_loop = std::make_shared<Model::ServerLoop>();
        Core::init_server_loop(cmdline_args, server_loop);

        const auto console_commands = std::make_shared<Model::ConsoleCommands>(system_interface);
        const auto input_history = std::make_shared<Model::UserInputHistory>("input_history.txt");

        const auto input_presenter =
          std::make_shared<Presenter::InputPresenter>(server_loop, input_history, console_commands);

        const auto output_presenter = std::make_unique<Presenter::OutputPresenter>(server_loop, view);
        const auto console_input = std::make_unique<View::ConsoleInput>(input_presenter);

        server_loop->run(serverapi_interface);
        serverapi_interface.shutdown();

        return 0;
    }
}

int main(const int argc, const char** const argv)
{
    auto cmdline_args = Core::CmdLineArgs::from_command_line(argc, argv);
    const auto console_view = std::make_shared<View::ConsoleView>();

    Core::init_logger(cmdline_args, console_view);
    const Core::CmdLineProcessor cmdline_processor{};
    cmdline_processor.process(cmdline_args);

    Core::init_locale();
    Core::init_socket();
    Core::init_filesystem();
    Core::init_engine(cmdline_args);

    return run_server_loop(cmdline_args, console_view);
}
