#pragma once

#include "Graphic.h"

class DialogHelper
{
public:
	/**	���ļ��Ի���ѡ�񵥸��ļ�
	@param  intialDir ��ʼĿ¼����Ϊ0��""
	@param  extension  �ļ���׺
	@param  selectedFile  ѡ����ļ�
	@param  bufSize  selectedFile�����С
	@return ����û�ȡ����ر�Open�Ի���������֣�����ֵΪ0�������0
	*/
	static int selectSingleFile( const char* intialDir,const char* extension, char* selectedFile, int bufSize );

	/**	����ɫѡ��Ի���ѡ����ɫ
	@param  selectedColor �û�ѡ�����ɫ
	@param  initialColor  ��ʼ��ɫ
	@return ����û�ȡ��������ֵΪfalse������Ϊtrue
	*/
	static bool selectColor( Color& selectedColor , Color initialColor = BLACK );

	/**	������ѡ��Ի���ѡ������
	@param  selectedFontName �û�ѡ�����������
	@param  selectedFontSize  �û�ѡ��������С
	@return ����û�ȡ��������ֵΪfalse������Ϊtrue
	*/
	static bool selectFont( char* selectedFontName, int& selectedFontSize );
};

