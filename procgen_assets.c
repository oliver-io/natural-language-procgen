

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


/*
#define SPAWN_DFORESTOAK			1
#define	SPAWN_DFORESTBIRCH			2
#define SPAWN_DFORESTREDMAPLE			3
#define SPAWN_DFORESTYEW			4
#define SPAWN_DFORESTRESERVED1			5
#define SPAWN_DFORESTRESERVED2			6
#define SPAWN_DFORESTMARIJUANA			7
#define SPAWN_DFORESTTOBACCO			8
#define SPAWN_DFORESTKINGSFOIL			9
#define SPAWN_DFORESTFIREFLOWER			10
#define SPAWN_DFORESTWATERFLOWER		11
#define SPAWN_DFORESTWINDFLOWER			12
#define SPAWN_DFORESTEARTHFLOWER		13
#define SPAWN_DFORESTMAGICFLOWER		14
#define SPAWN_DFORESTSTRAWBERRY			15
#define SPAWN_DFORESTRASPBERRY			16
#define SPAWN_DFORESTBLACKBERRY			17
#define SPAWN_DFORESTBLUEBERRY			18
#define SPAWN_DFORESTGRAPES			19
#define SPAWN_DFORESTTRUFFLEMUSHROOM		20
#define SPAWN_DFORESTPORTABELLAMUSHROOM		21
#define SPAWN_DFORESTMOREL			22
#define SPAWN_DFORESTPOISONMUSHROOM		23
#define SPAWN_DFORESTDEATHMUSHROOM		24
#define SPAWN_DFORESTSMALLBURROW		25
#define SPAWN_DFORESTLARGEBURROW		26
*/

//order matters in that a longer string must be higher on the table than the
//others if they share a beginning string.  i.e., songbirdfeathercondition
//must be higher than songbird
const	struct	trait_index_type	trait_table[] =
{
	//to add more: tables.c entry, tables.h entry, save_prototypes.c set_values() entries
	{ "autogendforeststreambank", autogendforeststreambank_table, 0 },
	{ "autogendforeststream", autogendforeststream_table, 0 },
	{ "autogendforestriverbank", autogendforestriverbank_table, 0 },
	{ "autogendforestriver", autogendforestriver_table, 0 },
	{ "autogendforestravinebank", autogendforestravinebank_table, 0 },
	{ "autogendforestravine", autogendforestravine_table, 0 },
	{ "eyecolor", ecolortrait_table, MAX_EYES },
	{ "haircolor", hcolortrait_table, MAX_HAIR },
	{ "age", agetrait_table, MAX_AGE },
	{ "hairstyle", hstyletrait_table, MAX_HSTYLE },
	{ "build", buildtrait_table, MAX_BUILDTRAIT },
	{ "face", facetrait_table, MAX_FACETRAIT },
	{ "size", sizetrait_table, MAX_SIZETRAIT },
	{ "foodsize", foodsizetrait_table, 0 },
	{ "plantsize", plantsizetrait_table, 0 },
	{ "partsize", partsizetrait_table, 0 },
	//hack alert: if this is indexed lower than the furcolor tables, rabbit generation might break
	{ "rabbittrait", rabbittrait_table, MAX_RABBITTRAIT },
	{ "furcolor", furcolortrait_table, 0 },
	{ "lightfurcolor", lightfurcolortrait_table, MAX_LIGHTFURCOLORTRAIT },
	{ "darkfurcolor", darkfurcolortrait_table, MAX_DARKFURCOLORTRAIT },
	{ "brownscalefurcolor", brownscalefurcolortrait_table, 0 },
	{ "furcondition", furconditiontrait_table, MAX_FURCONDITIONTRAIT },
	{ "furpattern", furpatterntrait_table, 0 },
	{ "haircolor", haircolortrait_table, 0 },
	{ "lighthaircolor", lighthaircolortrait_table, 0 },
	{ "darkhaircolor", darkhaircolortrait_table, 0 },
	{ "brownscalehaircolor", brownscalehaircolortrait_table, 0 },
	{ "haircondition", hairconditiontrait_table, 0 },
	{ "feathercondition", featherconditiontrait_table, 0 },
	{ "songbird", songbirdtrait_table, MAX_SONGBIRDTRAIT },
	{ "mediumbird", mediumbirdtrait_table, 0 },
	{ "bigbird", bigbirdtrait_table, 0 },
	{ "rosecolor", rosecolortrait_table, 0 },
	{ "dforestground", dforestgroundtrait_table, MAX_DFORESTGROUNDTRAIT },
	{ "dforestflora", dforestfloratrait_table, MAX_DFORESTFLORATRAIT },
	{ "dforestsense", dforestsensetrait_table, 0 },
	{ "dforesttrees", dforesttreetrait_table, 0 },
	{ "numero_uno", numero_uno, 0 },
	{ "numero_dos", numero_dos, 0 },
	{ "numero_tres", numero_tres, 0 },
	{ "numero_quatro", numero_quatro, 0 },
	{ "numero_cinco", numero_cinco, 0 },
	{ "hmarythstreet", hmarythstreettrait_table, 0 },
	{ "hmarythfacades", hmarythfacadestrait_table, 0 },
	{ "hmarythsense", hmarythsensetrait_table, 0 },
	{ "lmarythstreet", lmarythstreettrait_table, 0 },
	{ "lmarythfacades", lmarythfacadestrait_table, 0 },
	{ "lmarythpalace", lmarythpalacetrait_table, 0 },
	{ "lmarythsense", lmarythsensetrait_table, 0 },
	{ "lmarythwalls", lmarythwallstrait_table, 0 },
	{ NULL, NULL, 0 }
};


/* char traits - eye color */
const   struct  trait_type       ecolortrait_table[] =
{
	{ NULL, 100, 0 },
	{ "pure black", 99, 0 },
	{ "blue", 10, 0 },
	{ "light blue", 30, 0 },
	{ "dark blue", 50, 0 },
	{ "brown", 10, 0 },
	{ "light brown", 50, 0 },
	{ "dark brown", 30, 0 },
	{ "golden", 95, 0 },
	{ "green", 10, 0 },
	{ "light green", 30, 0 },
	{ "dark green", 50, 0 },
	{ "grey", 90, 0 },
	{ "hazel", 50, 0 },
	{ "pure white", 99, 0 },
	{ NULL, 100, 0 }
};

/* char traits - hair color */
const   struct  trait_type      hcolortrait_table[] =
{
	{ NULL, 100, 0 },
	{ "auburn", 1, 0 },
	{ "black", 1, 0 },
	{ "blonde", 50, 0 },
	{ "dirty blonde", 30, 0 },
	{ "platinum blonde", 90, 0 },
	{ "brown", 1, 0 },
	{ "light brown", 33, 0 },
	{ "dark brown", 33, 0 },
	{ "grey", 85, 0 },
	{ "red", 90, 75 },
	{ "silver", 99, 99 },
	{ "white", 99, 99 },
	{ NULL, 100, 0 }
};

const 	struct	trait_type	furcolortrait_table[] =
{
	{ NULL, 100, 0 },
	{ "brown", 1, 5 },
	{ "dark", 1, 5 },
	{ "grey", 1, 5 },
	{ "white", 20, 20 },
	{ "off-white", 30, 30 },
	{ "sable", 30, 30 },
	{ "rusty", 30, 30 },
	{ "mousy", 30, 30 },
	{ "sandy", 50, 30 },
	//	{ "roan",		50, 30},
	{ "red", 80, 30 },
	{ NULL, 100, 0 }
};

const 	struct	trait_type	haircolortrait_table[] =
{
	{ NULL, 100, 0 },
	{ "brown", 1, 5 },
	{ "dark", 1, 5 },
	{ "grey", 1, 5 },
	{ "white", 20, 20 },
	{ "off-white", 30, 30 },
	{ "sable", 30, 30 },
	{ "rusty", 30, 30 },
	{ "mousy", 30, 30 },
	{ "sandy", 50, 30 },
	//	{ "roan",		50, 30},
	{ "red", 80, 30 },
	{ NULL, 100, 0 }
};

const 	struct	trait_type	brownscalefurcolortrait_table[] =
{
	{ NULL, 100, 0 },
	{ "tan", 50, 20 },
	{ "chestnut", 50, 20 },
	{ "russet", 50, 20 },
	{ "beige", 50, 20 },
	{ "ochre", 50, 20 },
	{ "straw", 50, 20 },
	{ NULL, 100, 0 }
};


const 	struct	trait_type	brownscalehaircolortrait_table[] =
{
	{ NULL, 100, 0 },
	{ "tan", 50, 20 },
	{ "chestnut", 50, 20 },
	{ "russet", 50, 20 },
	{ "beige", 50, 20 },
	{ "ochre", 50, 20 },
	{ "straw", 50, 20 },
	{ NULL, 100, 0 }
};

const 	struct	trait_type	lightfurcolortrait_table[] =
{
	{ NULL, 100, 0 },
	{ "light brown", 10, 5 },
	{ "light grey", 10, 5 },
	{ "tan", 10, 10 },
	{ "cream", 10, 10 },
	{ "beige", 20, 15 },
	{ "flaxen", 20, 15 },
	{ "bright red", 90, 80 },
	{ "silver", 90, 95 },
	{ "golden", 90, 95 },
	{ "pure white", 97, 99 },
	{ NULL, 100, 0 }
};

const 	struct	trait_type	lighthaircolortrait_table[] =
{
	{ NULL, 100, 0 },
	{ "light brown", 10, 5 },
	{ "light grey", 10, 5 },
	{ "tan", 10, 10 },
	{ "cream", 10, 10 },
	{ "beige", 20, 15 },
	{ "flaxen", 20, 15 },
	{ "bright red", 90, 80 },
	{ "silver", 90, 95 },
	{ "golden", 90, 95 },
	{ "pure white", 97, 99 },
	{ NULL, 100, 0 }
};


const 	struct	trait_type	darkfurcolortrait_table[] =
{
	{ NULL, 100, 0 },
	{ "dark brown", 10, 5 },
	{ "earthy brown", 10, 5 },
	{ "taupe", 15, 10 },
	{ "dark grey", 10, 5 },
	{ "dark red", 50, 90 },
	{ "smoky", 50, 90 },
	{ "jet black", 95, 99 },
	{ NULL, 100, 0 }
};


const 	struct	trait_type	darkhaircolortrait_table[] =
{
	{ NULL, 100, 0 },
	{ "dark brown", 10, 5 },
	{ "earthy brown", 10, 5 },
	{ "taupe", 15, 10 },
	{ "dark grey", 10, 5 },
	{ "dark red", 50, 90 },
	{ "smoky", 50, 90 },
	{ "jet black", 95, 99 },
	{ NULL, 100, 0 }
};


const 	struct	trait_type	furpatterntrait_table[] =
{
	//... markings
	{ NULL, 100, 0 },
	{ "motley", 10, 5 },
	{ "plain", 10, 5 },
	{ "spotted", 30, 10 },//10
	{ "striped", 50, 15 },
	{ "dappled", 50, 20 },
	{ "barred", 50, 25 },
	{ NULL, 100, 0 }
};


const 	struct	trait_type	furconditiontrait_table[] =
{
	{ NULL, 100, 0 },
	{ "dense and silky", 95, 90 },
	{ "silky and clean", 90, 85 },
	{ "sleek and smooth", 80, 75 },
	{ "thick and soft", 70, 65 },
	{ "bristly and clean", 60, 40 },
	{ "bristly and dense", 20, 30 },
	{ "thin and ragged", 50, 4 },
	{ "patchy and dirty", 20, -20 },
	{ "dirty and ragged", 40, -30 },
	{ "dense and matted", 40, -40 },
	{ "patchy and matted", 20, -50 },
	{ NULL, 100, 0 }
};

const 	struct	trait_type	hairconditiontrait_table[] =
{
	{ NULL, 100, 0 },
	{ "dense and silky", 95, 90 },
	{ "silky and clean", 90, 85 },
	{ "sleek and smooth", 80, 75 },
	{ "thick and soft", 70, 65 },
	{ "bristly and clean", 60, 40 },
	{ "bristly and dense", 20, 30 },
	{ "thin and ragged", 50, 4 },
	{ "patchy and dirty", 20, -20 },
	{ "dirty and ragged", 40, -30 },
	{ "dense and matted", 40, -40 },
	{ "patchy and matted", 20, -50 },
	{ NULL, 100, 0 }
};


const	struct	trait_type	rabbittrait_table[] =
{
	{ NULL, 100, 0 },
	{ "huge, black ears", 99, 99 }, //sup george?
	{ "long, #lightfurcolor ears", 95, 90 },
	{ "long, #darkfurcolor ears", 95, 90 },
	{ "spots over its eyes", 90, 50 },
	{ "mottled and splotched fur", 80, 30 },
	{ "#lightfurcolor spots", 20, 20 },
	{ "#brownscalefurcolor spots", 20, 15 },
	{ "#darkfurcolor spots", 20, 15 },
	{ "#furcolor spots", 20, 15 },
	{ "#lightfurcolor splotches", 20, 15 },
	{ "#furcolor splotches", 20, 15 },
	{ "#brownscalefurcolor splotches", 20, 15 },
	{ "#darkfurcolor splotches", 20, 15 },
	{ NULL, 100, 0 }
};

const	struct	trait_type	dandeliontrait_table[] =
{
	{ NULL, 100, 0 },
	{ "a small, tiny-flowered dandelion", 1, 1 },
	{ "a huge, long-stemmed dandelion", 95, 90 },
	{ NULL, 100, 0 }
};

const   struct  trait_type      agetrait_table[] =
{
	//...years
	{ NULL, 100, 0 },
	{ "teenage", 90, 0 },
	{ "youthful", 20, 0 },
	{ "adult", 10, 0 },
	{ "middle", 50, 0 },
	{ "later", 75, 0 },
	{ "elderly", 85, 0 },
	{ "final", 95, 0 },
	{ NULL, 100, 0 }

};

const   struct  trait_type      featherconditiontrait_table[] =
{
	// ... feathers
	{ NULL, 100, 0 },
	{ "well-kempt, glossy and bright", 90, 90 },
	{ "glossy and smooth", 80, 55 },
	{ "bright and smooth", 70, 45 },
	{ "glossy, but ruffled and messy", 60, 30 },
	{ "smooth", 50, 15 },
	{ "glossy", 30, 15 },
	{ "bright", 1, 15 },
	{ "ruffled", 1, -10 },
	{ "patchy", 1, -20 },
	{ NULL, 100, 0 }

};

const   struct  trait_type      songbirdtrait_table[] =
{
	//... #songbird (alone)
	{ NULL, 100, 0 },

	{ "a brown and white robin with a red breast", 10, 0 }, //robin
	{ "a cardinal with a red crest and black face", 30, 0 }, //cardinal
	{ "a large-beaked crow with midnight feathers", 70, 0 }, //crow
	{ "a small chickadee with a white breast and grey wings", 10, 0 }, //chickadee
	{ "a small yellow goldfinch with a black forehead", 50, 0 }, //goldfinch
	{ "a tiny warbler with black and white feathers", 50, 0 }, //warbler
	{ "a reddish-brown thrush with a white underside", 25, 0 },	 //thrush
	{ "a small, brightly-colored, sharp-beaked woodpecker", 80, 0 },	 //green wp
	{ "a tiny hummingbird with thin, green wings", 95, 0 },	//hummingbird
	{ NULL, 100, 0 }
};

const   struct  trait_type      mediumbirdtrait_table[] =
{
	//... #mediumbird (alone)
	{ NULL, 100, 0 },
	{ "a large-beaked crow with midnight feathers", 10, 0 }, //crow
	{ "a large and round-headed dove with grey feathers", 10, 0 },	 //mourning dove
	{ "a grey owl with black spotting", 10, 0 },
	{ "a brown owl with large yellow eyes", 10, 0 },
	{ NULL, 100, 0 }
};

const   struct  trait_type      bigbirdtrait_table[] =
{
	//... #bigbird (alone)
	{ NULL, 100, 0 },
	{ "a large, white-headed eagle with brown feathers ", 10, 0 }, //bald eagle
	{ "an eagle with golden and brown feathers", 10, 0 },	 //some other eagle
	{ "a large hawk with brown feathers and a tail of red", 10, 0 },	 //big hawk
	{ "a taloned vulture with a bald head and red feathers", 10, 0 },	 //vulture
	{ NULL, 100, 0 }
};


const   struct  trait_type      autogendforeststreambank_table[] =
{
	{ NULL, 100, 0 },
	{ "#dforesttrees #waterdirection, a streambank leads downward into a wide stream.  Slow-flowing and clear-watered, the stream issues a sound of gurgling, and #,dforestsense", 1, 0 },
	{ "toward #waterdirection, a small drop leads into a stream.  The water of the stream is clear and slow, and several feet deep.  Atop the banks, #,dforestground", 1, 0 },
	{ "#...dforestflora, and #,dforestsense  Toward #waterdirection, a small downward bank leads into a slow-flowing stream", 1, 0 },
	{ "A slow, several-foot-deep flow of clear water occupies a streambed toward #waterdirection.  Atop the banks, #...dforesttrees, and #,dforestground", 1, 0 },
	{ NULL, 100, 0 }
};

const   struct  trait_type      autogendforeststream_table[] =
{
	{ NULL, 100, 0 },
	{ "the water of the stream here is clean, its bed littered with time-worn pebbles.  Wide and slow, its flow gurgles through the forest.  On the streambanks, #,dforestground", 1, 0 },
	{ "#waterdirection, the stream continues onward slowly, its water clean and cool, the banks wideset.  On the banks to either side, #...dforesttrees, and #,dforestflora", 1, 0 },
	{ "across a pebble-strewn bed, the water of this slow-flowing stream gurgles audibly.  Trees frame the clay streambanks, and #,dforestsense", 1, 0 },
	{ "several feet deep at the center, this wide stream harbors a fair amount of clear, cool water.   To the sides, along the banks to either side, #dforestflora", 1, 0 },
	{ NULL, 100, 0 }
};

const   struct  trait_type      autogendforestriver_table[] =
{
	{ NULL, 100, 0 },
	{ "the riverwater runs torrentially here, fast and deep.  Eddies swirl along log-strewn, rocky banks, and to the northwest, the bank is particularly steep.  On both sides, #,dforesttrees", 1, 0 },
	{ "deeper than any man at the center, the river is rather narrow, spanning no more than fourty feet; fast waters kick up mud, giving the water murk.  Along its steep banks, #,dforestflora", 1, 0 },
	{ "the river extends to #waterdirection, steep-banked and fast.  The loud-splashing waters wear continually at the bed, dragging silt along its careening path, and the banks are lined with rocks and boulders, logs and debris, natural detrius of all sorts", 1, 0 },
	{ "the riverwater has worn a ravine through the forest.  High on the banks above, where water previously cut away the earth, #,dforesttrees.  Down in the riverwater, dangerous curents and eddies drag along anything in their path", 1, 0 },
	{ NULL, 100, 0 }
};

const   struct  trait_type      autogendforestriverbank_table[] =
{
	{ NULL, 100, 0 },
	{ "#dforestsense To #waterdirection, the forest turf ends in a rather abrupt edge of grassy, rock-strewn dirt.  At the bottom of the steeply graded hill that follows, riverwater careens through a channel both deep and narrow, forming a fourty-foot-wide river", 1, 0 },
	{ "#...dforesttrees, and #,dforestsense To #waterdirection, the forest ends at the edge of a steep riverbank-- nearly a cliff-- and descends to torrential waters", 1, 0 },
	{ "#dforestflora The sound of rushing water is loud, from #waterdirection, and a steep riverbank leads down into a narrow, rushing river.  Above, #,dforestground", 1, 0 },
	{ "far down a steeply graded hill to the #waterdirection, a river cuts a narrow but deep ravine through the forest floor.  Above, #dforestground", 1, 0 },
	{ NULL, 100, 0 }
};

const   struct  trait_type      autogendforestravinebank_table[] =
{
	{ NULL, 100, 0 },
	{ "the forest turf ends in a sharp dropoff toward #declinedirection.  At the bottom of the decline, a dry riverbank cuts through the forest floor, now a ravine covered in the long-left remnants of fallen leaves and logs.  Above, #,dforesttrees", 1, 0 },
	{ "#...dforestground, and #,dforestflora Toward #declinedirection a sharp dropoff leads into a dry riverbed's steep-sloped ravine", 1, 0 },
	{ "toward #declinedirection the forest floor ends in a rooty, rocky, steep decline.  At the bottom, a dry riverbed cuts a small ravine through the forest.  Atop the depression in the ground, #,dforestflora", 1, 0 },
	{ "#...dforestground, but toward #declinedirection the forest floor ends abruptly.  At the bottom of the sharp decline, a narrow and steep-sided ravine has been carved into the forest by water, but as of now, the channel is bone-dry", 1, 0 },
	{ NULL, 100, 0 }
};

const   struct  trait_type      autogendforestravine_table[] =
{
	{ NULL, 100, 0 },
	{ "#dforestsense Toward #primarydirection a ravine continues, sharply-graded on both sides and narrow at the bottom.  Visible atop thirty feet of rooty, rock-strewn soil facades blocking #inclinedirection are the leafy greens and browns of further forest landscape", 1, 0 },
	{ "#...dforestground, here meeting narrowly at the center of a ravine.  Toward #inclinedirection, sharply graded walls of dirt and grass lead out of the ravine, which itself continues toward #primarydirection", 1, 0 },
	{ "travel toward #inclinedirection is obstructed by a thirty foot climb up rocky, root-strewn ravine walls, but  toward #primarydirection the ravine's path is left navigable.  Above, #,dforesttrees", 1, 0 },
	{ "along steeply graded slopes to #inclinedirection, #,dforestflora Passage toward #primarydirection is less treacherous, though the root-strewn walls of the ravine crowd close, and debris litters the way", 1, 0 },
	{ NULL, 100, 0 }
};

//const	struct	trait_type	riverfrog b

/*
"a red-breasted robin"
robin (red breast, white ring around the eye)
chirrups, chirps melodiously
cardinals (big red assholes, mohawk, black mask)
machine gun
crows
chickadee (black crown, black patch under throat, white breast, grey wings)
goldfinch (yellow bird, chickadee size, black forehead)
high-pitched, CHIRP
warbler (tiny black and white bird)
squeaky, lilting, warbling
thrush	(back reddish brown, underside is all white except dark brown spots)
flutish, rising,


HAWKS: bigger, use talons to attack
red-tailed	brown feathers, tail feathers are red, underside is white w/brown barred,
hooked beak, talons,


FALCONS: use beaks to catch prey


*/


const   struct  trait_type      hstyletrait_table[] =
{
	//...hair
	{ NULL, 0, 0 },
	{ "short", 10, 0 },
	{ "messy", 95, 0 },
	{ "long", 10, 0 },
	{ "medium-length", 10, 0 },
	{ "curly, short", 30, 0 },
	{ "curly, long", 90, 0 },
	{ "wavy, short", 50, 0 },
	{ "wavy, long", 75, 0 },
	{ NULL, 100, 0 }

};

const   struct  trait_type      buildtrait_table[] =
{
	//...build
	{ NULL, 0, 0 },
	{ "extremely short", 95, 0 },
	{ "short and squat", 10, 0 },
	{ "short and skinny", 15, 0 },
	{ "short", 10, 0 },
	{ "midsized", 10, 0 },
	{ "skinny", 25 },
	{ "stout", 30, 0 },
	{ "tall", 30, 0 },
	{ "tall and skinny", 80, 0 },
	{ "tall and stout", 70, 0 },
	{ "extremely tall", 75, 0 },
	{ NULL, 100, 0 }

};

const   struct  trait_type      facetrait_table[] =
{
	//...build
	{ NULL, 0, 0 },
	{ "pockmarked", 75, 0 },
	{ "pimpled", 75, 0 },
	{ "comely", 90, 0 },
	{ "square-jawed", 75, 0 },
	{ "round", 1, 0 },
	{ "angular", 1, 0 },
	{ "plain", 1, 0 },
	{ "homely", 1, 0 },
	{ "cherubic", 50, 0 },
	{ "weak-jawed", 50, 0 },
	{ "wide", 25, 0 },
	{ "large-nosed", 50, 0 },
	{ "squashed-looking", 75, 0 },
	{ NULL, 100, 0 }

};


const   struct  trait_type      sizetrait_table[] =
{
	//...build
	{ NULL, 100, 0 },
	{ "pygmy", 98, .2 },
	{ "meager", 90, .33 },
	{ "tiny", 85, .5 },
	{ "small", 25, .7 },
	{ "medium", 20, 1 },
	{ "large", 25, 1.2 },
	{ "sizeable", 75, 1.4 },
	{ "massive", 90, 1.6 },
	{ "gigantic", 95, 1.8 },
	{ "colossal", 98, 2.2 },
	{ NULL, 100, 0 }

};

const   struct  trait_type      foodsizetrait_table[] =
{
	//...build
	{ NULL, 100, 0 },
	{ "meager", 90, .33 },
	{ "small", 25, .7 },
	{ "medium", 20, 1 },
	{ "large", 25, 1.2 },
	{ "huge", 90, 1.6 },
	{ "massive", 95, 1.8 },
	{ NULL, 100, 0 }

};

const   struct  trait_type      plantsizetrait_table[] =
{
	//...build
	{ NULL, 100, 0 },
	{ "pygmy", 98, .2 },
	{ "meager", 90, .33 },
	{ "tiny", 85, .5 },
	{ "small", 25, .7 },
	{ "medium", 20, 1 },
	{ "large", 25, 1.2 },
	{ "sizeable", 75, 1.4 },
	{ "massive", 90, 1.6 },
	{ "gigantic", 95, 1.8 },
	{ "colossal", 98, 2.2 },
	{ NULL, 100, 0 }

};

const   struct  trait_type      partsizetrait_table[] =
{
	//...build
	{ NULL, 100, 0 },
	{ "pygmy", 98, .2 },
	{ "meager", 90, .33 },
	{ "tiny", 85, .5 },
	{ "small", 25, .7 },
	{ "medium", 20, 1 },
	{ "large", 25, 1.2 },
	{ "sizeable", 75, 1.4 },
	{ "massive", 90, 1.6 },
	{ "gigantic", 95, 1.8 },
	{ "colossal", 98, 2.2 },
	{ NULL, 100, 0 }

};

const   struct  trait_type      rosecolortrait_table[] =
{
	//...build
	{ NULL, 100, 0 },
	{ "red", 1, 1 },
	{ "light red", 2, 2 },
	{ "dark red", 2, 2 },
	{ NULL, 100, 0 }

};


//the ground in the forest
const	struct	trait_type	dforestgroundtrait_table[] =
{
	{ NULL, 100, 0 },
	// ... or ,
	{ "dark and grainy, underfoot the soil is soft and pliant to the touch", 1, 0 },
	{ "much of the dirt is covered in the remnants of leaves and decaying plants", 1, 0 },
	{ "the dirt is mostly a rich and uncompacted black turf, covered by leaves", 1, 0 },
	{ "dried leaves and broken twigs cover a large portion of the ground", 1, 0 },
	{ "beneath the layer of dead deciduous leaves, uneven black soil harbors thick roots", 1, 0 },
	{ "the surrounding trees root themselves deeply in the ground, a rich and black dirt", 1, 0 },
	{ "trees have dug their long roots into the sandy clay below, red and rich", 1, 0 },
	{ "the soil-- black turf and red clay-- makes for uneven flooring", 1, 0 },
	{ "covered in dead leaves and scraggly grass, the ground has been compacted", 1, 0 },
	{ "where the roots of trees surface and where rocks have been deposited, the ground is bumpy", 1, 0 },
	{ "dark earth lies under a thick layer of dead leaves, soft and loamy", 1, 0 },
	{ "loam waits beneath thick layers of dead and growing grass and leaves", 1, 0 },
	{ "giving home to the growth of nearby plants, rich and black dirt lies underfoot", 1, 0 },
	{ "turned over, the mat of twigs and dead leaves exposes black dirt and clay", 1, 0 },
	{ "beneath the remnants of decaying plants, dark soil lies, compacted and moist", 1, 0 },
	{ "the roots of trees and smaller plants make a knobbly surface of compact dirt", 1, 0 },
	{ "dirt, most of which is covered in grass or shrubs, is in some places exposed", 1, 0 },
	{ "black dirt, layered with dead plant matter, makes for loud walking", 1, 0 },
	{ "the twigs, grasses and topsoil crinkle and crack at the slightest errant move", 1, 0 },
	{ "the dirt is soft enough to leave obvious footprints, uncompacted", 1, 0 },
	{ "the black earth below is tumultuous and covered with dead leaves", 1, 0 },
	{ "covered by years of accumulated leaves, the forest floor is branch-strewn and uneven", 1, 0 },
	{ "soil underfoot is loamy, rich, red clay in some spots and dark black in others", 1, 0 },
	{ "loose black dirt has been layered with a host of thick, fallen branches", 1, 0 },
	{ "the earth is hard and cracked, dark clay fractured by thick roots", 1, 0 },
	{ "uneven and compact, the forest floor is swaddled in a thick mat of fallen leaves", 1, 0 },
	{ "the soil has been compacted beneath an inch of caked leaves and branches and dead plants", 1, 0 },
	{ "in some places, the soil underfoot has been hardened and flattened; in others, it remains loam", 1, 0 },
	{ "despite the thick mat of fallen leaves and grass, the earth is soft and yielding", 1, 0 },
	{ "the ground has been riddled holes burrowed into the dark soil", 1,
	SPAWN_DFORESTSMALLBURROW },
	{ "mounds of dirt form peaks in the compacted ground, surrounding burrow-holes", 1,
	SPAWN_DFORESTSMALLBURROW },
	{ NULL, 100, 0 }
};


const struct	trait_type	dforestfloratrait_table[] =
//flora in the forest
{
	{ NULL, 100, 0 },
	{ "a myriad of dead and flourishing flora are evident in shades of #brownscaleplantcolor", 1, 0 },
	{ "on the ground, thin blades of grass grow wherever they can find purchase", 1, 0 },
	{ "along the ground, low-thriving forest plants grow, grasses and mosses and wide-leafed vines", 1, 0 },
	{ "the leafy canopy above, while not entirely thick, casts shade on growing mosses", 1, 0 },
	{ "growing low, some small low-growing flora manage to compete with the tall trees", 1, 0 },
	{ "moss and low-growing plants flourish beneath the trees overhead", 1, 0 },
	{ "grasses and small flowers poke through a mat of dead leaves and discarded branches", 1, 0 },
	{ "the forest floor hosts a number of plants, some flowering, and some leafy", 1, 0 },
	{ "moss grows up the trunks of trees and over rocks, anywhere it can blanket", 1, 0 },
	{ "below, thin-bladed grass ekes out a habitat atop a tough bed of dirt and roots", 1, 0 },
	{ "growing between tree trunks and an eon of broken branches, grass proliferates", 1, 0 },
	{ "soft mosses climb inexorably up the trunks of trees, thick and green", 1, 0 },
	{ "where soil is exposed, thin grass has taken root, covering the forest in clumpy turf", 1, 0 },
	{ "on the ground, many plants have taken root, short and deprived of sunlight", 1, 0 },
	{ "within the sheltered forest shade, grasses and small-petaled flowers prevail", 1, 0 },
	{ "spatterings of small plants with red-streaked leaves cover the ground", 1, 0 },
	{ "low-growing and wide-leafed, the shade-choked undergrowth grows sparse", 1, 0 },
	{ "thick-rooted ivy grows, woven into the mat of leaves covering the ground", 1, 0 },
	{ "saplings and hardy plants spring forth from the matted leaves beneath", 1, 0 },
	{ "rooted in the soil, narrow-bladed grass in clusters grow side-by-side everywhere", 1, 0, },
	//{"wildflowers have taken hold, growing between fallen logs and broken twigs",1,0},
	//{"bright-petaled, wildflowers contrast with the naturally-hued surroundings",1,0},
	{ "small vines and grasses carpet the ground in a soft layer, flowering with white petals", 1,
	SPAWN_DFORESTWHITEVINE },
	{ "thin-stalked, small-petaled flowers grow here and there, bright and yellow", 1,
	SPAWN_DFORESTDANDELION },
	{ "with sharp thorns and soft flowers, rose bushes have taken root the forest floor", 1,
	SPAWN_DFORESTROSEBUSH },
	{ "ivy has vined fractally across the ground, bearing flowers with white petals", 1,
	SPAWN_DFORESTWHITEVINE },
	{ "brambles of a jagged-leafed berry plant form shoots, bearing red raspberries", 1,
	SPAWN_DFORESTRASPBERRY },
	{ "berries in various stages of ripeness hang from bramble shoots, protected by thorns", 1,
	SPAWN_DFORESTRASPBERRY },
	{ "dark (nearly purple) berries hang from bushes, tall with wicked thorns", 1,
	SPAWN_DFORESTBLACKBERRY },
	{ "thorny and leafy, dark-fruited berry-plants clump into large bushes", 1,
	SPAWN_DFORESTBLACKBERRY },
	{ "vining over a bushy substrate, grape plants bear clumps of dark and fleshy fruit", 1,
	SPAWN_DFORESTGRAPES },
	{ "a blanket of short, leafy, fruit-bearing plants grow low over the uneven soil", 1,
	SPAWN_DFORESTSTRAWBERRY },
	{ "the heart-shaped leaves of some short plants harbor a large and seeded berry", 1,
	SPAWN_DFORESTSTRAWBERRY },
	{ "a clump of fungus has grown here within the tall forest grasses carpeting the ground", 1,
	SPAWN_DFORESTRANDOMMUSHROOM },

	{ NULL, 100, 0 }
};

const struct	trait_type	dforesttreetrait_table[] =
{
	{ NULL, 100, 0 },
	//oaks
	{ "a copse of thick-trunked oaks, with gnarled bark, dominate the forest", 1,
	SPAWN_DFORESTOAK },
	{ "ridged and knobbly, mature oak trees have grown close together", 1,
	SPAWN_DFORESTOAK },
	{ "oak trees, wide and tall, grow side by side throughout the forest", 1,
	SPAWN_DFORESTOAK },
	{ "rough and brown-barked oak trees stand straight, each tall and many-boughed", 1,
	SPAWN_DFORESTOAK },
	{ "lobed and large leaved, rough-barked oak trees have crowded thick", 1,
	SPAWN_DFORESTOAK },
	{ "with many thick boughs, a grove of thick oak trees habitates the forest", 1,
	SPAWN_DFORESTOAK },
	{ "the product of decades, many thick oak trees habitate this section of the forest", 1,
	SPAWN_DFORESTOAK },
	{ "with gnarled bark, oak trees with lobed leaves the size of a large man's hand have taken root", 1,
	SPAWN_DFORESTOAK },
	{ "lobe-leafed oak trees have taken root, somewhat spread apart", 1,
	SPAWN_DFORESTOAK },
	{ "oak trees with thick boughs and thicker trunks have grown in a patch", 1,
	SPAWN_DFORESTOAK },
	{ "with thick roots lodged in the ground, wide-trunked oak trees populate the forest", 1,
	SPAWN_DFORESTOAK },
	//birch
	{ "birch trees with papery white bark have grown, many closely packed", 1,
	SPAWN_DFORESTBIRCH },
	{ "a collection of similar trees have grown adjacent to one another, birch trees with small leaves", 1,
	SPAWN_DFORESTBIRCH },
	{ "sprouting many small branches and one central trunk, many white-barked birch trees grow", 1,
	SPAWN_DFORESTBIRCH },
	{ "black-and-white bark grows on birch trees, tall and skinny", 1,
	SPAWN_DFORESTBIRCH },
	{ "many birch trees with white bark have grown in close quarters ", 1,
	SPAWN_DFORESTBIRCH },
	{ "birch trees, papery-white skinned, grow thick and many", 1,
	SPAWN_DFORESTBIRCH },
	{ "grouped closely, birch trees with small pinnate leaves and white bark grow here", 1,
	SPAWN_DFORESTBIRCH },
	{ "white and black-lined, birch trees with small leaves grow tall", 1,
	SPAWN_DFORESTBIRCH },
	{ "short birch trees harbor clumps of fungus growing at their bases and up their trunks here", 1,
	SPAWN_DFORESTRANDOMMUSHROOM },
	//maple
	{ "widely-grown maples with angular, palmate leaves grow in a patch here", 1,
	SPAWN_DFORESTREDMAPLE },
	{ "several wide maple trees, shorter than the surrounding forest, have formed a copse", 1,
	SPAWN_DFORESTREDMAPLE },
	{ "wide and thick-boughed, maple trees with large leaves grow above", 1,
	SPAWN_DFORESTREDMAPLE },
	{ "the limbs of thick maples extend overhead, bearing large and palmate leaves", 1,
	SPAWN_DFORESTREDMAPLE },
	{ "several maple trees grow here, with wide trunks and a wider span", 1,
	SPAWN_DFORESTREDMAPLE },
	{ "maple trees grow in the surrounding forest, their leaves large and angular", 1,
	SPAWN_DFORESTREDMAPLE },
	//yew
	{ "coniferous and squat, a stand of leafy yew trees has sprung up here", 1,
	SPAWN_DFORESTYEW },
	//willow
	{ "a stand of short but immensely broad willows casts its myriad long branches overhead", 1,
	SPAWN_DFORESTWILLOW },
	//apple
	{ "a series of trees here each bears fruit, small apples, hanging overhead", 1,
	SPAWN_DFORESTAPPLE },
	//
	{ NULL, 100, 0 }
};

const struct	trait_type	dforestsensetrait_table[] =
{
	{ NULL, 100, 0 },
	{ "the smell of decomposing plant material mixes with the odors of green flora", 1, 0 },
	//{"flowering plants and trees toss their scents to the ##windcondition winds",1,0},
	{ "rising from the ground, the smell of dead leaves and soft dirt is everywhere", 1, 0 },
	{ "the smell of years of dead flora, animal droppings, and verdant flora is present", 1, 0 },
	{ "a constant thrum of quiet sound accompanies the forest life, almost inaudible", 1, 0 },
	{ "with any disturbance, dead leaves and twigs creak at the smallest motion", 1, 0 },
	//{"the occasional hoot or call drifts from afar on ##windcondition winds",1,0},
	//{"the leaves of surrounding plants are ##leafmotion in the ##windcondition wind",1,0},
	//{"the air is ##windcondition, thick with the smell of plants and leaves",1,0},
	{ "the slightest motion against the plethora of dead leaves nearby creates a loud rustling", 1, 0 },
	{ "far in the distance, the intermittent sound of bird-calls is audible", 1, 0 },
	{ "the smells of moist earth and grass drift on the air, accompanied by the thrum of insects", 1, 0 },
	{ "insects buzz in the forest air, creating a backdrop of audible white-noise", 1, 0 },
	{ NULL, 100, 0 }
};








const struct	trait_type	numero_uno[] =
{
	{ NULL, 100, 0 },
	{ "bright colors spring from the undergrowth, tropical shades of red and violet", 11, 0 },
	{ "wide-leafed plants and tall ferns grow low to the ground, brightly colored", 11, 0 },
	{ "the vines and wide-leafed plants grow thick here, a palette of bright flowers and leaves", 11, 0 },
	{ "mosses and mushrooms of all varieties carpet the red clay, showing bright tropical coloring", 11, 0 },
	{ "ferns and spade-leafed shrubs climb out from the moss carpet, tall and colorfully shaded", 11, 0 },
	{ "red, brown, yellow and white, mushroom caps speckle the ground and treetrunks nearby", 11, 0 },
	{ "bright white and violet mushrooms spread thickly across the tree trunks and large plants here", 11, 0 },
	{ "from the ground, a myriad bright plants and decomposers spring forth", 11, 0 },
	{ "though trees dominate the jungle, a bright carpet of varied plantlife carpets the ground here", 11, 0 },
	{ "dense shrubs and vined plantlife crowd between every treetrunk, growing anywhere with clay to root", 11, 0 },
	{ "bright and thick moss climbs every solid surface, spread across trees and most of the red dirt", 11, 0 },
	{ "many of the hundreds of green leaves here are painted with bright reds and oranges and violets", 11, 0 },
	{ "violet and brown-flowered vines climb and hang from trees, creating a tangle of flora here", 11, 0 },
	{ "the branches and limbs of many trees hold aloft hanging, flowered vines of violet and brown", 11, 0 },
	{ "hanging vines and thick undergrowth form a nearly impenetrable tangle here", 11, 0 },
	{ NULL, 100, 0 }
};

const struct	trait_type	numero_dos[] =
{
	{ NULL, 100, 0 },
	{ "from wrist sized to elephantine, the trees of this jungle grow closely together and tall", 11, 0 },
	{ "close together, a myriad large trees climb upward, reaching for the canopy above", 11, 0 },
	{ "thick tree trunks climb high out of the bright undergrowth, toward the canopy", 11, 0 },
	{ "brown and tan, wide and tall, the trees here contrast with the bright jungle undergrowth", 11, 0 },
	{ "sprouting many branches, the vast number of trees here grow to the canopy and out of sight", 11, 0 },
	{ "trees wider than the widest of men grow tall, reaching toward the canopy above", 11, 0 },
	{ "bamboos and thicker trees crowd close, making travel extremely arduous", 11, 0 },
	{ "thick trees, crowded by bamboos and undergrowth, rise high", 11, 0 },
	{ "thick bark covers the trunks of the wide trees here, bamboo filling the gaps between larger trees", 11, 0 },
	{ "throughout the jungle, thick trees of many sizes have grown in close quarters", 11, 0 },
	{ "leaves weigh down the branches growing of the many tall trees, crowding the jungle floor", 11, 0 },
	{ "a straight line here can hardly travel a yard before encountering one of many wide trunks", 11, 0 },
	{ "trees crowd together thickly, their branches intertwined into a verdant mass", 11, 0 },
	{ "green leaves sprout from the many bamboo shoots that grow between massive trees", 11, 0 },
	{ "barrel-wide trunks crowd the jungle, dwarfing the small growth beneath", 11, 0 },
	{ NULL, 100, 0 }
};

const struct	trait_type	numero_tres[] =
{
	{ NULL, 100, 0 },
	{ "far above, the closely-grown trees grow into a closer-woven canopy, thick and dark", 11, 0 },
	{ "hundreds of feet above, the tops of trees and vines have woven into a thick lattice", 11, 0 },
	{ "the canopy looms high overhead, thousands of branches and vines grown together", 11, 0 },
	{ "the jungle vines thicken far overhead, woven amongst the canopy", 11, 0 },
	{ "above, where the trees are highest, a dense canopy forms", 11, 0 },
	{ "the dense canopy overhead strangles out most of the light that would otherwise shine here", 11, 0 },
	{ "far above, a dense canopy begins to form, the vegetation interwoven", 11, 0 },
	{ "green and brown, a verdant canopy resides far above ground level", 11, 0 },
	{ "a deluge of plantlife can be seen above, a dense canopy spreading every direction", 11, 0 },
	{ "the jungle's canopy begins forming above, quickly graduating from thin to dense", 11, 0 },
	{ "vines and interwoven branches, tangled above, create a thick canopy", 11, 0 },
	{ "a seemingly monolithic blanket of tangled foliage covers the jungle, far above", 11, 0 },
	{ "a hundred feet up, the flora is thicker than ground level; a dense canopy covers the jungle", 11, 0 },
	{ "smothering the light from the jungle, a close-knit mass of foliage rests high above", 11, 0 },
	{ "as the trees climb, the foliage thickens, forming a viney and green canopy", 11, 0 },
	{ NULL, 100, 0 }
};

const struct	trait_type	numero_quatro[] =
{
	{ NULL, 100, 0 },
	{ "moist and soft, the reddish clay here has been covered with dead leaves and twigs", 11, 0 },
	{ "decaying and molding leaves cover the red, coppery dirt here", 11, 0 },
	{ "the ground here is covered with a mat of soft, decaying leaves", 11, 0 },
	{ "waterlogged leaves have rotted and formed a mat over the ground, atop which moss grows", 11, 0 },
	{ "years of deadfall from the trees rots underfoot, soft and moist", 11, 0 },
	{ "red clay, moist and malleable, has been spread by numerous animal tracks here", 11, 0 },
	{ "the dirt underfoot is red and coppery, moist to the touch", 11, 0 },
	{ "beneath rotting leaves and mosses, the earth is a coppery red soil", 11, 0 },
	{ "fungi flourish amidst the rotting leaves and vines underfoot", 11, 0 },
	{ "the coppery clay underfoot has been carpeted with dead plant matter over decades", 11, 0 },
	{ "the trees above have dropped thousands of leaves to the coppery dirt, many of which now rot", 11, 0 },
	{ "an inch of dead material covers the clay here, decades of fallen leaves", 11, 0 },
	{ "a layer of steadily decomposing vines and leaves blankets the ground here", 11, 0 },
	{ "red, clayish dirt makes up the earth here, moss grown atop much of it", 11, 0 },
	{ "a mixture of red, coppery dirt and dead leaves coats the ground, rotting", 11, 0 },
	{ NULL, 100, 0 }
};

const struct	trait_type	numero_cinco[] =
{
	{ NULL, 100, 0 },
	{ "the occasional, far-off screech echoes between trees", 11, 0 },
	{ "a gentle thrum, the sound of a myriad small creatures, floats on the air", 11, 0 },
	{ "the air is still, the smells of rotting plant matter rising everywhere", 11, 0 },
	{ "a gentle buzz echoes, the sound of many insects in concert", 11, 0 },
	{ "throughout the jungle, the yelping and clicking sounds of far-off animals can be heard", 11, 0 },
	{ "the air is stiflingly still, carrying the smell of humid plant", 11, 0 },
	{ "rotting leaves and vines offer their odor to the still jungle air", 11, 0 },
	{ "the air is still here, with no breeze penetrating the jungle canopy", 11, 0 },
	{ "humid and still, the air is nearly as thick as the jungle itself", 11, 0 },
	{ "in the distance, the occasional cry of a mammal is audible", 11, 0 },
	{ "birds flap and caw above, the sounds of the canopy drifting downward", 11, 0 },
	{ "water droplets and leaves fall occasionally from the canopy above", 11, 0 },
	{ "the humid air carries the sound of buzzing, clickling insects", 11, 0 },
	{ "from all about, the hundreds of animals join in a choir of small noises", 11, 0 },
	{ "a sickly-sweet odor rises from the mat of rotting leaves and vines on the ground", 11, 0 },
	{ NULL, 100, 0 }
};

//LMARYTH
const struct	trait_type	lmarythstreettrait_table[] =
{
	{ NULL, 100, 0 },
	{ "cobbled together, grey stone bricks compose the well-worn, dusty city streets", 11, 0 },
	{ "the cobblestone streets are aplenty with wheel ruts, missing cobbling, and accumulated dirt", 1, 0 },
	{ "in grey stone and matching mortar the street has been cobbled skillfully, though worn", 1, 0 },
	{ "where grey stone cobbling meets beneath, grey mortar holds the worn bricks together", 1, 0 },
	{ "bricks of grey stone are pieced together beneath, showing ruts and well-worn paths", 1, 0 },
	{ "the mortared gaps of the grey cobblestone beneath are caked with dirt and sand", 1, 0 },
	{ "cobbled together with the hand of a decent mason, the streets beneath are trampled smooth", 1, 0 },
	{ "the streets beneath, while bumpy, have had their cobblestone cracked packed with mud", 1, 0 },
	{ "stones of various size, non-uniform, have been mortared together below to form a cobblestone", 1, 0 },
	{ "the street has been assembled from various-sized stones, forming a relatively smooth cobblestone path", 1, 0 },
	{ "the street has been cobbled in grey and brown stone, its edges lined in dirt and grime", 1, 0 },
	{ NULL, 100, 0 }
};

const struct	trait_type	lmarythfacadestrait_table[] =
{
	{ NULL, 100, 0 },
	{ "the tall houses nearby are wood-built and rickety-looking, but painted brightly", 1, 0 },
	{ "wooden and painted in bright trim, close-built houses crowd the sides of the narrow street", 1, 0 },
	{ "along the narrow streets, houses have been built close, primarily wooden and rough stone", 1, 0 },
	{ "wooden houses with slight tilts and brightly painted sides crowd close along the street", 1, 0 },
	{ "with nary a gap between, tall wooden residences and squat stone buildings crowd the streetsides", 1, 0 },
	{ "built vertically to conserve space, closely-packed and well-worn wooden houses border the city street", 1, 0 },
	{ "the surrounding houses, lacking room horizontally in the crowded city, have been built upward", 1, 0 },
	{ "colorfully painted, splintered wood comprises the trim of the tall, teetering houses along the street", 1, 0 },
	{ "bright in shade and shabby in construction, the tall and painted houses nearby lean precariously", 1, 0 },
	{ "the streets are surrounded by tall but visibly shoddy tenements and teetering homes", 1, 0 },
	{ "beneath, cobblestone has been patched together into a relatively smooth, mud-caked street", 1, 0 },
	{ NULL, 100, 0 }
};

const struct	trait_type	lmarythpalacetrait_table[] =
{
	{ NULL, 100, 0 },
	{ "against the tall northern walls of the city, the Palace is visible soaring above its ramparts", 1, 0 },
	{ "the Marythian Palace is visible, lodged against the city's northernmost wall", 1, 0 },
	{ "to the north, rising far above the both the city walls and its own ramparts, lies the Marythian Palace", 1, 0 },
	{ "the towers and ramparts of the Palace are visible, nestled deep in Maryth's northernmost side", 1, 0 },
	{ "taller even than the Marythian Walls, the Palace Towers rise over Maryth's northern horizon", 1, 0 },
	{ "toward the north of Maryth the Palace towers and ramparts juxtapose against the horizon", 1, 0 },
	{ "from the vantage of the city beneath, the Palace towers crowd the city's northern horizon", 1, 0 },
	{ "cutting the northern horizon, soaring stone spires rise from the Palace grounds", 1, 0 },
	{ "along the northern side of the city, tall walls and towers demarcate the Marythian Palace grounds", 1, 0 },
	{ "surrounded by tall walls of stone, the Marythian Palace can be seen against the city's northern side", 1, 0 },
	{ "in the distance to the north, walls of stone and tall structures denote the Palace grounds", 1, 0 },
	{ NULL, 100, 0 }
};

const struct	trait_type	lmarythwallstrait_table[] =
{
	{ NULL, 100, 0 },
	{ "visible surrounding the entirety of Maryth are tall walls, grey stone, a hundred feet tall", 1, 0 },
	{ "a hundred feet of stone against the sky, tall walls surround the entirety of the city", 1, 0 },
	{ "enclosed, the city's four horizons each are dominated by a tall wall of grey stone", 1, 0 },
	{ "in every direction, the city's edge ends with the view of monstrous grey walls of stone", 1, 0 },
	{ "holding Maryth inside a vague rectangle, walls a hundred feet tall make up the horizons", 1, 0 },
	{ "four walls of stone, a hundred feet each and far apart, encapsulate the entirety of the city", 1, 0 },
	{ "on each of the horizons, the broad-faced, hundred foot walls of Maryth pen in the city", 1, 0 },
	{ "occupying the perimeter of Maryth, one hundred feet of stone wall encloses the town", 1, 0 },
	{ "on each horizon, the rooftops of Maryth are juxtaposed against high-climbing stone walls", 1, 0 },
	{ "the walls of Maryth, one hundred feet tall, lend the horizons a great stone skyline", 1, 0 },
	{ "making a rectangle of Maryth, a stone wall one hundred feet tall surrounds the entire city", 1, 0 },
	{ NULL, 100, 0 }
};

const struct	trait_type	lmarythsensetrait_table[] =
{
	{ NULL, 100, 0 },
	{ "the sounds of a living city create an ever-present backdrop of human voices and movement", 1, 0 },
	{ "smells drift along the city streets, odors of refuse, smoke, and a thousand cooked meals", 1, 0 },
	{ "from far and near, the sounds of traveling carts and pedestrians drift over city rooftops", 1, 0 },
	{ "far-off shouts, tumbling cart-wheels, and creaking woodwork echo through the city", 1, 0 },
	{ "the city air smells of smoke and roasting food, and a myriad odors mixed within", 1, 0 },
	{ "dust and grime drift through the city, accompanied weakly by the smell of humans and their refuse", 1, 0 },
	{ "the clanking, rumblings, and faint words of men and women drift through the distant air", 1, 0 },
	{ "smells of smoke and grime waft along the streets, joined by the smells of men and their food", 1, 0 },
	{ "far-off calls and muffled speech echoes along the walls of nearby facades, unintelligible", 1, 0 },
	{ "smoke-ridden air drifts from chimneys, lending to the streets a permanently burnt odor", 1, 0 },
	{ "like insects humming, the thousands of Marythians occupying the city create ever-present backroud noise", 1, 0 },
	{ "", 1, 0 },
	{ NULL, 100, 0 }
};


const struct	trait_type	hmarythstreettrait_table[] =
{
	{ "joined tightly together, the cobblestone street forms a smooth and unrutted surface beneath", 1, 0 },
	{ "the cobblestone streets bear little wear, their grey stone bricks interlocked smoothly", 1, 0 },
	{ "well-maintained and nearly perfectly even cobblestone lies beneath, neither worn nor grooved", 1, 0 },
	{ "the bricks that compose the road are a matte grey, uncracked, and expertly cobbled", 1, 0 },
	{ "grey stone interlocked into a cobblestone pattern makes for a smooth and clean-swept street", 1, 0 },
	{ "", 1, 0 }
};
const struct	trait_type	hmarythfacadestrait_table[] =
{
	{ "the facades of the nearby houses have been recently painted with mostly pastel shades", 1, 0 },
	{ "fresh bouts of paint decorate the nearby houses, and large windows are plentiful nearby", 1, 0 },
	{ "bay windows and ornate iron-wrought fences line the streets, their houses tall and trim", 1, 0 },
	{ "the smooth streets and curbs here cede to the fronts of tall, well-painted homes and buildings", 1, 0 },
	{ "well-trimmed window bays and ornately-carved doors lead into the interiors of the surrounding houses", 1, 0 },
};
const struct	trait_type	hmarythsensetrait_table[] =
{
	{ "out of place in the city, the smell of gardens and trees can be smelled behind faint odors of smoke", 1, 0 },
	{ "the city air smells of smoke and grit, but fresh paint and flowers drift on the wind here too", 1, 0 },
	{ "though the city at large rings with an everpresent hum, the streets here have a gentle quietness to them", 1, 0 },
	{ "shattered by the occasional shout or clank, few sounds echo along the cobblestone streets", 1, 0 },
	{ "though the occasional sound drifts down the smooth street, it is quiet enough to make out quiet birdcalls", 1, 0 },
};


const struct	trait_type	lmaryth_lightconditions[] =
{
	{ NULL, 100, 0 },
	{ "the occasional, far-off screech echoes between trees", 11, 0 },
	{ NULL, 100, 0 }
};
