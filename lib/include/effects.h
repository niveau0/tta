#ifndef EFFECTS_H
#define EFFECTS_H

#define POISON_DIR           "/lib/poison/"
#define DISEASE_DIR          "/lib/disease/"
#define CURSE_DIR            "/lib/curse/"

/* effect type */
#define POISON_EFFECT   1
#define DISEASE_EFFECT  2
#define CURSE_EFFECT    3
#define BLIND_EFFECT    4
#define DEAF_EFFECT     5
#define PARALYSE_EFFECT 6

/* effect source/reason */
#define EFF_UNCONSCIOUS        1
#define EFF_MEDITATE           2
#define EFF_SLEEP              3
#define EFF_LOST_EYES          4
#define EFF_DEATH              5
#define EFF_UL_ACAANA        100 /* paralysing poison */
#define EFF_TRUSA            101 /* blinding poison */
#define EFF_DYNALLCA         102 /* deafening poison */
#define EFF_HENUIAL          103 /* blinding poison */
#define EFF_MUILFANA         104 /* blinding poison */
#define EFF_ZAGANZAR         105 /* blinding poison */
#define EFF_CURSE_BLIND      106 /* blinding curse */
#define EFF_CURSE_DEAF       107 /* deafening curse */
#define EFF_CURSE_PARALYSE   108 /* paralysing curse */
#define EFF_FUMBLE_PARALYSE  109 /* paralysed by a fumble */
#define EFF_SILO             110 /* deafing spell */

/*
 * the following macros require a living as argument and
 * a resistance modifier
 */

/* poison */
#define POISON_ACAANA(l, m) \
(POISON_DIR + "acaana")->poison_player(l, m);
#define POISON_ASGURASH(l, m) \
(POISON_DIR + "asgurash")->poison_player(l, m);
#define POISON_BRAGOLITH(l, m) \
(POISON_DIR + "bragolith")->poison_player(l, m);
#define POISON_BRITHAGURTH(l, m) \
(POISON_DIR + "brithagurth")->poison_player(l, m);
#define POISON_CARNEGURTH(l, m) \
(POISON_DIR + "carnegurth")->poison_player(l, m);
#define POISON_DYNALLCA(l, m) \
(POISON_DIR + "dynallca")->poison_player(l, m);
#define POISON_GALENAANA(l, m) \
(POISON_DIR + "galenaana")->poison_player(l, m);
#define POISON_HENUIAL(l, m) \
(POISON_DIR + "henuial")->poison_player(l, m);
#define POISON_JEGGA(l, m) \
(POISON_DIR + "jegga")->poison_player(l, m);
#define POISON_JEGGARUKH(l, m) \
(POISON_DIR + "jeggarukh")->poison_player(l, m);
#define POISON_JITSU(l, m) \
(POISON_DIR + "jitsu")->poison_player(l, m);
#define POISON_JITSUTYR(l, m) \
(POISON_DIR + "jitsutyr")->poison_player(l, m);
#define POISON_KARFAR(l, m) \
(POISON_DIR + "karfar")->poison_player(l, m);
#define POISON_KLY(l, m) \
(POISON_DIR + "kly")->poison_player(l, m);
#define POISON_KLYTUN(l, m) \
(POISON_DIR + "klytun")->poison_player(l, m);
#define POISON_MUILFANA(l, m) \
(POISON_DIR + "muilfana")->poison_player(l, m);
#define POISON_NIMNAUR(l, m) \
(POISON_DIR + "nimnaur")->poison_player(l, m);
#define POISON_ONDOHITHUI(l, m) \
(POISON_DIR + "ondohithui")->poison_player(l, m);
#define POISON_SHARKASAR(l, m) \
(POISON_DIR + "sharkasar")->poison_player(l, m);
#define POISON_SILMAANA(l, m) \
(POISON_DIR + "silmaana")->poison_player(l, m);
#define POISON_TAYNAGA(l, m) \
(POISON_DIR + "taynaga")->poison_player(l, m);
#define POISON_TRUSA(l, m) \
(POISON_DIR + "trusa")->poison_player(l, m);
#define POISON_UL_ACAANA(l, m) \
(POISON_DIR + "ul_acaana")->poison_player(l, m);
#define POISON_URAANA(l, m) \
(POISON_DIR + "uraana")->poison_player(l, m);
#define POISON_VURAANA(l, m) \
(POISON_DIR + "vuraana")->poison_player(l, m);
#define POISON_ZAGANZAR(l, m) \
(POISON_DIR + "zaganzar")->poison_player(l, m);

/* disease */
#define DISEASE_COLD(l, m) \
(DISEASE_DIR + "cold")->infect_player(l, m);
#define DISEASE_TBC(l, m) \
(DISEASE_DIR + "tbc")->infect_player(l, m);
#define DISEASE_TETANUS(l, m, slot) \
(DISEASE_DIR + "tetanus")->infect_player(l, m); \
(DISEASE_DIR + "tetanus")->infected_slot(l, slot);
#define DISEASE_UPSET_STOMACH(l, m) \
(DISEASE_DIR + "upset_stomach")->infect_player(l, m);

/* curse */
#define CURSE_PACIFIST(l, m) \
(CURSE_DIR + "pacifist")->curse_player(l, m);
#define CURSE_KLEPTOMANIA(l, m) \
(CURSE_DIR + "kleptomania")->curse_player(l, m);
#define CURSE_BLIND(l, m) \
(CURSE_DIR + "blind")->curse_player(l, m);
#define CURSE_DEAF(l, m) \
(CURSE_DIR + "deaf")->curse_player(l, m);
#define CURSE_PARALYSE(l, m) \
(CURSE_DIR + "paralyse")->curse_player(l, m);
#define CURSE_PARANOIA(l, m) \
(CURSE_DIR + "paranoia")->curse_player(l, m);
#define CURSE_FORGETFULNESS(l, m) \
(CURSE_DIR + "forgetfulness")->curse_player(l, m);
#define CURSE_ROUGHNESS(l, m) \
(CURSE_DIR + "roughness")->curse_player(l, m);
#define CURSE_SLOW(l, m) \
(CURSE_DIR + "slow")->curse_player(l, m);
#define CURSE_DROPPING_YOKEL(l, m) \
(CURSE_DIR + "dropping_yokel")->curse_player(l, m);
#endif
