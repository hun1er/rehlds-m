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

#include "util/file.hpp"
#include "util/logger.hpp"
#include <algorithm>
#include <fstream>
#include <iterator>

namespace {
    /** @brief 100 MB in bytes */
    constexpr auto MAX_FILE_SIZE = static_cast<std::streamoff>(100 * 1024 * 1024);

    bool handle_write_error(const Util::FileWriteErrorCode error_code, const std::string& path) noexcept
    {
        switch (error_code) {
            case Util::FileWriteErrorCode::success: {
                return true;
            }
            case Util::FileWriteErrorCode::open_error: {
                Util::log_error("Failed to open file '{}' for writing.", path);
                return false;
            }
            case Util::FileWriteErrorCode::write_error: {
                Util::log_error("Failed to file_write to file '{}'.", path);
                return false;
            }
            case Util::FileWriteErrorCode::close_error: {
                Util::log_error("Failed to close file '{}'.", path);
                return false;
            }
            default: {
                return false;
            }
        }
    }
}

namespace Util {
    FileReadErrorCode file_read(const std::string& path, std::string& content) noexcept
    {
        if (!file_exists(path)) {
            return FileReadErrorCode::not_exist;
        }

        constexpr auto mode = std::ios::in | std::ios::ate | std::ios::binary;
        std::ifstream file_stream{path, mode};

        if (!file_stream.is_open()) {
            return FileReadErrorCode::open_error;
        }

        const auto file_size = file_stream.tellg();

        if (-1 == file_size) {
            return FileReadErrorCode::size_invalid;
        }

        if (file_size > MAX_FILE_SIZE) {
            return FileReadErrorCode::size_exceeded;
        }

        file_stream.seekg(0);
        content.resize(static_cast<std::string::size_type>(file_size));
        file_stream.read(content.data(), static_cast<std::streamsize>(file_size));

        if (file_stream.fail() || file_stream.bad()) {
            return FileReadErrorCode::read_error;
        }

        if (file_stream.close(); file_stream.fail()) {
            return FileReadErrorCode::close_error;
        }

        return FileReadErrorCode::success;
    }

    FileWriteErrorCode file_write(const std::string& path, const std::string& content, const bool append) noexcept
    {
        const auto mode = std::ios::out | (append ? std::ios::app : std::ios::trunc);
        std::ofstream file_stream{path, mode};

        if (!file_stream.is_open()) {
            return FileWriteErrorCode::open_error;
        }

        if (file_stream << content; file_stream.fail() || file_stream.bad()) {
            return FileWriteErrorCode::write_error;
        }

        if (file_stream.close(); file_stream.fail()) {
            return FileWriteErrorCode::close_error;
        }

        return FileWriteErrorCode::success;
    }

    FileWriteErrorCode
      file_write_lines(const std::string& path, const std::vector<std::string>& lines, const bool append) noexcept
    {
        const auto mode = std::ios::out | (append ? std::ios::app : std::ios::trunc);
        std::ofstream file_stream{path, mode};

        if (!file_stream.is_open()) {
            return FileWriteErrorCode::open_error;
        }

        const std::ostream_iterator<std::string> output_iterator{file_stream, "\n"};
        std::copy(lines.begin(), lines.end(), output_iterator);

        if (file_stream.fail() || file_stream.bad()) {
            return FileWriteErrorCode::write_error;
        }

        if (file_stream.close(); file_stream.fail()) {
            return FileWriteErrorCode::close_error;
        }

        return FileWriteErrorCode::success;
    }

    std::error_code file_remove(const std::string_view path) noexcept
    {
        std::error_code error_code{};
        std::filesystem::remove(path, error_code);

        return error_code;
    }

    bool try_file_read(const std::string& path, std::string& content) noexcept
    {
        switch (file_read(path, content)) {
            case FileReadErrorCode::success: {
                return true;
            }
            case FileReadErrorCode::not_exist: {
                log_error("File '{}' does not exist.", path);
                return false;
            }
            case FileReadErrorCode::open_error: {
                log_error("Failed to open file '{}' for reading.", path);
                return false;
            }
            case FileReadErrorCode::size_invalid: {
                log_error("Failed to determine size of file '{}'.", path);
                return false;
            }
            case FileReadErrorCode::size_exceeded: {
                log_error("File '{}' size exceeds maximum allowed size.", path);
                return false;
            }
            case FileReadErrorCode::read_error: {
                log_error("Failed to read file '{}'.", path);
                return false;
            }
            case FileReadErrorCode::close_error: {
                log_error("Failed to close file '{}'.", path);
                return false;
            }
            default: {
                return false;
            }
        }
    }

    bool try_file_write(const std::string& path, const std::string& content, const bool append) noexcept
    {
        const auto error_code = file_write(path, content, append);
        return handle_write_error(error_code, path);
    }

    bool
      try_file_write_lines(const std::string& path, const std::vector<std::string>& lines, const bool append) noexcept
    {
        const auto error_code = file_write_lines(path, lines, append);
        return handle_write_error(error_code, path);
    }

    bool try_file_remove(std::string_view path) noexcept
    {
        const auto error_code = file_remove(path);

        if (!static_cast<bool>(error_code)) {
            return true;
        }

        log_error(
          "Failed to file_remove file '{}': error code {} - {}", path, error_code.value(), error_code.message());

        return false;
    }
}
