#include "Public.h"
#include "UI.h"

class CIniFile
{
public:
	CIniFile();
	~CIniFile();
	int GetRecord(char *path, INI_RECORD* record);
	int SetRrcord(char *path);

private:
	INI_RECORD *m_record;
};


