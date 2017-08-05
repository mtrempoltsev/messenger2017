#pragma once

/// Constants is a singleton for providing all global constants
/// TODO: implement method updateConfig (string filename)
static class Config
{


private:
    static int alpha;




public:
    Config() {
        alpha = 3;
    }





public:

    static int getAlpha() {
        return alpha;
    }





};

