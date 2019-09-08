/* **************************************************************************
 * msg - toggle listen to login message
 */
nomask int
msg_toggle()
{
    object tp;

    CHECK_LEVEL(_BUILDER);

    if ((tp = this_interactive())->query_prop(CODER_MSG_ENTER))
    {
        tp->catch_tell("Entering and leaving players are no longer shown.\n");
        tp->add_prop(CODER_MSG_ENTER, 0);
    }
    else 
    {
        tp->catch_tell("Entering and leaving players are now shown.\n");
        tp->add_prop(CODER_MSG_ENTER,1);
    }
    return 1;
}
