//
// Created by lol on 6/6/2017.
//

#ifndef UTILS_BASE64_H
#define UTILS_BASE64_H

#include <string>
/*
   base64.cpp and base64.h
   Copyright (C) 2004-2008 René Nyffenegger
   René Nyffenegger rene.nyffenegger@adp-gmbh.ch

*/

namespace m2
{
namespace base64
{
    inline bool is_base64(unsigned char c);

    std::string base64_encode(std::string const& string_to_encode);
    std::string base64_decode(std::string const& encoded_string);
}
}





#endif //LOG4CPP_BASE64_H
