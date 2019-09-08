/*
 * Function name: errorstat
 * Description:   display the runtime/compile time error list
 */
nomask int
errorstat()
{
    CHECK_LEVEL(_CREATOR);
    
    this_player()->catch_tell(_SYSTEMD->list_error_stat());
    return -1;
}
