#include"KeyInfo.h"

KeyInfo::KeyInfo()
{

}

KeyInfo::~KeyInfo()
{
}

int KeyInfo::GetHowManyKey()
{
    int howmanykey = 0;
    for (int i = 0; i < 3; i++)
    {
        if (key[i] == 1)
            howmanykey++;
    }
    return howmanykey;
}