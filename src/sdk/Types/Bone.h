#pragma once

struct bone_t
{
	char pad[0xCC];
	float x;
	char pad2[0xC];
	float y;
	char pad3[0xC];
	float z;
};