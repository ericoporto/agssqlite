#pragma once

#ifndef _AGSSQLITEDB_H
#define _AGSSQLITEDB_H

#include "plugin/agsplugin.h"
#include "sqlite3.h"

class AgsSQLiteDB {
public:
	sqlite3 *db;
	AgsSQLiteDB(char* path);
	~AgsSQLiteDB(void);
	char* ExecuteQuery(char* query);
	int IsOpen();
	void Close();
	void ClearResultStatus();
	int QueryStatus;
	char* Path;
	char* QueryResult;
	int callback(void *NotUsed, int argc, char **argv, char **azColName);
};

//------------------------------------------------------------------------------
// AGS interface instances

class AgsSQLiteDBInterface : public IAGSScriptManagedObject
{
public:
	static const char* name;

	AgsSQLiteDBInterface() {};

	virtual int Dispose(const char* address, bool force);
	virtual const char* GetType() { return (name); }
	virtual int Serialize(const char* address, char* buffer, int bufsize);

};

//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

class AgsSQLiteDBReader : public IAGSManagedObjectReader
{
public:

	AgsSQLiteDBReader() {}

	virtual void Unserialize(int key, const char* serializedData, int dataSize);

};

//------------------------------------------------------------------------------

extern AgsSQLiteDBInterface AgsSQLiteDB_Interface;
extern AgsSQLiteDBReader AgsSQLiteDB_Reader;

//------------------------------------------------------------------------------

#endif /* _AGSSQLITEDB_H */