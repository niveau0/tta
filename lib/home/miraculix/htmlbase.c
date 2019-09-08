#define HTMLHEAD \
"<!doctype html PUBLIC \"-//W3C//DTD HTML 3.2//EN\">\n"+ \
"<html>\n"+ \
"<head><title>The Third Age - %s</title></head>\n"+ \
"\n"+ \
"<body\n"+ \
"background=\"../pic/back.jpg\"\n"+ \
"text=\"#000000\"\n"+ \
"link=\"#701000\"\n"+ \
"vlink=\"#406000\"\n"+ \
"alink=\"#003300\"\n"+ \
">\n"+ \
"\n"+ \
"<a href=\"../index.html\">\n"+ \
"<img border=0 src=\"../pic/"+ \
"tta.mini.gif\"\nalt=\"TTA\" align=left></a>\n"+ \
"<br>"+ \
"<center>\n"+ \
"<h3>The Third Age - %s</h3>\n"+ \
"</center>\n"+ \
"\n"+ \
"<br>\n"+ \
"\n"+ \
"<hr noshade><p>\n"+ \
"\n"

#define HTMLTRAIL \
"<p><hr noshade>\n"+ \
"\n"+ \
"<address>&#169; <a href=\"mailto:Markus.Tippmann@"+ \
"informatik.uni-oldenburg.de\">manwe</a>, %s </address>\n"+ \
"</body>\n"+ \
"</html>\n"

static nomask string
get_htmlhead(string title)
{
    return sprintf(HTMLHEAD, title, title);
}

static nomask string
get_htmltrail()
{
    string t;

    t = ctime(time());
    t = t[4..9] + ", " + t[20..];
    return sprintf(HTMLTRAIL, t);
}
