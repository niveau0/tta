/*
 * Function name: execute
 * Description:   execute code
 * Arguments:     expr - the expression to execute
 */
nomask int
execute(string expr)
{
    int    l;
    string file;
    object ob;
    
    CHECK_LEVEL(_CREATOR);

    if (!expr || !(l = strlen(expr)))
    {
	call_other("/cmd/coder/exec_base", "show_vars");
	return 1;
    }
    
#ifdef _LOG_EXEC
    syslog("EXEC", BS(this_player()->query_name() + ": " + expr, 76, 1));
#endif

    expr = ("inherit \"/cmd/coder/exec_base\";\n" + 
	    "#include \"/cmd/coder/exec_include.h\"\n\n" +
	    "static mixed\nexecute()\n{ \n" + expr + "\n}\n");
	
    file = "/syslog/exec/_exec_" + geteuid();

    if (file_size(file + ".c") > 0)
	remove_file(file + ".c");
    
    write_file(file + ".c", expr);

    if (ob = find_object(file))
	destruct_object(ob); /* if there is a corpse left */

    if (expr = catch(ob = compile_object(file)))
    {
	remove_file(file + ".c");
	this_interactive()->catch_tell(expr+".\n");
	return 1;
    }
    seteuid(geteuid(), ob);
    if (expr = catch(ob->exec_code()))
    {
	remove_file(file + ".c");
	this_interactive()->catch_tell(expr+".\n");
    }
    else
    {
	destruct_object(ob);
	remove_file(file + ".c");
    }
    return 1;
}
