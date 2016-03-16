#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mysql.h>
#include <my_global.h>
#define SZ 128

char* fgets_alloc(FILE *fp)
{
  char* buf = NULL;
  size_t size = 0;
  size_t used = 0;
  do {
    size += SZ;
    char *buf_new = (char *) realloc(buf, size);
    if (buf_new == NULL) {
      // Out-of-memory
      free(buf);
      return NULL;
    }
    buf = buf_new;
    if (fgets(&buf[used], (int) (size - used), fp) == NULL) {
      // feof or ferror
      if (used == 0 || ferror(fp)) {
        free(buf);
        buf = NULL;
      }
      return buf;
    }
    size_t length = strlen(&buf[used]);
    if (length + 1 != size - used) break;
    used += length;
  } while (buf[used - 1] != '\n');
  return buf;
}

void print_stmt_error(MYSQL_STMT *stmt, const char *message)
{
  fprintf(stderr,"%s\n", message);
  if (stmt != NULL)
    {
      fprintf(stderr,"Error %u (%s): %s\n",
              mysql_stmt_errno(stmt),
              mysql_stmt_sqlstate(stmt),
              mysql_stmt_error(stmt));
    }
}

const int issueWarning = 1;
void finish_with_error(MYSQL *conn)
{
  if (issueWarning)
    {
      fprintf(stderr, "RTM %s\n", mysql_error(conn));
      mysql_close(conn);
    }
  exit(1);        
}

int main(int argc, char* argv[])
{


  const char*    host	= "localhost";
  const char*    user	= "tipBuilder";
  const char*    pass	= "test623";
  const char*    db	= "HPCTips";
  
  MYSQL *conn = mysql_init(NULL);
  
  if (conn == NULL)
    {
      fprintf(stderr, "mysql_init() failed\n");
      exit(1);
    }  
  
  /* (1) Open DB */

  if (mysql_real_connect(conn, host, user, pass, db, 3306, NULL, 0) == NULL) 
    finish_with_error(conn);

  //const char * stmt_str = "INSERT INTO tips(tips_id,msg) VALUES(NULL, ?)";
  const char * stmt_str = "INSERT INTO tips VALUES(NULL, ?)";
  MYSQL_BIND param[1];

  FILE *fp = fopen("../tips/general.tips","r");

  char * buf = fgets_alloc(fp); // read leading "##----"
  memset((void *) param, 0, sizeof(param));

  // set up tip parameter
  std::string tip      = "";
  char* my_msg = (char *)malloc(4096);
  unsigned long len;
  param[0].buffer_type   = MYSQL_TYPE_STRING;
  param[0].buffer_length = 4096;
  param[0].is_null       = 0;
  param[0].buffer        = my_msg;
  param[0].length        = &len;



  MYSQL_STMT* stmt = mysql_stmt_init(conn);
  if (mysql_stmt_prepare(stmt, stmt_str, strlen(stmt_str)) != 0)
    {
      print_stmt_error(stmt, "Could not prepare INSERT statement");
      return 1;
    }

  if (mysql_stmt_bind_param(stmt,param) != 0)
    {
      print_stmt_error(stmt, "Could not bind paramaters for INSERT");
      return 1;
    }



  int count = 0;

  while(1)  // loop over each tip
    {
      tip = "";
      while(1) // read in a single tip
        { 
          buf = fgets_alloc(fp); 
          if (buf == NULL)
            {
              printf("saved %d records\n",count);
              mysql_close(conn);
              return 0;
            }
          if (strcmp(buf,"##----\n") == 0)
            break;
          tip += buf;
        }

      strcpy(my_msg, tip.c_str());
      len = tip.size();
      count++;
      if (len > 2047)
        {
          fprintf(stderr,"msg %d too big: len: %ld\n", count, len);
          fprintf(stderr,"msg:%s\n", my_msg);
          return 1;
        }
      if (mysql_query(conn, "START TRANSACTION")) 
        finish_with_error(conn);

      if (mysql_stmt_execute(stmt) != 0)
        {
          print_stmt_error(stmt,"Could not execute statement!");
          return 1;
        }

      if (mysql_query(conn, "COMMIT")) 
        finish_with_error(conn);
    }
}



  






