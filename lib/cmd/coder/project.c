/* **************************************************************************
 * project - list/change project data
 */
nomask int
project(string str)
{
    int    ret;
    string option, project;
    object tp;

    CHECK_LEVEL(_BUILDER);

    tp = this_interactive();
    if (!str)
    {
	tp->catch_tell("Usage: project <project>|list|own\n" +
		       "       project <project> c\n" +
		       "       project <project> d <description>\n" +
		       "       project <project> D\n" +
		       "       project <project> a <member to add>\n" +
		       "       project <project> r <member to remove>\n" +
		       "       project <project> + <access to add>\n" +
		       "       project <project> - <access to remove>\n" +
		       "       project <name> q\n");
	return -1;
    }
    if (sscanf(str, "%s %s", project, option) != 2)
	return _ACCESSD->list_project(str, tp);
    str = 0;
    sscanf(option, "%s %s", option, str);
    switch (option)
    {
      case "c":
	  ret = _ACCESSD->add_project(project);
	  break;
      case "d":
	  ret = _ACCESSD->change_project_desc(project, str);
	  break;
      case "D":
	  ret = _ACCESSD->remove_project(project);
	  break;
      case "a":
	  ret = _ACCESSD->add_member(project, str);
	  break;
      case "r":
	  ret = _ACCESSD->remove_member(project, str);
	  break;
      case "+":
	  ret = _ACCESSD->change_access(project, '+', str);
	  break;
      case "-":
	  ret = _ACCESSD->change_access(project, '-', str);
	  break;
      case "q":
	  ret = _ACCESSD->query_projects(project, tp);
	  break;
      default:
	  tp->catch_tell("Wrong option.\n");
	  return -1;
    }
    if (ret == -1)
	tp->catch_tell("Failed.\n");
    else
	tp->catch_tell("Ok.\n");
    return 1;
}
