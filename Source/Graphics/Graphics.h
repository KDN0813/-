#pragma once
#include "System/ClassBase/Singleton.h"

class Graphics : public Singleton<Graphics>
{
public:
    Graphics():Singleton(this){}

};

