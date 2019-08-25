# agssqlite
AGS Sqlite plugin

## Debugging a sqlite database

There a many different softwares available to visualize a sqlite database. 

Right now, [Sqlite Browser](https://github.com/sqlitebrowser/sqlitebrowser) is a nice one, and you can look the database while the game is running on AGS.

## AgsSQLite Script API

**`AgsSQLite* AgsSQLite.Open(String db_path)`**

Use this method to create a Sqlite Database object that you can interact with. If the database doesn't exist, it's created. 

The resulting object will have all the methods below available to it and it will be connected with this database object.

Example:

```AGS Script
// creates a 'test.db' where the dll is located.
AgsSQLite* db = AgsSQLite.Open("test.db");
```


**`String GetDBPath()`**

Returns the path passed when DB was opened above.


**`bool IsOpen()`**

Returns true if DB is opened. DB can only be closed if you have called Close before.


**`SQLiteQueryStatus ExecuteQuery(String query)`**

Execute a query with sqlite and returns the status.

Example:

```AGS Script
String sql = "";

sql = sql.Append("CREATE TABLE COMPANY(");
sql = sql.Append( "ID INT PRIMARY KEY     NOT NULL,");
sql = sql.Append("NAME           TEXT    NOT NULL,");
sql = sql.Append("AGE            INT     NOT NULL,");
sql = sql.Append("ADDRESS        CHAR(50),");
sql = sql.Append("SALARY         REAL );");

SQLiteQueryStatus rc = db.ExecuteQuery(sql);
```


**`SQLiteQueryStatus GetQueryStatus()`**

Get the SQLiteQueryStatus of last executed query.


**`String GetQueryStatusText()`**

Gets the query status as a text. Useful to debug and figure out why a query is failing.


**`String GetQueryResult()`**

Gets the query result. 


**`void ClearResultStatus()`**

Clear query results. They are also cleared when a new query is executed or connection is closed.


**`void Close()`**

Close connection with a database. 
