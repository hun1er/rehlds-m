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

#include "util/windows/system/error.hpp"
#include <array>

namespace Util {
    std::string get_error_string(const ::DWORD error_code, const ::WORD lang, const ::WORD sublang)
    {
        constexpr std::size_t buffer_size = 1024;
        std::array<::CHAR, buffer_size> buffer{};

        ::FormatMessage(
          FORMAT_MESSAGE_FROM_SYSTEM, nullptr, error_code, static_cast<DWORD>(MAKELANGID(lang, sublang)), buffer.data(),
          buffer.size(), nullptr);

        return std::string{buffer.data()};
    }
}
