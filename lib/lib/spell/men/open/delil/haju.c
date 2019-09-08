inherit "../../men_base";

#include <combat.h>
#include <macros.h>
#include <magic.h>
#include <skills.h>
#include <properties.h>
#include <language.h>
#include <weapon.h>

string
query_desc()
{
    return "Tells the purpose of the item it is cast upon.";
}

mixed
query_similar_code()
{
    return "_covra"; // AENDERN !
}

void
spell_effect(int level, object *tar, object tp)
{
    object ob;
    string object_name;
    int missile_type;

    if ((!sizeof(tar)) || (tar[0]->query_prop(IS_ROOM)))
	return;
    if (living(tar[0]))
    {
        tp->catch_tell("You cannot cast this spell on livings.\n");
	return;
    }
    
    object_name=LANG_ADDART(tar[0]->query_name());

    if ((tar[0]->query_prop(IS_CONT)) ||
	(tar[0]->query_prop(IS_HEAP)) ||
	(tar[0]->query_prop(IS_HERB)) ||
	(tar[0]->query_prop(IS_PLANT)) ||
	(tar[0]->query_prop(IS_ARMOR)) ||
	(tar[0]->query_prop(IS_WEAPON)) ||
	(tar[0]->query_prop(IS_MISSILE_WEAPON)) ||
	(tar[0]->query_prop(IS_SHIELD)) ||
	(tar[0]->query_prop(IS_KEY)) ||
	(tar[0]->query_prop(IS_AMMO)))
    {
	tp->catch_tell("You get the following insight on the purpose of " +
		       object_name + ":\n");
	if (tar[0]->query_prop(IS_CONT))
	    tp->catch_tell("You can put things into " + object_name + ".\n");
    
	if (tar[0]->query_prop(IS_HEAP))
	    tp->catch_tell("You usually have more than one " + tar[0]->query_name() + ".\n");

	if (tar[0]->query_prop(IS_HERB))
	    tp->catch_tell(capitalize(object_name) + " is some kind of herb.\n");

	if (tar[0]->query_prop(IS_PLANT))
	    tp->catch_tell(capitalize(object_name) + " is a plant.\n");

	if (tar[0]->query_prop(IS_ARMOR))
	    tp->catch_tell(capitalize(object_name) + " can be used as armor.\n");   

	if (tar[0]->query_prop(IS_WEAPON))
	    tp->catch_tell(capitalize(object_name) + " can be used as a weapon.\n");

	if (tar[0]->query_prop(IS_MISSILE_WEAPON))
	    tp->catch_tell(capitalize(object_name) + " is some kind of missile weapon.\n");

	if (tar[0]->query_prop(IS_SHIELD))
	    tp->catch_tell(capitalize(object_name) + " can be used as a shield.\n");

	if (tar[0]->query_prop(IS_KEY))
	    tp->catch_tell(capitalize(object_name) + " is used to open something.\n");

	if (tar[0]->query_prop(IS_AMMO))
	{
	    missile_type=tar[0]->query_missile_type();
	    if ((missile_type == MISSILE_ARROW) ||
		(missile_type == MISSILE_BOLT) ||
		(missile_type == MISSILE_STONE))
	    {
		if (missile_type == MISSILE_ARROW)
		    tp->catch_tell(capitalize(object_name) +
				   " is some kind of arrow.\n");

		if (missile_type == MISSILE_BOLT)
		    tp->catch_tell(capitalize(object_name) +
				   " is some kind of bolt.\n");

		if (missile_type == MISSILE_STONE)
		    tp->catch_tell(capitalize(object_name) +
				   " is some kind of bullet stone.\n");
	    }
	    else
		tp->catch_tell(capitalize(object_name) +
			       " is some kind of ammo.\n");
	}
    }
    else
	tp->catch_tell("You get no impression about the purpose of " +
		       object_name + ".\n");
}

int
query_level()
{
    return 1;
}

int *
query_spell_data()
{
    return
	({
	    SP_INSTANTLY,
	    SP_NONE,
	    SP_TOUCH,
	    1,
	    SP_ALL_OBJECTS,
	    SK_ML_ASTER,
	});
}
