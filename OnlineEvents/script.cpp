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
#include <algorithm>
#include "Log.h"
#include "strings.h"
#include <random>

CIniReader Reader(".\\OnlineEvents.ini");
Log Logger(".\\OnlineEvents.log", LogNormal);

#pragma warning(disable : 4244 4305) // double <-> float conversions
#pragma warning(disable : 4302)

//GAMEPLAY::GET_HEADING_FROM_VECTOR_2D
// OBJECT::_GET_OBJECT_OFFSET_FROM_COORDS
// GAMEPLAY::GET_ANGLE_BETWEEN_2D_VECTORS
// GAMEPLAY::GET_HEADING_FROM_VECTOR_2D

std::random_device random_device; std::mt19937 generator(random_device()); // init a standard mersenne_twister_engine

Ped playerPed; // breaks naming convention because 
Player player;
std::vector<Vector4> reserved_vehicle_spawn_points;
std::vector<Vector4> vehicle_spawn_points;
std::vector<Hash> possible_vehicle_models;

// preference options
bool play_notification_beeps, use_default_blip;
uint mission_cooldown, mission_timeout;
uint spawn_point_minimum_range, spawn_point_maximum_range;
uint mission_minimum_range_for_timeout;

// debug options
LogLevel logging_level;
uint seconds_to_wait_for_vehicle_persistence_scripts, number_of_tries_to_select_items, vehicle_search_range_minimum;
uint maximum_number_of_spawn_points, maximum_number_of_vehicle_models;

// technically these could be added to the ini file pretty easily, but not by name - I'd have to include instructions...
int default_stealable_vehicle_flags = Compact | Sedan | SUV | Coupe |
										Muscle | SportsClassic | Sports | Super |
										Motorcycle | OffRoad;

int default_destroyable_vehicle_classes = SUV | Muscle | OffRoad | Motorcycle;

void NotifyBottomCenter(char* message) {
	Logger.Write("NotifyBottomCenter()", LogExtremelyVerbose);
	UI::BEGIN_TEXT_COMMAND_PRINT("STRING");
	UI::ADD_TEXT_COMPONENT_SUBSTRING_PLAYER_NAME(message);
	UI::END_TEXT_COMMAND_PRINT(2000, 1);
}

void PlayNotificationBeep() {
	Logger.Write("PlayNotificationBeep()", LogExtremelyVerbose);
	if (play_notification_beeps) AUDIO::PLAY_SOUND_FRONTEND(-1, "Text_Arrive_Tone", "Phone_SoundSet_Default", 0);
}

void CreateNotification(char* message, bool is_beep_enabled) {
	Logger.Write("CreateNotification()", LogExtremelyVerbose);
	UI::_SET_NOTIFICATION_TEXT_ENTRY("STRING");
	UI::ADD_TEXT_COMPONENT_SUBSTRING_PLAYER_NAME(message);
	UI::_DRAW_NOTIFICATION(0, 1);
	if (is_beep_enabled) AUDIO::PLAY_SOUND_FRONTEND(-1, "Text_Arrive_Tone", "Phone_SoundSet_Default", 0);
	Logger.Write("CreateNotification(): " + std::string(message), LogVerbose);
}

double Radians(double degrees) {
	Logger.Write("Radians():", LogExtremelyVerbose);
	return degrees * (M_PI / 180);
}

Vector4 GetVector4OfEntity(Entity entity) {
	Logger.Write("GetVector4OfEntity():", LogExtremelyVerbose);
	Vector3 v3 = ENTITY::GET_ENTITY_COORDS(entity, 0);
	Vector4 v4; v4.x = v3.x, v4.y = v3.y, v4.z = v3.z, v4.h = ENTITY::_GET_ENTITY_PHYSICS_HEADING(entity);
	return v4;
}

Vector3 GetCoordinatesByOriginBearingAndDistance(Vector4 v4, float bearing, float distance) {
	Logger.Write("GetCoordinatesByOriginBearingAndDistance():", LogExtremelyVerbose);
	Vector3 v3;	v3.x = v4.x + cos(Radians(v4.h + bearing)) * distance; v3.y = v4.y + sin((v4.h + bearing)*radian) * distance; v3.z = v4.z;
	return v3;
}

double GetFromUniformRealDistribution(double first, double second) {
	Logger.Write("GetFromUniformRealDistribution():", LogExtremelyVerbose);
	std::uniform_real_distribution<> uniform_real_distribution(first, second);
	return uniform_real_distribution(generator);
}

int GetFromUniformIntDistribution(int first, int second) {
	Logger.Write("GetFromUniformIntDistribution():", LogExtremelyVerbose);
	std::uniform_int_distribution<> uniform_int_distribution(first, second);
	return uniform_int_distribution(generator);
}

bool IsTheUniverseFavorable(float probability) {
	Logger.Write("IsTheUniverseFavorable():", LogExtremelyVerbose);
	return (GetFromUniformRealDistribution(0, 1) <= probability);
}

//void money_math() {
//	if (PED::IS_PED_MODEL(playerPed, GAMEPLAY::GET_HASH_KEY("player_zero"))) model = 0;
//	if (PED::IS_PED_MODEL(playerPed, GAMEPLAY::GET_HASH_KEY("player_one")))  model = 1;
//	if (PED::IS_PED_MODEL(playerPed, GAMEPLAY::GET_HASH_KEY("player_two")))  model = 2;
//	char statNameFull[32];
//	sprintf_s(statNameFull, "SP%d_TOTAL_CASH", model);
//	Hash hash = GAMEPLAY::GET_HASH_KEY(statNameFull);
//	STATS::STAT_GET_INT(hash, &val, -1);
//	//char * notification_message;
//	//if (type_of_event == 1) val += 20000;
//	//if (type_of_event == 2) val += 10000;
//	//if (type_of_event == 3)	{
//	//	int random = rand() % 5 + 1;
//	//	if (true) {
//	//		if (random == 1) {
//	//			val += 50000;
//	//			WEAPON::GIVE_DELAYED_WEAPON_TO_PED(playerPed, GAMEPLAY::GET_HASH_KEY("WEAPON_RPG"), 1000, 1);
//	//			notification_message = "Crate collected.\nContents:\n- ~g~$10000~w~\n- RPG";
//	//		}
//	//		if (random == 2) {
//	//			val += 75000;
//	//			WEAPON::GIVE_DELAYED_WEAPON_TO_PED(playerPed, GAMEPLAY::GET_HASH_KEY("WEAPON_MINIGUN"), 1000, 1);
//	//			notification_message = "Crate collected.\nContents:\n- ~g~$15000~w~\n- Minigun";
//	//		}
//	//		if (random == 3) {
//	//			val += 25000;
//	//			WEAPON::GIVE_DELAYED_WEAPON_TO_PED(playerPed, GAMEPLAY::GET_HASH_KEY("WEAPON_GRENADELAUNCHER"), 1000, 1);
//	//			notification_message = "Crate collected.\nContents:\n- ~g~$10000~w~\n- Grenade Launcher";
//	//		}
//	//		if (random == 4) {
//	//			val += 15000;
//	//			WEAPON::GIVE_DELAYED_WEAPON_TO_PED(playerPed, GAMEPLAY::GET_HASH_KEY("WEAPON_STICKYBOMB"), 1000, 1);
//	//			notification_message = "Crate collected.\nContents:\n- ~g~$15000~w~\n- Sticky Bombs";
//	//		}
//	//		if (random == 5) {
//	//			val += 20000;
//	//			WEAPON::GIVE_DELAYED_WEAPON_TO_PED(playerPed, GAMEPLAY::GET_HASH_KEY("WEAPON_MOLOTOV"), 1000, 1);
//	//			notification_message = "Crate collected.\nContents:\n- ~g~$20000~w~\n- Molotovs";
//	//		}
//	//	} else {
//	//		if (random == 1) {
//	//			val += 10000;
//	//			WEAPON::GIVE_DELAYED_WEAPON_TO_PED(playerPed, GAMEPLAY::GET_HASH_KEY("WEAPON_HEAVYSNIPER"), 1000, 1);
//	//			notification_message = "Crate collected.\nContents:\n- ~g~$1000~w~\n- Heavy Sniper";
//	//		}
//	//		if (random == 2) {
//	//			val += 15000;
//	//			WEAPON::GIVE_DELAYED_WEAPON_TO_PED(playerPed, GAMEPLAY::GET_HASH_KEY("WEAPON_ASSAULTRIFLE"), 1000, 1);
//	//			notification_message = "Crate collected.\nContents:\n- ~g~$1500~w~\n- Assault Rifle";
//	//		}
//	//		if (random == 3) {
//	//			val += 20000;
//	//			WEAPON::GIVE_DELAYED_WEAPON_TO_PED(playerPed, GAMEPLAY::GET_HASH_KEY("WEAPON_COMBATMG"), 1000, 1);
//	//			notification_message = "Crate collected.\nContents:\n- ~g~$2000~w~\n- Combat MG";
//	//		}
//	//		if (random == 4) {
//	//			val += 25000;
//	//			WEAPON::GIVE_DELAYED_WEAPON_TO_PED(playerPed, GAMEPLAY::GET_HASH_KEY("WEAPON_PUMPSHOTGUN"), 1000, 1);
//	//			notification_message = "Crate collected.\nContents:\n- ~g~$2500~w~\n- Pump Shotgun";
//	//		}
//	//		if (random == 5) {
//	//			val += 30000;
//	//			WEAPON::GIVE_DELAYED_WEAPON_TO_PED(playerPed, GAMEPLAY::GET_HASH_KEY("WEAPON_APPISTOL"), 1000, 1);
//	//			notification_message = "Crate collected.\nContents:\n- ~g~$3000~w~\n- AP Pistol";
//	//		}
//	//	}
//	//	CreateNotification(notification_message, play_notification_beeps);
//	//	//PlayNotificationBeep();
//	//}
//	//if (type_of_event == 4)
//	//	val += 15000;
//
//	STATS::STAT_SET_INT(hash, val, 1);
//}

void ChangeMoneyForCurrentPlayer(int value) {
	int current_player;
	int player_cash;
	if (PED::IS_PED_MODEL(playerPed, GAMEPLAY::GET_HASH_KEY("player_zero"))) current_player = 0;
	if (PED::IS_PED_MODEL(playerPed, GAMEPLAY::GET_HASH_KEY("player_one")))  current_player = 1;
	if (PED::IS_PED_MODEL(playerPed, GAMEPLAY::GET_HASH_KEY("player_two")))  current_player = 2;
	char statNameFull[32];
	sprintf_s(statNameFull, "SP%d_TOTAL_CASH", current_player);
	Hash hash = GAMEPLAY::GET_HASH_KEY(statNameFull);
	STATS::STAT_GET_INT(hash, &player_cash, -1);
	player_cash += value;
	STATS::STAT_SET_INT(hash, player_cash, 1);
}

Hash GetHashOfVehicleModel(Vehicle vehicle) {
	Logger.Write("GetHashOfVehicleModel():", LogExtremelyVerbose);
	return ENTITY::GET_ENTITY_MODEL(vehicle);
}

int GetVehicleClassBitwiseFromHash(Hash hash) {
	Logger.Write("GetVehicleClassFromHash()", LogExtremelyVerbose);
	int raw_int = VEHICLE::GET_VEHICLE_CLASS_FROM_NAME(hash);
	Logger.Write("GetVehicleClassFromHash(): " + std::string(VehicleClasses[raw_int]), LogVerbose); // from strings.h because c++ sucks at some things.
	switch (raw_int) {
	case  0: return Compact;
	case  1: return Sedan;
	case  2: return SUV;
	case  3: return Coupe;
	case  4: return Muscle;
	case  5: return SportsClassic;
	case  6: return Sports;
	case  7: return Super;
	case  8: return Motorcycle;
	case  9: return OffRoad;
	case 10: return Industrial;
	case 11: return Utility;
	case 12: return Van;
	case 13: return Cycle;
	case 14: return Boat;
	case 15: return Helicopter;
	case 16: return Plane;
	case 17: return Service;
	case 18: return Emergency;
	case 19: return Military;
	case 20: return Commercial;
	case 21: return Trainn;
	}
	return 0;
}

void SetPlayerMinimumWantedLevel(WantedLevel wanted_level) {
	Logger.Write("SetPlayerMinimumWantedLevel()", LogExtremelyVerbose);
	if (PLAYER::GET_PLAYER_WANTED_LEVEL(player) < wanted_level) {
		PLAYER::SET_PLAYER_WANTED_LEVEL(player, wanted_level, 0);
		PLAYER::SET_PLAYER_WANTED_LEVEL_NOW(player, 1);
	}
}

bool IsVehicleProbablyParked(Vehicle vehicle) {
	Logger.Write("IsVehicleProbablyParked()", LogExtremelyVerbose);
	if ((VEHICLE::IS_VEHICLE_STOPPED(vehicle)) &&
		(VEHICLE::GET_PED_IN_VEHICLE_SEAT(vehicle, -1) == 0) && // IS_VEHICLE_SEAT_FREE doesn't fucking work.
		(VEHICLE::GET_VEHICLE_NUMBER_OF_PASSENGERS(vehicle) == 0)) {
		return true;
	}
	return false;
}

bool IsVehicleDrivable(Vehicle vehicle) {
	Logger.Write("IsVehicleDrivable()", LogExtremelyVerbose);
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
	Logger.Write("DoesEntityExistAndIsNotNull()", LogExtremelyVerbose);
	return (entity != NULL && ENTITY::DOES_ENTITY_EXIST(entity));
}

std::vector<Hash> GetVehicleModelsFromWorld(Ped ped, std::vector<Hash> vector_of_hashes, int maximum_vector_size) {
	Logger.Write("GetVehicleModelsFromWorld()", LogExtremelyVerbose);
	const int ARR_SIZE = 1024;
	Vehicle all_world_vehicles[ARR_SIZE];
	int count = worldGetAllVehicles(all_world_vehicles, ARR_SIZE);
	Vector3 player_coordinates = ENTITY::GET_ENTITY_COORDS(playerPed, 0);

	if (all_world_vehicles != NULL) {
		for (int i = 0; i < count; i++) {
			if (DoesEntityExistAndIsNotNull(all_world_vehicles[i])) {
				Vehicle this_vehicle = all_world_vehicles[i];
				Vector3 this_vehicle_coordinates = ENTITY::GET_ENTITY_COORDS(this_vehicle, 0);
				if (DoesEntityExistAndIsNotNull(this_vehicle)) {
					Hash this_vehicle_hash = GetHashOfVehicleModel(this_vehicle);
					auto predicate = [this_vehicle_hash](const Hash & item) { // pretty sure this isn't necessary for hashes, but it works.
						return (item == this_vehicle_hash);
					};
					bool found = (std::find_if(vector_of_hashes.begin(), vector_of_hashes.end(), predicate) != vector_of_hashes.end());
					if (!found) {
						char *this_vehicle_display_name = VEHICLE::GET_DISPLAY_NAME_FROM_VEHICLE_MODEL(this_vehicle_hash);
						char *this_vehicle_string_literal = UI::_GET_LABEL_TEXT(this_vehicle_display_name);
						Logger.Write("GetVehicleModelsFromWorld(): " + std::string(this_vehicle_string_literal), LogVeryVerbose);
						//Blip test_blip;
						//test_blip = UI::GET_BLIP_FROM_ENTITY(this_vehicle);
						//if (test_blip) {
						//	UI::SET_BLIP_COLOUR(test_blip, 0);
						//	UI::SET_BLIP_SPRITE(test_blip, 1);
						//}
						vector_of_hashes.push_back(this_vehicle_hash);
					}
					if (vector_of_hashes.size() > maximum_vector_size) vector_of_hashes.erase(vector_of_hashes.begin()); // also pretty sure there's not more than a thousand models in the game, but safety first...
				}	
			}
		}
	}
	return vector_of_hashes;
}

std::vector<Vector4> GetParkedVehiclesFromWorld(Ped ped, std::vector<Vector4> vector_of_vector4s, int maximum_vector_size, int search_range_minimum) {
	Logger.Write("GetParkedCarsFromWorld()", LogExtremelyVerbose);
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
					(VEHICLE::_IS_VEHICLE_SHOP_RESPRAY_ALLOWED(this_vehicle)) && // this doesn't seem to work...
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
					if (!found) {
						Logger.Write("GetParkedCarsFromWorld(): vector_of_vector4s.size():" + std::to_string(vector_of_vector4s.size()), LogVeryVerbose);
						vector_of_vector4s.push_back(this_vehicle_position);
					}
					if (vector_of_vector4s.size() > maximum_vector_size) vector_of_vector4s.erase(vector_of_vector4s.begin()); // just in case it gets filled up, first in first out
					//if (notify_get_parked_cars_in_range) NotifyBottomCenter(&("GetParkedCarsInRange(): " + std::to_string(vector_of_vector4s.size()))[0u]); // lol so much work to get some ints and chars output on screen
				}
			}
		}
	}
	return vector_of_vector4s;
}

Vector4 SelectASpawnPoint(Ped pedestrian, std::vector<Vector4> vector_of_vector4s_to_search, std::vector<Vector4> vector_of_vector4s_to_exclude, uint max_range, uint min_range, uint max_tries) {
	Logger.Write("SelectASpawnPoint()", LogExtremelyVerbose);
	Vector3 pedestrian_coordinates = ENTITY::GET_ENTITY_COORDS(pedestrian, 0);
	Vector4 empty_spawn_point;
	if (vector_of_vector4s_to_search.size() == 0) {
		Logger.Write("SelectASpawnPoint(): vector_of_vector4s_to_search was empty, returning an empty Vector4!", LogError);
		return empty_spawn_point;
	}
	for (uint i = 0; i < max_tries; i++) {
		Vector4 spawn_point = vector_of_vector4s_to_search[(rand() % vector_of_vector4s_to_search.size())]; // pick a random possible point from our set
		float distance = GAMEPLAY::GET_DISTANCE_BETWEEN_COORDS(pedestrian_coordinates.x, pedestrian_coordinates.y, pedestrian_coordinates.z, spawn_point.x, spawn_point.y, spawn_point.z, 0);
		if (distance > min_range && distance < max_range) {
			if (vector_of_vector4s_to_exclude.size() == 0) {
				Logger.Write("SelectASpawnPoint(): selected from " + std::to_string(vector_of_vector4s_to_search.size()) + " possible points after " + std::to_string(i + 1) + " tries ( " + std::to_string(distance) + " meters )", LogVerbose);
				return spawn_point;
			}
				
			auto predicate = [spawn_point](const Vector4 & each_item) {
				return (each_item.x == spawn_point.x && each_item.y == spawn_point.y && each_item.z == spawn_point.z && each_item.h == spawn_point.h);
			};
			//bool found_in_exclude_vector = (std::find_if(vector_of_vector4s_to_exclude.begin(), vector_of_vector4s_to_exclude.end(), predicate) != vector_of_vector4s_to_exclude.end()); // make sure this_vehicle_position does not already exist in vector_of_vector4s
			//OBJECT::DOES_OBJECT_OF_TYPE_EXIST_AT_COORDS(spawn_point.x, spawn_point.y, spawn_point.z, 3.5f, 0, 0); // haven't tested, but requires a hash, unless the boolean at the end is 'any object'?
			bool occupied = GAMEPLAY::IS_POSITION_OCCUPIED(spawn_point.x, spawn_point.y, spawn_point.z, 3.5f, false, true, true, false, false, false, false);
			if (!occupied) {
				Logger.Write("SelectASpawnPoint(): selected from " + std::to_string(vector_of_vector4s_to_search.size()) + " possible points after " + std::to_string(i+1) + " tries ( " + std::to_string(distance) + " meters )", LogVerbose);
				return spawn_point;
			}
		}
		WAIT(0);
	}
	Logger.Write("SelectASpawnPoint(): exceeded " + std::to_string(max_tries) + " tries, returning an empty Vector4!", LogError);
	return empty_spawn_point;
}

Hash SelectAVehicleModel(std::vector<Hash> vector_of_hashes_to_search, uint vehicle_class_options, uint max_tries) {
	Logger.Write("SelectAVehicleModel()", LogExtremelyVerbose);
	Hash empty_hash;
	if (vector_of_hashes_to_search.size() == 0) {
		Logger.Write("SelectAVehicleModel(): vector_of_hashes_to_search was empty, returning an empty Hash!", LogError);
		return empty_hash;
	}
	for (uint i = 0; i < max_tries; i++) {
		Hash vehicle_hash = vector_of_hashes_to_search[(rand() % vector_of_hashes_to_search.size())]; // pick a random model from our set...
		char *this_vehicle_display_name = VEHICLE::GET_DISPLAY_NAME_FROM_VEHICLE_MODEL(vehicle_hash);
		char *this_vehicle_string_literal = UI::_GET_LABEL_TEXT(this_vehicle_display_name);
		Logger.Write("SelectAVehicleModel(): " + std::string(this_vehicle_string_literal) + "...?", LogVeryVerbose);
		int vehicle_class = GetVehicleClassBitwiseFromHash(vehicle_hash);
		if (vehicle_class_options & vehicle_class) {
			Logger.Write("SelectAVehicleModel(): selected a model out of " + std::to_string(vector_of_hashes_to_search.size()) + " Hashes after " + std::to_string(i+1) + " tries ( " + std::string(this_vehicle_string_literal) + " )", LogVerbose);
			return vehicle_hash;
		}
	}
	Logger.Write("SelectAVehicleModel(): exceeded " + std::to_string(max_tries) + " tries, returning an empty Hash!", LogError);
	return empty_hash;
}

Ped SpawnABadGuy(Ped skin, Vector4 spawn_point, float x_margin, float y_margin, float z_margin, char * weapon) {
	spawn_point.x += GetFromUniformRealDistribution(-x_margin, x_margin); spawn_point.y += GetFromUniformRealDistribution(-y_margin, y_margin); spawn_point.z += GetFromUniformRealDistribution(-z_margin, z_margin);
	Ped bad_guy = PED::CREATE_PED(26, skin, spawn_point.x, spawn_point.y, spawn_point.z, 40, false, true);
	PED::SET_PED_RELATIONSHIP_GROUP_HASH(bad_guy, GAMEPLAY::GET_HASH_KEY("HATES_PLAYER"));
	if (weapon = "SURPRISE_ME") {
		if (IsTheUniverseFavorable(0.01)) weapon = "WEAPON_RAILGUN";
		else if (IsTheUniverseFavorable(0.04)) weapon = "WEAPON_RPG";
		else if (IsTheUniverseFavorable(0.04)) weapon = "WEAPON_GRENADELAUNCHER";
		else if (IsTheUniverseFavorable(0.04)) weapon = "WEAPON_MG";
		else if (IsTheUniverseFavorable(0.12)) weapon = "WEAPON_SNIPERRIFLE";
		else if (IsTheUniverseFavorable(0.24)) weapon = "WEAPON_CARBINERIFLE";
		else if (IsTheUniverseFavorable(0.33)) weapon = "WEAPON_SMG";
		else if (IsTheUniverseFavorable(0.33)) weapon = "WEAPON_PUMPSHOTGUN";
		else if (IsTheUniverseFavorable(0.24)) weapon = "WEAPON_PISTOL50";
		else if (IsTheUniverseFavorable(0.24)) weapon = "WEAPON_COMBATPISTOL";
		else weapon = "WEAPON_PISTOL";
	}
	WEAPON::GIVE_DELAYED_WEAPON_TO_PED(bad_guy, GAMEPLAY::GET_HASH_KEY((char *)weapon), 1000, 1);
	WEAPON::SET_CURRENT_PED_WEAPON(bad_guy, GAMEPLAY::GET_HASH_KEY((char *)weapon), 1);
	AI::TASK_WANDER_IN_AREA(bad_guy, spawn_point.x, spawn_point.y, spawn_point.z, 30.0f, 1077936128, 1086324736); // last two values I copped from the native scripts - they're identical in multiple places...
	ENTITY::SET_ENTITY_INVINCIBLE(bad_guy, false);
}

class CrateDropMission {
public:
	MissionType Prepare();
	MissionType Execute();
	MissionType Timeout();
private:
	Blip crate_blip_ = NULL;
	Vector4 crate_spawn_location_;
	bool crate_is_special_ = false;
	bool objects_were_spawned_ = false;
	Object crate_;
	Ped guard_1_, guard_2_, guard_3_, guard_4_, guard_5_, guard_6_, guard_7_, guard_8_;
};

MissionType CrateDropMission::Prepare() {
	Logger.Write("CrateDropMission::Prepare()", LogVerbose);
	Vector3 player_coordinates = ENTITY::GET_ENTITY_COORDS(playerPed, 0);
	std::vector<Vector4> crate_spawn_locations;
	Vector4 v1; v1.x = -1782.33f; v1.y = -826.29f; v1.z = 7.83f; crate_spawn_locations.push_back(v1);
	Vector4 v2; v2.x = 1067.85; v2.y = 2362.45; v2.z = 43.87; crate_spawn_locations.push_back(v2);
	Vector4 v3; v3.x = -3.77; v3.y = 6840.14; v3.z = 13.46; crate_spawn_locations.push_back(v3);
	Vector4 v4; v4.x = 2061.37; v4.y = 2798.68; v4.z = 50.29; crate_spawn_locations.push_back(v4);
	Vector4 v5; v5.x = 1883.24; v5.y = 278.48; v5.z = 162.73; crate_spawn_locations.push_back(v5);
	Vector4 v6; v6.x = -2912.36; v6.y = 3077.48; v6.z = 3.39; crate_spawn_locations.push_back(v6);
	Vector4 v7; v7.x = 2822.70; v7.y = -634.39; v7.z = 2.15; crate_spawn_locations.push_back(v7);
	Vector4 v8; v8.x = 2184.12; v8.y = 5026.099; v8.z = 42.63; crate_spawn_locations.push_back(v8);
	Vector4 v9; v9.x = -3147.02; v9.y = 305.27; v9.z = 2.35; crate_spawn_locations.push_back(v9);
	Vector4 v10; v10.x = 1552.40; v10.y = 6644.04; v10.z = 2.55; crate_spawn_locations.push_back(v10);
	// Someday I'll figure out something to expand this. Not today.
	std::vector<Vector4> empty_vector;
	crate_spawn_location_ = SelectASpawnPoint(playerPed, crate_spawn_locations, empty_vector, spawn_point_maximum_range, spawn_point_minimum_range, number_of_tries_to_select_items);
	if (crate_spawn_location_.x == 0.0f && crate_spawn_location_.y == 0.0f && crate_spawn_location_.z == 0.0f && crate_spawn_location_.h == 0.0f) return NO_Mission;
	if (IsTheUniverseFavorable(0.05)) crate_is_special_ = true;
	crate_blip_ = UI::ADD_BLIP_FOR_COORD(crate_spawn_location_.x, crate_spawn_location_.y, crate_spawn_location_.z);
	if (use_default_blip) UI::SET_BLIP_SPRITE(crate_blip_, 1); 
	else UI::SET_BLIP_SPRITE(crate_blip_, 306); 
	UI::SET_BLIP_SCALE(crate_blip_, 1.5);
	if (crate_is_special_) UI::SET_BLIP_COLOUR(crate_blip_, 5);
	else UI::SET_BLIP_COLOUR(crate_blip_, 2);
	UI::SET_BLIP_DISPLAY(crate_blip_, (char)"you will never see this");
	if (crate_is_special_) CreateNotification("A ~y~special crate~w~ has been dropped.", play_notification_beeps);
	else CreateNotification("A ~g~crate~w~ has been dropped.", play_notification_beeps);
	return CrateDrop;
}

MissionType CrateDropMission::Execute() {
	Logger.Write("CrateDropMission::Execute()", LogExtremelyVerbose);
	Vector3 player_coordinates = ENTITY::GET_ENTITY_COORDS(playerPed, 0);
	int distance_to_crate = GAMEPLAY::GET_DISTANCE_BETWEEN_COORDS(player_coordinates.x, player_coordinates.y, player_coordinates.z, crate_spawn_location_.x, crate_spawn_location_.y, crate_spawn_location_.z, 0);
	if (distance_to_crate < 300 && !objects_were_spawned_) {
		Logger.Write("attempting to spawn objects", LogVerbose);
		STREAMING::REQUEST_MODEL(GAMEPLAY::GET_HASH_KEY("prop_box_ammo04a"));
		while (!STREAMING::HAS_MODEL_LOADED(GAMEPLAY::GET_HASH_KEY("prop_box_ammo04a"))) WAIT(0);
		crate_ = OBJECT::CREATE_AMBIENT_PICKUP(0x14568F28, crate_spawn_location_.x, crate_spawn_location_.y, crate_spawn_location_.z, -1, 0, GAMEPLAY::GET_HASH_KEY("prop_box_ammo04a"), 1, 1);
		ENTITY::SET_ENTITY_ALPHA(crate_, 255, 1);
		while (ENTITY::GET_ENTITY_HEIGHT(crate_, crate_spawn_location_.x, crate_spawn_location_.y, crate_spawn_location_.z, 1, 1) > 0) OBJECT::PLACE_OBJECT_ON_GROUND_PROPERLY(crate_);
		ENTITY::FREEZE_ENTITY_POSITION(crate_, 1);
		UI::REMOVE_BLIP(&crate_blip_);
		crate_blip_ = UI::ADD_BLIP_FOR_ENTITY(crate_);
		if (use_default_blip) UI::SET_BLIP_SPRITE(crate_blip_, 1);
		else UI::SET_BLIP_SPRITE(crate_blip_, 306);
		UI::SET_BLIP_SCALE(crate_blip_, 1.5);
		if (crate_is_special_) UI::SET_BLIP_COLOUR(crate_blip_, 5);
		else UI::SET_BLIP_COLOUR(crate_blip_, 2);
		UI::SET_BLIP_DISPLAY(crate_blip_, (char)"you will never see this");

		Ped skin = GAMEPLAY::GET_HASH_KEY("mp_g_m_pros_01");
		STREAMING::REQUEST_MODEL(skin);
		while (!STREAMING::HAS_MODEL_LOADED(skin)) WAIT(0);
		if (!ENTITY::DOES_ENTITY_EXIST(guard_1_)) guard_1_ = SpawnABadGuy(skin, crate_spawn_location_, 10, 10, 0, "SURPRISE_ME");
		if (!ENTITY::DOES_ENTITY_EXIST(guard_2_)) guard_2_ = SpawnABadGuy(skin, crate_spawn_location_, 10, 10, 0, "SURPRISE_ME");
		if (!ENTITY::DOES_ENTITY_EXIST(guard_3_)) guard_3_ = SpawnABadGuy(skin, crate_spawn_location_, 10, 10, 0, "SURPRISE_ME");
		if (!ENTITY::DOES_ENTITY_EXIST(guard_4_)) guard_4_ = SpawnABadGuy(skin, crate_spawn_location_, 10, 10, 0, "SURPRISE_ME");
		if (!ENTITY::DOES_ENTITY_EXIST(guard_5_)) guard_5_ = SpawnABadGuy(skin, crate_spawn_location_, 10, 10, 0, "SURPRISE_ME");
		if (!ENTITY::DOES_ENTITY_EXIST(guard_6_) && crate_is_special_) guard_6_ = SpawnABadGuy(skin, crate_spawn_location_, 10, 10, 0, "SURPRISE_ME");
		if (!ENTITY::DOES_ENTITY_EXIST(guard_7_) && crate_is_special_) guard_7_ = SpawnABadGuy(skin, crate_spawn_location_, 10, 10, 0, "SURPRISE_ME");
		if (!ENTITY::DOES_ENTITY_EXIST(guard_8_) && crate_is_special_) guard_8_ = SpawnABadGuy(skin, crate_spawn_location_, 10, 10, 0, "SURPRISE_ME");
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
		ENTITY::SET_PED_AS_NO_LONGER_NEEDED(&guard_1_);
		ENTITY::SET_PED_AS_NO_LONGER_NEEDED(&guard_2_);
		ENTITY::SET_PED_AS_NO_LONGER_NEEDED(&guard_3_);
		ENTITY::SET_PED_AS_NO_LONGER_NEEDED(&guard_4_);
		ENTITY::SET_PED_AS_NO_LONGER_NEEDED(&guard_5_);
		if (crate_is_special_) {
			ENTITY::SET_PED_AS_NO_LONGER_NEEDED(&guard_6_);
			ENTITY::SET_PED_AS_NO_LONGER_NEEDED(&guard_7_);
			ENTITY::SET_PED_AS_NO_LONGER_NEEDED(&guard_8_);
			ChangeMoneyForCurrentPlayer(GetFromUniformIntDistribution(50000, 150000));
		}
		else ChangeMoneyForCurrentPlayer(GetFromUniformIntDistribution(25000, 75000));
		CreateNotification("The drop was acquired.", play_notification_beeps);
		return NO_Mission;
	}
	return CrateDrop;
}

MissionType CrateDropMission::Timeout() {
	Logger.Write("CrateDropMission::Timeout()", LogExtremelyVerbose);
	Vector3 player_coordinates = ENTITY::GET_ENTITY_COORDS(playerPed, 0);
	uint distance_to_crate = GAMEPLAY::GET_DISTANCE_BETWEEN_COORDS(player_coordinates.x, player_coordinates.y, player_coordinates.z, crate_spawn_location_.x, crate_spawn_location_.y, crate_spawn_location_.z, 0);
	if (distance_to_crate > mission_minimum_range_for_timeout || ENTITY::IS_ENTITY_DEAD(playerPed)) {
		UI::REMOVE_BLIP(&crate_blip_);
		ENTITY::SET_OBJECT_AS_NO_LONGER_NEEDED(&crate_);
		ENTITY::SET_PED_AS_NO_LONGER_NEEDED(&guard_1_);
		ENTITY::SET_PED_AS_NO_LONGER_NEEDED(&guard_2_);
		ENTITY::SET_PED_AS_NO_LONGER_NEEDED(&guard_3_);
		ENTITY::SET_PED_AS_NO_LONGER_NEEDED(&guard_4_);
		ENTITY::SET_PED_AS_NO_LONGER_NEEDED(&guard_5_);
		CreateNotification("The ~g~crate~w~ has been claimed by smugglers.", play_notification_beeps);
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
	Ped truck_passenger_ = NULL;
	Blip truck_blip_ = NULL;
};

MissionType ArmoredTruckMission::Prepare() {
	Logger.Write("ArmoredTruckMission::Prepare()", LogVerbose);
	Vector4 vehicle_spawn_position = SelectASpawnPoint(playerPed, vehicle_spawn_points, reserved_vehicle_spawn_points, spawn_point_maximum_range, spawn_point_minimum_range, number_of_tries_to_select_items);
	if (vehicle_spawn_position.x == 0.0f && vehicle_spawn_position.y == 0.0f && vehicle_spawn_position.z == 0.0f && vehicle_spawn_position.h == 0.0f) return NO_Mission;
	STREAMING::REQUEST_MODEL(GAMEPLAY::GET_HASH_KEY("stockade"));
	while (!STREAMING::HAS_MODEL_LOADED(GAMEPLAY::GET_HASH_KEY("stockade"))) WAIT(0);
	armored_truck_ = VEHICLE::CREATE_VEHICLE(GAMEPLAY::GET_HASH_KEY("stockade"), vehicle_spawn_position.x, vehicle_spawn_position.y, vehicle_spawn_position.z, vehicle_spawn_position.h, 0, 0);
	VEHICLE::SET_VEHICLE_ON_GROUND_PROPERLY(armored_truck_);
	STREAMING::REQUEST_MODEL(GAMEPLAY::GET_HASH_KEY("mp_s_m_armoured_01"));
	while (!STREAMING::HAS_MODEL_LOADED(GAMEPLAY::GET_HASH_KEY("mp_s_m_armoured_01"))) WAIT(0);
	truck_driver_ = PED::CREATE_PED_INSIDE_VEHICLE(armored_truck_, 26, GAMEPLAY::GET_HASH_KEY("mp_s_m_armoured_01"), -1, false, false);
	if (IsTheUniverseFavorable(0.5)) truck_passenger_ = PED::CREATE_PED_INSIDE_VEHICLE(armored_truck_, 26, GAMEPLAY::GET_HASH_KEY("mp_s_m_armoured_01"), 0, false, false);
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
	Logger.Write("ArmoredTruckMission::Execute()", LogExtremelyVerbose);
	Vector4 truck_position = GetVector4OfEntity(armored_truck_);
	if (!VEHICLE::IS_VEHICLE_DRIVEABLE(armored_truck_, 1)) {
		CreateNotification("The ~b~armored truck~w~ has been destroyed. The cash cases inside have been ruined.", play_notification_beeps);
		UI::REMOVE_BLIP(&truck_blip_);
		ENTITY::SET_PED_AS_NO_LONGER_NEEDED(&truck_driver_);
		ENTITY::SET_VEHICLE_AS_NO_LONGER_NEEDED(&armored_truck_);
		return NO_Mission;
	}
	if (PED::IS_PED_IN_VEHICLE(playerPed, armored_truck_, 0)) {
		WAIT(500);
		Logger.Write("ArmoredTruckMission::Execute(): PED::IS_PED_IN_VEHICLE = TRUE", LogExtremelyVerbose);
		VEHICLE::SET_VEHICLE_DOOR_OPEN(armored_truck_, 2, true, false);
		VEHICLE::SET_VEHICLE_DOOR_OPEN(armored_truck_, 3, true, false);
		VEHICLE::SET_VEHICLE_DOOR_OPEN(armored_truck_, 6, true, false); // maybe these?
		VEHICLE::SET_VEHICLE_DOOR_OPEN(armored_truck_, 7, true, false);
	}
	if (VEHICLE::IS_VEHICLE_DOOR_FULLY_OPEN(armored_truck_, 2) || VEHICLE::IS_VEHICLE_DOOR_FULLY_OPEN(armored_truck_, 3)) {
		Pickup case1; Pickup case2; Pickup case3;
		Vector3 behind_truck = GetCoordinatesByOriginBearingAndDistance(truck_position, 180, 2.5f);
		int random = rand() % 3;
		switch (random) { // falls through!
		case 0:
			SetPlayerMinimumWantedLevel(Wanted_Three);
			case3 = OBJECT::CREATE_AMBIENT_PICKUP(0xDE78F17E, behind_truck.x, behind_truck.y, behind_truck.z + 1, -1, GetFromUniformIntDistribution(5, 25) * 1000, 0, 1, 1);
			ENTITY::SET_OBJECT_AS_NO_LONGER_NEEDED(&case3);
		case 1:
			SetPlayerMinimumWantedLevel(Wanted_Two);
			case2 = OBJECT::CREATE_AMBIENT_PICKUP(0xDE78F17E, behind_truck.x, behind_truck.y, behind_truck.z + 1, -1, GetFromUniformIntDistribution(5, 25) * 1000 , 0, 1, 1);
			ENTITY::SET_OBJECT_AS_NO_LONGER_NEEDED(&case2);
		case 2:
			SetPlayerMinimumWantedLevel(Wanted_One);
			case1 = OBJECT::CREATE_AMBIENT_PICKUP(0xDE78F17E, behind_truck.x, behind_truck.y, behind_truck.z + 1, -1, GetFromUniformIntDistribution(5, 25) * 1000, 0, 1, 1);
			ENTITY::SET_OBJECT_AS_NO_LONGER_NEEDED(&case1);
		}
		CreateNotification("The ~b~armored truck's~w~ doors have been opened. Cash has been dropped.", play_notification_beeps);
		UI::REMOVE_BLIP(&truck_blip_);
		ENTITY::SET_PED_AS_NO_LONGER_NEEDED(&truck_driver_);
		ENTITY::SET_VEHICLE_AS_NO_LONGER_NEEDED(&armored_truck_);
		return NO_Mission;
	}
	return ArmoredTruck;
}

MissionType ArmoredTruckMission::Timeout() {
	Logger.Write("ArmoredTruckMission::Timeout()", LogExtremelyVerbose);
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
	Logger.Write("AssassinationMission::Prepare()", LogVerbose);
	Vector4 target_spawn_position = SelectASpawnPoint(playerPed, vehicle_spawn_points, reserved_vehicle_spawn_points, spawn_point_maximum_range, spawn_point_minimum_range, number_of_tries_to_select_items);
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
	CreateNotification("A hit has been placed on a ~r~target~w~.", play_notification_beeps);
	return Assassination;
}

MissionType AssassinationMission::Execute() {
	Logger.Write("AssassinationMission::Execute()", LogExtremelyVerbose);
	if (ENTITY::IS_ENTITY_DEAD(assassination_target_)) {
		UI::REMOVE_BLIP(&target_blip_);
		ENTITY::SET_PED_AS_NO_LONGER_NEEDED(&assassination_target_);
		ChangeMoneyForCurrentPlayer(GetFromUniformIntDistribution(5, 25) * 1000);
		CreateNotification("The ~r~target~w~ has been eliminated.", play_notification_beeps);
		return NO_Mission;
	}
	return Assassination;
}

MissionType AssassinationMission::Timeout() {
	Logger.Write("AssassinationMission::Timeout()", LogExtremelyVerbose);
	Vector3 player_coordinates = ENTITY::GET_ENTITY_COORDS(playerPed, 0);
	Vector3 target_coordinates = ENTITY::GET_ENTITY_COORDS(assassination_target_, 0);
	uint distance_to_target = GAMEPLAY::GET_DISTANCE_BETWEEN_COORDS(target_coordinates.x, target_coordinates.y, target_coordinates.z, player_coordinates.x, player_coordinates.y, player_coordinates.z, 0);
	if (distance_to_target > mission_minimum_range_for_timeout || ENTITY::IS_ENTITY_DEAD(playerPed)) {
		CreateNotification("The contract on the ~r~target~w~ has expired.", play_notification_beeps);
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
	Logger.Write("DestroyVehicleMission::Prepare()", LogVerbose);
	Vector4 vehicle_spawn_position = SelectASpawnPoint(playerPed, vehicle_spawn_points, reserved_vehicle_spawn_points, spawn_point_maximum_range, spawn_point_minimum_range, number_of_tries_to_select_items);
	if (vehicle_spawn_position.x == 0.0f && vehicle_spawn_position.y == 0.0f && vehicle_spawn_position.z == 0.0f && vehicle_spawn_position.h == 0.0f) return NO_Mission;
	Hash vehicle_hash = SelectAVehicleModel(possible_vehicle_models, default_destroyable_vehicle_classes, number_of_tries_to_select_items);
	if (vehicle_hash == NULL) return NO_Mission;
	STREAMING::REQUEST_MODEL(vehicle_hash);
	while (!STREAMING::HAS_MODEL_LOADED(vehicle_hash)) WAIT(0);
	vehicle_to_destroy_ = VEHICLE::CREATE_VEHICLE(vehicle_hash, vehicle_spawn_position.x, vehicle_spawn_position.y, vehicle_spawn_position.z, vehicle_spawn_position.h, 0, 0);
	VEHICLE::SET_VEHICLE_ON_GROUND_PROPERLY(vehicle_to_destroy_);
	vehicle_blip_ = UI::ADD_BLIP_FOR_ENTITY(vehicle_to_destroy_);
	if (use_default_blip) UI::SET_BLIP_SPRITE(vehicle_blip_, 1);
	else if (VEHICLE::IS_THIS_MODEL_A_CAR(vehicle_hash)) UI::SET_BLIP_SPRITE(vehicle_blip_, 225);
	else UI::SET_BLIP_SPRITE(vehicle_blip_, 226);
	UI::SET_BLIP_COLOUR(vehicle_blip_, 1);
	UI::SET_BLIP_DISPLAY(vehicle_blip_, (char)"you will never see this");
	CreateNotification("A ~r~smuggler's vehicle~w~ has been identified.", play_notification_beeps);
	return DestroyVehicle;
}

MissionType DestroyVehicleMission::Execute() {
	Logger.Write("DestroyVehicleMission::Execute()", LogExtremelyVerbose);
	if (!VEHICLE::IS_VEHICLE_DRIVEABLE(vehicle_to_destroy_, 1)) {
		UI::REMOVE_BLIP(&vehicle_blip_);
		ENTITY::SET_VEHICLE_AS_NO_LONGER_NEEDED(&vehicle_to_destroy_);
		ChangeMoneyForCurrentPlayer(GetFromUniformIntDistribution(5, 25) * 1000);
		CreateNotification("~r~Vehicle~w~ destroyed.", play_notification_beeps);
		return NO_Mission;
	}
	return DestroyVehicle;
}

MissionType DestroyVehicleMission::Timeout() {
	Logger.Write("DestroyVehicleMission::Timeout()", LogExtremelyVerbose);
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
	int stealable_vehicle_flags;
	Vehicle vehicle_to_steal_ = NULL;
	Hash vehicle_hash_;
	Vector3 drop_off_coordinates;
	Blip vehicle_blip_ = NULL;
	Blip drop_off_blip_ = NULL;
	int vehicle_primary_color_initial_, vehicle_secondary_color_initial_;
	int vehicle_primary_color_compare_, vehicle_secondary_color_compare_;
	enum StealVehicleMissionStage { GetCar, ResprayCar, FixCar, DeliverCar };
	StealVehicleMissionStage mission_objective_;
	bool vehicle_must_be_undamaged_;
};

MissionType StealVehicleMission::Prepare() {
	Logger.Write("StealVehicleMission.Prepare()", LogVerbose);
	drop_off_coordinates.x = 1226.06; drop_off_coordinates.y = -3231.36; drop_off_coordinates.z = 5.02;
	Vector4 vehicle_spawn_position = SelectASpawnPoint(playerPed, vehicle_spawn_points, reserved_vehicle_spawn_points, spawn_point_maximum_range, spawn_point_minimum_range, number_of_tries_to_select_items);
	if (vehicle_spawn_position.x == 0.0f && vehicle_spawn_position.y == 0.0f && vehicle_spawn_position.z == 0.0f && vehicle_spawn_position.h == 0.0f) return NO_Mission;
	stealable_vehicle_flags = default_stealable_vehicle_flags; // we want some sort of weighted distribution of cars to steal...
	if (IsTheUniverseFavorable(0.6666)) stealable_vehicle_flags = stealable_vehicle_flags & ~Super; // disallow Supers 66% of the time.
	if (IsTheUniverseFavorable(0.3333)) stealable_vehicle_flags = stealable_vehicle_flags & ~Sports; // disallow Sports 33% of the time.
	if (IsTheUniverseFavorable(0.3333)) stealable_vehicle_flags = stealable_vehicle_flags & ~SportsClassic; // etc...
	if (IsTheUniverseFavorable(0.3333)) stealable_vehicle_flags = stealable_vehicle_flags & ~OffRoad; // etc...
	vehicle_hash_ = SelectAVehicleModel(possible_vehicle_models, stealable_vehicle_flags, number_of_tries_to_select_items);
	if (vehicle_hash_ == NULL) return NO_Mission;
	if (ENTITY::WOULD_ENTITY_BE_OCCLUDED(vehicle_hash_, vehicle_spawn_position.x, vehicle_spawn_position.y, vehicle_spawn_position.z, true)) return NO_Mission; // this doesn't always work (failed consistently testing against a stockade) but works enough, considering there's no GET_ENTITY_AT_COORDS
	STREAMING::REQUEST_MODEL(vehicle_hash_); // oh wait no, we can just load it.
	while (!STREAMING::HAS_MODEL_LOADED(vehicle_hash_)) WAIT(0);
	vehicle_to_steal_ = VEHICLE::CREATE_VEHICLE(vehicle_hash_, vehicle_spawn_position.x, vehicle_spawn_position.y, vehicle_spawn_position.z, vehicle_spawn_position.h, 0, 0);
	VEHICLE::SET_VEHICLE_ON_GROUND_PROPERLY(vehicle_to_steal_);
	//if (IsTheUniverseFavorable(1.0)) VEHICLE::SET_VEHICLE_IS_STOLEN(vehicle_to_steal_, true); // doesn't work.
	if (IsTheUniverseFavorable(0.6666)) VEHICLE::SET_VEHICLE_DOORS_LOCKED(vehicle_to_steal_, 7); 
	if (IsTheUniverseFavorable(0.6666)) VEHICLE::SET_VEHICLE_NEEDS_TO_BE_HOTWIRED(vehicle_to_steal_, true);
	vehicle_blip_ = UI::ADD_BLIP_FOR_ENTITY(vehicle_to_steal_);
	if (use_default_blip) UI::SET_BLIP_SPRITE(vehicle_blip_, 1);
	else if (VEHICLE::IS_THIS_MODEL_A_CAR(vehicle_hash_)) UI::SET_BLIP_SPRITE(vehicle_blip_, 225);
	else UI::SET_BLIP_SPRITE(vehicle_blip_, 226);
	UI::SET_BLIP_COLOUR(vehicle_blip_, 5);
	UI::SET_BLIP_DISPLAY(vehicle_blip_, (char)"you will never see this");
	VEHICLE::GET_VEHICLE_COLOURS(vehicle_to_steal_, &vehicle_primary_color_initial_, &vehicle_secondary_color_initial_);
	if (IsTheUniverseFavorable(0.1666)) {
		vehicle_must_be_undamaged_ = true;
		CreateNotification("A special ~y~vehicle~w~ has been requested for retrieval.", play_notification_beeps);
		WAIT(1500);
		CreateNotification("Additionally, the client requested that it be delivered without damage.", play_notification_beeps);
	} else {
		vehicle_must_be_undamaged_ = false;
		CreateNotification("A special ~y~vehicle~w~ has been requested for retrieval.", play_notification_beeps);
	}
	mission_objective_ = GetCar;
	return StealVehicle;
}

MissionType StealVehicleMission::Execute() {
	Logger.Write("StealVehicleMission::Execute()", LogExtremelyVerbose);
	if (!VEHICLE::IS_VEHICLE_DRIVEABLE(vehicle_to_steal_, 1)) {
		CreateNotification("The ~y~vehicle~w~ has been destroyed.", play_notification_beeps);
		if (mission_objective_ == DeliverCar) UI::REMOVE_BLIP(&drop_off_blip_);
		UI::REMOVE_BLIP(&vehicle_blip_);
		ENTITY::SET_VEHICLE_AS_NO_LONGER_NEEDED(&vehicle_to_steal_);
		return NO_Mission;
	}

	if (mission_objective_ == GetCar) {
		if (PED::IS_PED_IN_VEHICLE(playerPed, vehicle_to_steal_, 0)) {
			if (GetVehicleClassBitwiseFromHash(vehicle_hash_) == Super && IsTheUniverseFavorable(0.3333)) SetPlayerMinimumWantedLevel(Wanted_Three);
			else if (IsTheUniverseFavorable(0.3333)) SetPlayerMinimumWantedLevel(Wanted_Two);
			else if (IsTheUniverseFavorable(0.6666)) SetPlayerMinimumWantedLevel(Wanted_One);
			CreateNotification("Respray the ~y~vehicle~w~ before turning it in.", play_notification_beeps);
			mission_objective_ = ResprayCar;
		}
	}
	
	if (mission_objective_ == ResprayCar) {
		VEHICLE::GET_VEHICLE_COLOURS(vehicle_to_steal_, &vehicle_primary_color_compare_, &vehicle_secondary_color_compare_);
		if (vehicle_primary_color_initial_ != vehicle_primary_color_compare_ && ENTITY::GET_ENTITY_SPEED(vehicle_to_steal_) != 0 ||
			vehicle_secondary_color_initial_ != vehicle_secondary_color_compare_ && ENTITY::GET_ENTITY_SPEED(vehicle_to_steal_) != 0) {
			drop_off_blip_ = UI::ADD_BLIP_FOR_COORD(drop_off_coordinates.x, drop_off_coordinates.y, drop_off_coordinates.z);
			if (use_default_blip == 0) UI::SET_BLIP_SPRITE(drop_off_blip_, 50);
			else UI::SET_BLIP_SPRITE(drop_off_blip_, 1);
			UI::SET_BLIP_COLOUR(drop_off_blip_, 5);
			UI::SET_BLIP_DISPLAY(drop_off_blip_, (char)"you will never see this");
			CreateNotification("The ~y~vehicle~w~ is ready to be delivered.", play_notification_beeps);
			mission_objective_ = DeliverCar;
		}
	}

	if (mission_objective_ == FixCar) {
		if (!VEHICLE::_IS_VEHICLE_DAMAGED(vehicle_to_steal_)) {
			CreateNotification("The ~y~vehicle~w~ is ready to be delivered.", play_notification_beeps);
			mission_objective_ = DeliverCar;
		}
	}

	if (mission_objective_ == DeliverCar) {
		if (vehicle_must_be_undamaged_ && VEHICLE::_IS_VEHICLE_DAMAGED(vehicle_to_steal_)) {
			CreateNotification("Repair the ~y~vehicle~w~ before making delivery.", play_notification_beeps);
			mission_objective_ = FixCar;
		}
		GRAPHICS::DRAW_MARKER(1, drop_off_coordinates.x, drop_off_coordinates.y, drop_off_coordinates.z, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 5.0f, 5.0f, 1.0f, 204, 204, 0, 100, false, false, 2, false, false, false, false); // redraws every frame, no need to remove later
		Vector3 vehicle_coordinates = ENTITY::GET_ENTITY_COORDS(vehicle_to_steal_, 0);
		float dropOffDistance = GAMEPLAY::GET_DISTANCE_BETWEEN_COORDS(vehicle_coordinates.x, vehicle_coordinates.y, vehicle_coordinates.z, drop_off_coordinates.x, drop_off_coordinates.y, drop_off_coordinates.z, 0);
		if (dropOffDistance < 0.7f && PED::IS_PED_IN_VEHICLE(playerPed, vehicle_to_steal_, 0)) {
			VEHICLE::SET_VEHICLE_FORWARD_SPEED(vehicle_to_steal_, 0.0f);
			while (ENTITY::GET_ENTITY_SPEED(vehicle_to_steal_) != 0) WAIT(0);
			AI::TASK_LEAVE_VEHICLE(playerPed, vehicle_to_steal_, 0);
			VEHICLE::SET_VEHICLE_UNDRIVEABLE(vehicle_to_steal_, 1);
			int reward_amount_by_class;
			switch (GetVehicleClassBitwiseFromHash(vehicle_hash_)) {
			case Compact: reward_amount_by_class = 1969; break; // derived from a combination of SP and Online values for Legendary Motorsports
			case Sedan : reward_amount_by_class = 12200; break; // and Los Santos Customs. Probably not very accurate...
			case SUV : reward_amount_by_class = 13578; break;
			case Coupe : reward_amount_by_class = 20646; break;
			case Muscle : reward_amount_by_class = 14036; break;
			case SportsClassic : reward_amount_by_class = 92113; break;
			case Sports : reward_amount_by_class = 26555; break;
			case Super : reward_amount_by_class = 121232; break;
			case Motorcycle : reward_amount_by_class = 34171; break;
			case OffRoad : reward_amount_by_class = 10750; break;
			//case Industrial : reward_amount_by_class = 0; break;
			case Utility : reward_amount_by_class = 3500; break;
			case Van : reward_amount_by_class = 1886; break;
			//case Cycle : reward_amount_by_class = 0; break;
			//case Boat : reward_amount_by_class = 0; break;
			//case Helicopter : reward_amount_by_class = 0; break;
			//case Plane : reward_amount_by_class = 0; break;
			//case Service : reward_amount_by_class = 0; break;
			//case Emergency : reward_amount_by_class = 0; break;
			//case Military : reward_amount_by_class = 0; break;
			//case Commercial : reward_amount_by_class = 0; break;
			//case Train : reward_amount_by_class = 0; break;
			default: reward_amount_by_class = 29386; break; // until I define values for non-default classes, this average of all values will fill in.
			}
			float reward_modifier = GetFromUniformRealDistribution(-0.5f, 1.5f); // you lose some, you win some more.
			ChangeMoneyForCurrentPlayer(int(reward_amount_by_class * reward_modifier)); // who cares about rounding errors?
			CreateNotification("The ~y~vehicle~w~ has been delivered.", play_notification_beeps);
			UI::REMOVE_BLIP(&drop_off_blip_);
			UI::REMOVE_BLIP(&vehicle_blip_);
			ENTITY::SET_VEHICLE_AS_NO_LONGER_NEEDED(&vehicle_to_steal_);
			return NO_Mission;
		}
	}
	return StealVehicle;
}

MissionType StealVehicleMission::Timeout() {
	Logger.Write("StealVehicleMission::Timeout()", LogExtremelyVerbose);
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
	Logger.Write("MissionHandler::Update()", LogExtremelyVerbose);
	
	if (current_mission_type_ == NO_Mission) ticks_since_last_mission_ += (GetTickCount64() - tick_count_at_last_update_);
	else ticks_since_mission_start_ += (GetTickCount64() - tick_count_at_last_update_);
	tick_count_at_last_update_ = GetTickCount64();

	if (ticks_since_last_mission_ > ticks_between_missions_) { // Enough time has passed that we can start a new mission.
		current_mission_type_ = MissionType(rand() % MAX_Mission); // I used to think this was silly. Now I think it's awesome.
		switch (current_mission_type_) {
		case StealVehicle	:	current_mission_type_ = StealVehicleMission.Prepare();		break; // Prepare()s should return their MissionType on success.
		case DestroyVehicle	:	current_mission_type_ = DestroyVehicleMission.Prepare();	break; // If something goes wrong (StealVehicleMission takes too
		case Assassination	:	current_mission_type_ = AssassinationMission.Prepare();		break; // long to find a vehicle), they can return NO_Mission
		case ArmoredTruck	:	current_mission_type_ = ArmoredTruckMission.Prepare();		break;
		case CrateDrop		:	current_mission_type_ = CrateDropMission.Prepare();			break;
		}
		if (current_mission_type_ != NO_Mission) {
			ticks_since_last_mission_ = 0;  // Clear the timer since the last mission. This won't increment again until current_mission_type_ equals NO_Mission.
			ticks_since_mission_start_ = 0; // initialize the timeout delay as well, since it's about to begin incrementing.
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
	Logger.Write("Update()", LogExtremelyVerbose);
	playerPed = PLAYER::PLAYER_PED_ID();
	player = PLAYER::PLAYER_ID(); // need to be updated every cycle?
	vehicle_spawn_points = GetParkedVehiclesFromWorld(playerPed, vehicle_spawn_points, maximum_number_of_spawn_points, vehicle_search_range_minimum);
	possible_vehicle_models = GetVehicleModelsFromWorld(playerPed, possible_vehicle_models, maximum_number_of_vehicle_models);
	return;
}

void WaitDuringDeathArrestOrLoading(uint seconds) {
	Logger.Write("PauseDuringDeathArrestOrLoading()", LogExtremelyVerbose);
	if (ENTITY::IS_ENTITY_DEAD(PLAYER::PLAYER_PED_ID())) {
		Logger.Write("PauseDuringDeathArrestOrLoading(): Player is Wasted, waiting for player...", LogNormal);
		while (ENTITY::IS_ENTITY_DEAD(PLAYER::PLAYER_PED_ID())) WAIT(0);
	}

	if (PLAYER::IS_PLAYER_BEING_ARRESTED(PLAYER::PLAYER_ID(), TRUE)) {
		Logger.Write("PauseDuringDeathArrestOrLoading(): Player is Busted, waiting for player...", LogNormal);
		while (PLAYER::IS_PLAYER_BEING_ARRESTED(PLAYER::PLAYER_ID(), TRUE)) WAIT(0);
	}
	if (DLC2::GET_IS_LOADING_SCREEN_ACTIVE()) {
		Logger.Write("PauseDuringDeathArrestOrLoading(): Loading screen is active, waiting for game...", LogNormal);
		while (DLC2::GET_IS_LOADING_SCREEN_ACTIVE()) WAIT(0);
		
	}
	WAIT(seconds * 1000);
	Logger.Write("PauseDuringDeathArrestOrLoading(): Returning to game.", LogNormal);
	return;
}

void UglyHackForVehiclePersistenceScripts(uint seconds) {
	Logger.Write("UglyHackForVehiclePersistenceScripts()", LogNormal);
	if ((seconds == 0)) return; // no point in wasting time or spawns if the user doesn't use a persistence script.
	WAIT(DWORD(seconds * 1000)); // let the persistence script start up and spawn its own vehicles.
	reserved_vehicle_spawn_points = GetParkedVehiclesFromWorld(playerPed, reserved_vehicle_spawn_points, maximum_number_of_spawn_points, vehicle_search_range_minimum); // set aside these spawns. Note that some legitimate spawns will invariably get reserved. This is why it's an ugly hack.
	Logger.Write("reserved_vehicle_spawn_points_parked.size(): " + std::to_string(reserved_vehicle_spawn_points.size()), LogNormal);
	return;
}

void GetSettingsFromIniFile() {
	Logger.Write("GetSettingsFromIniFile()", LogNormal);
	// OPTIONS
	play_notification_beeps = Reader.ReadBoolean("Options", "play_notification_beeps", true);
	use_default_blip = Reader.ReadBoolean("Options", "use_default_blip", false);
	mission_timeout = std::max(Reader.ReadInteger("Options", "mission_timeout", 360), 180);
	mission_cooldown = std::max(Reader.ReadInteger("Options", "mission_cooldown", 60), 30); // these minimums should be mentioned in the ini.
	spawn_point_minimum_range = Reader.ReadInteger("Options", "spawn_point_minimum_range", 1000);
	spawn_point_maximum_range = Reader.ReadInteger("Options", "spawn_point_maximum_range", 3000);
	mission_minimum_range_for_timeout = Reader.ReadInteger("Options", "mission_minimum_range_for_timeout", 333);
	// DEBUG
	logging_level = LogLevel (std::max(Reader.ReadInteger("Debug", "logging_level", 1), 1)); // right now at least, I don't want to let anyone turn logging entirely off.
	seconds_to_wait_for_vehicle_persistence_scripts = Reader.ReadInteger("Debug", "seconds_to_wait_for_vehicle_persistence_scripts", 0);
	number_of_tries_to_select_items = Reader.ReadInteger("Debug", "number_of_tries_to_select_items", 100);
	vehicle_search_range_minimum = Reader.ReadInteger("Options", "vehicle_search_range_minimum", 30);
	maximum_number_of_spawn_points = Reader.ReadInteger("Options", "maximum_number_of_spawn_points", 20000);
	maximum_number_of_vehicle_models = Reader.ReadInteger("Options", "maximum_number_of_vehicle_models", 1000);
	return;
}

void main() {
	Logger.Write("main()", LogNormal);
	CreateNotification("~r~Online Events Redux!~w~ v0.2", play_notification_beeps);
	MissionHandler Handler;

	while (true) {
		WaitDuringDeathArrestOrLoading(1);
		Update();
		Handler.Update();
		WAIT(0);
	}

	Logger.Close(); // I don't think this will ever happen...
}

void init() {
	Logger.Write("init()", LogNormal);
	GetSettingsFromIniFile();
	Logger.SetLoggingLevel(logging_level);
	WaitDuringDeathArrestOrLoading(1); // TODO: decide how many additional seconds we must arbitrarily wait.
	UglyHackForVehiclePersistenceScripts(seconds_to_wait_for_vehicle_persistence_scripts); // UGLY HACK FOR VEHICLE PERSISTENCE!
}

void ScriptMain() {
	srand(GetTickCount());
	init();
	main();
}
