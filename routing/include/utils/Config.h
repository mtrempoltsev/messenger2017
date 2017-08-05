#pragma once

/// Constants is a singleton for providing all global constants
/// TODO: implement method updateConfig (string filename)
static class Config
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
    alpha = 3;
    k = 20;
  }

};
