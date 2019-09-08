/* **************************************************************************
 * mud_status - statistics about memory usage
 */
nomask int
mud_status(string str)
{
    int    i, sz, alloc, mem, dt;
    float  fm, om;
    object o, l, r, tp, *obj;
    mixed  data;

    CHECK_LEVEL(_BUILDER);
    
    tp = this_interactive();
    if (str && strlen(str))
    {
	if (sizeof(obj = parse(str, I(E(tp), 0), 0)))
	    o = obj[0];
	else if (!(o = find_living(str)))
	{
	    str = FPATH(tp->query_path(), str);
	    if (!str || !strlen(str) || !(o = find_object(str)))
	    {
		tp->catch_tell("No such object.\n");
		return -1;
	    }
	}
	data = status(o);
	str = ("\nData Sizes: " + data[O_PROGSIZE] + 
	       " bytes (program) " + data[O_DATASIZE] + " bytes (data).\n"+
	       "Callouts:   Function                    Time\n"+
	       "            -------------------------------------------\n");
	sz = sizeof(data[O_CALLOUTS]);
	for (i=0; i<sz; i++)
	    str += sprintf("            %-25s %6d\n",
			   data[O_CALLOUTS][i][3],
			   data[O_CALLOUTS][i][2]);
	tp->catch_tell(str);
	return 1;
    }
    data = status();
    mem  = data[ST_SMEMSIZE] + data[ST_DMEMSIZE];
    om   = floor((float)(data[ST_SWAPRATE1]) * 100.0 / 60.0) / 100.0;
    fm   = floor((float)(data[ST_SWAPRATE5]) * 100.0 / 300.0) / 100.0;

    if (!timestamp)
	timestamp = time();
    if (!memory)
	memory = mem;

    dt = time() - timestamp;
    alloc = mem - memory;
    if (dt > 60)
    {
	timestamp = time();
	memory = mem;
    }

    str = "\n --------- Status information for TTA ( DGD-" + data[ST_VERSION] +
	  " ) ---------\n" +
	  "Swap device:           " + data[ST_SWAPSIZE] + " sectors (" +
	  data[ST_SWAPUSED] + " used, " +
	  (data[ST_SWAPUSED] * 100 / data[ST_SWAPSIZE])+
	  "%). Sectorsize: " + data[ST_SECTORSIZE] + 
	  " byte\nObjects swapped out:   " + data[ST_SWAPRATE1] +
	  " per minute (" + om + " per sec.)\n                       " +
	  data[ST_SWAPRATE5] + " in the last 5 minutes (" + fm +
	  " per sec.).\n\nMemory usage:          " +
	  (data[ST_SMEMUSED]/1024) + " Kb of " + 
	  (data[ST_SMEMSIZE]/1024) + " Kb static.\n                       " +
	  (data[ST_DMEMUSED]/1024) + " Kb of " + 
	  (data[ST_DMEMSIZE]/1024) + " Kb dynamic.\nMemory allocated:      " +
	  mem + " byte (" + (mem - data[ST_SMEMUSED] - data[ST_DMEMUSED]) +
	  " byte free).\n                       " + alloc +
	  " allocated in the last " + dt +
	  " seconds.\n\nObjects:               " + data[ST_NOBJECTS] +
	  " of max. " + data[ST_OTABSIZE] + "\nCall-outs:             " +
	  (data[ST_NCOSHORT] + data[ST_NCOLONG]) +
	  " total of max. " + data[ST_COTABSIZE] +
	  "\n                       " + data[ST_NCOSHORT] + 
	  " short (0-127 seconds), " + data[ST_NCOLONG] +
	  " long (128+ seconds)\n\nLimits:                Array/Map: " +
	  data[ST_ARRAYSIZE] +
	  " indices\n                       Stringsize: " + data[ST_STRSIZE] + 
	  " characters\n                       Editor: " + data[ST_ETABSIZE] + 
	  " instances\n                       Users: " + data[ST_UTABSIZE] +
	  " connections\n";
    tp->catch_tell(str);
    return 1;
}
