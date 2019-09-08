static void
holy_water(int amount, object tp)
{
    if (tp->query_prop(LIVE_UNDEAD) > 0)
    {
	tp->catch_tell("Uh, the liquid nearly rips your stomach.\n");
	tell_objects(LISTEN(E(tp)) - ({ tp }),
		     ({ QCTNAME(tp), " twines in pain.\n" }), MSG_SEE, tp);
	tp->check_death(random(10) + 1, this_object());
    }
    else
    {
	tp->catch_tell("Ah, that tastes great, although it is only water.\n");
	tp->add_hp(random(5));
    }
}

static void
eldaana(int amount, object tp)
{
    if (!check_amount(100, amount, tp))
	return;
    poison_removal(tp, ({ "lhugruth", "ondohithui", "nimnaur", "silmaana",
			      "slota", "zaganzar" }), 9);
}

static void
menelar(int amount, object tp)
{
    if (!check_amount(100, amount, tp))
	return;
    poison_removal(tp, ({ "carnegurth", "jeggarukh", "karfar", "klytun",
			      "sharkasar", "sharduvaak" }), 7);
}
 
static void
quilmufur(int amount, object tp)
{
    if (!check_amount(100, amount, tp))
	return;
    poison_removal(tp, ({ "bragolith", "brithagurth", "henuial", "ondokamba",
			      "muilfana", "taynaga" }), 8);
}

static void
reglen(int amount, object tp)
{
    if (!check_amount(100, amount, tp))
	return;
    tp->catch_tell("You feel freshed and vitality flows through your body.\n");
    tp->add_hp(random(10)+1);
}

static void
rewk(int amount, object tp)
{
    if (!check_amount(100, amount, tp))
	return;
    tp->catch_tell("You feel freshed and vitality flows through your body.\n");
    tp->add_hp(random(19)+2);
}

static void
thurl(int amount, object tp)
{
    if (!check_amount(100, amount, tp))
	return;
    tp->catch_tell("You feel freshed and vitality flows through your body.\n");
    tp->add_hp(random(4)+1);
}

static void
fek(int amount, object tp)
{
    int     *ind, sz, i;
    mapping wounds;

    if (!check_amount(100, amount, tp) ||
	!(sz = map_sizeof(wounds = tp->query_bloodwounds())))
	return;

    ind = map_indices(wounds);

    for(i=0; i<sz; i++)
    	tp->add_bloodloss(-wounds[ind[i]], ind[i]);
}

static void
shatter_repair(int amount, object tp)
{
    mapping fracs;
    int i, sz, *ind;

    if (!check_amount(100, amount, tp) ||
	!(sz = sizeof(ind=map_indices(fracs = tp->query_wounds()))))
	return;
    for(i=0; i<sz; i++)
    {
    	if (fracs[ind[i]] == SLOT_BROKEN ||
	    fracs[ind[i]] == SLOT_SHATTERED)
	    tp->set_wound(ind[i],0);
	
	tp->catch_tell("Your bones are no longer broken.\n");
	tell_objects(LISTEN(E(tp)) - ({ tp }),
		({ capitalize(tp->query_pronoun()) +
	        " looks better now.\n"}),
	        MSG_SEE);
    }
}

static void
turn_invisible(int amount, object tp)
{
    int level;
    
    if (!check_amount(100, amount, tp))
	return;

    level = amount/100;
    
    if (!tp->query_prop(OBJ_INVIS))
    {
	tp->echo("Suddenly you can't see yourself anymore.\n");
	tell_objects(LISTEN(E(tp)) - ({ tp }),
			    ({ capitalize(tp->query_pronoun()) +
				   " suddenly disappears.\n"}), MSG_SEE);
    }
    else
	tp->echo("You feel a strange tingling.\n");
    
    tp->change_int_prop(OBJ_INVIS, 1);	
    call_out("decrease_invis_level", level*(random(120)+480)/TIME_SCALE, tp);
}

static void
see_invisible(int amount, object tp)
{
    int level;
    
    if (!check_amount(100, amount, tp))
	return;

    level = amount/100;
    
    if (!tp->query_prop(LIVE_SEE_INVIS))
	tp->echo("The world seems to be clearer now.\n");
    else
	tp->echo("Your eyes tingle.\n");
    
    tp->change_int_prop(LIVE_SEE_INVIS, 1);	 
    call_out("decrease_see_invis_level", level*(random(1800)+2700)/TIME_SCALE,
	     tp);
}

static void
infravision(int amount, object tp)
{
    int level;
    
    if (!check_amount(100, amount, tp))
	return;

    level = amount/100;
    
    if (!tp->query_prop(LIVE_SEE_DARK))
	tp->echo("Your eyes begin to glow.\n");
    else
	tp->echo("Your eyes tingle.\n");
    
    tp->change_int_prop(LIVE_SEE_DARK, 1);	 
    call_out("decrease_infravision", level*(random(120)+480)/TIME_SCALE, tp);
}
