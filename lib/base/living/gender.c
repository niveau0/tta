/* contains all gender related functions */

int     gender;          /* it, he, she */

/*
 * Function name:   query_gender_string
 * Description:     Gives back a string that contains the gender of a living
 * Returns:         The string
 */
nomask string
query_gender_string()
{
    if (gender == G_MALE)
	return "male";
    if (gender == G_FEMALE)
	return "female";
    return "neuter";
}

/*
 * Function name:   set_gender(int g)
 * Description:     Set the gender code (G_MALE, G_FEMALE or G_NEUTER)
 * Arguments:       g - The gender code
 */
nomask void
set_gender(int g)
{
    if (g == G_MALE || g == G_FEMALE || g == G_NEUTER)
	gender = g; 
}

/*
 * Function name:   query_gender()
 * Description:     Returns the gender code of the living.
 * Returns:         The code.
 */
nomask int
query_gender()
{
    return gender;
}

/*
 * Function name:   query_pronoun()
 * Description:     Returns the pronoun that goes with the gender of this
 *                  living.
 * Returns:         "he", "she" or "it", depending on gender.
 */
string
query_pronoun()
{
    if (gender == G_MALE)
	return "he";
    if (gender == G_FEMALE)
	return "she";
    return "it";
}

/*
 * Function name:   query_possessive()
 * Description:     Returns the possessive that goes with the gender of this
 *                  living.
 * Returns:         "his", "her" or "its", depending on gender.
 */
string
query_possessive()
{
    if (gender == G_MALE)
	return "his";
    if (gender == G_FEMALE)
	return "her";
    return "its";
}

/*
 * Function name:   query_objective()
 * Description:     Returns the objective that goes with the gender of this
 *                  living.
 * Returns:         "him", "her" or "it", depending on gender.
 */
string
query_objective()
{
    if (gender == G_MALE)
	return "him";
    if (gender == G_FEMALE)
	return "her";
    return "it";
}
