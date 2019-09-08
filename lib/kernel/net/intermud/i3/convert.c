
private int    elem;
private string *data;

private mixed type_check();

private int
next_char()
{
    data[elem] = data[elem][1..];

    if (!strlen(data[elem]))
    {
	elem++;
	if (elem >= sizeof(data))
	    return -1;
	return 1;
    }
    return 0;
}


private mixed
convert_number()
{
    int    chr, is_float, is_exp;
    string result;
    
    if (!strlen(data[elem]))
	error("Digit value '0-9,-,.' expected");
    result = "";

    if (data[elem][0] == '-')
    {
	next_char();
	result += "-";
    }
    else if (data[elem][0] == '+')
	next_char();

    while (1)
    {
	if (!strlen(data[elem]))
	    break;

	chr = data[elem][0];
	if (chr == '.')
	{
	    if (is_float)
		error("Second decimal sign '.' found");
	    is_float = 1;
	    result += ".";
	    next_char();
	}
	else if (chr == 'e')
	{
	    if (is_exp)
		error("Second exponent sign 'e' found");
	    if (next_char() == -1)
		error("Exponent expected");
	    result += "e";
	    if (data[elem][0] == '-')
	    {
		if (next_char() == -1)
		    error("Exponent expected");
		result += "-";
	    }
	    else if (data[elem][0] == '+')
		if (next_char() == -1)
		    error("Exponent expected");
	    if (data[elem][0] < '0' || data[elem][0] > '9')
		error("Exponent expected");
	    is_exp = 1;
	}
	else if (chr == '+')
	{
	    if (!is_exp)
		break;
	    next_char();
	}
	else if (chr < '0' || chr > '9')
	    break;
	else 
	{
	    result += data[elem][0..0];
	    next_char();
	}
    }
    if (!strlen(result))
	error("Value expected");

    if (is_float || is_exp)
	return (float)result;
    return (int)result;
}

private string
convert_string()
{
    string result;
    
    if (!strlen(data[elem]))
	error("String end '\"' expected");
    result = "";
    while (1)
    {
	if (next_char() == -1)
	    error("String end '\"' expected");
	if (data[elem][0] == '"')
	{
	    next_char();
	    break;
	}
	if (data[elem][0] == '\\')
	{
	    if (next_char() == -1)
		error("String end '\"' expected");

	    switch (data[elem][0])
	    {
	    case '0': result += "\0"; break;
	    case 'a': result += "\a"; break;
	    case 'b': result += "\b"; break;
	    case 't': result += "\t"; break;
	    case 'n': result += "\n"; break;
	    case 'v': result += "\v"; break;
	    case 'f': result += "\f"; break;
	    case 'r': result += "\r"; break;
	    default:
		result += data[elem][0..0];
	    }
	}
	else
	    result += data[elem][0..0];
    }

    return result;
}

private mapping
convert_mapping()
{
    int     end;
    mapping result;
    mixed   tmpval;
    
    result = ([ ]);
    while (1)
    {
	if (next_char() == -1)
	    error("Mapping end '])' expected");
        if (data[elem][0] == ']')
        {
            if (end != 0) error("Mapping end '])' expected");
            end = 1;
            continue;
        }
        else if (end == 1)
        {
            if (data[elem][0] == ')') 
            {
                next_char();
                break;
            }
	    error("Mapping end '])' expected");
        }
        else if (data[elem][0] == ')' && end != 1)
	    error("Mapping end '])' expected");
	tmpval = type_check();
	if (pointerp(tmpval) || mappingp(tmpval))
	    error("Bad mapping index");
	if (!strlen(data[elem]) || data[elem][0] != ':')
	    error("Seperator ':' expected");
	if (next_char() == -1)
	    error("Mapping value expected");
	result[tmpval] = type_check();
	if (!strlen(data[elem]) || data[elem][0] != ',')
	    error("Seperator ',' expected");
    }

    return result;
}

private mixed
convert_array()
{
    int   end;
    mixed result, tmpval;
    
    result = ({ });
    while (1)
    {
	if (next_char() == -1)
	    error("Array end '})' expected");
        if (data[elem][0] == '}')
        {
            if (end != 0) error("Array end '})' expected");
            end = 1;
            continue;
        }
        else if (end == 1)
        {
            if (data[elem][0] == ')') 
            {
                next_char();
                break;
            }
            error("Array end '})' expected");
        }
        else if (data[elem][0] == ')' && end != 1)
            error("Array end '})' expected");

	tmpval = type_check();
	if (!strlen(data[elem]) || data[elem][0] != ',')
	    error("Seperator ',' expected");
	result += ({ tmpval });
    }

    return result;
}

private mixed
type_check()
{
    switch (data[elem][0])
    {
      case '"':
	  return convert_string();
      case '(':
          next_char();
	  if (data[elem][0] == '[')
	      return convert_mapping();
	  else if (data[elem][0] == '{')
	      return convert_array();
	  break;
      default:
	  return convert_number();
    }
    error("Failed to convert");
}

nomask mixed
to_value(string *d)
{
    if (sizeof(d) < 1)
	error("Bad argument 1 to function to_value");
    data = d;
    elem = 0;
    return type_check();
}

nomask string
to_string(mixed value)
{
    int    i, sz;
    string result;
    mixed  ind;
    
    switch (typeof(value))
    {
      case T_INT:
      case T_FLOAT:
	  return (string)value;
      case T_STRING:
	  result = implode(explode(value, "\\"), "\\\\");
	  result = implode(explode(result, "\""), "\\\"");
	  return "\"" + result + "\"";
      case T_ARRAY:
	  result = "({";
	  for (i=0, sz=sizeof(value); i<sz; i++)
	      result += to_string(value[i]) + ",";
	  return result + "})";
      case T_MAPPING:
	  ind = map_indices(value);
	  result = "([";
	  for (i=0, sz=sizeof(ind); i<sz; i++)
	      result += to_string(ind[i]) + ":" +
			to_string(value[ind[i]]) + ",";
	  return result + "])";
    }
    error("Type serialisation not supported");
}
