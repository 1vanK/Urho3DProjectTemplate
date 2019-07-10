#include <rapidjson/document.h>
#include <rapidjson/prettywriter.h>

#include "Global.h"

// ������ � ���� ��������, ����� �������� ��������
static const rapidjson::Value::StringRefType STR_WINDOW_WIDTH = "WindowWidth";
static const rapidjson::Value::StringRefType STR_WINDOW_HEIGHT = "WindowHeight";
static const rapidjson::Value::StringRefType STR_FULLSCREEN = "Fullscreen";
static const rapidjson::Value::StringRefType STR_BORDERLESS = "Borderless";

Config::Config(Context* context) : Object(context)
{
    Load();
}

String Config::GetConfigFileName()
{
    // InitGlobal() ��� �� ������, ������� fileSystem ������������ ������
    FileSystem* fs = GetSubsystem<FileSystem>();

    return fs->GetAppPreferencesDir(COMPANY_NAME, APP_NAME) + "Config.json";
}

// ������ �������� ������������� ���� �� �������, ���� ��������
template<typename Type> void TryLoadValue(rapidjson::Document& document, const char* name, Type& out)
{
    rapidjson::Value::MemberIterator member = document.FindMember(name);
    if (member != document.MemberEnd())
    {
        rapidjson::Value& value = member->value;
        if (value.Is<Type>())
            out = value.Get<Type>();
    }
}

void Config::Load()
{
    // InitGlobal() ��� �� ������, ������� fileSystem ������������ ������
    FileSystem* fs = GetSubsystem<FileSystem>();

    String fileName = GetConfigFileName();

    // ��������� ������, ���� �� ����
    if (fs->FileExists(fileName))
    {
        File file(context_, fileName, FILE_READ);
        unsigned fileSize = file.GetSize();
        SharedArrayPtr<char> buf(new char[fileSize + 1]);
        if (file.Read(buf, fileSize) != fileSize)
            return;
        buf[fileSize] = '\0';

        rapidjson::Document document;
        const unsigned flags = rapidjson::kParseCommentsFlag | rapidjson::kParseTrailingCommasFlag;
        if (document.Parse<flags>(buf).HasParseError())
        {
            URHO3D_LOGERROR("Could not parse JSON data from " + fileName);
            return;
        }

        TryLoadValue(document, STR_WINDOW_WIDTH, windowWidth_);
        TryLoadValue(document, STR_WINDOW_HEIGHT, windowHeight_);
        TryLoadValue(document, STR_FULLSCREEN, fullscreen_);
        TryLoadValue(document, STR_BORDERLESS, borderless_);
    }
}

void Config::Save()
{
    rapidjson::Document document;
    document.SetObject(); // ���������� � ��������� ��� Null, ������ ��� �� Object
    rapidjson::MemoryPoolAllocator<>& allocator = document.GetAllocator();

    document.AddMember(STR_WINDOW_WIDTH, windowWidth_, allocator);
    document.AddMember(STR_WINDOW_HEIGHT, windowHeight_, allocator);
    document.AddMember(STR_FULLSCREEN, fullscreen_, allocator);
    document.AddMember(STR_BORDERLESS, borderless_, allocator);

    rapidjson::StringBuffer buffer;
    rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(buffer);
    writer.SetIndent(' ', 4);
    document.Accept(writer);

    File file(context_, GetConfigFileName(), FILE_WRITE);
    unsigned size = (unsigned)buffer.GetSize();
    file.Write(buffer.GetString(), size);
}
