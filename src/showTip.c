#include "my_global.h"
#include "mysql.h"

#define BUFSIZE 256

void finish_with_error(MYSQL *con)
{
  fprintf(stderr, "%s\n", mysql_error(con));
  mysql_close(con);
  exit(1);        
}

int main(int argc, char **argv)
{      
  char        cmd[BUFSIZE];
  int         i;
  int         numE = 0;
  const char* host = "localhost";
  const char* user = "readerOfTips";
  const char* pass = "tipReader123";
  const char* db   = "HPCTips";
  

  MYSQL *con = mysql_init(NULL);
  
  if (con == NULL)
    {
      fprintf(stderr, "mysql_init() failed\n");
      exit(1);
    }  
  
  /* (1) Open DB */

  if (mysql_real_connect(con, host, user, pass, db, 0, NULL, 0) == NULL) 
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

  /* (4) Select idx tip */

  sprintf(&cmd[0], "select msg from tips where %d", idx);

  if (mysql_query(con, cmd))
    finish_with_error(con);
  
  result = mysql_store_result(con);
  
  if (result == NULL) 
    finish_with_error(con);

  num_fields = mysql_num_fields(result);

  while ((row = mysql_fetch_row(result))) 
    { 
      for(i = 0; i < num_fields; i++) 
	printf("%s ", row[i] ? row[i] : "NULL"); 
      printf("\n"); 
    }
  
  mysql_free_result(result);
  mysql_close(con);
  
  exit(0);
}
