private string euid, uid;

/*
 * Function name: init_uid
 * Description:   set the initial user id of this object
 *                (only root may change it)
 */
private void
init_uid()
{
    string *ploder;
    
    ploder = explode(object_name(this_object()), "/");
    
    if (ploder[0] == "kernel")
	uid = _ROOT;
    else if (ploder[0] == "system")
	uid = _SYSTEM;
    else
	uid = "0";
    
    euid = uid;
}

/*
 * Function name: getuid
 * Description:   give user id of an object
 * Arguments:     obj - object to query 
 * Returns:       string
 */
nomask varargs string
getuid(object obj)
{
    if (!obj || obj == this_object())
	return uid;
    return obj->getuid();
}

/*
 * Function name: setuid
 * Description:   set user id (only root)
 * Arguments:     id - the new user id
 *                obj - the object to change
 */
static nomask varargs void
setuid(string id, object obj)
{
    if (!id)
	id = "0";
    if (!obj)
	obj = this_object();
    if (uid != _ROOT)
    {
	error(object_name(this_object()) + " tried illegally to set uid in " +
	      object_name(obj));
    }
    rlimits(-1; -1)
    {
	obj->_F_setuid(id);
    }
}

nomask void
_F_setuid(string id)
{
    if (PRIVILEGED())
	uid = id;
}

/*
 * Function name: geteuid
 * Description:   give effective user id of an object
 * Arguments:     obj - object to query 
 * Returns:       string
 */
nomask varargs string
geteuid(object obj)
{
    if (!obj || obj == this_object())
	return euid;
    return obj->geteuid();
}

/*
 * Function name: seteuid
 * Description:   set effective user id
 * Arguments:     id - effective user id to set
 *                obj - object to change
 */
static nomask varargs void
seteuid(string id, object obj)
{
    if (!id)
	id = "0";
    if (!obj)
	obj = this_object();
    if (!_ACCESS_SETEUID(this_object(), obj, id))
    {
	error(object_name(this_object()) + " tried illegally to set euid in " +
	      object_name(obj));
    }
    rlimits(-1; -1)
    {
	obj->_F_seteuid(id);
    }
}

nomask void
_F_seteuid(string id)
{
    if (PRIVILEGED())
    {
	euid = id;
	rlimits(20; 100000)
	{
	    _ACCESSD->reload_access(this_object());
	}
    }
}

/*
 * Function name: call_with_euid
 * Description:   change euid of an object to this objects euid, call the
 *                function and restore euid after command. Ensure that 
 *                call_with_euid is not calling insecure objects (e.g.
 *                objects that might be edited by non-administrators).
 * Arguments:     ob - the object to call
 *                func - function to call 
 *                args - 'func' arguments
 * Returns:       return value of function
 */
static nomask varargs mixed
call_with_euid(object ob, string func, mixed args...)
{
    string oeuid;
    mixed  result;
    
    if (!ob || ob == this_object())
	error("Illegal call to call_with_euid");
    oeuid = ob->geteuid(); /* save objects euid */
    rlimits (-1; -1)
    {
	ob->_F_seteuid(euid);
    }
    catch(result = call_other(ob, func, args...));
    rlimits (-1; -1)
    {
	if (ob)
	    ob->_F_seteuid(oeuid);
    }
    return result;
}
