static nomask
create()
{
    string n;
    
    if ((n = object_name(this_object())) && sscanf(n , "%*s#%*d") == 2)
	destruct_object();
}

/*
 * Function name: indent
 * Description:   indent an lpc file
 * Arguments:     file - filename
 * Returns:       string, editor response
 */
static nomask string
indent(string file)
{
    string ret;
    
    ret = editor("e " + file);
    editor("I");
    editor("x");
    return ret;
}
