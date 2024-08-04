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

#pragma once

#include "util/logger.hpp"
#include <stringpy/stringpy.hpp>
#include <cassert>
#include <charconv>
#include <optional>
#include <string>
#include <utility>

namespace Util::str {
    /**
     * @brief Converts a string to a specified type.
     *
     * Attempts to convert the given string to a value of the specified type T using the given base.
     * If the conversion is successful, returns an optional containing the converted value.
     * If the conversion fails, returns an empty optional.
     *
     * @tparam T The type to convert the string to.
     *
     * @param str The string to convert.
     * @param base The base to use for the conversion.
     *
     * @return An optional containing the converted value if the conversion is successful, an empty optional otherwise.
     */
    template <typename T>
    std::optional<T> convert_to_type(const std::string_view str, int base = 10)
    {
        T result{};
        const auto [ptr, error_code] = std::from_chars(str.data(), str.data() + str.size(), result, base);

        if (error_code == std::errc()) {
            return result;
        }

        return std::nullopt;
    }

    /**
     * @brief Compares two strings for equality.
     *
     * @param str1 The first string to compare.
     * @param str2 The second string to compare.
     *
     * @return \c true if the specified strings are equal, \c false otherwise.
     */
    [[nodiscard]] inline bool equal(const std::string_view str1, const std::string_view str2) noexcept
    {
        return strpy::equal(str1, str2);
    }

    /**
     * @brief Formats a string by replacing placeholders with the given arguments.
     *
     * The function uses <tt>"{}"</tt> as a placeholder to specify where the arguments
     * should be inserted in the string to produce a formatted string.
     *
     * @tparam Args The types of the arguments to be inserted into the string.
     *
     * @param str The string containing placeholders to be replaced.
     * @param args The arguments to be inserted into the string.
     *
     * @return A formatted string with placeholders replaced by the given arguments.
     */
    template <typename... Args>
    [[nodiscard]] std::string format(const std::string_view str, Args&&... args)
    {
        return fmt::format(str, std::forward<Args>(args)...);
    }

    /**
     * @brief Checks if the given string is empty or contains only whitespace characters.
     *
     * @param str The string to check.
     *
     * @return \c true if the string is empty or contains only whitespace characters, \c false otherwise.
     */
    [[nodiscard]] inline bool is_empty_or_space(const std::string_view str) noexcept
    {
        return strpy::is_empty_or_space(str);
    }

    /**
     * @brief Checks if the given character is printable.
     *
     * @tparam Char The type of the character to check.
     *
     * @param ch The character to check.
     *
     * @return \c true if the character is printable, \c false otherwise.
     */
    template <typename Char = std::string::value_type>
    [[nodiscard]] bool is_printable_char(const Char ch) noexcept
    {
        return strpy::is_printable_char<Char>(ch);
    }

    /**
     * @brief Checks if all characters in the given string are printable characters.
     *
     * @param str The string to check.
     *
     * @return \c true if all characters in the string are printable characters, \c false otherwise.
     */
    [[nodiscard]] inline bool is_printable(const std::string_view str) noexcept
    {
        return strpy::is_printable(str);
    }

    /**
     * @brief Returns a left-justified string of a given minimum width.
     *
     * @param str The input string to justify.
     * @param length The minimum width of the resulting string.
     * @param fill_char The character used to fill the resulting string (optional).
     *
     * @return A left-justified string of minimum width \p length.
     */
    [[nodiscard]] inline std::string ljust(
      const std::string_view str, const std::size_t length,
      const std::string::value_type fill_char = strpy::SPACE) noexcept
    {
        return strpy::ljust(str, length, fill_char);
    }

    /**
     * @brief Splits a string into substrings using any space character as delimiter.
     *
     * @param str The input string to split.
     *
     * @return A vector of strings containing the substrings.
     */
    [[nodiscard]] inline std::vector<std::string> split(const std::string_view str) noexcept
    {
        return strpy::split(str);
    }

    /**
     * @brief Splits a string into lines.
     *
     * @param str The input string to split.
     * @param keep_ends If \c true, line breaks are included in the resulting lines (optional).
     *
     * @return A vector of strings containing the lines from the input string.
     */
    [[nodiscard]] inline std::vector<std::string>
      split_lines(const std::string_view str, const bool keep_ends = false) noexcept
    {
        return strpy::split_lines(str, keep_ends);
    }

    /**
     * @brief Converts a string to lowercase.
     *
     * @param str The input string to convert to lowercase.
     *
     * @return A new string where all the characters are lowercase.
     */
    [[nodiscard]] inline std::string to_lower(const std::string_view str) noexcept
    {
        return strpy::to_lower(str);
    }

    /**
     * @brief Removes leading and trailing characters from a string.
     *
     * @param str The string to remove leading and trailing characters from.
     * @param chars The characters to remove (optional).
     *
     * @return A new string with the leading and trailing characters removed.
     */
    [[nodiscard]] inline std::string
      trim(const std::string_view str, const std::string_view chars = strpy::SPACE_CHARS) noexcept
    {
        return strpy::strip<std::string>(str, chars);
    }
}
