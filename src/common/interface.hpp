/*
 *  SPDX-FileCopyrightText: Valve Corporation.
 *  SPDX-License-Identifier: GPL-3.0
 *
 *  This header defines the interface convention used in the valve engine.
 *
 *  To make an interface and expose it:
 *    1. Derive from CommonInterface.
 *    2. The interface must be ALL pure virtual, and have no data members.
 *    3. Define a name for it.
 *    4. In its implementation file, use EXPOSE_INTERFACE or EXPOSE_SINGLE_INTERFACE.
 *
 *  Versioning
 *
 *  There are two versioning cases that are handled by this:
 *    1. You add functions to the end of an interface, so it is binary compatible with the previous interface.
 *       In this case, you need two EXPOSE_INTERFACE's: one to expose your class as the old interface
 *       and one to expose it as the new interface.
 *
 *    2. You update an interface, so it's not compatible anymore (but you still want to be able to
 *       expose the old for legacy code). In this case, you need to make a new version name for your new interface,
 *       and make a wrapper interface and expose it for the old interface.
 */

#pragma once

#include "common/platform.hpp"
#include <functional>
#include <string>

namespace common
{
    /**
     * @brief Name of the interface factory function.
     */
    constexpr auto* CREATE_INTERFACE_SYMBOL = "CreateInterface";

    /**
     * @brief Interface return status.
     */
    enum class CreateInterfaceStatus : int
    {
        succeeded = 0,
        failed
    };

    /**
     * @brief Interface factory function prototype.
     */
    using CreateInterfaceFunc = class CommonInterface* (*)(const char* name, CreateInterfaceStatus* return_code);

    /**
     * @brief A base class for all interfaces in the game.
     */
    class NO_VTABLE CommonInterface
    {
      public:
        /**
         * @brief Default constructor.
         */
        CommonInterface() = default;

        /**
         * @brief Move constructor.
         */
        CommonInterface(CommonInterface&&) = default;

        /**
         * @brief Copy constructor.
         */
        CommonInterface(const CommonInterface&) = default;

        /**
         * @brief Move assignment operator.
         */
        CommonInterface& operator=(CommonInterface&&) = default;

        /**
         * @brief Copy assignment operator.
         */
        CommonInterface& operator=(const CommonInterface&) = default;

        /**
         * @brief Virtual destructor.
         */
        FORCE_STACK_ALIGN virtual ~CommonInterface() = default;
    };

    /**
     * @brief Used internally to register interfaces.
     */
    struct InterfaceRegistry final
    {
        using InstantiateFunc = std::function<CommonInterface*()>;
        InterfaceRegistry(InstantiateFunc instantiate_func, const std::string& interface_name);
    };
}

/**
 * @brief Creates an instance of an interface based on the given name.
 *
 * Creates an instance of an interface by matching the specified name with the available interface registrations.
 *
 * @param name The name of the interface to create.
 * @param status Pointer to a CreateInterfaceStatus variable to store the creation status.
 *
 * @return A pointer to the created interface. If the creation fails, nullptr is returned.
 *
 * @note The status parameter can be nullptr if the creation status is not required.
 */
extern "C" DLL_EXPORT FORCE_STACK_ALIGN common::CommonInterface* // NOLINTNEXTLINE(readability-identifier-naming)
  CreateInterface(const char* name, common::CreateInterfaceStatus* status);

/**
 * @brief Use this to expose an interface that can have multiple instances.
 *
 * e.g.: \c EXPOSE_INTERFACE(Interface, "MyInterface001") This will expose a class called Interface.
 * Clients can receive a pointer to this class by calling \c CreateInterface("MyInterface001").
 */
#define EXPOSE_INTERFACE(class_name, interface_name)                                                                   \
  namespace                                                                                                            \
  {                                                                                                                    \
    common::CommonInterface* __Create##class_name##_interface()                                                        \
    {                                                                                                                  \
      return new class_name{};                                                                                         \
    }                                                                                                                  \
    common::InterfaceRegistry __g_Create##class_name##_reg{__Create##class_name##_interface, (interface_name)};        \
  }

/**
 * @brief Use this if you want to write the factory function.
 */
#define EXPOSE_INTERFACE_FN(class_name, interface_name, function)                                                      \
  namespace                                                                                                            \
  {                                                                                                                    \
    common::InterfaceRegistry __g_Create##class_name##_reg{function, (interface_name)};                                \
  }

/**
 * @brief Use this to expose a singleton interface with a global variable you've created.
 */
#define EXPOSE_SINGLE_INTERFACE_GLOBALVAR(class_name, interface_name, variable_name)                                   \
  namespace                                                                                                            \
  {                                                                                                                    \
    common::CommonInterface* __Create##class_name##_interface()                                                        \
    {                                                                                                                  \
      return &(variable_name);                                                                                         \
    }                                                                                                                  \
    common::InterfaceRegistry __g_Create##class_name##_reg{__Create##class_name##_interface, (interface_name)};        \
  }

/**
 * @brief Use this to expose a singleton interface. This creates the global variable for you automatically.
 */
#define EXPOSE_SINGLE_INTERFACE(class_name, interface_name)                                                            \
  namespace                                                                                                            \
  {                                                                                                                    \
    class_name __g_##class_name##_singleton{};                                                                         \
  }                                                                                                                    \
  EXPOSE_SINGLE_INTERFACE_GLOBALVAR(class_name, (interface_name), __g_##class_name##_singleton)
