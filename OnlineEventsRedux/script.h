
#pragma once

#define _USE_MATH_DEFINES

#include "..\..\inc\natives.h"
//#include "..\..\inc\types.h"
#include "..\..\inc\enums.h"
//#include "..\..\inc\main.h"

#include "math.h"
#include <set>


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

const std::set<int> professional_ped_hashes = { -67533719, -39239064, 2129936603, 1752208920, -636391810, -198252413, 361513884, -1868718465,
-442429178, 2114544056, -900269486, -1106743555, 1984382277, -96953009, -1275859404, 2047212121, 1349953339, -1613485779, -1697435671,
-912318012, -1280051738, -1366884940, -1589423867, -1211756494, -2063996617, 1975732938, 2119136831, -9308122, 1650288984, -429715051,
71929310, 436345731, 368603149, 1581098148, -37334073, -459818001, -1688898956, 1699403886, -173013091, 131961260, 377976310, 1371553700,
1755064960, 2010389054, -1806291497, -2051422616, 653289389, 1189322339, 331645324, -775102410, -1736970383, 815693290, -1289578670, -245247470,
691061163, 343259175, 587703123, 429425116, 51789996, -1768198658, 706935758, -254493138, -1395868234, -422822692, -619494093, 611648169,
321657486, -44746786, 1330042375, -1109568186, 1822283721, -568861381, -322270187, 1011059922, -951490775, 623927022, -1044093321, 1082572151,
-1398552374, -1408326184, 1535236204, 2035992488, 228715206, -1920001264, 1520708641, -829029621, -1275859404, -1222037748 };



void ScriptMain();