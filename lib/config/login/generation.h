#define GEN_EMAIL  1
#define GEN_RACE   2
#define GEN_DESC   3
#define GEN_GENDER 4
#define GEN_TALENT 5
#define GEN_CAREER 6
#define GEN_NAME   7
#define GEN_REALM  8

#define ELF \
"Elves are the first born race on Arda. Together with the humans they are\n"+\
"called 'the children of Iluvater'. All elves do not die due to aging,\n"+\
"they will live forever if they don't get tired by life. They are very\n"+\
"beautiful and their voices sound like they were always singing.\n"+\
"Elves can see and hear very well and also they are resistant to disease.\n"+\
"But the population of elves has grown very thin since the first age, so\n"+\
"sometimes the elves are just a mystery to other races.\n"

#define HUMAN \
"Humans are the second born on Arda and also, like the elves, called 'the\n"+\
"children of Iluvatar'. There are many different kinds of humans, all with\n"+\
"different advantages. Humans received the gift of death at their creation\n"+\
"so they might die of age if they don't expand their life by magic.\n"

#define HOBBIT \
"What is a hobbit? The hobbits are likely as the dwarves for some people,\n"+\
"because a hobbit is short, even shorter as a dwarf, and tenacious. But\n"+\
"they don't share the same lust for gold and richness, or better, they\n"+\
"see a good pipeweed and a huge meal as wealth. Also they love to sing\n"+\
"and write poems about everything.\n"

#define DWARF \
"Dwarves were born after elves and humans because they were first a secret\n"+\
"creation of the Vala Aule, who couldn't wait until the 'children of\n"+\
"Iluvatar' awake. But his work was discovered and after the dwarves got\n"+\
"their own breath of life by Iluvatar, Aule promised that they will sleep\n"+\
"until the elves and humans live on Arda.\n"+\
"Dwarves are short and very tenacious. They are good fighters and can get\n"+\
"sort of greedy if there is some great metal to acquire, especially the\n"+\
"mithril (magic silver). Their love for gold and jewels is unique save\n"+\
"elves. A bad fate laid on the dwarven clans, cause they lost all their\n"+\
"big cities during the third age.\n"

/***********************************************************************/

#define ARCTICMAN \
"A human race from far north.  They speak westron and some adunaic. The\n"+\
"arcticmen were born in the cold regions and therefore the constitutional\n"+\
"shape is good. They are not the biggest of men and known for rude\n"+\
"behaviour.\n"

#define DUNEDAN \
"They are the heirs of the kings of men, the Numenoreans of the three big\n"+\
"houses. But after the fall of the isle of Numenor and the battles against\n"+\
"Sauron their number has decreased and the real Dunedain are rarely seen.\n"+\
"The Dunedain carry some elven blood in their lines.\n"

#define HALFELF \
"Halfelves as said by the name are neither fully elf nor fully men. They\n"+\
"are the rare children raised from the love between an elf and a human.\n"+\
"Halfelfs are free to choose if they want to be immortal or perish after\n"+\
"a live as mortal man.\n"

#define NOLDO \
"The also called highelves are decendents of those that have seen the\n"+\
"trees of Valinor. The noldor are skilled craftsmen and love jewels and\n"+\
"treasure.\n"

#define NORTHMAN \
"Northman are the common inhabitants of northern Middleearth, that is\n"+\
"mostly the kingdom of Gondor and Arnor. Their line is a mixture between\n"+\
"the eastern races and the dunedain.\n"

#define SINDA \
"The sindar are decendents of the grey elves from the ancient kingdom\n"+\
"Doriath once ruled by Elu Thingol. They developed their own tongue, the\n"+\
"Sindarin which is now mainly spoken by all elves in middleearth. Only the\n"+\
"Noldor did not abandon the use of Quenya language (which is now also used\n"+\
"for poetry.\n"

#define VARIAG \
"This is one kind of the many different tribes of the easterlings. There\n"+\
"is not much to say about Variags, they speak westron like the most.\n"

#define WOODELF \
"The Avari, the darkelves, that have never seen the blessed lands or the\n"+\
"two trees. They did not follow Orome the Vala in the first age and\n"+\
"dwelled forever in middleearth.\n"

/***********************************************************************/

#define RACEGENERAL \
({ "elf", "human", "hobbit", "dwarf" })

#define RACEDESC \
([\
 "elf"       : ELF,\
 "human"     : HUMAN,\
 "hobbit"    : HOBBIT,\
 "dwarf"     : DWARF,\
 "arcticman" : ARCTICMAN,\
 "dunedan"   : DUNEDAN,\
 "halfelf"   : HALFELF,\
 "noldo"     : NOLDO,\
 "northman"  : NORTHMAN,\
 "sinda"     : SINDA,\
 "variag"    : VARIAG,\
 "woodelf"   : WOODELF,\
    ])

#define SUBRACE \
([\
 "elf"  : ({ "halfelf", "noldo", "sinda", "woodelf" }),\
 "human": ({ "arcticman", "dunedan", "northman", "variag" }),\
    ])

#define TALENTS \
({ "academic", "art", "athletic", "craft", "magic", "nature", })

/* possible maximum startskills for each talent 
 * general skills are available for everyone but can be overloaded by
 * the following talent-lists (e.g. climb is better for an athletic person)
 */
#define STARTSKILLS \
([\
 "general"  : ([\
	       SK_CLIMB            : 1,\
	       SK_ATTACK_BARE_FIST : 2,\
	       SK_ATTACK_DAGGER    : 1,\
	       SK_PERCEPTION       : 2,\
	       SK_FIRST_AID        : 1,\
     ]),\
 "academic" : ([\
	       SK_FAUNA_LORE       : 2,\
	       SK_FLORA_LORE       : 2,\
	       SK_HERB_LORE        : 1,\
	       SK_LOCK_LORE        : 2,\
	       SK_MAPPING          : 3,\
	       SK_MATHEMATICS      : 4,\
	       SK_NAVIGATION       : 3,\
	       SK_POISON_LORE      : 2,\
	       SK_WEATHER_WATCH    : 3,\
	       SK_FIRST_AID        : 2,\
	       SK_SECOND_AID       : 1,\
	       SK_DIAGNOSTICS      : 1,\
	       SK_APPRAISAL        : 1,\
     ]),\
 "art"      : ([\
	       SK_APPRAISAL        : 1,\
	       SK_DANCE            : 1,\
	       SK_PLAY_DRUM   	   : 1,\
	       SK_PLAY_FLUTE  	   : 3,\
	       SK_PLAY_GUITAR 	   : 3,\
	       SK_PLAY_HARP   	   : 1,\
	       SK_PLAY_HORN   	   : 2,\
	       SK_PLAY_LYRE   	   : 3,\
	       SK_PLAY_TRUMPET	   : 1,\
	       SK_PLAY_VIOLIN 	   : 1,\
	       SK_SINGING          : 3,\
	       SK_TALE_TELLING     : 1,\
	       SK_EVAL_ARMOR       : 1,\
	       SK_EVAL_WEAPON	   : 1,\
     ]),\
 "athletic" : ([\
               SK_ACROBATICS       : 2,\
	       SK_CLIMB       	   : 3,\
	       SK_CONTORTIONS 	   : 1,\
	       SK_DISTANCE_RUN	   : 3,\
	       SK_JUMPING     	   : 3,\
	       SK_RIDE        	   : 2,\
	       SK_SPRINTING    	   : 3,\
	       SK_SWIM             : 3,\
     ]),\
 "craft"    : ([\
	       SK_APPRAISAL        : 1,\
	       SK_TRADING          : 2,\
	       SK_LEATHER_WORK     : 4,\
	       SK_MINING       	   : 2,\
	       SK_SAILING      	   : 2,\
	       SK_WOOD_CRAFT       : 4,\
	       SK_RIGID_LEATHER    : 2,\
	       SK_SOFT_LEATHER     : 2,\
	       SK_FLETCHING        : 3,\
	       SK_ROPE_MASTERY     : 2,\
	       SK_EVAL_ARMOR       : 2,\
	       SK_EVAL_WEAPON	   : 2,\
     ]),\
 "magic"    : ([\
	       SK_CHANNELING       : 3,\
	       SK_POWER_PROJ  	   : 3,\
	       SK_RUNES       	   : 3,\
	       SK_SENSE_MAGIC 	   : 3,\
	       SK_STAVES_WANDS     : 3,\
	       SK_WARDING_LORE     : 3,\
     ]),\
 "nature"   : ([\
	       SK_FAUNA_LORE       : 4,\
	       SK_FLORA_LORE       : 4,\
	       SK_HERB_LORE        : 3,\
	       SK_MAPPING          : 2,\
	       SK_NAVIGATION       : 2,\
	       SK_WEATHER_WATCH    : 4,\
	       SK_HIDE             : 2,\
	       SK_ANIMAL_HEAL      : 2,\
	       SK_COOKERY          : 2,\
	       SK_READ_TRACKS      : 2,\
	       SK_FLETCHING        : 1,\
	       SK_SET_TRAPS        : 1,\
	       SK_STALK            : 1,\
	       SK_SLA              : 1,\
     ]),\
    ])

/***********************************************************************/
#define REALMDESC \
([\
 "cha" : "This is the power of the deities channeled to the world through\n"+\
 "their followers.  It is all of religious nature and independent\n"+\
 "of the essence. Channeling power from the deity doesn't require\n"+\
 "the direct  cooperation of  the deity.  But acting  against the\n"+\
 "goals and nature of the clerics religion may lead to innability\n"+\
 "to cast.\n",\
 \
 "ess" : "The power directly drawn from the essence that is all around.\n"+\
 "Therefore  'ess' spells are  almost elemental in nature (fire-,\n"+\
 "earth-, water-, wind- or lightbased, etc.).  Inert material as\n"+\
 "metal armor increases the difficulty in manipulating the essence.\n",\
 \
 "men" : "Powers drawn from the essence and channeled through the mind of\n"+\
 "the magic user. 'men' is very  personal power.  Spells from the\n"+\
 "mind interfere with head cover (like helmets).\n",\
 \
 "arc" : "The ancient knowledge of magic, the ways of the arcane mage, are\n"+\
 "mostly unknown.  But it is said  that arcane  magic combines the	\n"+\
 "three realms 'cha', 'ess', 'men' and this was the origin of magic\n"+\
 "before it was split.  Arcane  mages could draw  their power from \n"+\
 "every source.\n",\
    ])
