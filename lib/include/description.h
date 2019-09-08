/* global purpose messages or descriptions */
#ifndef DESCRIPTION_H
#define DESCRIPTION_H

#define GENERAL_SUBDESC "/base/misc/subdesc"
#define ADD_GENERAL_SUBDESC(s, id, o) \
(GENERAL_SUBDESC->add_general_subdesc(s, id, o))
#define REMOVE_GENERAL_SUBDESC(id, o) \
(GENERAL_SUBDESC->remove_general_subdesc(id, o))

/* subdesc id's for aura spells */
#define AURA_DESC      "_aura_" 
#define SKINNED_DESC   "_skinned_" 
#define BUTCHERED_DESC "_butchered_" 

/* globally used descriptions */

#define POWER_DESC_1 " looks powerful."
#define POWER_DESC_2 " looks very powerful."
#define POWER_DESC_3 " looks like a divine being."

#endif
