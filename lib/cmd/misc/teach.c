/*
 * Function name: teach
 * Description:   teach someone in something
 * Arguments:     str - the players argument
 *                tp - acting living
 * Returns:       1 if success, -1 else
 */
nomask int
teach(string str, object tp)
{
    return LEARN_OB->teach_cmd(str);
}
