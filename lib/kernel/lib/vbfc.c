static nomask void
create()
{
    setuid("0");
    seteuid("0");
}

/*
 * Function name: vbfc
 * Description:   execute a vbfc call (see process_value)
 * Arguments:     ob - object to call
 *                func - function to call
 *                args - 'func' arguments
 * Returns:       return value of 'func'
 */
nomask varargs mixed
vbfc(object ob, string func, mixed args...)
{
    return call_other(ob, func, args...);
}
