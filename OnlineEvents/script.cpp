/*
	THIS FILE IS A PART OF GTA V SCRIPT HOOK SDK
				http://dev-c.com			
			(C) Alexander Blade 2015
*/

#include "script.h"
#include <vector>
#include "IniWriter.h"
#include "IniReader.h"
#include "keyboard.h"
#include <string>
#include <ctime>
#include "iostream"
#include <algorithm>

CIniReader reader(".\\OnlineEvents.ini");

typedef struct
{
	float x_;
	DWORD _paddingx;
	float y_;
	DWORD _paddingy;
	float z_;
	DWORD _paddingz;
	float h_;
	DWORD _paddingw;
} Vector4;
// a change.

#pragma warning(disable : 4244 4305) // double <-> float conversions

int minute, base_minute, type_of_event;
int missionTime = 0;
int waitTime = 0;
bool makeEvent = false;
bool gotMin = false;
bool eventMade = false;
bool eventOver = false;
bool specialCrate = false;
int beep, missionEndTime, waitEndTime, blipStyle;
Ped playerPed;
Player player;

bool DoesEntityExistAndIsNotNull(Entity entity) {
	return (entity != NULL && ENTITY::DOES_ENTITY_EXIST(entity));
}

Hash GetHashOfVehicleModel(Vehicle vehicle) {
	return ENTITY::GET_ENTITY_MODEL(vehicle);
}

bool IsVehicleDrivable(Vehicle vehicle) {
	Hash hash_of_vehicle_model
		= GetHashOfVehicleModel(vehicle);
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
			))
	{
		return true;
	}
	return false;
}

bool IsVehicleProbablyNotCurrentlyBeingUsed(Vehicle vehicle) {
	if ((VEHICLE::IS_VEHICLE_STOPPED(vehicle)) &&
		(VEHICLE::GET_PED_IN_VEHICLE_SEAT(vehicle, -1) == 0) && // IS_VEHICLE_SEAT_FREE doesn't fucking work.
		(VEHICLE::GET_VEHICLE_NUMBER_OF_PASSENGERS(vehicle) == 0))
	{
		return true;
	}
	return false;
}

void NotifyAboveMap(char* message) {
	UI::_SET_NOTIFICATION_TEXT_ENTRY("STRING");
	UI::ADD_TEXT_COMPONENT_SUBSTRING_PLAYER_NAME(message);
	UI::_DRAW_NOTIFICATION(0, 1);
}

void NotifyBottomCenter(char* message) {
	UI::BEGIN_TEXT_COMMAND_PRINT("STRING");
	UI::ADD_TEXT_COMPONENT_SUBSTRING_PLAYER_NAME(message);
	UI::END_TEXT_COMMAND_PRINT(2000, 1);
}

void PlayNotificationBeep() {
	AUDIO::PLAY_SOUND_FRONTEND(-1, "Text_Arrive_Tone", "Phone_SoundSet_Default", 0);
}

int model;
int val;

void money_math()
{
	if (PED::IS_PED_MODEL(playerPed, GAMEPLAY::GET_HASH_KEY("player_zero")))
	{
		model = 0;
	}
	if (PED::IS_PED_MODEL(playerPed, GAMEPLAY::GET_HASH_KEY("player_one")))
	{
		model = 1;
	}
	if (PED::IS_PED_MODEL(playerPed, GAMEPLAY::GET_HASH_KEY("player_two")))
	{
		model = 2;
	}

	char statNameFull[32];
	sprintf_s(statNameFull, "SP%d_TOTAL_CASH", model);
	Hash hash = GAMEPLAY::GET_HASH_KEY(statNameFull);
	STATS::STAT_GET_INT(hash, &val, -1);

	if (type_of_event == 1)
		val += 20000;
	if (type_of_event == 2)
		val += 10000;
	if (type_of_event == 3)
	{
		int random = rand() % 5 + 1;

		if (specialCrate)
		{
			if (random == 1)
			{
				val += 50000;
				WEAPON::GIVE_DELAYED_WEAPON_TO_PED(playerPed, GAMEPLAY::GET_HASH_KEY("WEAPON_RPG"), 1000, 1);
				NotifyAboveMap("Crate collected.\nContents:\n- ~g~$10000~w~\n- RPG");
				if (beep == 1)
					PlayNotificationBeep();
			}
			if (random == 2)
			{
				val += 75000;
				WEAPON::GIVE_DELAYED_WEAPON_TO_PED(playerPed, GAMEPLAY::GET_HASH_KEY("WEAPON_MINIGUN"), 1000, 1);
				NotifyAboveMap("Crate collected.\nContents:\n- ~g~$15000~w~\n- Minigun");
				if (beep == 1)
					PlayNotificationBeep();
			}
			if (random == 3)
			{
				val += 25000;
				WEAPON::GIVE_DELAYED_WEAPON_TO_PED(playerPed, GAMEPLAY::GET_HASH_KEY("WEAPON_GRENADELAUNCHER"), 1000, 1);
				NotifyAboveMap("Crate collected.\nContents:\n- ~g~$10000~w~\n- Grenade Launcher");
				if (beep == 1)
					PlayNotificationBeep();
			}
			if (random == 4)
			{
				val += 15000;
				WEAPON::GIVE_DELAYED_WEAPON_TO_PED(playerPed, GAMEPLAY::GET_HASH_KEY("WEAPON_STICKYBOMB"), 1000, 1);
				NotifyAboveMap("Crate collected.\nContents:\n- ~g~$15000~w~\n- Sticky Bombs");
				if (beep == 1)
					PlayNotificationBeep();
			}
			if (random == 5)
			{
				val += 20000;
				WEAPON::GIVE_DELAYED_WEAPON_TO_PED(playerPed, GAMEPLAY::GET_HASH_KEY("WEAPON_MOLOTOV"), 1000, 1);
				NotifyAboveMap("Crate collected.\nContents:\n- ~g~$20000~w~\n- Molotovs");
				if (beep == 1)
					PlayNotificationBeep();
			}
		}
		else
		{
			if (random == 1)
			{
				val += 10000;
				WEAPON::GIVE_DELAYED_WEAPON_TO_PED(playerPed, GAMEPLAY::GET_HASH_KEY("WEAPON_HEAVYSNIPER"), 1000, 1);
				NotifyAboveMap("Crate collected.\nContents:\n- ~g~$1000~w~\n- Heavy Sniper");
				if (beep == 1)
					PlayNotificationBeep();
			}
			if (random == 2)
			{
				val += 15000;
				WEAPON::GIVE_DELAYED_WEAPON_TO_PED(playerPed, GAMEPLAY::GET_HASH_KEY("WEAPON_ASSAULTRIFLE"), 1000, 1);
				NotifyAboveMap("Crate collected.\nContents:\n- ~g~$1500~w~\n- Assault Rifle");
				if (beep == 1)
					PlayNotificationBeep();
			}
			if (random == 3)
			{
				val += 20000;
				WEAPON::GIVE_DELAYED_WEAPON_TO_PED(playerPed, GAMEPLAY::GET_HASH_KEY("WEAPON_COMBATMG"), 1000, 1);
				NotifyAboveMap("Crate collected.\nContents:\n- ~g~$2000~w~\n- Combat MG");
				if (beep == 1)
					PlayNotificationBeep();
			}
			if (random == 4)
			{
				val += 25000;
				WEAPON::GIVE_DELAYED_WEAPON_TO_PED(playerPed, GAMEPLAY::GET_HASH_KEY("WEAPON_PUMPSHOTGUN"), 1000, 1);
				NotifyAboveMap("Crate collected.\nContents:\n- ~g~$2500~w~\n- Pump Shotgun");
				if (beep == 1)
					PlayNotificationBeep();
			}
			if (random == 5)
			{
				val += 30000;
				WEAPON::GIVE_DELAYED_WEAPON_TO_PED(playerPed, GAMEPLAY::GET_HASH_KEY("WEAPON_APPISTOL"), 1000, 1);
				NotifyAboveMap("Crate collected.\nContents:\n- ~g~$3000~w~\n- AP Pistol");
				if (beep == 1)
					PlayNotificationBeep();
			}
		}
	}
	if (type_of_event == 4)
		val += 15000;

	STATS::STAT_SET_INT(hash, val, 1);
}

int spawn_points_vector_maximum_size = 1000; // I'm hoping that 4000 floats is not considered large for find_if with a custom comparator...
int vehicle_models_max_size = 1000;
//int vehicle_search_range_maximum = 500; // I don't know how far out the engine places cars to begin with. This could probably be smaller.
int vehicle_search_range_minimum = 50; // I just don't want to grab cars that the player just exited. Although, it's probably smarter to check that the player was not the last driver...
std::vector<Vector4> possible_vehicle_spawn_points_parked;
std::vector<Hash> possible_vehicle_models;

void GatherCollectionMissionSpawnPoints()
{
	// looks for an unused vehicle, saves the position and models to vectors.
	NotifyAboveMap("Starting collectCollectionMissionSpawnPoints()...");
	const int ARR_SIZE = 1024;
	Vehicle all_world_vehicles[ARR_SIZE];
	int count = worldGetAllVehicles(all_world_vehicles, ARR_SIZE);
	Vector3 player_coordinates = ENTITY::GET_ENTITY_COORDS(playerPed, 0);
		
	if (all_world_vehicles != NULL)
	{
		for (int i = 0; i < count; i++)
		{
			if (DoesEntityExistAndIsNotNull(all_world_vehicles[i]))
			{
				Vehicle this_vehicle = all_world_vehicles[i];
				Vector3 this_vehicle_coordinates = ENTITY::GET_ENTITY_COORDS(this_vehicle, 0);
				Vector4 this_vehicle_position;
				this_vehicle_position.x_ = this_vehicle_coordinates.x;
				this_vehicle_position.y_ = this_vehicle_coordinates.y;
				this_vehicle_position.z_ = this_vehicle_coordinates.z;
				this_vehicle_position.h_ = ENTITY::GET_ENTITY_HEADING(this_vehicle);
				
				if (DoesEntityExistAndIsNotNull(this_vehicle) &&
					IsVehicleDrivable(this_vehicle) && // is the vehicle a car/bike/etc and can the player start driving it?
					IsVehicleProbablyNotCurrentlyBeingUsed(this_vehicle) && // not moving, no driver?
					!(VEHICLE::GET_LAST_PED_IN_VEHICLE_SEAT(this_vehicle, -1)) && // probably not previously used by the player? We can hope?
					!(VEHICLE::_IS_VEHICLE_DAMAGED(this_vehicle)) && // probably not an empty car in the street as a result of a pileup...
					(VEHICLE::_IS_VEHICLE_SHOP_RESPRAY_ALLOWED(this_vehicle)) && // hopefully this actually works so airport luggage trains don't get tagged...
					GAMEPLAY::GET_DISTANCE_BETWEEN_COORDS(player_coordinates.x, player_coordinates.y, player_coordinates.z, this_vehicle_coordinates.x, this_vehicle_coordinates.y, this_vehicle_coordinates.z, 0) > vehicle_search_range_minimum
					)
				{
					{
						// make sure thisVehPosition does not already exist in possCollectMsnSpawnPts
						// didn't want to define a lambda inline, it gets ugly fast.
						auto predicate = [this_vehicle_position](const Vector4 & item) {
							return (item.x_ == this_vehicle_position.x_ && item.y_ == this_vehicle_position.y_ && item.z_ == this_vehicle_position.z_ && item.h_ == this_vehicle_position.h_);
						};
						bool found = (std::find_if(possible_vehicle_spawn_points_parked.begin(), possible_vehicle_spawn_points_parked.end(), predicate) != possible_vehicle_spawn_points_parked.end());
						if (!found)
							possible_vehicle_spawn_points_parked.push_back(this_vehicle_position);
						// now make sure thisVehPosition is not larger than SpawnPointsVectorMaxSize
						// if it is, remove the first element - first in, first out
						if (possible_vehicle_spawn_points_parked.size() > spawn_points_vector_maximum_size)
							possible_vehicle_spawn_points_parked.erase(possible_vehicle_spawn_points_parked.begin());

						std::string string_spawn_points_found = std::to_string(possible_vehicle_spawn_points_parked.size()); // lolol so much work to get some ints and chars output on screen.
						std::string concatenated_text = "Spawn Points: " + string_spawn_points_found;
						NotifyAboveMap(&concatenated_text[0u]); // combined into one line I hope it works...
						// char *cstr = &concatenatedText[0u];
						// NotifyAboveMap(cstr);
						// also add the vehicle!
					}
					{
						Hash this_vehicle_model = GetHashOfVehicleModel(this_vehicle);
						auto predicate = [this_vehicle_model](const Hash & item) { // pretty sure this isn't necessary for hashes.
							return (item == this_vehicle_model);
						};
						bool found = (std::find_if(possible_vehicle_models.begin(), possible_vehicle_models.end(), predicate) != possible_vehicle_models.end());
						if (!found)
							possible_vehicle_models.push_back(this_vehicle_model);
						// also pretty sure there's not more than a thousand models in the game, but safety first...
						if (possible_vehicle_models.size() > vehicle_models_max_size)
							possible_vehicle_models.erase(possible_vehicle_models.begin());

						std::string string_vehicle_models_found = std::to_string(possible_vehicle_models.size()); // lolol so much work to get some ints and chars output on screen.
						std::string concatenated_text = "Vehicle Models: " + string_vehicle_models_found;
						NotifyAboveMap(&concatenated_text[0u]); // combined into one line I hope it works...
					}
				}
			}
		}
	}

		//Vector3 playerCoords = ENTITY::GET_ENTITY_COORDS(playerPed, 0);
		//Vehicle thisVeh = VEHICLE::GET_RANDOM_VEHICLE_IN_SPHERE(playerCoords.x, playerCoords.y, playerCoords.z, vehicleSearchRange, 0, 0);
		//Vector3 thisVehCoords = ENTITY::GET_ENTITY_COORDS(thisVeh, 0);
		//Vector4 thisVehPosition;
		//thisVehPosition.x = thisVehCoords.x;
		//thisVehPosition.y = thisVehCoords.y;
		//thisVehPosition.z = thisVehCoords.z;
		//thisVehPosition.w = ENTITY::GET_ENTITY_HEADING(thisVeh);
		//
		//if (isVehicleDrivable(thisVeh) && // is the vehicle a car/bike/etc and can the player start driving it?
		//	isVehicleProbablyNotCurrentlyBeingUsed(thisVeh) && // not moving, no driver?
		//	!(VEHICLE::GET_LAST_PED_IN_VEHICLE_SEAT(thisVeh, -1)) && // probably not previously used by the player? We can hope?
		//	!(VEHICLE::_IS_VEHICLE_DAMAGED(thisVeh)) && // probably not an empty car in the street as a result of a pileup...
		//	(VEHICLE::_IS_VEHICLE_SHOP_RESPRAY_ALLOWED(thisVeh)) && // hopefully this actually works so airport luggage trains don't get tagged...
		//	GAMEPLAY::GET_DISTANCE_BETWEEN_COORDS(playerCoords.x, playerCoords.y, playerCoords.z, thisVehCoords.x, thisVehCoords.y, thisVehCoords.z, 0) > vehicleMinSuitableRange) // maybe a script spawns a car near the player? don't want to tag that.
		//{
		//	// make sure thisVehPosition does not already exist in possCollectMsnSpawnPts
		//	// didn't want to define a lambda inline, it gets ugly fast.
		//	auto predicate = [thisVehPosition](const Vector4 & item) {
		//		return (item.x == thisVehPosition.x && item.y == thisVehPosition.y && item.z == thisVehPosition.z && item.w == thisVehPosition.w);
		//	};
		//	bool found = (std::find_if(possCollectMsnSpawnPts.begin(), possCollectMsnSpawnPts.end(), predicate) != possCollectMsnSpawnPts.end());
		//	if (!found)
		//		possCollectMsnSpawnPts.push_back(thisVehPosition);
		//	// now make sure thisVehPosition is not larger than SpawnPointsVectorMaxSize
		//	// if it is, remove the first element - first in, first out
		//	if (possCollectMsnSpawnPts.size() > 1000)
		//		possCollectMsnSpawnPts.erase(possCollectMsnSpawnPts.begin());
		//	// lolol so much work to get some ints and chars output on screen.
		//	std::string stringSpawnPointsFound = std::to_string(possCollectMsnSpawnPts.size());
		//	std::string concatenatedText = "Spawn Points: " + stringSpawnPointsFound;
		//	// combined into one line I hope it works...
		//	NotifyAboveMap(&concatenatedText[0u]);
		//	// char *cstr = &concatenatedText[0u];
		//	// NotifyAboveMap(cstr);
		//
		//	break;
		//}
		//WAIT(0);
}

Vehicle collectible_vehicle;
Blip vehicle_blip;
int vehicle_spawn_point_minimum_range = 500;
int vehicle_spawn_point_maximum_range = 1000;


void CreateCollectibleVehicleMission()
{
	NotifyAboveMap("Starting vehicle_collection_spawn...");

	Vector3 player_coordinates = ENTITY::GET_ENTITY_COORDS(playerPed, 0);
	Vector4 vehicle_spawn_point_to_use;
	while (true)
	{
		vehicle_spawn_point_to_use = possible_vehicle_spawn_points_parked[(rand() % possible_vehicle_spawn_points_parked.size())]; // pick a random possible point from our set
		if (vehicle_spawn_point_minimum_range < GAMEPLAY::GET_DISTANCE_BETWEEN_COORDS(player_coordinates.x, player_coordinates.y, player_coordinates.z, vehicle_spawn_point_to_use.x_, vehicle_spawn_point_to_use.y_, vehicle_spawn_point_to_use.z_, 0) < vehicle_spawn_point_maximum_range)
		{
			break;
		}
		WAIT(0);
	}
	Hash vehicle_model_to_use = possible_vehicle_models[(rand() % possible_vehicle_models.size())]; // pick a random model from our set, pray that it's still loaded in memory?
	
	// FINALLY create the vehicle
	STREAMING::REQUEST_MODEL(vehicle_model_to_use);
	while (!STREAMING::HAS_MODEL_LOADED(vehicle_model_to_use))
		WAIT(0);
	collectible_vehicle = VEHICLE::CREATE_VEHICLE(vehicle_model_to_use, vehicle_spawn_point_to_use.x_, vehicle_spawn_point_to_use.y_, vehicle_spawn_point_to_use.z_, vehicle_spawn_point_to_use.h_, 0, 0);
	VEHICLE::SET_VEHICLE_ON_GROUND_PROPERLY(collectible_vehicle);
	vehicle_blip = UI::ADD_BLIP_FOR_ENTITY(collectible_vehicle);
	if (blipStyle == 0)
	{
		if (VEHICLE::IS_THIS_MODEL_A_CAR(GetHashOfVehicleModel(collectible_vehicle)))
			UI::SET_BLIP_SPRITE(vehicle_blip, 225);
		else
			UI::SET_BLIP_SPRITE(vehicle_blip, 226);
	}
	else
	{
		UI::SET_BLIP_SPRITE(vehicle_blip, 1);
	}
	UI::SET_BLIP_COLOUR(vehicle_blip, 5);
	UI::SET_BLIP_DISPLAY(vehicle_blip, (char)"you will never see this");
	eventMade = true;
	type_of_event = 1;
	NotifyAboveMap("A ~y~special vehicle~w~ has been requested for pickup.");
	if (beep == 1)
		PlayNotificationBeep();
}

bool wantedSet = false;
bool gotColor = false;
bool resprayed = false;
bool blipMade = false;
int red1, green1, blue1;
int red2, green2, blue2;
int Sred1, Sgreen1, Sblue1;
int Sred2, Sgreen2, Sblue2;
int primary1, secondary1;
int primary2, secondary2;
int wanted;
Blip dropOff;

void vehicle_collection_mission()
{
	if (eventMade && type_of_event == 1)
	{
		Vector3 position = ENTITY::GET_ENTITY_COORDS(playerPed, 0);
		Vector3 vehiclePos = ENTITY::GET_ENTITY_COORDS(collectible_vehicle, 0);
		float dropOffDistance = GAMEPLAY::GET_DISTANCE_BETWEEN_COORDS(vehiclePos.x, vehiclePos.y, vehiclePos.z, 1226.06, -3231.36, 6.02, 0);
		int vehicleDistance = GAMEPLAY::GET_DISTANCE_BETWEEN_COORDS(vehiclePos.x, vehiclePos.y, vehiclePos.z, position.x, position.y, position.z, 0);

		if (PED::IS_PED_IN_VEHICLE(playerPed, collectible_vehicle, 0) && !wantedSet)
		{
			wanted = PLAYER::GET_PLAYER_WANTED_LEVEL(player);
			if (wanted < 2)
			{
				PLAYER::SET_PLAYER_WANTED_LEVEL(player, 2, 0);
				PLAYER::SET_PLAYER_WANTED_LEVEL_NOW(player, 1);
			}
			wantedSet = true;
			NotifyAboveMap("Respray the vehicle before turning it in.");
			if (beep == 1)
				PlayNotificationBeep();
		}

		if (!gotColor)
		{
			VEHICLE::GET_VEHICLE_COLOURS(collectible_vehicle, &primary1, &secondary1);
			gotColor = true;
		}

		VEHICLE::GET_VEHICLE_COLOURS(collectible_vehicle, &primary2, &secondary2);
		VEHICLE::GET_VEHICLE_CUSTOM_PRIMARY_COLOUR(collectible_vehicle, &red2, &green2, &blue2);
		//if (VEHICLE::_DOES_VEHICLE_HAVE_SECONDARY_COLOUR(collectVehicle))
			VEHICLE::GET_VEHICLE_CUSTOM_SECONDARY_COLOUR(collectible_vehicle, &Sred2, &Sgreen2, &Sblue2);

		if (primary1 != primary2 && ENTITY::GET_ENTITY_SPEED(collectible_vehicle) != 0 && !resprayed ||
			secondary1 != secondary2  && ENTITY::GET_ENTITY_SPEED(collectible_vehicle) != 0 && !resprayed)
		{
			NotifyAboveMap("The vehicle is ready to be turned in to the ~y~garage~w~ at the docks.");
			if (beep == 1)
				PlayNotificationBeep();
			dropOff = UI::ADD_BLIP_FOR_COORD(1226.06f, -3231.36f, 6.02f);
			if (blipStyle == 0)
				UI::SET_BLIP_SPRITE(dropOff, 50);
			else
				UI::SET_BLIP_SPRITE(dropOff, 1);
			UI::SET_BLIP_COLOUR(dropOff, 5);
			UI::SET_BLIP_DISPLAY(dropOff, (char)"you will never see this");
			resprayed = true;
		}

		if (resprayed && eventMade)
		{
			GRAPHICS::DRAW_MARKER(1, 1226.06, -3231.36, 4.9, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 5.0f, 5.0f, 1.0f, 204, 204, 0, 100, false, false, 2, false, false, false, false);
		}

		if (dropOffDistance < 0.7f && PED::IS_PED_IN_VEHICLE(playerPed, collectible_vehicle, 0) && resprayed && eventMade)
		{
			VEHICLE::SET_VEHICLE_FORWARD_SPEED(collectible_vehicle, 0.0f);
			while (ENTITY::GET_ENTITY_SPEED(collectible_vehicle) != 0)
				WAIT(0);
			AI::TASK_LEAVE_VEHICLE(playerPed, collectible_vehicle, 0);
			VEHICLE::SET_VEHICLE_UNDRIVEABLE(collectible_vehicle, 1);
			NotifyAboveMap("Vehicle collected.\nReward: ~g~$2000~w~");
			if (beep == 1)
				PlayNotificationBeep();
			money_math();
			UI::REMOVE_BLIP(&dropOff);
			UI::REMOVE_BLIP(&vehicle_blip);
			ENTITY::SET_VEHICLE_AS_NO_LONGER_NEEDED(&collectible_vehicle);
			eventMade = false;
			wantedSet = false;
			gotColor = false;
			resprayed = false;
			blipMade = false;
			eventOver = false;
			missionTime = 0;
		}

		if (!VEHICLE::IS_VEHICLE_DRIVEABLE(collectible_vehicle, 1) && eventMade)
		{
			NotifyAboveMap("The ~y~special vehicle~w~ has been destroyed.");
			if (beep == 1)
				PlayNotificationBeep();

			if (resprayed)
				UI::REMOVE_BLIP(&dropOff);

			UI::REMOVE_BLIP(&vehicle_blip);
			ENTITY::SET_VEHICLE_AS_NO_LONGER_NEEDED(&collectible_vehicle);
			eventMade = false;
			wantedSet = false;
			gotColor = false;
			resprayed = false;
			blipMade = false;
			eventOver = false;
			missionTime = 0;
		}

		if (eventOver && vehicleDistance > 1000 || eventMade && ENTITY::IS_ENTITY_DEAD(playerPed))
		{
			NotifyAboveMap("The ~y~special vehicle~w~ is no longer requested.");
			if (beep == 1)
				PlayNotificationBeep();

			if (resprayed)
				UI::REMOVE_BLIP(&dropOff);

			UI::REMOVE_BLIP(&vehicle_blip);
			ENTITY::SET_VEHICLE_AS_NO_LONGER_NEEDED(&collectible_vehicle);
			eventMade = false;
			wantedSet = false;
			gotColor = false;
			resprayed = false;
			blipMade = false;
			eventOver = false;
			missionTime = 0;
		}
	}
}

Vehicle smugglerVehicle;
Blip smugglerBlip;

void arms_smuggler_spawn()
{
	Vector3 spawn;
	DWORD vehicle;
	float heading;

	while (true)
	{
		int random = rand() % 10 + 1;

		if (random == 1)
		{
			vehicle = GAMEPLAY::GET_HASH_KEY("rebel");
			spawn.x = 632.31f;
			spawn.y = 2807.72f;
			spawn.z = 41.59f;
			heading = 274.60f;
		}
		if (random == 2)
		{
			vehicle = GAMEPLAY::GET_HASH_KEY("rancherxl");
			spawn.x = -2088.22f;
			spawn.y = 2645.72f;
			spawn.z = 2.49f;
			heading = 49.00f;
		}
		if (random == 3)
		{
			vehicle = GAMEPLAY::GET_HASH_KEY("dloader");
			spawn.x = 1520.75f;
			spawn.y = 6504.73f;
			spawn.z = 21.67f;
			heading = 68.38f;
		}
		if (random == 4)
		{
			vehicle = GAMEPLAY::GET_HASH_KEY("voodoo2");
			spawn.x = 2008.36f;
			spawn.y = 4968.20f;
			spawn.z = 41.13f;
			heading = 276.87f;
		}
		if (random == 5)
		{
			vehicle = GAMEPLAY::GET_HASH_KEY("bfinjection");
			spawn.x = -393.68f;
			spawn.y = 6375.97f;
			spawn.z = 13.47f;
			heading = 23.84f;
		}
		if (random == 6)
		{
			vehicle = GAMEPLAY::GET_HASH_KEY("sanchez2");
			spawn.x = 335.60f;
			spawn.y = 4468.75f;
			spawn.z = 62.75f;
			heading = 13.05f;
		}
		if (random == 7)
		{
			vehicle = GAMEPLAY::GET_HASH_KEY("hexer");
			spawn.x = -194.96f;
			spawn.y = 3652.16f;
			spawn.z = 51.21f;
			heading = 244.75f;
		}
		if (random == 8)
		{
			vehicle = GAMEPLAY::GET_HASH_KEY("daemon");
			spawn.x = 2657.63f;
			spawn.y = 3509.00f;
			spawn.z = 52.86f;
			heading = 340.33f;
		}
		if (random == 9)
		{
			vehicle = GAMEPLAY::GET_HASH_KEY("journey");
			spawn.x = 2185.36f;
			spawn.y = 5558.47f;
			spawn.z = 53.15f;
			heading = 215.16f;
		}
		if (random == 10)
		{
			vehicle = GAMEPLAY::GET_HASH_KEY("sadler");
			spawn.x = 181.71f;
			spawn.y = 2228.65f;
			spawn.z = 89.55f;
			heading = 233.02f;
		}

		Vector3 position = ENTITY::GET_ENTITY_COORDS(playerPed, 0);
		int spawnDistance = GAMEPLAY::GET_DISTANCE_BETWEEN_COORDS(position.x, position.y, position.z, spawn.x, spawn.y, spawn.z, 0);

		if (spawnDistance > 1000)
			break;
	}

	STREAMING::REQUEST_MODEL(vehicle);
	while (!STREAMING::HAS_MODEL_LOADED(vehicle))
		WAIT(0);
	smugglerVehicle = VEHICLE::CREATE_VEHICLE(vehicle, spawn.x, spawn.y, spawn.z, heading, 0, 0);
	VEHICLE::SET_VEHICLE_ON_GROUND_PROPERLY(smugglerVehicle);
	smugglerBlip = UI::ADD_BLIP_FOR_ENTITY(smugglerVehicle);
	if (blipStyle == 0)
	{
		if (VEHICLE::IS_THIS_MODEL_A_CAR(vehicle))
			UI::SET_BLIP_SPRITE(smugglerBlip, 225);
		else
			UI::SET_BLIP_SPRITE(smugglerBlip, 226);
	}
	else
	{
		UI::SET_BLIP_SPRITE(smugglerBlip, 1);
	}
	UI::SET_BLIP_COLOUR(smugglerBlip, 1);
	UI::SET_BLIP_DISPLAY(smugglerBlip, (char)"you will never see this");
	eventMade = true;
	type_of_event = 2;
	NotifyAboveMap("A ~r~smuggler vehicle~w~ has been spotted.  Destroy it for a reward.");
	if (beep == 1)
		PlayNotificationBeep();
}

void arms_smuggler_mission()
{
	if (eventMade && type_of_event == 2)
	{
		Vector3 position = ENTITY::GET_ENTITY_COORDS(playerPed, 0);
		Vector3 vehiclePos = ENTITY::GET_ENTITY_COORDS(smugglerVehicle, 0);
		int vehicleDistance = GAMEPLAY::GET_DISTANCE_BETWEEN_COORDS(vehiclePos.x, vehiclePos.y, vehiclePos.z, position.x, position.y, position.z, 0);

		if (!VEHICLE::IS_VEHICLE_DRIVEABLE(smugglerVehicle, 1) && eventMade)
		{
			money_math();
			UI::REMOVE_BLIP(&smugglerBlip);
			ENTITY::SET_VEHICLE_AS_NO_LONGER_NEEDED(&smugglerVehicle);
			NotifyAboveMap("Vehicle destroyed.\nReward: ~g~$1000~w~");
			if (beep == 1)
				PlayNotificationBeep();
			eventMade = false;
			eventOver = false;
			missionTime = 0;
		}

		if (eventOver && vehicleDistance > 1000 || eventMade && ENTITY::IS_ENTITY_DEAD(playerPed))
		{
			NotifyAboveMap("The ~r~smuggler vehicle~w~ has been claimed by smugglers.");
			if (beep == 1)
				PlayNotificationBeep();
			UI::REMOVE_BLIP(&smugglerBlip);
			ENTITY::SET_VEHICLE_AS_NO_LONGER_NEEDED(&smugglerVehicle);
			eventMade = false;
			eventOver = false;
			missionTime = 0;
		}
	}
}

Blip crateBlip;
Vector3 crateSpawn;

void crate_drop_spawn()
{
	while (true)
	{
		int random = rand() % 10 + 1;

		if (random == 1)
		{
			crateSpawn.x = -1782.33f;
			crateSpawn.y = -826.29f;
			crateSpawn.z = 7.83f;
		}
		if (random == 2)
		{
			crateSpawn.x = 1067.85f;
			crateSpawn.y = 2362.45f;
			crateSpawn.z = 43.87f;
		}
		if (random == 3)
		{
			crateSpawn.x = -3.77f;
			crateSpawn.y = 6840.14f;
			crateSpawn.z = 13.46f;
		}
		if (random == 4)
		{
			crateSpawn.x = 2061.37f;
			crateSpawn.y = 2798.68f;
			crateSpawn.z = 50.29f;
		}
		if (random == 5)
		{
			crateSpawn.x = 1883.24f;
			crateSpawn.y = 278.48f;
			crateSpawn.z = 162.73f;
		}
		if (random == 6)
		{
			crateSpawn.x = -2912.36f;
			crateSpawn.y = 3077.48f;
			crateSpawn.z = 3.39f;
		}
		if (random == 7)
		{
			crateSpawn.x = 2822.70f;
			crateSpawn.y = -634.39f;
			crateSpawn.z = 2.15f;
		}
		if (random == 8)
		{
			crateSpawn.x = 2184.12f;
			crateSpawn.y = 5026.09f;
			crateSpawn.z = 42.63f;
		}
		if (random == 9)
		{
			crateSpawn.x = -3147.02f;
			crateSpawn.y = 305.27f;
			crateSpawn.z = 2.35f;
		}
		if (random == 10)
		{
			crateSpawn.x = 1552.40f;
			crateSpawn.y = 6644.04f;
			crateSpawn.z = 2.55f;
		}

		Vector3 position = ENTITY::GET_ENTITY_COORDS(playerPed, 0);
		int spawnDistance = GAMEPLAY::GET_DISTANCE_BETWEEN_COORDS(position.x, position.y, position.z, crateSpawn.x, crateSpawn.y, crateSpawn.z, 0);

		if (spawnDistance > 1000)
			break;
	}

	int random = rand() % 25 + 1;

	if (random == 20)
		specialCrate = true;
	crateBlip = UI::ADD_BLIP_FOR_COORD(crateSpawn.x, crateSpawn.y, crateSpawn.z);
	if (blipStyle == 0)
		UI::SET_BLIP_SPRITE(crateBlip, 306);
	else
		UI::SET_BLIP_SPRITE(crateBlip, 1);
	UI::SET_BLIP_SCALE(crateBlip, 1.5);
	if (specialCrate)
		UI::SET_BLIP_COLOUR(crateBlip, 5);
	if (!specialCrate)
		UI::SET_BLIP_COLOUR(crateBlip, 2);
	UI::SET_BLIP_DISPLAY(crateBlip, (char)"you will never see this");
	if (specialCrate)
		NotifyAboveMap("A ~y~special crate~w~ has been dropped.");
	if (!specialCrate)
		NotifyAboveMap("A ~g~crate~w~ has been dropped.");
	if (beep == 1)
		PlayNotificationBeep();
	eventMade = true;
	type_of_event = 3;
}

bool crateMade = false;
bool spawnguards = false;
Object crate;
Ped guard1, guard2, guard3, guard4, guard5, guard6, guard7, guard8, guard9, guard10;

void crate_drop_mission()
{
	if (eventMade && type_of_event == 3)
	{
		Vector3 position = ENTITY::GET_ENTITY_COORDS(playerPed, 0);
		int spawnDistance = GAMEPLAY::GET_DISTANCE_BETWEEN_COORDS(position.x, position.y, position.z, crateSpawn.x, crateSpawn.y, crateSpawn.z, 0);

		if (spawnDistance < 300 && !crateMade)
		{
			STREAMING::REQUEST_MODEL(GAMEPLAY::GET_HASH_KEY("prop_box_ammo04a"));
			while (!STREAMING::HAS_MODEL_LOADED(GAMEPLAY::GET_HASH_KEY("prop_box_ammo04a")))
				WAIT(0);
			crate = OBJECT::CREATE_AMBIENT_PICKUP(0x14568F28, crateSpawn.x, crateSpawn.y, crateSpawn.z, -1, 0, GAMEPLAY::GET_HASH_KEY("prop_box_ammo04a"), 1, 1);
			ENTITY::SET_ENTITY_ALPHA(crate, 255, 1);
			while (ENTITY::GET_ENTITY_HEIGHT(crate, crateSpawn.x, crateSpawn.y, crateSpawn.z, 1, 1) > 0)
				OBJECT::PLACE_OBJECT_ON_GROUND_PROPERLY(crate);
			ENTITY::FREEZE_ENTITY_POSITION(crate, 1);
			UI::REMOVE_BLIP(&crateBlip);
			crateBlip = UI::ADD_BLIP_FOR_ENTITY(crate);
			if (blipStyle == 0)
				UI::SET_BLIP_SPRITE(crateBlip, 306);
			else
				UI::SET_BLIP_SPRITE(crateBlip, 1);
			UI::SET_BLIP_SCALE(crateBlip, 1.5);
			if (specialCrate)
				UI::SET_BLIP_COLOUR(crateBlip, 5);
			if (!specialCrate)
				UI::SET_BLIP_COLOUR(crateBlip, 2);
			UI::SET_BLIP_DISPLAY(crateBlip, (char)"you will never see this");
			crateMade = true;
			spawnguards = true;
		}

		if (crateMade && spawnguards)
		{
			Ped skin = GAMEPLAY::GET_HASH_KEY("mp_g_m_pros_01");
			STREAMING::REQUEST_MODEL(skin);

			if (!ENTITY::DOES_ENTITY_EXIST(guard1))
			{
				guard1 = PED::CREATE_PED(26, skin, crateSpawn.x + 3, crateSpawn.y, crateSpawn.z + 1, 20, false, true);
				PED::SET_PED_RELATIONSHIP_GROUP_HASH(guard1, GAMEPLAY::GET_HASH_KEY("HATES_PLAYER"));
				WEAPON::GIVE_DELAYED_WEAPON_TO_PED(guard1, GAMEPLAY::GET_HASH_KEY("WEAPON_PISTOL"), 1000, 1);
				WEAPON::GIVE_DELAYED_WEAPON_TO_PED(guard1, GAMEPLAY::GET_HASH_KEY("WEAPON_PUMPSHOTGUN"), 1000, 1);
				WEAPON::SET_CURRENT_PED_WEAPON(guard1, GAMEPLAY::GET_HASH_KEY("WEAPON_PUMPSHOTGUN"), 1);
				AI::TASK_TURN_PED_TO_FACE_ENTITY(guard1, crate, 5000);
				ENTITY::SET_ENTITY_INVINCIBLE(guard1, false);
			}
			if (!ENTITY::DOES_ENTITY_EXIST(guard2))
			{
				guard2 = PED::CREATE_PED(26, skin, crateSpawn.x, crateSpawn.y + 3, crateSpawn.z + 1, 40, false, true);
				PED::SET_PED_RELATIONSHIP_GROUP_HASH(guard2, GAMEPLAY::GET_HASH_KEY("HATES_PLAYER"));
				WEAPON::GIVE_DELAYED_WEAPON_TO_PED(guard2, GAMEPLAY::GET_HASH_KEY("WEAPON_PISTOL"), 1000, 1);
				WEAPON::SET_CURRENT_PED_WEAPON(guard2, GAMEPLAY::GET_HASH_KEY("WEAPON_PISTOL"), 1);
				AI::TASK_TURN_PED_TO_FACE_ENTITY(guard2, crate, 5000);
				ENTITY::SET_ENTITY_INVINCIBLE(guard2, false);
			}
			if (!ENTITY::DOES_ENTITY_EXIST(guard3))
			{
				guard3 = PED::CREATE_PED(26, skin, crateSpawn.x + 3, crateSpawn.y + 3, crateSpawn.z + 1, 60, false, true);
				PED::SET_PED_RELATIONSHIP_GROUP_HASH(guard3, GAMEPLAY::GET_HASH_KEY("HATES_PLAYER"));
				WEAPON::GIVE_DELAYED_WEAPON_TO_PED(guard3, GAMEPLAY::GET_HASH_KEY("WEAPON_PISTOL"), 1000, 1);
				WEAPON::GIVE_DELAYED_WEAPON_TO_PED(guard3, GAMEPLAY::GET_HASH_KEY("WEAPON_PUMPSHOTGUN"), 1000, 1);
				WEAPON::SET_CURRENT_PED_WEAPON(guard3, GAMEPLAY::GET_HASH_KEY("WEAPON_PUMPSHOTGUN"), 1);
				AI::TASK_TURN_PED_TO_FACE_ENTITY(guard3, crate, 5000);
				ENTITY::SET_ENTITY_INVINCIBLE(guard3, false);
			}
			if (!ENTITY::DOES_ENTITY_EXIST(guard4))
			{
				guard4 = PED::CREATE_PED(26, skin, crateSpawn.x - 3, crateSpawn.y, crateSpawn.z + 1, 80, false, true);
				PED::SET_PED_RELATIONSHIP_GROUP_HASH(guard4, GAMEPLAY::GET_HASH_KEY("HATES_PLAYER"));
				WEAPON::GIVE_DELAYED_WEAPON_TO_PED(guard4, GAMEPLAY::GET_HASH_KEY("WEAPON_PISTOL"), 1000, 1);
				WEAPON::GIVE_DELAYED_WEAPON_TO_PED(guard4, GAMEPLAY::GET_HASH_KEY("WEAPON_ASSAULTRIFLE"), 1000, 1);
				WEAPON::SET_CURRENT_PED_WEAPON(guard4, GAMEPLAY::GET_HASH_KEY("WEAPON_ASSAULTRIFLE"), 1);
				AI::TASK_TURN_PED_TO_FACE_ENTITY(guard4, crate, 5000);
				ENTITY::SET_ENTITY_INVINCIBLE(guard4, false);
			}
			if (!ENTITY::DOES_ENTITY_EXIST(guard5))
			{
				guard5 = PED::CREATE_PED(26, skin, crateSpawn.x, crateSpawn.y - 3, crateSpawn.z + 1, 100, false, true);
				PED::SET_PED_RELATIONSHIP_GROUP_HASH(guard5, GAMEPLAY::GET_HASH_KEY("HATES_PLAYER"));
				WEAPON::GIVE_DELAYED_WEAPON_TO_PED(guard5, GAMEPLAY::GET_HASH_KEY("WEAPON_PISTOL"), 1000, 1);
				WEAPON::GIVE_DELAYED_WEAPON_TO_PED(guard5, GAMEPLAY::GET_HASH_KEY("WEAPON_PUMPSHOTGUN"), 1000, 1);
				WEAPON::SET_CURRENT_PED_WEAPON(guard5, GAMEPLAY::GET_HASH_KEY("WEAPON_PUMPSHOTGUN"), 1);
				AI::TASK_TURN_PED_TO_FACE_ENTITY(guard5, crate, 5000);
				ENTITY::SET_ENTITY_INVINCIBLE(guard5, false);
			}
			if (!ENTITY::DOES_ENTITY_EXIST(guard6))
			{
				guard6 = PED::CREATE_PED(26, skin, crateSpawn.x - 3, crateSpawn.y - 3, crateSpawn.z + 1, 120, false, true);
				PED::SET_PED_RELATIONSHIP_GROUP_HASH(guard6, GAMEPLAY::GET_HASH_KEY("HATES_PLAYER"));
				WEAPON::GIVE_DELAYED_WEAPON_TO_PED(guard6, GAMEPLAY::GET_HASH_KEY("WEAPON_PISTOL"), 1000, 1);
				WEAPON::GIVE_DELAYED_WEAPON_TO_PED(guard6, GAMEPLAY::GET_HASH_KEY("WEAPON_ASSAULTRIFLE"), 1000, 1);
				WEAPON::SET_CURRENT_PED_WEAPON(guard6, GAMEPLAY::GET_HASH_KEY("WEAPON_ASSAULTRIFLE"), 1);
				AI::TASK_TURN_PED_TO_FACE_ENTITY(guard6, crate, 5000);
				ENTITY::SET_ENTITY_INVINCIBLE(guard6, false);
			}
			if (!ENTITY::DOES_ENTITY_EXIST(guard7))
			{
				guard7 = PED::CREATE_PED(26, skin, crateSpawn.x + 5, crateSpawn.y, crateSpawn.z + 1, 140, false, true);
				PED::SET_PED_RELATIONSHIP_GROUP_HASH(guard7, GAMEPLAY::GET_HASH_KEY("HATES_PLAYER"));
				WEAPON::GIVE_DELAYED_WEAPON_TO_PED(guard7, GAMEPLAY::GET_HASH_KEY("WEAPON_PISTOL"), 1000, 1);
				WEAPON::SET_CURRENT_PED_WEAPON(guard7, GAMEPLAY::GET_HASH_KEY("WEAPON_PISTOL"), 1);
				AI::TASK_TURN_PED_TO_FACE_ENTITY(guard7, crate, 5000);
				ENTITY::SET_ENTITY_INVINCIBLE(guard7, false);
			}
			if (!ENTITY::DOES_ENTITY_EXIST(guard8))
			{
				guard8 = PED::CREATE_PED(26, skin, crateSpawn.x, crateSpawn.y + 5, crateSpawn.z + 1, 160, false, true);
				PED::SET_PED_RELATIONSHIP_GROUP_HASH(guard8, GAMEPLAY::GET_HASH_KEY("HATES_PLAYER"));
				WEAPON::GIVE_DELAYED_WEAPON_TO_PED(guard8, GAMEPLAY::GET_HASH_KEY("WEAPON_PISTOL"), 1000, 1);
				WEAPON::GIVE_DELAYED_WEAPON_TO_PED(guard8, GAMEPLAY::GET_HASH_KEY("WEAPON_ASSAULTRIFLE"), 1000, 1);
				WEAPON::SET_CURRENT_PED_WEAPON(guard8, GAMEPLAY::GET_HASH_KEY("WEAPON_ASSAULTRIFLE"), 1);
				AI::TASK_TURN_PED_TO_FACE_ENTITY(guard8, crate, 5000);
				ENTITY::SET_ENTITY_INVINCIBLE(guard8, false);
			}
			if (!ENTITY::DOES_ENTITY_EXIST(guard9))
			{
				guard9 = PED::CREATE_PED(26, skin, crateSpawn.x - 5, crateSpawn.y, crateSpawn.z + 1, 180, false, true);
				PED::SET_PED_RELATIONSHIP_GROUP_HASH(guard9, GAMEPLAY::GET_HASH_KEY("HATES_PLAYER"));
				WEAPON::GIVE_DELAYED_WEAPON_TO_PED(guard9, GAMEPLAY::GET_HASH_KEY("WEAPON_PISTOL"), 1000, 1);
				WEAPON::GIVE_DELAYED_WEAPON_TO_PED(guard9, GAMEPLAY::GET_HASH_KEY("WEAPON_PUMPSHOTGUN"), 1000, 1);
				WEAPON::SET_CURRENT_PED_WEAPON(guard9, GAMEPLAY::GET_HASH_KEY("WEAPON_PUMPSHOTGUN"), 1);
				AI::TASK_TURN_PED_TO_FACE_ENTITY(guard9, crate, 5000);
				ENTITY::SET_ENTITY_INVINCIBLE(guard9, false);
			}
			if (!ENTITY::DOES_ENTITY_EXIST(guard10))
			{
				guard10 = PED::CREATE_PED(26, skin, crateSpawn.x, crateSpawn.y - 5, crateSpawn.z + 1, 200, false, true);
				PED::SET_PED_RELATIONSHIP_GROUP_HASH(guard10, GAMEPLAY::GET_HASH_KEY("HATES_PLAYER"));
				WEAPON::GIVE_DELAYED_WEAPON_TO_PED(guard10, GAMEPLAY::GET_HASH_KEY("WEAPON_PISTOL"), 1000, 1);
				WEAPON::GIVE_DELAYED_WEAPON_TO_PED(guard10, GAMEPLAY::GET_HASH_KEY("WEAPON_ASSAULTRIFLE"), 0, 1);
				WEAPON::SET_CURRENT_PED_WEAPON(guard10, GAMEPLAY::GET_HASH_KEY("WEAPON_ASSAULTRIFLE"), 1);
				AI::TASK_TURN_PED_TO_FACE_ENTITY(guard10, crate, 5000);
				ENTITY::SET_ENTITY_INVINCIBLE(guard10, false);
			}

			if (ENTITY::DOES_ENTITY_EXIST(guard1) &&
				ENTITY::DOES_ENTITY_EXIST(guard2) &&
				ENTITY::DOES_ENTITY_EXIST(guard3) &&
				ENTITY::DOES_ENTITY_EXIST(guard4) &&
				ENTITY::DOES_ENTITY_EXIST(guard5) &&
				ENTITY::DOES_ENTITY_EXIST(guard6) &&
				ENTITY::DOES_ENTITY_EXIST(guard7) &&
				ENTITY::DOES_ENTITY_EXIST(guard8) &&
				ENTITY::DOES_ENTITY_EXIST(guard9) &&
				ENTITY::DOES_ENTITY_EXIST(guard10))
			{
				Hash flare = GAMEPLAY::GET_HASH_KEY("WEAPON_FLARE");
				if (!WEAPON::HAS_WEAPON_ASSET_LOADED(flare))
				{
					WEAPON::REQUEST_WEAPON_ASSET(flare, 31, 0);
					while (!WEAPON::HAS_WEAPON_ASSET_LOADED(flare))
						WAIT(0);
				}
				GAMEPLAY::SHOOT_SINGLE_BULLET_BETWEEN_COORDS(crateSpawn.x, crateSpawn.y + 1.0f, crateSpawn.z, crateSpawn.x, crateSpawn.y + 1.5, crateSpawn.z, 0, 1, flare, playerPed, 1, 1, 100.0);
				FIRE::ADD_EXPLOSION(crateSpawn.x, crateSpawn.y + 1.0f, crateSpawn.z, 19, 0.0f, 0, 0, 0);
				spawnguards = false;
			}
		}

		if (eventMade && crateMade && !ENTITY::DOES_ENTITY_EXIST(crate))
		{
			money_math();
			ENTITY::SET_PED_AS_NO_LONGER_NEEDED(&guard1);
			ENTITY::SET_PED_AS_NO_LONGER_NEEDED(&guard2);
			ENTITY::SET_PED_AS_NO_LONGER_NEEDED(&guard3);
			ENTITY::SET_PED_AS_NO_LONGER_NEEDED(&guard4);
			ENTITY::SET_PED_AS_NO_LONGER_NEEDED(&guard5);
			ENTITY::SET_PED_AS_NO_LONGER_NEEDED(&guard6);
			ENTITY::SET_PED_AS_NO_LONGER_NEEDED(&guard7);
			ENTITY::SET_PED_AS_NO_LONGER_NEEDED(&guard8);
			ENTITY::SET_PED_AS_NO_LONGER_NEEDED(&guard9);
			ENTITY::SET_PED_AS_NO_LONGER_NEEDED(&guard10);
			specialCrate = false;
			eventMade = false;
			eventOver = false;
			crateMade = false;
			missionTime = 0;
		}

		if (eventOver && spawnDistance > 1000 || eventMade && ENTITY::IS_ENTITY_DEAD(playerPed))
		{
			if (specialCrate)
				NotifyAboveMap("The ~y~special crate~w~ has been claimed by smugglers.");
			if (!specialCrate)
				NotifyAboveMap("The ~g~crate~w~ has been claimed by smugglers.");
			if (beep == 1)
				PlayNotificationBeep();
			UI::REMOVE_BLIP(&crateBlip);
			ENTITY::SET_OBJECT_AS_NO_LONGER_NEEDED(&crate);
			ENTITY::SET_PED_AS_NO_LONGER_NEEDED(&guard1);
			ENTITY::SET_PED_AS_NO_LONGER_NEEDED(&guard2);
			ENTITY::SET_PED_AS_NO_LONGER_NEEDED(&guard3);
			ENTITY::SET_PED_AS_NO_LONGER_NEEDED(&guard4);
			ENTITY::SET_PED_AS_NO_LONGER_NEEDED(&guard5);
			ENTITY::SET_PED_AS_NO_LONGER_NEEDED(&guard6);
			ENTITY::SET_PED_AS_NO_LONGER_NEEDED(&guard7);
			ENTITY::SET_PED_AS_NO_LONGER_NEEDED(&guard8);
			ENTITY::SET_PED_AS_NO_LONGER_NEEDED(&guard9);
			ENTITY::SET_PED_AS_NO_LONGER_NEEDED(&guard10);
			specialCrate = false;
			eventMade = false;
			eventOver = false;
			crateMade = false;
			missionTime = 0;
		}
	}
}

Ped target;
Blip targetBlip;

void assassination_spawn()
{
	Vector3 spawn;
	float heading;

	while (true)
	{
		int random = rand() % 10 + 1;

		if (random == 1)
		{
			spawn.x = 145.13f;
			spawn.y = 189.83f;
			spawn.z = 106.39f;
			heading = 289.36f;
		}
		if (random == 2)
		{
			spawn.x = 499.06f;
			spawn.y = -236.07f;
			spawn.z = 48.71f;
			heading = 209.44f;
		}
		if (random == 3)
		{
			spawn.x = 91.20f;
			spawn.y = -1436.35f;
			spawn.z = 29.30f;
			heading = 146.08f;
		}
		if (random == 4)
		{
			spawn.x = -1177.02f;
			spawn.y = -1404.81f;
			spawn.z = 4.65f;
			heading = 281.10f;
		}
		if (random == 5)
		{
			spawn.x = -1845.61f;
			spawn.y = -1213.09f;
			spawn.z = 13.01f;
			heading = 200.24f;
		}
		if (random == 6)
		{
			spawn.x = -1350.38f;
			spawn.y = -78.39f;
			spawn.z = 50.82f;
			heading = 276.20f;
		}
		if (random == 7)
		{
			spawn.x = -543.53f;
			spawn.y = -42.11f;
			spawn.z = 42.88f;
			heading = 56.28f;
		}
		if (random == 8)
		{
			spawn.x = 215.99f;
			spawn.y = -870.98f;
			spawn.z = 30.49f;
			heading = 300.32f;
		}
		if (random == 9)
		{
			spawn.x = 1066.14f;
			spawn.y = -608.32f;
			spawn.z = 56.83f;
			heading = 160.16f;
		}
		if (random == 10)
		{
			spawn.x = 335.83f;
			spawn.y = -1969.57f;
			spawn.z = 24.36f;
			heading = 61.14f;
		}

		Vector3 position = ENTITY::GET_ENTITY_COORDS(playerPed, 0);
		int spawnDistance = GAMEPLAY::GET_DISTANCE_BETWEEN_COORDS(position.x, position.y, position.z, spawn.x, spawn.y, spawn.z, 0);
		
		if (spawnDistance > 1000)
			break;
	}

	target = PED::CREATE_RANDOM_PED(spawn.x, spawn.y, spawn.z);
	while (!ENTITY::DOES_ENTITY_EXIST(target))
		WAIT(0);
	PED::SET_PED_DESIRED_HEADING(target, heading);
	targetBlip = UI::ADD_BLIP_FOR_ENTITY(target);
	AI::TASK_WANDER_STANDARD(target, 1000.0f, 0);
	if (blipStyle == 0)
		UI::SET_BLIP_SPRITE(targetBlip, 432);
	else
		UI::SET_BLIP_SPRITE(targetBlip, 1);
	UI::SET_BLIP_COLOUR(targetBlip, 1);
	UI::SET_BLIP_DISPLAY(targetBlip, (char)"you will never see this");
	eventMade = true;
	type_of_event = 4;
	NotifyAboveMap("A hit has been placed on a ~r~target~w~.  Kill them for a reward.");
	if (beep == 1)
		PlayNotificationBeep();
}

void assassination_mission()
{
	if (eventMade && type_of_event == 4)
	{
		Vector3 position = ENTITY::GET_ENTITY_COORDS(playerPed, 0);
		Vector3 targetPos = ENTITY::GET_ENTITY_COORDS(target, 0);
		int targetDistance = GAMEPLAY::GET_DISTANCE_BETWEEN_COORDS(targetPos.x, targetPos.y, targetPos.z, position.x, position.y, position.z, 0);

		if (ENTITY::IS_ENTITY_DEAD(target) && eventMade)
		{
			money_math();
			UI::REMOVE_BLIP(&targetBlip);
			ENTITY::SET_PED_AS_NO_LONGER_NEEDED(&target);
			NotifyAboveMap("Target killed.\nReward: ~g~$1500~w~");
			if (beep == 1)
				PlayNotificationBeep();
			eventMade = false;
			eventOver = false;
			missionTime = 0;
		}

		if (eventOver && targetDistance > 1000 || eventMade && ENTITY::IS_ENTITY_DEAD(playerPed))
		{
			NotifyAboveMap("The hit on the ~r~target~w~ has expired.");
			if (beep == 1)
				PlayNotificationBeep();
			UI::REMOVE_BLIP(&targetBlip);
			ENTITY::SET_PED_AS_NO_LONGER_NEEDED(&target);
			eventMade = false;
			eventOver = false;
			missionTime = 0;
		}
	}
}

Vehicle truck;
Blip truckBlip;

void armored_truck_spawn()
{
	Vector3 spawn;
	float heading;

	while (true)
	{
		int random = rand() % 10 + 1;

		if (random == 1)
		{
			spawn.x = 206.62f;
			spawn.y = -40.01f;
			spawn.z = 68.49f;
			heading = 144.72f;
		}
		if (random == 2)
		{
			spawn.x = -273.17f;
			spawn.y = -883.21f;
			spawn.z = 30.82f;
			heading = 339.09f;
		}
		if (random == 3)
		{
			spawn.x = -46.17f;
			spawn.y = -1739.08f;
			spawn.z = 28.82f;
			heading = 51.90f;
		}
		if (random == 4)
		{
			spawn.x = -1887.05f;
			spawn.y = -359.63f;
			spawn.z = 48.79f;
			heading = 139.09f;
		}
		if (random == 5)
		{
			spawn.x = 881.33f;
			spawn.y = 22.07f;
			spawn.z = 78.31f;
			heading = 51.88f;
		}
		if (random == 6)
		{
			spawn.x = 1189.92f;
			spawn.y = -1415.68f;
			spawn.z = 34.73f;
			heading = 182.73f;
		}
		if (random == 7)
		{
			spawn.x = -1065.47f;
			spawn.y = -1391.93f;
			spawn.z = 4.77f;
			heading = 82.02f;
		}
		if (random == 8)
		{
			spawn.x = 365.46f;
			spawn.y = -835.60f;
			spawn.z = 28.89f;
			heading = 180.54f;
		}
		if (random == 9)
		{
			spawn.x = -150.84f;
			spawn.y = 6491.31f;
			spawn.z = 29.29f;
			heading = 44.16f;
		}
		if (random == 10)
		{
			spawn.x = 1317.85f;
			spawn.y = 6506.17f;
			spawn.z = 19.66f;
			heading = 98.89f;
		}

		Vector3 position = ENTITY::GET_ENTITY_COORDS(playerPed, 0);
		int spawnDistance = GAMEPLAY::GET_DISTANCE_BETWEEN_COORDS(position.x, position.y, position.z, spawn.x, spawn.y, spawn.z, 0);

		if (spawnDistance > 1000)
			break;
	}

	STREAMING::REQUEST_MODEL(GAMEPLAY::GET_HASH_KEY("stockade"));
	while (!STREAMING::HAS_MODEL_LOADED(GAMEPLAY::GET_HASH_KEY("stockade")))
		WAIT(0);
	truck = VEHICLE::CREATE_VEHICLE(GAMEPLAY::GET_HASH_KEY("stockade"), spawn.x, spawn.y, spawn.z, heading, 0, 0);
	VEHICLE::SET_VEHICLE_ON_GROUND_PROPERLY(truck);
	truckBlip = UI::ADD_BLIP_FOR_ENTITY(truck);
	if (blipStyle == 0)
		UI::SET_BLIP_SPRITE(truckBlip, 67);
	else
		UI::SET_BLIP_SPRITE(truckBlip, 1);
	UI::SET_BLIP_COLOUR(truckBlip, 3);
	UI::SET_BLIP_DISPLAY(truckBlip, (char)"you will never see this");
	eventMade = true;
	type_of_event = 5;
	NotifyAboveMap("An ~b~armored truck~w~ has been spotted carrying cash.");
	if (beep == 1)
		PlayNotificationBeep();
}

Object case1, case2, case3;
Vector3 truckPos;
Ped driver;
bool driverMade = false;

void armored_truck_mission()
{
	if (eventMade && type_of_event == 5)
	{
		Vector3 position = ENTITY::GET_ENTITY_COORDS(playerPed, 0);
		truckPos = ENTITY::GET_ENTITY_COORDS(truck, 0);
		int truckDistance = GAMEPLAY::GET_DISTANCE_BETWEEN_COORDS(truckPos.x, truckPos.y, truckPos.z, position.x, position.y, position.z, 0);

		if (!driverMade)
		{
			Ped skin = GAMEPLAY::GET_HASH_KEY("mp_s_m_armoured_01");
			STREAMING::REQUEST_MODEL(skin);
			driver = PED::CREATE_PED_INSIDE_VEHICLE(truck, 26, skin, -1, false, false);
			if (ENTITY::DOES_ENTITY_EXIST(driver))
			{
				AI::TASK_VEHICLE_DRIVE_WANDER(driver, truck, 10.0f, 153);
				driverMade = true;
			}
		}

		if (VEHICLE::IS_VEHICLE_DOOR_FULLY_OPEN(truck, 2) && eventMade || VEHICLE::IS_VEHICLE_DOOR_FULLY_OPEN(truck, 3) && eventMade)
		{
			int random = rand() % 3 + 1;

			if (random == 1)
				case1 = OBJECT::CREATE_AMBIENT_PICKUP(0xDE78F17E, truckPos.x, truckPos.y, truckPos.z + 1, -1, 10000, 0, 1, 1);
			if (random == 2)
			{
				case1 = OBJECT::CREATE_AMBIENT_PICKUP(0xDE78F17E, truckPos.x, truckPos.y, truckPos.z + 1, -1, 10000, 0, 1, 1);
				case2 = OBJECT::CREATE_AMBIENT_PICKUP(0xDE78F17E, truckPos.x, truckPos.y, truckPos.z + 1, -1, 12500, 0, 1, 1);
			}
			if (random == 3)
			{
				case1 = OBJECT::CREATE_AMBIENT_PICKUP(0xDE78F17E, truckPos.x, truckPos.y, truckPos.z + 1, -1, 10000, 0, 1, 1);
				case2 = OBJECT::CREATE_AMBIENT_PICKUP(0xDE78F17E, truckPos.x, truckPos.y, truckPos.z + 1, -1, 12500, 0, 1, 1);
				case3 = OBJECT::CREATE_AMBIENT_PICKUP(0xDE78F17E, truckPos.x, truckPos.y, truckPos.z + 1, -1, 15000, 0, 1, 1);
			}

			ENTITY::SET_OBJECT_AS_NO_LONGER_NEEDED(&case1);
			ENTITY::SET_OBJECT_AS_NO_LONGER_NEEDED(&case1);
			ENTITY::SET_OBJECT_AS_NO_LONGER_NEEDED(&case1);

			NotifyAboveMap("The ~b~armored truck's~w~ doors have opened.  Cash has been dropped.");
			if (beep == 1)
				PlayNotificationBeep();
			wanted = PLAYER::GET_PLAYER_WANTED_LEVEL(player);
			if (wanted < 3)
			{
				PLAYER::SET_PLAYER_WANTED_LEVEL(player, 3, 0);
				PLAYER::SET_PLAYER_WANTED_LEVEL_NOW(player, 1);
			}
			UI::REMOVE_BLIP(&truckBlip);
			ENTITY::SET_PED_AS_NO_LONGER_NEEDED(&driver);
			ENTITY::SET_VEHICLE_AS_NO_LONGER_NEEDED(&truck);
			eventMade = false;
			eventOver = false;
			driverMade = false;
			missionTime = 0;
		}

		if (eventOver && truckDistance > 1000 || eventMade && ENTITY::IS_ENTITY_DEAD(playerPed))
		{
			NotifyAboveMap("The ~b~armored truck~w~ has finished carrying cash.");
			if (beep == 1)
				PlayNotificationBeep();
			ENTITY::SET_OBJECT_AS_NO_LONGER_NEEDED(&case1);
			ENTITY::SET_OBJECT_AS_NO_LONGER_NEEDED(&case1);
			ENTITY::SET_OBJECT_AS_NO_LONGER_NEEDED(&case1);
			UI::REMOVE_BLIP(&truckBlip);
			ENTITY::SET_PED_AS_NO_LONGER_NEEDED(&driver);
			ENTITY::SET_VEHICLE_AS_NO_LONGER_NEEDED(&truck);
			eventMade = false;
			eventOver = false;
			driverMade = false;
			missionTime = 0;
		}

		if (!VEHICLE::IS_VEHICLE_DRIVEABLE(truck, 1) && eventMade)
		{
			NotifyAboveMap("The ~b~armored truck~w~ has been destroyed.  The cash cases inside have been ruined.");
			if (beep == 1)
				PlayNotificationBeep();
			UI::REMOVE_BLIP(&truckBlip);
			ENTITY::SET_PED_AS_NO_LONGER_NEEDED(&driver);
			ENTITY::SET_VEHICLE_AS_NO_LONGER_NEEDED(&truck);
			eventMade = false;
			eventOver = false;
			driverMade = false;
			missionTime = 0;
		}
	}
}

void update()
{
	playerPed = PLAYER::PLAYER_PED_ID();
	player = PLAYER::PLAYER_ID();

	if (gotMin == false)
	{
		base_minute = TIME::GET_CLOCK_MINUTES();
		gotMin = true;
	}

	minute = TIME::GET_CLOCK_MINUTES();

	if (minute != base_minute)
	{
		if (!eventMade)
		{
			if (makeEvent)
			{
				int random = rand() % 1;

				if (random == 0)
				{
					CreateCollectibleVehicleMission();
				}
				if (random == 1)
				{
					arms_smuggler_spawn();
				}
				if (random == 2)
				{
					crate_drop_spawn();
				}
				if (random == 3)
				{
					assassination_spawn();
				}
				if (random == 4)
				{
					armored_truck_spawn();
				}
				makeEvent = false;
				waitTime = 0;
			}
			else
			{
				waitTime += 1;
				if (waitTime == waitEndTime)
					makeEvent = true;
			}
		}

		if (eventMade)
		{
			missionTime += 1;
			if (missionTime == missionEndTime)
				eventOver = true;
		}
		gotMin = false;
	}
}

void main()
{
	beep = reader.ReadInteger("Options", "BeepEnabled", 1);
	missionEndTime = reader.ReadInteger("Options", "MissionTime", 210);
	waitEndTime = reader.ReadInteger("Options", "WaitTime", 150);
	blipStyle = reader.ReadInteger("Options", "BlipStyle", 0);

	while (true)
	{
		GatherCollectionMissionSpawnPoints();
		vehicle_collection_mission();
		arms_smuggler_mission();
		crate_drop_mission();
		assassination_mission();
		armored_truck_mission();
		update();
		WAIT(0);
	}
}

void ScriptMain()
{
	srand(GetTickCount());
	main();
}
