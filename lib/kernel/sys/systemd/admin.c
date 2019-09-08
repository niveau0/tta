/*
 * Coder level administration
 */

nomask mixed * banish(string name, int what);

mapping	m_coders;  /* The coder mapping */

private string reason, name; /* shutdown reason, shutdowners name */

/*
 * This function is called if the master-savefile cannot
 * be found. The default values are within <config.h>
 */
static nomask void
init_admin()
{
    m_coders = ([ ]);
}

/************************************************************************
 * The level administration code.
 */

/*
 * Function name: query_coder_path
 * Description:   Gives a default homedirectory path for a coder
 * Arguments:     coder - the coders name
 * Returns:       A default path for the coder
 */
nomask string
query_coder_path(string coder)
{
    if (!coder || !strlen(coder))
	return 0;
    if (coder == _ROOT)
	return "/syslog";
    return "/home/" + coder;
}

/*
 * Function name: query_plan_file
 * Description:   read the coders plan file from home directory
 * Arguments:     coder - the coders name
 * Returns:       string, plan file contents 
 */
nomask string
query_plan_file(string coder)
{
    string path;
    
    if (!(path = query_coder_path(coder)))
	return 0;
    return read_file(path + "/.plan");
}

/*
 * Function name: _Q_coder_level
 * Description:   Return the level of a coder.
 * Arguments:     coder - the coders name
 */
nomask int
_Q_coder_level(string coder)
{
    if (!coder || !m_coders)
        return 0;
    if (coder == _ROOT)
	return _ADMIN;
    return m_coders[lower_case(coder)];
}

/*
 * Function name: query_coder_list
 * Description:   Return a list of all coders of a given type.
 * Arguments:	  type of coder. -1 = all.
 */
nomask string *
query_coder_list(int type)
{
    int     i;
    string  *names;

    names = map_indices(m_coders);
    if (type < _BUILDER)
	return names;
    for (i=sizeof(names); i--;)
    {
	if (m_coders[names[i]] != type)
	    names[i] = 0;
    }
    return names - ({ 0 });
}

/*
 * Function name: do_chcoder_lev
 * Description:   Change the level of a coder.
 *		  This routine assumes that the coder _does_ exist in the
 *		  m_coders array. This must have been checked before calling.
 * Arguments:     coder - the coders name
 *		  nlev  - the new level
 */
static nomask void
do_chcoder_lev(string coder, int nlev)
{
    int    olev;
    object ob;
    
    if ((olev = m_coders[coder]) == nlev)
	return;

    m_coders[coder] = nlev;

    ob = find_player(coder);
    if (!nlev)
    {
	if (ob)
	{
	    ob->catch_tell("Your level was changed to mortal by " +
			   capitalize(this_player()->query_name()) + ".\n");
	    destruct_object(ob);
	}
	rename_file(PLAYER_DIR(coder), "/players/demoted/" + coder);
	save_object(SAVEFILE);
	/* try to remove coder directory */
	remove_dir(query_coder_path(coder));
	return;
    }

    save_object(SAVEFILE);

    if (!olev)
    {
	make_dir(query_coder_path(coder)); /* create home directory */
	if (ob)
	    ob->set_race("ainu");
	_ACCESSD->update_access(({ coder }));
    }

    if (ob)
    {
	setuid(coder, ob);
	seteuid(coder, ob);
	/*ob->set_coder_level(nlev);*/
	ob->save();
	ob->catch_tell("Your level was changed to " +
		       (nlev == _ADMIN ? "admin" :
			(nlev == _CREATOR ? "creator" : "builder")) + " by " +
			capitalize(this_player()->query_name()) + ".\n");
    }
}

/*
 * Function name: change_coder_level
 * Description:   Change the level (and type) of a coder.
 * Arguments:     coder - the coders name
 *		  nlev - the new level
 * Returns:	  string
 */
nomask string
change_coder_level(string coder, int nlev)
{
    int    l;
    string euid;

    coder = lower_case(coder);
    euid = geteuid(previous_object());
    
    if ((l = _Q_coder_level(euid)) < _CREATOR || l < nlev ||
	l <= _Q_coder_level(coder))
	return "You are not allowed to do this.";
#ifdef _LOG_LEVEL
    syslog("LEVEL", break_string(coder + ": " + _Q_coder_level(coder) + "->" +
				 nlev + " by " + euid, 76, 1));
#endif
    do_chcoder_lev(coder, nlev);
    return "Ok.";
}

/*
 * Function name: query_coder_pretitle
 * Description:   Gives a pretitle for a specific coder
 * Arguments:     coder - The coders name
 * Returns:       string, pretitle for the coder
 */
nomask string
query_coder_pretitle(string coder)
{
    int    l;
    object ob;

    if (!coder || !strlen(coder))
	return "";

    l = _Q_coder_level(coder);

    if (l < _BUILDER)
        return "";
    if (l < _CREATOR)
    {
	if (!(ob = find_player(coder)) || !ob->query_gender())
	    return "Maia";
	return "Maie";
    }
    if (!(ob = find_player(coder)) || !ob->query_gender())
        return "Vala";
    return "Valie";
}
