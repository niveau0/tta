static nomask void
do_climate(string str)
{
    int i;
    
    if ((i = member_array(str, CLIMATES)) == -1)
	tp->catch_tell("No such climate, cancelled.\n");
    else
    {
	climate = CLIMATES[i+1];
	tp->catch_tell("New climate set.\n");
    }
}
