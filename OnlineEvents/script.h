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

typedef struct { float x = 0; DWORD _paddingx; float y = 0; DWORD _paddingy; float z = 0; DWORD _paddingz; float h = 0; DWORD _paddingw; } Vector4;

enum MissionType { StealVehicle, DestroyVehicle, Assassination, ArmoredTruck, CrateDrop, MAX_Mission, NO_Mission };
enum WantedLevel { Wanted_Zero, Wanted_One, Wanted_Two, Wanted_Three, Wanted_Four, Wanted_Five };
enum VehiclesToSelect {
	SelectCompacts = 0x000001,
	SelectSedans = 0x000002,
	SelectSUVs = 0x000004,
	SelectCoupes = 0x000008,
	SelectMuscle = 0x000010,
	SelectSportsClassics = 0x000020,
	SelectSports = 0x000040,
	SelectSuper = 0x000080,
	SelectMotorcycles = 0x000100,
	SelectOffRoad = 0x000200,
	SelectIndustrial = 0x000400,
	SelectUtility = 0x000800,
	SelectVans = 0x001000,
	SelectCycles = 0x002000,
	SelectBoats = 0x004000,
	SelectHelicopters = 0x008000,
	SelectPlanes = 0x010000,
	SelectService = 0x020000,
	SelectEmergency = 0x040000,
	SelectMilitary = 0x080000,
	SelectCommercial = 0x100000,
	SelectTrains = 0x200000,
};

void ScriptMain();

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