/* 
 * Created: 14.11.1996 - Mandos (chuzakh@informatik.uni-bremen.de)
 * To create a dice of your own, just inherit this one, and use the
 * set_dice() method to set the number of sides the dice has.
 */

inherit "/base/object";

#include <event.h>
#include <macros.h>
#include <material.h>
#include <properties.h>

static int dice;

/*
 * Function name: create_object
 * Description:   creates the object ;)
 */
void
create_object()
{
    dice = 6;
    set_short("dice");
    set_name("dice");
    set_pname("dices");
    set_pshort("dices");
    set_lwvs(0, 5, 10, 1);
    add_prop(OBJ_MATERIAL, M_STONE);
}

/*
 * Function name: set_dice
 * Description:   sets the dice-sides
 * Returns:       nothing
 */
nomask void
set_dice(int arg)
{
    dice = arg;
}

/*
 * Function name: query_dice
 * Description:   returns the dice-sides
 * Returns:       the dice-sides
 */
nomask int
query_dice()
{
    return dice;
}

/*
 * Function name: long
 * Description:   shows the long description
 * Returns:       the long of the object
 */
string
long()
{
    return "This is a dice with " + dice + " sides.";
}

/*
 * Function name: roll_cmd
 * Description:   performs a dice-roll
 * Returns:       1
 */
nomask int
roll_cmd()
{
    int    result;
    object tp;
    
    tp = this_player();
    result = random(dice)+1;
    
    tp->catch_tell("You roll the dice. The result is " + result + ".\n");
    tell_objects(LISTEN(E(tp)) - ({ tp }),
		 ({ QCTNAME(tp), " rolls a dice. The result is " +
			result + ".\n" }), MSG_SEE, this_object());
    return 1;
}
