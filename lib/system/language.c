/*
 * This file holds some standard natural language functions.
 */
static string  *nums, *numt, *numnt, *numo;
static mapping abnormal;

static nomask void
create()
{
    nums = ({ "one", "two", "three", "four", "five", "six", "seven",
              "eight", "nine", "ten","eleven","twelve","thirteen",
              "fourteen", "fifteen", "sixteen", "seventeen", "eighteen",
              "nineteen"});
    numo = ({ "first", "second", "third", "fourth", "fifth", "sixth",
              "seventh", "eighth", "ninth", "tenth","eleventh","twelfth",
              "thirteenth", "fourteenth", "fifteenth", "sixteenth",
              "seventeenth", "eighteenth", "nineteenth"});
    numt = ({ "twenty", "thirty", "forty", "fifty", "sixty", "seventy",
              "eighty", "ninety"});
    numnt = ({ "twent", "thirt", "fort", "fift", "sixt", "sevent", "eight",
	       "ninet" });
    abnormal = ([
	"ainu":"ainur",
	"bus":"busses",
	"child":"children",
	"deer":"deer",
	"die":"dice",
	"dunedan":"dunedain",
	"fish":"fish",
	"human":"humans",
	"index":"indices",
	"maia":"maiar",
	"maie":"maiar",
	"man":"men",
	"moose":"moose",
	"mouse":"mice",
	"noldo":"noldor",
	"ox":"oxen",
	"sheep":"sheep",
	"sinda":"sindar",
	"tooth":"teeth",
	"vala":"valar",
	"valie":"valar",
	"woman":"women",
	]);
}

/******************************************************************************
 * article, place "a" or "an" before a string
 */
nomask string
article(string str)
{
    int l, len;

    l = str[0] | 32;
    if (l == 'a' || l == 'e' || l == 'i' || l == 'o')
	return "an ";
    if (l == 'u')
    {
	len = strlen(str);
	if (len < 2)
	    return "a ";
	l = str[1] | 32;
	if (l == 'n')
	{
	    if (len < 3)
		return "an ";
	    if ((str[2] | 32) == 'i')
	    {
		if (len < 4)
		    return "a ";
		l = str[3] | 32;
		if (l == 'm' || l == 'n' ||
		    (len > 7 && str[..6] == "unident"))
		    return "an ";
		return "a ";
	    }
	}
	if (l == 'r' || l == 's' || l == 't')
	{
	    l = str[2] | 32;
	    if (l == 'a' || l == 'e' || l == 'i' || l == 'o' || l == 'u')
		return "a ";
	    return "an ";
	}
	if (str == "ubiquitous")
	    return "a ";
	return "an ";
    }
    return "a ";
}

nomask string
add_article(string str) 
{
    return article(str)+str;
}

/******************************************************************************
 * return integer for a number word, negative if number represents an
 * ordinal-number
 */
nomask int
word_2_num(string str)
{
    int    value, pos;
    string ext, *num;

    if (!str)
	return 0;

    if (sscanf(str, "%d", value) > 0)
    {
	if (str[strlen(str)-1] == '.')
	    return -value;
	return value;
    }
    if (sscanf(str, "%sy%s", str, ext) == 2)
    {
        if ((pos = member_array(str, numnt)) == -1)
            return 0;
	value = (pos + 2) * 10;
	str = ext;
    }
    else
    {
    	if ((pos = member_array(str, nums)) > -1)
    	    return pos + 1;
    	if ((pos = member_array(str, numo)) > -1)
    	    return -pos - 1;
    }
    if ((pos = member_array(str, numo)) > -1)
    	return -value - pos - 1;
    if ((pos = member_array(str, nums)) > -1)
    	return value + pos + 1;
    return value;
}

/******************************************************************************
 * returns a number in text form
 */
nomask string
num_2_word(int num)
{
    if (num < 1)
	return "zero";
    if (num < 20)
	return nums[num-1];
    if (num > 99)
    {
	if (num > 999)
	{
	    if (num > 999999)
		return "many";
	    return num_2_word(num / 1000) + "thousand" +
		num_2_word(num % 1000);
	}
	return (num_2_word(num / 100) + "hundred" + (num % 100 > 0 ? 
						     num_2_word(num % 100) :
						      ""));
    }
    if ((num % 10) == 0)
	return numt[num / 10 - 2];
    else
	return numt[num / 10 - 2] + nums[num % 10 - 1];
}

/*****************************************************************************
 * singular to plural converter
 */
nomask string
plural_word(string str)
{
    int sl, ch;

    if (!str)
	return 0;
    if (abnormal[str])
	return abnormal[str];
    
    sl = strlen(str) - 1;
    if (sl < 1)
	return str;
    ch = str[sl];
    switch (ch)
    {
      case 's':
	return str[..sl-1] + "ses";
      case 'x':
	return str[..sl-1] + "xes";
      case 'h':
	return str[..sl-1] + "hs";
      case 'y':
	if ((str[sl-1] == 'a') || (str[sl-1] == 'e') ||
	    (str[sl-1] == 'o') || (str[sl-1] == 'u'))
	    break;
	return str[..sl-1] + "ies";
      case 'f':
	return str[..sl-1] + "ves";
      case 'e':
	if (str[sl-1] == 'f')
	    return str[..sl-2] + "ves";
    }
    return str + "s";
}

nomask mixed
plural_sentence(mixed str)
{
    int    c, i, sz, sz2;
    string *a;

    if (!pointerp(str))
    {
	if (!str)
	    return 0;
    
	a = explode(str, " ");
	if ((sz = sizeof(a)) < 1)
	    return 0;
	for (c=1; c<sz; c++)
	{
	    if (a[c] == "of")
	    {
		a[c-1] = plural_word(a[c-1]);
		return implode(a, " ");
	    }
	}
	a[sz-1] = plural_word(a[sz-1]);
	return implode(a, " ");
    }
	
    for (i=0, sz2 = sizeof(str); i<sz2; i++)
    {
	if (!str[i])
	    return 0;
    
	a = explode(str[i], " ");
	if ((sz = sizeof(a)) < 1)
	    continue;
	for (c=1; c<sz; c++)
	{
	    if (a[c] == "of")
	    {
		a[c-1] = plural_word(a[c-1]);
		str[i] = implode(a, " ");
	    }
	}
	a[sz-1] = plural_word(a[sz-1]);
	str[i] = implode(a, " ");
    }
    return str;
}

/****************************************************************************
 * Name in possessive form
 */
nomask string
name_possessive(string str)
{
    if (!strlen(str))
	return 0;
    
    switch (str)
    {
      case "it": return "its";
      case "It": return "Its";
      case "he": return "his";
      case "He": return "His";
      case "she": return "her";
      case "She": return "Her";
    }
    if (extract(str, strlen(str) - 1) == "s")
	return (str + "'");
    return (str + "'s");
}

nomask string
ord_2_word(int num)
{
    int tmp;

    if (num < 1)
	return "zero";
    if (num < 20)
	return numo[num-1];
    if (num > 99)
    {
        if (num > 999)
	{
            if (num > 9999)   /* was 999999 */
                return "many";
            if (num == 1000)
                return "thousandth";
            if (!(tmp = num % 1000))
                return num_2_word(num / 1000) + " thousandth";
            return num_2_word(num / 1000) + " thousand " +
                ord_2_word(tmp);
        }
        if (num == 100)
            return "hundredth";
   	if (!(tmp = num % 100))
            return num_2_word(num / 100) + " hundredth";
        return num_2_word(num / 100) + " hundred " +
	    ord_2_word(tmp);
    }
    if (!(tmp = num % 10))
        return numnt[num / 10 - 2] + "ieth";
    return numt[num / 10 - 2] + " " + numo[tmp - 1];
}

nomask int
word_2_ord(string str)
{
    int i;

    if (!str)
	return 0;

    if ((i = member_array(str, numo)) == -1)
        return 0;
    return i + 1;
}
