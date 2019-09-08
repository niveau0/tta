
/* name: seconds to brew,
   ingrediens and their amount,
   amount of water,
   LIQ_EFFECT,
   difficulty
   look of the brewing
*/

#define RECIPE_TIME        0
#define RECIPE_INGREDIENTS 1
#define RECIPE_WATER       2
#define RECIPE_LIQ_EFFECT  3
#define RECIPE_DIFFICULTY  4
#define RECIPE_LOOK        5

#define RECIPES  \
([							\
    "baalak":     ({ 250,				\
		    ({ INGREDIENT_BAALAK, 1, }),	\
		    120,				\
		    LIQ_BAALAK,				\
		    5,                                  \
		    "milky yellow" }),	       		\
    "bursthelas": ({ 300,				\
		    ({ INGREDIENT_BURSTHELAS, 1, }),	\
		    100,				\
		    LIQ_BURSTHELAS,			\
		    5 , 				\
		    "clear green" }),	       		\
    "eldaana":    ({ 300,				\
		    ({ INGREDIENT_ELDAANA, 1, }),	\
		    120,				\
		    LIQ_ELDAANA,			\
		    5 , 				\
		    "milky white" }),	       		\
    "fek":        ({ 240,				\
		    ({ INGREDIENT_FEK, 2,               \
		       INGREDIENT_EGG, 1, }),      	\
		    100,				\
		    LIQ_FEK,				\
		    5 , 				\
		    "metallic blue" }),	       		\
    "menelar":    ({ 300,				\
		    ({ INGREDIENT_MENELAR, 1, }),	\
		    150,				\
		    LIQ_MENELAR,			\
		    5 , 				\
		    "red" }),   	       		\
    "quilmufur":  ({ 280,				\
		    ({ INGREDIENT_QUILMUFUR, 1, }),	\
		    500,				\
		    LIQ_QUILMUFUR,			\
		    5 , 				\
		    "green" }), 	       		\
    "reglen":     ({ 500,				\
		    ({ INGREDIENT_REGLEN, 1, }),	\
		    200,				\
		    LIQ_REGLEN,				\
		    5 , 				\
		    "dark yellow" }),	       		\
    "rewk":       ({ 120,				\
		    ({ INGREDIENT_REWK, 1, }),	        \
		    100,				\
		    LIQ_REWK,				\
		    5 , 				\
		    "light green" }),	       		\
    "thurl":      ({ 360,				\
		    ({ INGREDIENT_THURL, 1, }),	        \
		    70,					\
		    LIQ_THURL,				\
		    5 ,  				\
		    "dark white" }),	       		\
    "soup":       ({ 400,	   			\
		    ({ INGREDIENT_EGG, 2, }),           \
		    500,                                \
		    LIQ_SOUP,                           \
		    1 ,                                 \
		    "clear yellow" }),	       		\
])
