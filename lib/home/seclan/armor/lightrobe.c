inherit "/base/object";  
inherit "/base/container";  
inherit "/base/armor";  

static void
create_object()
{
    create_container();
    create_armor();

    set_name("robe");
    add_adj("amorph");
    add_adj("shimmering");
    set_short("shimmering robe");
    set_pshort("shimmering robes");
    set_long_bs("A foil which has been vaguely formed like a robe. "+
	     "The shimmering and glowing material makes it hard to "+
	     "appraise its size. There are pockets of all sizes on the "+
	     "inside.\n");
    set_at(A_S_LEATHER);
    set_layer(LAYER_PLATE);
    set_locations( ({ SLT_CHEST+1, SLT_ARM+1, SLT_ARM+2, SLT_ABDOMEN+1,
			  SLT_LEG+1, SLT_LEG+2,
			  SLT_NECK+1, SLT_BACK+1, SLT_HEAD+1 }) );
    add_lwvs(1, 0, 0, 65);
    set_race_sizes(0, 500);
    set_value(1300);
    add_prop(OBJ_MATERIAL, M_AIR);
    add_prop(OBJ_THIN_MATERIAL, 1);

    set_max_weight(0 + 30000);
    set_max_volume(0 + 5000);    
}
