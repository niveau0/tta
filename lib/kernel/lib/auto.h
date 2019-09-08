private void init_uid();
varargs void remove_object(object obj);
static nomask varargs void destruct(object obj);
static nomask string type_to_string(mixed value, int indent);
static nomask string program_name(object obj);
static nomask varargs string break_string(string str, int num, int indent);
static nomask mixed exclude_array(mixed arr, int first, int last);