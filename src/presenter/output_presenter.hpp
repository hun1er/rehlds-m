/*
 * SPDX-FileCopyrightText: Valve Corporation.
 * SPDX-License-Identifier: GPL-3.0
 */

#pragma once

#include "abstract/view.hpp"
#include "model/server_loop.hpp"
#include <cassert>
#include <memory>
#include <utility>

namespace presenter
{
    /**
     * @brief A presenter class that handles the communication between the ServerLoop and the View in an MVP pattern.
     */
    class OutputPresenter final
    {
      public:
        /**
         * @brief Type alias for a shared pointer to a ServerLoop object.
         */
        using ServerLoopPtr = std::shared_ptr<model::ServerLoop>;

        /**
         * @brief Type alias for a shared pointer to a View object.
         */
        using ViewPtr = std::shared_ptr<abstract::View>;

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
        void update_server_status(model::ServerLoopEvent event) const;
    };

    inline OutputPresenter::OutputPresenter(ServerLoopPtr server_loop, ViewPtr view) :
      server_loop_(std::move(server_loop)),
      view_(std::move(view))
    {
        assert(server_loop_ != nullptr);
        assert(view_ != nullptr);

        server_loop_->append_listener([this](const model::ServerLoopEvent event) {
            update_server_status(event);
        });
    }

    inline void OutputPresenter::update_server_status(const model::ServerLoopEvent event) const
    {
        if (model::ServerLoopEvent::status_updated == event) {
            const auto& [fps, num_players, max_players, current_map] = server_loop_->get_status();
            view_->display_server_status(fps, num_players, max_players, current_map);
        }
    }
}
