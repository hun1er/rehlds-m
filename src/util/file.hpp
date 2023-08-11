/*
 * SPDX-FileCopyrightText: Valve Corporation.
 * SPDX-License-Identifier: GPL-3.0
 */

#pragma once

#include <filesystem>
#include <string>
#include <string_view>
#include <system_error>
#include <vector>

namespace util
{
    /**
     * @brief An enumeration of possible error codes for file reading operations.
     */
    enum class FileReadErrorCode
    {
        success,       ///< No error occurred.
        not_exist,     ///< The file does not exist.
        open_error,    ///< The file could not be opened.
        size_invalid,  ///< The file size is invalid (negative or zero).
        size_exceeded, ///< The file size exceeds the maximum allowed size.
        read_error,    ///< The file could not be read successfully.
        close_error    ///< The file could not be closed successfully.
    };

    /**
     * @brief An enumeration of possible error codes for file writing operations.
     */
    enum class FileWriteErrorCode
    {
        success,     ///< No error occurred.
        open_error,  ///< The file could not be opened for writing.
        write_error, ///< The file could not be written successfully.
        close_error  ///< The file could not be closed successfully.
    };

    /**
     * @brief Reads the content of a file into a string.
     *
     * @param path The path to the file to be read.
     * @param content A reference to a string where the content of the file will be stored if successful.
     *
     * @return A \c FileReadErrorCode indicating the result of the operation.
     */
    FileReadErrorCode file_read(const std::string& path, std::string& content) noexcept;

    /**
     * @brief Writes a string to a file, overwriting its existing content.
     *
     * @param path The path to the file to be written to.
     * @param content The string to be written to the file.
     * @param append A boolean flag indicating whether the content should be appended
     * to the end of the file or overwrite its existing content.
     *
     * @return A \c FileWriteErrorCode indicating the result of the operation.
     */
    FileWriteErrorCode file_write(const std::string& path, const std::string& content, bool append = false) noexcept;

    /**
     * @brief Writes a sequence of lines to a file, overwriting its existing content or appending to it.
     *
     * @param path The path to the file to be written to.
     * @param lines A vector of strings representing the lines to be written to the file.
     * @param append A boolean flag indicating whether the content should be appended
     * to the end of the file or overwrite its existing content.
     *
     * @return A \c FileWriteErrorCode indicating the result of the operation.
     */
    FileWriteErrorCode file_write_lines(const std::string& path, const std::vector<std::string>& lines,
                                        bool append = false) noexcept;

    /**
     * @brief Removes a file.
     *
     * @param path The path to the file to be removed.
     *
     * @return A \c std::error_code indicating the result of the operation.
     */
    std::error_code file_remove(std::string_view path) noexcept;

    /**
     * @brief Reads the content of a file into a string and logs any errors.
     *
     * @param path The path to the file to be read.
     * @param content A reference to a string where the content of the file will be stored if successful.
     *
     * @return \c true if the operation was successful, \c false otherwise.
     */
    bool try_file_read(const std::string& path, std::string& content) noexcept;

    /**
     * @brief Writes content to a file, overwriting its existing content and logs any errors.
     *
     * @param path The path to the file to be written to.
     * @param content The content to be written to the file.
     * @param append A boolean flag indicating whether the content should be appended
     * to the end of the file or overwrite its existing content.
     *
     * @return \c true if the file_write operation was successful, \c false otherwise.
     */
    bool try_file_write(const std::string& path, const std::string& content, bool append = false) noexcept;

    /**
     * @brief Writes a sequence of lines to a file, overwriting its existing content or appending to it, and logs any
     * errors.
     *
     * @param path The path to the file to be written to.
     * @param lines A vector of strings representing the lines to be written to the file.
     * @param append A boolean flag indicating whether the content should be appended
     * to the end of the file or overwrite its existing content.
     *
     * @return \c true if the file_write operation was successful, \c false otherwise.
     */
    bool try_file_write_lines(const std::string& path, const std::vector<std::string>& lines,
                              bool append = false) noexcept;

    /**
     * @brief Removes a file and logs any errors.
     *
     * @param path The path to the file to be removed.
     *
     * @return true if the removal operation was successful, false otherwise.
     */
    bool try_file_remove(std::string_view path) noexcept;

    /**
     * @brief Checks if a file file_exists.
     *
     * @param path The path to the file to be checked.
     *
     * @return \c true if the file file_exists, \c false otherwise.
     */
    [[nodiscard]] inline bool file_exists(const std::string_view path) noexcept
    {
        return (!path.empty()) && std::filesystem::exists(path);
    }
}
