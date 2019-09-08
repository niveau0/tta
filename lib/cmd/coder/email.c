/* **************************************************************************
 * email - set the email address
 */
nomask int
email(string str)
{
    object tp;
    
    CHECK_LEVEL(_ADMIN);

    tp = this_interactive();
    if (!str)
    {
	tp->catch_tell("Your email is: " + tp->query_email() + "\n");
	return 1;
    }
    tp->set_email(str);
    tp->echo("You've set your email to: " + str + "\n");
    return 1;
}
