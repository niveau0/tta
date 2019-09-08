
static int time_create;
static int time_rot;
static int time_decompose;

create_rot()
{
	time_create = time();
}

set_rot(int rot, int decomp) 
{
	time_rot = time_create + rot;
	time_decompose = time_create + decomp;
}

long()
{
	check_rot();
	::long();
}

short()
{
	check_rot();
	::short();
}
