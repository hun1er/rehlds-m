/*
 * SPDX-FileCopyrightText: Valve Corporation.
 * SPDX-License-Identifier: GPL-3.0
 */

#include "common/hlds/interface/hlds_exports.hpp"
#include "common/interface.hpp"
#include "common/platform.hpp"
#include "util/logger.hpp"

namespace
{
    class Exports final : public common::HldsExports
    {
      public:
        FORCE_STACK_ALIGN void print(const char* text) override;
    };

    void Exports::print(const char* const text)
    {
        if ((text != nullptr) && (*text != '\0')) {
            util::log_info(text);
        }
    }
}

EXPOSE_SINGLE_INTERFACE(Exports, common::HldsExports::INTERFACE_NAME)
