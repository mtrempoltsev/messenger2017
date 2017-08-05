#ifndef M2_SERVER_STDAFX_H_H
#define M2_SERVER_STDAFX_H_H



#include <boost/enable_shared_from_this.hpp>
#include <boost/multiprecision/cpp_int.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/noncopyable.hpp>

#include <fstream>
#include <iomanip>




/****************************/

// make splitter macros '/'  '\\'

/****************************/

#define check(EXP)  if(!(EXP))
#define checkR(EXP) check(EXP) return
#define checkB(EXP) check(EXP) break
#define checkC(EXP) check(EXP) continue
#define checkT(EXP) check(EXP) throw

/****************************/

#define ifR(EXP)    if(EXP) return
#define ifB(EXP)    if(EXP) break
#define ifC(EXP)    if(EXP) continue
#define ifT(EXP)    if(EXP) throw

/****************************/

namespace indices { void MakeDir(const std::string& Path);  }

namespace uuids   { typedef boost::multiprecision::uint128_t uuid; }


#endif //M2_SERVER_STDAFX_H_H


