/*
 * SPDX-FileCopyrightText: Valve Corporation.
 * SPDX-License-Identifier: GPL-3.0
 */

#include "util/observable.hpp"
#include <gtest/gtest.h>

namespace
{
    struct TestEvent
    {
        int data{};

        [[nodiscard]] bool operator==(const TestEvent& other) const noexcept
        {
            return data == other.data;
        }
    };

    struct TestListener
    {
        TestEvent received_event{};
        int received_arg{};

        void on_event(const TestEvent& event, int arg)
        {
            received_event = event;
            received_arg = arg;
        }
    };

    class TestObservable : public util::Observable<TestEvent, int>
    {
      public:
        using Observable::notify;
    };

    class ObservableTest : public testing::Test
    {
      protected:
        TestObservable observable{};
    };

    TEST_F(ObservableTest, AppendAndRemoveListener)
    {
        TestListener listener1{};
        TestListener listener2{};
        TestListener listener3{};

        const auto handle1 = observable.append_listener([&](const TestEvent& event, const int arg) {
            listener1.on_event(event, arg);
        });

        const auto handle2 = observable.append_listener([&](const TestEvent& event, const int arg) {
            listener2.on_event(event, arg);
        });

        const TestEvent event{42};
        observable.notify(event, 10);

        EXPECT_EQ(listener1.received_event, event);
        EXPECT_EQ(listener1.received_arg, 10);
        EXPECT_EQ(listener2.received_event, event);
        EXPECT_EQ(listener2.received_arg, 10);

        observable.remove_listener(handle2);
        const auto handle3 = observable.append_listener([&](const TestEvent& event, const int arg) {
            listener3.on_event(event, arg);
        });

        listener2.received_event = TestEvent{};
        listener2.received_arg = 0;

        observable.notify(event, 20);
        EXPECT_EQ(listener1.received_event, event);
        EXPECT_EQ(listener1.received_arg, 20);
        EXPECT_EQ(listener2.received_event, TestEvent{});
        EXPECT_EQ(listener2.received_arg, 0);
        EXPECT_EQ(listener3.received_event, event);
        EXPECT_EQ(listener3.received_arg, 20);
    }

    TEST_F(ObservableTest, NotifyListeners)
    {
        TestListener listener1{};
        TestListener listener2{};

        observable.append_listener([&](const TestEvent& event, const int arg) {
            listener1.on_event(event, arg);
        });

        observable.append_listener([&](const TestEvent& event, const int arg) {
            listener2.on_event(event, arg);
        });

        const TestEvent event{42};
        observable.notify(event, 10);

        EXPECT_EQ(listener1.received_event, event);
        EXPECT_EQ(listener1.received_arg, 10);
        EXPECT_EQ(listener2.received_event, event);
        EXPECT_EQ(listener2.received_arg, 10);
    }

    TEST_F(ObservableTest, NotifyListenersNoArguments)
    {
        TestListener listener1{};
        TestListener listener2{};

        observable.append_listener([&](const TestEvent& event, const int arg) {
            listener1.on_event(event, arg);
        });

        observable.append_listener([&](const TestEvent& event, const int arg) {
            listener2.on_event(event, arg);
        });

        const TestEvent event{42};
        observable.notify(event, 0);

        EXPECT_EQ(listener1.received_event, event);
        EXPECT_EQ(listener1.received_arg, 0);
        EXPECT_EQ(listener2.received_event, event);
        EXPECT_EQ(listener2.received_arg, 0);
    }
}
