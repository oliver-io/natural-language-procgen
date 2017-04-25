/******************************************************************************
* The questionable procedural code written by Oliver Carrillo (Saire). Do not *
* redistribute this code without permission from Oliver Carrillo.  This code  *
* allows for variable in-game customizations to mobiles and objects and rooms *
* and such things, in a foolhearted attempt to make the game look interesting.*
*******************************************************************************/


#if defined(macintosh)
#include <types.h>
#else
#include <sys/types.h>
#endif
#include <uuid/uuid.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <malloc.h>
#include <stdlib.h>
#include "merc.h"
#include "recycle.h"
#include "tables.h"
#include "lookup.h"
#include "olc.h"


//struct: 	vnum 		(int; terminus)
//		location	(string; interpreted as a room's vnum if 
//				 numeric and an area-name if alphanumeric)
//		max 		(int: the max for the area or room depending on location)
//		sector 		(int: if -1, anywhere, otherwise limited to the delimited sector)
const   struct  spawn_table_type      demo_spawn_table[] =
{
	//load#	    loadarea	//max	//loadsector
	{ 1000001, "demo", 75, -1 }, //songbirds
	{ 1000002, "demo", 10, -1 }, //squirrels
	{ 1000003, "demo", 10, -1 }, //chipmunks
	{ 1000004, "demo", 8, -1 }, //fox
	{ 1000005, "demo", 4, -1 }, //wolfa
	{ 1000006, "demo", 4, -1 }, //wolfb
	{ 1000007, "demo", 1, -1 }, //lonewolfa
	{ 1000008, "demo", 1, -1 }, //lonewolfb
	{ 1000009, "demo", 4, -1 }, //brownbear
	{ 1000010, "demo", 1, -1 }, //grizzlybear
	{ 1000011, "demo", 15, -1 }, //rabbita
	{ 1000012, "demo", 15, -1 }, //rabbitb
	{ 1000013, "demo", 10, -1 }, //doe
	{ 1000014, "demo", 6, -1 }, //buck
	{ 1000015, "demo", 6, -1 }, //bambi
	{ 1000016, "demo", 3, -1 }, //moose
	{ 1000017, "demo", 15, -1 }, //coon
	{ 1000018, "demo", 10, -1 }, //lynx
	{ 1000019, "demo", 7, -1 }, //bobcat
	{ 1000020, "demo", 2, -1 }, //cougar
	{ 1000021, "demo", 10, -1 }, //pig
	{ 1000022, "demo", 13, -1 }, //boar
	{ 1000023, "demo", 2, -1 }, //superboar
	{ 1000024, "demo", 15, -1 }, //mediumbird
	{ 1000025, "demo", 13, -1 }, //bigbird
	{ 1000026, "demo", 10, -1 }, //porcupine
	{ 1000027, "demo", 6, -1 }, //badger
	{ -1, NULL, 0, -1 },
};


void set_procgen_mobile_values_by_tag(CHAR_DATA *npc, char *search, sh_int tablevalue, int selection)
{

	char comparison[MSL];


	//calculate size
	strcpy(comparison, "#size");
	if (!str_cmp(search, comparison))
	{
		if (npc->size == 0)
		{
			bugf("set_procgen_mobile_values_by_tag(): no animal principle size");
			return;
		}

		double size = npc->size;
		size = size * trait_table[tablevalue].table[selection].value;
		int size_i = (int)size;
		npc->size = number_range(size, size * 2);
	}

	//calculate rarity
	//a trait will only give rarity to an NPC if its tag is in this
	//list of !str_cmp searches
	if (!str_cmp(search, "#furcolor") ||
		!str_cmp(search, "#haircolor") ||
		!str_cmp(search, "#darkhaircolor") ||
		!str_cmp(search, "#lighthaircolor") ||
		!str_cmp(search, "#brownscalehaircolor") ||
		!str_cmp(search, "#darkfurcolor") ||
		!str_cmp(search, "#lightfurcolor") ||
		!str_cmp(search, "#brownscalefurcolor") ||
		!str_cmp(search, "#furpattern") ||
		!str_cmp(search, "#furcondition") ||
		!str_cmp(search, "#rabbittrait") ||
		!str_cmp(search, "#feathercondition") ||
		!str_cmp(search, "#songbird") ||
		!str_cmp(search, "#mediumbird") ||
		!str_cmp(search, "#bigbird"))
	{
		int rarity_i = UMAX(1, trait_table[tablevalue].table[selection].value);
		increment_rarity(npc->rarity, rarity_i);
	}

	//calculate furcolor.  proc only once, except for primary colors
	if (!npc->color || !str_cmp(search, "#furcolor") || !str_cmp(search, "#haircolor"))
	{
		if (!str_cmp(search, "#furcolor") ||
			!str_cmp(search, "#darkfurcolor") ||
			!str_cmp(search, "#lightfurcolor") ||
			!str_cmp(search, "#brownscalefurcolor") ||
			!str_cmp(search, "#haircolor") ||
			!str_cmp(search, "#darkhaircolor") ||
			!str_cmp(search, "#lighthaircolor") ||
			!str_cmp(search, "#brownscalehaircolor"))
		{
			char furcolor[MSL];
			sprintf(furcolor, "%s", trait_table[tablevalue].table[selection].description);
			npc->color = str_dup(furcolor);
		}
	}

	if (!str_cmp(search, "#furpattern"))
	{
		char furpattern[MSL];
		sprintf(furpattern, "%s", trait_table[tablevalue].table[selection].description);
		npc->furpattern = str_dup(furpattern);
	}

	if (!str_cmp(search, "#furcondition") || !str_cmp(search, "#haircondition"))
	{
		char furcondition[MSL];
		sprintf(furcondition, "%s", trait_table[tablevalue].table[selection].description);
		npc->furcondition = str_dup(furcondition);
	}

	return;
}

//going to go through a room and procedurally fill in tags.  This is where we put special cases
//for generated values that need to alter the game-state as well
void set_procgen_room_values_by_tag(ROOM_INDEX_DATA *room, char *search, sh_int tablevalue, int selection)
{

	if (trait_table[tablevalue].table[selection].value > 0)
		initiate_special_procgen_condition(room, trait_table[tablevalue].table[selection].value);

	return;
}

//fill in procedural stuff but pay attention to special values that need to alter the gamestate
void set_procgen_obj_values_by_tag(OBJ_DATA *pObj, char *search, sh_int tablevalue, int selection)
{

	if (!str_cmp(search, "#plantcolor") ||
		!str_cmp(search, "#brightplantcolor") ||
		!str_cmp(search, "#boringplantcolor") ||
		!str_cmp(search, "#brownscaleplantcolor") ||
		!str_cmp(search, "#rosecolor"))
	{
		if (pObj->color) free_string(pObj->color);

		pObj->color = str_dup(trait_table[tablevalue].table[selection].description);
	}
	return;
}

//fill in procedural stuff but pay attention to special values that need to alter the gamestate
//i.e. here, gender, changing the inventory of a random mobile, etc
void procgen_string_replace_mobile(CHAR_DATA *npc)
{

	int x, roll, max;
	char search[MSL] = "", rand_buf[MSL];
	rand_buf[0] = '\0';
	static char * const he_she[] = { "it", "he", "she" };
	static char * const him_her[] = { "it", "him", "her" };
	static char * const his_her[] = { "its", "his", "her" };
	static char * const man_woman[] = { "creature", "man", "woman" };

	for (x = 0; trait_table[x].name != NULL; x++)
	{
		sprintf(search, "#%s", trait_table[x].name);
		if (strcasestr(npc->description, search) == NULL &&
			strcasestr(npc->name, search) == NULL &&
			strcasestr(npc->short_descr, search) == NULL &&
			strcasestr(npc->long_descr, search) == NULL) continue;

		do
		{
			//while loop executes nothing... just counts.......
			for (max = 1; trait_table[x].table[max].description != NULL; max++);//note this semicolon
			roll = number_range(1, max);

			if (trait_table[x].table[roll].description == NULL)
				continue;

			//check for trait incidence
			if (number_percent() < trait_table[x].table[roll].incidence)
				continue;

			sprintf(rand_buf, "%s", trait_table[x].table[roll].description);

			//make sure the trait works
		} while (rand_buf[0] == '\0');

		bool firstsearch = TRUE;
		while (strcasestr(npc->description, search) != NULL ||
			strcasestr(npc->name, search) != NULL ||
			strcasestr(npc->short_descr, search) != NULL ||
			strcasestr(npc->long_descr, search) != NULL)
		{
			if (strcasestr(npc->description, search) != NULL)
			{
				if (firstsearch) set_procgen_mobile_values_by_tag(npc, search, x, roll);
				npc->description =
					string_replace(npc->description, search, rand_buf);
			}
			if (strcasestr(npc->name, search) != NULL)
				npc->name =
				string_replace(npc->name, search, rand_buf);
			if (strcasestr(npc->short_descr, search) != NULL)
				npc->short_descr =
				string_replace(npc->short_descr, search, rand_buf);
			if (strcasestr(npc->long_descr, search) != NULL)
				npc->long_descr =
				string_replace(npc->long_descr, search, rand_buf);

			firstsearch = FALSE;

		}

		rand_buf[0] = '\0';
	}

	int gender = npc->sex;
	if (gender < 0 || gender > 2) gender = 0;

	sprintf(search, "%s", "#manwoman");
	while (strcasestr(npc->description, search) != NULL ||
		strcasestr(npc->name, search) != NULL ||
		strcasestr(npc->short_descr, search) != NULL ||
		strcasestr(npc->long_descr, search) != NULL)
	{
		npc->description =
			string_replace(npc->description, search, man_woman[gender]);
		npc->name =
			string_replace(npc->name, search, man_woman[gender]);
		npc->short_descr =
			string_replace(npc->short_descr, search, man_woman[gender]);
		npc->long_descr =
			string_replace(npc->long_descr, search, man_woman[gender]);
	}

	sprintf(search, "%s", "#himher");
	while (strcasestr(npc->description, search) != NULL ||
		strcasestr(npc->name, search) != NULL ||
		strcasestr(npc->short_descr, search) != NULL ||
		strcasestr(npc->long_descr, search) != NULL)
	{
		npc->description =
			string_replace(npc->description, search, him_her[gender]);
		npc->name =
			string_replace(npc->name, search, him_her[gender]);
		npc->short_descr =
			string_replace(npc->short_descr, search, him_her[gender]);
		npc->long_descr =
			string_replace(npc->long_descr, search, him_her[gender]);
	}

	sprintf(search, "%s", "#hisher");
	while (strcasestr(npc->description, search) != NULL ||
		strcasestr(npc->name, search) != NULL ||
		strcasestr(npc->short_descr, search) != NULL ||
		strcasestr(npc->long_descr, search) != NULL)
	{
		npc->description =
			string_replace(npc->description, search, his_her[gender]);
		npc->name =
			string_replace(npc->name, search, his_her[gender]);
		npc->short_descr =
			string_replace(npc->short_descr, search, his_her[gender]);
		npc->long_descr =
			string_replace(npc->long_descr, search, his_her[gender]);
	}

	sprintf(search, "%s", "#heshe");
	while (strcasestr(npc->description, search) != NULL ||
		strcasestr(npc->name, search) != NULL ||
		strcasestr(npc->short_descr, search) != NULL ||
		strcasestr(npc->long_descr, search) != NULL)
	{
		npc->description =
			string_replace(npc->description, search, he_she[gender]);
		npc->name =
			string_replace(npc->name, search, he_she[gender]);
		npc->short_descr =
			string_replace(npc->short_descr, search, he_she[gender]);
		npc->long_descr =
			string_replace(npc->long_descr, search, he_she[gender]);
	}
	return;
}

//string search and replace for objects tagged with #values
void procgen_string_replace_object(OBJ_DATA *obj)
{

	int x, roll, max;
	char search[MSL] = "", rand_buf[MSL];
	rand_buf[0] = '\0';

	for (x = 0; trait_table[x].name != NULL; x++)
	{

		sprintf(search, "#%s", trait_table[x].name);
		if (strcasestr(obj->description, search) == NULL &&
			strcasestr(obj->name, search) == NULL &&
			strcasestr(obj->short_descr, search) == NULL &&
			strcasestr(obj->long_descr, search) == NULL) continue;

		do
		{
			//while loop executes nothing... just counts.......
			for (max = 1; trait_table[x].table[max].description != NULL; max++);//note this semicolon
			roll = number_range(1, max);

			//check for trait incidence
			if (number_percent() < trait_table[x].table[roll].incidence)
				continue;
			if (trait_table[x].table[roll].description != NULL)
				sprintf(rand_buf, "%s", trait_table[x].table[roll].description);

			//make sure the trait works
		} while (rand_buf[0] == '\0');

		bool firstsearch = TRUE;
		while (strcasestr(obj->description, search) != NULL ||
			strcasestr(obj->name, search) != NULL ||
			strcasestr(obj->short_descr, search) != NULL ||
			strcasestr(obj->long_descr, search) != NULL)
		{
			if (firstsearch) set_procgen_obj_values_by_tag(obj, search, x, roll);
			obj->description =
				string_replace(obj->description, search, rand_buf);
			obj->name =
				string_replace(obj->name, search, rand_buf);
			obj->short_descr =
				string_replace(obj->short_descr, search, rand_buf);
			obj->long_descr =
				string_replace(obj->long_descr, search, rand_buf);
			firstsearch = FALSE;
		}

		rand_buf[0] = '\0';
	}

	sprintf(search, "%s", "#objweight");
	while (strcasestr(obj->description, search) != NULL ||
		strcasestr(obj->name, search) != NULL ||
		strcasestr(obj->short_descr, search) != NULL ||
		strcasestr(obj->long_descr, search) != NULL)
	{
		char buf[MSL];
		describe_obj_weight(obj, buf);

		obj->description =
			string_replace(obj->description, search, buf);
		obj->name =
			string_replace(obj->name, search, buf);
		obj->short_descr =
			string_replace(obj->short_descr, search, buf);
		obj->long_descr =
			string_replace(obj->long_descr, search, buf);
	}



	sprintf(search, "%s", "#objstate");
	while (strcasestr(obj->description, search) != NULL ||
		strcasestr(obj->name, search) != NULL ||
		strcasestr(obj->short_descr, search) != NULL ||
		strcasestr(obj->long_descr, search) != NULL)
	{
		char buf[MSL];
		describe_obj_state(obj, buf);

		obj->description =
			string_replace(obj->description, search, buf);
		obj->name =
			string_replace(obj->name, search, buf);
		obj->short_descr =
			string_replace(obj->short_descr, search, buf);
		obj->long_descr =
			string_replace(obj->long_descr, search, buf);
	}

	sprintf(search, "%s", "#material");
	while (strcasestr(obj->description, search) != NULL ||
		strcasestr(obj->name, search) != NULL ||
		strcasestr(obj->short_descr, search) != NULL ||
		strcasestr(obj->long_descr, search) != NULL)
	{
		char buf[MSL];
		describe_obj_material(obj, buf);

		obj->description =
			string_replace(obj->description, search, buf);
		obj->name =
			string_replace(obj->name, search, buf);
		obj->short_descr =
			string_replace(obj->short_descr, search, buf);
		obj->long_descr =
			string_replace(obj->long_descr, search, buf);
	}

	sprintf(search, "%s", "#alloytype");
	while (strcasestr(obj->description, search) != NULL ||
		strcasestr(obj->name, search) != NULL ||
		strcasestr(obj->short_descr, search) != NULL ||
		strcasestr(obj->long_descr, search) != NULL)
	{
		char buf[MSL];
		describe_obj_alloy(obj, buf);

		obj->description =
			string_replace(obj->description, search, buf);
		obj->name =
			string_replace(obj->name, search, buf);
		obj->short_descr =
			string_replace(obj->short_descr, search, buf);
		obj->long_descr =
			string_replace(obj->long_descr, search, buf);
	}

	rand_buf[0] = '\0';

	return;
}



//autogenerate a dforest-area type room: this is going to fill in a whole area
//with a set of autogen'd values, as a sort of a first-pass for large forest
//areas pre-build
void autogen_dforest(ROOM_INDEX_DATA *room)
{
	char string[MSL] = "";
	int MAX_PICK_ELEMENT = 4;

	static char * const tag[] = { "#", "#...", "#," };
	static char * const str[] = { "dforesttrees", "dforestsense", "dforestground", "dforestflora" };

	int conjunct;
	bool third;
	int pickone, picktwo, pickthree;

gotoisawesome:
	strcpy(string, "");
	conjunct = (number_range(1, 2) - 1);
	third = 1;
	pickone = (number_range(1, MAX_PICK_ELEMENT) - 1);
	picktwo = (number_range(1, MAX_PICK_ELEMENT) - 1);
	pickthree = (number_range(1, MAX_PICK_ELEMENT) - 1);
	while (picktwo == pickone) picktwo = (number_range(1, MAX_PICK_ELEMENT) - 1);
	while (pickthree == pickone || pickthree == picktwo)
		pickthree = (number_range(1, MAX_PICK_ELEMENT) - 1);


	if (conjunct == 1)
	{
		strcat(string, tag[1]);
		strcat(string, str[pickone]);
		strcat(string, ", and ");
		strcat(string, tag[2]);
		strcat(string, str[picktwo]);
		conjunct = 0;
	}
	else
	{
		strcat(string, tag[0]);
		strcat(string, str[pickone]);
		strcat(string, " ");
		if (third)
			strcat(string, tag[1]);
		else	strcat(string, tag[0]);
		strcat(string, str[picktwo]);
		conjunct = 1;
	}

	if (third)
	{
		if (conjunct == 1)
		{
			strcat(string, ", and ");
			strcat(string, tag[2]);
			strcat(string, str[pickthree]);
		}
		else
		{
			strcat(string, " ");
			strcat(string, tag[0]);
			strcat(string, str[pickthree]);
		}
	}

	strcat(string, "\n");

	free_string(room->description);
	room->description = str_dup(string);
	free_string(room->gen_description);
	room->gen_description = str_dup(string);
	procedurally_describe(NULL, NULL, room);
	if (strlen(room->gen_description) < 200)
		goto gotoisawesome;

	return;
}


//this is going to fill in an area with lmarythstreet type autogens
void autogen_lmarythstreet(ROOM_INDEX_DATA *room)
{
	char string[MSL] = "";
	int MAX_PICK_ELEMENT = 5;

	static char * const tag[] = { "#", "#...", "#," };
	static char * const str[] = { "lmarythstreet", "lmarythfacades", "lmarythpalace", "lmarythsense", "lmarythwalls" };

	int conjunct;
	int pickone, picktwo, pickthree;

gotoisawesome:
	strcpy(string, "");
	conjunct = (number_range(1, 2) - 1);
	pickone = (number_range(1, MAX_PICK_ELEMENT) - 1);
	picktwo = (number_range(1, MAX_PICK_ELEMENT) - 1);
	pickthree = (number_range(1, MAX_PICK_ELEMENT) - 1);
	while (picktwo == pickone) picktwo = (number_range(1, MAX_PICK_ELEMENT) - 1);
	while (pickthree == pickone || pickthree == picktwo)
		pickthree = (number_range(1, MAX_PICK_ELEMENT) - 1);


	if (conjunct)
	{
		strcat(string, tag[1]);
		strcat(string, str[pickone]);
		strcat(string, ", and ");
		strcat(string, tag[2]);
		strcat(string, str[picktwo]);
		conjunct = 0;
	}
	else
	{
		strcat(string, tag[0]);
		strcat(string, str[pickone]);
		strcat(string, " ");
		strcat(string, tag[1]);
		strcat(string, str[picktwo]);
		conjunct = 1;
	}

	if (conjunct)
	{
		strcat(string, ", and ");
		strcat(string, tag[2]);
		strcat(string, str[pickthree]);
	}
	else
	{
		strcat(string, " ");
		strcat(string, tag[0]);
		strcat(string, str[pickthree]);
	}


	strcat(string, "\n");

	free_string(room->description);
	room->description = str_dup(string);
	free_string(room->gen_description);
	room->gen_description = str_dup(string);
	procedurally_describe(NULL, NULL, room);
	if (strlen(room->gen_description) < 200)
		goto gotoisawesome;

	return;
}

//autogens hmarythstreet areas
void autogen_hmarythstreet(ROOM_INDEX_DATA *room)
{
	char string[MSL] = "";
	int MAX_PICK_ELEMENT = 5;

	static char * const tag[] = { "#", "#...", "#," };
	static char * const str[] = { "hmarythstreet", "hmarythfacades", "lmarythpalace", "hmarythsense", "lmarythwalls" };

	int conjunct;
	int pickone, picktwo, pickthree;

gotoisawesome:
	strcpy(string, "");
	conjunct = (number_range(1, 2) - 1);
	pickone = (number_range(1, MAX_PICK_ELEMENT) - 1);
	picktwo = (number_range(1, MAX_PICK_ELEMENT) - 1);
	pickthree = (number_range(1, MAX_PICK_ELEMENT) - 1);
	while (picktwo == pickone) picktwo = (number_range(1, MAX_PICK_ELEMENT) - 1);
	while (pickthree == pickone || pickthree == picktwo)
		pickthree = (number_range(1, MAX_PICK_ELEMENT) - 1);


	if (conjunct)
	{
		strcat(string, tag[1]);
		strcat(string, str[pickone]);
		strcat(string, ", and ");
		strcat(string, tag[2]);
		strcat(string, str[picktwo]);
		conjunct = 0;
	}
	else
	{
		strcat(string, tag[0]);
		strcat(string, str[pickone]);
		strcat(string, " ");
		strcat(string, tag[1]);
		strcat(string, str[picktwo]);
		conjunct = 1;
	}

	if (conjunct)
	{
		strcat(string, ", and ");
		strcat(string, tag[2]);
		strcat(string, str[pickthree]);
	}
	else
	{
		strcat(string, " ");
		strcat(string, tag[0]);
		strcat(string, str[pickthree]);
	}


	strcat(string, "\n");

	free_string(room->description);
	room->description = str_dup(string);
	free_string(room->gen_description);
	room->gen_description = str_dup(string);
	procedurally_describe(NULL, NULL, room);
	if (strlen(room->gen_description) < 200)
		goto gotoisawesome;

	return;
}


//contextually describes the direction of nearby water if there is any
//it occurs to me now that no water near may break this, let's revisit
char *water_direction_string(ROOM_INDEX_DATA *room)
{

	int a, b = 0;
	int dir_array[4] = { 0, 0, 0, 0 };

	for (a = 0; a<4; a++)
	{
		if (room->exit[a] && room->exit[a]->u1.to_room && is_sector_wet(room->exit[a]->u1.to_room->sector_type))
		{
			dir_array[a] = TRUE;
			b++;
		}
	}

	if (b == 1)
	{
		for (a = 0; a<4; a++)
		{
			if (dir_array[a] == TRUE)
				return dir_string_name[a];
		}
	}

	if (b == 2)
	{
		if (dir_array[DIR_NORTH] && dir_array[DIR_WEST])
			return "the north and west";

		if (dir_array[DIR_NORTH] && dir_array[DIR_EAST])
			return "the north and east";

		if (dir_array[DIR_NORTH] && dir_array[DIR_SOUTH])
			return "the north and south";

		if (dir_array[DIR_NORTH] && dir_array[DIR_WEST])
			return "the east and south";

		if (dir_array[DIR_NORTH] && dir_array[DIR_WEST])
			return "the east and west";

		if (dir_array[DIR_SOUTH] && dir_array[DIR_WEST])
			return "the south and west";
	}


	if (b == 3)
	{
		for (a = 0; a<4; a++)
		{
			if (dir_array[a] == FALSE)
			{
				if (dir_array[a] == DIR_NORTH)
					return "all directions but north";
				if (dir_array[a] == DIR_EAST)
					return "all directions but east";
				if (dir_array[a] == DIR_SOUTH)
					return "all directions but south";
				if (dir_array[a] == DIR_WEST)
					return "all directions but west";
			}
		}
	}

	return "all directions";
}

//describe the direction(s) that there are exits
char *exit_flag_direction_string(ROOM_INDEX_DATA *room, long flag)
{

	int a, b = 0;
	int dir_array[4] = { 0, 0, 0, 0 };

	for (a = 0; a<4; a++)
	{
		if (room->exit[a] && room->exit[a]->u1.to_room && IS_SET(room->exit[a]->exit_info, flag))
		{
			dir_array[a] = TRUE;
			b++;
		}
	}

	if (b == 0)
	{
		bugf("exit_flag_direction_string(): room \'%s\' has no \'%s\'", room->name, flag_string(exit_flags, flag));
	}

	if (b == 1)
	{
		for (a = 0; a<4; a++)
		{
			if (dir_array[a] == TRUE)
				return dir_string_name[a];
		}
	}

	if (b == 2)
	{
		if (dir_array[DIR_NORTH] && dir_array[DIR_WEST])
			return "the north and west";

		if (dir_array[DIR_NORTH] && dir_array[DIR_EAST])
			return "the north and east";

		if (dir_array[DIR_NORTH] && dir_array[DIR_SOUTH])
			return "the north and south";

		if (dir_array[DIR_NORTH] && dir_array[DIR_WEST])
			return "the east and south";

		if (dir_array[DIR_EAST] && dir_array[DIR_SOUTH])
			return "the east and west";

		if (dir_array[DIR_EAST] && dir_array[DIR_WEST])
			return "the south and west";

		if (dir_array[DIR_SOUTH] && dir_array[DIR_WEST])
			return "the south and west";

	}


	if (b == 3)
	{
		for (a = 0; a<4; a++)
		{
			if (dir_array[a] == FALSE)
			{
				switch (a)
				{
				default:
					break;
				case DIR_NORTH:
					return "all directions but north";
					break;
				case DIR_EAST:
					return "all directions but east";
					break;
				case DIR_SOUTH:
					return "all directions but south";
					break;
				case DIR_WEST:
					return "all directions but west";
					break;
				}
			}
		}
	}

	return "all directions";
}


//replace room tags with the relevant strings from the procgen pool
void procgen_string_replace_room(ROOM_INDEX_DATA *room)
{

	int x, roll, max;
	char search[MSL] = "", rand_buf[MSL], *pString;
	rand_buf[0] = '\0';

	for (x = 0; trait_table[x].name != NULL; x++)
	{

		sprintf(search, "#...%s", trait_table[x].name);
		if (strcasestr(room->gen_description, search) == NULL &&
			strcasestr(room->name, search) == NULL)
		{
			//if there is a [#,tag], we only pick options that can
			//follow a comma.
			sprintf(search, "#,%s", trait_table[x].name);
			if (strcasestr(room->gen_description, search) == NULL &&
				strcasestr(room->name, search) == NULL)
			{
				//if there is a [#...tag], we only pick options that can
				//be followed by a comma
				sprintf(search, "#%s", trait_table[x].name);
			}

		}

		//at this point we have no string, so we abort this
		//runthrough of the trait_table
		if (strcasestr(room->gen_description, search) == NULL &&
			strcasestr(room->name, search) == NULL) continue;

		do
		{
			//while loop executes nothing... just counts.......
			for (max = 1; trait_table[x].table[max].description != NULL; max++);//note this semicolon
			roll = number_range(1, max);

			//check for trait incidence
			if (number_percent() < trait_table[x].table[roll].incidence)
				continue;
			if (trait_table[x].table[roll].description != NULL)
				sprintf(rand_buf, "%s", trait_table[x].table[roll].description);

			//so we know this string will be accepted
			//let's format up for #... and #, tags.
			if (rand_buf[0] != '\0')
			{
				if (search[1] == '.')
				{
					//this tag needs to end w/ ambig
					if (has_lastpunct(rand_buf)) //i.e. it doesn't...
					{
						rand_buf[0] = '\0';
						continue;
					}

					rand_buf[0] = UPPER(rand_buf[0]);
				}
				else if (search[1] == ',')
				{
					//this tag needs to begin ambig
					if (rand_buf[0] == UPPER(rand_buf[0])) //i.e. it doesn't...
					{
						rand_buf[0] = '\0';
						continue;
					}

					if (has_lastpunct(rand_buf) == FALSE)
						;
					lastpunct(rand_buf);
				}
				else //let's assume this is a normal #tag
				{
					rand_buf[0] = UPPER(rand_buf[0]);
					if (has_lastpunct(rand_buf) == FALSE)
					{
						lastpunct(rand_buf);
					}
				}
			}

			//make sure the trait works
		} while (rand_buf[0] == '\0');

		bool firstsearch = TRUE;
		while (strcasestr(room->gen_description, search) != NULL ||
			strcasestr(room->name, search) != NULL)
		{
			if (firstsearch) set_procgen_room_values_by_tag(room, search, x, roll);
			room->gen_description =
				string_replace(room->gen_description, search, rand_buf);
			room->name =
				string_replace(room->name, search, rand_buf);
			firstsearch = FALSE;
		}

		rand_buf[0] = '\0';
	}


	//here come some special cases

	sprintf(search, "%s", "#waterdirection");
	while (strcasestr(room->gen_description, search) != NULL ||
		strcasestr(room->name, search))
	{
		room->gen_description =
			string_replace(room->gen_description, search, water_direction_string(room));
		room->name =
			string_replace(room->name, search, water_direction_string(room));
	}

	sprintf(search, "%s", "#declinedirection");
	while (strcasestr(room->gen_description, search) != NULL ||
		strcasestr(room->name, search))
	{
		room->gen_description =
			string_replace(room->gen_description, search, exit_flag_direction_string(room, EX_CLIMB_DOWN));
		room->name =
			string_replace(room->name, search, exit_flag_direction_string(room, EX_CLIMB_DOWN));
	}

	sprintf(search, "%s", "#inclinedirection");
	while (strcasestr(room->gen_description, search) != NULL ||
		strcasestr(room->name, search))
	{
		room->gen_description =
			string_replace(room->gen_description, search, exit_flag_direction_string(room, EX_CLIMB_UP));
		room->name =
			string_replace(room->name, search, exit_flag_direction_string(room, EX_CLIMB_UP));
	}

	sprintf(search, "%s", "#primarydirection");
	while (strcasestr(room->gen_description, search) != NULL ||
		strcasestr(room->name, search))
	{
		room->gen_description =
			string_replace(room->gen_description, search, exit_flag_direction_string(room, EX_PRIMARY));
		room->name =
			string_replace(room->name, search, exit_flag_direction_string(room, EX_PRIMARY));
	}

	sprintf(search, "%s", "#flowdirection");
	while (strcasestr(room->gen_description, search) != NULL ||
		strcasestr(room->name, search))
	{
		room->gen_description =
			string_replace(room->gen_description, search, exit_flag_direction_string(room, (EX_FLOW_SLOW | EX_FLOW_MEDIUM | EX_FLOW_FAST)));
		room->name =
			string_replace(room->name, search, exit_flag_direction_string(room, (EX_FLOW_SLOW | EX_FLOW_MEDIUM | EX_FLOW_FAST)));
	}


	sprintf(search, "%s", "#autogendforest");
	while (strcasestr(room->gen_description, search) != NULL)
	{
		autogen_dforest(room);
		return;
	}

	sprintf(search, "%s", "#autogenlmarythstreet");
	while (strcasestr(room->gen_description, search) != NULL)
	{
		autogen_lmarythstreet(room);
		return;
	}

	sprintf(search, "%s", "#autogenhmarythstreet");
	while (strcasestr(room->gen_description, search) != NULL)
	{
		autogen_hmarythstreet(room);
		return;
	}

	return;
}

//find and replace!  this will look for #tags and search through the procgen tables
//to find a relevant string to replace with, and it should only take ONE of the variables;
//i.e. either npc, obj, or room
void procgen_string_replace(CHAR_DATA *npc, OBJ_DATA *obj, ROOM_INDEX_DATA *room)
{

	if (npc != NULL)
	{
		procgen_string_replace_mobile(npc);
		return;
	}
	else if (obj != NULL)
	{
		procgen_string_replace_object(obj);
		return;
	}
	else if (room != NULL);
	{
		procgen_string_replace_room(room);
		return;
	}

}

//stringreplace for mobile procgen
void procgen_mobile(CHAR_DATA *npc)
{
	char *ptr;

	procgen_string_replace(npc, NULL, NULL);
	npc->description = format_string(npc->description);
	npc->long_descr = format_string(npc->long_descr);
	for (ptr = npc->long_descr; *ptr; ptr++)
		if (*ptr == '\n')
			*ptr = '\0';
	return;
}

//stringreplace for mobile objects
void procgen_object(OBJ_DATA *obj)
{

	if (IS_NULLSTR(obj->gen_description) ||
		IS_NULLSTR(obj->gen_long_descr) ||
		IS_NULLSTR(obj->gen_short_descr ||
		IS_NULLSTR(obj->gen_name))) return;

	if (!IS_NULLSTR(obj->description)) free_string(obj->description);
	if (!IS_NULLSTR(obj->short_descr)) free_string(obj->short_descr);
	if (!IS_NULLSTR(obj->long_descr)) free_string(obj->long_descr);
	if (!IS_NULLSTR(obj->name)) free_string(obj->name);

	obj->description = str_dup(obj->gen_description);
	obj->short_descr = str_dup(obj->gen_short_descr);
	obj->long_descr = str_dup(obj->gen_long_descr);
	obj->name = str_dup(obj->gen_name);

	char *ptr;
	procgen_string_replace(NULL, obj, NULL);

	return;
}

//stringreplace for rooms
void procgen_room(ROOM_INDEX_DATA *room)
{
	char *ptr;

	//here we copy from description to gen_description
	if (!IS_NULLSTR(room->gen_description))
		free_string(room->gen_description);

	room->gen_description = str_dup(room->description);
	procgen_string_replace(NULL, NULL, room);

	room->gen_description = format_string(room->gen_description);
	room->name = format_string(room->name);

	for (ptr = room->name; *ptr; ptr++)
		if (*ptr == '\n')
			*ptr = '\0';

	while (strcasestr(room->gen_description, "..") != NULL)
		room->gen_description =
		string_replace(room->gen_description, "..", ".");


	while (strcasestr(room->gen_description, "\n\n") != NULL)
		room->gen_description =
		string_replace(room->gen_description, "\n", "");


	return;
}

//create a geometrically square set of grid linked rooms automatically
//the last boolean requests whether or not this swath of terrain is connected
//westward to another set of vnums in the appropriate sequence, used to make
//large square areas as you continue eastward
void generate_swath(int vnum_start, int y_dimension, bool linkwest)
{
	ROOM_INDEX_DATA *room;
	int top, vnum, iHash;
	top = vnum_start + y_dimension;
	room = get_room_index(vnum_start);

	for (vnum = vnum_start; vnum < top; vnum++)
	{

		if (!get_room_index(vnum))
		{
			room = new_room_index();

			iHash = vnum % MAX_KEY_HASH;
			room->next = room_index_hash[iHash];
			room_index_hash[iHash] = room;

			room->area = get_vnum_area(vnum);
			room->vnum = vnum;
		}

		room->name = str_dup("THE MOTHAFUCKIN TEST ROOM");
		room->description = str_dup("It's a motherfucking TEST.");
		//		room->sector_type 	= SECT_MEDIUM_OCEAN;

		ROOM_INDEX_DATA *to_room;

		if (vnum != vnum_start)
		{
			to_room = get_room_index(vnum - 1);
			link_rooms(room, to_room, DIR_SOUTH);
		}

		if (linkwest)
		{
			to_room = get_room_index(vnum - y_dimension);
			link_rooms(room, to_room, DIR_WEST);
		}
	}

	return;
}

//character function for generating large square shapes of terrain
void do_autogen_shape(CHAR_DATA *ch, char *argument)
{
	int x, y;

	int start_vnum, x_dimension, y_dimension, current_vnum;
	char arg_x[MIL], arg_y[MIL];

	if (!ch->in_room) return;

	argument = one_argument(argument, arg_x);
	argument = one_argument(argument, arg_y);

	if (!*arg_x || !*arg_y || (!is_number(arg_x) || !is_number(arg_y)))
	{
		PTC(ch, "You must specify an X dimension and a Y dimension.\r\n");
		return;
	}

	x_dimension = atoi(arg_x);
	y_dimension = atoi(arg_y);

	start_vnum = ch->in_room->vnum;
	current_vnum = start_vnum;

	//add the area
	if ((start_vnum + (x_dimension * y_dimension)) > ch->in_room->area->max_vnum)
	{
		PTC(ch, "The area of that shape would exceed this zone's vnum limit.\r\n");
		return;
	}

	for (x = 0; x<x_dimension; x++)
	{
		generate_swath(current_vnum, y_dimension, (x>0));
		current_vnum += y_dimension;
	}


}


//make a pelt-type object for harvesting from the body of NPC animals
OBJ_DATA *generate_pelt(CHAR_DATA *mobile_test)
{
	char pelt_string[MSL];


	char noun[MSL], adjective[MSL] = "";

	if (!mobile_test->species || !str_cmp(mobile_test->species, "(no specie)"))
		strcpy(adjective, "");
	else	sprintf(adjective, "%s ", mobile_test->species);

	if (mobile_test->parts & PART_FUR)
		strcpy(noun, "fur");
	else if (mobile_test->parts & PART_SKIN)
		strcpy(noun, "skin");
	else
	{
		bugf("Make_pelt(): no part to create");
		return NULL;
	}


	if (mobile_test->furcondition && mobile_test->furpattern && mobile_test->color)
		sprintf(pelt_string, "a %s pelt of %s %s %s%s",
		mobile_test->furcondition,
		mobile_test->furpattern,
		mobile_test->color,
		adjective,
		noun);

	else if (mobile_test->color && mobile_test->furpattern)
		sprintf(pelt_string, "a pelt of %s %s %s%s",
		mobile_test->furpattern,
		mobile_test->color,
		adjective,
		noun);

	else if (mobile_test->color && mobile_test->furcondition)
		sprintf(pelt_string, "a %s pelt of %s %s%s",
		mobile_test->furcondition,
		mobile_test->color,
		adjective,
		noun);

	else if (mobile_test->color)
		sprintf(pelt_string, "a pelt of %s %s%s",
		mobile_test->color,
		adjective,
		noun);


	else
	{
		bugf("BUGGY!");
		sprintf(pelt_string, "a pelt of a %s",
			race_table[mobile_test->race].name);
	}


	OBJ_DATA *new_pelt;
	new_pelt = create_object(get_obj_index(OBJ_VNUM_GENERIC_REAGENT), 0);
	new_pelt->short_descr = str_dup(pelt_string);
	new_pelt->long_descr = str_dup(pelt_string);
	new_pelt->description = str_dup(pelt_string);
	new_pelt->name = str_dup(pelt_string);
	return new_pelt;

}



//rarity[0] represents the rarity total
//rarity[1] represents the total averagings
int calculate_rarity(int rarity_array[])
{
	double numerator, denominator, eval;

	numerator = rarity_array[0];
	denominator = rarity_array[1];
	eval = numerator / denominator;

	int val = (int)eval;
	eval = UMAX(1, val);

	return eval;
}

//bump up the rarity of an item by incremento
void increment_rarity(int rarity_array[], int incrementor)
{
	int r1, r2;
	rarity_array[0] += incrementor;
	rarity_array[0] = UMAX(1, rarity_array[0]);
	rarity_array[1]++;

	//no black holes
	rarity_array[1] = UMAX(1, rarity_array[1]);
}

//find what kind of animal part we're dealing with
int get_part_material(int part)
{

	if (part == PART_CLAWS ||
		part == PART_ANTLERS ||
		part == PART_HORNS ||
		part == PART_DOUBLEHORN ||
		part == PART_TUSKS ||
		part == PART_SCALES)
		return MATERIAL_BONE;

	return MATERIAL_RAW_MEAT;
}

//set the weight of an animal part according to the animal's size
//this is going to stop bears and birds from having a part with the
//same weight
void set_part_weight(OBJ_DATA *part, OBJ_DATA *corpse, int bit)
{
	double decrement = get_physics_obj_weight(corpse);
	if (bit == PART_EYE ||
		bit == PART_FINGERS ||
		bit == PART_EYE ||
		bit == PART_TONGUE ||
		bit == PART_CLAWS ||
		bit == PART_FANGS ||
		bit == PART_SCALES)
	{
		decrement *= .05;
	}
	else if (bit == PART_SCALES || bit == PART_FEATHERS)
	{
		decrement *= .005;
	}
	else decrement *= .1;

	decrement = UMAX(.01, decrement);
	arbitrary_decrement_weight(corpse, decrement);
	part->material[get_part_material(bit)] = decrement;
	return;
}

//somewhat self explanatory
OBJ_DATA *generate_corpse_piece(CHAR_DATA *dying_npc, OBJ_DATA *corpse, int table_entry)
{
	OBJ_DATA *corpse_piece;

	//special cases
	if (part_flags[table_entry].bit == PART_SKIN)
	{
		corpse_piece = generate_pelt(dying_npc);
	}
	//elseif (other special condition parts)
	//{
	//}
	else
	{

		char noun[MSL];
		//normal cases
		if (!dying_npc->species || !str_cmp(dying_npc->species, "(no specie)"))
			sprintf(noun, "%s", race_table[dying_npc->race].name);
		else sprintf(noun, "%s", dying_npc->species);

		char object_short_desc[MSL];
		sprintf(object_short_desc, "the %s of a %s",
			part_flags[table_entry].name,
			noun);
		char object_long_desc[MSL];
		sprintf(object_long_desc, "The %s of a %s %s here.",
			part_flags[table_entry].name,
			noun,
			//cover the guts plural... bit of a hack but whatever
			((part_flags[table_entry].bit == PART_GUTS ||
			part_flags[table_entry].bit == PART_FEATHERS) ?
			"are" : "is"));

		corpse_piece = create_object(get_obj_index(OBJ_VNUM_GENERIC_REAGENT), 0);
		corpse_piece->name = str_dup(object_short_desc);
		corpse_piece->short_descr = str_dup(object_short_desc);
		corpse_piece->long_descr = str_dup(object_long_desc);
		corpse_piece->description = str_dup(object_long_desc);
	}

	SET_BIT(corpse_piece->extra_flags, ITEM_VIS_DEATH);
	corpse_piece->value[0] = REAGENT_BODY_PART;
	corpse_piece->value[1] = part_flags[table_entry].bit;
	corpse_piece->condition = 100;
	corpse_piece->rarity[0] = dying_npc->rarity[0];
	corpse_piece->rarity[1] = dying_npc->rarity[1];//denom
	if (corpse) set_part_weight(corpse_piece, corpse, part_flags[table_entry].bit);
	return corpse_piece;
}

//how many legs?
int count_legs(CHAR_DATA *victim)
{
	if (victim->form & FORM_BIPED)
		return 2;

	if (victim->form & FORM_QUADRAPED)
		return 4;

	return 0;
}

//how many arms?
int count_arms(CHAR_DATA *victim)
{
	if (count_legs(victim) == 2)
		return 2;

	return 0;
}

//feet?
int count_feet(CHAR_DATA *victim)
{
	return count_legs(victim);
}

//hands?
int count_hands(CHAR_DATA *victim)
{
	return count_arms(victim);
}

//fingers?
int count_fingers(CHAR_DATA *victim)
{
	return count_hands(victim);
}

//eyes?
int count_eyes(CHAR_DATA *victim)
{
	return 2;
}

//tentacleS?
int count_tentacles(CHAR_DATA *victim)
{
	//let's just assume it's an octopus until this is relevant
	return 8;
}

//fins?
int count_fins(CHAR_DATA *victim)
{
	//two and a dorsal..?  until it's relevant
	return 3;
}

//wings?
int count_wings(CHAR_DATA *victim)
{
	//two wings until butterflies or whatever are relevant
	return 2;
}

int count_claws(CHAR_DATA *victim)
{
	return (4 * count_legs(victim));
}

//fangs?
int count_fangs(CHAR_DATA *victim)
{
	//cats generate more?  I don't know
	return 2;
}

//scales..
int count_scales(CHAR_DATA *victim)
{
	return 2 * victim->size;
}


//find the number of pieces a corpse should have; i.e., a bird will drop
//2 legs but a wolf will drop 4 legs
int get_corpse_piece_number(CHAR_DATA *victim, int bit)
{
	int number = 1;

	if (bit == PART_EAR) number = 2;
	if (bit == PART_TUSKS) number = 2;
	if (bit == PART_ARMS) number = 2;
	if (bit == PART_QUILL) number = number_range(1, 5);

	if (bit == PART_HORNS && (victim->parts & PART_DOUBLEHORN))
		number = 2;

	if (bit == PART_LEGS) number = count_arms(victim);
	if (bit == PART_HANDS) number = count_hands(victim);
	if (bit == PART_FEET) number = count_feet(victim);
	if (bit == PART_FINGERS) number = count_fingers(victim);
	if (bit == PART_EYE) count_eyes(victim);
	if (bit == PART_TENTACLES) number = count_tentacles(victim);
	if (bit == PART_FINS) number = count_fins(victim);
	if (bit == PART_WINGS) number = count_wings(victim);
	if (bit == PART_CLAWS) number = count_claws(victim);
	if (bit == PART_FANGS) number = count_fangs(victim);
	if (bit == PART_SCALES) number = count_scales(victim);

	return number;
}

//initialize the pieces of the corpse to be harvested
void populate_corpse_pieces(CHAR_DATA *dying_npc, OBJ_DATA *corpse)
{
	OBJ_DATA *piece;
	int x, repeat, multiples;

	for (x = 0; part_flags[x].name != NULL; x++)
	{
		if (part_flags[x].settable == FALSE) continue;

		//figure out how many to load.  0 for special loads.
		multiples = get_corpse_piece_number(dying_npc, part_flags[x].bit);
		if (dying_npc->parts & part_flags[x].bit)
		{
			for (repeat = 1; repeat <= multiples; repeat++)
			{
				piece = NULL;
				piece = generate_corpse_piece(dying_npc, corpse, x);
				if (piece) obj_to_obj(piece, corpse);
			}
		}
	}

	return;
}

//autogenerate the template for a dforest directional room, which will
//pay attention to the pre-defined creator-defined topography... i.e.
//when you set elevation, it will describe the room accordingly
char *generate_dforest_directional_template(ROOM_INDEX_DATA *room)
{
	int a;
	free_string(room->name);
	room->name = str_dup("Within a Verdant Deciduous Forest");
	for (a = 0; a<6; a++)
	{
		if (room->sector_type == SECT_SHALLOW_RIVER)
		{
			free_string(room->name);
			room->name = str_dup("A Slow, Wide Stream");
			return "#autogendforeststream";
		}

		if (room->sector_type == SECT_MEDIUM_RIVER)
		{
			free_string(room->name);
			room->name = str_dup("A Rushing River, Deep and Narrow");
			return "#autogendforestriver";
		}

		if (room->exit[a] && room->exit[a]->u1.to_room->sector_type == SECT_SHALLOW_RIVER)
		{
			free_string(room->name);
			room->name = str_dup("Near the Bank of a Slow, Wide Stream");
			return "#autogendforeststreambank";
		}

		if (room->exit[a] && room->exit[a]->u1.to_room->sector_type == SECT_MEDIUM_RIVER)
		{
			free_string(room->name);
			room->name = str_dup("Near the Bank of a Narrow, Quick River");
			return "#autogendforestriverbank";
		}

		if (room->exit[a] &&
			room->exit[a]->u1.to_room &&
			!is_sector_wet(room->exit[a]->u1.to_room->sector_type) &&
			(room->exit[a]->exit_info & EX_CLIMB_DOWN))
		{
			free_string(room->name);
			room->name = str_dup("Above A Steeply-Banked, Dry Ravine");
			return "#autogendforestravinebank";
		}

		if (room->exit[a] &&
			room->exit[a]->u1.to_room &&
			!is_sector_wet(room->sector_type) &&
			(room->exit[a]->exit_info & EX_CLIMB_UP))
		{
			free_string(room->name);
			room->name = str_dup("The Bottom of A Steeply-Banked, Dry Ravine");
			return "#autogendforestravine";
		}
	}

	return "#autogendforest";
}


const   struct  spawn_table_type      sforestb_spawn_table[] =
{
	{ 1000001, "sforestb", 2500, -1 },
	{ -1, NULL, 0, -1 },
};



//spawntable index
const	struct	spawn_index_type	spawn_index_table[] =
{
	//to add more: tables.c entry, tables.h entry, save_prototypes.c set_values() entries
	{ demo_spawn_table },
	{ sforestb_spawn_table },
	{ NULL }
};


//pretty simple
int count_area_mobiles(int vnum, AREA_DATA *pArea)
{
	CHAR_DATA *list = NULL;
	int counter = 0;

	for (list = char_list; list; list = list->next)
	{
		if (!IS_NPC(list)
			|| (list->pIndexData->vnum != vnum)
			|| (!list->in_room)
			|| (!list->in_room->area))
			continue;

		if (list->in_room->area == pArea)
			counter++;
	}

	return counter;
}

bool area_has_one_room(AREA_DATA *pArea)
{
	int x = 0, uvnum = 0, lvnum = 0;

	uvnum = pArea->max_vnum;
	lvnum = pArea->min_vnum;

	for (x = lvnum; x <= uvnum; x++)
	{
		if (get_room_index(x))
			return TRUE;
	}

	return FALSE;
}

ROOM_INDEX_DATA *get_random_area_room(AREA_DATA *pArea)
{
	int uvnum = 0, lvnum = 0, random = 0;
	ROOM_INDEX_DATA *room;

	if (!area_has_one_room(pArea))
		return NULL;

	uvnum = pArea->max_vnum;
	lvnum = pArea->min_vnum;

	while (1)
	{
		random = number_range(lvnum, uvnum);
		if ((room = get_room_index(random)))
			return room;
	}

	bugf("Get_random_area_room(): no room found");
	return NULL;
}

//string searches by name for an area
AREA_DATA *get_area_by_name(char *argument)
{
	AREA_DATA *pArea;

	//iterate through the areas
	for (pArea = area_first; pArea; pArea = pArea->next)
	{
		if (!str_cmp(argument, pArea->name))
			return pArea;
	}

	return NULL;
}

//this function is going to do maintenance on spawn states, making sure
//not too many or too few mobiles have been added/lost to an area.  It will
//do the respawning in the case there is a deficit, according to the area's
//spawntable
void maintain_area_spawn_states(AREA_DATA *pArea)
{

	//TESTING -- TESTING -- TESTING
	//MOB_AI
	return;

	MOB_INDEX_DATA *pSpawn;
	CHAR_DATA *char_load;
	ROOM_INDEX_DATA *spawn_room;
	int vnum = 0, x = 0, y = 0;

	for (x = 0; spawn_index_table[x].spawn_table != NULL; x++)
	{
		for (y = 0; spawn_index_table[x].spawn_table[y].vnum != -1; y++)
		{
			//specific room case.  not sure if i want to support this here.
			if (is_number(spawn_index_table[x].spawn_table[y].location))
				continue;

			//if it's not the area we're looking for, move on
			if (str_cmp(pArea->name, spawn_index_table[x].spawn_table[y].location))
				continue;

			spawn_room = get_random_area_room(pArea);

			if (!(pSpawn = get_mob_index(spawn_index_table[x].spawn_table[y].vnum)))
			{
				bugf("Maintain_area_spawns(): null mobile prototype");
				continue;
			}

			if (count_area_mobiles(pSpawn->vnum, pArea) >=
				spawn_index_table[x].spawn_table[y].max)
				continue;

			while (count_area_mobiles(pSpawn->vnum, pArea) <
				spawn_index_table[x].spawn_table[y].max)
			{
				if (spawn_index_table[x].spawn_table[y].sector == -1)
					spawn_room = get_random_area_room(pArea);
				else do { spawn_room = get_random_area_room(pArea); } while (spawn_room->sector_type !=
					spawn_index_table[x].spawn_table[y].sector);

				char_load = create_mobile(pSpawn);
				char_to_room(char_load, spawn_room);
			}
		}
	}

	CHAR_DATA *charlist, *charbuf;
	charlist = char_list;
	if (!charlist) return;
	do {
		if (charlist) charbuf = charlist->next;
		else break;

		if (!charlist->in_room ||
			charlist->in_room->area != pArea ||
			!IS_NPC(charlist))
		{
			charlist = charbuf;
			continue;
		}

		contextualize_mobile(charlist);
		charlist = charbuf;

	} while (charlist);

	return;
}

//little admin tool for seeing what has been randomly spawned into the area NPCwise
//it will display some metrics to the user when called
void display_area_spawn_states(CHAR_DATA *ch, AREA_DATA *pArea)
{

	CHAR_DATA *list = NULL;
	int counter = 0;

	for (list = char_list; list; list = list->next)
		if (IS_NPC(list))
			list->pIndexData->display_dirty = FALSE;

	for (list = char_list; list; list = list->next)
	{
		if (!IS_NPC(list)
			|| (!list->in_room)
			|| (!list->in_room->area)
			|| (list->in_room->area != pArea))
			continue;

		list->pIndexData->display_dirty = TRUE;
	}

	for (list = char_list; list; list = list->next)
	{
		if (!IS_NPC(list)
			|| (!list->in_room)
			|| (!list->in_room->area)
			|| (list->in_room->area != pArea))
			continue;

		counter = count_area_mobiles(list->pIndexData->vnum, pArea);

		if (list->pIndexData->display_dirty)
			PTC(ch, "[%d] %s (%d)\r\n",
			counter,
			list->pIndexData->short_descr,
			list->pIndexData->vnum);
		list->pIndexData->display_dirty = FALSE;
	}

	return;
}

//initialize the procgen states from the spawntables
void load_initial_spawn_states(void)
{
	AREA_DATA *pArea;

	//iterate through the areas
	for (pArea = area_first; pArea; pArea = pArea->next)
	{
		maintain_area_spawn_states(pArea);
	}

	log_f("Loaded... initial mobile spawns");

	return;
}

//load a mob with the burrowed/hidden state rather than default
CHAR_DATA *load_burrowed_forest_mob(OBJ_DATA *burrow, bool danger)
{
	int roll;
	CHAR_DATA *mob = NULL;

	if (!danger)
	{
		roll = number_range(1, 4);
		switch (roll)
		{
		default:
			bugf("load_burrowed_forest_mob()... what?");
			return NULL;



		case 1:	mob = create_mobile(get_mob_index(MOB_VNUM_DFOREST_FOX));
			break;
		case 2: mob = create_mobile(get_mob_index(MOB_VNUM_DFOREST_SQUIRREL));
			break;
		case 3:
			if (number_range(1, 2) == 1)
				mob = create_mobile(get_mob_index(MOB_VNUM_DFOREST_RABBIT_LONG_HAIR));
			else	mob = create_mobile(get_mob_index(MOB_VNUM_DFOREST_RABBIT_ANGORA));
			break;
		case 4: mob = create_mobile(get_mob_index(MOB_VNUM_DFOREST_BADGER));
			break;
		}
	}
	else
	{
		roll = number_range(1, 5);
		switch (roll)
		{
		default:
			bugf("load_burrowed_forest_mob()... whaaaat?");
			return NULL;


		case 1:	mob = create_mobile(get_mob_index(MOB_VNUM_DFOREST_FOX));
			break;
		case 2: mob = create_mobile(get_mob_index(MOB_VNUM_DFOREST_WEASEL));
			break;
		case 3: mob = create_mobile(get_mob_index(MOB_VNUM_DFOREST_GOPHER));
			break;
		case 4: mob = create_mobile(get_mob_index(MOB_VNUM_DFOREST_PORCUPINE));
			break;
		case 5: mob = create_mobile(get_mob_index(MOB_VNUM_DFOREST_BEAR));
			break;
		}
	}

	return mob;
}

//tree!  explains itself I think
void load_tree(ROOM_INDEX_DATA *room, int type)
{
	OBJ_DATA *tree;
	int x;
	char tree_string[MSL];

	for (x = 0; tree_table[x].name != NULL; x++)
	{
		if (tree_table[x].type == type)
		{
			tree = create_object(get_obj_index(OBJ_VNUM_GENERIC_PLANT), 0);
			tree->value[0] = PLANT_TREE;
			tree->value[1] = type;
			tree->material[MATERIAL_WOOD] = tree_table[x].weight;
			//			SET_BIT(tree->extra_flags,ITEM_EPHEMERAL);

			sprintf(tree_string, "stand #size %s trees", tree_table[x].name);
			tree->name = str_dup(tree_string);
			sprintf(tree_string, "a stand of #size %s trees", tree_table[x].name);
			tree->short_descr = str_dup(tree_string);
			sprintf(tree_string, "A stand of #size %s trees grows here.", tree_table[x].name);
			tree->long_descr = str_dup(tree_string);
			sprintf(tree_string, "A stand of %s trees grows here, most of which have grown to a #heightadjective size.  Upon each of the trees grow %s.", tree_table[x].name, tree_table[x].leaf_desc);
			tree->description = str_dup(tree_string);
			obj_to_room(tree, room);
			procedurally_describe(NULL, tree, NULL);
			return;
		}
	}

	return;
}


//for special circumstances, some randomly-generated values must be accompanied by 
//gamestate adjustments.  this function will do so for rooms, based on the key
//which is actually the seed that the procgen is using for the field
void initiate_special_procgen_condition(ROOM_INDEX_DATA *room, int key)
{
	OBJ_DATA *oload = NULL;
	CHAR_DATA *mload = NULL;
	int vnum = 0;
	int mushrooms[] = { OBJ_VNUM_DFORESTTRUFFLEMUSHROOM,
		OBJ_VNUM_DFORESTPORTABELLAMUSHROOM,
		OBJ_VNUM_DFORESTMORELMUSHROOM,
		OBJ_VNUM_DFORESTDEATHMUSHROOM,
		OBJ_VNUM_DFORESTPOISONMUSHROOM };

	switch (key)
	{
	default:
		bugf("Initiate_special_procgen_condition(): bad key");
		return;


		/*
		-- Likely these need to be made into fruit so they can be gathered

		case SPAWN_DdemoPPLE:
		load_tree(room, TREE_APPLE);
		break;

		case SPAWN_DFORESTWILLOW:
		load_tree(room, TREE_WILLOW);
		break;*/

	case SPAWN_DFORESTYEW:
		load_tree(room, TREE_YEW);
		break;

	case SPAWN_DFORESTREDMAPLE:
		load_tree(room, TREE_RED_MAPLE);
		break;

	case SPAWN_DFORESTBIRCH:
		load_tree(room, TREE_BIRCH);
		break;

	case SPAWN_DFORESTOAK:
		load_tree(room, ((number_range(1, 2) == 2) ? TREE_WHITE_OAK : TREE_RED_OAK));
		break;

	case SPAWN_DFORESTLARGEBURROW:
		oload = create_object(get_obj_index(OBJ_VNUM_DFORESTLARGEBURROW), 0);
		obj_to_room(oload, room);
		char_to_room(load_burrowed_forest_mob(oload, TRUE), room);
		break;

	case SPAWN_DFORESTSMALLBURROW:
		oload = create_object(get_obj_index(OBJ_VNUM_DFORESTSMALLBURROW), 0);
		obj_to_room(oload, room);
		char_to_room(load_burrowed_forest_mob(oload, FALSE), room);
		break;

	case SPAWN_DFORESTPOISONMUSHROOM:
		oload = create_object(get_obj_index(OBJ_VNUM_DFORESTPOISONMUSHROOM), 0);
		obj_to_room(oload, room);
		break;
	case SPAWN_DFORESTDEATHMUSHROOM:
		oload = create_object(get_obj_index(OBJ_VNUM_DFORESTDEATHMUSHROOM), 0);
		obj_to_room(oload, room);
		break;
	case SPAWN_DFORESTMORELMUSHROOM:
		oload = create_object(get_obj_index(OBJ_VNUM_DFORESTMORELMUSHROOM), 0);
		obj_to_room(oload, room);
		break;
	case SPAWN_DFORESTPORTABELLAMUSHROOM:
		oload = create_object(get_obj_index(OBJ_VNUM_DFORESTPORTABELLAMUSHROOM), 0);
		obj_to_room(oload, room);
		break;
	case SPAWN_DFORESTTRUFFLEMUSHROOM:
		oload = create_object(get_obj_index(OBJ_VNUM_DFORESTTRUFFLEMUSHROOM), 0);
		obj_to_room(oload, room);
		break;
	case SPAWN_DFORESTRANDOMMUSHROOM:
		oload = create_object(get_obj_index(mushrooms[number_range(0, 4)]), 0);
		obj_to_room(oload, room);
		break;

	case SPAWN_DFORESTSTRAWBERRY:
		oload = create_object(get_obj_index(OBJ_VNUM_DFORESTSTRAWBERRY), 0);
		obj_to_room(oload, room);
		break;
	case SPAWN_DFORESTRASPBERRY:
		oload = create_object(get_obj_index(OBJ_VNUM_DFORESTRASPBERRY), 0);
		obj_to_room(oload, room);
		break;
	case SPAWN_DFORESTBLACKBERRY:
		oload = create_object(get_obj_index(OBJ_VNUM_DFORESTBLACKBERRY), 0);
		obj_to_room(oload, room);
		break;
	case SPAWN_DFORESTBLUEBERRY:
		oload = create_object(get_obj_index(OBJ_VNUM_DFORESTBLUEBERRY), 0);
		obj_to_room(oload, room);
		break;
	case SPAWN_DFORESTGRAPES:
		oload = create_object(get_obj_index(OBJ_VNUM_DFORESTGRAPES), 0);
		obj_to_room(oload, room);
		break;
	case SPAWN_DFORESTWHITEVINE:
		oload = create_object(get_obj_index(OBJ_VNUM_DFORESTWHITEVINE), 0);
		obj_to_room(oload, room);
		break;
	case SPAWN_DFORESTDANDELION:
		oload = create_object(get_obj_index(OBJ_VNUM_DFORESTDANDELION), 0);
		obj_to_room(oload, room);
		break;
	case SPAWN_DFORESTROSEBUSH:
		oload = create_object(get_obj_index(OBJ_VNUM_DFORESTROSEBUSH), 0);
		obj_to_room(oload, room);
		break;
	case SPAWN_DFORESTMARIJUANA:
		oload = create_object(get_obj_index(OBJ_VNUM_DFORESTMARIJUANA), 0);
		obj_to_room(oload, room);
		break;
	case SPAWN_DFORESTTOBACCO:
		oload = create_object(get_obj_index(OBJ_VNUM_DFORESTTOBACCO), 0);
		obj_to_room(oload, room);
		break;
	case SPAWN_DFORESTKINGSFOIL:
		oload = create_object(get_obj_index(OBJ_VNUM_DFORESTKINGSFOIL), 0);
		obj_to_room(oload, room);
		break;
		/*		case SPAWN_DFORESTFIREFLOWER:
		oload = create_object(get_obj_index(OBJ_VNUM_DFORESTFIREFLOWER),0);
		obj_to_room(oload,room);
		break;
		case SPAWN_DFORESTWATERFLOWER:
		oload = create_object(get_obj_index(OBJ_VNUM_DFORESTWATERFLOWER),0);
		obj_to_room(oload,room);
		break;
		case SPAWN_DdemoIRFLOWER:
		oload = create_object(get_obj_index(OBJ_VNUM_DdemoIRFLOWER),0);
		obj_to_room(oload,room);
		break;
		case SPAWN_DFORESTEARTHFLOWER:
		oload = create_object(get_obj_index(OBJ_VNUM_DFORESTEARTHFLOWER),0);
		obj_to_room(oload,room);
		break;
		case SPAWN_DFORESTMAGICFLOWER:
		oload = create_object(get_obj_index(OBJ_VNUM_DFORESTMAGICFLOWER),0);
		obj_to_room(oload,room);
		break;*/
	}

	if (oload && oload->in_room)
		contextualize_object(oload);
	//leave the mobile contextualization for later because
	//of the way we're iterating over lists and such

	return;
}


//obvious enough
void flower_describe(OBJ_DATA *flower, OBJ_DATA *plant)
{

	char noun_string[MSL], desc_string[MSL];
	sprintf(noun_string, "a %s %s",
		((plant->color == NULL) ? "" : plant->color),
		flower_table[flower->value[1]].name);
	generic_describe(flower, noun_string);

	sprintf(desc_string, "%s %s %s",
		flower->description,
		flower_table[flower->value[1]].leaf_desc,
		flower_table[flower->value[1]].wood_desc);
	free_string(flower->description);
	flower->description = str_dup(desc_string);

	return;
}

//places an affect on an object that will regrow when expired
void regrow_obj(OBJ_DATA *plant)
{

	AFFECT_DATA paf;
	paf.where = TO_OBJECT;
	paf.location = APPLY_NONE;
	paf.type = gsn_growing;
	paf.duration = plant->value[4];
	paf.resolution = PULSE_TICK;
	paf.bitvector = 0;
	paf.stacks = 0;
	paf.modifier = 1;

	return;
}

//check if a plant object is in the regrowth state
bool is_growing(OBJ_DATA *obj)
{
	AFFECT_DATA *paf = affect_find(obj->affected, gsn_growing);

	if (!paf) return FALSE;

	return TRUE;
}

//directly eat from plant (presumably for wild/tamed animals mostly)
OBJ_DATA *eat_from_plant(CHAR_DATA *ch, OBJ_DATA *plant)
{

	OBJ_DATA *fruit = NULL;
	int vnum;

	switch (plant->value[0])
	{
	default:
		bugf("Gather_from_plant(): bad plant type");
		return NULL;
	case PLANT_FRUIT:
		vnum = plant->value[1];

		if (!get_obj_index(vnum))
			return NULL;

		fruit = create_object(get_obj_index(vnum), 0);
		regrow_obj(plant);
		SET_BIT(fruit->wear_flags, (ITEM_TAKE | WEAR_HOLD));
		break;

	case PLANT_HERB:
		vnum = plant->value[1];

		if (!get_obj_index(vnum))
			return NULL;

		fruit = create_object(get_obj_index(vnum), 0);
		SET_BIT(fruit->wear_flags, (ITEM_TAKE | WEAR_HOLD));
		regrow_obj(plant);
		SET_BIT(plant->extra_flags, ITEM_STAFFINVIS);
		break;

	case PLANT_MUSHROOM:
		fruit = create_object(get_obj_index(OBJ_VNUM_GENERIC_FOOD), 0);
		SET_BIT(fruit->wear_flags, (ITEM_TAKE | WEAR_HOLD));
		default_describe(fruit, plant->pIndexData);
		//regrow_obj uses its short description
		regrow_obj(plant);
		generic_describe(plant, "a mushroom stalk");
		SET_BIT(plant->value[3], PLANT_CAN_GATHER);
		break;

	case PLANT_FLOWER:
		fruit = create_object(plant->pIndexData, 0);
		clone_object(plant, fruit);
		flower_describe(fruit, plant);
		SET_BIT(fruit->wear_flags, (ITEM_TAKE | WEAR_HOLD));

		regrow_obj(plant);
		SET_BIT(plant->extra_flags, ITEM_STAFFINVIS);
		break;

	}

	return fruit;
}

//if harvestable, gather from the plant
OBJ_DATA *gather_from_plant(CHAR_DATA *ch, OBJ_DATA *plant)
{

	OBJ_DATA *fruit = NULL;
	int vnum;

	switch (plant->value[0])
	{
	default:
		bugf("Gather_from_plant(): bad plant type");
		return NULL;
	case PLANT_FRUIT:
		vnum = plant->value[1];

		if (!get_obj_index(vnum))
			return NULL;

		fruit = create_object(get_obj_index(vnum), 0);
		SET_BIT(fruit->wear_flags, (ITEM_TAKE | WEAR_HOLD));
		regrow_obj(plant);
		act("You gather $p from $P.", ch, fruit, plant, TO_CHAR);
		act("$n gathers $p from $P.", ch, fruit, plant, TO_ROOM);
		break;

	case PLANT_HERB:
		vnum = plant->value[1];

		if (!get_obj_index(vnum))
			return NULL;

		fruit = create_object(get_obj_index(vnum), 0);
		SET_BIT(fruit->wear_flags, (ITEM_TAKE | WEAR_HOLD));
		act("You gather $p from $P.", ch, fruit, plant, TO_CHAR);
		act("$n gathers $p from $P.", ch, fruit, plant, TO_ROOM);
		regrow_obj(plant);
		SET_BIT(plant->extra_flags, ITEM_STAFFINVIS);
		break;

	case PLANT_MUSHROOM:
		fruit = create_object(get_obj_index(OBJ_VNUM_GENERIC_FOOD), 0);
		SET_BIT(fruit->wear_flags, (ITEM_TAKE | WEAR_HOLD));
		default_describe(fruit, plant->pIndexData);
		//regrow_obj uses its short description
		regrow_obj(plant);
		generic_describe(plant, "a mushroom stalk");
		act("You gather $p.", ch, fruit, NULL, TO_CHAR);
		act("$n gathers $p.", ch, fruit, NULL, TO_ROOM);
		break;

	case PLANT_FLOWER:
		fruit = create_object(plant->pIndexData, 0);
		clone_object(plant, fruit);
		flower_describe(fruit, plant);
		SET_BIT(fruit->wear_flags, (ITEM_TAKE | WEAR_HOLD));

		act("You pick $p.", ch, fruit, plant, TO_CHAR);
		act("$n pick $p.", ch, fruit, plant, TO_ROOM);
		regrow_obj(plant);
		SET_BIT(plant->extra_flags, ITEM_STAFFINVIS);
		break;

	}

	return fruit;
}


//set context-relevant qualities (post generation of other random qualities) for mobiles
void contextualize_mobile(CHAR_DATA *mob)
{

	ROOM_INDEX_DATA *in_room = mob->in_room;

	if (!in_room) return;

	if (!ai_habitates(mob, in_room))
	{
		extract_char(mob, TRUE);
	}

	return;
}

//set context-relevant qualities for objects after their setting has been proc'gend
void contextualize_object(OBJ_DATA *obj)
{
	ROOM_INDEX_DATA *in_room = obj->in_room;

	if (!in_room) return;

	switch (obj->item_type)
	{
	default:
		break;

	case ITEM_PLANT:
		if (is_sector_wet(in_room->sector_type))
		{
			clear_obj_pose(obj);
			obj->pose = str_dup("has taken root in one of the banks.");
		}

		break;
	}
	return;
}

//initialize pelt according to some stuff
void apply_random_pelt_property(OBJ_DATA *pelt)
{
	AFFECT_DATA paf;
	paf.where = TO_OBJECT;
	paf.location = APPLY_NONE;
	paf.type = -1;
	paf.duration = -1;
	paf.resolution = PULSE_TICK;
	paf.bitvector = 0;
	paf.stacks = 0;
	paf.modifier = 1;

	int ac_array[] = { DAM_SLASH, DAM_PIERCE, DAM_PIERCE, DAM_BASH, DAM_BASH, DAM_BASH };

	switch (number_range(1, 5))
	{
	default: break;

	case 1:
		paf.location = APPLY_AC;
		paf.effect_type = ac_array[number_range(0, 5)];
		paf.modifier = 1;
		break;
	case 2: paf.location = APPLY_AC;
		paf.effect_type = DAM_COLD;
		paf.modifier = 1;
		break;
	case 3: paf.location = APPLY_HITROLL;
		paf.modifier = 1;
		break;
	case 4: paf.location = APPLY_DAMROLL;
		paf.modifier = 1;
		break;
	case 5: paf.location = APPLY_SAVES;
		paf.modifier = 1;
		break;
		//ability bonuses?
	}

	affect_to_obj(pelt, &paf);
	return;
}

//generate qualities for the pelt that was initialized
void generate_random_pelt_properties(CHAR_DATA *ch, OBJ_DATA *pelt)
{

	int sn = gsn_butchery;
	double weight = get_physics_obj_weight(pelt);
	double chance;

	int x, cap = (int)weight;

	for (x = 0; x <= cap; x++)
	{
		chance = pelt->condition *
			(calculate_rarity(pelt->rarity) / 100) *
			ability_coefficient(ch, sn, 16);

		if (chance < number_percent()) continue;

		apply_random_pelt_property(pelt);
	}

	return;
}



//for the demo area, this is going to randomly generate an animal
void make_tutorial_animal(DESCRIPTOR_DATA *d)
{
	PC_DATA *pcdata = d->character->pcdata;
	char *string = str_dup(d->character->name);

	d->character = NULL;
	d->character = create_mobile(get_mob_index(number_range(1000001, 1000029)));
	CHAR_DATA *ch = d->character;
	ch->pcdata = pcdata;

	ch->pIndexData = NULL;
	ch->name = string;
	ch->desc = d;
	d->connected = CON_PLAYING;

	REMOVE_BIT(ch->act, ACT_IS_NPC);
	ch->hit = ch->max_hit;
	ch->mana = ch->max_mana;
	ch->move = ch->max_move;
	ch->comm = COMM_COMBINE
		| COMM_PROMPT;
	ch->prompt = str_dup("{c> %h health %v move{x ");
	SET_BIT(ch->act, PLR_AUTOEXIT);
	SET_BIT(ch->act, PLR_AUTOGOLD);
	SET_BIT(ch->yazfu_flags, YAZFU_BASIC);

	ROOM_INDEX_DATA *load_room;
	do
	{
		load_room = get_random_area_room(get_area_by_name("demo"));
	} while (!load_room || is_sector_wet(load_room->sector_type));

	char_to_room(ch, get_random_area_room(get_area_by_name("demo")));

	AFFECT_DATA paf;
	paf.where = TO_AFFECTS;
	paf.location = APPLY_NONE;
	paf.type = gsn_demo_begun;
	paf.resolution = 1;
	paf.duration = -1;
	paf.boolean = FALSE;
	paf.skipshow = TRUE;
	paf.stacks = 0;
	affect_overwrite_char(ch, &paf);
	check_tutorial_status(ch, 0);
	ch->condition[COND_HUNGER] = 100;
	ch->condition[COND_THIRST] = 0;


	return;
}


//returns the coefficient representing the difference between a generated animal's size
//and the size of a default animal that had been generated without random variance
double calculate_size_differential(CHAR_DATA *ch)
{
	double size_mobile, weight_mobile, size_index, weight_index;

	size_mobile = ch->size;
	weight_mobile = ch->weight;

	if (!ch->pIndexData) return 1;

	size_index = ch->pIndexData->size;
	weight_index = ch->pIndexData->weight;

	size_mobile = UMAX(1, size_mobile);
	size_index = UMAX(1, size_index);
	weight_mobile = UMAX(1, weight_mobile);
	weight_index = UMAX(1, weight_index);

	double size_differential = size_mobile / size_index;
	double weight_differential = weight_mobile / weight_index;
	double total_differential = (100 * size_differential) + (100 * weight_differential);

	return UMAX(1, total_differential);
}

//describes the weight of an object in absolute terms
void describe_obj_weight(OBJ_DATA *pObj, char *empty_string)
{

	char phrase[MSL], units[MSL], output[MSL];
	float obj_weight = get_physics_obj_weight(pObj);

	//ounces
	if (obj_weight < 1)
	{
		strcpy(units, "ounce");
		obj_weight *= 16;
	}
	else strcpy(units, "pound");

	number_to_english((int)obj_weight, phrase);

	if (obj_weight < 1) sprintf(output, "less than one %s", units);
	else sprintf(output, "about %s %s%s", phrase, units, (((int)obj_weight > 1) ? "s" : ""));

	strcpy(empty_string, output);

	return;
}

//describes the physical state (i.e. liquid, gas, etc) of an object
void describe_obj_state(OBJ_DATA *pObj, char *empty_string)
{

	int x = 0, phase = PHASE_SOLID;
	char *desc_string;

	if (is_composite(pObj)) desc_string = "solid";

	else
	{
		//find material
		while (pObj->material[x] <= 0) x++;
		phase = physics_material_phase(x, physics_obj_temperature(pObj));

		if (phase == PHASE_SOLID) desc_string = "solid";
		else if (phase == PHASE_LIQUID) desc_string = "liquid";
		else if (phase == PHASE_BOILING) desc_string = "boiling";
		else if (phase == PHASE_GAS) desc_string = "gaseous";
		else desc_string = "solid";
	}

	strcpy(empty_string, desc_string);

	return;
}


//describes the material type.  for composite materials, composite sentences will be returned
void describe_obj_material(OBJ_DATA *pObj, char *empty_string)
{

	int x, count = 0, intbuf = 0;
	char desc_string[MSL] = "", mat_string[MSL] = "", buf_string[MSL] = "";

	for (x = 0; material_table[x].name; x++)
		if (pObj->material[x] > 0) count++;

	for (x = 0; material_table[x].name; x++)
		if (pObj->material[x] > 0)
		{
		intbuf++;

		//if it's 1, copy and return
		//if it's 2, no spaces or commas
		//if it's 2+ and we're on the last one, copy "mat" to buf_string

		//one material
		if (count == 1 || (count>1 && intbuf == count)) sprintf(buf_string, "%s", material_table[x].name);
		//two materials
		else if (count == 2 && intbuf < count) sprintf(buf_string, "%s and ", material_table[x].name);
		//three materials
		//not yet on the second to last material
		else if (count>2 && (intbuf + 1) < count) sprintf(buf_string, "%s, ", material_table[x].name);
		//we are on the second to last material
		else if (count>2 && (intbuf + 1) == count) sprintf(buf_string, "%s, and ", material_table[x].name);

		strcat(desc_string, buf_string);

		}

	strcpy(empty_string, buf_string);

	return;
}

//here's the main function: this should only accept one of the variables, and it will
//try for an NPC first otherwise.  Any valid #tags will be replaced by function recursively
void procedurally_describe(CHAR_DATA *npc, OBJ_DATA *obj, ROOM_INDEX_DATA *room)
{
	if (npc)
	{
		//	initalize_npc_rng(npc);
		if (npc->color) npc->color = NULL;
		if (npc->furcondition) npc->furcondition = NULL;
		if (npc->furpattern) npc->furpattern = NULL;
		procgen_mobile(npc);
	}
	else if (obj)
	{
		//	initalize_item_rng(obj);
		procgen_object(obj);
		contextualize_object(obj);
	}
	else if (room)
	{
		procgen_room(room);
	}

	return;
}
