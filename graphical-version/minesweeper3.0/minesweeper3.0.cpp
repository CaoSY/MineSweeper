/*
gamename:minesweeper
creater:Cao Shuyang
release date:2014 1 1
version number: 3.0
*/
#include "stdafx.h"
#include<iostream>
#include<graphics.h>
#include<conio.h>
#include<Windows.h>
#include<mmsystem.h>
#include<cstdlib>
#include<string.h>
#include<ctime>
#include<iomanip>
#include<fstream>
#pragma comment(lib,"Winmm.lib")
using namespace std;
////////////////////////////////////////////////////////////////////ȫ�ֱ���
TCHAR user_name[20]=L"admin";
bool new_flag;						//�ж��Ƿ������û�
int row,column,mine_number;			//�У��У����ܵ�ĸ���
bool field[52][52]={0};
int scaned[52][52]={0},	mine_number_map[50][50]={0};
bool replaychoice=0;			//���̱��
bool first_click=1;				//��һ�α�����
float width,height;				//������	
int mouse_x,mouse_y;			//���еĸ��ӵ�����
bool win_or_lose_sound;			//������Ч
IMAGE number1,number2,number3,number4,number5,number6,number7,number8;		//��������Ƭ�İ˸�����
IMAGE grid_picture,mark_picture,hole;			//����ͼƬ�����ͼƬ����
IMAGE picture_hidden;							//�������
int picture_number;				//���ͼƬ�ı��
HCURSOR hcur;					//�����
HWND hwnd;						//���ھ��
clock_t time_mark;				//��ʱ��
int duration=0;					//����ʱ��
///////////////////////////////////////////////////////////////////�����ú���
void begineffect();		//��ʼЧ��
void loadingeffect();//��Ϊ���ֿ�ͷ�д�Լ4��Ŀհף�����Ҫ��һ��loading
int firstmenu();    //���á���0����������1
void new_user_register();//���û�ע��
void welcome_back();     //���û���½
void main_menu();		//���˵�
void seeyoueffect();			//����Ч��
void game();			//��Ϸ����
int loadrecord();		//����
bool replayquestion();	//����ѯ��
void scan_map(const int x,const int y);			//չ����ͼ�ĵݹ麯��
void initialize_field();					//��ʼ����ͼ
void showfield();					//�����ͼ
bool win();							//�ж��Ƿ��ʤ
int suspend();						//��ͣ
void storefield();					//�浵
void fail_effect();					//ʧ��Ч��
void win_effect();					//�ɹ�Ч��
void attribute();					//��Ϸ����
int newgame();						//�Ѷ�ѡ��
bool selfdefine();					//�Զ����Ѷ�
void correct_memory();				//����ȷ���Ƿ��Ѿ��й���¼
void picturechoice(int n);			//����ѡ����ͼƬ
int _tmain(int argc, _TCHAR* argv[])
{
//��������	
	mciSendString(TEXT("open res\\����\\���ֻ���Ч.mp3 alias beginsong"),NULL,0,NULL);
	mciSendString(TEXT("open res\\����\\����ʱ����ٰ�.mp3 alias menubackground"),NULL,0,NULL);
	mciSendString(TEXT("open res\\����\\˺ֽ��Ч.mp3 alias lose_sound_effect"),NULL,0,NULL);
	mciSendString(TEXT("open res\\����\\ĪʧĪ��.mp3 alias lose_sound"),NULL,0,NULL);
	mciSendString(TEXT("open res\\����\\��ң̾.mp3 alias win_sound"),NULL,0,NULL);
//��ʼ
	initgraph(970,630);
	hcur = LoadCursor(NULL, IDC_ARROW); // ����ϵͳԤ�õ������ʽ
	hwnd = GetHWnd(); // ��ȡ��ͼ���ھ��
	SetClassLong(hwnd, GCL_HCURSOR, (long)hcur); // ���ô�����������ʽ
	begineffect();
	cleardevice();
	mciSendString(TEXT("play menubackground repeat"),NULL,0,NULL);
	loadingeffect();
	bool user_flag=firstmenu();					//����û���
	switch (user_flag)
	{
		case 0: new_flag=1;new_user_register(); break;
		case 1: new_flag=0;welcome_back(); break;
		default:closegraph();cerr<<"�����������������"; break;
	}
	correct_memory();
	for(bool play_flag=1;play_flag;)
	{
		main_menu();
		game();
		new_flag=0;
		play_flag=replayquestion();
		if(play_flag)
		{
			mciSendString(TEXT("seek menubackground to start"),NULL,0,NULL);
			mciSendString(TEXT("play menubackground repeat"),NULL,0,NULL);
		}
	}
	if(win_or_lose_sound)
		mciSendString(TEXT("play menubackground repeat"),NULL,0,NULL);
	else
		mciSendString(TEXT("play lose_sound repeat"),NULL,0,NULL);
	seeyoueffect();
	mciSendString(TEXT("stop menubackground"),NULL,0,NULL);
 	return 0;
}
void correct_memory()
{
	ifstream infile(user_name);
	if ((infile.is_open()==0)&&new_flag==0)
	{
		new_flag=1;
		loadimage(NULL,L"res\\ͼƬ\\��ʼ����.jpg",970,630,false);
		TCHAR pity[4][30]={_T("���������ĺ�����"),_T("ֱ�Ѵ˵�������"),_T("����δ�������˵�"),_T("���μ��棬�����һ�")};
		LOGFONT choice;
		gettextstyle(&choice);                     // ��ȡ��ǰ��������
		choice.lfHeight = 48;                      // ��������߶�Ϊ 48
		_tcscpy_s(choice.lfFaceName, _T("����"));    // ��������Ϊ�����塱(�߰汾 VC �Ƽ�ʹ�� _tcscpy_s ����)
		choice.lfQuality = ANTIALIASED_QUALITY;    // �������Ч��Ϊ�����  
		settextstyle(&choice);  // 
		setbkmode(TRANSPARENT);
		settextcolor(BLACK);
		Sleep(100);
		for(int i=0;i<4;i++)
		{
			for (int j = 0;pity[i][j]!='\0'; j++)
			{
				outtextxy(230+48*j,150+i*80,pity[i][j]);
				Sleep(110);
			}
		}
		Sleep(500);
	}else if (infile.is_open()==1&&new_flag==1)
	{
		new_flag=0;
		loadimage(NULL,L"res\\ͼƬ\\��ʼ����.jpg",970,630,false);
		TCHAR pity[4][30]={_T("���������ĺ�����"),_T("ֱ���������˵�"),_T("��������������Զӭ"),_T("������������")};
		LOGFONT choice;
		gettextstyle(&choice);                     // ��ȡ��ǰ��������
		choice.lfHeight = 48;                      // ��������߶�Ϊ 48
		_tcscpy_s(choice.lfFaceName, _T("����"));    // ��������Ϊ�����塱(�߰汾 VC �Ƽ�ʹ�� _tcscpy_s ����)
		choice.lfQuality = ANTIALIASED_QUALITY;    // �������Ч��Ϊ�����  
		settextstyle(&choice);  // 
		setbkmode(TRANSPARENT);
		settextcolor(BLACK);
		Sleep(100);
		for(int i=0;i<4;i++)
		{
			for (int j = 0;pity[i][j]!='\0'; j++)
			{
				outtextxy(230+48*j,150+i*80,pity[i][j]);
				Sleep(110);
			}
		}
		Sleep(500);
	}
}
bool replayquestion()
{
	if(win_or_lose_sound==0)
		mciSendString(TEXT("play lose_sound repeat"),NULL,0,NULL);
	else if(win_or_lose_sound==1)
		mciSendString(TEXT("play win_sound repeat"),NULL,0,NULL);
	IMAGE wholescreen,lineimg[970];
	if (win_or_lose_sound==0)
	{
		loadimage(&wholescreen,L"res\\ͼƬ\\��ʼ����.jpg",970,630,false);
		SetWorkingImage(&wholescreen);
		for(int i=0;i<970;i++)
			getimage(&lineimg[i],i,0,1,630);
		SetWorkingImage(NULL);
		for(int i=0;i<970;i++)
		{
			putimage(i,0,&lineimg[i],SRCCOPY);
			Sleep(1);
		}
	}else if(win_or_lose_sound==1)
	{
		Sleep(200);
	}else
	{
		loadimage(NULL,L"res\\ͼƬ\\��ʼ����.jpg",970,630,false);
	}
	TCHAR pity[4][30]={_T("�����ѻ�һ��"),_T("��֪�����˺��������"),_T("�뿪Ƨ�������ķ�"),_T("�ֻ�������һ����������")};
	LOGFONT choice;
	gettextstyle(&choice);                     // ��ȡ��ǰ��������
	choice.lfHeight = 48;                      // ��������߶�Ϊ 48
	_tcscpy_s(choice.lfFaceName, _T("����"));    // ��������Ϊ�����塱(�߰汾 VC �Ƽ�ʹ�� _tcscpy_s ����)
	choice.lfQuality = ANTIALIASED_QUALITY;    // �������Ч��Ϊ�����  
	settextstyle(&choice);  // 
	setbkmode(TRANSPARENT);
	settextcolor(BLACK);
	Sleep(100);
	for(int i=0;i<4;i++)
	{
		for (int j = 0;pity[i][j]!='\0'; j++)
		{
			outtextxy(230+48*j,150+i*80,pity[i][j]);
			Sleep(110);
		}
	}
	MOUSEMSG m;
	FlushMouseMsgBuffer();
	while (true)
	{
		m=GetMouseMsg();
		if((230<m.x&&m.x<614&&310<m.y&&m.y<358)||(230<m.x&&m.x<758&&390<m.y&&m.y<438))
		{
				hcur = LoadCursor(NULL, IDC_HAND); // ����ϵͳԤ�õ������ʽ
				SetClassLong(hwnd, GCL_HCURSOR, (long)hcur); // ���ô�����������ʽ
		}else
		{
				hcur = LoadCursor(NULL, IDC_ARROW); // ����ϵͳԤ�õ������ʽ
				SetClassLong(hwnd, GCL_HCURSOR, (long)hcur); // ���ô�����������ʽ
		}
		switch (m.uMsg)
		{
			case WM_LBUTTONDOWN:if(win_or_lose_sound==0)
									mciSendString(TEXT("pause lose_sound"),NULL,0,NULL);
								else if(win_or_lose_sound==1)
									mciSendString(TEXT("stop win_sound"),NULL,0,NULL);
								if(230<m.x&&m.x<614&&310<m.y&&m.y<358)
								{
									hcur = LoadCursor(NULL, IDC_ARROW); // ����ϵͳԤ�õ������ʽ
									SetClassLong(hwnd, GCL_HCURSOR, (long)hcur); // ���ô�����������ʽ
									return 0;
								}
								else if(230<m.x&&m.x<758&&390<m.y&&m.y<438)
								{
									hcur = LoadCursor(NULL, IDC_ARROW); // ����ϵͳԤ�õ������ʽ
									SetClassLong(hwnd, GCL_HCURSOR, (long)hcur); // ���ô�����������ʽ
									return 1;
								}
			default:break;
		}
	}
}
//////////////////////////////////////////////��Ϸ����
void game()
{
	if(replaychoice)							//�������ʼ������
	{
		new_flag=0;
		first_click=0;
		loadrecord();
		picturechoice(picture_number);
	}else
	{
		first_click=1;
		memset(scaned,0,sizeof(scaned));
		memset(field,0,sizeof(field));
		memset(mine_number_map,0,sizeof(mine_number_map));
		for(int i=0;i<column+2;i++)				//��ʼ��scaned�ı߽�Ϊ1�����Ѵ�
		{
			scaned[0][i]=1;
			scaned[row+1][i]=1;
		}
		for(int i=0;i<row+2;i++)
		{
			scaned[i][0]=1;
			scaned[i][column+1]=1;
		}
		srand(time(NULL));
		picture_number=rand()%20;
		picturechoice(picture_number);
		duration=0;
	}
	width=970.0/column;
	height=630.0/row;
	loadimage(&number1,L"res\\ͼƬ\\1.jpg",width,height,true);
	loadimage(&number2,L"res\\ͼƬ\\2.jpg",width,height,true);
	loadimage(&number3,L"res\\ͼƬ\\3.jpg",width,height,true);
	loadimage(&number4,L"res\\ͼƬ\\4.jpg",width,height,true);
	loadimage(&number5,L"res\\ͼƬ\\5.jpg",width,height,true);
	loadimage(&number6,L"res\\ͼƬ\\6.jpg",width,height,true);
	loadimage(&number7,L"res\\ͼƬ\\7.jpg",width,height,true);
	loadimage(&number8,L"res\\ͼƬ\\8.jpg",width,height,true);
	loadimage(&grid_picture,L"res\\ͼƬ\\�໨��.jpg",width,height,true);
	loadimage(&mark_picture,L"res\\ͼƬ\\÷��.jpg",width,height,true);
	loadimage(&hole,L"res\\ͼƬ\\��.gif",width,height,true);
	time_mark=clock();				//��ʱ��
//	duration=0;
	showfield();
	MOUSEMSG m;
	FlushMouseMsgBuffer();
	for(bool game_flag=1;game_flag;)
	{
		m=GetMouseMsg();
		switch(m.uMsg)
		{
			case WM_LBUTTONDOWN:mouse_x=m.y/height;
								mouse_y=m.x/width;
								if(first_click)
								{
									scaned[mouse_x+1][mouse_y+1]=1;
									initialize_field();
									scaned[mouse_x+1][mouse_y+1]=0;
									game_flag=win();
									if(game_flag)
										showfield();
									first_click=0;
								}else
								{
									game_flag=win();
									if(game_flag)
										showfield();
								}
								break;
			case WM_RBUTTONDOWN:mouse_x=m.y/height;
								mouse_y=m.x/width;
								if (scaned[mouse_x+1][mouse_y+1]==0)
								{
									scaned[mouse_x+1][mouse_y+1]=2;
								}else if (scaned[mouse_x+1][mouse_y+1]==2)
								{
									scaned[mouse_x+1][mouse_y+1]=0;
								}
								showfield();
								break;
			case WM_RBUTTONDBLCLK:duration+=(clock()-time_mark)/1000;
								game_flag=suspend();
								time_mark=clock();
								break;
			default:break;
		}
	}
}
void picturechoice(int n)
{
	switch (n)
	{
		case  0:loadimage(&picture_hidden,L"res\\�������\\00.jpg",970,630,false);break;
		case  1:loadimage(&picture_hidden,L"res\\�������\\01.jpg",970,630,false);break;
		case  2:loadimage(&picture_hidden,L"res\\�������\\02.jpg",970,630,false);break;
		case  3:loadimage(&picture_hidden,L"res\\�������\\03.jpg",970,630,false);break;
		case  4:loadimage(&picture_hidden,L"res\\�������\\04.jpg",970,630,false);break;
		case  5:loadimage(&picture_hidden,L"res\\�������\\05.jpg",970,630,false);break;
		case  6:loadimage(&picture_hidden,L"res\\�������\\06.jpg",970,630,false);break;
		case  7:loadimage(&picture_hidden,L"res\\�������\\07.jpg",970,630,false);break;
		case  8:loadimage(&picture_hidden,L"res\\�������\\08.jpg",970,630,false);break;
		case  9:loadimage(&picture_hidden,L"res\\�������\\09.jpg",970,630,false);break;
		case 10:loadimage(&picture_hidden,L"res\\�������\\10.jpg",970,630,false);break;
		case 11:loadimage(&picture_hidden,L"res\\�������\\11.jpg",970,630,false);break;
		case 12:loadimage(&picture_hidden,L"res\\�������\\12.jpg",970,630,false);break;
		case 13:loadimage(&picture_hidden,L"res\\�������\\13.jpg",970,630,false);break;
		case 14:loadimage(&picture_hidden,L"res\\�������\\14.jpg",970,630,false);break;
		case 15:loadimage(&picture_hidden,L"res\\�������\\15.jpg",970,630,false);break;
		case 16:loadimage(&picture_hidden,L"res\\�������\\16.jpg",970,630,false);break;
		case 17:loadimage(&picture_hidden,L"res\\�������\\17.jpg",970,630,false);break;
		case 18:loadimage(&picture_hidden,L"res\\�������\\18.jpg",970,630,false);break;
		case 19:loadimage(&picture_hidden,L"res\\�������\\19.jpg",970,630,false);break;
		default:break;
	}
}
int suspend()
{
	IMAGE wholescreen;
	getimage(&wholescreen,0,0,970,630);
	DWORD *buf=GetImageBuffer(&wholescreen);
	for(int i=0;i<611100;i++)
		buf[i]=RGBtoGRAY(buf[i]);
	IMAGE choicehead(50,50),choiceframe(210,60);
	LOGFONT choice;
	gettextstyle(&choice);                     // ��ȡ��ǰ��������
	choice.lfHeight = 48;                      // ��������߶�Ϊ 48
	_tcscpy_s(choice.lfFaceName, _T("������κ"));    // ��������Ϊ��������κ��(�߰汾 VC �Ƽ�ʹ�� _tcscpy_s ����)
	choice.lfQuality = ANTIALIASED_QUALITY;    // �������Ч��Ϊ�����  
	settextstyle(&choice);  // 
	setbkmode(TRANSPARENT);
	settextcolor(HSLtoRGB(120,1,0.5));
	MOUSEMSG m;
	char itoatime[30]={0};
	sprintf_s(itoatime,"%d",duration);
	TCHAR ctottime[30]={0};
	int i=0;
	for(;itoatime[i]!='\0';i++)
		ctottime[i]=itoatime[i];
	putimage(0,0,&wholescreen,SRCCOPY);
	outtextxy(200,200,L"δ����ѣ��ݴ�˻�");
	outtextxy(200,300,L"���");
	outtextxy(200,400,L"�����ѷ���");
	outtextxy(440,400,ctottime);
	outtextxy(545,400,L"��");
	FlushMouseMsgBuffer();
	while (true)
	{
		m=GetMouseMsg();
		if((200<m.x&&m.x<632&&200<m.y&&m.y<250)||(200<m.x&&m.x<<296&&300<m.y&&m.y<350))
		{
			hcur = LoadCursor(NULL, IDC_HAND); // ����ϵͳԤ�õ������ʽ
			SetClassLong(hwnd, GCL_HCURSOR, (long)hcur); // ���ô�����������ʽ
		}else
		{
			hcur = LoadCursor(NULL, IDC_ARROW); // ����ϵͳԤ�õ������ʽ
			SetClassLong(hwnd, GCL_HCURSOR, (long)hcur); // ���ô�����������ʽ
		}
		switch (m.uMsg)
		{
			case WM_LBUTTONDOWN:if(200<m.x&&m.x<632&&200<m.y&&m.y<250)
								{
									storefield();
									showfield();
									hcur = LoadCursor(NULL, IDC_ARROW); // ����ϵͳԤ�õ������ʽ
									SetClassLong(hwnd, GCL_HCURSOR, (long)hcur); // ���ô�����������ʽ
									return 1;
								}else if (200<m.x&&m.x<<296&&300<m.y&&m.y<350)
								{
									putimage(0,0,&wholescreen,SRCCOPY);
									outtextxy(200,200,L"�����ڴ˸��");
									outtextxy(200,300,L"������������");
									outtextxy(200,400,L"�ֻ�ʹ˳���");
									FlushMouseMsgBuffer();
									while(true)
									{
										m=GetMouseMsg();
										if((200<m.x&&m.x<488&&300<m.y&&m.y<350)||(200<m.x&&m.x<488&&400<m.y&&m.y<450))
										{
											hcur = LoadCursor(NULL, IDC_HAND); // ����ϵͳԤ�õ������ʽ
											SetClassLong(hwnd, GCL_HCURSOR, (long)hcur); // ���ô�����������ʽ
										}else
										{
											hcur = LoadCursor(NULL, IDC_ARROW); // ����ϵͳԤ�õ������ʽ
											SetClassLong(hwnd, GCL_HCURSOR, (long)hcur); // ���ô�����������ʽ
										}
										if(m.uMsg==WM_LBUTTONDOWN)
										{
											if (200<m.x&&m.x<488&&300<m.y&&m.y<350)
											{
												storefield();
												hcur = LoadCursor(NULL, IDC_ARROW); // ����ϵͳԤ�õ������ʽ
												SetClassLong(hwnd, GCL_HCURSOR, (long)hcur); // ���ô�����������ʽ
												return 0;
											}else if(200<m.x&&m.x<488&&400<m.y&&m.y<450)
											{
												hcur = LoadCursor(NULL, IDC_ARROW); // ����ϵͳԤ�õ������ʽ
												SetClassLong(hwnd, GCL_HCURSOR, (long)hcur); // ���ô�����������ʽ
												return 0;
											}
										}
									}
								}
								break;
			case WM_RBUTTONDBLCLK:showfield();
								hcur = LoadCursor(NULL, IDC_ARROW); // ����ϵͳԤ�õ������ʽ
								SetClassLong(hwnd, GCL_HCURSOR, (long)hcur); // ���ô�����������ʽ
								return 1 ;break;
			default:break;
		}
	}
}
void storefield()
{
	ofstream outfile(user_name);;
	outfile<<row<<" "<<column<<" "<<mine_number<<" "<<first_click<<" "<<picture_number<<" "<<duration<<endl;
	outfile<<endl;
	for(int i=0;i<52;i++)						//��field
	{
		for (int j = 0; j < 52; j++)
		{
			outfile<<field[i][j]<<" ";
		}
		outfile<<endl;
	}
	outfile<<endl;
	for (int i = 0; i < 52; i++)				//��scaned
	{
		for (int j = 0; j < 52; j++)
		{
			outfile<<scaned[i][j]<<" ";
		}
		outfile<<endl;
	}
	outfile<<endl;
	for (int i = 0; i < 50; i++)				//��mine_number_map
	{
		for (int j = 0; j < 50; j++)
		{
			outfile<<mine_number_map[i][j]<<" ";
		}
		outfile<<endl;
	}
	outfile.close();
}
bool win()
{
	if(field[mouse_x+1][mouse_y+1])
	{
		fail_effect();
		return 0;
	}else
	{
		scan_map(mouse_x+1,mouse_y+1);
		int blackbox=0;
		for(int i=1;i<row+1;i++)
			for(int j=1;j<column+1;j++)
			{
				if(scaned[i][j]!=1)
				blackbox++;
			}
			if(blackbox==mine_number)
			{	
				showfield();
				win_effect();
				return 0;
			}
	}
	return 1;
}
void win_effect()
{
	mciSendString(TEXT("pause menubackground"),NULL,0,NULL);
	mciSendString(TEXT("play win_sound repeat"),NULL,0,NULL);
	TCHAR pity[3][30]={_T("�����������ֵ�һ�ѻ�"),_T("����������������"),_T("�������")};
	LOGFONT choice;
	gettextstyle(&choice);                     // ��ȡ��ǰ��������
	choice.lfHeight = 48;                      // ��������߶�Ϊ 48
	_tcscpy_s(choice.lfFaceName, _T("����"));    // ��������Ϊ�����塱(�߰汾 VC �Ƽ�ʹ�� _tcscpy_s ����)
	choice.lfQuality = ANTIALIASED_QUALITY;    // �������Ч��Ϊ�����  
	settextstyle(&choice);  // 
	setbkmode(TRANSPARENT);
	settextcolor(BLACK);
	Sleep(100);
	for(int i=0;i<3;i++)
	{
		for (int j = 0;pity[i][j]!='\0'; j++)
		{
			outtextxy(230+48*j,170+i*70,pity[i][j]);
			Sleep(110);
		}
	}
	IMAGE lineimg[970],left,right;
	SetWorkingImage(&picture_hidden);
	getimage(&left,0,0,485,630);
	getimage(&right,485,0,485,630);
	for(int i=0;i<970;i++)
		getimage(&lineimg[i],i,0,1,630);
	SetWorkingImage(NULL);
	for(int i=0;i<970;i++)
	{
		putimage(i,0,&lineimg[i],SRCCOPY);
		Sleep(1);
	}
	cleardevice();
	Sleep(1);
	IMAGE bk;
	loadimage(&bk,L"res\\ͼƬ\\��ʼ����.jpg",970,630,false);
	putimage(0,0,&picture_hidden,SRCCOPY);
	MOUSEMSG m;
	do
	{
		m=GetMouseMsg();
	} while (m.uMsg!=WM_LBUTTONDBLCLK);
	BeginBatchDraw();
	for (int i = 0; i < 485; i++)
	{
		putimage(0,0,&bk,SRCCOPY);
		putimage(-i,0,&left,SRCCOPY);
		putimage(485+i,0,&right,SRCCOPY);
		FlushBatchDraw();
	}
	EndBatchDraw();
	win_or_lose_sound=1;
	mciSendString(TEXT("pause win_sound"),NULL,0,NULL);
}
void fail_effect()
{	
	mciSendString(TEXT("pause menubackground"),NULL,0,NULL);
	mciSendString(TEXT("seek lose_sound to start"),NULL,0,NULL);
	mciSendString(TEXT("play lose_sound repeat"),NULL,0,NULL);
	IMAGE hole;
	loadimage(&hole,L"res\\ͼƬ\\��.gif",width,height);
	for(int i=0;i<row;i++)
		for(int j=0;j<column;j++)
		{
			if (mine_number_map[i][j]==-1&&(scaned[i+1][j+1]==0||scaned[i+1][j+1]==2))
			{
				putimage(j*width,i*height,&hole,SRCCOPY);	
				mciSendString(TEXT("seek lose_sound_effect to start"),NULL,0,NULL);
				mciSendString(TEXT("play lose_sound_effect repeat"),NULL,0,NULL);
				Sleep(200);
				mciSendString(TEXT("stop lose_sound_effect"),NULL,0,NULL);
			}
		}
	Sleep(1000);
	IMAGE wholescreen;
	getimage(&wholescreen,0,0,970,630);
	DWORD *buf=GetImageBuffer(&wholescreen);
	for(int i=0;i<611100;i++)
		buf[i]=RGBtoGRAY(buf[i]);
	putimage(0,0,&wholescreen,SRCCOPY);
	Sleep(1000);
	setlinecolor(BLACK);
	for(int i=0;i<970;i++)
	{
		line(i,0,i,630);
		Sleep(2);
	}
	TCHAR pity[3][30]={_T("��ϧ����̾��"),_T("����һʱ���"),_T("����һ���û�")};
	LOGFONT choice;
	gettextstyle(&choice);                     // ��ȡ��ǰ��������
	choice.lfHeight = 48;						// ��������߶�Ϊ 48
	_tcscpy_s(choice.lfFaceName, _T("����"));    // ��������Ϊ�����塱(�߰汾 VC �Ƽ�ʹ�� _tcscpy_s ����)
	choice.lfQuality = ANTIALIASED_QUALITY;    // �������Ч��Ϊ�����  
	settextstyle(&choice);  // 
	setbkmode(TRANSPARENT);
	settextcolor(WHITE);
	Sleep(100);
	for (int j = 0;pity[0][j]!='\0'; j++)
	{
		outtextxy(230+48*j,170,pity[0][j]);
		Sleep(700);
	}
	for(int i=1;i<3;i++)
	{
		for (int j = 0;pity[i][j]!='\0'; j++)
		{
			outtextxy(230+48*j,170+i*70,pity[i][j]);
			Sleep(250);
		}
	}
	Sleep(1500);
	for(int i=0;i<970;i++)
	{
		line(i,0,i,630);
		Sleep(2);
	}
	win_or_lose_sound=0;
	mciSendString(TEXT("pause lose_sound"),NULL,0,NULL);		
}
void scan_map(const int x,const int y)
{
	if (scaned[x][y]==0||(first_click==1&&field[x][y]==0))
	{
		scaned[x][y]=1;
		first_click=0;
 		if(mine_number_map[x-1][y-1]==0)
		{
			scan_map(x-1,y-1);
			scan_map(x-1,y);
			scan_map(x-1,y+1);
			scan_map(x,y-1);
			scan_map(x,y+1);
			scan_map(x+1,y-1);
			scan_map(x+1,y);
			scan_map(x+1,y+1);
		}
	}
}
void initialize_field()
{
	srand(time(NULL));
	int counter=0;
	while(counter<mine_number)
	{

		int minex=rand()%row;
		int miney=rand()%column;
		if(field[minex+1][miney+1]||scaned[minex+1][miney+1]==1)
			continue;
		else
		{
			field[minex+1][miney+1]=1;
			counter++;
		}
	}
	for(int i=0;i<row;i++)
	{
		for(int j=0;j<column;j++)
		{
			if(field[i+1][j+1]==1)
				mine_number_map[i][j]=-1;				//��ͼӳ����-1��ʾ����
			else
			{
				mine_number_map[i][j]=field[i][j+1]+field[i+2][j+1]+field[i+1][j]+field[i+1][j+2];
				mine_number_map[i][j]+=field[i][j]+field[i][j+2]+field[i+2][j]+field[i+2][j+2];
			}
		}
	}
}
void showfield()
{
	BeginBatchDraw();
	putimage(0,0,&picture_hidden,SRCCOPY);
	for(int i=0;i<row;i++)
		for(int j=0;j<column;j++)
		{
			if(scaned[i+1][j+1]==0)
				putimage(j*width,i*height,&grid_picture,SRCCOPY);
			else if (scaned[i+1][j+1]==2)
			{
				putimage(j*width,i*height,&grid_picture,SRCCOPY);
				putimage(j*width,i*height,&mark_picture,SRCAND);
			}else if(mine_number_map[i][j]==0)
			{
				continue;			
			}else if (mine_number_map[i][j]==-1)
			{
				putimage(j*width,i*height,&hole,SRCCOPY);
			}else
			{
				switch (mine_number_map[i][j])
				{
					case 1: putimage(j*width,i*height,&grid_picture,SRCCOPY);
							putimage(j*width,i*height,width,height,&number1,0,0,SRCAND); break;
					case 2: putimage(j*width,i*height,&grid_picture,SRCCOPY);
							putimage(j*width,i*height,width,height,&number2,0,0,SRCAND); break;
					case 3: putimage(j*width,i*height,&grid_picture,SRCCOPY);
							putimage(j*width,i*height,width,height,&number3,0,0,SRCAND); break;
					case 4: putimage(j*width,i*height,&grid_picture,SRCCOPY);
							putimage(j*width,i*height,width,height,&number4,0,0,SRCAND); break;
					case 5: putimage(j*width,i*height,&grid_picture,SRCCOPY);
							putimage(j*width,i*height,width,height,&number5,0,0,SRCAND); break;
					case 6: putimage(j*width,i*height,&grid_picture,SRCCOPY);
							putimage(j*width,i*height,width,height,&number6,0,0,SRCAND); break;
					case 7: putimage(j*width,i*height,&grid_picture,SRCCOPY);
							putimage(j*width,i*height,width,height,&number7,0,0,SRCAND); break;
					case 8: putimage(j*width,i*height,&grid_picture,SRCCOPY);
							putimage(j*width,i*height,width,height,&number8,0,0,SRCAND); break;
					default:break;
				}
				
			}
		}
	EndBatchDraw();
}
////////////////////////////////////////////////////////////////////////�˵������Ϳ�ʼ����Ч������
void seeyoueffect()
{
	loadimage(NULL,L"res\\ͼƬ\\��ʼ����.jpg",970,630,false);
	TCHAR saybye[3][30]={_T("�����˴�����"),_T("�ֲ�֪��ʱ�ù�"),_T("�ʹ˱���������ߺ�")};
	LOGFONT choice;
	gettextstyle(&choice);                     // ��ȡ��ǰ��������
	choice.lfHeight = 48;                      // ��������߶�Ϊ 48
	_tcscpy_s(choice.lfFaceName, _T("����"));    // ��������Ϊ�����塱(�߰汾 VC �Ƽ�ʹ�� _tcscpy_s ����)
	choice.lfQuality = ANTIALIASED_QUALITY;    // �������Ч��Ϊ�����  
	settextstyle(&choice);  // 
	setbkmode(TRANSPARENT);
	settextcolor(BLACK);
	Sleep(100);
	for(int i=0;i<3;i++)
	{
		for (int j = 0;saybye[i][j]!='\0'; j++)
		{
			outtextxy(230+48*j,170+i*70,saybye[i][j]);
			Sleep(110);
		}
	}
	Sleep(500);
	setcolor(BLACK);
	for(int i=0;i<500;i++)
	{
		line(i,0,i,630);
		Sleep(2);
		line(970-i,0,970-i,630);
		Sleep(2);
	}
}
void welcome_back()
{
	loadimage(NULL,L"res\\ͼƬ\\��ʼ����.jpg",970,630,false);
	TCHAR greet[3][30]={_T("����Զ�ι�����������ӭ"),_T("�����������������ţ�"),_T("������������")};
	LOGFONT choice;
	gettextstyle(&choice);                     // ��ȡ��ǰ��������
	choice.lfHeight = 48;                      // ��������߶�Ϊ 48
	_tcscpy_s(choice.lfFaceName, _T("����"));    // ��������Ϊ�����塱(�߰汾 VC �Ƽ�ʹ�� _tcscpy_s ����)
	choice.lfQuality = ANTIALIASED_QUALITY;    // �������Ч��Ϊ�����  
	settextstyle(&choice);  // 
	setbkmode(TRANSPARENT);
	settextcolor(BLACK);
	Sleep(100);
	for(int i=0;i<3;i++)
	{
		for (int j = 0;greet[i][j]!='\0'; j++)
		{
			outtextxy(230+48*j,170+i*48,greet[i][j]);
			Sleep(70);
		}
	}
	IMAGE choiceframe(400,60);
	loadimage(&choiceframe,L"res\\ͼƬ\\ѡ���.jpg",400,60,false);
	putimage(200,350,&choiceframe,SRCAND);
	IMAGE temporary_image;
	getimage(&temporary_image,235,320,615,50);
	HRGN input_box=CreateRectRgn(235,320,850,370);
	setcliprgn(input_box);
	DeleteObject(input_box);
	int key,counter=0;
	fflush(stdin);
	for(bool name_input_flag=1;name_input_flag;)
	{
		key=_getch();
		if(key==13)
		{
			if(counter>0)
				name_input_flag=0;
		}
		else if(key==32||(97<=key&&key<=122)||(65<=key&&key<=90))
		{
			if(counter<19)
			{
				user_name[counter]=TCHAR(key);
				counter++;
				user_name[counter]='\0';
			}
		}else if(key==8)
		{
			if(counter>0)
			{
				counter--;
				user_name[counter]='\0';
			}
		}
		clearcliprgn();
		putimage(235,320,&temporary_image,SRCCOPY);
		outtextxy(235,320,user_name);
	}
	setcliprgn(NULL);
}
void new_user_register()
{
	loadimage(NULL,L"res\\ͼƬ\\��ʼ����.jpg",970,630,false);
	TCHAR greet[2][30]={_T("����Զ����������ӭ"),_T("������������")};
	LOGFONT choice;
	gettextstyle(&choice);                     // ��ȡ��ǰ��������
	choice.lfHeight = 48;                      // ��������߶�Ϊ 48
	_tcscpy_s(choice.lfFaceName, _T("����"));    // ��������Ϊ�����塱(�߰汾 VC �Ƽ�ʹ�� _tcscpy_s ����)
	choice.lfQuality = ANTIALIASED_QUALITY;    // �������Ч��Ϊ�����  
	settextstyle(&choice);  // 
	setbkmode(TRANSPARENT);
	settextcolor(BLACK);
	Sleep(100);
	for(int i=0;i<2;i++)
	{
		for (int j = 0;greet[i][j]!='\0'; j++)
		{
			outtextxy(230+48*j,170+i*48,greet[i][j]);
			Sleep(70);
		}
	}
	IMAGE choiceframe(400,60);
	loadimage(&choiceframe,L"res\\ͼƬ\\ѡ���.jpg",400,60,false);
	putimage(200,300,&choiceframe,SRCAND);
	IMAGE temporary_image;
	getimage(&temporary_image,235,270,615,50);
	HRGN input_box=CreateRectRgn(235,270,850,320);
	setcliprgn(input_box);
	DeleteObject(input_box);
	int key,counter=0;
	fflush(stdin);
	for(bool name_input_flag=1;name_input_flag;)
	{
		key=_getch();
		if(key==13)
		{
			if(counter>0)
				name_input_flag=0;
		}
		else if(key==32||(97<=key&&key<=122)||(65<=key&&key<=90))
		{
			if(counter<19)
			{
				user_name[counter]=TCHAR(key);
				counter++;
				user_name[counter]='\0';
			}
		}else if(key==8)
		{
			if(counter>0)
			{
				counter--;
				user_name[counter]='\0';
			}
		}
		clearcliprgn();
		putimage(235,270,&temporary_image,SRCCOPY);
		outtextxy(235,270,user_name);
	}
	setcliprgn(NULL);
}
void main_menu()
{
	IMAGE choicehead(50,50),choiceframe(210,60);
	loadimage(NULL,L"res\\ͼƬ\\��ʼ����.jpg",970,630,false);
	loadimage(&choicehead,L"res\\ͼƬ\\ѡ��ͷͼƬ.jpg",50,50,false);
	loadimage(&choiceframe,L"res\\ͼƬ\\ѡ���.jpg",210,60,false);
	putimage(200,210,&choicehead,SRCAND);
	putimage(110,300,&choicehead,SRCAND);
	putimage(50,400,&choicehead,SRCAND);
	putimage(250,220,&choiceframe,SRCAND);
	putimage(160,310,&choiceframe,SRCAND);
	putimage(100,410,&choiceframe,SRCAND);
	LOGFONT choice;
	gettextstyle(&choice);                     // ��ȡ��ǰ��������
	choice.lfHeight = 48;                      // ��������߶�Ϊ 48
	_tcscpy_s(choice.lfFaceName, _T("����"));    // ��������Ϊ�����塱(�߰汾 VC �Ƽ�ʹ�� _tcscpy_s ����)
	choice.lfQuality = ANTIALIASED_QUALITY;    // �������Ч��Ϊ�����  
	settextstyle(&choice);  // 
	setbkmode(TRANSPARENT);
	settextcolor(BLACK);
	outtextxy(265,205,_T("�»���"));
	outtextxy(175,295,_T("�����ż�"));
	outtextxy(115,395,_T("�˼����"));
	MOUSEMSG m;
	bool trace =0;//���Ƿ�����newgame����
	FlushMouseMsgBuffer();
	for(bool main_menu_flag=1;main_menu_flag;)
	{
		m=GetMouseMsg();
		if((175<m.x&&m.x<370&&295<m.y&&m.y<345)||(115<m.x&&m.x<310&&395<m.y&&m.y<445)||(265<m.x&&m.x<410&&205<m.y&&m.y<255))
		{
			hcur = LoadCursor(NULL, IDC_HAND); // ����ϵͳԤ�õ������ʽ
			SetClassLong(hwnd, GCL_HCURSOR, (long)hcur); // ���ô�����������ʽ
		}else
		{
			hcur = LoadCursor(NULL, IDC_ARROW); // ����ϵͳԤ�õ������ʽ
			SetClassLong(hwnd, GCL_HCURSOR, (long)hcur); // ���ô�����������ʽ
		}
		if(trace)
		{
			loadimage(NULL,L"res\\ͼƬ\\��ʼ����.jpg",970,630,false);
			putimage(200,210,&choicehead,SRCAND);
			putimage(110,300,&choicehead,SRCAND);
			putimage(50,400,&choicehead,SRCAND);
			putimage(250,220,&choiceframe,SRCAND);
			putimage(160,310,&choiceframe,SRCAND);
			putimage(100,410,&choiceframe,SRCAND);
			LOGFONT choice;
			gettextstyle(&choice);                     // ��ȡ��ǰ��������
			choice.lfHeight = 48;                      // ��������߶�Ϊ 48
			_tcscpy_s(choice.lfFaceName, _T("����"));    // ��������Ϊ�����塱(�߰汾 VC �Ƽ�ʹ�� _tcscpy_s ����)
			choice.lfQuality = ANTIALIASED_QUALITY;    // �������Ч��Ϊ�����  
			settextstyle(&choice);  // 
			setbkmode(TRANSPARENT);
			settextcolor(BLACK);
			outtextxy(265,205,_T("�»���"));
			outtextxy(175,295,_T("�����ż�"));
			outtextxy(115,395,_T("�˼����"));
			trace=0;
		}
		switch (m.uMsg)
		{
			case WM_LBUTTONDOWN:if(175<m.x&&m.x<370&&295<m.y&&m.y<345)//�����ż�
								{	
									main_menu_flag=loadrecord();
									trace=0;
									if(main_menu_flag==0)
										replaychoice=1;
								}
								if(115<m.x&&m.x<310&&395<m.y&&m.y<445)//�˼����
								{	 attribute(); trace=0;}
								if(265<m.x&&m.x<410&&205<m.y&&m.y<255)//�»���
								{
									int newgame_choice=newgame();
									trace=1;
									switch (newgame_choice)
									{
										case 1:row =5,column=5;mine_number=3;main_menu_flag=0;replaychoice=0; break;
										case 2:row=10;column=10;mine_number=10;main_menu_flag=0;replaychoice=0;break;
										case 3:main_menu_flag=selfdefine();replaychoice=0;break;
										case 4:break;
										default:break;
									}
								}
								break;
			default:break;
		}
	}
	hcur = LoadCursor(NULL, IDC_ARROW); // ����ϵͳԤ�õ������ʽ
	SetClassLong(hwnd, GCL_HCURSOR, (long)hcur); // ���ô�����������ʽ
}
bool selfdefine()
{
	IMAGE choicehead(50,50),choiceframe(140,60);	
	loadimage(NULL,L"res\\ͼƬ\\��ʼ����.jpg",970,630,false);
	loadimage(&choicehead,L"res\\ͼƬ\\ѡ��ͷͼƬ.jpg",50,50,false);
	loadimage(&choiceframe,L"res\\ͼƬ\\ѡ���.jpg",140,60,false);
	putimage(200,210,&choicehead,SRCAND);
	putimage(110,300,&choicehead,SRCAND);
	putimage(50,400,&choicehead,SRCAND);
	putimage(250,220,&choiceframe,SRCAND);
	putimage(160,310,&choiceframe,SRCAND);
	putimage(100,410,&choiceframe,SRCAND);
	LOGFONT choice;
	gettextstyle(&choice);                     // ��ȡ��ǰ��������
	choice.lfHeight = 48;                      // ��������߶�Ϊ 48
	_tcscpy_s(choice.lfFaceName, _T("����"));    // ��������Ϊ�����塱(�߰汾 VC �Ƽ�ʹ�� _tcscpy_s ����)
	choice.lfQuality = ANTIALIASED_QUALITY;    // �������Ч��Ϊ�����  
	settextstyle(&choice);  // 
	setbkmode(TRANSPARENT);
	settextcolor(BLACK);
	outtextxy(405,205,_T("��"));
	outtextxy(315,295,_T("��"));
	outtextxy(255,395,_T("���"));
	TCHAR hang[3]={0},lie[3]={0},dotnumber[5]={0};
	IMAGE hangimg,lieimg,dotnumberimg;
	getimage(&hangimg,250,200,140,60);
	getimage(&lieimg,160,290,140,60);
	getimage(&dotnumberimg,100,390,140,60);
	int key,counter=0;
	fflush(stdin);
	for(bool hang_flag=1;hang_flag;)		//�����ֵ
	{
		key=_getch();
		if(key==13)
		{
			if(counter>0)
			{
				row=0;
				for(int i=0;hang[i]!='\0';i++)
				row=row*10+hang[i]-48;
				if(row<46)
					hang_flag=0;
			}
		}
		else if(48<=key&&key<=57)
		{
			if(counter<2)
			{
				hang[counter]=TCHAR(key);
				counter++;
				hang[counter]='\0';
			}
		}else if(key==8)
		{
			if(counter>0)
			{
				counter--;
				hang[counter]='\0';
			}
		}
		putimage(250,200,&hangimg,SRCCOPY);
		outtextxy(270,200,hang);
	}
	counter=0;
	fflush(stdin);
	for(bool lie_flag=1;lie_flag;)		//�����ֵ
	{
		key=_getch();
		if(key==13)
		{
			if(counter>0)
			{
				column=0;
				for(int i=0;lie[i]!='\0';i++)
					column=column*10+lie[i]-48;
				if(column<45)
					lie_flag=0;
			}
		}
		else if(48<=key&&key<=57)
		{
			if(counter<2)
			{
				lie[counter]=TCHAR(key);
				counter++;
				lie[counter]='\0';
			}
		}else if(key==8)
		{
			if(counter>0)
			{
				counter--;
				lie[counter]='\0';
			}
		}
		putimage(160,290,&lieimg,SRCCOPY);
		outtextxy(180,290,lie);
	}
	counter=0;
	fflush(stdin);
	for(bool dot_flag=1;dot_flag;)							//���"��"ֵ
	{
		key=_getch();
		if(key==13)
		{
			if(counter>0)
			{
				mine_number=0;
				for(int i=0;dotnumber[i]!='\0';i++)
					mine_number=mine_number*10+dotnumber[i]-48;
				if(mine_number<row*column)
					dot_flag=0;
			}
		}
		else if(48<=key&&key<=57)
		{
			if(counter<4)
			{
				dotnumber[counter]=TCHAR(key);
				counter++;
				dotnumber[counter]='\0';
			}
		}else if(key==8)
		{
			if(counter>0)
			{
				counter--;
				dotnumber[counter]='\0';
			}
		}
		putimage(100,390,&dotnumberimg,SRCCOPY);
		outtextxy(120,390,dotnumber);
	}
	return 0;
}
int newgame()
{
	IMAGE choicehead(50,50),choiceframe(210,60);
	loadimage(NULL,L"res\\ͼƬ\\��ʼ����.jpg",970,630,false);
	loadimage(&choicehead,L"res\\ͼƬ\\ѡ��ͷͼƬ.jpg",50,50,false);
	loadimage(&choiceframe,L"res\\ͼƬ\\ѡ���.jpg",210,60,false);
	putimage(320,120,&choicehead,SRCAND);
	putimage(200,210,&choicehead,SRCAND);
	putimage(110,300,&choicehead,SRCAND);
	putimage(50,400,&choicehead,SRCAND);
	putimage(370,130,&choiceframe,SRCAND);
	putimage(250,220,&choiceframe,SRCAND);
	putimage(160,310,&choiceframe,SRCAND);
	putimage(100,410,&choiceframe,SRCAND);
	LOGFONT choice;
	gettextstyle(&choice);                     // ��ȡ��ǰ��������
	choice.lfHeight = 48;                      // ��������߶�Ϊ 48
	_tcscpy_s(choice.lfFaceName, _T("����"));    // ��������Ϊ�����塱(�߰汾 VC �Ƽ�ʹ�� _tcscpy_s ����)
	choice.lfQuality = ANTIALIASED_QUALITY;    // �������Ч��Ϊ�����  
	settextstyle(&choice);  // 
	setbkmode(TRANSPARENT);
	settextcolor(BLACK);
	outtextxy(385,115,_T("С¶����"));
	outtextxy(265,205,_T("�緶����"));
	outtextxy(175, 295, _T("��������"));
	outtextxy(130,395,_T("�� ��"));
	MOUSEMSG m;
	FlushMouseMsgBuffer();
	while (true)
	{
		m=GetMouseMsg();
		if((385<m.x&&m.x<480&&115<m.y&&m.y<165)||(265<m.x&&m.x<460&&205<m.y&&m.y<255)||(175<m.x&&m.x<280&&295<m.y&&m.y<345)||(130<m.x&&m.x<250&&395<m.y&&m.y<445))
		{
			hcur = LoadCursor(NULL, IDC_HAND); // ����ϵͳԤ�õ������ʽ
			SetClassLong(hwnd, GCL_HCURSOR, (long)hcur); // ���ô�����������ʽ
		}else
		{
			hcur = LoadCursor(NULL, IDC_ARROW); // ����ϵͳԤ�õ������ʽ
			SetClassLong(hwnd, GCL_HCURSOR, (long)hcur); // ���ô�����������ʽ
		}
		switch (m.uMsg)
		{
		case WM_LBUTTONDOWN:if(385<m.x&&m.x<480&&115<m.y&&m.y<165)//��һ��
							{
								hcur = LoadCursor(NULL, IDC_ARROW); // ����ϵͳԤ�õ������ʽ
								SetClassLong(hwnd, GCL_HCURSOR, (long)hcur); // ���ô�����������ʽ
								return 1;
							}
							if(265<m.x&&m.x<460&&205<m.y&&m.y<255)//�ڶ���
							{
								hcur = LoadCursor(NULL, IDC_ARROW); // ����ϵͳԤ�õ������ʽ
								SetClassLong(hwnd, GCL_HCURSOR, (long)hcur); // ���ô�����������ʽ
								return 2;
							}
							if(175<m.x&&m.x<280&&295<m.y&&m.y<345)//������
							{
								hcur = LoadCursor(NULL, IDC_ARROW); // ����ϵͳԤ�õ������ʽ
								SetClassLong(hwnd, GCL_HCURSOR, (long)hcur); // ���ô�����������ʽ
								return 3;
							}
							if(130<m.x&&m.x<250&&395<m.y&&m.y<445)//������
							{
								hcur = LoadCursor(NULL, IDC_ARROW); // ����ϵͳԤ�õ������ʽ
								SetClassLong(hwnd, GCL_HCURSOR, (long)hcur); // ���ô�����������ʽ
								return 4;
							}
								break;
			default:break;
		}
	}
}
int loadrecord()
{
	if(new_flag)
	{
		IMAGE temporary_image;
		getimage(&temporary_image,500,0,470,640);
		HRGN input_box=CreateRectRgn(500,0,970,640);
		setcliprgn(input_box);
		DeleteObject(input_box);
		TCHAR version[2][30]={_T("�������ε���"),
							  _T("��δ������δ��ɵ���Ʒ")};
		LOGFONT choice;
		gettextstyle(&choice);                     // ��ȡ��ǰ��������
		choice.lfHeight = 35;                      // ��������߶�Ϊ 48
		_tcscpy_s(choice.lfFaceName, _T("����"));    // ��������Ϊ�����塱(�߰汾 VC �Ƽ�ʹ�� _tcscpy_s ����)
		choice.lfQuality = ANTIALIASED_QUALITY;    // �������Ч��Ϊ�����  
		settextstyle(&choice);  // 
		setbkmode(TRANSPARENT);
		settextcolor(BLACK);
		for(int i=0;i<2;i++)
		{
			for(int j=0;version[i][j]!='\0';j++)
			{
				outtextxy(750-48*i,35+48*j,version[i][j]);
				Sleep(50);
			}
			Sleep(100);
		}
		MOUSEMSG m;
		do
		{
			m=GetMouseMsg();
		} while (m.uMsg!=WM_LBUTTONDOWN);
		putimage(500,0,&temporary_image,SRCCOPY);
		setcliprgn(NULL);
		return 1;
	}else
	{
		ifstream infile(user_name);
		infile>>row>>column>>mine_number>>first_click>>picture_number>>duration;
		for(int i=0;i<52;i++)
		{
			for (int j = 0; j < 52; j++)
			{
				infile>>field[i][j];
			}
		}
		for (int i = 0; i < 52; i++)
		{
			for (int j = 0; j < 52; j++)
			{
				infile>>scaned[i][j];
			}
		}
		for (int i = 0; i < 50; i++)
		{
			for (int j = 0; j < 50; j++)
			{
				infile>>mine_number_map[i][j];
			}
		}
		infile.close();
		return 0;
	}
}
void attribute()
{
	IMAGE temporary_image;
	getimage(&temporary_image,500,0,470,500);
	HRGN input_box=CreateRectRgn(500,0,970,500);
	setcliprgn(input_box);
	DeleteObject(input_box);
	TCHAR version[5][30]={_T("��Ϸ�����޸��ɻ�"),
						  _T("������������"),
						  _T("����ʱ�䣺"),
						  _T("����һ����һ��һ��"),
						  _T("�汾�ţ�������")};
	LOGFONT choice;
	gettextstyle(&choice);                     // ��ȡ��ǰ��������
	choice.lfHeight = 35;                      // ��������߶�Ϊ 48
	_tcscpy_s(choice.lfFaceName, _T("����"));    // ��������Ϊ�����塱(�߰汾 VC �Ƽ�ʹ�� _tcscpy_s ����)
	choice.lfQuality = ANTIALIASED_QUALITY;    // �������Ч��Ϊ�����  
	settextstyle(&choice);  // 
	setbkmode(TRANSPARENT);
	settextcolor(BLACK);
	for(int i=0;i<5;i++)
	{
		for(int j=0;version[i][j]!='\0';j++)
		{
			Sleep(50);
			outtextxy(800-48*i,35+48*j,version[i][j]);
		}
	}
	MOUSEMSG m;
	do
	{
		m=GetMouseMsg();
	} while (m.uMsg!=WM_LBUTTONDOWN);
	putimage(500,0,&temporary_image,SRCCOPY);
	setcliprgn(NULL);
}
int firstmenu()
{
	loadimage(NULL,L"res\\ͼƬ\\��ʼ����.jpg",970,630,false);
	IMAGE choicehead(50,50),choiceframe(140,60);
	loadimage(&choicehead,L"res\\ͼƬ\\ѡ��ͷͼƬ.jpg",50,50,false);
	loadimage(&choiceframe,L"res\\ͼƬ\\ѡ���.jpg",140,60,false);
	putimage(110,300,&choicehead,SRCAND);
	putimage(50,400,&choicehead,SRCAND);
	putimage(160,310,&choiceframe,SRCAND);
	putimage(100,410,&choiceframe,SRCAND);
	LOGFONT choice;
	gettextstyle(&choice);                     // ��ȡ��ǰ��������
	choice.lfHeight = 48;                      // ��������߶�Ϊ 48
	_tcscpy_s(choice.lfFaceName, _T("����"));    // ��������Ϊ�����塱(�߰汾 VC �Ƽ�ʹ�� _tcscpy_s ����)
	choice.lfQuality = ANTIALIASED_QUALITY;    // �������Ч��Ϊ�����  
	settextstyle(&choice);  // 
	setbkmode(TRANSPARENT);
	settextcolor(BLACK);
	outtextxy(175, 295, _T("����"));
	outtextxy(115,395,_T("����"));
	MOUSEMSG m;
	FlushMouseMsgBuffer();
	while (true)
	{
		m=GetMouseMsg();
		if((175<m.x&&m.x<280&&295<m.y&&m.y<345)||(115<m.x&&m.x<220&&395<m.y&&m.y<445))
		{
			hcur = LoadCursor(NULL, IDC_HAND); // ����ϵͳԤ�õ������ʽ
			SetClassLong(hwnd, GCL_HCURSOR, (long)hcur); // ���ô�����������ʽ
		}else
		{
			hcur = LoadCursor(NULL, IDC_ARROW); // ����ϵͳԤ�õ������ʽ
			SetClassLong(hwnd, GCL_HCURSOR, (long)hcur); // ���ô�����������ʽ
		}

		switch (m.uMsg)
		{
			case WM_LBUTTONDOWN:if(175<m.x&&m.x<280&&295<m.y&&m.y<345)
								{
									hcur = LoadCursor(NULL, IDC_ARROW); // ����ϵͳԤ�õ������ʽ
									SetClassLong(hwnd, GCL_HCURSOR, (long)hcur); // ���ô�����������ʽ
									return 0;
								}
								if(115<m.x&&m.x<220&&395<m.y&&m.y<445)
								{
									hcur = LoadCursor(NULL, IDC_ARROW); // ����ϵͳԤ�õ������ʽ
									SetClassLong(hwnd, GCL_HCURSOR, (long)hcur); // ���ô�����������ʽ
									return 1;
								}
								break;
			default:break;
		}
	}
}
void loadingeffect()
{
	TCHAR loading[]=_T("loading");
	outtextxy(300,250,loading);
	for(int i=0;i<4;i++)
	{
		Sleep(400);
		outtextxy(350+i*10,250,'.');
		outtextxy(350+i*10+5,250,' ');
		Sleep(400);
	}
}
void begineffect()
{
	mciSendString(TEXT("play beginsong repeat"),NULL,0,NULL);
	TCHAR s[10][100] ={ _T(" ��ӭ���������Ϸ"),
						_T(" �������Ϸ�У��㽫��Ϊһ����ɫ��ͼ���޸�ʦ"),
						_T(" ��ȥ��Щͼ������ʷ�ĳ���"),
						_T(" ���ھͿ�ʼ�ɣ���ʼ��ΰ��Ĺ���"),
						_T(" ......"),
						_T(" �����ո��������")};
	for(int i=0;i<6;i++)
	{
		for (int j = 0;s[i][j]!='\0'; j++)
		{
			outtextxy(230+18*j,170+i*25,s[i][j]);
			Sleep(70);
		}
	}
	mciSendString(TEXT("stop beginsong"),NULL,0,NULL);
	int key;
	do
	{
		key=_getch();
	} while (key!=32);
}

