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

#include "util/observable.hpp"
#include <gtest/gtest.h>

namespace {
    struct TestEvent {
        int data{};

        [[nodiscard]] bool operator==(const TestEvent& other) const noexcept
        {
            return data == other.data;
        }
    };

    struct TestListener {
        TestEvent received_event{};
        int received_arg{};

        void on_event(const TestEvent& event, int arg)
        {
            received_event = event;
            received_arg = arg;
        }
    };

    class TestObservable : public Util::Observable<TestEvent, int> {
      public:
        using Observable::notify;
    };

    class ObservableTest : public testing::Test {
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
