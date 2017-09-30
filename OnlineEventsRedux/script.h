
#pragma once

#define _USE_MATH_DEFINES

#include "..\..\inc\natives.h"
//#include "..\..\inc\types.h"
#include "..\..\inc\enums.h"
//#include "..\..\inc\main.h"

#include "math.h"
#include <set>
#include <vector>


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

enum PedConfigFlags
{
	PED_FLAG_CAN_FLY_THRU_WINDSCREEN = 32,
	PED_FLAG_DIES_BY_RAGDOLL = 33,
	PED_FLAG_NO_COLLISION = 52,
	_PED_FLAG_IS_SHOOTING = 58,
	_PED_FLAG_IS_ON_GROUND = 60,
	PED_FLAG_NO_COLLIDE = 62,
	PED_FLAG_DEAD = 71,
	PED_FLAG_IS_SNIPER_SCOPE_ACTIVE = 72,
	PED_FLAG_SUPER_DEAD = 73,
	_PED_FLAG_IS_IN_AIR = 76,
	PED_FLAG_IS_AIMING = 78,
	PED_FLAG_DRUNK = 100,
	_PED_FLAG_IS_NOT_RAGDOLL_AND_NOT_PLAYING_ANIM = 104,
	PED_FLAG_NO_PLAYER_MELEE = 122,
	PED_FLAG_NM_MESSAGE_466 = 125,
	PED_FLAG_INJURED_LIMP = 166,
	PED_FLAG_INJURED_LIMP_2 = 170,
	PED_FLAG_INJURED_DOWN = 187,
	PED_FLAG_SHRINK = 223,
	PED_FLAG_MELEE_COMBAT = 224,
	_PED_FLAG_IS_ON_STAIRS = 253,
	_PED_FLAG_HAS_ONE_LEG_ON_GROUND = 276,
	PED_FLAG_NO_WRITHE = 281,
	PED_FLAG_FREEZE = 292,
	PED_FLAG_IS_STILL = 301,
	PED_FLAG_NO_PED_MELEE = 314,
	_PED_SWITCHING_WEAPON = 331,
	PED_FLAG_ALPHA = 410,
};

enum VehicleClassBitwise {
  Compact = 0x000001, Sedan = 0x000002, SUV = 0x000004, Coupe = 0x000008, Muscle = 0x000010, SportsClassic = 0x000020, Sports = 0x000040,
	Super = 0x000080, Motorcycle = 0x000100, OffRoad = 0x000200, Industrial = 0x000400, Utility = 0x000800, Van = 0x001000, Cycle = 0x002000,
	Boat = 0x004000, Helicopter = 0x008000, Plane = 0x010000, Service = 0x020000, Emergency = 0x040000, Military = 0x080000,
	Commercial = 0x100000, Trainn = 0x200000, // Train is a typedef...
};

static char *VehicleClasses[] = { 
	"VehicleClassCompacts", "VehicleClassSedans", "VehicleClassSUVs", "VehicleClassCoupes", "VehicleClassMuscle",
	"VehicleClassSportsClassics", "VehicleClassSports", "VehicleClassSuper", "VehicleClassMotorcycles", "VehicleClassOffRoad",
	"VehicleClassIndustrial", "VehicleClassUtility", "VehicleClassVans", "VehicleClassCycles", "VehicleClassBoats", "VehicleClassHelicopters",
	"VehicleClassPlanes", "VehicleClassService", "VehicleClassEmergency", "VehicleClassMilitary", "VehicleClassCommercial", "VehicleClassTrains", };


void ScriptMain();