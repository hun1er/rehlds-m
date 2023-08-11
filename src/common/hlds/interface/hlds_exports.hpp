/*
 * SPDX-FileCopyrightText: Valve Corporation.
 * SPDX-License-Identifier: GPL-3.0
 */

#pragma once

#include "common/interface.hpp"
#include "common/platform.hpp"

namespace common
{
    class NO_VTABLE HldsExports : public CommonInterface
    {
      public:
        static constexpr auto* INTERFACE_NAME = "VENGINE_DEDICATEDEXPORTS_API_VERSION001";
        virtual void print(const char* text) = 0;
    };
}
