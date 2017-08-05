#pragma once
#include <string>

namespace m2
{
    struct Error final
    {
        enum class Code
        {
            NetworkError
        };

        Error(Code code, std::string&& message);

        Error(Error&) = delete;
        Error& operator=(Error&) = delete;

        Error(Error&&) = default;
        Error& operator=(Error&&) = default;

        Code code;
        std::string message;
    };
}
