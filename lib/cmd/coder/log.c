/* **************************************************************************
 * show_errorlog - list the global error log
 */
nomask int
show_errorlog()
{
    CHECK_LEVEL(_BUILDER);
    
    if (!tail("/log/ERRORS"))
	this_interactive()->catch_tell("No such file.\n");
    return 1;
}
