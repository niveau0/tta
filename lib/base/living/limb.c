inherit "/base/object";

static void
create_object()
{
    ::set_name("limb");
}

void
set_name(string n)
{
    string split;
    
    ::set_name(n);
    if (n && sscanf(n, "%*sright %s", split) == 2)
	::set_name(split);
    else if (n && sscanf(n, "%*sleft %s", split) == 2)
	::set_name(split);
}
