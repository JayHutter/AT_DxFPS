#include "Billboard.h"

Billboard::Billboard(float* pitch, float* yaw) : pitch(pitch), yaw(yaw)
{
}

void Billboard::FacePlayer(float playPitch, float playYaw)
{
	if (pitch) *pitch = playPitch;
	if (yaw)*yaw = playYaw;
}

void Billboard::FacePlayer(DirectX::XMFLOAT2 rot)
{
	if (pitch) *pitch = rot.y;
	if (pitch) *yaw = rot.x;
}
