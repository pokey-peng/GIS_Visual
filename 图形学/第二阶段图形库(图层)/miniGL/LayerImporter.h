#pragma once

struct Layer;
//ͼ�㵼����
class LayerImporter
{
public:
	static Layer* importShpLayer( const char* path, const char* labelFieldName = "" );//����shp�ļ�������ͼ����󣬵���ʧ���򷵻�NULL
};

