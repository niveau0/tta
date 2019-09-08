#include <access.h>
#include <living.h>
#include "../living/prototypes.h"

mapping               m_remember_name;	 /* Names of players we have met */

static int	      coder_unmet;       /* If coders want see unmet */

/*
 * Function name:   set_coder_unmet(int flag)
 * Description:     if this is a coder, he may chose to see only nonmet names
 * Arguments:       flag - 1 if see as unmet, 0 if see as met
 */
nomask void
set_coder_unmet(int flag)
{
    coder_unmet = flag;
}

/*
 * Function name:   query_remembered()
 * Description:     Gives back a mapping with all names of people we remember
 * Return:          A mapping with names
 */
nomask mapping
query_remembered()
{
    if (!mappingp(m_remember_name))
	return ([ ]);
    if (_ACCESS_LEVEL(geteuid()))
    {
	m_remember_name = 0;
	return ([ ]);
    }
    return m_remember_name;
}

/*
 * Function name:   add_remembered(string name, string rname)
 * Description:     Adds a living to those whom we want to remember.
 * Arguments:       str - Realname of living that we want to remember.
 *                  rname - remember-name of living that we want to remember.
 * Returns:         -1 if at limit for remember,
 *                  1 if remember ok,
 *                  2 if already known
 */
nomask int
add_remembered(string name, string rname)
{
    int max;
	
    if (_ACCESS_LEVEL(geteuid()))
	return 2;
    
    if (!mappingp(m_remember_name))
	m_remember_name = ([ ]);
    else if (m_remember_name[name])
    {
	/* already known */
	m_remember_name[name] = rname;
	return 2;
    }
    else
    {
	max = query_stat(STAT_ME) * 5;
	if (map_sizeof(m_remember_name) >= max)
	    return -1;
    }
    m_remember_name[name] = rname;
    return 1;
}

/*
 * Function name:   remove_remembered(string name)
 * Description:     Removes a remembered person from our list.
 * Arguments:       name: Name of living to forget
 * Returns:         false if the name was not introduced or remembered,
 *                  true otherwise.
 */
nomask int
remove_remembered(string name)
{
    int    i, flag;
    string *ind;

    if (mappingp(m_remember_name))
    {
	ind = map_indices(m_remember_name);
	for (i=sizeof(ind); i--;)
	    if (m_remember_name[ind[i]] == name)
	    {
		m_remember_name[ind[i]] = 0;
		flag = 1;
		break;
	    }
    }
    return flag;
}
