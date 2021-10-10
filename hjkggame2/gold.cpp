#include<stdio.h>
#include<math.h>
#include<graphics.h>//easyx ����ͷ�ļ�������ʹ���ṩ�Ľӿڣ�����
#define WIN_WIDTH 1080
#define WIN_HEIGHT 640
#define MAX_IMAGE 19 //���ͼƬ����
#define MAX_MINE 10   //�����Ʒ����
#define MAX_ANGLE 80 //���Ƕ�
#define PI 3.1415926535898
int gamestage = 0;
int kaishi_x = 120;
int kaishi_y = 155;
int goldnumingame1 = 2;
int exist_of_boom = 1;
int have_boom = 0;
int boom_price =rand() % 50 + 20;
double end_word_size = 0;
int boom_use = 0;
int boom_using = 0;
//int a,char b
enum MINE //ö��ͼƬ�����±�
{
	i_gold = 1,
	i_money = 3,
	i_soledown = 5,
	i_soleup = 7,
	i_stone = 9,
	bkfirst = 10,
	i_bk = MAX_IMAGE - 1,
};
enum MINE_TYPE //��Ʒ����ö��
{
	GOLD,
	MONEY,
	STONE,
	//����
	LEFT,
	RIGHT,
	//״̬���������쳤������
	M_NOMAL,
	M_LONG,
	M_SHORT,
};
struct BOOM
{
	int x = 850;
	int y = 20;
}boom;
struct Sole //��ɫ��Ϣ
{
	int x;//��ͼλ��
	int y;
	int width;//ͼƬ���
	int height;
	int coin=0;//���
}sole;
struct Mine //��Ʒ
{
	int x;
	int y;
	int szie; //�߽��ж�
	bool flag;//�Ƿ����
	int type;//��Ʒ���ͣ�ʯͷ����飬Ǯ��
	int gold;//��Ʒ��ֵ
}mine[MAX_MINE];
struct Hook //����
{
	int x;
	int y;
	double endx;//Բ�ϵ�����
	double endy;//Բ�ϵ�����
	double len;//����
	double angle;//�Ƕ�
	int dir;//�ڶ�����
	double vx;//�ٶȷ���
	double vy;
	int state;//�����Ƿ������쳤
	int index;//��¼ץ������Ʒ�±�
}hook;
IMAGE img[MAX_IMAGE];
int distance(Hook hook);
//��ʼ������
void GameInit()
{
	//�������� init ��ʼ��  graphicsͼ�ν���
	initgraph(WIN_WIDTH, WIN_HEIGHT, SHOWCONSOLE);
	//�������������
	srand(GetTickCount());
	//����ͼƬ
	for (int i = 0; i < MAX_IMAGE - 1; i++)
	{
		char temp[40] = "";
		sprintf(temp, "./images/%d.jpg", i);
		if (i <= 1)
		{
			loadimage(&img[i], temp, 73, 62);//ע�⣺�ַ�������
		}
		else 
		{
			loadimage(&img[i], temp);//ע�⣺�ַ�������
		}
	

	}
	loadimage(&img[i_bk], "./images/bk.jpg", WIN_WIDTH, WIN_HEIGHT - 120);
	//��ʼ����ɫ��Ϣ
	sole.width = 140;
	sole.height = 120;
	sole.x = WIN_WIDTH / 2 - sole.width / 2;//x�����
	sole.y = 0;
	boom_using = 0;
	while (true)
	{
		int goldnum = 0;
		int stonenum = 0;
		//��ʼ����Ʒ��Ϣ


		for (int i = 0; i < MAX_MINE; i++)
		{
			mine[i].flag = true;

			mine[i].szie = 60;
			mine[i].type = rand() % 3;
			mine[i].x = rand() % (WIN_WIDTH - 60);
			mine[i].y = rand() % (WIN_HEIGHT - 150) + 150;
			if (mine[i].type == GOLD)
			{
				mine[i].gold = rand() % 100 + 100;
				goldnum++;
			}
			else if (mine[i].type == STONE)
			{
				mine[i].gold = 0;
				stonenum++;
			}
			else if (mine[i].type == MONEY)
			{
				mine[i].gold = rand() % 500 + 1;
			}
		}
		if ((goldnum == 2)&&(stonenum>=4))
			break;
	}

	
	//��ʼ������
	hook.x = sole.x + 70;
	hook.y = sole.y + 100;
	hook.len = 50;
	hook.endx = hook.x;
	hook.endy = hook.y + hook.len;
	hook.angle = 0;
	hook.dir = RIGHT;
	hook.vx = 0;
	hook.vy = 0;
	hook.state = M_NOMAL;
	hook.index = -1;
}
//��Ϸ����
void GameDraw()
{
	//���Ʊ���
	putimage(0, 120, &img[i_bk]);
	//���ƽ�ɫ������ɫ
	setfillcolor(RGB(255, 208, 52));
	solidrectangle(0, 0, WIN_WIDTH, 120);
	//���ƽ�ɫ
	putimage(sole.x, sole.y, &img[i_soledown - 1], SRCAND);//֪��Ϊɶ��1����
	putimage(sole.x, sole.y, &img[i_soledown], SRCPAINT);
	for (int i = 0; i < MAX_MINE; i++)
	{
		if (mine[i].flag == true)
		{

		}
	}
	//������Ʒ
	for (int i = 0; i < MAX_MINE; i++)
	{
		if (mine[i].flag == true)
		{
			switch (mine[i].type)
			{
			case GOLD:
				putimage(mine[i].x, mine[i].y, &img[i_gold - 1], SRCAND);
				putimage(mine[i].x, mine[i].y, &img[i_gold], SRCPAINT);
				break;
			case MONEY:
				putimage(mine[i].x, mine[i].y, &img[i_money - 1], SRCAND);
				putimage(mine[i].x, mine[i].y, &img[i_money], SRCPAINT);
				break;
			case STONE:
				putimage(mine[i].x, mine[i].y, &img[i_stone - 1], SRCAND);
				putimage(mine[i].x, mine[i].y, &img[i_stone], SRCPAINT);
				break;
			}
		}
	}
	//��ը��
	putimage(boom.x, boom.y, &img[15], NOTSRCERASE);
	putimage(boom.x, boom.y, &img[16], SRCINVERT);
	char scoreforboom[30] = "";
	sprintf(scoreforboom, "ը������%d", have_boom);
	settextcolor(WHITE);
	setbkmode(TRANSPARENT);
	settextstyle(20, 0, "����");
	outtextxy(900, 50, scoreforboom);
	//���ƹ���
	setlinecolor(BROWN);
	setlinestyle(PS_SOLID, 2);
	line(hook.x, hook.y, hook.endx, hook.endy);

	//���Ʒ���
	char score[30] = "";
	sprintf(score, "������%d", sole.coin);
	settextcolor(WHITE);
	setbkmode(TRANSPARENT);
	settextstyle(50, 0, "����");
	outtextxy(20, 20, score);
}
double lerp(double a,int b,int c)
{
	if(hook.angle>=0)return fabs((b - a) /( (double)b*5.0)) + 0.01;
	if (hook.angle <= 0)return fabs((c - a) / ((double)c * 5.0)) + 0.01;
}
//���Ӱڶ�
void HookRock()
{
	if (hook.state == M_NOMAL)
	{
		if (hook.dir == RIGHT)
		{
			hook.angle+=lerp(hook.angle,MAX_ANGLE,-MAX_ANGLE);
		}
		else
		{
			hook.angle-= lerp(hook.angle, MAX_ANGLE ,-MAX_ANGLE);
		}
		if (hook.angle > MAX_ANGLE)
		{
			hook.dir = LEFT;
		}
		else if (hook.angle < -MAX_ANGLE)
		{
			hook.dir = RIGHT;
		}
		//�Ƕ�ת����  360 2pai  180  PI  
		hook.endx = hook.x + sin(PI / 180 * hook.angle) * hook.len;
		hook.endy = hook.y + cos(PI / 180 * hook.angle) * hook.len;
	}

}

//��Ϸ����
void GameControl(double speed)
{
	if (GetAsyncKeyState(VK_DOWN) && hook.state == M_NOMAL)
	{
		//����������ٶȷ���
		hook.vx = sin(PI / 180 * hook.angle) * speed;
		hook.vy = cos(PI / 180 * hook.angle) * speed;
		hook.state = M_LONG;
	}
	//�����߽緵��
	if (hook.endx<0 || hook.endx>WIN_WIDTH || hook.endy > WIN_HEIGHT)
	{
		hook.state = M_SHORT;
	}
	if (hook.state == M_LONG)
	{
		hook.endx += hook.vx;
		hook.endy += hook.vy;
	}
	else if (hook.state == M_SHORT)
	{
		hook.endx -= hook.vx;
		hook.endy -= hook.vy;
		//�������
		if (distance(hook))
		{
			hook.state = M_NOMAL;
		}
	}
	//������������hook.endx ����hook.vx
}
void JudgeGrap()
{
	//�ж�ץ���Ǹ���Ʒ
	for (int i = 0; i < MAX_MINE; i++)
	{
		if (mine[i].flag == true &&
			hook.endx > mine[i].x && hook.endx<mine[i].x +(double) mine[i].szie &&
			hook.endy>mine[i].y && hook.endy < mine[i].y + (double)mine[i].szie)
		{
			hook.index = i;
			break;
		}
	}
	
	//���ץ����
	if (hook.index != -1)
	{
		hook.state = M_SHORT;
		mine[hook.index].x = hook.endx;
		mine[hook.index].y = hook.endy;
		//�������
		if (!distance(hook))
		{
			if (GetAsyncKeyState(VK_UP)&&have_boom>0)
			{
				have_boom--;

				boom_use = 1;
			}
		}
		if (boom_use == 1)
		{
			boom.x = hook.endx;
			boom.y = hook.endy;
		}
		if (boom_use == 1 && distance(hook))
		{
			boom.x = 850;
			boom.y = 20;
		}
		if (distance(hook))
		{
			mine[hook.index].flag = false;
			sole.coin += mine[hook.index].gold;
			if (mine[hook.index].type == GOLD)
			{
				goldnumingame1--;
			}
			if (mine[hook.index].type == STONE&&boom_use==0)
			{
				gamestage = 3;
			}
			hook.index = -1;
			hook.state = M_NOMAL;
			boom_use = 0;
		}
	}
}
int distance(Hook hook)
{
	int dis = sqrt((double)(hook.x - hook.endx) *(double)(hook.x - hook.endx) + (double)(hook.y - hook.endy) * (double)(hook.y - hook.endy));
	return dis <= hook.len;
}
void gamestart()
{
	if (gamestage == 0)
	{
		putimage(0, 0, &img[bkfirst]);//���س�ʼҳ��
	}
	MOUSEMSG m1;
	m1 = GetMouseMsg();
	BeginBatchDraw();
	if (m1.uMsg == WM_MOUSEMOVE&&gamestage==0)
	{
		if ((m1.x - kaishi_x )< 250 && (m1.y - kaishi_y) < 100 && gamestage == 0)
		{
			putimage(120, 155, &img[11]);
		}
		if (((m1.x - kaishi_x) >= 250 || (m1.y - kaishi_y) >= 100) && gamestage == 0)
		{
			putimage(120, 155, &img[12]);
		}
	}
	if (m1.uMsg == WM_LBUTTONDOWN)
	{
		if ((m1.x - kaishi_x) < 250 && (m1.y - kaishi_y) < 100)
		{
			gamestage = 1;
		}
	}
	FlushBatchDraw();

	Sleep(5);

}
void shop()
{
	while (1)
	{
		putimage(0, 0, &img[14]);
		MOUSEMSG m2;
		m2 = GetMouseMsg();
		BeginBatchDraw();
		char score[30] = "";
		sprintf(score, "������%d", sole.coin-boom_price);
		settextcolor(YELLOW);
		setbkmode(TRANSPARENT);
		settextstyle(30, 30, "����");
		outtextxy(20, 20, score);
		if (exist_of_boom == 1)
		{
			putimage(115, 500, &img[15], NOTSRCERASE);
			putimage(115, 500, &img[16], SRCINVERT);
			char score4[30] = "";
			sprintf(score4, "�۸�%d",  boom_price);
			settextcolor(YELLOW);
			setbkmode(TRANSPARENT);
			settextstyle(25, 20, "����");
			outtextxy(115, 600, score4);
		}

		if (m2.uMsg == WM_LBUTTONDOWN)
		{
				if((abs(m2.x - 140) <= 25) && (abs(m2.y - 550) <= 50))
				{
					if (sole.coin > boom_price&& exist_of_boom == 1)
					{
						exist_of_boom = 0;
						have_boom++;
						sole.coin =sole.coin-2* boom_price;
					}

				}		
				if (abs(m2.x - 910) <= 80 && abs(m2.y - 110) <= 60)

				{
					gamestage = 1;
					exist_of_boom = 1;
					break;
				}
		}
		FlushBatchDraw();
	}
}

void gamecome()
{
	BeginBatchDraw();
	HookRock();
	GameDraw();
	GameControl(0.5);
	JudgeGrap();
	FlushBatchDraw();
}

void gameend()
{
	BeginBatchDraw();
	putimage(0, 0, &img[17]);
	LPCSTR endword = "GAME OVER!!!";
	settextcolor(YELLOW);
	setbkmode(TRANSPARENT);
	settextstyle(end_word_size, end_word_size, "����");
	outtextxy(200, 320, endword);
	end_word_size += 0.02;
	FlushBatchDraw();
}
//void main 
int main()
{

	

	while (1)
	{
		GameInit();
		while (1)
		{
			if (gamestage == 0)
			{
				gamestart();
			}
			else if (gamestage == 1)
			{
				gamecome();

			}
			if (goldnumingame1 == 0)
			{
				gamestage = 2;
				goldnumingame1 = 2;
				break;

			}
			if (gamestage == 3)
			{
				while (true)
				{
					gameend();

				}
			}
		}
		cleardevice();
		putimage(0, 0, &img[13]);
		Sleep(1000);
		shop();

	}


	return 0;
}

