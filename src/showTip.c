#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>
#include <sys/ioctl.h>

#include "my_global.h"
#include "mysql.h"

#define BUFSIZE 256

void finish_with_error(MYSQL *con)
{
  fprintf(stderr, "%s\n", mysql_error(con));
  mysql_close(con);
  exit(1);        
}

void printwrap(const char *s, int lineSize, const char *prefix) 
{
  const char *head = s;
  int pos, lastSpace;

  pos = lastSpace = 0;

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
          printf("\n");

          if (isLf)
            head++;  // jump the line feed

          while (*head!=0 && *head==' ')
            head++; // clear the leading space

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
 

int main(int argc, char **argv)
{      
  struct winsize w;
  char           cmd[BUFSIZE];
  int            i, twidth;
  int            numE = 0;
  const char*    host = "rios.tacc.utexas.edu";
  const char*    user = "readerOfTips";
  const char*    pass = "tipReader123";
  const char*    db   = "HPCTips";
  

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
  
  
  /* (3) Pick random tip: idx */
  srand(time(NULL));
  int idx = rand() % (numE - 1) + 1;

  /* (4) get term width */
  
  ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
  twidth = w.ws_col - 2;


  /* (4) Select idx tip */

  sprintf(&cmd[0], "select msg from tips where tips_id = %d", idx);

  if (mysql_query(con, cmd))
    finish_with_error(con);
  
  result = mysql_store_result(con);
  
  if (result == NULL) 
    finish_with_error(con);

  num_fields = mysql_num_fields(result);

  while ((row = mysql_fetch_row(result))) 
    {
      printf("\n-------------\n"
             "Tip : %d"
             "\n-------------\n\n",idx);
      printwrap(row[0], twidth, NULL);
      printf("\n");
      break;
    }
  
  mysql_free_result(result);
  mysql_close(con);
  
  exit(0);
}
