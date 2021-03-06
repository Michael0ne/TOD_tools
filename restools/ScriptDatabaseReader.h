#pragma once
#include "GenericResourceReader.h"
#include <vector>

class ScriptDatabaseReader : public GenericResourceReader
{
private:
	unsigned int	m_TotalProperties = NULL;
	unsigned int	m_TotalCommands = NULL;
	std::vector<std::string> m_PropertiesList;
	std::vector<std::string> m_CommandsList;

public:
	ScriptDatabaseReader(const char* const filename);
	~ScriptDatabaseReader();

	virtual void	ReadInfo() override;
	virtual void	PrintInfo() const override;
	virtual void	DumpData() const override;
};