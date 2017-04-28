/*
	THIS FILE IS A PART OF GTA V SCRIPT HOOK SDK
				http://dev-c.com
			(C) Alexander Blade 2015
*/

#pragma once

#define _USE_MATH_DEFINES

#include "..\..\inc\natives.h"
#include "..\..\inc\types.h"
#include "..\..\inc\enums.h"

#include "..\..\inc\main.h"

#include "math.h"

static constexpr auto radian = M_PI / 180;

struct Vector4 {
	Vector4() {};
	Vector4(float x, float y, float z, float h) : x(x), y(y), z(z), h(h) {}
	Vector4(Vector3 v3, float h) : x(v3.x), y(v3.y), z(v3.z), h(h) {}
	float x = 0.0;
	float y = 0.0;
	float z = 0.0;
	float h = 0.0;
} ;

enum MissionType { StealVehicle, DestroyVehicle, Assassination, ArmoredTruck, CrateDrop, MAX_Mission, NO_Mission };
enum WantedLevel { Wanted_Zero, Wanted_One, Wanted_Two, Wanted_Three, Wanted_Four, Wanted_Five };
enum VehicleClassBitwise {
	Compact = 0x000001,
	Sedan = 0x000002,
	SUV = 0x000004,
	Coupe = 0x000008,
	Muscle = 0x000010,
	SportsClassic = 0x000020,
	Sports = 0x000040,
	Super = 0x000080,
	Motorcycle = 0x000100,
	OffRoad = 0x000200,
	Industrial = 0x000400,
	Utility = 0x000800,
	Van = 0x001000,
	Cycle = 0x002000,
	Boat = 0x004000,
	Helicopter = 0x008000,
	Plane = 0x010000,
	Service = 0x020000,
	Emergency = 0x040000,
	Military = 0x080000,
	Commercial = 0x100000,
	Trainn = 0x200000, // Train is a typedef...
};
enum VehicleClassValues {
	
};

static char *VehicleClasses[] =
{
	"VehicleClassCompacts",
	"VehicleClassSedans",
	"VehicleClassSUVs",
	"VehicleClassCoupes",
	"VehicleClassMuscle",
	"VehicleClassSportsClassics",
	"VehicleClassSports",
	"VehicleClassSuper",
	"VehicleClassMotorcycles",
	"VehicleClassOffRoad",
	"VehicleClassIndustrial",
	"VehicleClassUtility",
	"VehicleClassVans",
	"VehicleClassCycles",
	"VehicleClassBoats",
	"VehicleClassHelicopters",
	"VehicleClassPlanes",
	"VehicleClassService",
	"VehicleClassEmergency",
	"VehicleClassMilitary",
	"VehicleClassCommercial",
	"VehicleClassTrains",
};

void ScriptMain();