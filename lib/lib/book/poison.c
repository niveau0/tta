inherit "/base/book";

#include <skills.h>

static void
create_book()
{
    set_name("tome");
    add_name("book");
    add_adj("yellowed");
    set_short("yellowed tome");
    set_pshort("yellowed tomes");
    set_long("A huge yellowed tome.\n");
    set_maxpages(10);
    set_language(SK_BLACK_SPEECH);
    add_lwvs(0, 4500, 3200, 30);
    set_value(3000);
}
