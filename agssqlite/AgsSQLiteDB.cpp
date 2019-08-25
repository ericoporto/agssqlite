#include "AgsSQLiteDB.h"
#include <cstdio>

static int redir_callback(void *instanceAgsSQLiteDB, int argc, char **argv, char **azColName)
{
	AgsSQLiteDB* myAgsSQLiteDB = reinterpret_cast<AgsSQLiteDB*>(instanceAgsSQLiteDB);
	return myAgsSQLiteDB->callback(argc, argv, azColName);
}

AgsSQLiteDB::AgsSQLiteDB(char* path) {
	OpenStatus = sqlite3_open(path, &db);
	
	if (OpenStatus) {
		snprintf(OpenStatusText, 300, "Error: Can't open database: %s\n", sqlite3_errmsg(db));
	}
	else {
		snprintf(OpenStatusText, 300, "Opened database successfully\n");
	}
}

int AgsSQLiteDB::ExecuteQuery(char* query) {
	char* zErrMsg;

	if (QueryResult != NULL) {
		delete QueryResult;
	}

	QueryResult = (char*) "";

	QueryStatus = sqlite3_exec(db, query, redir_callback, this, &zErrMsg);
	if (QueryStatus != SQLITE_OK) {
		snprintf(QueryStatusText, 300, "SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
	}
	else {
		snprintf(QueryStatusText, 300, "Query executed successfully\n");
	}

	return QueryStatus;
}

int AgsSQLiteDB::IsOpen() {
	if (OpenStatus || db != NULL) {
		return false;
	}
	else {
		return true;
	}
}

void AgsSQLiteDB::Close() {

	if (OpenStatus || db != NULL) {
		sqlite3_close(db);
	}
	OpenStatus = true;

}

void AgsSQLiteDB::ClearQueryStatus() {
	QueryStatus = -1;
	delete QueryStatusText;
	QueryStatusText = (char*) "";
}

int AgsSQLiteDB::callback(int argc, char **argv, char **azColName) {
	if (QueryResult != NULL) {
		delete QueryResult;
	}
	
	int i;
	for (i = 0; i < argc; i++) {
		snprintf(QueryResult, 1000, "%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
	}
	snprintf(QueryResult, 1000, "\n");
	return 0;
}

AgsSQLiteDB::~AgsSQLiteDB(void)
{

}

// -------------

extern IAGSEngine* engine;

AgsSQLiteDBInterface AgsSQLiteDB_Interface;
AgsSQLiteDBReader AgsSQLiteDB_Reader;

const char* AgsSQLiteDBInterface::name = "AgsSQLiteDB";

//------------------------------------------------------------------------------

int AgsSQLiteDBInterface::Dispose(const char* address, bool force)
{
	delete ((AgsSQLiteDB*)address);
	return (1);
}

//------------------------------------------------------------------------------

int AgsSQLiteDBInterface::Serialize(const char* address, char* buffer, int bufsize)
{
	AgsSQLiteDB* arr = (AgsSQLiteDB*)address;
	char* ptr = buffer;

	
	return (ptr - buffer);
}

//------------------------------------------------------------------------------

void AgsSQLiteDBReader::Unserialize(int key, const char* serializedData, int dataSize)
{
	AgsSQLiteDB* arr = new AgsSQLiteDB((char*)"");

	const char* ptr = serializedData;

	engine->RegisterUnserializedObject(key, arr, &AgsSQLiteDB_Interface);
}

//..............................................................................
