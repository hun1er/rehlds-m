/*
 * SPDX-FileCopyrightText: Valve Corporation.
 * SPDX-License-Identifier: GPL-3.0
 */

#pragma once

#ifdef _WIN32
  #include "util/windows/system/crash_dumper.hpp"
  #include "util/windows/system/error.hpp"
  #include "util/windows/system/gdi.hpp"
  #include "util/windows/system/module.hpp"
  #include "util/windows/system/ntdll.hpp"
  #include "util/windows/system/process.hpp"
#else
  #include "util/linux/system/error.hpp"
  #include "util/linux/system/module.hpp"
  #include "util/linux/system/process.hpp"
#endif
