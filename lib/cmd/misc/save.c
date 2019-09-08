/*
 * Function name: save
 * Description:   saves the player to disk
 * Arguments:     str - not used
 *                tp - acting living
 * Returns:       1 if success, -1 else
 */
nomask int
save(string str, object tp)
{
    tp->save();
    tp->echo("Your character has been saved.\n");
    return 1;
}
