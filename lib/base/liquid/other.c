static void
brewing(int amount, object tp)
{
    tp->catch_tell("Ouch! That was a bit too hot! " +
		   "Better careful next time.\n");
    tp->add_hp(-1);
}

static void
juice(int amount, object tp)
{
}

static void
milk(int amount, object tp)
{
}

static void
oil(int amount, object tp)
{
}

static void
vinegar(int amount, object tp)
{
}

static void
water(int amount, object tp)
{
}

static void
soup(int amount, object tp)
{
    tp->catch_tell("Yummie. This soup tastes wonderful.\n");
    tp->add_food(amount);
}
