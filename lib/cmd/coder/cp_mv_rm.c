#define MULTI_CP 0
#define MULTI_MV 1
#define MULTI_RM 2
#define MULTI_STRS ({ "Copied", "Moved", "Removed" })

/*
 * Function name: copy
 * Description:   Copy a (text) file. Line width limited by GD. 
 * Arguments:     From file path, to file path.
 * Returns:       1 if success, otherwise 0.
 */
private int
copy(string path1, string path2)
{
    string buf;

    /* try to read the file */
    if (!(buf = read_file(path1)))
        return 0;

    /* Test if target file exists and proceed accordingly */
    switch (file_size(path2))
    {
      case -2:
	return 0;
      case -1:
      case  0:
	break;
      default:
	if (!remove_file(path2))
	    return 0;
    }
    return write_file(path2, buf);
}

/*
 * Function name: do_multi_file
 * Description:   Commands containing multiple files are executed here.
 * Arguments:     operation - MULTI_CP, MULTI_MV, MULTI_RM
 *                p1 - first path
 *                p2 - (optional) second path
 * Returns:       1 if arguments are valid, otherwise 0.
 */
private int
do_multi_file(int operation, string p1, string p2)
{
    int    i, sz, count_files, count_dir;
    string tmp, *old_names, *new_names;
    object tp;
    
    tp = this_interactive();
    p1 = FPATH(tp->query_path(), p1);
    /* Remove directory '.' from p1 path */
    if (p1[strlen(p1)-2..] == "/.")
	p1 = p1[..strlen(p1)-3];
    
    old_names = get_dir(p1);
    if (!sizeof(old_names))
    {
        tp->catch_tell("File(s) not found.\n");
        return -1;
    }
    if (sizeof(old_names) > MAXFILES)
    {
        tp->catch_tell("Selected over " + MAXFILES + " files.\n");
        return -1;
    }
    /* Remove file name from p1 path */
    i = strlen(p1)-1;
    while (p1[i] != '/') --i;
    p1 = p1[..i];
    /* Is p2 directory or a file name */
    if (p2)
    {
        p2 = FPATH(this_interactive()->query_path(), p2);
	/* Remove directory '.' from p2 path */
	if (p2[strlen(p2)-2..] == "/.")
	    p2 = p2[..strlen(p2)-3];
        if (file_size(p2) != -2)
        {
            if (sizeof(old_names) != 1)
            {
                tp->catch_tell("Destination must be a directory for "+
			       "multiple copy/move.\n");
                return -1;
            }
            /* use a dummy so no need to fix p2 */
            new_names = ({ "" });
        }
        else
        {
            p2 += "/";
            new_names = old_names;
        }
    }
    /* moving a directory */
    if (operation == MULTI_MV && sizeof(old_names) == 1 &&
        file_size(p1 + old_names[0]) == -2)
    {
        tp->catch_tell(rename_file(p1 + old_names[0], p2 + new_names[0])
		       ? "Moved a directory.\n"
		       : "Failed to move a directory.\n" );
        return 1;
    }

    for (count_dir=count_files=i=0, sz=sizeof(old_names);
	 i<sz && i==(count_files + count_dir); ++i)
    {
	tmp = p1 + old_names[i];
        if (file_size(tmp) < 0)
            count_dir++;
        else
        {
            switch (operation)
            {
	      case MULTI_CP:
                count_files += copy(tmp, p2 + new_names[i]);
                break;
	      case MULTI_MV:
		if (file_size(p2 + new_names[i]) >= 0)
                    remove_file(p2 + new_names[i]);
                count_files += rename_file(tmp, p2 + new_names[i]);
                break;
	      case MULTI_RM:
		tp->catch_tell("Trying to remove "+tmp+"\n");
                count_files += remove_file(tmp);
                break;
            }
        }
    }
    if (count_files + count_dir != sizeof(old_names))
        tp->catch_tell("Failed at file '"+tmp+"'.\n");
    tp->catch_tell(MULTI_STRS[operation] + " " + count_files + " file" +
		   ((count_files==1) ? "" : "s") + ".\n");
    return 1;
}

/* **************************************************************************
 * cp - copy multiple files
 */
nomask int
cp_cmd(string arg)
{
    string p1, p2;

    CHECK_LEVEL(_BUILDER);

    if (!arg || (sscanf(arg,"%s %s",p1,p2)!=2))
    {
        this_interactive()->catch_tell("Usage: cp <from> <to>\n");
        return -1;
    }
    return do_multi_file(MULTI_CP, p1, p2);
}

/* **************************************************************************
 * mv - move multiple files or a single directory
 */
nomask int
mv_cmd(string arg)
{
    string p1,p2;

    CHECK_LEVEL(_BUILDER);

    if (!arg || (sscanf(arg,"%s %s",p1,p2) != 2))
    {
        this_interactive()->catch_tell("Usage: mv <from> <to>\n");
        return -1;
    }
    return do_multi_file(MULTI_MV, p1, p2);
}

/* **************************************************************************
 * rm - remove multiple files
 */
nomask int
rm_cmd(string arg)
{
    CHECK_LEVEL(_BUILDER);

    if (!arg)
    {
        this_interactive()->catch_tell("Usage: rm <filename(s)>\n");
        return -1;
    }
    return do_multi_file(MULTI_RM, arg, 0);
}
