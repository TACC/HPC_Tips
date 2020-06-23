#include <iostream>
#include <sstream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>

#include <unistd.h>
#include <sys/ioctl.h>

#include "my_global.h"
#include "mysql.h"
#include "version.h"

int issueWarning = 1;

void finish_with_error(MYSQL *con)
{
  if (issueWarning)
    {
      fprintf(stderr, "HPC_Tips %s\n", mysql_error(con));
      mysql_close(con);
    }
  exit(1);        
}


std::string wrap(std::string line, size_t line_length = 72)
{
  std::string indent = "   ";
  std::istringstream words(line);
  std::ostringstream wrapped;
  std::string word;

  std::string::size_type found = line.find_first_not_of(" ");
  if (found != std::string::npos)
    indent += line.substr(0, found);

  wrapped << indent;
  size_t space_left = line_length - indent.length();


  if (words >> word) {
    wrapped << word;
    space_left -= word.length();
    while (words >> word) {
      if (space_left < word.length() + 1) {
        wrapped << '\n' << indent << word;
        space_left = line_length - word.length() - indent.length();
      } else {
        wrapped << ' ' << word;
        space_left -= word.length() + 1;
      }
    }
  }
  return wrapped.str();
}

void printwrap(const char *s, int lineSize, const char *prefix) 
{
  const char *head = s;
  int pos, lastSpace;

  pos = lastSpace = 0;

  printf("   ");
  while(head[pos]!=0)
    {

      int isLf = (head[pos]=='\n');

      if (isLf || pos==lineSize)
        {
          if (isLf || lastSpace == 0)
            lastSpace = pos;  // just cut it

          if (prefix!=NULL)
            printf("%s", prefix); 

          while(*head!=0 && lastSpace-- > 0) 
            printf("%c", *head++); 
          printf("\n  ");

          if (isLf)
            head++;  // jump the line feed

          //while (*head!=0 && *head==' ')
          //   head++; // clear the leading space

          lastSpace = pos = 0;

        } 
      else
        {
          if (head[pos]==' ') { lastSpace = pos; }
          pos++;
        }
    }
  printf("%s\n", head);
}
 
void timer_handler(int sig)
{
  exit(0);
}


void printUsage(const char* cmd)
{
  printf("%s [options]\n\n"
	 "Options:\n"
	 " -h -?            : Print usage\n"
	 " -v               : Print version\n"
	 " -a               : print all tips\n"
	 " -w               : do not print warnings\n"
	 " -n num           : print tip num\n",
	 cmd);
}

MYSQL_RES * printOneTip(MYSQL* con, int twidth, const char* hlp, const char* moduleNm, int idx)
{
  MYSQL_RES*  result;
  char*       cmd    = NULL;
  char*       hlpMsg = NULL;
  MYSQL_ROW   row;

  asprintf(&cmd, "select msg from tips where tips_id = %d", idx);

  if (mysql_query(con, cmd))
    finish_with_error(con);
  
  free(cmd);
  result = mysql_store_result(con);
  
  if (result == NULL) 
    finish_with_error(con);

  int num_fields = mysql_num_fields(result);
  asprintf(&hlpMsg,hlp, moduleNm);

  while ((row = mysql_fetch_row(result)))
    {
      std::cout << "\nTip " << idx << "   " << hlpMsg << "\n\n";
      std::stringstream whole(row[0]);
      std::string line;
      while (std::getline(whole, line, '\n'))
        {
          std::string oline = wrap(line, twidth);
          std::cout << oline << "\n";
        }
      std::cout << "\n";
    }

  free(hlpMsg);
  return result;
}



#define INTERVAL 400000

int main(int argc, char **argv)
{      
  struct winsize w;
  int            i, twidth, opt;
  int            numE     = 0;
  int            idx      = -1;
  int            help     = 0;
  int            ver      = 0;
  int            factor   = 1;
  int            all      = 0;
  int            bypass   = 0;
  const char*    host     = "xalt.tacc.utexas.edu";
  const char*    user     = "readerOfTips";
  const char*    pass     = "tipReader123";
  const char*    db       = "HPCTips";
  const char*    moduleNm = "tacc_tips";
  const char*    hlp      = "(See \"module help %s\" for features or how to disable)";
  


  struct itimerval timer;
  
  while ( (opt = getopt(argc, argv, "avn:wWh?")) != -1)
    {
      switch (opt)
	{
	case 'a':
	  all = 1;
	  break;
	case 'v':
	  ver = 1;
	  break;
	case 'n':
	  idx = strtol(optarg, (char **) NULL, 10);
	  break;
	case 'w':
	  issueWarning = 0;
	  break;
	case 'W':
	  bypass = 1;
	  break;
	case 'h':
	case '?':
	  help = 1;
	  break;
	}
    }

  if (ver)
    {
      printf("showTip Version: %s\n",HPC_TIPS_VERSION);
      return 0;
    }


  if (help)
    {
      printUsage(argv[0]);
      return 0;
    }

  if (all)
    factor = 100;

  if (! bypass)
    {
      timer.it_interval.tv_sec = 0;
      timer.it_interval.tv_usec = 0;
      timer.it_value.tv_sec = 0;
      timer.it_value.tv_usec = factor*INTERVAL;
      setitimer(ITIMER_REAL, &timer,0);
      
      signal(SIGALRM,timer_handler);
    }



  MYSQL *con = mysql_init(NULL);
  
  if (con == NULL)
    {
      fprintf(stderr, "mysql_init() failed\n");
      exit(1);
    }  
  
  /* (1) Open DB */

  if (mysql_real_connect(con, host, user, pass, db, 3306, NULL, 0) == NULL) 
    finish_with_error(con);
  
  /* (2) Find number of tips */

  if (mysql_query(con, "SELECT count(*) FROM tips")) 
    finish_with_error(con);
  
  MYSQL_RES *result = mysql_store_result(con);
  
  if (result == NULL) 
    finish_with_error(con);

  int num_fields = mysql_num_fields(result);

  MYSQL_ROW row;
  
  while ((row = mysql_fetch_row(result))) 
    { 
      numE = strtol(row[0], (char **) NULL, 10);
      break;
    }
  
  /* (3) get term width */
  
  ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
  twidth = w.ws_col - 3;
  
  /* (4) Select idx tip */
  if (idx < 0 || idx > numE)
    {
      /* (4a) Pick random tip: idx */
      srand(time(NULL));
      idx = rand() % (numE - 1) + 1;
    }

  if (! all)
    result = printOneTip(con,twidth, hlp, moduleNm, idx);
  else
    {
      for (idx = 1; idx <= numE; ++idx)
	result = printOneTip(con,twidth, "%s", "", idx);
    }

  mysql_free_result(result);
  mysql_close(con);
  
  exit(0);
}
