#ifndef MATERIAL_H
#define MATERIAL_H

/* After adding new materials please also add their burning
 * abilities in 'burn.h' .
 */

/* normal */
#define M_SILK			 1
#define M_LINEN			 2
#define M_COTTON		 3
#define M_GLASS			 4
#define M_STONE			 5
#define M_BONE			 6
#define M_IVORY			 7
#define M_PAPER			 8
#define M_LEATHER		 9
#define M_EARTH		        10
#define M_AIR		        11
#define M_LIQUID	        12
#define M_ICE	                13
#define M_WOOL	                14
#define M_WAX	                15
#define M_FOOD	                16
#define M_FLESH	                17

/* gems */			  
#define M_GEMS_FIRST            30
#define M_DIAMOND		30
#define M_EMERALD		31
#define M_RUBY			32
#define M_SAPPHIRE		33
#define M_OPAL			34
#define M_PEARL			35
#define M_GEMS_LAST             35
				  
/* wood */			  
#define M_WOOD_FIRST            40
#define M_TREANT		40
#define M_ELF_GOLDEN_LEAF	41
#define M_ELVISH_ASH		42
#define M_ELVISH_HAZEL		43
#define M_ELVISH_OAK		44
#define M_ELVISH_YEW		45
#define M_ASH			46
#define M_HAZEL			47
#define M_OAK			48
#define M_YEW			49
#define M_LINDEN		50
#define M_OTHER			51
#define M_WOOD_LAST             51
				  
/* metal */			  
#define M_METAL                 60
#define M_METAL_FIRST	        60 
#define M_COPPER		60
#define M_BRONZE		61
#define M_SILVER		62
#define M_GOLD		        63
#define M_IRON			64
#define M_STEEL			65
#define M_HIGH_STEEL		66
#define M_WHITE_ALLOY		67
#define M_BLACK_ALLOY		68
#define M_METAL_LAST	        68 
				  
/* magic */			  
#define M_MAGICMETAL_FIRST	80
#define M_ENCH_IRON		80
#define M_ENCH_STEEL		81
#define M_ENCH_HIGH_STEEL	82
#define M_MITHRIL_ALLOY		83
#define M_LAEN			84
#define M_EOG			85
#define M_MAGICMETAL_LAST	85

/* the following materials are allowed to burn to dust,
 * so the object will be removed
 */
#define M_INFLAMMABLE \
({ \
       M_SILK,              \
       M_LINEN,             \
       M_COTTON,            \
       M_PAPER,             \
       M_WOOL,              \
       M_WAX,               \
       M_TREANT,            \
       M_ELF_GOLDEN_LEAF,   \
       M_ELVISH_ASH,        \
       M_ELVISH_HAZEL,      \
       M_ELVISH_OAK,        \
       M_ELVISH_YEW,        \
       M_ASH,               \
       M_HAZEL,             \
       M_OAK,               \
       M_YEW,               \
       M_LINDEN,            \
       M_OTHER,             \
       })

#endif
