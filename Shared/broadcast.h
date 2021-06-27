#ifndef BROADCAST_H
#define BROADCAST_H

#include "token.h"

class Broadcast
{
public:
    TOKEN token;
    char msg[255];

    Broadcast(){}
};

#endif // BROADCAST_H
