/*
 * Function name: stats
 * Description:   personal information
 * Arguments:     str - not used
 *                tp - acting living
 * Returns:       1 if success, else -1
 */
nomask int
stats(string str, object tp)
{
    int    r, y, m, d, birth, *sizes;
    string out, realm, tmp, *age;

    if ((r = tp->query_realm()) > 0)
    {
	if (r == CHA)
	    realm = "cha";
	else if (r == MEN)
	    realm = "ment";
	else if (r == ESS)
	    realm = "ess";
	else if (r == ARC)
	    realm = "arc";
    }
    if (birth = time() - tp->query_birth())
    {
	sscanf(CONV_REALTIME(birth * TIME_SCALE + 18 * 365 * 24 * 3600),
	       "%d year%*s, %d month%*s, %d day%*s", y, m, d);
	age = ({ });
	if (y > 0)
	    age += ({ y + " year" + (y > 1 ? "s" : "") });
	if (m > 0)
	    age += ({ m + " month" + (m > 1 ? "s" : "") });
	if (d > 0)
	    age += ({ d + " day" + (d > 1 ? "s" : "") });
    }
    out = "\n=========================== Personal information " +
          "============================\n" +
	  " You are " + tp->query_presentation(tp);
    if (sizes = RACE_SIZES[tp->query_race()])
    {
	out += " and ";
	switch (100 * (tp->query_size()-sizes[0]) / (sizes[1]-sizes[0]))
	{
	  case -1000..20:
	      out += "very small"; break;
	  case 21..40:
	      out += "small"; break;
	  case 41..70:
	      out += "medium sized"; break;
	  case 71..90:
	      out += "tall"; break;
	  case 91..100:
	      out += "very tall"; break;
	  case 101..1000:
	      out += "unnaturally tall"; break;
	}
	out += " for that race";
    }
    out += ".\n" + (r > 0 ? " Your magic realm is '" + realm + "'.\n" : "\n");
    tmp = tp->query_state_desc(tp);
    out += (tmp == "" ? "" : " " + tmp) +
	   " You are carrying " + tp->weight_encumbrance() +
	   "% of your weight and " + tp->volume_encumbrance() +
	   "% of your volume encumbrance.\n" +
	   (tp->query_hunger() > 30
	    ? " You could eat some food."
	    : " You are not hungry.") +
	   (tp->query_thirst() > 10
	    ? " You could drink some water.\n"
	    : " You are not thirsty.\n") +
	   " You can at least learn " +
	   LANG_ADDART(LEARN_OB->
		       difficulty_desc(tp->query_development_points())) +
	   " skill.\n" +
	   (birth ? " You are " + COMPOSITE_WORDS(age) + " old.\n" : "") + 
	   " Email: " + tp->query_email() + "\n";
    
    tp->catch_tell(out);
    return 1;
}
