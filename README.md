# natural-language-procgen
Natural language procedural generation for MUD game assets (NPC, Object, Rooms)

The purpose of this engine is to integrate procedural generation into the normal “natural language” used by game writers to fill in game content.  Objects, NPCs, and game areas are assigned numerical and categorical variables by the content-creator (for example, a creator may set a tote bag to be created with a random color and static fabric), and this engine will accordingly assign appropriate “natural language” (i.e., context-aware English language) that describes the entity’s coded values.  The main draw of this package is that it can integrate procedural generation into a 100% manually-built gameworld without any modifications, and leaves content-creators to hand-create anything normally, but gives them the option to use the normal game system to create static, pseudo-procedural, or full dynamically procedural content.  In the cases that 100% manual creation is not preferable, this engine supplements the game-creators with a seamless environment for case-by-case procedural generation of content.  This engine scales easily, works recursively, and is easily understood by example:

	a #color #fabric bag lies on the ground, #containerfill
	---> a brown canvas bag lies on the ground, half-full

This shows off several functions: if the “color” field has been set, it will correspond to the value set by the game-builder, but if there is no hand-built value, the “#color” tag will be randomized.  Not only do these static qualities get transformed to context-sensitive natural language, but it can replace dynamic states (whether or not the bag is full or empty). 

ACTUAL EXAMPLE TAKEN FROM GAME:

	#...dforestflora, and #,dforestsense  Toward #waterdirection, a small downward bank leads into a slow-flowing stream
  
	---> On the ground, thin blades of grass grow wherever they can find purchase, and a constant thrum of quiet sound accompanies the forest life, almost inaudible.  Toward the south, a small downward bank leads into a slow-flowing stream.

This shows a bit of the engine’s recursive power.  By writing a simple general description (the seed string), the game fills details into the surroundings of an area that a game-creator doesn’t have the need to add specific details to—but if there are details necessary to the area (like the fact that water is nearby), the engine allows the creator to integrate this info alongside the procedurally generated content.  In addition, the game will pay attention to the English language context, punctuating according to the surrounding text.

For more examples, view the “example_procgen.txt” file included.

Technical stuff:

	procgen_engine.c <- source code for most of the engine's relevant functions
	procgen_assets.c <- tables, some declares, etc that support the functions in procgen_engine.c
	procgen.h <- headers, declares, etc

Important Functions:

	void procedurally_describe(CHAR_DATA *npc, OBJ_DATA *obj, ROOM_INDEX_DATA *room)

This function is the heavy-hitter of this whole package.  It is going to expect only one argument, the rest NULL.  This one will basically do the work for you: any tags inside the description fields for the passed object (NPC, OBJ, or ROOM) is going to get replaced as long as the #tagged value is on the table of valid replacements.

	void procgen_string_replace_mobile(CHAR_DATA *npc)
	void procgen_string_replace_object(OBJ_DATA *obj)
	void procgen_string_replace_room(ROOM_INDEX_DATA *room)

These functions contain the special-case circumstances for values that cannot be subbed out according to a table.  For example, the #hisher tag should be replaced contextually depending on gender rather than depending on seed value of the random generation process; inside the above functions, these special cases are listed at the end.

TABLES:

	const struct  trait_index_type  trait_table[]

This table contains the list of valid #tags and references another table which contains the possible replacements for the tag invoked.  Those tables are all currently located inside the procgen_assets.c file; look there for examples of the trait_table that is being referenced.


