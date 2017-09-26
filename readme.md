**TL;DR: events randomly occur on a timer, do the thing to get money. Also, this mod may set you or your computer on fire, and I won't care or help.**


CONTENTS OF THIS DOCUMENT
----------------------------------

 * [Introduction](#INTRODUCTION)
 * [Disclaimer](#DISCLAIMER)
 * [License](#LICENSE)
 * [Requirements](#REQUIREMENTS)
 * [Installation](#INSTALLATION)
 * [Configuration](#CONFIGURATION)
 * [Known Issues](#KNOWN-ISSUES)
 * [Ideas/Todos](#IDEAS-TODOS)
 * [FAQ](#FAQ)
 * [Changelog](#CHANGELOG)
 * [Credits](#CREDITS)

<a name="INTRODUCTION"></a>

INTRODUCTION
-----------------------

Online Events Redux is a complete rewrite of Plastic Tangerine's excellent 'Online Random Events for Single Player v1.2' (https://www.gta5-mods.com/scripts/online-random-events-for-single-player). I had a couple nitpicks with the implementation however, and since the source was provided, I decided to finally learn some C++ and get my feet wet modding GTA. Holy cow C++ sucks. Of course now, after dozens of hours of coding, I see that I could have been using C# the whole time, but you know what? C# looks like it's for hipsters. It looks like the kind of language you can only code on a MacBook, sitting at a bare wooden table, with a mason jar of iced coffee in arm's reach. But I digress.

The original concept of the mod is more or less the same. There are five mission types:

* Steal Vehicle
* Destroy Vehicle
* Assassination
* Armored Truck
* Crate Drop

I think they're pretty self-explanatory. If you ignore the event (or take too long), the mission will time out (unless you're relatively close). There is also a cooldown period after each mission. On completion, you will get a lot of money (compared to vanilla events). Almost all of these values can be modified.



<a name="DISCLAIMER"></a>

DISCLAIMER
---------------------

This product is meant for educational purposes only. Any resemblance to real persons, living or dead is purely coincidental. Void where prohibited. Some assembly required. List each check separately by bank number. Batteries not included. Contents may settle during shipment. Use only as directed. No other warranty expressed or implied. Do not use while operating a motor vehicle or heavy equipment. Postage will be paid by addressee. Subject to CAB approval. This is not an offer to sell securities. Apply only to affected area. May be too intense for some viewers. Do not stamp. Use other side for additional listings. For recreational use only. Do not disturb. All models over 18 years of age. If condition persists, consult your physician. No user-serviceable parts inside. Freshest if eaten before date on carton. Subject to change without notice. Times approximate. Simulated picture. No postage necessary if mailed in the United States. Please remain seated until the ride has come to a complete stop. Breaking seal constitutes acceptance of agreement. For off-road use only. As seen on TV. One size fits all. Many suitcases look alike. Contains a substantial amount of non-tobacco ingredients. Colors may, in time, fade. We have sent the forms which seem right for you. Slippery when wet. For office use only. Not affiliated with the American Red Cross. Drop in any mailbox. Edited for television. Keep cool; process promptly. Post office will not deliver without postage. List was current at time of printing. Return to sender, no forwarding order on file, unable to forward. Not responsible for direct, indirect, incidental or consequential damages resulting from any defect, error or failure to perform. At participating locations only. Not the Beatles. Penalty for private use. See label for sequence. Substantial penalty for early withdrawal. Do not write below this line. Falling rock. Lost ticket pays maximum rate. Nap was here. Your canceled check is your receipt. Add toner. Place stamp here. Avoid contact with skin. Sanitized for your protection. Be sure each item is properly endorsed. Sign here without admitting guilt. Slightly higher west of the Mississippi. Employees and their families are not eligible. Beware of dog. Contestants have been briefed on some questions before the show. Limited time offer, call now to ensure prompt delivery. You must be present to win. No passes accepted for this engagement. No purchase necessary. Processed at location stamped in code at top of carton. Shading within a garment may occur. Use only in a well-ventilated area. Keep away from fire or flames. Replace with same type. Approved for veterans. Booths for two or more. Check here if tax deductible. Some equipment shown is optional. Price does not include taxes. No Canadian coins. Not recommended for children. Prerecorded for this time zone. Reproduction strictly prohibited. No solicitors. No alcohol, dogs or horses. No anchovies unless otherwise specified. Restaurant package, not for resale. List at least two alternate dates. First pull up, then pull down. Call toll free number before digging. Driver does not carry cash. Some of the trademarks mentioned in this product appear for identification purposes only. Objects in mirror may be closer than they appear. Record additional transactions on back of previous stub. Unix is a registered trademark of AT&T. Do not fold, spindle or mutilate. No transfers issued until the bus comes to a complete stop. Package sold by weight, not volume. Your mileage may vary. If the flow controls supplied are not installed, this unit will not operate properly. Keep out of reach of children. When this set is used with other equipment, if the picture is not stable or the buzz sound is heard, try to change the mutual position of relevant equipment or take enough distance between them. This unit not labeled for retail sale. Phenylketonurics: may contain phenylalanine. Close cover before striking. Mind the gap. No merchantability expressed or implied. Parental discretion is advised. Sold as a novelty item only. Although robust enough for general use, adventures into the esoteric periphery may reveal unexpected quirks. Not available in stores. May cause abdominal cramping and loose stools. Vitamins A, D, E, and K may have been added. Not designed or intended for use in on-line control of aircraft, air traffic, aircraft navigation or aircraft communications; or in the design, construction, operation or maintenance of any nuclear facility. Container may explode if heated. May contain traces of various nuts and seeds.

This supersedes all previous notices. *(credit: idiot-dog.com/disclaimer)*

No but seriously, if you use this and something goes sideways, refer to the [LICENSE.TXT](LICENSE.TXT), specifically the part in all caps where it says I won't help you.

<a name="LICENSE"></a>

LICENSE
------------------

Plastic Tangerine included this on their mod page: "You may modify, and use code from this program without my permission. I feel providing source code allows the modding community to grow. You may re-upload this without my permission on any other site but this one. I only request that you credit me when you do."

I've almost completely rewritten the code. It bears virtually no resemblance to the original. Although the original license appears to be attribution-only, I suspect that they would feel it fair for me to license its derivative under the MIT license. Thus, please refer to [LICENSE.TXT](LICENSE.TXT).

<a name="REQUIREMENTS"></a>

REQUIREMENTS
-----------------------

* GTA V 		v1.0.1032.1
* Script Hook V	v1.0.1032.1		http://www.dev-c.com/gtav/scripthookv/

<a name="INSTALLATION"></a>

INSTALLATION
-----------------------

Place OnlineEvents.asi and OnlineEvents.ini in the same folder as ScriptHookV.dll.

<a name="CONFIGURATION"></a>

CONFIGURATION
------------------------

Here's where it gets interesting. The INI file has a lot of values. The defaults are what I settled on to provide balanced, fun gameplay. With few exceptions, you can set them to whatever you want- but extreme values may very well break the game. I will try to elaborate below. Bonus: I literally copied the OnlineEvents.ini. If you want to reset to defaults, you can just copy the following section back in.
```
[OPTIONS]
; Set to false to turn beeps off.
play_notification_beeps=true
; Set to true to use plain circles instead of descriptive blips.
use_default_blip=false
; Once a mission starts, you will have this many seconds to finish it. If this time elapses and you are more than mission_minimum_range_for_timeout meters away, the event will end.
mission_timeout=360
; When the game loads, and after an event ends, this many seconds must elapse before the script will start a new one. Since the script dynamically gathers spawn points, setting this too low will cause it to fail to create events until it has found a spawn point that satisfies spawn_point_minimum_range and spawn_point_maximum_range. I hard-coded a minimum of 30 seconds on this.
mission_cooldown=60
; Generally speaking, events will not begin closer than this number. Again, since spawn points are dynamically gathered, if you start the game and just stand around for a while (mission_cooldown), events won't be able to start until you move about this far from your position at load (and then they're likely to start right around where you loaded the game...).
spawn_point_minimum_range=1000
; Same as above, but maximum. AFAIK, you can set this as high as you like, but one of the primary reasons I decided to rewrite the original is because the script always chose spawn points VERY far away (and there were only 10 hard-coded points for each event, so it got kinda repetitive always driving 5 miles to the same spot).
spawn_point_maximum_range=3000
; As noted above, this is how close you must be to avoid the timeout. You can probably set this to 0 if you want the event to always be over based on time alone (or 10000 if you never want it to time out). I haven't tested this though...
mission_minimum_range_for_timeout=333
; The reward numbers I chose are incredibly arbitrary. I may modify them depending on feedback. For now, you can multiply all rewards by the same amount here. Here's a perfect example of a number you shouldn't set very high. Setting this to 10000000000000.0 might give you 10000000000000000 dollars for each mission - or it may crash the game. I have no idea. Just don't do it.
mission_reward_modifier=1.0
; the following two options are bitwise integers. Feel free to look it up. It's just a rather simple method of supplying flags as a single parameter. Check it out:
;	Compact = 1,
;	Sedan = 2,
;	SUV = 4,
;	Coupe = 8,
;	Muscle = 16,
;	SportsClassic = 32,
;	Sports = 64,
;	Super = 128,
;	Motorcycle = 256,
;	OffRoad = 512,
;	Industrial = 1024,
;	Utility = 2048,
;	Van = 4096,
;	Cycle = 8192,
;	Boat = 16384,
;	Helicopter = 32768,
;	Plane = 65536,
;	Service = 131072,
;	Emergency = 262144,
;	Military = 524288,
;	Commercial = 1048576,
;	Train = 2097152
;
;	Each of these values is twice the last. This has the simple property that no combination of these numbers will add up to any other one of these numbers. Try it.
;	So all you need to do is look at what classes you want, and add up the numbers next to them. Then use that number below.
;	By default I want the destroy mission to pick SUVs, Muscles, Offroads, and Motorcycles. So I add up 4, 16, 512, and 256. Voila.
destroyable_vehicle_flags=788
;	I'll let you figure out how to decipher the classes I default to here (hint: start by subtracting the largest number you can)
stealable_vehicle_flags=1023
;
[DEBUG]
; You can usually ignore all of these...
; You can't turn logging entirely off, but you can change it some. 
;	LogError			= 1, Only things that should never have happened.
;	LogNormal			= 2, Some informational things, usually will only log once. Should get a couple lines a minute on this.
;	LogVerbose			= 4, Some informational things that repeat a lot.
;	LogVeryVerbose		= 8, Mostly to follow program flow, but spammy.
;	LogExtremelyVerbose	= 16, These will probably send a hundred times a second. This should always be off, unless you want a log file larger than your GTA install.
logging_level=1
; Set this to about 10 or 15 seconds, and it will wait for the persistence script to load the vehicles, and then reserve those spawn points so they can't be used later.
; If this is set to 0 however, it won't wait or reserve spawns at all. It (mostly) works.
seconds_to_wait_for_vehicle_persistence_scripts=0
; The script randomly selects from dynamically populated spawn points and vehicle models, but constrains them via the preferences we defined above. This means that at certain times (namely right after loading) there may not be any items in a list that satisfy all the criteria. There's really no reason to change this number, though. It shouldn't ever affect performance, anyway.
number_of_tries_to_select_items=100
; The script won't consider vehicles for spawn points inside this range, primarily to avoid adding a player's car if they get out in the middle of the freeway. It's an imperfect method. Find me a better one.
vehicle_search_range_minimum=30
; In practice, I've gathered over two thousand spawns in a few minutes of driving through downtown LS, and over ten thousand after ~30 minutes of gameplay. The script uses a First-In-First-Out method to replace entries after this limit is reached, but feel free to increase this. I haven't tested over 10000 though, it may very well affect performance.
maximum_number_of_spawn_points=10000
; This probably never needs to be touched. It works the same way as above, but the game only has a few hundred models anyway. Also, this isn't holding the models themselves, only hashes, so it's not really a memory issue.
maximum_number_of_vehicle_models=1000
```

<a name="KNOWN-ISSUES"></a>

KNOWN ISSUES
-----------------------

* Vehicle Persistence. Since the script dynamically gathers spawn points (I wonder how many times I will type that), the 30-some vehicles that are spawned by the otherwise superlative 'I'm Not MentaL's Persistance Mod'[sic] are considered possible spawns, even though the game engine would normally never place vehicles where I parked most of mine... I have some techniques in place to try to avoid spawning vehicles where vehicles already exist, but I'm open to further ideas.

* The script uses parked spawn points for the armored cars. This means that they spawn in parking garages with embarrassing regularity. This wouldn't be embarrassing, except that they generally have a difficult time navigating out. Sometimes, they literally give up and run away from the truck, which obviously makes the event rather trivial. Possible fixes include a) discovering un-parked spawn points (via my method, probably infeasible. I could gather millions or billions of points...), and b) checking for a roof over spawns (which won't prevent spawns on the roof of the garage, and will prevent otherwise legitimate spawns under freeways, etc...).

* The doors on the armored truck must be blown open to get the reward, but it seems you're just as likely to blow the truck up. I tried to make the script open the doors when the player gets in the driver's seat, but for some reason it doesn't actually work until the player backs the truck up. Which is odd. I'm actively trying to figure out why.

* The script will only spawn certain vehicle classes for steal missions, but certain vehicles still can't be resprayed (and VEHICLE::_IS_VEHICLE_SHOP_RESPRAY_ALLOWED() doesn't appear to work, at all), such as the Taxi. The most useful (but terribly inelegant) solution in the short term is probably to hard-code an exclude filter for unusable models, so please report any such vehicles.

* The guards protecting the crate are supposed to wander around. They don't. I don't consider this high-priority (they still fight the player), but it's there.

* The script doesn't check for actual (like, story-mode) mission status, so... probably don't use this until you've finished the game.

<a name="IDEAS-TODOS"></a>

IDEAS/TODOS
----------------------

[ ] Ensure next spawn is (n) meters from last spawn - minimum_distance_between_spawns

[ ] Mark parking garages to avoid for Armored Truck missions.

[ ] YOU'VE DONE A LOT OF CRIME
	Randomly receive some star(s).
	Escape.

[ ] CRATE DROP
	Plane or helicopter appears on map.
	Enemy vehicles spawn as well.
	Follow it to drop point.
	Try to acquire the crate before the enemy does.

[ ] DISTRACT
	Go to a point.
	Get (n) star(s).
	Keep those stars for (n) seconds, but don't go further than (n) meters from origin.
	Receive warning when you're further than (n/2) meters.

[ ] SELF DEFENSE
	An enemy hitman (or squad) has found you.
	Survive.

* Assassination missions should include some context, so the player isn't just killing random people. It's a little bit immersion-breaking for me to run around killing random peds who are literally just minding their own business (it doesn't help that I'm morally opposed to killing innocent people anyway - incidentally, GTAV is a hard game for me to play...) Also, bodyguards might at least provide a challenge.

* The destroy vehicle mission is actually rather boring. I'm thinking of adding a driver, and/or opposing 'smugglers' who will try to steal the vehicle and escape. This seems complicated to code, though.

* I'm certainly open to new ideas.

<a name="FAQ"></a>

FAQ 
--------------

* As requested by several people, here's what I think the difference are between Plastic Tangerine's mod and mine. There may be other minor differences I can't remember or that were unintentional:
	- The single biggest change (really the reason I decided to rewrite it in the first place.) is the dynamic spawn points: each of the missions in PT's mod had 10 hard-coded spawn points. This means that after playing for a while, you would begin revisiting the same spots. I got tired of finding a smuggler's vehicle behind the strip mall 5 times in a row. My mod uses an algorithm to try and dynamically find suitable places to spawn a vehicle, and thus you may find that the missions point you to virtually anywhere on the map (I have had upwards of twenty thousand possible spawn points in a single game).

* **Steal Vehicle**
 	- Picks from dynamically generated spawn points rather than 10 hard-coded locations.
 	- Picks from random vehicle models instead of 10 hard-coded. Allows you to select which vehicle classes are available to pick from (but always has a chance to not pick Supers, Sports, Sport Classics, and Off-Road vehicles).
 	- Has a chance to lock the doors and or require hot-wiring.
 	- Sometimes requires that the vehicle be undamaged on delivery.
 	- Randomly gives 0, 1 or 2 stars (or even 3 if the vehicle is a Super) when the player steals the vehicle.
 	- Rewards are based on the average value of vehicles in the class, plus or minus a random amount (but much more than PT's mod).

* **Destroy Vehicle & Assassination**
	- Randomly picks from spawn points and models, as above.
	- Rewards more money than original mod.

* **Armored Truck**
	- Randomly picks from spawn points, as above (although as noted in Known Issues, this can sometimes result in them getting stuck).
	- Sometimes spawns a passenger, and locks the doors until they leave the vehicle (which can happen if they can't drive it anymore), or die.
	- The rear doors can be opened by entering the driver's seat (instead of only by using explosives).

* **Crate Drop**
	 - Randomly spawns 5 guards instead of 10 (plus 3 more for a 'special' crate), and randomly selects the weapons they use.
	 - Bad guys face away from the crate (which might not seem like a big deal but it took me hours to figure out!).
	 - Recovering the crate no longer rewards weapons because why? If you're playing this in the endgame like you should be, you already have access to all the weapons.
	 - Recovering the crate rewards more money.


* "Is the source code available?"
	- Yes: https://github.com/superelitist/OnlineEvents

* "I installed your mod and my computer set my cat on fire! Can you fix my cat?"
	- See below.

* "I installed your mod and my computer became sentient and spends its time browsing the Internet for porn, and won't let me play GTAV anymore!"
	- See above.

* "Will you add the 1998 Volvo XC70 to the game?"
	- It's a beautiful car, but no.

* "How did you learn to make mods? Will you teach me?"
	- By excruciating trial-and-error. No.

<a name="CHANGELOG"></a>

CHANGELOG 
--------------------

 1.1	- Armored Truck mission: Killing the driver no longer unlocks the door, but you can force the doors open by damaging the truck enough. This is not perfect, but it's a WIP. The doors should still unlock if the driver flees the vehicle. The doors should open and release the cash more reliably. The truck itself spawns on the closest vehicle node to the chosen spawn, so it shouldn't get stuck as often.

		- Crate Drop mission: Guards respawn! Unfortunately they don't always spawn behind you, so it can be a bit immersion breaking. Still, even the default setting of 4 guards is challenging: that's four enemies AT A TIME. Also, I added over a hundred hand-picked spawn points for the crate. Some of them cause it to spawn inside things, which is its own problem. I'll try to fix that later. At least it's not the same ten any more. Oh, also the guards should wander around, at least until you start shooting. Oh, also the guards spawn more randomly, and *should* always get set on the ground, but I make no promises. You may sometimes see them fall out of the sky. You've been warned.

		- The algorithm that finds spawn points was collecting points that were basically the same as existing ones, but off by ~0.0001 meters. Now the function only adds points that are at least 1 meter from all the other points. Unfortunately, this added a lot of overhead, and in some cases can actually slow the game down (I'm talking 16+ ms to complete, which cuts the framerate in half...). I've added some self-profiling code to prevent this. 
		
		- Added load_without_notification flag to prevent the popup and beep when the mod starts.

		- Changed some default settings in the ini file, and edited some descriptions.

		- Logging at the most verbose level shouldn't fill up your disk anymore. It also now pad columns a bit more effectively.
 
 1.0	- Umm...

<a name="CREDITS"></a>

CREDITS
------------------

* Plastic Tangerine
	- Created the original mod on which this work is based. Plastic Tangerine, if not for you including your source, I never would have tried something like this. So thank you, very much.
* Alexander Blade
	- Created Script Hook V. Plus I, like Plastic Tangerine, appropriated code from his SDK.
* xiaohe521
	- Although no attribution appears to be requested, it seems that the INIReader/INIWriter code came from here: https://www.codeproject.com/Articles/10809/A-Small-Class-to-Read-INI-File