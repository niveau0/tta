inherit"apple";

#include <effects.h>


static void
create_food()
{
    ::create_food();
    switch(random(27))
    {
      case 1:
	  set_poison(POISON_DIR + "acaana", 0);
	  break;
      case 2:
	  set_poison(POISON_DIR + "carnegurth", 0);
	  break;
      case 3:
	  set_poison(POISON_DIR + "jegga", 0);
	  break;
      case 4:
	  set_poison(POISON_DIR + "jeggarukh", 0);
	  break;
      case 5:
	  set_poison(POISON_DIR + "jitsu", 0);
	  break;
      case 6:
	  set_poison(POISON_DIR + "jitsutyr", 0);
	  break;
      case 7:
	  set_poison(POISON_DIR + "karfar", 0);
	  break;
      case 8:
	  set_poison(POISON_DIR + "kly", 0);
	  break;
      case 9:
	  set_poison(POISON_DIR + "sharkasar", 0);
	  break;
      case 10:
	  set_poison(POISON_DIR + "silmaana", 0);
	  break;
      case 11:
	  set_poison(POISON_DIR + "uraana", 0);
	  break;
      case 12:
	  set_poison(POISON_DIR + "dynallca", 0);
	  break;
      case 13:
	  set_poison(POISON_DIR + "trusa", 0);
	  break;
      case 14:
	  set_poison(POISON_DIR + "zaganzar", 0);
	  break;
      case 15:
	  set_poison(POISON_DIR + "galenaana", 0);
	  break;
      case 16:
	  set_poison(POISON_DIR + "taynaga", 0);
	  break;
      case 17:
	  set_poison(POISON_DIR + "henuial", 0);
	  break;
      case 18:
	  set_poison(POISON_DIR + "klytun", 0);
	  break;
      case 19:
	  set_poison(POISON_DIR + "muilfana", 0);
	  break;
      case 20:
	  set_poison(POISON_DIR + "nimnaur", 0);
	  break;
      case 21:
	  set_poison(POISON_DIR + "ondohithui", 0);
	  break;
      case 22:
	  set_poison(POISON_DIR + "ul_acaana", 0);
	  break;
      case 23:
	  set_poison(POISON_DIR + "bragolith", 0);
	  break;
      case 24:
	  set_poison(POISON_DIR + "brithagurth", 0);
	  break;
      case 25:
	  set_poison(POISON_DIR + "asgurash", 0);
	  break;
      default:
	  set_poison(POISON_DIR + "vuraana", 0);
	  break;
    } 
} 
