#pragma once

#include "contact.h"
#include "error.h"
#include "guid.h"
#include "message.h"
#include <functional>
#include <vector>

namespace m2
{
    namespace core {
        struct RegisterNewUserHandler final
        {
            using CompletionHandler = std::function<void (Uuid&& userId)>;
            using ErrorHandler = std::function<void (Error&& error)>;

            CompletionHandler onComletion;
            ErrorHandler onError;
        };

        struct LoginHandler final
        {
            using CompletionHandler = std::function<void ()>;
            using ErrorHandler = std::function<void (Error&& error)>;

            CompletionHandler onComletion;
            ErrorHandler onError;
        };

        class CoreDispatcher final
        {
        public:
            CoreDispatcher() = default;
            CoreDispatcher(CoreDispatcher&) = delete;
            CoreDispatcher& operator=(CoreDispatcher&) = delete;

            CoreDispatcher(CoreDispatcher&&) = delete;
            CoreDispatcher& operator=(CoreDispatcher&&) = delete;

            void registerNewUser(RegisterNewUserHandler handler);
            void login(const Uuid& uuid, LoginHandler handler);

        };

    } // core
} // m2
