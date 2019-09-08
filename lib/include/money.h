#ifndef MONEY_H
#define MONEY_H

#define MONEY_FILE "/system/money"

/* coin types */
#define ORB           1
#define GOLD_HUNDRED  2
#define GOLD_CROWN    3
#define SILVER_TREE   4
#define SILVER_ROYAL  5
#define BRONZE_COPPER 6
#define COPPER_HALF   7
#define COPPER_BIT    8

#define COIN_NAMES    			\
({                    			\
   "orb",				\
   "goldhundred",			\
   "goldcrown",				\
   "silvertree",			\
   "silverroyal",			\
   "bronzecopper",			\
   "copperhalf",			\
   "copperbit",				\
});

#define COIN_SHORT_NAMES \
({ "orb", "gh", "gc", "st", "sr", "bc", "ch", "cb", });

#define COIN_VALUES ({ 10000,10000, 1000, 200, 50, 5, 2, 1 })
#define COIN_WEIGHT ({ 10, 25, 18, 12, 12, 10, 10, 6 })
#define COIN_VOLUME ({ 5, 8, 6, 4, 4, 4, 4, 2 })
#define COIN_SIZE   ({ 2, 3, 2, 2, 2, 1, 1, 1 })

/*
 * Macro:       MONEY_MAKE
 * Description: prepares a money object
 * Arguments:   num  - how much money to produce (of a given type)
 *              type - which money type, see coin types abovee
 *              to   - move money to which object? if not given
 *                     moneyobject will be removed
 * Returns:     objectpointer to the money heap object
 */
#define MONEY_MAKE(num, type, to) \
call_other(MONEY_FILE, "make_coins", num, type, to)

/*
 * Macro:       MONEY_TOTAL
 * Description: calculates the total money value a living carries
 * Arguments:   l - which living to check
 * Returns:     total money in copper bits
 */
#define MONEY_TOTAL(l) \
call_other(MONEY_FILE, "total", l)

/*
 * Macro:       MONEY_CHANGE
 * Description: add or reduce money of living
 * Arguments:   n - the amount in copper bits, negative reduces
 *              l - the living whose money changes
 * Returns:     1 if successful, 0 if not enough money to take
 */
#define MONEY_CHANGE(n, l) \
call_other(MONEY_FILE, "change", n, l)

/*
 * Macro:       MONEY_TEXT, MONEY_SHORT_DESC
 * Description: create a textual description for a given amount
 * Arguments:   n 
 * Returns:     string
 */
#define MONEY_TEXT(n) \
call_other(MONEY_FILE, "money_text", n)
#define MONEY_SHORT_DESC(n) \
call_other(MONEY_FILE, "short_desc", n)

#endif
