/* Standard C++ includes */
#include <stdlib.h>
#include <time.h>
#include <sstream>
#include <iostream>

/*
  Include directly the different
  headers from cppconn/ and mysql_driver.h + mysql_util.h
  (and mysql_connection.h). This will reduce your build time!
*/
#include "mysql_connection.h"

#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>

using namespace std;

int main(void)
{

try {
  sql::Driver *driver;
  sql::Connection *con;
  sql::Statement *stmt;
  sql::ResultSet *res;
  ostringstream cmd;

  int numEntries = 0 ;

  /* Create a connection */
  driver = get_driver_instance();
  con = driver->connect("tcp://rios.tacc.utexas.edu:3306", "readerOfTips", "tipReader123");
  /* Connect to the MySQL test database */
  con->setSchema("HPCTips");

  stmt = con->createStatement();
  res = stmt->executeQuery("SELECT count(*) from tips");
  while (res->next()) {
    numEntries = strtol(res->getString(1).c_str(), (char **) NULL, 10);
    break;
  }
  delete res;
  delete stmt;

  
  srand(time(NULL));

  int idx = rand() % (numEntries - 1) + 1;


  cmd << "select msg from tips where tips_id = " << idx;

  stmt = con->createStatement();
  res = stmt->executeQuery(cmd.str());
  while (res->next()) {

    cout << "----------\n"
         << "Tip " << idx << ":\n"
         << "----------\n\n";

    cout << res->getString(1) << endl;
    break;
  }
  delete res;
  delete stmt;
  delete con;
  
  
  



} catch (sql::SQLException &e) {
  cout << "# ERR: SQLException in " << __FILE__;
  cout << "(" << __FUNCTION__ << ") on line " 
     << __LINE__ << endl;
  cout << "# ERR: " << e.what();
  cout << " (MySQL error code: " << e.getErrorCode();
  cout << ", SQLState: " << e.getSQLState() << " )" << endl;
}

cout << endl;

return EXIT_SUCCESS;
}
