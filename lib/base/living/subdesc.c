#include <composite.h>
#include <config.h>
#include <description.h>
#include <living.h>
#include <macros.h>
#include <race.h>
#include <skills.h>
#include <slots.h>
#include <properties.h>
#include "prototypes.h"

private mapping subdesc;

/*
 * Function name: add_subdesc
 * Description:   add an object that creates extra description
 * Arguments:     id - integer id of the extra description
 *                     (see <description.h>)
 *                ob - the object to call for the description
 */
nomask void
add_subdesc(string id, object ob)
{
    if (!subdesc)
	subdesc = ([ ]);
    subdesc[id] = ob;
}

/*
 * Function name: remove_subdesc
 * Description:   remove an object that creates extra description
 * Arguments:     id - the subdescription id to remove
 */
nomask void
remove_subdesc(string id)
{
    if (subdesc)
	subdesc[id] = 0;
}

/*
 * Function name: query_subdesc_map
 * Description:   gives the subdescription mapping
 * Returns:       mapping ([ id : object, ... ])
 */
nomask mapping
query_subdesc_map()
{
    return subdesc;
}

/*
 * Function name: query_subdescs
 * Description:   gives subdescription string for all subdescs
 * Arguments:     ob - looking object
 * Returns:       string, 0 if no subdescriptions  
 */
nomask string
query_subdescs(object ob)
{
    int    i;
    string res;
    mixed  ids;
    
    if (!subdesc)
	return "";
    ids = map_indices(subdesc);
    res = "";
    for (i=sizeof(ids); i--;)
	res += subdesc[ids[i]]->subdesc(this_object(), ob, ids[i]);
    return res;
}

nomask string
query_wound_desc(object ob)
{
    int     i, sz, curr, *ind;
    string  pron, res, *missing, *broken, *shatter, *unusable;
    mapping wounds;
    
    wounds = query_wounds();
    if (!(sz = sizeof(ind = map_indices(wounds))))
	return "";
    res = "";
    missing  = allocate(sz); 
    shatter  = allocate(sz); 
    broken   = allocate(sz); 
    unusable = allocate(sz); 
    for (i=sz; i--;)
    {
	if ((curr = wounds[ind[i]]) == SLOT_LOST)
	    missing[i] = query_slot_desc(ind[i]);
	else if (curr == SLOT_UNUSABLE)
	    unusable[i] = query_slot_desc(ind[i]);
	else if (curr == SLOT_SHATTERED)
	    shatter[i] = query_slot_desc(ind[i]);
	else
	    broken[i] = query_slot_desc(ind[i]);
    }
    missing  -= ({ 0 });
    unusable -= ({ 0 });
    shatter  -= ({ 0 });
    broken   -= ({ 0 });
    if (sizeof(missing))
	res += BS((ob == this_object() ? "You are missing your "
		   : (capitalize(query_pronoun()) + " is missing " +
		      query_possessive() + " ")) +
		  COMPOSITE_WORDS(missing), 75, 0) + ".\n";
    if (sz = sizeof(shatter))
	res += BS((ob == this_object() ? "Your " :
		   capitalize(query_possessive()) + " ") +
		  COMPOSITE_WORDS(shatter) + (sz > 1 ? " are " : " is ") +
		  "shattered", 75, 0) + ".\n";
    if (sz = sizeof(broken))
	res += BS((ob == this_object() ? "Your " :
		   capitalize(query_possessive()) + " ") +
		  COMPOSITE_WORDS(broken) + (sz > 1 ? " are " : " is ") +
		  "broken", 75, 0) + ".\n";
    if (ob == this_object() && (sz = sizeof(unusable)))
	res += BS("Your " + COMPOSITE_WORDS(unusable) +
		  (sz > 1 ? " are " : " is ") +
		  "somehow paralysed", 75, 0) + ".\n";
    return res;
}

nomask string
query_blood_desc(object ob)
{
    int     i, sz;
    mapping bl;
    mixed   ind;
    
    if (map_sizeof(bl = query_bloodwounds()))
    {
	if (query_prop(LIVE_BLOODFLOW_STOP) < query_bloodloss())
	{
	    
	    ind = map_indices(bl);
	    for (i=0, sz=sizeof(ind); i<sz; i++)
		ind[i] = query_slot_desc(ind[i]);
	    return ((ob == this_object()
		     ? "Your " : capitalize(query_possessive()) + " ") +
		    COMPOSITE_WORDS(ind) + (sz == 1 ? " is" : " are") +
		    " bleeding.\n");
	}
	return ((ob == this_object()
		 ? "Your " : capitalize(query_possessive()) + " ") +
		"wounds are open but bleeding has been stopped.\n");
    }
    return "";
}

nomask string
extra_desc(object ob)
{
    int     i, diff;
    string  pron, res;
    
    res = "";
    if (ob == this_object())
	pron = "You";
    else
    {
	int *sizes;
	
	pron = capitalize(query_pronoun());

	if (sizes = RACE_SIZES[query_race()])
	{
	    switch (100 * (query_size()-sizes[0]) / (sizes[1]-sizes[0]))
	    {
	      case -1000..20:
		  res += pron + " a very small " + query_race() + ".\n";
		  break;
	      case 21..40:
		  res += pron + " a small " + query_race() + ".\n";
		  break;
	      case 41..70:
		  res += pron + " is a medium sized " + query_race() + ".\n";
		  break;
	      case 71..90:
		  res += pron + " is a tall " + query_race() + ".\n";
		  break;
	      case 91..100:
		  res += pron + " is a very tall " + query_race() + ".\n";
		  break;
	      case 101..1000:
		  res += pron + " is an unnaturally tall " +
			 query_race() + ".\n";
		  break;
	    }
	}
	switch (query_stat(STAT_ST))
	{
	  case -1000..20:
	      res += pron + " looks somehow weak.\n"; break;
	  case 71..90:
	      res += pron + " seems to be sturdy.\n"; break;
	  case 91..102:
	      res += pron + " looks quite strong.\n"; break;
	  case 103..1000:
	      res += pron + " looks unnaturally strong.\n"; break;
	}
    }
    
    if (!query_linkdead() && !living(this_object()))
	return query_subdescs(ob) +
	    pron + (pron == "You" ? " are " : " is ") + "obviously dead.\n";
    
    switch (query_stat(STAT_AP))
    {
      case 0..19:
	  res += pron + " look" + (pron == "You" ? " " : "s ") +
		 "loathsome.\n";
	  break;
      case 20..39:
	  res += pron + " look" + (pron == "You" ? " " : "s ") + "ugly.\n";
	  break;
      case 40..59:
	  res += pron + (pron == "You" ? " are " : " is ") +
		 "of average beauty.\n";
	  break;
      case 60..79:
	  res += pron + (pron == "You" ? " are " : " is ") + "beautiful.\n";
	  break;
      case 80..92:
	  res += pron + (pron == "You" ? " are " : " is ") +
		 "extremely beautiful.\n";
	  break;
      case 93..97:
	  res += pron + (pron == "You" ? " are " : " is ") +
		 "of exceptional beauty.\n";
	  break;
      case 98..101:
	  res += pron + " look" + (pron == "You" ? " " : "s ") +
		 "like the image of perfection.\n";
	  break;
      default:
	  res += pron + " look" + (pron == "You" ? " " : "s ") +
		 "so wonderful, like only gods do.\n";
    }

    if ((i = query_max_hp()) < 1)
	i = 1;
    i = query_hp() * 4 / i;
    if (i < 0) i = 0;
    switch (i)
    {
      case 0:
	res += pron + (pron == "You" ? " are " : " is ") +
	    "in a very bad shape.\n";
	break;
      case 1:
	res += pron + (pron == "You" ? " are " : " is ") +
	    "in a bad shape.\n";
	break;
      case 2:
	res += pron + (pron == "You" ? " seem " : " seems ") +
	    "to be quite wounded.\n";
	break;
      case 3:
	res += pron + (pron == "You" ? " seem " : " seems ") +
	    "to be somewhat hurt.\n";
	break;
      default:
	res += pron + (pron == "You" ? " are " : " is ") +
	    "in a good shape.\n";
    }
    res += query_blood_desc(ob) + query_wound_desc(ob) + query_subdescs(ob);
    if (subdesc && subdesc[AURA_DESC])
	return res;
    if (ob == this_object())
	return res;
    diff = LEARN_OB->query_cost_modifier(SK_GRP_COVERED, this_object());
    if (diff < 1)
	return res;
    diff = LEARN_OB->query_cost_modifier(SK_GRP_COVERED, ob) * 100 / diff;
    
    switch (diff)
    {
      case 0..19:
	  res += pron + POWER_DESC_3 + "\n";
	  break;
      case 20..49:
	  res += pron + POWER_DESC_2 + "\n";
	  break;
      case 50..80:
	  res += pron + POWER_DESC_1 + "\n";
	  break;
    }
    
    return res;
}
