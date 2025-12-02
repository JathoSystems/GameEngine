#pragma once
#include "Packet.h"
#include "GameObjects/Transform/Transform.h"

class PlayerInputPacket : public Packet
{
private:
    Position position;
    Rotation rotation;
    int inputFlags;

};
