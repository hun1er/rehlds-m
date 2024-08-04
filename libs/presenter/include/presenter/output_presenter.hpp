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

#pragma once

#include "model/server_loop.hpp"
#include "view/base_view.hpp"
#include <cassert>
#include <memory>
#include <utility>

namespace Presenter {
    /**
     * @brief A presenter class that handles the communication between the ServerLoop and the View in an MVP pattern.
     */
    class OutputPresenter final {
      public:
        /**
         * @brief Type alias for a shared pointer to a ServerLoop object.
         */
        using ServerLoopPtr = std::shared_ptr<Model::ServerLoop>;

        /**
         * @brief Type alias for a shared pointer to a View object.
         */
        using ViewPtr = std::shared_ptr<View::BaseView>;

        /**
         * @brief Constructs an OutputPresenter object.
         *
         * @param server_loop A shared pointer to a ServerLoop object.
         * @param view A shared pointer to a View object.
         */
        OutputPresenter(ServerLoopPtr server_loop, ViewPtr view);

      private:
        /// A shared pointer to a ServerLoop object.
        ServerLoopPtr server_loop_{};

        /// A shared pointer to a View object.
        ViewPtr view_{};

        /// Updates the server status in the view when a ServerLoopEvent::status_updated event occurs.
        void update_server_status(Model::ServerLoopEvent event) const;
    };

    inline OutputPresenter::OutputPresenter(ServerLoopPtr server_loop, ViewPtr view) :
      server_loop_(std::move(server_loop)),
      view_(std::move(view))
    {
        assert(server_loop_ != nullptr);
        assert(view_ != nullptr);

        server_loop_->append_listener([this](const Model::ServerLoopEvent event) {
            update_server_status(event);
        });
    }

    inline void OutputPresenter::update_server_status(const Model::ServerLoopEvent event) const
    {
        if (Model::ServerLoopEvent::status_updated == event) {
            const auto& [fps, num_players, max_players, current_map] = server_loop_->get_status();
            view_->display_server_status(fps, num_players, max_players, current_map);
        }
    }
}
