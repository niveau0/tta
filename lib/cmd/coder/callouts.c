/* **************************************************************************
 * call_outs - list the pending call_outs if logged
 */
nomask int
call_outs(string str)
{
    CHECK_LEVEL(_BUILDER);
    
#ifndef _LOG_CALLOUT
    this_interactive()->
	catch_tell("The call_outs are currently not logged.\n");
    return 1;
#else
    _SYSTEMD->list_call_outs();
    return 1;
#endif
}
