/* tool for player creation */

#include <config.h>
#include <macros.h>
#include <magic.h>
#include <living.h>
#include <race.h>
#include <skills.h>
#include <properties.h>

#include "/config/login/generation.h"

static nomask void height_and_weight(string race);
static nomask void roll_stats();
static nomask void set_language(string race);

mapping data;

static string  name;
static object  player, tp;
static mapping sk_desc;

static nomask void
create()
{
    sk_desc = SK_DESC;
}

/*
 * Function name: new_player
 * Description:   called by generation object (see <config.h>)
 * Arguments:     d - new player choices
 */
nomask void
new_player(mapping d)
{
    if (object_name(previous_object()) != LOGIN_GENERATE_PLAYER)
	error("Illegal call to new_player");

    data = d;
    save_object(LOGIN_APPLICATION_DIR + "/" + data[GEN_NAME]);
}

/*
 * Function name: cr_create
 * Description:   called by the command `create' (CODER_SOUL)
 * Arguments:     str - player name
 * Returns:       1 if success, -1 else
 */
nomask int
cr_create(string str)
{
    if (!IS_CLONE)
	return -1;
    if (object_name(previous_object()) != CODER_SOUL)
    {
	destruct_object();
	return -1;
    }
    tp = this_player();
    if (tp->query_prop(CURRENT_EDITOR))
    {
	tp->catch_tell("You are already in edit mode.\n");
	destruct_object();
	return -1;
    }
    if (str)
    {
	if (_SYSTEMD->exist_player(str))
	{
	    tp->catch_tell("This player already exists.\n");
	    destruct_object();
	    return -1;
	}
	if (file_size(LOGIN_APPLICATION_DIR + "/" + str + ".o") < 1)
	{
	    tp->catch_tell("No such application file: " +
			   LOGIN_APPLICATION_DIR + "/" + str + ".o\n");
	    destruct_object();
	    return -1;
	}
	restore_object(LOGIN_APPLICATION_DIR + "/" + str);
	player = clone_object(RACE_FILE(data[GEN_RACE]));
	player->set_email(data[GEN_EMAIL]);
	player->set_race(data[GEN_RACE]);
	player->set_name(name = data[GEN_NAME]);
	player->set_gender(data[GEN_GENDER] == "male" ? G_MALE : G_FEMALE);
	player->set_realm(REALM_ID[data[GEN_REALM]]);
	height_and_weight(data[GEN_RACE]);
	roll_stats();
	set_language(data[GEN_RACE]);
	player->set_coordinates(tp->query_coordinates());
	player->move(E(tp));
	tp->catch_tell("Player application loaded.\n" +
		       "Email:  " + data[GEN_EMAIL] + "\n" +
		       "Race:   " + data[GEN_RACE] + "\n" +
		       "Name:   " + data[GEN_NAME] + "\n" +
		       "Desc:   " + data[GEN_DESC] + "\n" +
		       "Gender: " + data[GEN_GENDER] + "\n" +
		       "Talent: " + data[GEN_TALENT] + "\n" +
		       "Realm:  " + data[GEN_REALM] + "\n" +
		       "Set the allowed adjectives by yourself.\n");
    }
    tp->add_prop(CURRENT_EDITOR, this_object());
    tp->define_primary("catch_cmd", this_object());
    return 1;
}

nomask object
query_editor()
{
    return tp;
}

nomask object
query_player()
{
    return player;
}

static nomask int
save()
{
    if (!player)
    {
	tp->catch_tell("You need to edit the race.\n");
	return -1;
    }
    if (!name)
    {
	tp->catch_tell("You must set a name.\n");
	return -1;
    }
    if (!player->query_password())
    {
	tp->catch_tell("You must set a password.\n");
	return -1;
    }
    make_dir(PLAYER_DIR(name));
    player->start_new_player();
    if (file_size(PLAYER_FILE(name) + ".o") > 0)
    {
	restore_object("/syslog/POPULATION");
	if (!data)
	    data = ([ ]);
	data[player->query_race()]++;
	save_object("/syslog/POPULATION");
	tp->catch_tell("Ok.\n");
	remove_file(LOGIN_APPLICATION_DIR + "/" + name + ".o");
    }
    else
	tp->catch_tell("Failed.\n");
    return 1;
}

/*
 * Function name: trans_pot_stat
 * Description:   compute potential stat from roll and tmp stat
 * Arguments:     roll - a dice roll (0-100)
 *                tmp - tmp stat (21 - 100)
 * Returns:       integer, potential stat
 */
static nomask int
trans_pot_stat(int roll, int tmp)
{
    int pot;

    pot = (tmp + roll * (101 - tmp) / 100) - (4 - (tmp - 1) / 20);
    if (tmp > pot)
	return tmp;
    return pot;
}

/*
 * Function name: roll_stats
 * Description:   create character stats
 */
static nomask void
roll_stats()
{
    int i, *tmp_stats, *pot_stats;

    tmp_stats = allocate(STAT_LAST + 1);
    pot_stats = allocate(STAT_LAST + 1);
    for (i=STAT_FIRST; i<=STAT_LAST; i++)    
    {
	tmp_stats[i] = random(80) + 21;
	pot_stats[i] = trans_pot_stat(random(101), tmp_stats[i]);
    }
    for (i=STAT_FIRST; i<=STAT_LAST; i++)
    {
	player->set_tmp_stat(i, tmp_stats[i]);
	player->set_pot_stat(i, pot_stats[i]);
    }
    player->set_base_hp(player->query_pot_stat(STAT_CO) / 10);
    player->add_hp(player->query_max_hp());
}

static nomask void
stat_ch2(string str, int stat)
{
    int tmp, pot;
    
    if (!str || sscanf(str, "%d %d", tmp, pot) != 2)
    {
	tp->catch_tell("Nothing changed.\n");
	return;
    }
    if (pot < tmp)
    {
	tp->catch_tell("Potential must be greater or equal temporal.\n" +
		       "New values: ", 1);
	input_to("stat_ch2", 0, stat);
	return;
    }
    player->set_tmp_stat(stat, tmp);
    player->set_pot_stat(stat, pot);
    tp->catch_tell("New values set.\n");
}

static nomask void
stat_ch1(string str)
{
    int stat;
    
    if (!str || (stat = member_array(str, STAT_ABBREV)) == -1)
    {
	tp->catch_tell("Wrong stat.\n");
	return;
    }
    tp->catch_tell("New values (tmp pot, seperate by space): ", 1);
    input_to("stat_ch2", 0, stat);
}

static nomask void
change_stat()
{
    tp->catch_tell(implode(STAT_ABBREV, ", ") +"\nStat to change: ", 1);
    input_to("stat_ch1", 0);
}

static nomask void
change_skill_prac(string str, int skill)
{
    int value;
    
    if (str)
    {
	sscanf(str, "%d", value);
	player->set_skill_rank(skill, value);
    }
    tp->catch_tell("Skill value (theoretical): ", 1);
    input_to("change_skill_theo", 0, skill);
}

static nomask void
change_skill_theo(string str, int skill)
{
    int value;
    
    if (str)
    {
	sscanf(str, "%d", value);
	player->set_theoretical_rank(skill, value);
    }
    tp->catch_tell("New skill values set.\n");
}

static nomask void
height_and_weight(string race)
{
    int   w, v, s;
    float height, weight, roll;
    mixed mod;

    roll = (float) roll_dice(-1);
    height = 66.0 + (roll / 14.0);

    mod = LEARN_OB->query_race_mod(race);
    height *= mod[HEIGHT_MOD];

    if (height < 25.0)
	height = 25.0;

    weight = (height * 0.08898 * /* calculate weight, by density */
	      height * 0.08898 * /* assuming humanoid forms. */
	      height * 0.00136 * 
	      44.0 * (float) mod[WEIGHT_MOD] *
	      0.454);		 /* convert pounds to kg */
    if (height < 50.0)
	weight += 20.0;
    else if (height < 60.0)
	weight += 30.0;
    
    if (player->query_gender() == G_FEMALE)
	weight *= 0.9;

    w = ((int)weight + random(20) - 10) * 1000;
    v = w * 90 / 100;
    s = (int) height;
    player->set_lwvs(0,w,v,s);
}

static nomask void
set_language(string race)
{
    int rnd, lang1, lang2, lang3;
    
    switch (race)
    {
      case "arcticman":
	  lang1 = SK_WESTRON;
	  lang2 = SK_ADUNAIC;
	  lang3 = SK_SINDARIN;
	  break;
      case "halfelf":
      case "woodelf":
      case "sinda":
	  lang1 = SK_SINDARIN;
	  lang2 = SK_WESTRON; 
	  lang3 = SK_QUENYA;
	  break;
      case "noldo":
	  lang1 = SK_SINDARIN;
	  lang2 = SK_QUENYA;
	  lang3 = SK_WESTRON;
	  break;
      case "dwarf":
	  lang1 = SK_KHUZDUL; 
	  lang2 = SK_WESTRON;
	  break;
      case "dunadan":
      case "northman":
	  lang1 = SK_WESTRON;
	  lang2 = SK_SINDARIN;
	  lang3 = SK_ADUNAIC;
      default:
	  lang1 = SK_WESTRON;
    }
    player->add_prop(LIVE_USED_LANGUAGE, lang1);
    player->set_skill_rank(lang1, 20);
    player->set_theoretical_rank(lang1, 20);
    if (lang2)
    {
	rnd = random(7) + 5;
	player->set_skill_rank(lang2, rnd);
	player->set_theoretical_rank(lang2, rnd);
    }
    if (lang3)
    {
	rnd = random(5) + 3;
	player->set_skill_rank(lang3, rnd);
	player->set_theoretical_rank(lang3, rnd);
    }
}

/****************************************************************************
 * Functions called by edit command
 */
static nomask void
do_adj(string str)
{
    if (!str)
    {
	tp->catch_tell("Nothing changed.\n");
	return;
    }
    player->add_adj(str);
    tp->catch_tell("New adjective added.\n");
}

static nomask void
do_gender(string str)
{
    switch(str)
    {
      case "n":
      case "neuter":
	  player->set_gender(G_NEUTER);
	  break;
      case "m":
      case "male":
	  player->set_gender(G_MALE);
	  break;
      case "f":
      case "female":
	  player->set_gender(G_FEMALE);
	  break;
      default:
	  tp->catch_tell("Wrong gender.\n");
	  return;
    }
    tp->catch_tell("New gender set.\n");
}

static nomask void
do_email(string str)
{
    if (!str)
    {
	tp->catch_tell("Nothing changed.\n");
	return;
    }
    player->set_email(str);
    tp->catch_tell("New email set.\n");
}

static nomask void
do_name(string str)
{
    if (_SYSTEMD->exist_player(str))
    {
	tp->catch_tell("This player already exists.\n");
	return;
    }
    name = str;
    player->set_name(str);
    tp->catch_tell("New name set.\n");
}
	
static nomask void
do_password(string str)
{
    player->set_password(crypt(str));
    tp->catch_tell("New password set.\n");
}
	
static nomask void
do_race(string str)
{
    if (!str || !IN_ARRAY(str, RACES))
    {
	tp->catch_tell("Wrong race.\n");
	return;
    }
    if (player)
    {
	tp->catch_tell("Start a new player to change the race.\n");
	return;
    }
    player = clone_object(RACE_FILE(str));
    
    player->set_race(str);
    player->set_name("nobody");
    player->set_realm(CHA);
    height_and_weight(str);
    roll_stats();
    set_language(str);
    
    player->set_coordinates(tp->query_coordinates());
    player->move(E(tp));

    tp->catch_tell("New race set.\n");
}

static nomask void
do_realm(string str)
{
    int i;
    
    if (!(i = REALM_ID[str]))
    {
	tp->catch_tell("Wrong realm.\n");
	return;
    }
    player->set_realm(i);
    tp->catch_tell("New realm set.\n");
}

static nomask void
do_skill(string str)
{
    int i, *skills;
    
    skills = map_indices(sk_desc);
    for (i=sizeof(skills); i--;)
    {
	if (str == sk_desc[skills[i]])
	    break;
    }
    if (i < 0)
    {
	tp->catch_tell("Wrong skill.\n");
	return;
    }
    tp->catch_tell("Skill value (practical): ", 1);
    input_to("change_skill_prac", 0, skills[i]);
}

static nomask void
do_stats(string str)
{
    if (str == "r")
    {
	roll_stats();
	tp->catch_tell("New stats set.\n");
    }
    else if (str == "c")
	change_stat();
    else
	tp->catch_tell("Wrong input.\n");
}

static nomask void
do_size(string str)
{
    int s;
    
    if (str && sscanf(str, "%d", s))
    {
	player->add_lwvs(0, 0, 0, s - player->query_size());
	tp->catch_tell("New size set.\n");
	return;
    }
    tp->catch_tell("Nothing changed.\n");
}

static nomask void
do_volume(string str)
{
    int v;
    
    if (str && sscanf(str, "%d", v))
    {
	player->add_lwvs(0, 0, v - player->query_volume(), 0);
	tp->catch_tell("New volume set.\n");
	return;
    }
    tp->catch_tell("Nothing changed.\n");
}

static nomask void
do_weight(string str)
{
    int w;
    
    if (str && sscanf(str, "%d", w))
    {
	player->add_lwvs(0, w - player->query_weight(), 0, 0);
	tp->catch_tell("New weight set.\n");
	return;
    }
    tp->catch_tell("Nothing changed.\n");
}

/*
 * Function name: edit
 * Description:   choose something to change
 * Arguments:     arg - what to change
 * Returns:       1 if successful, -1 else
 */
static nomask int
edit(string cmd)
{
    string arg;

    if (cmd)
	sscanf(cmd, "%s %s", cmd, arg);
    if (!player && cmd != "race")
    {
	tp->catch_tell("First you need to choose the race.\n");
	return -1;
    }
    if (arg)
    {
	switch (cmd)
	{
	  case "adj":
	      do_adj(arg);
	      return 1;
	  case "gender":
	      do_gender(arg);
	      return 1;
	  case "mailaddr":
	      do_email(arg);
	      return 1;
	  case "name":
	      do_name(arg);
	      return 1;
	  case "passwd":
	      do_password(arg);
	      return 1;
	  case "race":
	      do_race(arg);
	      return 1;
	  case "realm":
	      do_realm(arg);
	      return 1;
	  case "skill":
	      do_skill(arg);
	      return 1;
	  case "stats":
	      do_stats(arg);
	      return 1;
	  case "size":
	      do_size(arg);
	      return 1;
	  case "volume":
	      do_volume(arg);
	      return 1;
	  case "weight":
	      do_weight(arg);
	      return 1;
	  default:
	      tp->catch_tell("No such edit command, try '?' for help.\n");
	      return -1;
	}
    }
    switch (cmd)
    {
      case "adj":
	  tp->catch_tell("New adjective: ");
	  input_to("do_adj");
	  return 1;
      case "gender":
	  tp->catch_tell("(n)eutral, (m)ale, (f)emale: ");
	  input_to("do_gender");
	  return 1;
      case "mailaddr":
	  tp->catch_tell("New email: ");
	  input_to("do_email");
	  return 1;
      case "name":
	  tp->catch_tell("New name: ");
	  input_to("do_name");
	  return 1;
      case "passwd":
	  tp->catch_tell("New password: ");
	  input_to("do_password");
	  return 1;
      case "race":
	  tp->catch_tell("Possible races:\n" +
			 sprintf("% - *78s", implode(RACES, "\n")) +
			 "New race: ");
	  input_to("do_race");
	  return 1;
      case "realm":
	  tp->catch_tell("New realm, cha, ess, men, arc: ");
	  input_to("do_realm");
	  return 1;
      case "skill":
	  tp->catch_tell("Skill to change: ");
	  input_to("do_skill");
	  return 1;
      case "stats":
	  tp->catch_tell("(r)eroll stats, (c)hange stat: ");
	  input_to("do_stats");
	  return 1;
      case "size":
	  tp->catch_tell("New size: ");
	  input_to("do_size");
	  return 1;
      case "volume":
	  tp->catch_tell("New volume: ");
	  input_to("do_volume");
	  return 1;
      case "weight":
	  tp->catch_tell("New weight: ");
	  input_to("do_weight");
	  return 1;
      default:
	  tp->catch_tell("No such edit command, try '?' for help.\n");
	  return -1;
    }
}

/*
 * Function name: help
 * Description:   show help pages
 * Arguments:     arg - optional help page name
 * Returns:       1 if success, -1 else
 */
static nomask int
help(string arg)
{
    if (!arg)
    {
	tp->catch_tell("Use 'edit <argument>' to edit the following " +
		       "characteristics:\n" +
		       " adj      - set adjectives\n" +
		       " gender   - characters sex\n" +
		       " mailaddr - mail address\n" +
		       " name     - player name\n" +
		       " passwd   - initial password\n" +
		       " race     - elf, human, etc.\n" + 
		       " realm    - magic realm\n" + 
		       " skill    - change skills\n" +
		       " stats    - physical stats\n" +
		       " size     - in inch\n" +
		       " volume   - in milliliter\n" +
		       " weight   - in gram\n" +
		       "Use 'save' to save the new player to disk\n" +
		       "Use 'done' to finish editing\n");
    }
    return 1;
}

/*
 * Function name: catch_cmd 
 * Description:   catch every command while editing
 * Arguments:     cmd - command
 *                arg - command argument
 * Returns:       1 if successful, -1 if not, 0 if not valid
 */
nomask int
catch_cmd(string cmd, string arg)
{
    if (!tp || cmd == "done")
    {
	tp->undefine_primary("catch_cmd", this_object());
	if (player)
	    remove_object(player);
	destruct_object();
	return 1;
    }
    if (tp != this_player())
	return 0;
    if (cmd == "?")
	return help(arg);
    if (cmd == "edit")
	return edit(arg);
    if (cmd == "save")
	return save();
    return 0;
}
