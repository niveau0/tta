/* **************************************************************************
 * ed - start the editor
 */
nomask int
ed(string str)
{
    CHECK_LEVEL(_BUILDER);

    this_interactive()->query_user()->ed(str);
    return 1;
}
