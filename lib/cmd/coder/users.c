/* **************************************************************************
 * user listing for differnt types (user, ftp...)
 */
nomask int
user_list(string type)
{
    int    i;
    string out;
    object *u;
    
    CHECK_LEVEL(_BUILDER);

    if (!type)
	return people(0);

    switch (type)
    {
      case "ftp":
	  out = "User       IP                   Idle\n";
	  break;
      default:
	  this_interactive()->catch_tell("No such user type.\n");
	  return -1;
    }
    u = users(type);
    for (i=sizeof(u); i--;)
    {
	out += sprintf("%-10s %-20s %-45s\n", geteuid(u[i]),
		       query_ip_number(u[i]), CONV_REALTIME(u[i]->_Q_idle()));
    }
    this_interactive()->catch_tell(out);
    return 1;
}
