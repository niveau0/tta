/*
 * handle liquid effects such as potions or poison
 */

#include <effects.h>
#include <event.h>
#include <liquid.h>
#include <macros.h>
#include <mudtime.h>
#include <slots.h>
#include <properties.h>

static int check_amount(int needed, int amount, object tp);
static void poison_removal(object tp, string *poisons, int chance);
static void decrease_invis_level(object tp);
static void decrease_see_invis_level(object tp);
static void decrease_infravision(object tp);

#include "other.c"
#include "potion.c"
#include "alcohol.c"

/*
 * Function name: drink_liquid
 * Description:   drink some liquid (called from /base/drink.c)
 * Arguments:     ltype - what type of liquid (see <liquid.h>)
 *                amount - how much has been drunk
 *                tp - the acting player
 */
nomask void
drink_liquid(int type, int amount, object tp)
{
    if (!tp)
	return;
    
    switch (type)
    {
	/* alcohol */
      case LIQ_BEER:
	beer(amount, tp);
	break;
      case LIQ_WINE:
	wine(amount, tp);
	break;

	/* potions */
      case LIQ_BAALAK: /* shatter repair */
	shatter_repair(amount, tp);
	break;
      case LIQ_BURSTHELAS: /* shatter repair */
	shatter_repair(amount, tp);
	break;
      case LIQ_FEK: /* stop bleeding */
	fek(amount, tp);
	break;
      case LIQ_HOLY_WATER:
	holy_water(amount, tp);
	break;
      case LIQ_MENELAR: /* poison antidote */
	menelar(amount, tp);
	break;
      case LIQ_ELDAANA: /* herb poison antidote */
	eldaana(amount, tp);
	break;
      case LIQ_QUILMUFUR: /* herb poison antidote */
	quilmufur(amount, tp);
	break;
      case LIQ_REGLEN: /* healing potion */
	reglen(amount, tp);
	break;
      case LIQ_REWK: /* healing potion */
	rewk(amount, tp);
	break;
      case LIQ_THURL: /* healing potion */
	thurl(amount, tp);
	break;
      case LIQ_DRAAK: /* turn invisible potion */
	turn_invisible(amount, tp);
	break;
      case LIQ_KARAD: /* see invisible potion */
	see_invisible(amount, tp);
	break;
      case LIQ_FEROL: /* infravision potion */
	infravision(amount, tp);
	break;
	      
	/* other */
      case LIQ_BREWING:
	brewing(amount, tp);
	break;
      case LIQ_JUICE:
	juice(amount, tp);
	break;
      case LIQ_MILK:
	milk(amount, tp);
	break;
      case LIQ_OIL:
	oil(amount, tp);
	break;
      case LIQ_SOUP:
	soup(amount, tp);
        break;
      case LIQ_VINEGAR:
	vinegar(amount, tp);
	break;
      case LIQ_WATER:
	water(amount, tp);
	break;
      default:
	return;
    }
}

/*
 * Function name: check_amount
 * Description:   check if enough has been drunk
 * Arguments:     needed - how much liquid must be drunk
 *                amount - how much has been drunk
 *                tp - drinking player
 * Returns:       1 if success, 0 else
 */
static int
check_amount(int needed, int amount, object tp)
{
    if (amount < needed)
    {
	tp->catch_tell("You didn't drink enough of the liquid to "+
		       "get its effect.\n");
	return 0;
    }
    return 1;
}

/*
 * Function name: poison_removal
 * Description:   remove the first poison of the list, if found
 * Arguments:     tp - the poisoned, drinking player
 *                poisons - the list of affected poisons
 *                chance - chance to get a cleaning effect
 */
static void
poison_removal(object tp, string *poisons, int chance)
{
    int     i, sz;
    string  *files;
    mapping poison;
    
    if (!mappingp(poison = tp->query_effects()[POISON_EFFECT]))
	return;
    if (!(sz = sizeof(files = map_indices(poison))))
	return;
    for (i=0; i<sz; i++)
    {
        if (IN_ARRAY((files[i] = "/lib/poison/" + files[i]), poisons))
            break;
    }
    if (i == sz || (random(chance) - random(files[i]->query_level()) <= 0))
    {
        tp->catch_tell("Nothing seems to happen.\n");
        tell_objects(LISTEN(E(tp)) - ({ tp }),
		     ({ QCTNAME(tp), " seems to be disappointed.\n" }),
	             MSG_SEE, tp);
    }
    else
    {
        files[i]->remove_poison(tp);
        tp->catch_tell("You feel your body cleaned from the poison.\n");
        tell_objects(LISTEN(E(tp)) - ({ tp }),
		     ({ QCTNAME(tp), " seems to be glad about something.\n" }),
                     MSG_SEE);
    }
}

/*
 * Function name: decrease_invis_level
 * Description:   decrease the invisibility-level of a player, he got
 *                by a invisibility potion
 * Arguments:     tp - the invisible player
 */

static void
decrease_invis_level(object tp)
{
    tp->change_int_prop(OBJ_INVIS, -1);
    
    if (!tp->query_prop(OBJ_INVIS))
    {
	tp->catch_tell("You suddenly turn visible.\n");
	tell_objects(LISTEN(E(tp)) - ({ tp }),
		     ({ QCTNAME(tp)," suddenly appears.\n"}), MSG_SEE);
    }
}

/*
 * Function name: decrease_invis_level
 * Description:   decrease the see-invisibility-level of a player, he got
 *                by a see-invisibility potion
 * Arguments:     tp - the drinking player
 */

static void
decrease_see_invis_level(object tp)
{
    tp->change_int_prop(LIVE_SEE_INVIS);
    
    if (!tp->query_prop(LIVE_SEE_INVIS))
	tp->catch_tell("Your eyes stop tingling.\n");
}
/*
 * Function name: decrease_infravision
 * Description:   decrease the infravision-level of a player, which he got
 *                by a infravision potion
 * Arguments:     tp - the drinking player
 */

static void
decrease_infravision(object tp)
{
    tp->change_int_prop(LIVE_SEE_DARK, -1);
    
    if (!tp->query_prop(LIVE_SEE_DARK))
	tp->catch_tell("Your eyes stop glowing.\n");
}
