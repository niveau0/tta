/* Amount of data moved in one operation */
#define BLOCK 16060
        
/* Where do we have the html files ? */
#define BASE "/home/hubbe/ulpc/src/"
        
/* File to return when we can't find the file requested */
#define NOFILE "/home/hubbe/www/html/nofile.html"
        
/* Port to open */
#define PORT 1905

void write_callback()
{
    int written;
    string data;
    file::seek(offset);
    data=file::read(BLOCK);
	    

    if(strlen(data))
    {    written=socket::write(data);

	 if(written >= 0)
	 {
	     offset+=written;
	     return;
	 }
		 

	 perror("Error: "+socket::errno()+".\n");
    }
    destruct_object(this_object());
}
 
string input="";
void read_callback(mixed id,string data)
{
    string cmd;
        
    input+=data;
    if(sscanf(input,"%s %s%*[\012\015 \t]",cmd,input))
    {
	if(cmd!="GET")
	{
	    perror("Only method GET is supported.\n");
	    destruct_object(this_object());
	    return;
	}
		   
	sscanf(input,"%*[/]%s",input);
	input=combine_path(BASE,input);
		   

              
	if(!file::open(input,"r"))
	{
		  

	    if(!file::open(NOFILE,"r"))
	    {
		perror("Couldn't find default file.\n");
		destruct_object(this_object());
		return;
	    }
	}
	socket::set_buffer(65536,"w");
	socket::set_nonblocking(0,write_callback,0);
	write_callback();
    }
}


void selfdestruct_object() { destruct_object(this_object()); }

void create(object f)
{
    socket::assign(f);
	      

    socket::set_nonblocking(read_callback,0,selfdestruct);

}

};


        void accept_callback()
        {
object tmp_output;
tmp_output=accept();
if(!tmp_output) return;
clone(output_class, tmp_output);
destruct_object(tmp_output);

        }



        int main(int argc, string *argv)
        {
          perror("Starting minimal httpd\n");

        
          if(!bind(PORT, accept_callback))
          {
            perror("Failed to open socket (already bound?)\n");
            return 17;
}   return - 17; /* Keep going */


        }
