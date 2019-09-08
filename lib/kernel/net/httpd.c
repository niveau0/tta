/* TODO:
   - http POST method
   - http PUT method (is this HTTP/1.1 ?)
   - HEAD -> file not correct
   
   Mostly finished:
   - error handling, 400, 401, 404 und 501 (see RFC1945, RFC2068)
   - HEAD method
   - GET method for html/gif/jpg/cgi files (cgi == lpc)
   - check for authorisation
   */
   
#include <config.h>
#include <filepath.h>
#include "httpd/httpd.h"

private int    pos, len, transtype;
private string file, transdata, method;

static void create()
{
    transtype = 0;
    transdata = 0;
}

static void
open()
{
}

nomask void
close(int f)
{
    if (!f)
	destruct_object();
}

private string
gmt(int t)
{
    string tstr;
    
    return (tstr=gmt_ctime(t))[..2] + ", " + tstr[8..9] + " " +
		 tstr[4..6] + " " + tstr[20..23] + " " +
		 tstr[11..18] + " GMT";
}

private string
get_dir_data(string dir, string url)
{
    int    i, sz;
    string out, *contents;

    contents = get_dir(dir);
    i = strlen(url) - 2;
    while (i >= 0 && url[i] != '/') i--;
    
    out = ("<head><title>Index of /" + url + "</title></head><body>\n" +
	   "<h1>Index of /" + url + "</h1>\n" + 
	   "<pre><img src=\"/icons/blank.gif\" alt=\"     \">" +
	   "Name                        Last modified     "+
	   "              Size     Description\n<hr>\n" +
	   "<img src=\"/icons/folder.gif\" alt=\"[dir]\"> " +
	   "<a href=\"/" + url[..i] + "\">Parent directory</a>           " +
	   gmt_ctime(file_time(dir + "..")) + "      -\n");
    
    for (i=0, sz=sizeof(contents); i<sz; i++)
    {
	if (file_size(dir + contents[i]) == -2)
	    out += ("<img src=\"/icons/folder.gif\" alt=\"[dir]\"> " +
		    "<a href=\"" + contents[i] + "/\">" +
		    sprintf("%-29s", contents[i] + "</a>") +
		    "  " + gmt_ctime(file_time(dir + contents[i])) +
		    "      -\n");
	else
	    out += ("<img src=\"/icons/text.gif\" alt=\"[txt]\"> " +
		    "<a href=\"" + contents[i] + "\">" + 
		    sprintf("%-29s", contents[i] + "</a>") +
		    "  " + gmt_ctime(file_time(dir + contents[i])) +
		    "      -\n");
    }
    out += "</pre></body>\n";
    return out;
}

private string
get_header(int num, string errstr, int t, string conttype)
{
    return "HTTP/1.0 " + num + " " + errstr + "\r\n" +
	"Date: " + gmt(t) + "\r\nServer: " + SERVERNAME +
	"\r\nContent-type: " + conttype + "\r\n";
}

private varargs void
send_error(int num, string url)
{
    string hline, text, err, aut;

    aut = "";
    switch (num)
    {
      case 400:
	  hline = "Bad request";
	  text = "The request was not understood by this server.";
	  break;
      case 401:
	  hline = "Unauthorized";
	  text = "The server could not verify that you are authorized " +
		 "to access the document you requested.";
	  aut = "WWW-Authenticate: Basic realm=\"TTA Coders\"\r\n";
	  break;
      case 404:
	  hline = "File Not found";
	  text = "The requested URL " + url + " was not found on this server.";
	  break;
      case 501:
	  hline = "Not implemented";
	  text = "The requested method is not implemented at this server.";
	  break;
    }
    
    err = sprintf("HTTP/1.0 %d %s" +
		  "\r\nDate: " + gmt(time()) +
		  "\r\nServer: " + SERVERNAME +
		  "\r\n%sContent-type: text/html\r\n" +
		  "\r\n<head><title>%s</title></head>" +
		  "<body><h1>%s</h1>%s<p></body>\r\n",
		  num, hline, aut, hline, hline, text);
    send_message(err);
    destruct_object();
}

private void
transfer() 
{
    int    length, startpos;
    string str;

    length = (pos+BLOCK_SIZE >= len) ? (len-pos) : BLOCK_SIZE;
    
    startpos = pos;
    
    if (transtype == STRING)
    {
	pos += length;
	while (!send_message(transdata[startpos..pos-1]))
	{
	    startpos = pos;
	    if (startpos >= len)
		break;
            length = (pos+BLOCK_SIZE >= len) ? (len-pos) : BLOCK_SIZE;
	    pos += length;
	}
    }
    else
    {
	str = read_file(file, startpos, length);
	pos += length;
	while (!send_message(str))
	{
	    str = read_file(file, startpos, length);
	    
	    startpos = pos;
	    if (startpos >= len)
		break;
	    length = (pos+BLOCK_SIZE >= len) ? (len-pos) : BLOCK_SIZE;	    
	    str = read_file(file, pos, length);
	    pos += length;
	}
    }
    if (startpos >= len && !send_message(-1))
	destruct_object();
}

static nomask void
message_done()
{
    if (pos >= len)
    {
        destruct_object();
        return;
    }
    transfer();
}

private string
decodeBase64(string str)
{
    int     i, j, sz, v;
    string  res;
    mapping alpha;

    if (!str)
	return 0;
    
    alpha = ([
	'A' : 0 , 'B' : 1 , 'C' : 2 , 'D' : 3 ,	'E' : 4 , 'F' : 5 ,
	'G' : 6 , 'H' : 7 , 'I' : 8 , 'J' : 9 , 'K' : 10, 'L' : 11, 
	'M' : 12, 'N' : 13, 'O' : 14, 'P' : 15,	'Q' : 16, 'R' : 17,
	'S' : 18, 'T' : 19, 'U' : 20, 'V' : 21,	'W' : 22, 'X' : 23,
	'Y' : 24, 'Z' : 25, 'a' : 26, 'b' : 27,	'c' : 28, 'd' : 29,
	'e' : 30, 'f' : 31, 'g' : 32, 'h' : 33,	'i' : 34, 'j' : 35,
	'k' : 36, 'l' : 37, 'm' : 38, 'n' : 39,	'o' : 40, 'p' : 41,
	'q' : 42, 'r' : 43, 's' : 44, 't' : 45, 'u' : 46, 'v' : 47,
	'w' : 48, 'x' : 49, 'y' : 50, 'z' : 51,	'0' : 52, '1' : 53,
	'2' : 54, '3' : 55, '4' : 56, '5' : 57,	'6' : 58, '7' : 59,
	'8' : 60, '9' : 61, '+' : 62, '/' : 63,	]);

    res = "";
    for (i=j=0, sz=strlen(str); i<sz; i+=4)
    {
	v = (alpha[str[i]] << 18) + (alpha[str[i+1]] << 12) +
	    (alpha[str[i+2]] << 6) + alpha[str[i+3]];
	res += "   ";
	res[j++] = (v & 0xff0000) >> 16;
	res[j++] = (v & 0xff00) >> 8;
	res[j++] = (v & 0xff);
    }
    i = strlen(res)-1;
    while (i >= 0 && !res[i]) i--;
    return res[..i];
}

private int
exec_cgi(string file, string data)
{
    int     i;
    string  var, ctype, *cgidata;
    mapping args;
    mixed   value;
    
    args = ([]);
    if (data && strlen(data))
    {
	cgidata = explode(data, "&");
	for (i=0; i<sizeof(cgidata); i++)
	    if (sscanf(cgidata[i],"%s=%d", var, value) == 2 ||
		sscanf(cgidata[i],"%s=%s", var, value) == 2)
		args[var] = value;
	    else
		args[cgidata[i]] = 1;
    }
    if (catch(transdata = file->start(args)) || !transdata)
	return 0;
    if (!(ctype = file->query_ctype()))
	ctype = "text/html";
    len = strlen(transdata);
    transtype = STRING;
    if (!send_message(get_header(200, "OK", time(), ctype) +
		      "Content-length: " + len +
		      "\r\nLast-modified: " +
		      gmt(time()) + "\r\n\r\n"))
	transfer();
    return 1;
}

private string
check_indexfile(string file)
{
    int    i;
    
    if (file_size(file) != -2)
	return file;
    i = strlen(file)-1;
    if (file[i-1..] == "/.")
	file = file[..i-1];
    else if (file[i] != '/')
	file += "/";
    if (file_size(file + INDEXFILE + ".c") != -1)
	file += INDEXFILE + ".c";
    if (file_size(file + INDEXFILE + ".html") != -1)
	file += INDEXFILE + ".html";
    return file;
}

private void
parse_post_data(string data)
{
    if (!len)
	sscanf(data, "%*sContent-length: %d\r\n\r\n%s", len, data);
    if (len > 1)
	transdata += data;
    if (!len || strlen(transdata) < len)
	method = "post";
    else
    {
	method = 0;
	if (transdata[len-2..] == "\r\n")
	    transdata = transdata[..len-3];
	if (sscanf(file, "%s.c", file))
	{
	    transdata += "&REQUEST_METHOD=post";
	    if (!exec_cgi(file, transdata))
		send_error(404, "");
	}
    }
}

static nomask int
receive_message(string input)
{
    int    i;
    string ctype, cmd, url, version, data, pw;

    set_this_player(0);

    if (method == "post")
    {
	parse_post_data(input);
	return 0;
    }
    if (sscanf(input, "%s /%s %s\r\n%s", cmd, url, version, data) != 4)
    {
	send_error(400);
	return 0;
    }
    
    switch (cmd)
    {
      case "GET":
	  pos = 0;
	  
	  if (sscanf(url, "icons/%s", file))
	      file = ICONS + file;
	  else
	  {
	      sscanf(url, "%s?%s", url, data);
	      file = FPATH(WWWBASE, url);
	      if (file_size(file) == -1)
	      {
		  send_error(404, url);
		  return 0;
	      }
    
	      file = check_indexfile(file);
	      i = strlen(file)-1;
	      while (i >= 0 && file[i] != '/') i--;
	      
#ifdef AUTHFILE
	      if (file_size(file[..i] + AUTHFILE) >= 0)
#endif
	      {
		  string user;
	      
		  if (!sscanf(data, "%*sAuthorization: Basic %s\r\n%*s", pw) ||
		      !(pw = decodeBase64(pw)) ||
		      sscanf(pw, "%s:%s", user, pw) != 2)
		  {
		      send_error(401);
		      return 0;
		  }
		  if (!CHECK_HTTPREAD(file, user, pw))
		  {
		      send_error(401);
		      return 0;
		  }
	      }
	      if ((len = file_size(file = check_indexfile(file))) == -2)
	      {
		  /* no indexfile, send directory */
		  transdata = get_dir_data(file, url);
		  len = strlen(transdata);
		  transtype = STRING;
		  if (!send_message(get_header(200, "OK", time(),
					       "text/html") +
				    "Content-length: " + len +
				    "\r\nLast-modified: " +
				    gmt(file_time(file)) + "\r\n\r\n"))
		      transfer();
		  return 0;
	      }
	      if (sscanf(file, "%s.c", file))
	      {
		  if (!exec_cgi(file, data))
		      send_error(404, url);
		  return 0;
	      }
	  }	  
	  
	  if (sscanf(file, "%*s.htm%*c"))
	      ctype = "text/html";
	  else if (sscanf(file, "%*s.gif"))
	      ctype = "image/gif";
	  else if (sscanf(file, "%*s.jpg"))
	      ctype = "image/jpeg";
	  else if (sscanf(file, "%*s.xpm"))
	      ctype = "image/xpm";
	  else
	  {
	      send_error(404, url);
	      return 0;
	  }
	  transtype = FILE;
	  if (!send_message(get_header(200, "OK", time(), ctype) +
			    "Content-length: " + len +
			    "\r\nLast-modified: " + gmt(file_time(file)) +
			    "\r\n\r\n"))
	    /* send more data if send_message queue is not full */
	    transfer();
	  return 0;
      case "HEAD":
	  sscanf(url, "%s?%s", url, data);
	  file = FPATH(WWWBASE, url);
	  if (file_size(file) == -1)
	  {
	      send_error(404, url);
	      return 0;
	  }
	  send_message(get_header(200, "OK", time(), "text/html") +
		       "Content-length: " + file_size(file) +
		       "\r\nLast-modified: " + gmt(file_time(file)) +
		       "\r\n\r\n");
	  destruct_object();
	  return 0;
      case "OPTIONS":
      case "POST":
	  file = FPATH(WWWBASE, url);
	  if (file_size(file) == -1)
	  {
	      send_error(404, url);
	      return 0;
	  }
	  len = 0;
	  transdata = "";
	  parse_post_data(input);
	  return 0;
      case "PUT":
      case "DELETE":
      case "TRACE":
      default:
	  send_error(501);
	  return 0;
    }

    destruct_object();
    return 0;
}
