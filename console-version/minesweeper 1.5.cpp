/*
gamename:minesweeper
creater:Cao Shuyang
release date:2014 12 21
version number: 1.5
*/ 
#include<iostream>
#include<string.h>
#include<conio.h>
#include<ctime>
#include<cstdlib>
#include<windows.h>
#include<iomanip>
//global variable declaration
using namespace std;
int row=10,column=10,mine_number=10, key, x, y, xrow, ycolumn, operation;
int numberofmine=mine_number;
bool choice=1,language=0;
bool first_click=1;
bool field[52][52]={0};
int scaned[52][52]={0},	mine_number_map[50][50]={0};
char user_name[100];
COORD pos={0,1};
HANDLE hOut=GetStdHandle(STD_OUTPUT_HANDLE);
//function in main declaration
bool help();
bool levelchoice(bool &menuflag);
void menu();
void game();
bool languagechoice();
bool doublechoice(int x,int y,int dis);
int multichoice(int choice_number,int x,int y);
void welcome();
int main()
{
	cout<<"Select your language"<<endl;
	cout<<"⊕English ⊕中文"<<endl;
	language=doublechoice(0,1,10);
	welcome();
	menu();
	for(bool main_flag=1;main_flag;)
	{
		game();
		system("cls");
		switch(language)
		{
			case 0: cout<<"Play again?"<<endl;
					cout<<"⊕Yes ⊕No"<<endl;break;
			case 1: cout<<"再来一盘？"<<endl;
					cout<<"⊕是  ⊕否"<<endl;break;
			default :break;
		}
		bool replay_choice=doublechoice(0,1,7);
		if(replay_choice)
		{
			system("cls");
			switch(language)
			{
				case 0: cout<<user_name<<", see you next time!"<<endl;break;
				case 1: cout<<user_name<<"，下次见！"<<endl;break;
			}
			cout<<"( ^_^ )/~~"<<endl;
			main_flag=0;
		}else
		{
			switch(language)
			{
				case 0: cout<<"⊕reuse last field ⊕a new field"<<endl;break;
				case 1: cout<<"⊕重玩上次的地图   ⊕新地图"<<endl;break;;
			}
			choice=doublechoice(0,1,19);
			switch(choice)
			{
				case 1: menu();
						break;
				case 0: break;
			}
		}
	}
	system("pause");
	return 0;
}
//menu function
bool file(bool &menu_flag);
bool levelchoice(bool &menu_flag);
bool help();
void menu()
{
	for(bool menu_flag=1;menu_flag;)
	{
		system("cls");
		int menu_choice;
		for(bool flag=1;flag;)
		{
			system("cls");
			switch(language)
			{
				case 0:	cout<<"Menu: ⊕File"<<endl
					    	<<"      ⊕Help"<<endl
							<<"      ⊕Start"<<endl;
						cout<<"(To return to the previous menu(submenu), press the key \"Esc\")"<<endl;
						break;
				case 1: cout<<"菜单: ⊕文件"<<endl
							<<"      ⊕帮助"<<endl
							<<"      ⊕开始"<<endl;
						cout<<"（返回上级菜单（子菜单），按”Esc“键）"<<endl;
						break;
			}
			menu_choice=multichoice(3,7,0);
			flag=!bool(menu_choice);
		}
		pos={0,3};
		SetConsoleCursorPosition(hOut,pos);
		switch(menu_choice)
		{
			case 1:menu_flag=!file(menu_flag);break;
			case 2:help();break;
			case 3:menu_flag=!levelchoice(menu_flag);break;
			default :break;
		}
	}	
}
bool help()
{
	switch(language)
	{
		case 0: cout<<"1) use the arrow buttons on your keyboard to move the cursor on your screen"<<endl
					<<"   so that you can decide which box to choose."<<endl;
				cout<<"2) use the space bar to open a box, if you are sure there is no mine behind it,"<<endl
					<<"   wish you good luck!"<<endl;
				cout<<"3) use key-\"Enter\"to mark a mine."<<endl;
				cout<<"Wish you enjoy the game!"<<endl;
				cout<<"OK";
				break;
		case 1: cout<<"1) 用键盘上的方向键移动屏幕上的光标到你想点开的位置"<<endl;
				cout<<"2) 如果你确定光标所在的位置没有地雷，按空格键打开盒子"<<endl
					<<"   祝你好运！"<<endl; 
				cout<<"3) 按回车键标记地雷"<<endl;
				cout<<"希望你玩得愉快！"<<endl;
				cout<<"确定";
				break; 
	}
	getch();
	return 0;
}
bool levelchoice(bool &menu_flag)
{
	system("cls");
	switch(language)
	{
		case 0: cout<<"Which level do you want?"<<endl
					<<"     area   mine's number"<<endl
					<<"⊕:   5*5       3"<<endl
					<<"⊕: 10*10      10"<<endl
					<<"⊕: self-defined"<<endl;
				break;
		case 1: cout<<"难度选择"<<endl
					<<"    雷区大小 地雷个数"<<endl
					<<"⊕:   5*5       3"<<endl
					<<"⊕: 10*10      10"<<endl
					<<"⊕: 自定义"<<endl;
				break;
	}
	int choice=multichoice(3,0,2);
	switch(choice)
	{
		case 0: return 0;
		case 1: row=column=5;mine_number=3;menu_flag=0;return 1;
		case 2: row=column=10;mine_number=10;return 1;
		case 3: pos={0,5};
				SetConsoleCursorPosition(hOut,pos);
				switch(language)
				{
					case 0: cout<<"Row: ";cin>>row;fflush(stdin);;
							cout<<"Column(<19): ";cin>>column;fflush(stdin);
							cout<<"Number of mine(s): ";cin>>mine_number;fflush(stdin);
							break;
					case 1: cout<<"行数：";cin>>row;fflush(stdin);
							cout<<"列数(<19)：";cin>>column;fflush(stdin);
							cout<<"地雷数：";cin>>mine_number;fflush(stdin);
							break;
				}
				menu_flag=0;
				return 1;
	}
}
bool file(bool &menu_flag)
{
	while(1)
	{
		system("cls");
		switch(language)
		{
			case 0: cout<<"File: ⊕Level"<<endl
					    <<"      ⊕Attribute"<<endl;
					break;
			case 1: cout<<"文件: ⊕难度"<<endl
					    <<"      ⊕属性"<<endl;
					break; 
		}
		int file_choice=multichoice(2,7,0);
		switch(file_choice)
		{
			case 2: pos={0,3};
					SetConsoleCursorPosition(hOut,pos);
					switch(language)
					{ 
						case 0: cout<<"gamename:       minesweeper"<<endl
					 	 		    <<"creater:        Cao Shuyang"<<endl
						 		    <<"release date:   2014 12 21"<<endl
						 		    <<"version number: 1.5"<<endl;
						 		cout<<"OK";
					 			break;	   
					 	case 1: cout<<"游戏名称：扫雷"<<endl
					 				<<"制作者：  曹书阳"<<endl
					 				<<"发行时间：2014 12 21"<<endl
					 				<<"版本号：  1.5"<<endl;
					 			cout<<"确定"; 
								break;
					}
					getch();
					break;		 
			case 1: if(levelchoice(menu_flag))
						return 1;
					else
						break; 
			case 0: return 0;
			default :break;
		}
	}
}
void welcome()
{
	system("cls");
	switch(language)
	{
		case 0:cout<<"Please create a name for yourself."<<endl;break;
		case 1:cout<<"为自己起个名字吧。"<<endl;break;
		default :break;
	}
//	char user_name[100];
	cin>>user_name;
	fflush(stdin);
	switch(language)
	{
		case 0:cout<<user_name<<", welcome to minesweeper!"<<endl;break;
		case 1:cout<<user_name<<"，欢迎来到扫雷世界。"<<endl;break;	
	}
	cout<<"           /  \\     ,    ,"<<endl;
	cout<<"  _._    _ |oo| _  / \\__/ \\"<<endl;
	cout<<" _||||  ((/ () \\))   /  \\"<<endl;
	cout<<" |||||/| ( ==== )    |oo| "<<endl;   
	cout<<" \\____/  _`\\  /'_    /  \\"<<endl;
	cout<<" /   / .-' /\\<>/\\`\\.( () )_._ "<<endl;     
	cout<<" |    `  /  \\/  \\  /`'--'////)"<<endl;    
	cout<<"  \\__,-'`|  |.  |\\/ |/\\/\\|\"\\\"` "<<endl;
	cout<<"         |  |.  | \\___/\\___/ "<<endl;  
	cout<<"         |  |.  |   |    |"<<endl;
	switch(language)
	{
		case 0: cout<<"Go to acquire your task and fulfill it."<<endl;
				cout<<"⊕OK!"<<endl;break;
		case 1: cout<<"开始你的任务吧。"<<endl;
				cout<<"⊕是！"<<endl;break;
		default :break;
	}
	pos={0,14};
	SetConsoleCursorPosition(hOut,pos);
	getch();	
}
int multichoice(int choice_number,int x,int y)
{
	pos={x,y};
	SetConsoleCursorPosition(hOut,pos);
	while(1)
	{
		key=getch();
		switch(key)
		{
			case 72:if(pos.Y==y)
						pos.Y=y+choice_number-1;
					else
						pos.Y--;
					SetConsoleCursorPosition(hOut,pos);
					break;
			case 80:if(pos.Y==y+choice_number-1)
						pos.Y=y;
					else
						pos.Y++;
					SetConsoleCursorPosition(hOut,pos);
					break;
			case 32:return pos.Y-y+1;
			case 27:return 0;
			default :break;	
		}
	}
}
bool doublechoice(int x,int y,int dis)
{
	pos={x,y};
	SetConsoleCursorPosition(hOut,pos);
	while(1)
	{
		key=getch();
		switch(key)
		{
			case 75:if(pos.X==0)
						pos.X=dis;
					else
						pos.X=0;
					SetConsoleCursorPosition(hOut,pos);
					break;
			case 77:if(pos.X==dis)
						pos.X=0;
					else
						pos.X=dis;
					SetConsoleCursorPosition(hOut,pos);
					break;
			case 32:if(pos.X==0)
						return 0;
					else 
						return 1;	
					break;
			default:break;	
		}
	}
}
//game function
int move_and_click();
void scan_map(const int x,const int y);
void initialize_field();
void printfield();
bool win(clock_t &start);
void game()
{
	numberofmine=mine_number;
	system("cls");
	memset(scaned,0,sizeof(scaned));
	for(int i=0;i<column+2;i++)				//initialize the margin of array scaned
	{
			scaned[0][i]=1;
			scaned[row+1][i]=1;
	}
	for(int i=0;i<row+2;i++)
	{
		scaned[i][0]=1;
		scaned[i][column+1]=1;
	}
	if(choice) 
	{
		memset(field,0,sizeof(field));
		memset(mine_number_map,0,sizeof(field));
	}
	clock_t start=clock();
	first_click=1;
	x=1;
	y=1;
	for(bool game_flag=1;game_flag;)
	{
		COORD pos1={0,0};
		SetConsoleCursorPosition(hOut,pos1);
//		system("cls");
		switch(language)
		{
			case 0: cout<<"Keep fighting!"<<endl;break;
			case 1: cout<<"加油！"<<endl;break;
		}
		if(first_click)
		{
			while(first_click)
			{
				COORD pos2={0,1};
				SetConsoleCursorPosition(hOut,pos2);
				printfield();
				pos={2,2};
				SetConsoleCursorPosition(hOut,pos);
				if(choice)
				{
					operation=move_and_click();
					xrow=pos.Y/2;
					ycolumn=(pos.X+2)/4;
					switch(operation)
					{
						case 32:scaned[xrow][ycolumn]=1;break;
						case 13:scaned[xrow][ycolumn]=2;break;
					}
					if(operation==32)
					{
						initialize_field();
						scan_map(xrow,ycolumn);
						first_click=0;
						game_flag=win(start);
					}
				}
				else
					first_click=0;
			}
		}
		else
		{
			printfield();
			SetConsoleCursorPosition(hOut,pos);
			operation=move_and_click();
			game_flag=win(start);
		}
	}
}
bool win(clock_t &start)
{
	switch(operation)
	{
		case 32:if(field[y][x])
				{
					for(int i=1;i<51;i++)					//mark array scaned with "1" if there is a mine there
						for(int j=1;j<51;j++)
							scaned[i][j]+=field[i][j];
					system("cls");
					printfield();
					switch(language)
					{
						case 0: cout<<"Sorry, you lose."<<endl;
								cout<<"I know";break;
						case 1: cout<<"很遗憾，你失败了。"<<endl;
								cout<<"确定";break;
					}
					getch(); 
					return 0;
				}else
				{
					scan_map(y,x);
					int blackbox=0;
					for(int i=1;i<row+1;i++)
						for(int j=1;j<column+1;j++)
							//blackbox+=!scaned[i][j];
						{
							if(scaned[i][j]!=1)
							blackbox++;
						}
					if(blackbox==mine_number)
					{
						system("cls");
						switch(language)
						{
							case 0: cout<<"congratulations! You win!"<<endl;
										cout<<"You clear all mines in "<<(clock()-start)/60000<<" minute";
									if((clock()-start)/60>1)
									cout<<"s";
									cout<<" "<<((clock()-start)/1000)%60<<" second";
									if((clock()-start)%60>1)
										cout<<"s";
									cout<<"."; 
									break;
							case 1: cout<<"你赢了，恭喜你！"<<endl;
									cout<<"你在"<<(clock()-start)/60000<<"分钟";
									cout<<((clock()-start)/1000)%60<<"秒内扫清了所有的地雷。";
									break;
						}
						cout<<endl<<endl;
						cout<<"  .\"\".    .\"\","<<endl;
						cout<<"  |  |   /  /"<<endl;
						cout<<"  |  |  /  /"<<endl;
						cout<<"  |  | /  /"<<endl;
						cout<<"  |  |/  ;-._ "<<endl;
						cout<<"  }  ` _/  / ;"<<endl;
						cout<<"  |  /` ) /  /"<<endl;
						cout<<"  | /  /_/\\_/\\"<<endl;
						cout<<"  |/  /      |"<<endl;
						cout<<"  (  ' \\ '-  |"<<endl;
						cout<<"   \\    `.  /"<<endl;
						cout<<"    |      |"<<endl;
		    			cout<<"    |      |"<<endl;
		    			switch(language)
		    			{
		    				case 0: cout<<"I know.";break;
		    				case 1: cout<<"我知道了";break; 
						}
						getch();
						return 0;
					}else
						return 1;
				}
//				return 0;
				break;
		case 13:if(scaned[y][x]==0)
				{
					scaned[y][x]=2;
					numberofmine--;
				}
				else
				{
					scaned[y][x]=0;
					numberofmine++;
				}
				return 1;
				break;
		default :return 1;break;
	}
}
void scan_map(const int x,const int y)			//judge user's choice and give three results
{
	if(scaned[x][y]==0||(first_click==1&&field[x][y]==0))
	{
		scaned[x][y]=1;
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
				mine_number_map[i][j]=-1;
			else
			{
				mine_number_map[i][j]=field[i][j+1]+field[i+2][j+1]+field[i+1][j]+field[i+1][j+2];
				mine_number_map[i][j]+=field[i][j]+field[i][j+2]+field[i+2][j]+field[i+2][j+2];
			}
		}
	}
}
void printfield()
{
	cout<<"┌";
	for(int i=0;i<column-1;i++)
		cout<<"─┬";
	cout<<"─┐"<<endl;
	for(int j=0;j<row-1;j++)
	{
		cout<<"│";
		for(int i=0;i<column;i++)
		{
			if(scaned[j+1][i+1]==0)
				cout<<"█"<<"│";
			else if(scaned[j+1][i+1]==2)
				cout<<"♂"<<"│";
			else if(mine_number_map[j][i]==0)
					cout<<"　"<<"│";
			else if(mine_number_map[j][i]==-1)
				cout<<"※"<<"│";
			else
				cout<<mine_number_map[j][i]<<" "<<"│";
		}
		cout<<endl<<"├";
		for(int i=0;i<column-1;i++)
			cout<<"─┼";
		cout<<"─┤"<<endl;
	}
	cout<<"│";
	for(int i=0;i<column;i++)
	{
		if(scaned[row][i+1]==0)
			cout<<"█"<<"│";
		else if(scaned[row][i+1]==2)
				cout<<"♂"<<"│";
		else if(mine_number_map[row-1][i]==0)
				cout<<"　"<<"│";
		else if(mine_number_map[row-1][i]==-1)
			cout<<"※"<<"│";
		else
			cout<<mine_number_map[row-1][i]<<" "<<"│";

	}
	cout<<endl<<"└";
	for(int i=0;i<column-1;i++)
		cout<<"─┴";
	cout<<"─┘"<<endl;
	switch(language)
	{
		case 0: cout<<"The number of left mine(s): ";
				cout<<right<<setw(2)<<numberofmine<<endl;break;
		case 1: cout<<"剩余雷数：";
				cout<<right<<setw(2)<<numberofmine<<endl;break; 
	}
}
int move_and_click()
{
	while(1)
	{
		key=getch();
		switch(key)
		{
			case 72:if(y==1)						//up
					{
						pos.Y=2*row;
						y=row;
					}
					else
					{
						y--; 
						pos.Y-=2;
					} 
					SetConsoleCursorPosition(hOut,pos);
					break;
			case 80:if(y==row)					//down
					{
						pos.Y=2;
						y=1;
					}
					else
					{
						pos.Y+=2;
						y++;
					}
					SetConsoleCursorPosition(hOut,pos);
					break;
			case 75:if(x==1)						//left
					{
						pos.X=4*column-2;
						x=column;
					}
					else
					{
						pos.X-=4;
						x--;
					}
					SetConsoleCursorPosition(hOut,pos);
					break;
			case 77:if(x==column)					//right
					{						
						pos.X=2;
						x=1;
					}
					else
					{
						pos.X+=4;
						x++;
					}
					SetConsoleCursorPosition(hOut,pos);
					break;
			case 32:if(scaned[y][x]==0)
						return 32;
					else
						break;
			case 13:if(scaned[y][x]==1)
						break;
					else
						return 13;
			default :break;
		}
	}
}





 