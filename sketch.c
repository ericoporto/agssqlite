#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h> 

enum SQLiteQueryStatus {
  eSQLiteQueryNotRun=0,
  eSQLiteQueryRunning=1,
  eSQLiteQueryFinished=2,
};

/// Opens a SQLite DB and returns error message
String AgsSQLite.Open(String db);

/// Returns true if connection is open
bool AgsSQLite.IsOpen();

/// Run a query on previously opened DB
String AgsSQLite.ExecuteQuery(String query);

/// Get the status of currently executed query
SQLiteQueryStatus AgsSQLite.GetQueryStatus();

/// Get the results from the previously executed query
String AgsSQLite.GetQueryResult();

/// Clear query results. They are also cleared when a new query is executed or connection is closed.
String AgsSQLite.ClearResultStatus();

/// Close DB connection. 
AgsSQLite.Close();

static int callback(void *NotUsed, int argc, char **argv, char **azColName) {
   int i;
   for(i = 0; i<argc; i++) {
      printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
   }
   printf("\n");
   return 0;
}

int main(int argc, char* argv[]) {
   sqlite3 *db;
   char *zErrMsg = 0;
   int rc;
   char *sql;

   /* Open database */
   rc = sqlite3_open("test.db", &db);
   
   if( rc ) {
      fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
      return(0);
   } else {
      fprintf(stderr, "Opened database successfully\n");
   }

   /* Create SQL statement */
   sql = "INSERT INTO COMPANY (ID,NAME,AGE,ADDRESS,SALARY) "  \
         "VALUES (1, 'Paul', 32, 'California', 20000.00 ); " \
         "INSERT INTO COMPANY (ID,NAME,AGE,ADDRESS,SALARY) "  \
         "VALUES (2, 'Allen', 25, 'Texas', 15000.00 ); "     \
         "INSERT INTO COMPANY (ID,NAME,AGE,ADDRESS,SALARY)" \
         "VALUES (3, 'Teddy', 23, 'Norway', 20000.00 );" \
         "INSERT INTO COMPANY (ID,NAME,AGE,ADDRESS,SALARY)" \
         "VALUES (4, 'Mark', 25, 'Rich-Mond ', 65000.00 );";

   /* Execute SQL statement */
   rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);
   
   if( rc != SQLITE_OK ){
      fprintf(stderr, "SQL error: %s\n", zErrMsg);
      sqlite3_free(zErrMsg);
   } else {
      fprintf(stdout, "Records created successfully\n");
   }
   sqlite3_close(db);
   return 0;
}
