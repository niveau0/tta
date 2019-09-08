nomask string
look_window(string text, string room)
{
    object r, tp;

    tp = this_player();
    tp->catch_tell(text);
    if (r = _ROOMD->load_room(room))
	tp->look_room(r);
    else
	tp->catch_tell("There is only the blurry void of a not-found "+
		       "roomfile.\n");
    return "";
}
