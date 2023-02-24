#include "ScriptDatabaseReader.h"

ScriptDatabaseReader::ScriptDatabaseReader(const char* const filename)
{
    m_ResourceExtension = ".bin";
    m_ResourceName = filename;
    m_ResourceName += m_ResourceExtension;

    m_PropertiesList.reserve(5280);
    m_CommandsList.reserve(1400);
}

ScriptDatabaseReader::~ScriptDatabaseReader()
{
    m_PropertiesList.clear();
    m_CommandsList.clear();
}

void ScriptDatabaseReader::ReadInfo()
{
    if (!m_FilePtr ||
        m_LastErrorCode)
        return;

    fread(&m_TotalProperties, sizeof(m_TotalProperties), 1, m_FilePtr);

    //	NOTE: this shouldn't happen.
    if (!m_TotalProperties)
        return;

    for (unsigned int i = 0; i < m_TotalProperties; i++)
    {
        unsigned int slen = NULL;
        char* sbuf = nullptr;

        fread(&slen, sizeof(slen), 1, m_FilePtr);

        if (slen == NULL)
            continue;

        sbuf = new char[slen];

        fread(sbuf, slen, 1, m_FilePtr);
        m_PropertiesList.push_back(sbuf);

        delete[] sbuf;
    }

    fread(&m_TotalCommands, sizeof(m_TotalCommands), 1, m_FilePtr);

    //	NOTE: this shouldn't happen.
    if (!m_TotalCommands)
        return;

    for (unsigned int i = 0; i < m_TotalCommands; i++)
    {
        unsigned int slen = NULL;
        char* sbuf = nullptr;

        fread(&slen, sizeof(slen), 1, m_FilePtr);

        if (slen == NULL)
            continue;

        sbuf = new char[slen];

        fread(sbuf, slen, 1, m_FilePtr);
        m_CommandsList.push_back(sbuf);

        delete[] sbuf;
    }
}

void ScriptDatabaseReader::PrintInfo() const
{
    printf("\tTotal properties:\t%u\n", m_TotalProperties);
    printf("\tTotal commands:\t%u\n", m_TotalCommands);
    printf("\tRead actual properties:\t%u\n", m_PropertiesList.size());
    printf("\tRead actual commands:\t%u\n", m_CommandsList.size());

    if (m_ShouldDumpData)
        DumpData();
}

void ScriptDatabaseReader::DumpData() const
{
    FILE* outfile = fopen("properties.dump", "wb");
    
    for (unsigned int i = 0; i < m_TotalProperties; i++)
    {
        fwrite(m_PropertiesList[i].c_str(), m_PropertiesList[i].size(), 1, outfile);
        fwrite("\n", 1, 1, outfile);
    }

    fclose(outfile);

    printf("Saved dump: %s(properties) -> %s\n", m_ResourceName.c_str(), "properties.dump");

    outfile = fopen("commands.dump", "wb");

    for (unsigned int i = 0; i < m_TotalCommands; i++)
    {
        fwrite(m_CommandsList[i].c_str(), m_CommandsList[i].size(), 1, outfile);
        fwrite("\n", 1, 1, outfile);
    }

    fclose(outfile);

    printf("Saved dump: %s(commands) -> %s\n", m_ResourceName.c_str(), "commands.dump");
}