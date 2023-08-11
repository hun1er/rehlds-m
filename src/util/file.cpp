/*
 * SPDX-FileCopyrightText: Valve Corporation.
 * SPDX-License-Identifier: GPL-3.0
 */

#include "util/file.hpp"
#include "util/logger.hpp"
#include <algorithm>
#include <fstream>
#include <iterator>

namespace
{
    /** @brief 100 MB in bytes */
    constexpr auto MAX_FILE_SIZE = static_cast<std::streamoff>(100 * 1024 * 1024);

    bool handle_write_error(const util::FileWriteErrorCode error_code, const std::string& path) noexcept
    {
        switch (error_code) {
            case util::FileWriteErrorCode::success: {
                return true;
            }
            case util::FileWriteErrorCode::open_error: {
                util::log_error("Failed to open file '{}' for writing.", path);
                return false;
            }
            case util::FileWriteErrorCode::write_error: {
                util::log_error("Failed to file_write to file '{}'.", path);
                return false;
            }
            case util::FileWriteErrorCode::close_error: {
                util::log_error("Failed to close file '{}'.", path);
                return false;
            }
            default: {
                return false;
            }
        }
    }
}

namespace util
{
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

    FileWriteErrorCode file_write_lines(const std::string& path, const std::vector<std::string>& lines,
                                        const bool append) noexcept
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

    bool try_file_write_lines(const std::string& path, const std::vector<std::string>& lines,
                              const bool append) noexcept
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

        log_error("Failed to file_remove file '{}': error code {} - {}", path, error_code.value(),
                  error_code.message());

        return false;
    }
}
