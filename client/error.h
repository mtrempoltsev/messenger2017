#pragma once

namespace m2
{
    struct Error final
    {
        Error(Code code, std::string&& message);

        Error(Error&) = delete;
        Error& operator=(Error&) = delete;

        Error(Error&&) = default;
        Error& operator=(Error&&) = default;

        enum class Code
        {
            NetworkError
        };

        Code code;
        std::string message;
    };
}
