#include <macros.h>
#include <language.h>
#include <skills.h>
#include <state.h>
#include <properties.h>

/*
 * Function name: dirdesc_to_integer
 * Description:   splits a direction desc (e.g. 2n3e) to 
 *                integer values
 * Arguments:     suffix - the direction desc
 * Returns:       array,  here: ({ 2, 3 })
 */
nomask int *
dirdesc_to_integer(string desc)
{
    int r, d1, d2, dir1, dir2, *res;

    if (!desc || (r = sscanf(desc, "%d%c%d%c", d1, dir1, d2, dir2)) < 2 ||
	r == 3)
	return 0;
    if (r == 2)
    {
	if (dir1 == 'e' || dir1 == 'w')
	{
	    dir2 = dir1;
	    dir1 = 'n';
	    d2 = d1;
	    d1 = 0;
	}
	else
	{
	    dir2 = 'e';
	    d2 = 0;
	}
    }
    if (dir1 == 'n')
	res = ({ d1, 0 });
    else if (dir1 == 's')
	res = ({ -d1, 0 });
    else
	return 0;
    if (dir2 == 'e')
	res[1] = d2;
    else if (dir2 == 'w')
	res[1] = -d2;
    else
	return 0;
    return res;
}

/*
 * Function name: object_desc
 * Description:   get correct object description
 * Arguments:     ob - object to describe
 *                cap - if true, capitalize description
 *                pre - prefix type (0 - none, 't' - the, 'a' - article)
 * Returns:       description array
 */
nomask mixed
object_desc(object o, int cap, int pre)
{
    int    heap;
    mixed  desc;
    
    if (living(o))
    {
	desc = ({ o, o->query_met_name(),
		  o->query_nonmet_name(), "someone", });
	if (pre == 't')
	    desc[2] = "the " + desc[2];
	else if (pre == 'a')
	    desc[2] = LANG_ADDART(desc[2]);
	if (cap)
	{
	    desc[2] = capitalize(desc[2]);
	    desc[3] = capitalize(desc[3]);
	}
	return desc;
    }
    
    desc = ({ o, 0, 0, "something" });
    
    if ((heap = o->query_msg_size()) == 0) /* heap is not moving */
	heap = o->query_split_size();
    
    if (heap < 2)
	desc[2] = o->short();
    else if (heap < 8)
	desc[2] = heap + " " + o->plural_short();
    else if (heap < 16)
	desc[2] = "some " + o->plural_short();
    else if (heap < 80)
	desc[2] = "many " + o->plural_short();
    else 
	desc[2] = "huge heap of " + o->plural_short();

    if (heap < 2 || heap >= 80)
    {
	if (pre == 't')
	    desc[2] = "the " + desc[2];
	else if (pre == 'a')
	    desc[2] = LANG_ADDART(desc[2]);
    }
    if (cap)
    {
	desc[2] = capitalize(desc[2]);
	desc[3] = capitalize(desc[3]);
    }
    return desc;
}

/*
 * Function name: coord_suffix
 * Description:   creates a distance description of the form: [XnYe]
 * Arguments:     source - the source coordinates (player coordinates)
 *                dest - target coordinates
 * Returns:       description in feet
 */
nomask string
coord_suffix(int *source, int *dest)
{
    int    diff, s, tmp;
    string desc;

    desc = "";
    diff = source[1] - dest[1];
    if (diff > 4000 || diff < -4000)
	tmp = diff;
    else if (diff)
    {
	s = diff / 12;
	if (!s)
	    desc = (diff > 0 ? "s" : "n");
	else
	    desc = (s > 0 ? s + "s" : -s + "n");
    }
    
    diff = source[0] - dest[0];
    if (tmp || diff > 4000 || diff < -4000)
	desc = "?" + (tmp > 0 ? "s" : "n") + (diff > 0 ? "w" : "e");
    else if (diff)
    {
	s = diff / 12;
	if (!s)
	    desc += (diff > 0 ? "w" : "e");
	else
	    desc += (s > 0 ? s + "w" : -s + "e");
    }
    
    if (diff = (source[2] - dest[2]) / 12)
    	desc += (diff > 0 ? diff + "d" : -diff + "u");
    if (desc == "")
	return desc;
    return "[" + desc + "]";
}

/*
 * Function name: desc_same
 * Description:   Gives a textdescription, pluralform, if more then one,
 *                articles are added to singular objects that do not have a
 *                capitalized short description
 * Arguments:     ob - object to describe
 *                sh - the short desc
 *                co - coordinate string
 *                sz - number of objects with equal 'sh'
 *                sk - mathmatics skill (number that can be seen at once)
 * Return:        string, description
 */
nomask string
desc_same(object ob, string sh, string co, int sz, int sk)
{
    if (sz > 1)
    {
	sh = ob->plural_short() + co;
	if (sz < 9 || sz <= sk)
	{
	    if (sz < 13)
		return LANG_NUMW(sz) + " " + sh;
	    return sz + " " + sh;
	}
	if (sz < 13)
	    return "some " + sh;
	return "many " + sh;
    }
    if (sh[0] > 'Z')
	return LANG_ADDART(sh);
    return sh;
}

/*
 * Function name: composite_words
 * Description:   give a composite string of the form xx, xx, xx and xx
 * Arguments:     wlist - a list of strings to concat
 * Returns:       the concatenation string
 */
nomask string
composite_words(string *wlist)
{
    int sz;

    if (!(sz = sizeof(wlist)))
	return 0;

    if (sz == 1)
	return wlist[0];

    return implode(wlist[0..sz - 2], ", ") + " and " + wlist[sz - 1];
}

/*
 * Function name: composite
 * Description:   creates a description-string for a certain
 *                player. No visible check!
 * Arguments:     a - Array of the objects to describe
 *                tp - looking player
 * Returns:       A description string on the format:
 *                desc1, desc2 and desc3
 */
nomask string
composite(object *a, object tp)
{
    int     i, sz, h, sk;
    string  short, *arr;
    object  ob;
    mixed   obarr;
    mapping map;

    if (!tp || !(sz = sizeof(a)))
	return 0;
    
    map = ([ ]);
    for (i=0, obarr=allocate(sz*2); i<sz; i++)
    {
	ob = a[i];

	if (ob->query_prop(OBJ_NO_SHOW) || !(short = ob->short()))
	    continue;
	if (!map[short])
	{
	    obarr[i << 1]       = ob;
	    obarr[(i << 1) + 1] = short;
	}
	if (h = ob->query_heap_size())
	    map[short] += h;
	else
	    map[short]++;
    }
    obarr -= ({ 0 });
    if (!(sz = sizeof(obarr)))
	return 0;

    arr = allocate(sz/2);
    sk = tp->query_skill_rank(SK_MATHEMATICS);
    for (i=h=0; i<sz; i+=2)
	arr[h++] = desc_same(obarr[i], obarr[i+1], "", map[obarr[i+1]], sk);

    return composite_words(arr);
}

/*
 * Function name: composite_unseen
 * Description:   create a description-string for a certain player,
 *                also adds an extra description for invisible objects.
 * Arguments:     a - objects to describe
 *                tp - player object
 * Returns:       description string
 */
nomask string
composite_unseen(object *a, object tp)
{
    string str;
    object *visible;
    
    if (!tp)
	return 0;
    visible = tp->visible(a);
    a -= visible;
    str = composite(visible, tp);
    if (sizeof(a))
    {
	if (!str || !sizeof(visible))
	    return "something invisible";
	if (sizeof(visible) > 1)
	    return "something invisible, " + str;
	return "something invisible and " + str;
    }
    return str;
}

/*
 * Function name: composite_array
 * Description:   creates a description-array
 * Arguments:     a - array of the objects to describe
 *                all - if true, include also no-show objects
 * Returns:       A description array of the format:
 *                ({ name, nonmet_name, unseen name }), ", ",
 *                ({ name, nonmet_name, unseen name }), " and ",
 *                ({ name, nonmet_name, unseen name })
 */
nomask varargs string *
composite_array(object *a, int all)
{
    int     i, heap, sz;
    object  ob;
    mixed   arr;

    if (!(sz = sizeof(a)))
	return 0;

    arr = allocate(sz*2);
    for (i=0; i<sz; i++)
    {
	ob = a[i];
	if (!ob->short())
	    continue;
	if (!all && ob->query_prop(OBJ_NO_SHOW))
	    continue;
	arr[i << 1] = object_desc(ob, 0, 'a');
	arr[(i << 1) + 1] = ", ";
    }
    arr -= ({ 0 });
    if (!(sz = sizeof(arr)))
	return ({ });
    if (sz < 4)
	return ({ arr[0] });
    arr[sz-3] = " and ";
    return arr[..sz-2];
}

/*
 * Function name: composite_room_contents
 * Description:   Creates a description-string of a rooms contents
 *                for a certain player
 * Arguments:     room - room to describe
 *                tp - looking player
 * Returns:       string
 */
nomask string
composite_room_contents(object room, object tp)
{
    int     i, j, sz, heap, sk, *co, *tar_co;
    string  short, coord, desc_extra, desc_obj, desc_liv, *arr;
    object  ob, *list;
    mixed   obarr;
    mapping map;

    if (!tp || !(co = tp->query_coordinates()))
	return 0;
    sk = tp->query_skill_rank(SK_MATHEMATICS);
    desc_extra = desc_obj = desc_liv = "";

    /*
     * describe non-living objects
     */
    map  = ([ ]);
    list = tp->visible(I(room, 1));
    for (i=j=0, sz=sizeof(list), obarr=allocate(sz*3); i<sz; i++)
    {
	ob = list[i];
	if (!(tar_co = ob->query_coordinates(co)))
	    continue;
	if (ob->query_prop(OBJ_NO_SHOW) || !(short = ob->short()))
	{
	    if (!(short = ob->room_desc(tp)))
		continue;
	    desc_extra += short + coord_suffix(co, tar_co) + ".\n";
	    continue;
	}
	coord  = coord_suffix(co, tar_co);
	short += coord;
	if (!map[short])
	{
	    obarr[j++] = ob;
	    obarr[j++] = short;
	    obarr[j++] = coord;
	}
	if (heap = ob->query_heap_size())
	    map[short] += heap;
	else
	    map[short]++;
    }
    obarr -= ({ 0 });
    if (sz = sizeof(obarr))
    {
	arr = allocate(sz / 3);
	for (i=j=0; i<sz;)
	    arr[j++] = desc_same(obarr[i++], obarr[i++], obarr[i++],
				 map[obarr[i-2]], sk);
	desc_obj = break_string(capitalize(composite_words(arr)),
				77, 1) + ".\n";
    }
    
    /*
     * describe living objects
     */
    map  = ([ ]);
    list = tp->visible(I(room, 2) - ({ tp }));
    for (i=j=0, sz=sizeof(list), obarr=allocate(sz*3); i<sz; i++)
    {
	ob = list[i];
	if (ob->query_state() & MOUNT || ob->query_prop(OBJ_NO_SHOW) ||
	    !(short = ob->short(tp)) || !(tar_co = ob->query_coordinates(co)))
	    continue;
	coord  = coord_suffix(co, tar_co);
	short += coord;
	if (!map[short])
	{
	    obarr[j++] = ob;
	    obarr[j++] = short;
	    obarr[j++] = coord;
	}
	map[short]++;
    }
    obarr -= ({ 0 });
    if (sz = sizeof(obarr))
    {
	arr = allocate(sz / 3);
	for (i=j=0; i<sz;)
	    arr[j++] = desc_same(obarr[i++], obarr[i++], obarr[i++],
				 map[obarr[i-2]], sk);
	desc_liv = break_string(capitalize(composite_words(arr)),
				77, 1) + ".\n";
    }
    return desc_extra + desc_obj + desc_liv;
}
