/*
 * Function name: users
 * Description:   give user list
 * Returns:       array, objects
 */
static nomask varargs object *
users(string type)
{
    int    i;
    object user, *usr, *nusr;

    usr = ::users();
    switch (type)
    {
      case "ftp":
	  for (i=sizeof(usr), nusr=allocate(i); i--;)
	  {
	      if (usr[i] && program_name(usr[i]) == _FTP)
		  nusr[i] = usr[i];
	  }
	  break;
      default:
	  for (i=sizeof(usr), nusr=allocate(i); i--;)
	  {
	      if (usr[i] && (user = usr[i]->query_player()))
		  nusr[i] = user;
	  }
	  break;
    }
    
    return nusr - ({ 0 });
}

/*
 * Function name: this_interactive
 * Description:   give current interactive object
 * Returns:       object
 */
static nomask object
this_interactive()
{
    object in;
    
    if (this_user() && (in = this_user()->query_player()))
	return in;
    
    if ((in = this_player()) &&
	in->query_user() == _GLOBAL->query_interactive())
	return in;
    return 0;
}

/*
 * help function
 * allows call to static function for user object (input_to)
 */
nomask varargs void
_F_call(string func, string str, mixed args...)
{
    if (previous_program() != _USER)
	error("Illegal call to _F_call");
    call_other(this_object(), func, str, args...);
}
