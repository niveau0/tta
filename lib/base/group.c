inherit "object";

#include <group.h>
#include <macros.h>
#include <properties.h>

object group_leader, *group_members;

static void
create_group()
{
    if(!group_members)
	group_members = ({ });
}

static nomask void
create_object()
{
    set_name("group");
    add_prop(IS_CONT,1);
    create_group();
}

string
short()
{
}

string
long()
{
}

void
add_member(object obj, object member)
{
    if(!obj || !member)
	return;
    if(obj != group_leader)
    {
	obj->catch_tell("You are not the group leader and though cannot add "+
			"new members.\n");
	return;
    }
    if(!living(member))
    {
	obj->catch_tell("You cannot add that to your group.\n");
	return;
    }
    group_members += ({ member });
    obj->catch_msg(({"You added "})+QCTNAME(member)+({" to your group.\n"}));
}

void
remove_member(object obj, object member)
{
    if(!obj || !member)
	return;
    if(obj != group_leader)
    {
	obj->catch_tell("You are not the group leader and though cannot "+
			"remove members.\n");
	return;
    }
    if(!IN_ARRAY(member, group_members))
    {
	obj->catch_tell("That is not in your group.\n");
	return;
    }
    group_members -= ({ member });
    obj->catch_msg( ({"You removed "}) + QCTNAME(member) +
                    ({" from your group.\n"}));
}

// return a lost member automatically
int
return_member(object member)
{
}

void
disband_group(object obj)
{
}

object
query_group_leader()
{
    return group_leader;
}

object *
query_group_members()
{
    return group_members;
}

int
query_group_member_number()
{
    return sizeof(group_members);
}

void
set_group_leader(object leader)
{
    group_leader = leader;
}

