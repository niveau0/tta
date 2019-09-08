/*
 *    MuhveliMud - Ident information receiver
 * 
 *    28.10.95 - Sieni
 */ 
 
#include <mudcon.h>

object user;
int port;
string host;
int localport;
int closed;
int id;

varargs void set_data(object obj, string h, int p, int p2, int i)
{
  user=obj;
  host=h;
  port=p;
  localport=p2;
  id=i;
}

string query_host()
{
  return host;
}

void open()
{
  send_message(port + " , "+localport+"\n");
  call_out("time_out",60);
}

void close() 
{  
  if(!closed)
  {
    closed=1;
    destruct_object();
  }
}

int execute_ident(string host)
{
  string str;
  str=catch(connect(host,113,"tcp"));
  
  if(str) destruct_object();  
  call_out("time_out",60);  
}

int receive_message(string str)
{
  int a,b,c,d;
  string *arr;
  
  if(str[strlen(str)-1]=='\n') str=str[0..strlen(str)-3];
  
  arr=explode(str,":");
  
  for(a=0;a<sizeof(arr);a++)
  {
    while(arr[a][0]==' ') 
    { if(strlen(arr[a])>1) arr[a]=arr[a][1..]; else arr[a]="";  }
    while((b=strlen(arr[a]))&&arr[a][b-1]==' ') { if(strlen(arr[a])>1) arr[a]=arr[a][0..b-2]; else arr[a]=""; }
  }  
    
  if(arrayp(arr)&&sizeof(arr)==4)
  {   
   if(arr[1]=="USERID")
   {    
     if(user)  user->set_userid(arr[3],id,port);
   }
   else
     log_file("NOIDENT",host+" "+str+"\n");
  }  
  else
   log_file("NOIDENT",host+" "+str+"\n");  
   
  if(!closed)
  { 
    closed=1;  
    destruct_object();  
  }
}

void time_out()
{
  DRIVER->stdout("Timeout.");
  closed=1;
  destruct_object();
}
