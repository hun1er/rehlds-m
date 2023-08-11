/*
 * SPDX-FileCopyrightText: Valve Corporation.
 * SPDX-License-Identifier: GPL-3.0
 */

#pragma once

#ifdef _WIN32
  #include "util/windows/console.hpp"
#else
  #include "util/linux/console.hpp"
#endif
