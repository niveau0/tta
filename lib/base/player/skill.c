nomask void save();

mapping skill_usage;

/*
 * Function name: skill_usage
 * Description:   mark a skill as used, increases the counter
 * Arguments:     skill - the used skill id
 */
static nomask void
skill_usage(int skill)
{
    if (!mappingp(skill_usage))
	skill_usage = ([ skill:1 ]);
    else
	skill_usage[skill]++;
}

/*
 * Function name: query_skill_usage
 * Description:   gives the skill usage mapping
 * Returns:       mapping
 */
nomask mapping
query_skill_usage()
{
    return skill_usage;
}

/*
 * Function name: reset_skill_usage
 * Description:   reset skill usage mapping to 0
 */
nomask void
reset_skill_usage()
{
    skill_usage = 0;
}
