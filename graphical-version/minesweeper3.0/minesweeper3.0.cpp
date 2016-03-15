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
////////////////////////////////////////////////////////////////////全局变量
TCHAR user_name[20]=L"admin";
bool new_flag;						//判断是否是新用户
int row,column,mine_number;			//行，列，不能点的格数
bool field[52][52]={0};
int scaned[52][52]={0},	mine_number_map[50][50]={0};
bool replaychoice=0;			//复盘标记
bool first_click=1;				//第一次保护用
float width,height;				//网格宽度	
int mouse_x,mouse_y;			//点中的格子的坐标
bool win_or_lose_sound;			//控制音效
IMAGE number1,number2,number3,number4,number5,number6,number7,number8;		//存数字照片的八个对象
IMAGE grid_picture,mark_picture,hole;			//网格图片，标记图片，洞
IMAGE picture_hidden;							//随机背景
int picture_number;				//随机图片的编号
HCURSOR hcur;					//鼠标光标
HWND hwnd;						//窗口句柄
clock_t time_mark;				//计时用
int duration=0;					//已用时长
///////////////////////////////////////////////////////////////////供调用函数
void begineffect();		//开始效果
void loadingeffect();//因为音乐开头有大约4秒的空白，所以要加一个loading
int firstmenu();    //初访——0，归来——1
void new_user_register();//新用户注册
void welcome_back();     //老用户登陆
void main_menu();		//主菜单
void seeyoueffect();			//结束效果
void game();			//游戏函数
int loadrecord();		//读档
bool replayquestion();	//重玩询问
void scan_map(const int x,const int y);			//展开地图的递归函数
void initialize_field();					//初始化地图
void showfield();					//输出地图
bool win();							//判断是否获胜
int suspend();						//暂停
void storefield();					//存档
void fail_effect();					//失败效果
void win_effect();					//成功效果
void attribute();					//游戏属性
int newgame();						//难度选择
bool selfdefine();					//自定义难度
void correct_memory();				//帮助确定是否已经有过记录
void picturechoice(int n);			//加载选定的图片
int _tmain(int argc, _TCHAR* argv[])
{
//缓冲音乐	
	mciSendString(TEXT("open res\\音乐\\打字机音效.mp3 alias beginsong"),NULL,0,NULL);
	mciSendString(TEXT("open res\\音乐\\忘记时间钢琴版.mp3 alias menubackground"),NULL,0,NULL);
	mciSendString(TEXT("open res\\音乐\\撕纸音效.mp3 alias lose_sound_effect"),NULL,0,NULL);
	mciSendString(TEXT("open res\\音乐\\莫失莫忘.mp3 alias lose_sound"),NULL,0,NULL);
	mciSendString(TEXT("open res\\音乐\\逍遥叹.mp3 alias win_sound"),NULL,0,NULL);
//开始
	initgraph(970,630);
	hcur = LoadCursor(NULL, IDC_ARROW); // 加载系统预置的鼠标样式
	hwnd = GetHWnd(); // 获取绘图窗口句柄
	SetClassLong(hwnd, GCL_HCURSOR, (long)hcur); // 设置窗口类的鼠标样式
	begineffect();
	cleardevice();
	mciSendString(TEXT("play menubackground repeat"),NULL,0,NULL);
	loadingeffect();
	bool user_flag=firstmenu();					//获得用户名
	switch (user_flag)
	{
		case 0: new_flag=1;new_user_register(); break;
		case 1: new_flag=0;welcome_back(); break;
		default:closegraph();cerr<<"程序出错，请重新运行"; break;
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
		loadimage(NULL,L"res\\图片\\开始背景.jpg",970,630,false);
		TCHAR pity[4][30]={_T("许是先生四海云游"),_T("直把此地作他乡"),_T("先生未曾到过此地"),_T("初次见面，先生幸会")};
		LOGFONT choice;
		gettextstyle(&choice);                     // 获取当前字体设置
		choice.lfHeight = 48;                      // 设置字体高度为 48
		_tcscpy_s(choice.lfFaceName, _T("楷体"));    // 设置字体为“楷体”(高版本 VC 推荐使用 _tcscpy_s 函数)
		choice.lfQuality = ANTIALIASED_QUALITY;    // 设置输出效果为抗锯齿  
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
		loadimage(NULL,L"res\\图片\\开始背景.jpg",970,630,false);
		TCHAR pity[4][30]={_T("许是先生四海云游"),_T("直把他乡作此地"),_T("先生归来，不及远迎"),_T("还请先生见谅")};
		LOGFONT choice;
		gettextstyle(&choice);                     // 获取当前字体设置
		choice.lfHeight = 48;                      // 设置字体高度为 48
		_tcscpy_s(choice.lfFaceName, _T("楷体"));    // 设置字体为“楷体”(高版本 VC 推荐使用 _tcscpy_s 函数)
		choice.lfQuality = ANTIALIASED_QUALITY;    // 设置输出效果为抗锯齿  
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
		loadimage(&wholescreen,L"res\\图片\\开始背景.jpg",970,630,false);
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
		loadimage(NULL,L"res\\图片\\开始背景.jpg",970,630,false);
	}
	TCHAR pity[4][30]={_T("览过佳画一幅"),_T("不知先生此后意欲如何"),_T("离开僻地云游四方"),_T("抑或再留下一幅传世佳作")};
	LOGFONT choice;
	gettextstyle(&choice);                     // 获取当前字体设置
	choice.lfHeight = 48;                      // 设置字体高度为 48
	_tcscpy_s(choice.lfFaceName, _T("楷体"));    // 设置字体为“楷体”(高版本 VC 推荐使用 _tcscpy_s 函数)
	choice.lfQuality = ANTIALIASED_QUALITY;    // 设置输出效果为抗锯齿  
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
				hcur = LoadCursor(NULL, IDC_HAND); // 加载系统预置的鼠标样式
				SetClassLong(hwnd, GCL_HCURSOR, (long)hcur); // 设置窗口类的鼠标样式
		}else
		{
				hcur = LoadCursor(NULL, IDC_ARROW); // 加载系统预置的鼠标样式
				SetClassLong(hwnd, GCL_HCURSOR, (long)hcur); // 设置窗口类的鼠标样式
		}
		switch (m.uMsg)
		{
			case WM_LBUTTONDOWN:if(win_or_lose_sound==0)
									mciSendString(TEXT("pause lose_sound"),NULL,0,NULL);
								else if(win_or_lose_sound==1)
									mciSendString(TEXT("stop win_sound"),NULL,0,NULL);
								if(230<m.x&&m.x<614&&310<m.y&&m.y<358)
								{
									hcur = LoadCursor(NULL, IDC_ARROW); // 加载系统预置的鼠标样式
									SetClassLong(hwnd, GCL_HCURSOR, (long)hcur); // 设置窗口类的鼠标样式
									return 0;
								}
								else if(230<m.x&&m.x<758&&390<m.y&&m.y<438)
								{
									hcur = LoadCursor(NULL, IDC_ARROW); // 加载系统预置的鼠标样式
									SetClassLong(hwnd, GCL_HCURSOR, (long)hcur); // 设置窗口类的鼠标样式
									return 1;
								}
			default:break;
		}
	}
}
//////////////////////////////////////////////游戏函数
void game()
{
	if(replaychoice)							//读档或初始化棋盘
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
		for(int i=0;i<column+2;i++)				//初始化scaned的边界为1，即已打开
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
	loadimage(&number1,L"res\\图片\\1.jpg",width,height,true);
	loadimage(&number2,L"res\\图片\\2.jpg",width,height,true);
	loadimage(&number3,L"res\\图片\\3.jpg",width,height,true);
	loadimage(&number4,L"res\\图片\\4.jpg",width,height,true);
	loadimage(&number5,L"res\\图片\\5.jpg",width,height,true);
	loadimage(&number6,L"res\\图片\\6.jpg",width,height,true);
	loadimage(&number7,L"res\\图片\\7.jpg",width,height,true);
	loadimage(&number8,L"res\\图片\\8.jpg",width,height,true);
	loadimage(&grid_picture,L"res\\图片\\青花瓷.jpg",width,height,true);
	loadimage(&mark_picture,L"res\\图片\\梅花.jpg",width,height,true);
	loadimage(&hole,L"res\\图片\\洞.gif",width,height,true);
	time_mark=clock();				//计时用
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
		case  0:loadimage(&picture_hidden,L"res\\随机背景\\00.jpg",970,630,false);break;
		case  1:loadimage(&picture_hidden,L"res\\随机背景\\01.jpg",970,630,false);break;
		case  2:loadimage(&picture_hidden,L"res\\随机背景\\02.jpg",970,630,false);break;
		case  3:loadimage(&picture_hidden,L"res\\随机背景\\03.jpg",970,630,false);break;
		case  4:loadimage(&picture_hidden,L"res\\随机背景\\04.jpg",970,630,false);break;
		case  5:loadimage(&picture_hidden,L"res\\随机背景\\05.jpg",970,630,false);break;
		case  6:loadimage(&picture_hidden,L"res\\随机背景\\06.jpg",970,630,false);break;
		case  7:loadimage(&picture_hidden,L"res\\随机背景\\07.jpg",970,630,false);break;
		case  8:loadimage(&picture_hidden,L"res\\随机背景\\08.jpg",970,630,false);break;
		case  9:loadimage(&picture_hidden,L"res\\随机背景\\09.jpg",970,630,false);break;
		case 10:loadimage(&picture_hidden,L"res\\随机背景\\10.jpg",970,630,false);break;
		case 11:loadimage(&picture_hidden,L"res\\随机背景\\11.jpg",970,630,false);break;
		case 12:loadimage(&picture_hidden,L"res\\随机背景\\12.jpg",970,630,false);break;
		case 13:loadimage(&picture_hidden,L"res\\随机背景\\13.jpg",970,630,false);break;
		case 14:loadimage(&picture_hidden,L"res\\随机背景\\14.jpg",970,630,false);break;
		case 15:loadimage(&picture_hidden,L"res\\随机背景\\15.jpg",970,630,false);break;
		case 16:loadimage(&picture_hidden,L"res\\随机背景\\16.jpg",970,630,false);break;
		case 17:loadimage(&picture_hidden,L"res\\随机背景\\17.jpg",970,630,false);break;
		case 18:loadimage(&picture_hidden,L"res\\随机背景\\18.jpg",970,630,false);break;
		case 19:loadimage(&picture_hidden,L"res\\随机背景\\19.jpg",970,630,false);break;
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
	gettextstyle(&choice);                     // 获取当前字体设置
	choice.lfHeight = 48;                      // 设置字体高度为 48
	_tcscpy_s(choice.lfFaceName, _T("华文新魏"));    // 设置字体为“华文新魏”(高版本 VC 推荐使用 _tcscpy_s 函数)
	choice.lfQuality = ANTIALIASED_QUALITY;    // 设置输出效果为抗锯齿  
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
	outtextxy(200,200,L"未雨绸缪，暂存此画");
	outtextxy(200,300,L"搁笔");
	outtextxy(200,400,L"先生已伏案");
	outtextxy(440,400,ctottime);
	outtextxy(545,400,L"秒");
	FlushMouseMsgBuffer();
	while (true)
	{
		m=GetMouseMsg();
		if((200<m.x&&m.x<632&&200<m.y&&m.y<250)||(200<m.x&&m.x<<296&&300<m.y&&m.y<350))
		{
			hcur = LoadCursor(NULL, IDC_HAND); // 加载系统预置的鼠标样式
			SetClassLong(hwnd, GCL_HCURSOR, (long)hcur); // 设置窗口类的鼠标样式
		}else
		{
			hcur = LoadCursor(NULL, IDC_ARROW); // 加载系统预置的鼠标样式
			SetClassLong(hwnd, GCL_HCURSOR, (long)hcur); // 设置窗口类的鼠标样式
		}
		switch (m.uMsg)
		{
			case WM_LBUTTONDOWN:if(200<m.x&&m.x<632&&200<m.y&&m.y<250)
								{
									storefield();
									showfield();
									hcur = LoadCursor(NULL, IDC_ARROW); // 加载系统预置的鼠标样式
									SetClassLong(hwnd, GCL_HCURSOR, (long)hcur); // 设置窗口类的鼠标样式
									return 1;
								}else if (200<m.x&&m.x<<296&&300<m.y&&m.y<350)
								{
									putimage(0,0,&wholescreen,SRCCOPY);
									outtextxy(200,200,L"先生在此搁笔");
									outtextxy(200,300,L"是欲留待他日");
									outtextxy(200,400,L"抑或就此尘封");
									FlushMouseMsgBuffer();
									while(true)
									{
										m=GetMouseMsg();
										if((200<m.x&&m.x<488&&300<m.y&&m.y<350)||(200<m.x&&m.x<488&&400<m.y&&m.y<450))
										{
											hcur = LoadCursor(NULL, IDC_HAND); // 加载系统预置的鼠标样式
											SetClassLong(hwnd, GCL_HCURSOR, (long)hcur); // 设置窗口类的鼠标样式
										}else
										{
											hcur = LoadCursor(NULL, IDC_ARROW); // 加载系统预置的鼠标样式
											SetClassLong(hwnd, GCL_HCURSOR, (long)hcur); // 设置窗口类的鼠标样式
										}
										if(m.uMsg==WM_LBUTTONDOWN)
										{
											if (200<m.x&&m.x<488&&300<m.y&&m.y<350)
											{
												storefield();
												hcur = LoadCursor(NULL, IDC_ARROW); // 加载系统预置的鼠标样式
												SetClassLong(hwnd, GCL_HCURSOR, (long)hcur); // 设置窗口类的鼠标样式
												return 0;
											}else if(200<m.x&&m.x<488&&400<m.y&&m.y<450)
											{
												hcur = LoadCursor(NULL, IDC_ARROW); // 加载系统预置的鼠标样式
												SetClassLong(hwnd, GCL_HCURSOR, (long)hcur); // 设置窗口类的鼠标样式
												return 0;
											}
										}
									}
								}
								break;
			case WM_RBUTTONDBLCLK:showfield();
								hcur = LoadCursor(NULL, IDC_ARROW); // 加载系统预置的鼠标样式
								SetClassLong(hwnd, GCL_HCURSOR, (long)hcur); // 设置窗口类的鼠标样式
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
	for(int i=0;i<52;i++)						//存field
	{
		for (int j = 0; j < 52; j++)
		{
			outfile<<field[i][j]<<" ";
		}
		outfile<<endl;
	}
	outfile<<endl;
	for (int i = 0; i < 52; i++)				//存scaned
	{
		for (int j = 0; j < 52; j++)
		{
			outfile<<scaned[i][j]<<" ";
		}
		outfile<<endl;
	}
	outfile<<endl;
	for (int i = 0; i < 50; i++)				//存mine_number_map
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
	TCHAR pity[3][30]={_T("恭贺先生，又得一佳画"),_T("先生真乃天生妙手"),_T("晚生佩服")};
	LOGFONT choice;
	gettextstyle(&choice);                     // 获取当前字体设置
	choice.lfHeight = 48;                      // 设置字体高度为 48
	_tcscpy_s(choice.lfFaceName, _T("楷体"));    // 设置字体为“楷体”(高版本 VC 推荐使用 _tcscpy_s 函数)
	choice.lfQuality = ANTIALIASED_QUALITY;    // 设置输出效果为抗锯齿  
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
	loadimage(&bk,L"res\\图片\\开始背景.jpg",970,630,false);
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
	loadimage(&hole,L"res\\图片\\洞.gif",width,height);
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
	TCHAR pity[3][30]={_T("可惜！可叹！"),_T("先生一时疏忽"),_T("毁了一幅好画")};
	LOGFONT choice;
	gettextstyle(&choice);                     // 获取当前字体设置
	choice.lfHeight = 48;						// 设置字体高度为 48
	_tcscpy_s(choice.lfFaceName, _T("楷体"));    // 设置字体为“楷体”(高版本 VC 推荐使用 _tcscpy_s 函数)
	choice.lfQuality = ANTIALIASED_QUALITY;    // 设置输出效果为抗锯齿  
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
				mine_number_map[i][j]=-1;				//地图映射上-1表示有雷
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
////////////////////////////////////////////////////////////////////////菜单函数和开始结束效果函数
void seeyoueffect()
{
	loadimage(NULL,L"res\\图片\\开始背景.jpg",970,630,false);
	TCHAR saybye[3][30]={_T("先生此次云游"),_T("又不知几时得归"),_T("就此别过，先生走好")};
	LOGFONT choice;
	gettextstyle(&choice);                     // 获取当前字体设置
	choice.lfHeight = 48;                      // 设置字体高度为 48
	_tcscpy_s(choice.lfFaceName, _T("楷体"));    // 设置字体为“楷体”(高版本 VC 推荐使用 _tcscpy_s 函数)
	choice.lfQuality = ANTIALIASED_QUALITY;    // 设置输出效果为抗锯齿  
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
	loadimage(NULL,L"res\\图片\\开始背景.jpg",970,630,false);
	TCHAR greet[3][30]={_T("先生远游归来，不及相迎"),_T("先生此来宜先留名号，"),_T("以免名画无主")};
	LOGFONT choice;
	gettextstyle(&choice);                     // 获取当前字体设置
	choice.lfHeight = 48;                      // 设置字体高度为 48
	_tcscpy_s(choice.lfFaceName, _T("楷体"));    // 设置字体为“楷体”(高版本 VC 推荐使用 _tcscpy_s 函数)
	choice.lfQuality = ANTIALIASED_QUALITY;    // 设置输出效果为抗锯齿  
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
	loadimage(&choiceframe,L"res\\图片\\选项卡底.jpg",400,60,false);
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
	loadimage(NULL,L"res\\图片\\开始背景.jpg",970,630,false);
	TCHAR greet[2][30]={_T("先生远来，不及相迎"),_T("敢问先生芳名")};
	LOGFONT choice;
	gettextstyle(&choice);                     // 获取当前字体设置
	choice.lfHeight = 48;                      // 设置字体高度为 48
	_tcscpy_s(choice.lfFaceName, _T("楷体"));    // 设置字体为“楷体”(高版本 VC 推荐使用 _tcscpy_s 函数)
	choice.lfQuality = ANTIALIASED_QUALITY;    // 设置输出效果为抗锯齿  
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
	loadimage(&choiceframe,L"res\\图片\\选项卡底.jpg",400,60,false);
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
	loadimage(NULL,L"res\\图片\\开始背景.jpg",970,630,false);
	loadimage(&choicehead,L"res\\图片\\选项头图片.jpg",50,50,false);
	loadimage(&choiceframe,L"res\\图片\\选项卡底.jpg",210,60,false);
	putimage(200,210,&choicehead,SRCAND);
	putimage(110,300,&choicehead,SRCAND);
	putimage(50,400,&choicehead,SRCAND);
	putimage(250,220,&choiceframe,SRCAND);
	putimage(160,310,&choiceframe,SRCAND);
	putimage(100,410,&choiceframe,SRCAND);
	LOGFONT choice;
	gettextstyle(&choice);                     // 获取当前字体设置
	choice.lfHeight = 48;                      // 设置字体高度为 48
	_tcscpy_s(choice.lfFaceName, _T("楷体"));    // 设置字体为“楷体”(高版本 VC 推荐使用 _tcscpy_s 函数)
	choice.lfQuality = ANTIALIASED_QUALITY;    // 设置输出效果为抗锯齿  
	settextstyle(&choice);  // 
	setbkmode(TRANSPARENT);
	settextcolor(BLACK);
	outtextxy(265,205,_T("新画卷"));
	outtextxy(175,295,_T("先生遗迹"));
	outtextxy(115,395,_T("此间何人"));
	MOUSEMSG m;
	bool trace =0;//看是否进入过newgame（）
	FlushMouseMsgBuffer();
	for(bool main_menu_flag=1;main_menu_flag;)
	{
		m=GetMouseMsg();
		if((175<m.x&&m.x<370&&295<m.y&&m.y<345)||(115<m.x&&m.x<310&&395<m.y&&m.y<445)||(265<m.x&&m.x<410&&205<m.y&&m.y<255))
		{
			hcur = LoadCursor(NULL, IDC_HAND); // 加载系统预置的鼠标样式
			SetClassLong(hwnd, GCL_HCURSOR, (long)hcur); // 设置窗口类的鼠标样式
		}else
		{
			hcur = LoadCursor(NULL, IDC_ARROW); // 加载系统预置的鼠标样式
			SetClassLong(hwnd, GCL_HCURSOR, (long)hcur); // 设置窗口类的鼠标样式
		}
		if(trace)
		{
			loadimage(NULL,L"res\\图片\\开始背景.jpg",970,630,false);
			putimage(200,210,&choicehead,SRCAND);
			putimage(110,300,&choicehead,SRCAND);
			putimage(50,400,&choicehead,SRCAND);
			putimage(250,220,&choiceframe,SRCAND);
			putimage(160,310,&choiceframe,SRCAND);
			putimage(100,410,&choiceframe,SRCAND);
			LOGFONT choice;
			gettextstyle(&choice);                     // 获取当前字体设置
			choice.lfHeight = 48;                      // 设置字体高度为 48
			_tcscpy_s(choice.lfFaceName, _T("楷体"));    // 设置字体为“楷体”(高版本 VC 推荐使用 _tcscpy_s 函数)
			choice.lfQuality = ANTIALIASED_QUALITY;    // 设置输出效果为抗锯齿  
			settextstyle(&choice);  // 
			setbkmode(TRANSPARENT);
			settextcolor(BLACK);
			outtextxy(265,205,_T("新画卷"));
			outtextxy(175,295,_T("先生遗迹"));
			outtextxy(115,395,_T("此间何人"));
			trace=0;
		}
		switch (m.uMsg)
		{
			case WM_LBUTTONDOWN:if(175<m.x&&m.x<370&&295<m.y&&m.y<345)//先生遗迹
								{	
									main_menu_flag=loadrecord();
									trace=0;
									if(main_menu_flag==0)
										replaychoice=1;
								}
								if(115<m.x&&m.x<310&&395<m.y&&m.y<445)//此间何人
								{	 attribute(); trace=0;}
								if(265<m.x&&m.x<410&&205<m.y&&m.y<255)//新画卷
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
	hcur = LoadCursor(NULL, IDC_ARROW); // 加载系统预置的鼠标样式
	SetClassLong(hwnd, GCL_HCURSOR, (long)hcur); // 设置窗口类的鼠标样式
}
bool selfdefine()
{
	IMAGE choicehead(50,50),choiceframe(140,60);	
	loadimage(NULL,L"res\\图片\\开始背景.jpg",970,630,false);
	loadimage(&choicehead,L"res\\图片\\选项头图片.jpg",50,50,false);
	loadimage(&choiceframe,L"res\\图片\\选项卡底.jpg",140,60,false);
	putimage(200,210,&choicehead,SRCAND);
	putimage(110,300,&choicehead,SRCAND);
	putimage(50,400,&choicehead,SRCAND);
	putimage(250,220,&choiceframe,SRCAND);
	putimage(160,310,&choiceframe,SRCAND);
	putimage(100,410,&choiceframe,SRCAND);
	LOGFONT choice;
	gettextstyle(&choice);                     // 获取当前字体设置
	choice.lfHeight = 48;                      // 设置字体高度为 48
	_tcscpy_s(choice.lfFaceName, _T("楷体"));    // 设置字体为“楷体”(高版本 VC 推荐使用 _tcscpy_s 函数)
	choice.lfQuality = ANTIALIASED_QUALITY;    // 设置输出效果为抗锯齿  
	settextstyle(&choice);  // 
	setbkmode(TRANSPARENT);
	settextcolor(BLACK);
	outtextxy(405,205,_T("行"));
	outtextxy(315,295,_T("列"));
	outtextxy(255,395,_T("黏结点"));
	TCHAR hang[3]={0},lie[3]={0},dotnumber[5]={0};
	IMAGE hangimg,lieimg,dotnumberimg;
	getimage(&hangimg,250,200,140,60);
	getimage(&lieimg,160,290,140,60);
	getimage(&dotnumberimg,100,390,140,60);
	int key,counter=0;
	fflush(stdin);
	for(bool hang_flag=1;hang_flag;)		//获得行值
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
	for(bool lie_flag=1;lie_flag;)		//获得列值
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
	for(bool dot_flag=1;dot_flag;)							//获得"雷"值
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
	loadimage(NULL,L"res\\图片\\开始背景.jpg",970,630,false);
	loadimage(&choicehead,L"res\\图片\\选项头图片.jpg",50,50,false);
	loadimage(&choiceframe,L"res\\图片\\选项卡底.jpg",210,60,false);
	putimage(320,120,&choicehead,SRCAND);
	putimage(200,210,&choicehead,SRCAND);
	putimage(110,300,&choicehead,SRCAND);
	putimage(50,400,&choicehead,SRCAND);
	putimage(370,130,&choiceframe,SRCAND);
	putimage(250,220,&choiceframe,SRCAND);
	putimage(160,310,&choiceframe,SRCAND);
	putimage(100,410,&choiceframe,SRCAND);
	LOGFONT choice;
	gettextstyle(&choice);                     // 获取当前字体设置
	choice.lfHeight = 48;                      // 设置字体高度为 48
	_tcscpy_s(choice.lfFaceName, _T("楷体"));    // 设置字体为“楷体”(高版本 VC 推荐使用 _tcscpy_s 函数)
	choice.lfQuality = ANTIALIASED_QUALITY;    // 设置输出效果为抗锯齿  
	settextstyle(&choice);  // 
	setbkmode(TRANSPARENT);
	settextcolor(BLACK);
	outtextxy(385,115,_T("小露功力"));
	outtextxy(265,205,_T("风范初显"));
	outtextxy(175, 295, _T("先生独创"));
	outtextxy(130,395,_T("且 慢"));
	MOUSEMSG m;
	FlushMouseMsgBuffer();
	while (true)
	{
		m=GetMouseMsg();
		if((385<m.x&&m.x<480&&115<m.y&&m.y<165)||(265<m.x&&m.x<460&&205<m.y&&m.y<255)||(175<m.x&&m.x<280&&295<m.y&&m.y<345)||(130<m.x&&m.x<250&&395<m.y&&m.y<445))
		{
			hcur = LoadCursor(NULL, IDC_HAND); // 加载系统预置的鼠标样式
			SetClassLong(hwnd, GCL_HCURSOR, (long)hcur); // 设置窗口类的鼠标样式
		}else
		{
			hcur = LoadCursor(NULL, IDC_ARROW); // 加载系统预置的鼠标样式
			SetClassLong(hwnd, GCL_HCURSOR, (long)hcur); // 设置窗口类的鼠标样式
		}
		switch (m.uMsg)
		{
		case WM_LBUTTONDOWN:if(385<m.x&&m.x<480&&115<m.y&&m.y<165)//第一排
							{
								hcur = LoadCursor(NULL, IDC_ARROW); // 加载系统预置的鼠标样式
								SetClassLong(hwnd, GCL_HCURSOR, (long)hcur); // 设置窗口类的鼠标样式
								return 1;
							}
							if(265<m.x&&m.x<460&&205<m.y&&m.y<255)//第二排
							{
								hcur = LoadCursor(NULL, IDC_ARROW); // 加载系统预置的鼠标样式
								SetClassLong(hwnd, GCL_HCURSOR, (long)hcur); // 设置窗口类的鼠标样式
								return 2;
							}
							if(175<m.x&&m.x<280&&295<m.y&&m.y<345)//第三排
							{
								hcur = LoadCursor(NULL, IDC_ARROW); // 加载系统预置的鼠标样式
								SetClassLong(hwnd, GCL_HCURSOR, (long)hcur); // 设置窗口类的鼠标样式
								return 3;
							}
							if(130<m.x&&m.x<250&&395<m.y&&m.y<445)//第四排
							{
								hcur = LoadCursor(NULL, IDC_ARROW); // 加载系统预置的鼠标样式
								SetClassLong(hwnd, GCL_HCURSOR, (long)hcur); // 设置窗口类的鼠标样式
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
		TCHAR version[2][30]={_T("先生初次到访"),
							  _T("还未曾留下未完成的作品")};
		LOGFONT choice;
		gettextstyle(&choice);                     // 获取当前字体设置
		choice.lfHeight = 35;                      // 设置字体高度为 48
		_tcscpy_s(choice.lfFaceName, _T("楷体"));    // 设置字体为“楷体”(高版本 VC 推荐使用 _tcscpy_s 函数)
		choice.lfQuality = ANTIALIASED_QUALITY;    // 设置输出效果为抗锯齿  
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
	TCHAR version[5][30]={_T("游戏名：修复旧画"),
						  _T("制作：曹书阳"),
						  _T("发布时间："),
						  _T("二零一五年一月一日"),
						  _T("版本号：三点零")};
	LOGFONT choice;
	gettextstyle(&choice);                     // 获取当前字体设置
	choice.lfHeight = 35;                      // 设置字体高度为 48
	_tcscpy_s(choice.lfFaceName, _T("楷体"));    // 设置字体为“楷体”(高版本 VC 推荐使用 _tcscpy_s 函数)
	choice.lfQuality = ANTIALIASED_QUALITY;    // 设置输出效果为抗锯齿  
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
	loadimage(NULL,L"res\\图片\\开始背景.jpg",970,630,false);
	IMAGE choicehead(50,50),choiceframe(140,60);
	loadimage(&choicehead,L"res\\图片\\选项头图片.jpg",50,50,false);
	loadimage(&choiceframe,L"res\\图片\\选项卡底.jpg",140,60,false);
	putimage(110,300,&choicehead,SRCAND);
	putimage(50,400,&choicehead,SRCAND);
	putimage(160,310,&choiceframe,SRCAND);
	putimage(100,410,&choiceframe,SRCAND);
	LOGFONT choice;
	gettextstyle(&choice);                     // 获取当前字体设置
	choice.lfHeight = 48;                      // 设置字体高度为 48
	_tcscpy_s(choice.lfFaceName, _T("楷体"));    // 设置字体为“楷体”(高版本 VC 推荐使用 _tcscpy_s 函数)
	choice.lfQuality = ANTIALIASED_QUALITY;    // 设置输出效果为抗锯齿  
	settextstyle(&choice);  // 
	setbkmode(TRANSPARENT);
	settextcolor(BLACK);
	outtextxy(175, 295, _T("初访"));
	outtextxy(115,395,_T("归来"));
	MOUSEMSG m;
	FlushMouseMsgBuffer();
	while (true)
	{
		m=GetMouseMsg();
		if((175<m.x&&m.x<280&&295<m.y&&m.y<345)||(115<m.x&&m.x<220&&395<m.y&&m.y<445))
		{
			hcur = LoadCursor(NULL, IDC_HAND); // 加载系统预置的鼠标样式
			SetClassLong(hwnd, GCL_HCURSOR, (long)hcur); // 设置窗口类的鼠标样式
		}else
		{
			hcur = LoadCursor(NULL, IDC_ARROW); // 加载系统预置的鼠标样式
			SetClassLong(hwnd, GCL_HCURSOR, (long)hcur); // 设置窗口类的鼠标样式
		}

		switch (m.uMsg)
		{
			case WM_LBUTTONDOWN:if(175<m.x&&m.x<280&&295<m.y&&m.y<345)
								{
									hcur = LoadCursor(NULL, IDC_ARROW); // 加载系统预置的鼠标样式
									SetClassLong(hwnd, GCL_HCURSOR, (long)hcur); // 设置窗口类的鼠标样式
									return 0;
								}
								if(115<m.x&&m.x<220&&395<m.y&&m.y<445)
								{
									hcur = LoadCursor(NULL, IDC_ARROW); // 加载系统预置的鼠标样式
									SetClassLong(hwnd, GCL_HCURSOR, (long)hcur); // 设置窗口类的鼠标样式
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
	TCHAR s[10][100] ={ _T(" 欢迎来到这个游戏"),
						_T(" 在这个游戏中，你将成为一名出色的图画修复师"),
						_T(" 拂去那些图画上历史的尘埃"),
						_T(" 现在就开始吧，开始你伟大的工作"),
						_T(" ......"),
						_T(" （按空格键继续）")};
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

