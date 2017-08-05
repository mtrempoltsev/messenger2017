#pragma once

namespace m2 {
namespace routing {

/// Constants is a singleton for providing all global constants
/// TODO: implement method updateConfig (string filename)
class Config
{
public:
    Config() {
      setDefaultConfig();
    }

public:
    static int getAlpha() {
        return alpha;
    }

    static int getK(){
      return k;
    }

private:
    static int alpha;
    static int k;

  static void setDefaultConfig(){
      // for now, do nothing 
  }

};

} // namespace routing
} // namespace m2
