/*
    ����������, ������� ����� ��������� � ����
*/

#pragma once
#include <Urho3D/Urho3DAll.h>

class Config : public Object
{
    URHO3D_OBJECT(Config, Object);

private:
    // ���� ��� ���������� �������
    String GetConfigFileName();

    // ��������� ������ (���������� � ������������)
    void Load();

public:
    Config(Context* context);

    // ���������� ������� � ����
    void Save();

    // ���������� �� ��������� �������� ���
    int windowWidth_ = 800;
    int windowHeight_ = 600;
    bool fullscreen_ = false;
    bool borderless_ = false;
};
