#pragma once

#include "error.h"
#include "guid.h"

namespace m2
{
    struct RegisterNewUserHandler final
    {
        using CompletionHandler = std::function<void (Guid&& userId)>;
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
        static CoreDispatcher& instance();

        CoreDispatcher(CoreDispatcher&) = delete;
        CoreDispatcher& operator=(CoreDispatcher&) = delete;

        CoreDispatcher(CoreDispatcher&&) = delete;
        CoreDispatcher& operator=(CoreDispatcher&&) = delete;

        void registerNewUser(RegisterNewUserHandler handler);
        void login(const Guid& guid, LoginHandler handler);

    private:
        CoreDispatcher();
    };
}
