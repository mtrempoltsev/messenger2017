#pragma once

/// Constants is a singleton for providing all global constants
/// TODO: implement method updateConfig (string filename)
static class Config
{

#pragma region __ Fields __ 
private:
    static int alpha;
#pragma endregion


#pragma region __ Constructor __
public:
    Constants() {
        alpha = 3;
    }
#pragma endregion


#pragma region __ Methods __

public:
#pragma region __ Public methods __
    static int getAlpha() {
        return alpha;
    }
#pragma endregion  
#pragma endregion



};

