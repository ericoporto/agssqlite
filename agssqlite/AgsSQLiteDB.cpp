#include "AgsSQLiteDB.h"
#include <cstdio>
#include <string.h>

static int redir_callback(void *instanceAgsSQLiteDB, int argc, char **argv, char **azColName)
{
	AgsSQLiteDB* myAgsSQLiteDB = reinterpret_cast<AgsSQLiteDB*>(instanceAgsSQLiteDB);
	return myAgsSQLiteDB->callback(argc, argv, azColName);
}

AgsSQLiteDB::AgsSQLiteDB(char* path) {
	OpenStatus = sqlite3_open(path, &db);
	
	if (OpenStatusText != NULL) {
		delete OpenStatusText;
	}

	if (OpenStatus) {
		int str_size = snprintf(NULL, 0, "Error: Can't open database: %s\n", sqlite3_errmsg(db));
		OpenStatusText = new char[str_size+1];
		snprintf(OpenStatusText, str_size, "Error: Can't open database: %s\n", sqlite3_errmsg(db));
	}
	else {
		int str_size = snprintf(NULL, 0, "Opened database successfully\n");
		OpenStatusText = new char[str_size+1];
		snprintf(OpenStatusText, str_size, "Opened database successfully\n");
	}

	if (Path != NULL) {
		delete Path;
	}

	int str_size = snprintf(NULL, 0, "%s", path);
	Path = new char[str_size + 1];
	snprintf(Path, str_size, "%s", path);
}

int AgsSQLiteDB::ExecuteQuery(char* query) {
	char* zErrMsg;

	if (QueryStatusText != NULL) {
		delete QueryStatusText;
	}

	QueryResult.clear();

	QueryStatus = sqlite3_exec(db, query, redir_callback, this, &zErrMsg);
	if (QueryStatus != SQLITE_OK) {
		int str_size = snprintf(NULL, 0, "SQL error: %s\n", zErrMsg);
		QueryStatusText = new char[str_size + 1];
		snprintf(QueryStatusText, str_size, "SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
	}
	else {
		int str_size = snprintf(NULL, 0, "Query executed successfully\n");
		QueryStatusText = new char[str_size + 1];
		snprintf(QueryStatusText, str_size, "Query executed successfully\n");
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
	int i;
	int str_size = 0;
	for (i = 0; i < argc; i++) {
		str_size += snprintf(NULL, 0, "%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
	}
	str_size += snprintf(NULL, 0, "\n");

	char* TempStr = new char[str_size + 1];

	char* cur = TempStr;
	const char* end = cur + str_size;

	for (i = 0; i < argc; i++) {
		cur += snprintf(cur, end-cur, "%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
	}
	cur += snprintf(cur, end-cur, "\n");

	QueryResult.append(TempStr);

	return 0;
}

AgsSQLiteDB::~AgsSQLiteDB(void)
{

}

// -------------

extern IAGSEngine* engine;

AgsSQLiteDBInterface AgsSQLiteDB_Interface;
AgsSQLiteDBReader AgsSQLiteDB_Reader;

const char* AgsSQLiteDBInterface::name = "AgsSQLite";

//------------------------------------------------------------------------------

int AgsSQLiteDBInterface::Dispose(const char* address, bool force)
{
	((AgsSQLiteDB*)address)->Close();
	delete ((AgsSQLiteDB*)address);
	return (1);
}

//------------------------------------------------------------------------------

int AgsSQLiteDBInterface::Serialize(const char* address, char* buffer, int bufsize)
{
	AgsSQLiteDB* agsSQLiteDB = (AgsSQLiteDB*)address;
	int path_length = strlen((*agsSQLiteDB).Path);
	
	memcpy(buffer, (*agsSQLiteDB).Path, path_length);
	return sizeof(path_length);
}

//------------------------------------------------------------------------------

void AgsSQLiteDBReader::Unserialize(int key, const char* serializedData, int dataSize)
{
	char* path = new char[dataSize];
	memcpy(path, serializedData, dataSize);

	AgsSQLiteDB* agsSQLiteDB = new AgsSQLiteDB((char*)path);

	engine->RegisterUnserializedObject(key, agsSQLiteDB, &AgsSQLiteDB_Interface);
}

//..............................................................................
