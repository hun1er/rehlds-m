/*
 * SPDX-FileCopyrightText: Valve Corporation.
 * SPDX-License-Identifier: GPL-3.0
 */

#pragma once

#ifdef _WIN32
  #include "console/windows/console_view.hpp"
#else
  #include "console/linux/console_view.hpp"
#endif
