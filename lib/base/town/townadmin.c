/*
 * town administration object
 * keeps all town related information, including:
 * - shop room object pointer
 * - 
 */

#include <town.h>

private mapping towndata;

static void
create_town()
{
}

static nomask void
create()
{
    towndata = ([ ]);
    create_town();
}

nomask void
set_town_data(int index, mixed data)
{
    towndata[index] = data;
}

nomask mixed
query_town_data(int index)
{
    return towndata[index];
}
