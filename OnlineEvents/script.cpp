/*
	THIS FILE IS A PART OF GTA V SCRIPT HOOK SDK
				http://dev-c.com			
			(C) Alexander Blade 2015
*/

#define NOMINMAX
#include "script.h"
#include <vector>
#include "IniWriter.h"
#include "IniReader.h"
#include <string>
#include <ctime>
#include "iostream"
#include <sstream>
#include <algorithm>
#include "Log.h"
//#include "keyboard.h"

CIniReader Reader(".\\OnlineEvents.ini");
Log Logger(".\\OnlineEvents.log");


typedef struct { float x = 0; DWORD _paddingx; float y = 0; DWORD _paddingy; float z = 0; DWORD _paddingz; float h = 0; DWORD _paddingw; } Vector4;

#pragma warning(disable : 4244 4305) // double <-> float conversions
#pragma warning(disable : 4302)

// LEGACY VARIABLES MOST OF WHICH ALMOST CERTAINLY SHOULD NOT BE GLOBAL BUT WHATEVER
// LEGACY VARIABLES MOST OF WHICH ALMOST CERTAINLY SHOULD NOT BE GLOBAL BUT WHATEVER
// LEGACY VARIABLES MOST OF WHICH ALMOST CERTAINLY SHOULD NOT BE GLOBAL BUT WHATEVER

//int timer_start_minute, type_of_event;
//int missionTime = 0;
//int waitTime = 0;
//bool ready_to_prepare_mission = false;
//bool timer_is_started = false;
//bool mission_is_prepared = false;
//bool eventOver = false;
//bool specialCrate = false;
int model;
int val;
//Vehicle vehicle_to_steal;
//Blip vehicle_blip;
// bool player_acquired_vehicle_ = false;
//bool gotColor = false;
//bool resprayed = false;
//bool blipMade = false;
//int red1, green1, blue1;
//int red2, green2, blue2;
//int Sred1, Sgreen1, Sblue1;
//int Sred2, Sgreen2, Sblue2;
//int primary1, secondary1;
//int vehicle_primary_color_compare_, secondary2;
//int wanted;
//Blip drop_off_blip_;
//Vehicle smugglerVehicle;
//Blip smugglerBlip;
//Blip crateBlip;
//Vector3 crateSpawn;
//bool crateMade = false;
//bool spawnguards = false;
//Object crate;
//Ped guard1, guard2, guard3, guard4, guard5, guard6, guard7, guard8, guard9, guard10;
//Ped assassination_target;
//Blip targetBlip;
//Vehicle armored_truck;
//Blip truckBlip;
//Object case1, case2, case3;
//Vector3 truckPos;
//Ped truck_driver_;
//bool driverMade = false;

// LEGACY VARIABLES MOST OF WHICH ALMOST CERTAINLY SHOULD NOT BE GLOBAL BUT WHATEVER
// LEGACY VARIABLES MOST OF WHICH ALMOST CERTAINLY SHOULD NOT BE GLOBAL BUT WHATEVER
// LEGACY VARIABLES MOST OF WHICH ALMOST CERTAINLY SHOULD NOT BE GLOBAL BUT WHATEVER

enum MissionType { StealVehicle, DestroyVehicle, Assassination, ArmoredTruck, CrateDrop, MAX_Mission, NO_Mission };
enum WantedLevel { Wanted_Zero, Wanted_One, Wanted_Two, Wanted_Three, Wanted_Four, Wanted_Five };

Ped playerPed; // breaks naming convention because 
Player player;
std::vector<Vector4> reserved_vehicle_spawn_points;
std::vector<Vector4> vehicle_spawn_points;
std::vector<Hash> possible_vehicle_models;


bool play_notification_beeps, use_default_blip;
ULONGLONG mission_cooldown;
uint mission_timeout;
uint maximum_number_of_spawn_points, maximum_number_of_vehicle_models, vehicle_search_range_minimum;
uint spawn_point_min_search_range, spawn_point_max_search_range;
uint mission_minimum_range_for_timeout, logging_level;
bool notify_get_parked_cars_in_range;
bool notify_number_of_possible_vehicle_models, notify_number_of_possible_spawn_points, notify_distance_to_spawn_point, notify_number_of_reserved_spawn_points;
uint seconds_to_wait_for_vehicle_persistence_scripts, number_of_tries_to_find_spawn_point;

void NotifyBottomCenter(char* message) {
	UI::BEGIN_TEXT_COMMAND_PRINT("STRING");
	UI::ADD_TEXT_COMPONENT_SUBSTRING_PLAYER_NAME(message);
	UI::END_TEXT_COMMAND_PRINT(2000, 1);
}

void PlayNotificationBeep() {
	if (play_notification_beeps) AUDIO::PLAY_SOUND_FRONTEND(-1, "Text_Arrive_Tone", "Phone_SoundSet_Default", 0);
}

void CreateNotification(char* message, bool is_beep_enabled) {
	Logger.Write("CreateNotification()", LogVerbose);
	UI::_SET_NOTIFICATION_TEXT_ENTRY("STRING");
	UI::ADD_TEXT_COMPONENT_SUBSTRING_PLAYER_NAME(message);
	UI::_DRAW_NOTIFICATION(0, 1);
	if (is_beep_enabled) AUDIO::PLAY_SOUND_FRONTEND(-1, "Text_Arrive_Tone", "Phone_SoundSet_Default", 0);
}

void money_math() {
	if (PED::IS_PED_MODEL(playerPed, GAMEPLAY::GET_HASH_KEY("player_zero"))) model = 0;
	if (PED::IS_PED_MODEL(playerPed, GAMEPLAY::GET_HASH_KEY("player_one")))  model = 1;
	if (PED::IS_PED_MODEL(playerPed, GAMEPLAY::GET_HASH_KEY("player_two")))  model = 2;

	char statNameFull[32];
	sprintf_s(statNameFull, "SP%d_TOTAL_CASH", model);
	Hash hash = GAMEPLAY::GET_HASH_KEY(statNameFull);
	STATS::STAT_GET_INT(hash, &val, -1);
	//char * notification_message;

	//if (type_of_event == 1) val += 20000;
	//if (type_of_event == 2) val += 10000;
	//if (type_of_event == 3)	{
	//	int random = rand() % 5 + 1;
	//	if (true) {
	//		if (random == 1) {
	//			val += 50000;
	//			WEAPON::GIVE_DELAYED_WEAPON_TO_PED(playerPed, GAMEPLAY::GET_HASH_KEY("WEAPON_RPG"), 1000, 1);
	//			notification_message = "Crate collected.\nContents:\n- ~g~$10000~w~\n- RPG";
	//		}
	//		if (random == 2) {
	//			val += 75000;
	//			WEAPON::GIVE_DELAYED_WEAPON_TO_PED(playerPed, GAMEPLAY::GET_HASH_KEY("WEAPON_MINIGUN"), 1000, 1);
	//			notification_message = "Crate collected.\nContents:\n- ~g~$15000~w~\n- Minigun";
	//		}
	//		if (random == 3) {
	//			val += 25000;
	//			WEAPON::GIVE_DELAYED_WEAPON_TO_PED(playerPed, GAMEPLAY::GET_HASH_KEY("WEAPON_GRENADELAUNCHER"), 1000, 1);
	//			notification_message = "Crate collected.\nContents:\n- ~g~$10000~w~\n- Grenade Launcher";
	//		}
	//		if (random == 4) {
	//			val += 15000;
	//			WEAPON::GIVE_DELAYED_WEAPON_TO_PED(playerPed, GAMEPLAY::GET_HASH_KEY("WEAPON_STICKYBOMB"), 1000, 1);
	//			notification_message = "Crate collected.\nContents:\n- ~g~$15000~w~\n- Sticky Bombs";
	//		}
	//		if (random == 5) {
	//			val += 20000;
	//			WEAPON::GIVE_DELAYED_WEAPON_TO_PED(playerPed, GAMEPLAY::GET_HASH_KEY("WEAPON_MOLOTOV"), 1000, 1);
	//			notification_message = "Crate collected.\nContents:\n- ~g~$20000~w~\n- Molotovs";
	//		}
	//	} else {
	//		if (random == 1) {
	//			val += 10000;
	//			WEAPON::GIVE_DELAYED_WEAPON_TO_PED(playerPed, GAMEPLAY::GET_HASH_KEY("WEAPON_HEAVYSNIPER"), 1000, 1);
	//			notification_message = "Crate collected.\nContents:\n- ~g~$1000~w~\n- Heavy Sniper";
	//		}
	//		if (random == 2) {
	//			val += 15000;
	//			WEAPON::GIVE_DELAYED_WEAPON_TO_PED(playerPed, GAMEPLAY::GET_HASH_KEY("WEAPON_ASSAULTRIFLE"), 1000, 1);
	//			notification_message = "Crate collected.\nContents:\n- ~g~$1500~w~\n- Assault Rifle";
	//		}
	//		if (random == 3) {
	//			val += 20000;
	//			WEAPON::GIVE_DELAYED_WEAPON_TO_PED(playerPed, GAMEPLAY::GET_HASH_KEY("WEAPON_COMBATMG"), 1000, 1);
	//			notification_message = "Crate collected.\nContents:\n- ~g~$2000~w~\n- Combat MG";
	//		}
	//		if (random == 4) {
	//			val += 25000;
	//			WEAPON::GIVE_DELAYED_WEAPON_TO_PED(playerPed, GAMEPLAY::GET_HASH_KEY("WEAPON_PUMPSHOTGUN"), 1000, 1);
	//			notification_message = "Crate collected.\nContents:\n- ~g~$2500~w~\n- Pump Shotgun";
	//		}
	//		if (random == 5) {
	//			val += 30000;
	//			WEAPON::GIVE_DELAYED_WEAPON_TO_PED(playerPed, GAMEPLAY::GET_HASH_KEY("WEAPON_APPISTOL"), 1000, 1);
	//			notification_message = "Crate collected.\nContents:\n- ~g~$3000~w~\n- AP Pistol";
	//		}
	//	}
	//	CreateNotification(notification_message, play_notification_beeps);
	//	//PlayNotificationBeep();
	//}
	//if (type_of_event == 4)
	//	val += 15000;

	STATS::STAT_SET_INT(hash, val, 1);
}

Hash GetHashOfVehicleModel(Vehicle vehicle) {
	return ENTITY::GET_ENTITY_MODEL(vehicle);
}

void SetPlayerMinimumWantedLevel(WantedLevel wanted_level) {
	Logger.Write("SetPlayerMinimumWantedLevel()", LogVerbose);
	if (PLAYER::GET_PLAYER_WANTED_LEVEL(player) < wanted_level) {
		PLAYER::SET_PLAYER_WANTED_LEVEL(player, wanted_level, 0);
		PLAYER::SET_PLAYER_WANTED_LEVEL_NOW(player, 1);
	}
}

bool IsVehicleProbablyParked(Vehicle vehicle) {
	if ((VEHICLE::IS_VEHICLE_STOPPED(vehicle)) &&
		(VEHICLE::GET_PED_IN_VEHICLE_SEAT(vehicle, -1) == 0) && // IS_VEHICLE_SEAT_FREE doesn't fucking work.
		(VEHICLE::GET_VEHICLE_NUMBER_OF_PASSENGERS(vehicle) == 0)) {
		return true;
	}
	return false;
}

bool IsVehicleDrivable(Vehicle vehicle) {
	Hash hash_of_vehicle_model = GetHashOfVehicleModel(vehicle);
	if (VEHICLE::IS_VEHICLE_DRIVEABLE(vehicle, false) &&
		(VEHICLE::IS_THIS_MODEL_A_CAR(hash_of_vehicle_model) ||
			VEHICLE::IS_THIS_MODEL_A_BIKE(hash_of_vehicle_model) ||
			VEHICLE::IS_THIS_MODEL_A_QUADBIKE(hash_of_vehicle_model) // || // lol can't have an OR to an empty expression
																	 //VEHICLE::IS_THIS_MODEL_A_TRAIN(hash_of_vehicle_model) || // use if you like to
																	 //VEHICLE::IS_THIS_MODEL_A_HELI(hash_of_vehicle_model) ||
																	 //VEHICLE::_IS_THIS_MODEL_A_SUBMERSIBLE(hash_of_vehicle_model) || // doesn't seem to work
																	 //VEHICLE::IS_THIS_MODEL_A_PLANE(hash_of_vehicle_model) ||
																	 //VEHICLE::IS_THIS_MODEL_A_BOAT(hash_of_vehicle_model) ||
																	 //VEHICLE::IS_THIS_MODEL_A_BICYCLE(hash_of_vehicle_model
			)) {
		return true;
	}
	return false;
}

bool DoesEntityExistAndIsNotNull(Entity entity) {
	return (entity != NULL && ENTITY::DOES_ENTITY_EXIST(entity));
}

std::vector<Hash> GetVehicleModelsFromWorld(Ped ped, std::vector<Hash> vector_of_hashes, int maximum_vector_size, int search_range_minimum) {
	//Logger.Write("GetVehicleModelsFromWorld()", LogVerbose);
	const int ARR_SIZE = 1024;
	Vehicle all_world_vehicles[ARR_SIZE];
	int count = worldGetAllVehicles(all_world_vehicles, ARR_SIZE);
	Vector3 player_coordinates = ENTITY::GET_ENTITY_COORDS(playerPed, 0);

	if (all_world_vehicles != NULL) {
		for (int i = 0; i < count; i++) {
			if (DoesEntityExistAndIsNotNull(all_world_vehicles[i])) {
				Vehicle this_vehicle = all_world_vehicles[i];
				Vector3 this_vehicle_coordinates = ENTITY::GET_ENTITY_COORDS(this_vehicle, 0);
				if (DoesEntityExistAndIsNotNull(this_vehicle) &&
					IsVehicleDrivable(this_vehicle) && // is the vehicle a car/bike/etc and can the player start driving it?
					IsVehicleProbablyParked(this_vehicle) && // not moving, no driver?
					!(VEHICLE::GET_LAST_PED_IN_VEHICLE_SEAT(this_vehicle, -1)) && // probably not previously used by the player? We can hope?
					!(VEHICLE::_IS_VEHICLE_DAMAGED(this_vehicle)) && // probably not an empty car in the street as a result of a pileup...
					(VEHICLE::_IS_VEHICLE_SHOP_RESPRAY_ALLOWED(this_vehicle)) && // hopefully this actually works so airport luggage trains don't get tagged...
					GAMEPLAY::GET_DISTANCE_BETWEEN_COORDS(player_coordinates.x, player_coordinates.y, player_coordinates.z, this_vehicle_coordinates.x, this_vehicle_coordinates.y, this_vehicle_coordinates.z, 0) > search_range_minimum
					) {
					Hash this_vehicle_model = GetHashOfVehicleModel(this_vehicle);
					auto predicate = [this_vehicle_model](const Hash & item) { // pretty sure this isn't necessary for hashes, but it works.
						return (item == this_vehicle_model);
					};
					bool found = (std::find_if(vector_of_hashes.begin(), vector_of_hashes.end(), predicate) != vector_of_hashes.end());
					if (!found) vector_of_hashes.push_back(this_vehicle_model);
					if (vector_of_hashes.size() > maximum_vector_size) vector_of_hashes.erase(vector_of_hashes.begin()); // also pretty sure there's not more than a thousand models in the game, but safety first...
				}
			}
		}
	}
	return vector_of_hashes;
}

std::vector<Vector4> GetParkedCarsFromWorld(Ped ped, std::vector<Vector4> vector_of_vector4s, int maximum_vector_size, int search_range_minimum) {
	//Logger.Write("GetParkedCarsFromWorld()", LogVerbose);
	const int ARR_SIZE = 1024;
	Vehicle all_world_vehicles[ARR_SIZE];
	int count = worldGetAllVehicles(all_world_vehicles, ARR_SIZE);
	Vector3 player_coordinates = ENTITY::GET_ENTITY_COORDS(ped, 0);

	if (all_world_vehicles != NULL) {
		for (int i = 0; i < count; i++) {
			if (DoesEntityExistAndIsNotNull(all_world_vehicles[i])) {
				Vehicle this_vehicle = all_world_vehicles[i];
				Vector3 this_vehicle_coordinates = ENTITY::GET_ENTITY_COORDS(this_vehicle, 0);
				Vector4 this_vehicle_position;
				this_vehicle_position.x = this_vehicle_coordinates.x;
				this_vehicle_position.y = this_vehicle_coordinates.y;
				this_vehicle_position.z = this_vehicle_coordinates.z;
				this_vehicle_position.h = ENTITY::GET_ENTITY_HEADING(this_vehicle);

				if (DoesEntityExistAndIsNotNull(this_vehicle) &&
					IsVehicleDrivable(this_vehicle) && // is the vehicle a car/bike/etc and can the player start driving it?
					IsVehicleProbablyParked(this_vehicle) && // not moving, no driver?
					!(VEHICLE::GET_LAST_PED_IN_VEHICLE_SEAT(this_vehicle, -1)) && // probably not previously used by the player? We can hope?
					!(VEHICLE::_IS_VEHICLE_DAMAGED(this_vehicle)) && // probably not an empty car in the street as a result of a pileup...
					(VEHICLE::_IS_VEHICLE_SHOP_RESPRAY_ALLOWED(this_vehicle)) && // hopefully this actually works so airport luggage trains don't get tagged...
					GAMEPLAY::GET_DISTANCE_BETWEEN_COORDS(player_coordinates.x, player_coordinates.y, player_coordinates.z, this_vehicle_coordinates.x, this_vehicle_coordinates.y, this_vehicle_coordinates.z, 0) > search_range_minimum
					) {
					// CHECK WHETHER THERE IS A BLIP AT THIS POSITION
					// CHECK WHETHER THERE IS A BLIP AT THIS POSITION
					// CHECK WHETHER THERE IS A BLIP AT THIS POSITION
					// CHECK WHETHER THERE IS A BLIP AT THIS POSITION
					auto predicate = [this_vehicle_position](const Vector4 & item) { // didn't want to define a lambda inline, it gets ugly fast.
						return (item.x == this_vehicle_position.x && item.y == this_vehicle_position.y && item.z == this_vehicle_position.z && item.h == this_vehicle_position.h);
					};
					bool found = (std::find_if(vector_of_vector4s.begin(), vector_of_vector4s.end(), predicate) != vector_of_vector4s.end()); // make sure this_vehicle_position does not already exist in vector_of_vector4s
					if (!found) vector_of_vector4s.push_back(this_vehicle_position);
					if (vector_of_vector4s.size() > maximum_vector_size) vector_of_vector4s.erase(vector_of_vector4s.begin()); // just in case it gets filled up, first in first out
					//if (notify_get_parked_cars_in_range) NotifyBottomCenter(&("GetParkedCarsInRange(): " + std::to_string(vector_of_vector4s.size()))[0u]); // lol so much work to get some ints and chars output on screen
				}
			}
		}
	}
	return vector_of_vector4s;
}

Vector4 SelectASpawnPoint(Ped pedestrian, std::vector<Vector4> vector_of_vector4s_to_search, std::vector<Vector4> vector_of_vector4s_to_exclude, uint max_range, uint min_range, uint max_tries) {
	Logger.Write("SelectASpawnPoint()", LogNormal);
	Vector3 pedestrian_coordinates = ENTITY::GET_ENTITY_COORDS(pedestrian, 0);
	Vector4 empty_spawn_point;
	if (vector_of_vector4s_to_search.size() == 0) {
		Logger.Write("search vector was empty, returning an empty Vector4!", LogError);
		return empty_spawn_point;
	}
	for (uint i = 0; i < max_tries; i++) {
		Vector4 spawn_point = vector_of_vector4s_to_search[(rand() % vector_of_vector4s_to_search.size())]; // pick a random possible point from our set
		float distance = GAMEPLAY::GET_DISTANCE_BETWEEN_COORDS(pedestrian_coordinates.x, pedestrian_coordinates.y, pedestrian_coordinates.z, spawn_point.x, spawn_point.y, spawn_point.z, 0);
		if (distance > min_range && distance < max_range) {
			if (vector_of_vector4s_to_exclude.size() == 0) return spawn_point;
			auto predicate = [spawn_point](const Vector4 & each_item) {
				return (each_item.x == spawn_point.x && each_item.y == spawn_point.y && each_item.z == spawn_point.z && each_item.h == spawn_point.h);
			};
			bool found_in_exclude_vector = (std::find_if(vector_of_vector4s_to_exclude.begin(), vector_of_vector4s_to_exclude.end(), predicate) != vector_of_vector4s_to_exclude.end()); // make sure this_vehicle_position does not already exist in vector_of_vector4s
			if (!found_in_exclude_vector) {
				return spawn_point;
			}
		}
		WAIT(0);
	}
	Logger.Write("exceeded max tries, returning an empty Vector4!", LogError);
	return empty_spawn_point;
}

class CrateDropMission {
public:
	MissionType Prepare();
	MissionType Execute();
	MissionType Timeout();
private:
	Blip crate_blip_ = NULL;
	Vector3 crate_spawn_location_;
	bool crate_is_special_ = false;
	bool objects_were_spawned_ = false;
	Object crate_;
	Ped guard_1_, guard_2_, guard_3_, guard_4_, guard_5_;
};

MissionType CrateDropMission::Prepare() {
	Logger.Write("CrateDropMission::Prepare()", LogNormal);
	Vector3 player_coordinates = ENTITY::GET_ENTITY_COORDS(playerPed, 0);
	Vector3 spawn_location_1 = { -1782.33, DWORD(-826.29f), 7.83 };

	std::vector<Vector3> crate_spawn_locations = { 
		Vector3 { (1067.85), (2362.45), (43.87) },
		Vector3 { -3.77, (6840.14), (13.46) },
		Vector3 { (2061.37), (2798.68), (50.29) },
		Vector3 { (1883.24), (278.48), (162.73) },
		Vector3 { -2912.36, (3077.48), (3.39) },
		Vector3 { (2822.70), (-634.39), (2.15) },
		Vector3 { (2184.12), (5026.09), (42.63) },
		Vector3 { -3147.02, (305.27), (2.35) },
		Vector3 { (1552.40), (6644.04), (2.55) }
	}; // Someday I'll figure out something to expand this. Not today.
	uint tries = 0;
	while (true) {
		crate_spawn_location_ = crate_spawn_locations[(rand() % crate_spawn_locations.size())]; // pick a random possible point from our set
		float distance_between_player_and_spawn_point = GAMEPLAY::GET_DISTANCE_BETWEEN_COORDS(player_coordinates.x, player_coordinates.y, player_coordinates.z, crate_spawn_location_.x, crate_spawn_location_.y, crate_spawn_location_.z, 0);
		if (distance_between_player_and_spawn_point > spawn_point_min_search_range &&
			distance_between_player_and_spawn_point < spawn_point_max_search_range) {
			if (notify_distance_to_spawn_point) CreateNotification(&("Distance to Spawn: " + std::to_string(distance_between_player_and_spawn_point))[0u], play_notification_beeps); // combined into one line I hope it works...
			break;
		}
		if (tries++ > number_of_tries_to_find_spawn_point) {
			Logger.Write("failed to find a spawn point!", LogError);
			return NO_Mission;
		}
		WAIT(0);
	}
	if (rand() % 20 == 20) crate_is_special_ = true;
	crate_blip_ = UI::ADD_BLIP_FOR_COORD(crate_spawn_location_.x, crate_spawn_location_.y, crate_spawn_location_.z);
	if (use_default_blip) UI::SET_BLIP_SPRITE(crate_blip_, 306);
	else UI::SET_BLIP_SPRITE(crate_blip_, 1);
	UI::SET_BLIP_SCALE(crate_blip_, 1.5);
	if (crate_is_special_) UI::SET_BLIP_COLOUR(crate_blip_, 5);
	else UI::SET_BLIP_COLOUR(crate_blip_, 2);
	UI::SET_BLIP_DISPLAY(crate_blip_, (char)"you will never see this");
	if (crate_is_special_) CreateNotification("A ~y~special crate~w~ has been dropped.", play_notification_beeps);
	else CreateNotification("A ~g~crate~w~ has been dropped.", play_notification_beeps);
	return CrateDrop;
}

MissionType CrateDropMission::Execute() {
	//Logger.Write("CrateDropMission::Execute()", LogVerbose);
	Vector3 player_coordinates = ENTITY::GET_ENTITY_COORDS(playerPed, 0);
	int distance_to_crate = GAMEPLAY::GET_DISTANCE_BETWEEN_COORDS(player_coordinates.x, player_coordinates.y, player_coordinates.z, crate_spawn_location_.x, crate_spawn_location_.y, crate_spawn_location_.z, 0);

	if (distance_to_crate < 300 && !objects_were_spawned_) {
		STREAMING::REQUEST_MODEL(GAMEPLAY::GET_HASH_KEY("prop_box_ammo04a"));
		while (!STREAMING::HAS_MODEL_LOADED(GAMEPLAY::GET_HASH_KEY("prop_box_ammo04a"))) WAIT(0);
		crate_ = OBJECT::CREATE_AMBIENT_PICKUP(0x14568F28, crate_spawn_location_.x, crate_spawn_location_.y, crate_spawn_location_.z, -1, 0, GAMEPLAY::GET_HASH_KEY("prop_box_ammo04a"), 1, 1);
		ENTITY::SET_ENTITY_ALPHA(crate_, 255, 1);
		while (ENTITY::GET_ENTITY_HEIGHT(crate_, crate_spawn_location_.x, crate_spawn_location_.y, crate_spawn_location_.z, 1, 1) > 0) OBJECT::PLACE_OBJECT_ON_GROUND_PROPERLY(crate_);
		ENTITY::FREEZE_ENTITY_POSITION(crate_, 1);
		UI::REMOVE_BLIP(&crate_blip_);
		crate_blip_ = UI::ADD_BLIP_FOR_ENTITY(crate_);
		if (use_default_blip) UI::SET_BLIP_SPRITE(crate_blip_, 306);
		else UI::SET_BLIP_SPRITE(crate_blip_, 1);
		UI::SET_BLIP_SCALE(crate_blip_, 1.5);
		if (crate_is_special_) UI::SET_BLIP_COLOUR(crate_blip_, 5);
		else UI::SET_BLIP_COLOUR(crate_blip_, 2);
		UI::SET_BLIP_DISPLAY(crate_blip_, (char)"you will never see this");

		Ped skin = GAMEPLAY::GET_HASH_KEY("mp_g_m_pros_01");
		STREAMING::REQUEST_MODEL(skin);
		while (!STREAMING::HAS_MODEL_LOADED(skin)) WAIT(0);
		if (!ENTITY::DOES_ENTITY_EXIST(guard_1_)) {
			guard_1_ = PED::CREATE_PED(26, skin, crate_spawn_location_.x + 3, crate_spawn_location_.y, crate_spawn_location_.z + 1, 20, false, true);
			PED::SET_PED_RELATIONSHIP_GROUP_HASH(guard_1_, GAMEPLAY::GET_HASH_KEY("HATES_PLAYER"));
			WEAPON::GIVE_DELAYED_WEAPON_TO_PED(guard_1_, GAMEPLAY::GET_HASH_KEY("WEAPON_PISTOL"), 1000, 1);
			WEAPON::SET_CURRENT_PED_WEAPON(guard_1_, GAMEPLAY::GET_HASH_KEY("WEAPON_PISTOL"), 1);
			AI::TASK_TURN_PED_TO_FACE_ENTITY(guard_1_, crate_, 5000);
			ENTITY::SET_ENTITY_INVINCIBLE(guard_1_, false);
		}
		if (!ENTITY::DOES_ENTITY_EXIST(guard_2_)) {
			guard_2_ = PED::CREATE_PED(26, skin, crate_spawn_location_.x, crate_spawn_location_.y + 3, crate_spawn_location_.z + 1, 40, false, true);
			PED::SET_PED_RELATIONSHIP_GROUP_HASH(guard_2_, GAMEPLAY::GET_HASH_KEY("HATES_PLAYER"));
			WEAPON::GIVE_DELAYED_WEAPON_TO_PED(guard_2_, GAMEPLAY::GET_HASH_KEY("WEAPON_PISTOL"), 1000, 1);
			WEAPON::SET_CURRENT_PED_WEAPON(guard_2_, GAMEPLAY::GET_HASH_KEY("WEAPON_PISTOL"), 1);
			AI::TASK_TURN_PED_TO_FACE_ENTITY(guard_2_, crate_, 5000);
			ENTITY::SET_ENTITY_INVINCIBLE(guard_2_, false);
		}
		if (!ENTITY::DOES_ENTITY_EXIST(guard_3_)) {
			guard_3_ = PED::CREATE_PED(26, skin, crate_spawn_location_.x + 3, crate_spawn_location_.y + 3, crate_spawn_location_.z + 1, 60, false, true);
			PED::SET_PED_RELATIONSHIP_GROUP_HASH(guard_3_, GAMEPLAY::GET_HASH_KEY("HATES_PLAYER"));
			WEAPON::GIVE_DELAYED_WEAPON_TO_PED(guard_3_, GAMEPLAY::GET_HASH_KEY("WEAPON_PUMPSHOTGUN"), 1000, 1);
			WEAPON::SET_CURRENT_PED_WEAPON(guard_3_, GAMEPLAY::GET_HASH_KEY("WEAPON_PUMPSHOTGUN"), 1);
			AI::TASK_TURN_PED_TO_FACE_ENTITY(guard_3_, crate_, 5000);
			ENTITY::SET_ENTITY_INVINCIBLE(guard_3_, false);
		}
		if (!ENTITY::DOES_ENTITY_EXIST(guard_4_)) {
			guard_4_ = PED::CREATE_PED(26, skin, crate_spawn_location_.x - 3, crate_spawn_location_.y, crate_spawn_location_.z + 1, 80, false, true);
			PED::SET_PED_RELATIONSHIP_GROUP_HASH(guard_4_, GAMEPLAY::GET_HASH_KEY("HATES_PLAYER"));
			WEAPON::GIVE_DELAYED_WEAPON_TO_PED(guard_4_, GAMEPLAY::GET_HASH_KEY("WEAPON_PUMPSHOTGUN"), 1000, 1);
			WEAPON::SET_CURRENT_PED_WEAPON(guard_4_, GAMEPLAY::GET_HASH_KEY("WEAPON_PUMPSHOTGUN"), 1);
			AI::TASK_TURN_PED_TO_FACE_ENTITY(guard_4_, crate_, 5000);
			ENTITY::SET_ENTITY_INVINCIBLE(guard_4_, false);
		}
		if (!ENTITY::DOES_ENTITY_EXIST(guard_5_)) {
			guard_5_ = PED::CREATE_PED(26, skin, crate_spawn_location_.x, crate_spawn_location_.y - 3, crate_spawn_location_.z + 1, 100, false, true);
			PED::SET_PED_RELATIONSHIP_GROUP_HASH(guard_5_, GAMEPLAY::GET_HASH_KEY("HATES_PLAYER"));
			WEAPON::GIVE_DELAYED_WEAPON_TO_PED(guard_5_, GAMEPLAY::GET_HASH_KEY("WEAPON_ASSAULTRIFLE"), 1000, 1);
			WEAPON::SET_CURRENT_PED_WEAPON(guard_5_, GAMEPLAY::GET_HASH_KEY("WEAPON_ASSAULTRIFLE"), 1);
			AI::TASK_TURN_PED_TO_FACE_ENTITY(guard_5_, crate_, 5000);
			ENTITY::SET_ENTITY_INVINCIBLE(guard_5_, false);
		}
		if (ENTITY::DOES_ENTITY_EXIST(guard_1_) ||
			ENTITY::DOES_ENTITY_EXIST(guard_2_) ||
			ENTITY::DOES_ENTITY_EXIST(guard_3_) ||
			ENTITY::DOES_ENTITY_EXIST(guard_4_) ||
			ENTITY::DOES_ENTITY_EXIST(guard_5_)) {
			Hash flare = GAMEPLAY::GET_HASH_KEY("WEAPON_FLARE");
			while (!WEAPON::HAS_WEAPON_ASSET_LOADED(flare)) {
				WEAPON::REQUEST_WEAPON_ASSET(flare, 31, 0);
				WAIT(0);
			}
			GAMEPLAY::SHOOT_SINGLE_BULLET_BETWEEN_COORDS(crate_spawn_location_.x, crate_spawn_location_.y + 1.0f, crate_spawn_location_.z, crate_spawn_location_.x, crate_spawn_location_.y + 1.5, crate_spawn_location_.z, 0, 1, flare, playerPed, 1, 1, 100.0);
			FIRE::ADD_EXPLOSION(crate_spawn_location_.x, crate_spawn_location_.y + 1.0f, crate_spawn_location_.z, 19, 0.0f, 0, 0, 0);
		}
		objects_were_spawned_ = true;
	}

	if (objects_were_spawned_ && !ENTITY::DOES_ENTITY_EXIST(crate_)) {
		//money_math();
		ENTITY::SET_PED_AS_NO_LONGER_NEEDED(&guard_1_);
		ENTITY::SET_PED_AS_NO_LONGER_NEEDED(&guard_2_);
		ENTITY::SET_PED_AS_NO_LONGER_NEEDED(&guard_3_);
		ENTITY::SET_PED_AS_NO_LONGER_NEEDED(&guard_4_);
		ENTITY::SET_PED_AS_NO_LONGER_NEEDED(&guard_5_);
		return NO_Mission;
	}
	return CrateDrop;
}

MissionType CrateDropMission::Timeout() {
	//Logger.Write("CrateDropMission::Timeout()", LogVerbose);
	Vector3 player_coordinates = ENTITY::GET_ENTITY_COORDS(playerPed, 0);
	uint distance_to_crate = GAMEPLAY::GET_DISTANCE_BETWEEN_COORDS(player_coordinates.x, player_coordinates.y, player_coordinates.z, crate_spawn_location_.x, crate_spawn_location_.y, crate_spawn_location_.z, 0);
	if (distance_to_crate > mission_minimum_range_for_timeout || ENTITY::IS_ENTITY_DEAD(playerPed)) {
		CreateNotification("The ~g~crate~w~ has been claimed by smugglers.", play_notification_beeps);
		UI::REMOVE_BLIP(&crate_blip_);
		ENTITY::SET_OBJECT_AS_NO_LONGER_NEEDED(&crate_);
		ENTITY::SET_PED_AS_NO_LONGER_NEEDED(&guard_1_);
		ENTITY::SET_PED_AS_NO_LONGER_NEEDED(&guard_2_);
		ENTITY::SET_PED_AS_NO_LONGER_NEEDED(&guard_3_);
		ENTITY::SET_PED_AS_NO_LONGER_NEEDED(&guard_4_);
		ENTITY::SET_PED_AS_NO_LONGER_NEEDED(&guard_5_);
		return NO_Mission;
	}
	return CrateDrop;
}

class ArmoredTruckMission {
public:
	MissionType Prepare();
	MissionType Execute();
	MissionType Timeout();
private:
	Vehicle armored_truck_ = NULL;
	Ped truck_driver_ = NULL;
	Blip truck_blip_ = NULL;
};

MissionType ArmoredTruckMission::Prepare() {
	Logger.Write("ArmoredTruckMission::Prepare()", LogNormal);
	Vector4 vehicle_spawn_position = SelectASpawnPoint(playerPed, vehicle_spawn_points, reserved_vehicle_spawn_points, spawn_point_max_search_range, spawn_point_min_search_range, number_of_tries_to_find_spawn_point);
	if (vehicle_spawn_position.x == 0.0f && vehicle_spawn_position.y == 0.0f && vehicle_spawn_position.z == 0.0f && vehicle_spawn_position.h == 0.0f) return NO_Mission;
	STREAMING::REQUEST_MODEL(GAMEPLAY::GET_HASH_KEY("stockade"));
	while (!STREAMING::HAS_MODEL_LOADED(GAMEPLAY::GET_HASH_KEY("stockade"))) WAIT(0);
	armored_truck_ = VEHICLE::CREATE_VEHICLE(GAMEPLAY::GET_HASH_KEY("stockade"), vehicle_spawn_position.x, vehicle_spawn_position.y, vehicle_spawn_position.z, vehicle_spawn_position.h, 0, 0);
	VEHICLE::SET_VEHICLE_ON_GROUND_PROPERLY(armored_truck_);
	STREAMING::REQUEST_MODEL(GAMEPLAY::GET_HASH_KEY("mp_s_m_armoured_01"));
	truck_driver_ = PED::CREATE_PED_INSIDE_VEHICLE(armored_truck_, 26, GAMEPLAY::GET_HASH_KEY("mp_s_m_armoured_01"), -1, false, false);
	if (ENTITY::DOES_ENTITY_EXIST(truck_driver_)) AI::TASK_VEHICLE_DRIVE_WANDER(truck_driver_, armored_truck_, 10.0f, 153);
	truck_blip_ = UI::ADD_BLIP_FOR_ENTITY(armored_truck_);
	if (use_default_blip) UI::SET_BLIP_SPRITE(truck_blip_, 1);
	else UI::SET_BLIP_SPRITE(truck_blip_, 67);
	UI::SET_BLIP_COLOUR(truck_blip_, 3);
	UI::SET_BLIP_DISPLAY(truck_blip_, (char)"you will never see this");
	CreateNotification("An ~b~armored truck~w~ has been spotted carrying cash.", play_notification_beeps);
	return ArmoredTruck;
}

MissionType ArmoredTruckMission::Execute() {
	//Logger.Write("ArmoredTruckMission::Execute()", LogVerbose);
	Vector3 truck_coordinates = ENTITY::GET_ENTITY_COORDS(armored_truck_, 0);
	if (!VEHICLE::IS_VEHICLE_DRIVEABLE(armored_truck_, 1)) {
		CreateNotification("The ~b~armored truck~w~ has been destroyed.  The cash cases inside have been ruined.", play_notification_beeps);
		UI::REMOVE_BLIP(&truck_blip_);
		ENTITY::SET_PED_AS_NO_LONGER_NEEDED(&truck_driver_);
		ENTITY::SET_VEHICLE_AS_NO_LONGER_NEEDED(&armored_truck_);
		return NO_Mission;
	}
	if (VEHICLE::IS_VEHICLE_DOOR_FULLY_OPEN(armored_truck_, 2) || VEHICLE::IS_VEHICLE_DOOR_FULLY_OPEN(armored_truck_, 3)) {
		Pickup case1;
		Pickup case2;
		Pickup case3;
		int random = rand() % 3;
		switch (random) { // falls through!
		case 0:
			SetPlayerMinimumWantedLevel(Wanted_Three);
			case3 = OBJECT::CREATE_AMBIENT_PICKUP(0xDE78F17E, truck_coordinates.x, truck_coordinates.y, truck_coordinates.z + 1, -1, 10000, 0, 1, 1);
			ENTITY::SET_OBJECT_AS_NO_LONGER_NEEDED(&case3);
		case 1:
			SetPlayerMinimumWantedLevel(Wanted_Two);
			case2 = OBJECT::CREATE_AMBIENT_PICKUP(0xDE78F17E, truck_coordinates.x, truck_coordinates.y, truck_coordinates.z + 1, -1, 20000, 0, 1, 1);
			ENTITY::SET_OBJECT_AS_NO_LONGER_NEEDED(&case2);
		case 2:
			SetPlayerMinimumWantedLevel(Wanted_One);
			case1 = OBJECT::CREATE_AMBIENT_PICKUP(0xDE78F17E, truck_coordinates.x, truck_coordinates.y, truck_coordinates.z + 1, -1, 30000, 0, 1, 1);
			ENTITY::SET_OBJECT_AS_NO_LONGER_NEEDED(&case1);
		}
		CreateNotification("The ~b~armored truck's~w~ doors have opened.  Cash has been dropped.", play_notification_beeps);
		UI::REMOVE_BLIP(&truck_blip_);
		ENTITY::SET_PED_AS_NO_LONGER_NEEDED(&truck_driver_);
		ENTITY::SET_VEHICLE_AS_NO_LONGER_NEEDED(&armored_truck_);
		return NO_Mission;
	}
	return ArmoredTruck;
}

MissionType ArmoredTruckMission::Timeout() {
	//Logger.Write("ArmoredTruckMission::Timeout()", LogVerbose);
	Vector3 player_coordinates = ENTITY::GET_ENTITY_COORDS(playerPed, 0);
	Vector3 truck_coordinates = ENTITY::GET_ENTITY_COORDS(armored_truck_, 0);
	uint distance_to_truck = GAMEPLAY::GET_DISTANCE_BETWEEN_COORDS(truck_coordinates.x, truck_coordinates.y, truck_coordinates.z, player_coordinates.x, player_coordinates.y, player_coordinates.z, 0);
	if (distance_to_truck > mission_minimum_range_for_timeout || ENTITY::IS_ENTITY_DEAD(playerPed)) {
		UI::REMOVE_BLIP(&truck_blip_);
		ENTITY::SET_PED_AS_NO_LONGER_NEEDED(&truck_driver_);
		ENTITY::SET_VEHICLE_AS_NO_LONGER_NEEDED(&armored_truck_);
		CreateNotification("The ~b~armored truck~w~ has finished carrying cash.", play_notification_beeps);
		return NO_Mission;
	}
	return ArmoredTruck;
}

class AssassinationMission {
public:
	MissionType Prepare();
	MissionType Execute();
	MissionType Timeout();
private:
	Ped assassination_target_ = NULL;
	Blip target_blip_ = NULL;
	
};

MissionType AssassinationMission::Prepare() {
	Logger.Write("AssassinationMission::Prepare()", LogNormal);
	Vector4 target_spawn_position = SelectASpawnPoint(playerPed, vehicle_spawn_points, reserved_vehicle_spawn_points, spawn_point_max_search_range, spawn_point_min_search_range, number_of_tries_to_find_spawn_point);
	if (target_spawn_position.x == 0.0f && target_spawn_position.y == 0.0f && target_spawn_position.z == 0.0f && target_spawn_position.h == 0.0f) return NO_Mission;
	assassination_target_ = PED::CREATE_RANDOM_PED(target_spawn_position.x, target_spawn_position.y, target_spawn_position.z);
	while (!ENTITY::DOES_ENTITY_EXIST(assassination_target_)) WAIT(0);
	PED::SET_PED_DESIRED_HEADING(assassination_target_, target_spawn_position.h);
	target_blip_ = UI::ADD_BLIP_FOR_ENTITY(assassination_target_);
	AI::TASK_WANDER_STANDARD(assassination_target_, 1000.0f, 0);
	if (use_default_blip) UI::SET_BLIP_SPRITE(target_blip_, 1);
	else UI::SET_BLIP_SPRITE(target_blip_, 432);
	UI::SET_BLIP_COLOUR(target_blip_, 1);
	UI::SET_BLIP_DISPLAY(target_blip_, (char)"you will never see this");
	CreateNotification("A hit has been placed on a ~r~target~w~.  Kill them for a reward.", play_notification_beeps);
	return Assassination;
}

MissionType AssassinationMission::Execute() {
	//Logger.Write("AssassinationMission::Execute()", LogVerbose);
	if (ENTITY::IS_ENTITY_DEAD(assassination_target_)) {
		//money_math();
		UI::REMOVE_BLIP(&target_blip_);
		ENTITY::SET_PED_AS_NO_LONGER_NEEDED(&assassination_target_);
		CreateNotification("Target killed.\nReward: ~g~$1500~w~", play_notification_beeps);
		return NO_Mission;
	}
	return Assassination;
}

MissionType AssassinationMission::Timeout() {
	//Logger.Write("AssassinationMission::Timeout()", LogVerbose);
	Vector3 player_coordinates = ENTITY::GET_ENTITY_COORDS(playerPed, 0);
	Vector3 target_coordinates = ENTITY::GET_ENTITY_COORDS(assassination_target_, 0);
	uint distance_to_target = GAMEPLAY::GET_DISTANCE_BETWEEN_COORDS(target_coordinates.x, target_coordinates.y, target_coordinates.z, player_coordinates.x, player_coordinates.y, player_coordinates.z, 0);
	if (distance_to_target > mission_minimum_range_for_timeout || ENTITY::IS_ENTITY_DEAD(playerPed)) {
		CreateNotification("The hit on the ~r~target~w~ has expired.", play_notification_beeps);
		UI::REMOVE_BLIP(&target_blip_);
		ENTITY::SET_PED_AS_NO_LONGER_NEEDED(&assassination_target_);
		return NO_Mission;
	}
	return Assassination;
}

class DestroyVehicleMission {
public:
	MissionType Prepare();
	MissionType Execute();
	MissionType Timeout();
private:
	Vehicle vehicle_to_destroy_ = NULL;
	Blip vehicle_blip_ = NULL;
};

MissionType DestroyVehicleMission::Prepare() {
	Logger.Write("DestroyVehicleMission::Prepare()", LogNormal);
	Vector4 vehicle_spawn_position = SelectASpawnPoint(playerPed, vehicle_spawn_points, reserved_vehicle_spawn_points, spawn_point_max_search_range, spawn_point_min_search_range, number_of_tries_to_find_spawn_point);
	if (vehicle_spawn_position.x == 0.0f && vehicle_spawn_position.y == 0.0f && vehicle_spawn_position.z == 0.0f && vehicle_spawn_position.h == 0.0f) return NO_Mission;
	std::vector<char *> vehicle_names = { "rebel", "rancherxl", "dloader", "voodoo2", "bfinjection", 
												"sanchez2", "hexer", "daemon", "journey", "sadler" };
	Hash vehicle_model = GAMEPLAY::GET_HASH_KEY(vehicle_names[rand() % vehicle_names.size()]);
	//Hash vehicle_model_to_use = possible_vehicle_models[(rand() % possible_vehicle_models.size())];
	STREAMING::REQUEST_MODEL(vehicle_model);
	while (!STREAMING::HAS_MODEL_LOADED(vehicle_model)) WAIT(0);
	vehicle_to_destroy_ = VEHICLE::CREATE_VEHICLE(vehicle_model, vehicle_spawn_position.x, vehicle_spawn_position.y, vehicle_spawn_position.z, vehicle_spawn_position.h, 0, 0);
	VEHICLE::SET_VEHICLE_ON_GROUND_PROPERLY(vehicle_to_destroy_);
	vehicle_blip_ = UI::ADD_BLIP_FOR_ENTITY(vehicle_to_destroy_);
	if (use_default_blip) UI::SET_BLIP_SPRITE(vehicle_blip_, 1);
	else if (VEHICLE::IS_THIS_MODEL_A_CAR(vehicle_model)) UI::SET_BLIP_SPRITE(vehicle_blip_, 225);
	else UI::SET_BLIP_SPRITE(vehicle_blip_, 226);
	UI::SET_BLIP_COLOUR(vehicle_blip_, 1);
	UI::SET_BLIP_DISPLAY(vehicle_blip_, (char)"you will never see this");
	CreateNotification("A ~r~smuggler vehicle~w~ has been spotted.  Destroy it for a reward.", play_notification_beeps);
	return DestroyVehicle;
}

MissionType DestroyVehicleMission::Execute() {
	//Logger.Write("DestroyVehicleMission::Execute()", LogVerbose);
	if (!VEHICLE::IS_VEHICLE_DRIVEABLE(vehicle_to_destroy_, 1)) {
		UI::REMOVE_BLIP(&vehicle_blip_);
		ENTITY::SET_VEHICLE_AS_NO_LONGER_NEEDED(&vehicle_to_destroy_);
		CreateNotification("Vehicle destroyed.\nReward: ~g~$1000~w~", play_notification_beeps);
		//money_math();
		return NO_Mission;
	}
	return DestroyVehicle;
}

MissionType DestroyVehicleMission::Timeout() {
	//Logger.Write("DestroyVehicleMission::Timeout()", LogVerbose);
	Vector3 player_coordinates = ENTITY::GET_ENTITY_COORDS(playerPed, 0);
	Vector3 vehicle_coordinates = ENTITY::GET_ENTITY_COORDS(vehicle_to_destroy_, 0);
	uint vehicleDistance = GAMEPLAY::GET_DISTANCE_BETWEEN_COORDS(vehicle_coordinates.x, vehicle_coordinates.y, vehicle_coordinates.z, player_coordinates.x, player_coordinates.y, player_coordinates.z, 0);
	if (vehicleDistance > mission_minimum_range_for_timeout || ENTITY::IS_ENTITY_DEAD(playerPed)) {
		CreateNotification("The ~r~smuggler vehicle~w~ has been claimed by smugglers.", play_notification_beeps);
		UI::REMOVE_BLIP(&vehicle_blip_);
		ENTITY::SET_VEHICLE_AS_NO_LONGER_NEEDED(&vehicle_to_destroy_);
		return NO_Mission;
	}
	return DestroyVehicle;
}

class StealVehicleMission {
public:
	MissionType Prepare();
	MissionType Execute();
	MissionType Timeout();
private:
	Vehicle vehicle_to_steal_ = NULL;
	Vector3 drop_off_coordinates = { static_cast<DWORD>(1226.06), static_cast<DWORD>(-3231.36), static_cast<DWORD>(6.02) };
	Blip vehicle_blip_ = NULL;
	Blip drop_off_blip_ = NULL;
	int vehicle_primary_color_initial_, vehicle_secondary_color_initial_;
	int vehicle_primary_color_compare_, vehicle_secondary_color_compare_;
	enum MissionStage { GetCar, ResprayCar, DeliverCar };
	MissionStage mission_objective_ = GetCar;
};

MissionType StealVehicleMission::Prepare() {
	Logger.Write("StealVehicleMission.Prepare()", LogNormal);
	Vector4 vehicle_spawn_position = SelectASpawnPoint(playerPed, vehicle_spawn_points, reserved_vehicle_spawn_points, spawn_point_max_search_range, spawn_point_min_search_range, number_of_tries_to_find_spawn_point);
	if (vehicle_spawn_position.x == 0.0f && vehicle_spawn_position.y == 0.0f && vehicle_spawn_position.z == 0.0f && vehicle_spawn_position.h == 0.0f) return NO_Mission;
	Hash vehicle_model = possible_vehicle_models[(rand() % possible_vehicle_models.size())]; // pick a random model from our set, pray that it's still loaded in memory?
	STREAMING::REQUEST_MODEL(vehicle_model); // oh wait no, we can just load it.
	while (!STREAMING::HAS_MODEL_LOADED(vehicle_model)) WAIT(0);
	vehicle_to_steal_ = VEHICLE::CREATE_VEHICLE(vehicle_model, vehicle_spawn_position.x, vehicle_spawn_position.y, vehicle_spawn_position.z, vehicle_spawn_position.h, 0, 0);
	VEHICLE::SET_VEHICLE_ON_GROUND_PROPERLY(vehicle_to_steal_);
	vehicle_blip_ = UI::ADD_BLIP_FOR_ENTITY(vehicle_to_steal_);
	if (use_default_blip) UI::SET_BLIP_SPRITE(vehicle_blip_, 1);
	else if (VEHICLE::IS_THIS_MODEL_A_CAR(vehicle_model)) UI::SET_BLIP_SPRITE(vehicle_blip_, 225);
	else UI::SET_BLIP_SPRITE(vehicle_blip_, 226);
	UI::SET_BLIP_COLOUR(vehicle_blip_, 5);
	UI::SET_BLIP_DISPLAY(vehicle_blip_, (char)"you will never see this");
	VEHICLE::GET_VEHICLE_COLOURS(vehicle_to_steal_, &vehicle_primary_color_initial_, &vehicle_secondary_color_initial_);
	CreateNotification("A ~y~special vehicle~w~ has been requested for pickup.", play_notification_beeps);
	return StealVehicle;
}

MissionType StealVehicleMission::Execute() {
	//Logger.Write("StealVehicleMission::Execute()", LogVerbose);
	if (!VEHICLE::IS_VEHICLE_DRIVEABLE(vehicle_to_steal_, 1)) {
		CreateNotification("The ~y~special vehicle~w~ has been destroyed.", play_notification_beeps);
		if (mission_objective_ = DeliverCar) UI::REMOVE_BLIP(&drop_off_blip_);
		UI::REMOVE_BLIP(&vehicle_blip_);
		ENTITY::SET_VEHICLE_AS_NO_LONGER_NEEDED(&vehicle_to_steal_);
		return NO_Mission;
	}

	if (mission_objective_ == GetCar) {
		if (PED::IS_PED_IN_VEHICLE(playerPed, vehicle_to_steal_, 0)) {
			SetPlayerMinimumWantedLevel(Wanted_Two);
			CreateNotification("Respray the vehicle before turning it in.", play_notification_beeps);
			mission_objective_ = ResprayCar;
		}
	}
	
	if (mission_objective_ == ResprayCar) {
		VEHICLE::GET_VEHICLE_COLOURS(vehicle_to_steal_, &vehicle_primary_color_compare_, &vehicle_secondary_color_compare_);
		if (vehicle_primary_color_initial_ != vehicle_primary_color_compare_	&& ENTITY::GET_ENTITY_SPEED(vehicle_to_steal_) != 0 && (mission_objective_ == ResprayCar) ||
			vehicle_secondary_color_initial_ != vehicle_secondary_color_compare_	&& ENTITY::GET_ENTITY_SPEED(vehicle_to_steal_) != 0 && (mission_objective_ == ResprayCar)) {
			drop_off_blip_ = UI::ADD_BLIP_FOR_COORD(drop_off_coordinates.x, drop_off_coordinates.y, drop_off_coordinates.z);
			if (use_default_blip == 0) UI::SET_BLIP_SPRITE(drop_off_blip_, 50);
			else UI::SET_BLIP_SPRITE(drop_off_blip_, 1);
			UI::SET_BLIP_COLOUR(drop_off_blip_, 5);
			UI::SET_BLIP_DISPLAY(drop_off_blip_, (char)"you will never see this");
			CreateNotification("The vehicle is ready to be turned in to the ~y~garage~w~ at the docks.", play_notification_beeps);
			mission_objective_ = DeliverCar;
		}
	}

	if (mission_objective_ = DeliverCar) {
		GRAPHICS::DRAW_MARKER(1, drop_off_coordinates.x, drop_off_coordinates.y, drop_off_coordinates.z, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 5.0f, 5.0f, 1.0f, 204, 204, 0, 100, false, false, 2, false, false, false, false);
		Vector3 vehicle_coordinates = ENTITY::GET_ENTITY_COORDS(vehicle_to_steal_, 0);
		float dropOffDistance = GAMEPLAY::GET_DISTANCE_BETWEEN_COORDS(vehicle_coordinates.x, vehicle_coordinates.y, vehicle_coordinates.z, drop_off_coordinates.x, drop_off_coordinates.y, drop_off_coordinates.z, 0);
		if (dropOffDistance < 0.7f && PED::IS_PED_IN_VEHICLE(playerPed, vehicle_to_steal_, 0)) {
			VEHICLE::SET_VEHICLE_FORWARD_SPEED(vehicle_to_steal_, 0.0f);
			while (ENTITY::GET_ENTITY_SPEED(vehicle_to_steal_) != 0) WAIT(0);
			AI::TASK_LEAVE_VEHICLE(playerPed, vehicle_to_steal_, 0);
			VEHICLE::SET_VEHICLE_UNDRIVEABLE(vehicle_to_steal_, 1);
			CreateNotification("Vehicle collected.\nReward: ~g~$2000~w~", play_notification_beeps);
			//money_math();
			UI::REMOVE_BLIP(&drop_off_blip_);
			UI::REMOVE_BLIP(&vehicle_blip_);
			ENTITY::SET_VEHICLE_AS_NO_LONGER_NEEDED(&vehicle_to_steal_);
			return NO_Mission;
		}
	}
	return StealVehicle;
}

MissionType StealVehicleMission::Timeout() {
	//Logger.Write("StealVehicleMission::Timeout()", LogVerbose);
	Vector3 player_coordinates = ENTITY::GET_ENTITY_COORDS(playerPed, 0);
	Vector3 vehicle_coordinates = ENTITY::GET_ENTITY_COORDS(vehicle_to_steal_, 0);
	uint vehicleDistance = GAMEPLAY::GET_DISTANCE_BETWEEN_COORDS(vehicle_coordinates.x, vehicle_coordinates.y, vehicle_coordinates.z, player_coordinates.x, player_coordinates.y, player_coordinates.z, 0);
	if (vehicleDistance > mission_minimum_range_for_timeout || ENTITY::IS_ENTITY_DEAD(playerPed)) {
		CreateNotification("The ~y~special vehicle~w~ is no longer requested.", play_notification_beeps);
		if (mission_objective_ = DeliverCar) UI::REMOVE_BLIP(&drop_off_blip_);
		UI::REMOVE_BLIP(&vehicle_blip_);
		ENTITY::SET_VEHICLE_AS_NO_LONGER_NEEDED(&vehicle_to_steal_);
		return NO_Mission;
	}
	return StealVehicle;
}

class MissionHandler {
public:
	void Update();
private:
	MissionType current_mission_type_ = NO_Mission;
	ULONGLONG tick_count_at_last_update_ = GetTickCount64();
	ULONGLONG ticks_since_last_mission_ = 0;
	ULONGLONG ticks_between_missions_ = mission_cooldown * 1000;
	ULONGLONG ticks_since_mission_start_ = 0;
	ULONGLONG ticks_until_timeout_ = mission_timeout * 1000;
	StealVehicleMission StealVehicleMission;
	DestroyVehicleMission DestroyVehicleMission;
	AssassinationMission AssassinationMission;
	ArmoredTruckMission ArmoredTruckMission;
	CrateDropMission CrateDropMission;
};

void MissionHandler::Update() {
	//Logger.Write("MissionHandler::Update()", LogNormal);
	
	if (current_mission_type_ == NO_Mission) ticks_since_last_mission_ += (GetTickCount64() - tick_count_at_last_update_);
	else ticks_since_mission_start_ += (GetTickCount64() - tick_count_at_last_update_);
	tick_count_at_last_update_ = GetTickCount64();

	if (ticks_since_last_mission_ > ticks_between_missions_) { // Enough time has passed that we can start a new mission.
		ticks_since_last_mission_ = 0;  // Clear the timer since the last mission. This won't increment again until current_mission_type_ equals NO_Mission.
		ticks_since_mission_start_ = 0; // initialize the timeout delay as well, since it's about to begin incrementing.
		//current_mission_type_ = MissionType(rand() % MAX_Mission); // what a silly and convoluted method, but w.e
		current_mission_type_ = DestroyVehicle;
		switch (current_mission_type_) {
		case StealVehicle	:	current_mission_type_ = StealVehicleMission.Prepare();		break; // Prepare()s should return their MissionType on success.
		case DestroyVehicle	:	current_mission_type_ = DestroyVehicleMission.Prepare();	break; // If something goes wrong (StealVehicleMission takes too
		case Assassination	:	current_mission_type_ = AssassinationMission.Prepare();		break; // long to find a vehicle), they can return NO_Mission
		case ArmoredTruck	:	current_mission_type_ = ArmoredTruckMission.Prepare();		break;
		case CrateDrop		:	current_mission_type_ = CrateDropMission.Prepare();			break;
		}
	}
	
	switch (current_mission_type_) {
	case StealVehicle	:	current_mission_type_ = StealVehicleMission.Execute();		break; // if MissionType matches, the subroutine will execute.
	case DestroyVehicle	:	current_mission_type_ = DestroyVehicleMission.Execute();	break; // Each Execute will be responsible for returning its
	case Assassination	:	current_mission_type_ = AssassinationMission.Execute();		break; // own type while active, and returning NO_Mission
	case ArmoredTruck	:	current_mission_type_ = ArmoredTruckMission.Execute();		break; // when finished.
	case CrateDrop		:	current_mission_type_ = CrateDropMission.Execute();			break;
	}
	
	// Timeout() asks the mission to timeout, but if the player hasn't met certain criteria (like a minimum range), 
	// the mission may allow them to continue. So the Handler needs to wait until the Mission returns NO_Mission.

	if (ticks_since_mission_start_ > ticks_until_timeout_) {
		switch (current_mission_type_) {
		case StealVehicle	:	current_mission_type_ = StealVehicleMission.Timeout();		break; 
		case DestroyVehicle	:	current_mission_type_ = DestroyVehicleMission.Timeout();	break; 
		case Assassination	:	current_mission_type_ = AssassinationMission.Timeout();		break; 
		case ArmoredTruck	:	current_mission_type_ = ArmoredTruckMission.Timeout();		break; 
		case CrateDrop		:	current_mission_type_ = CrateDropMission.Timeout();			break;
		}
	}
}

void Update() {
	//Logger.Write("Update()", LogNormal);
	playerPed = PLAYER::PLAYER_PED_ID();
	player = PLAYER::PLAYER_ID(); // need to be updated every cycle?
	vehicle_spawn_points = GetParkedCarsFromWorld(playerPed, vehicle_spawn_points, maximum_number_of_spawn_points, vehicle_search_range_minimum);
	possible_vehicle_models = GetVehicleModelsFromWorld(playerPed, possible_vehicle_models, maximum_number_of_vehicle_models, vehicle_search_range_minimum);
	return;
}

void UglyHackForVehiclePersistenceScripts(uint seconds) {
	Logger.Write("UglyHackForVehiclePersistenceScripts()", LogNormal);
	if (!(seconds > 0)) return; // no point in wasting time or spawns if the user doesn't use a persistence script.
	WAIT(DWORD(seconds * 1000)); // let the persistence script start up and spawn its own vehicles.
	reserved_vehicle_spawn_points = GetParkedCarsFromWorld(playerPed, reserved_vehicle_spawn_points, maximum_number_of_spawn_points, vehicle_search_range_minimum); // set aside these spawns. Note that some legitimate spawns will invariably get reserved. This is why it's an ugly hack.
	Logger.Write("reserved_vehicle_spawn_points_parked.size(): " + std::to_string(reserved_vehicle_spawn_points.size()), LogNormal);
	return;
}

void InitialWaitForGame(uint seconds) {
	Logger.Write("InitialWaitForGame()", LogNormal);
	while (true) {
		if (!DLC2::GET_IS_LOADING_SCREEN_ACTIVE()) if (PLAYER::IS_PLAYER_PLAYING(player)) break; // this appears to work...
		WAIT(0);
	}
	WAIT(seconds * 1000);
	return;
}

void GetSettingsFromIniFile() {
	Logger.Write("GetSettingsFromIniFile()", LogNormal);
	// OPTIONS
	play_notification_beeps = Reader.ReadBoolean("Options", "play_notification_beeps", true);
	use_default_blip = Reader.ReadBoolean("Options", "use_default_blip", false);
	mission_timeout = Reader.ReadInteger("Options", "mission_timeout", 360);
	mission_cooldown = std::max((Reader.ReadInteger("Options", "mission_cooldown", 30)), 5);
	maximum_number_of_spawn_points = Reader.ReadInteger("Options", "maximum_number_of_spawn_points", 10000);
	maximum_number_of_vehicle_models = Reader.ReadInteger("Options", "maximum_number_of_vehicle_models", 1000);
	vehicle_search_range_minimum = Reader.ReadInteger("Options", "vehicle_search_range_minimum", 50);
	spawn_point_min_search_range = Reader.ReadInteger("Options", "vehicle_spawn_point_minimum_range", 666);
	spawn_point_max_search_range = Reader.ReadInteger("Options", "vehicle_spawn_point_maximum_range", 1332);
	mission_minimum_range_for_timeout = Reader.ReadInteger("Options", "mission_minimum_range_for_timeout", 666);
	// DEBUG
	logging_level = Reader.ReadInteger("Debug", "logging_level", 0);
	seconds_to_wait_for_vehicle_persistence_scripts = Reader.ReadInteger("Debug", "seconds_to_wait_for_vehicle_persistence_scripts", 15);
	number_of_tries_to_find_spawn_point = Reader.ReadInteger("Debug", "number_of_tries_to_find_spawn_point", 1000);
	notify_get_parked_cars_in_range = Reader.ReadBoolean("Debug", "notify_get_parked_cars_in_range", false);
	notify_number_of_possible_vehicle_models = Reader.ReadBoolean("Debug", "notify_number_of_possible_vehicle_models", false);
	notify_number_of_possible_spawn_points = Reader.ReadBoolean("Debug", "notify_number_of_possible_spawn_points", false);
	notify_distance_to_spawn_point = Reader.ReadBoolean("Debug", "notify_distance_to_spawn_point", false);
	notify_number_of_reserved_spawn_points = Reader.ReadBoolean("Debug", "notify_number_of_reserved_spawn_points", false);
	return;
}

void main() {
	Logger.Write("main()", LogNormal);

	GetSettingsFromIniFile();
	InitialWaitForGame(1); // ehh... I don't even know if any additional wait time is necessary...
	UglyHackForVehiclePersistenceScripts(seconds_to_wait_for_vehicle_persistence_scripts); // UGLY HACK FOR VEHICLE PERSISTENCE!
	MissionHandler Handler;

	while (true) {
		Update();
		Handler.Update();
		WAIT(0);
	}

	Logger.Close(); // I don't think this will ever happen...
}

void ScriptMain() {
	srand(GetTickCount());
	main();
}
