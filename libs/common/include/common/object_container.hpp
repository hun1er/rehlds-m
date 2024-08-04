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

#include "common/platform.hpp"
#include <cstddef>

namespace Common {
    /**
     * @brief Abstract base class for object containers.
     *
     * Represents an interface for managing collections of objects.
     */
    class NO_VTABLE ObjectContainer {
      public:
        /**
         * @brief Default constructor.
         */
        ObjectContainer() = default;

        /**
         * @brief Move constructor.
         */
        ObjectContainer(ObjectContainer&&) = default;

        /**
         * @brief Copy constructor.
         */
        ObjectContainer(const ObjectContainer&) = default;

        /**
         * @brief Move assignment operator.
         */
        ObjectContainer& operator=(ObjectContainer&&) = default;

        /**
         * @brief Copy assignment operator.
         */
        ObjectContainer& operator=(const ObjectContainer&) = default;

        /**
         * @brief Virtual destructor.
         */
        FORCE_STACK_ALIGN virtual ~ObjectContainer() = default;

        /**
         * @brief Initializes the object container.
         */
        virtual void init() = 0;

        /**
         * @brief Adds an object to the container.
         *
         * @param object Pointer to the object being added.
         *
         * @return Returns \c true if the object was added successfully, \c false otherwise.
         */
        virtual bool add(void* object) = 0;

        /**
         * @brief Removes an object from the container.
         *
         * @param object Pointer to the object being removed.
         *
         * @return Returns \c true if the object was removed successfully, \c false otherwise.
         */
        virtual bool remove(void* object) = 0;

        /**
         * @brief Clears the container.
         *
         * @param free_elements_memory Specifies whether to free the memory occupied by the objects.
         */
        virtual void clear(bool free_elements_memory) = 0;

        /**
         * @brief Retrieves the first object in the container.
         *
         * @return Pointer to the first object in the container.
         */
        virtual void* first() = 0;

        /**
         * @brief Retrieves the next object in the container.
         *
         * @return Pointer to the next object in the container.
         */
        virtual void* next() = 0;

        /**
         * @brief Retrieves the size of the container.
         *
         * @return The size of the container (number of objects).
         */
        [[nodiscard]] virtual std::size_t size() const = 0;

        /**
         * @brief Checks if the container contains the specified object.
         *
         * @param object Pointer to the object being checked.
         *
         * @return Returns \c true if the object is present in the container, \c false otherwise.
         */
        virtual bool contains(void* object) = 0;

        /**
         * @brief Checks if the container is empty.
         *
         * @return Returns \c true if the container is empty, \c false otherwise.
         */
        [[nodiscard]] virtual bool empty() const = 0;
    };
}
