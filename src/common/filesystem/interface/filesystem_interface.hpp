/*
 * SPDX-FileCopyrightText: Valve Corporation.
 * SPDX-License-Identifier: GPL-3.0
 */

#pragma once

#include "common/interface.hpp"
#include "common/platform.hpp"

namespace common
{
    using FileHandle = void*;
    using FileFindHandle = int;
    using WaitForResourcesHandle = int;

    enum class FileSystemSeek
    {
        head = 0,
        current,
        tail
    };

    enum class FileWarningLevel
    {
        /**
         * @brief Don't print anything.
         */
        quiet = 0,

        /**
         * @brief On shutdown, report names of files left unclosed.
         */
        report_unclosed,

        /**
         * @brief Report number of times a file was opened, closed.
         */
        report_usage,

        /**
         * @brief Report all open/close events to console (slow).
         */
        report_all_accesses
    };

    constexpr auto FILESYSTEM_INVALID_FIND_HANDLE = -1;
    constexpr void* FILESYSTEM_INVALID_HANDLE = nullptr;

    /**
     * @brief Main file system interface.
     */
    class NO_VTABLE FileSystemInterface : public CommonInterface
    {
      public:
        using WarningFn = void (*)(const char* format, ...);

        /**
         * @brief The string name of this interface.
         */
        static constexpr auto* INTERFACE_NAME = "VFileSystem009";

        /**
         * @brief Mount filesystem.
         */
        virtual void mount() = 0;

        /**
         * @brief Unmount the filesystem.
         */
        virtual void unmount() = 0;

        /**
         * @brief Remove all search paths (including write path?).
         */
        virtual void remove_all_search_paths() = 0;

        /**
         * @brief Add paths in priority order (mod dir, game dir, etc).
         *
         * @note If one or more .pak files are in the specified directory,
         * then they are added after the file system path.
         *
         * @note If the path is the relative path to a .bsp file, then any previous .bsp file
         * override is cleared and the current .bsp is searched for an embedded PAK file
         * and this file becomes the highest priority search path (i.e., it's looked at first
         * even before the mod's file system path).
         */
        virtual void add_search_path(const char* path, const char* path_id) = 0;

        virtual bool remove_search_path(const char* path) = 0;

        /**
         * @brief Deletes a file.
         */
        virtual void remove_file(const char* relative_path, const char* path_id) = 0;

        /**
         * @brief This isn't implementable on STEAM as is.
         */
        virtual void create_dir_hierarchy(const char* path, const char* path_id) = 0;

        virtual bool file_exists(const char* filename) = 0;

        virtual bool is_directory(const char* filename) = 0;

        /**
         * @brief Opens a file.
         *
         * If \c path_id is \c nullptr, all paths will be searched for the file.
         */
        virtual FileHandle open(const char* filename, const char* options, const char* path_id) = 0;

        virtual void close(FileHandle file) = 0;

        virtual void seek(FileHandle file, int position, FileSystemSeek type) = 0;

        virtual unsigned int tell(FileHandle file) = 0;

        virtual unsigned int size(FileHandle file) = 0;

        virtual unsigned int size(const char* filename) = 0;

        virtual long get_filetime(const char* filename) = 0;

        virtual void filetime_to_string(char* strip, int max_chars_including_terminator, long filetime) = 0;

        virtual bool is_ok(FileHandle file) = 0;

        virtual void flush(FileHandle file) = 0;

        virtual bool end_of_file(FileHandle file) = 0;

        virtual int read(void* output, int size, FileHandle file) = 0;

        virtual int write(const void* input, int size, FileHandle file) = 0;

        virtual char* read_line(char* output, int max_chars, FileHandle file) = 0;

        virtual int print(FileHandle file, char* format, ...) = 0;

        /**
         * @brief Direct filesystem buffer access.
         *
         * Returns a handle to a buffer containing the file data.
         * This is the optimal way to access the complete data for a file,
         * since the file preloader has probably already got it in memory.
         */
        virtual void* get_read_buffer(FileHandle file, int* out_buffer_size, bool fail_if_not_in_cache) = 0;

        virtual void release_read_buffer(FileHandle file, void* read_buffer) = 0;

        virtual const char* find_first(const char* wild_card, FileFindHandle* handle, const char* path_id) = 0;

        virtual const char* find_next(FileFindHandle handle) = 0;

        virtual bool find_is_directory(FileFindHandle handle) = 0;

        virtual void find_close(FileFindHandle handle) = 0;

        virtual void get_local_copy(const char* filename) = 0;

        virtual const char* get_local_path(const char* filename, char* local_path, int local_path_buffer_size) = 0;

        virtual char* parse_file(char* file_bytes, char* token, bool* was_quoted) = 0;

        /**
         * @brief Returns true on success (based on current list of search paths,
         * otherwise false if it can't be resolved).
         */
        virtual bool full_path_to_relative_path(const char* full_path, char* relative) = 0;

        /**
         * @brief Gets the current working directory.
         */
        virtual bool get_current_directory(char* directory, int max_length) = 0;

        /**
         * @brief Dump to printf/OutputDebugString the list of files that have not been closed.
         */
        virtual void print_opened_files() = 0;

        virtual void set_warning_func(WarningFn warning_func) = 0;

        virtual void set_warning_level(FileWarningLevel level) = 0;

        virtual void log_level_load_started(const char* name) = 0;

        virtual void log_level_load_finished(const char* name) = 0;

        virtual int hint_resource_need(const char* hint_list, int forget_everything) = 0;

        virtual int pause_resource_preloading() = 0;

        virtual int resume_resource_preloading() = 0;

        virtual int set_buffer(FileHandle stream, char* buffer, int mode, long size) = 0;

        virtual void get_interface_version(char* buffer, int max_length) = 0;

        virtual bool is_file_immediately_available(const char* filename) = 0;

        /**
         * @brief Starts waiting for resources to be available.
         *
         * @note Returns 0 if there is nothing to wait on.
         */
        virtual WaitForResourcesHandle wait_for_resources(const char* resource_list) = 0;

        /**
         * @brief Get progress on waiting for resources; progress is a float [0, 1],
         * complete is true on the waiting being done.
         *
         * @note Returns false if no progress is available.
         *
         * @note Any calls after complete is \c true or on an invalid handle will return \c false, \c 0.0f, \c true.
         */
        virtual bool get_wait_for_resources_progress(WaitForResourcesHandle handle, float* progress /* out */,
                                                     bool* complete /* out */) = 0;

        /**
         * @brief Cancels a progress call.
         */
        virtual void cancel_wait_for_resources(WaitForResourcesHandle handle) = 0;

        /**
         * @brief returns \c true if the \c app_id has all its caches fully preloaded.
         */
        virtual bool is_app_ready_for_offline_play(int app_id) = 0;

        /**
         * @brief Interface for custom pack files > 4Gb.
         */
        virtual bool add_pack_file(const char* full_path, const char* path_id) = 0;

        /**
         * @brief Open a file but force the data to come from the steam cache, NOT from disk.
         */
        virtual FileHandle open_from_cache_for_read(const char* filename, const char* options, const char* path_id) = 0;

        virtual void add_search_path_no_write(const char* path, const char* path_id) = 0;
    };
}
