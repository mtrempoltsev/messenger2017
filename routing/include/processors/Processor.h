#pragma once
class Processor
{
#pragma region __ Constructor and destructor __ 
public:
    Processor();
    ~Processor();
#pragma endregion

public:
#pragma region __ Public methods __
    virtual void process();
#pragma endregion


};

