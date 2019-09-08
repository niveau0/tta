#ifndef LANGUAGE_H
#define LANGUAGE_H

#include <config.h>

#define LANG_FILE "/system/language"

/* LANG_PWORD  -- Get the plural form of a noun
 * LANG_PSENT  -- Get the plural form of a noun phrase
 * LANG_ART    -- Get the article of a noun
 * LANG_ADDART -- Get the article of a noun + the noun
 * LANG_POSS   -- Get the possessive form of a name
 * LANG_NUMW   -- Get the number of textform
 * LANG_WNUM   -- Get the textform of a number
 * LANG_ORDW   -- Get the text in ordinal from from a number, 2 -> "second"
 * LANG_WORD   -- Get the number of an ordinal textform, "first" -> 1
 * LANG_MOD    -- modify sentence by language skill
 */

#define LANG_PWORD(x)  call_other(LANG_FILE,"plural_word", x)
#define LANG_PSENT(x)  call_other(LANG_FILE,"plural_sentence", x)
#define LANG_ART(x)    call_other(LANG_FILE,"article", x)
#define LANG_ADDART(x) call_other(LANG_FILE,"add_article", x)
#define LANG_POSS(x)   call_other(LANG_FILE,"name_possessive", x)
#define LANG_NUMW(x)   call_other(LANG_FILE,"num_2_word", x)
#define LANG_WNUM(x)   call_other(LANG_FILE,"word_2_num", x)
#define LANG_ORDW(x)   call_other(LANG_FILE,"ord_2_word", x)
#define LANG_WORD(x)   call_other(LANG_FILE,"word_2_ord", x)
#define LANG_MOD(s, l, o) call_other(MISC_SOUL, "modify_string", s, l, o)

#endif
