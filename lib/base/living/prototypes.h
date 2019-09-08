/* /base/object.c */
static nomask void enable_commands();
static nomask void disable_commands();
void add_name(string name);
void remove_name(mixed name);
void remove_adj(mixed adj);
int query_size();
nomask int move(object dest);
nomask varargs int * query_coordinates(int *co);
nomask void set_coordinates(int *c);
string * query_names();
string * query_adjs();
nomask void add_prop(mixed prop, mixed val);
nomask void add_state(int s);
nomask void delete_state(int s);
nomask int change_int_prop(mixed prop, int val);
nomask int query_state();
nomask string query_name();
nomask mixed query_prop(mixed prop);
nomask int query_light();

/* /base/container.c */
nomask int query_internal_weight();
nomask int query_internal_volume();
nomask int query_weight();
nomask int query_volume();


/* /base/living.c */
int query_linkdead();
varargs void catch_tell(string str, int no_prompt);
void set_linkdead(int i);
nomask int query_tell_flags();
nomask void look_room(object room);
nomask void add_tell_flags(int i);
nomask void remove_tell_flags(int i);
nomask void add_extra_desc(string desc);
nomask void remove_extra_desc(string desc);
nomask void reset_extra_desc();
nomask object query_user();
nomask varargs void catch_msg(mixed str, object from, int type,
			      int dist, int no_prompt);


/* /base/living/carry.c */
int query_max_volume();
int query_max_weight();
nomask int weight_encumbrance();


/* /base/living/cmd.c */
int command(string cmd);

/* /base/living/combat.c */
int do_die(object killer);
nomask int query_hp();
nomask int query_max_hp();
nomask int check_death(int damage, object killer);
nomask int add_bloodloss(int bl, int slot);
nomask int query_bloodloss();
nomask int add_unconscious(int t);
nomask mapping query_bloodwounds();


/* /base/living/combat_intern.c */
nomask int cb_query_bonus(int id);
nomask void cb_stop_fight(mixed e);
nomask object cb_query_attacker();
nomask void cb_set_parry_skill(int sk);
static nomask void cb_add_attack(int slot, mixed data);
static nomask void cb_add_hitloc(int slot, int *data);
static nomask void cb_remove_attack(int slot);
static nomask void cb_remove_hitloc(int slot);


/* /base/living/extra.c */
nomask int query_fatigue();
nomask int query_max_fatigue();
nomask int can_see(object ob);
nomask int query_npc();
nomask int resistance_roll(int attack_type, int attack_level, int modifier);
nomask void set_race(string name);
nomask void add_fatigue(int f);
nomask void add_effect(int type, mixed path, int count);
nomask void remove_effect(int type, mixed path);
nomask string query_race();
nomask string query_introduce_string();
static nomask void set_level(int l);


/* /base/living/gender.c */
nomask string query_gender_string();
string query_objective();
string query_possessive();
string query_pronoun();


/* /base/living/magic.c */
nomask int query_realm();
nomask int query_mana();
nomask int query_max_mana();

/* /base/living/move.c */
nomask void set_move_msg(string m);
nomask void set_run_msg(string m);
nomask void set_teleport_in(string m);
nomask void set_teleport_out(string m);
nomask string query_move_msg();
nomask string query_run_msg();
nomask string query_teleport_in();
nomask string query_teleport_out();


/* /base/living/skill.c */
nomask int query_skill_rank(int stat);
nomask int query_skill_bonus(int skill);
nomask int * query_skill_attrib(int skill);
nomask void calc_skill_bonus(int skill);
nomask void reset_skill_boni();


/* /base/living/slots.c */
nomask int * query_slot_type(int type);
nomask void add_tool_slot(int slot, string desc, int size);
nomask mixed query_wield(int slot);
nomask string query_slot_desc(int slot);
nomask object * query_hold(int slot);
nomask object * query_worn(int slot);
nomask object * query_protection(int slot);
nomask varargs int remove_tool(object tool, int flag);
nomask mapping query_wounds();
static nomask void set_connections(mapping c);

    
/* /base/living/stat.c */
nomask int query_stat(int stat);
nomask int query_stat_bonus(int stat);
nomask int query_stat_race_bonus(int stat);
static nomask void calc_dev_points();
static nomask void improve_stats();

/* /base/living/description.c */
string query_met(string name);
nomask string query_nonmet_name();
