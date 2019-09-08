/*
 * Function name: learn
 * Description:   learn a skill
 * Arguments:     str - the players argument
 *                tp - acting living
 * Returns:       1 if success, -1 else
 */
nomask int
learn(string str, object tp)
{
    return LEARN_OB->learn_cmd(str);
}
