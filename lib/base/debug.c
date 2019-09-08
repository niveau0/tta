#include <macros.h>
#include <material.h>
#include <properties.h>

nomask void
debug_object(object ob)
{
    string oname;
    
    if (!ob)
	error("Illegal call to debug_object");

    oname = object_name(ob);
    if (strlen(oname) > 4 &&
	(oname[..5] == "/base/" || oname[..5] == "/confi"))
	return;
    
    if (!ob->short())
	log("DEBUG", oname + ": missing short description");
    if (!ob->plural_short())
	log("DEBUG", oname + ": missing plural description");
    if (!ob->long())
	log("DEBUG", oname + ": missing long description");
    if (!sizeof(ob->query_names()))
	log("DEBUG", oname + ": missing names");

    if (ob->query_heap_id() && !ob->query_plong())
	log("DEBUG", oname + ": missing plural long description");
    else if (function_object("query_max_weight", ob))
    {
	if (!ob->query_max_weight())
	    log("DEBUG", oname + ": missing maximum weight");
	if (!ob->query_max_volume())
	    log("DEBUG", oname + ": missing maximum volume");
	if (living(ob))
	{
	    if (!ob->query_name())
		log("DEBUG", oname + ": missing name");
	    if (!ob->query_size())
		log("DEBUG", oname + ": missing size");
	    return;
	}
	if (!function_object("query_door_id", ob) && !ob->query_value())
	    log("DEBUG", oname + ": missing value");
    }
    
    if (!ob->query_prop(OBJ_MATERIAL))
	log("DEBUG", oname + ": missing material");
    if (ob->query_prop(OBJ_MATERIAL) != M_AIR)
    {
	if (!ob->query_size())
	    log("DEBUG", oname + ": missing size");
	if (!ob->query_weight())
	    log("DEBUG", oname + ": missing weight");
	if (!ob->query_volume())
	    log("DEBUG", oname + ": missing volume");
    }
    
    if (function_object("query_data", ob) == "/base/weapon")
    {
	if (!ob->query_data())
	    log("DEBUG", oname + ": missing weapon data");
	return;
    }
    
    if (function_object("query_at", ob))
    {
	if (!ob->query_at())
	    log("DEBUG", oname + ": missing armor type");
	if (!ob->query_race_sizes())
	    log("DEBUG", oname + ": missing race size");
	return;
    }
}
