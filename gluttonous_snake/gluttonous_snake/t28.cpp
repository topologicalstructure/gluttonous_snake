#include <graphics.h>
#include <iostream>
#include <sstream>
#include <conio.h>
#include <cstring>
#include <ctime>
#include <fstream>
#include <cmath>
#pragma comment(lib,"Winmm.lib")
using namespace std;
#define PI 3.1415926
const int T1 = 85;
const int T2 = 200;
//const int T2 = 50;

string usernamestr = "user";

class block {    //用于储存组成蛇的块的类
public:
	int x;       //x,y为块坐标
	int y;
	int dir;     //块的方向
	int type;    //块的类型，0指蛇头，1指蛇身，2指蛇尾
	block* next;
	void printblock(int k);      //打印块k为0则覆盖块
};
   
class snake {              //用于储存蛇的类 
private:
	block* head = NULL;    //链表头
public:
	int size = 3;          //链表长（蛇长）
	int score = 0;
	int state = 1;         //蛇的状态
	int step = 0;          //状态剩余步数
	void printsnake(int c = 1)
	{
		block* p = head;
		for (int i = 0; i < size; i++) {
			if (c == 1)
				p->printblock(state);
			else if (c == 0)
				p->printblock(-2);
			else
				p->printblock(0);
			p = p->next;
		}
	}
	void add(int* map, int dir = 0)
	{
		if (dir != 0 && state == 4 && dir != head->dir)
			score += 10;
		size++;
		int dx = 0, dy = 0, di, xh, yh, to = head->dir;
		head->type = 1;        //对头部的操作
		head->printblock(0);
		xh = head->x;
		yh = head->y;
		if (dir == 0)
			di = head->dir;
		else
			di = dir;
		if (di == 1) {
			dx = 1;
			dy = 0;
		}
		else if (di == 2) {
			dx = 0;
			dy = 1;
		}
		else if (di == 3) {
			dx = -1;
			dy = 0;
		}
		else if (di == 4) {
			dx = 0;
			dy = -1;
		}
		block* newblock = new(nothrow) block;
		if (newblock == NULL)
			return;
		newblock->next = head;
		head = newblock;
		head->x = xh + dx;
		head->y = yh + dy;
		head->type = 0;
		head->dir = di;
		map[37 * (head->x + 1) + head->y + 1] = -1;
		if (dir) {           //若蛇头转向，原头方向变为拐方向
			head->next->dir = 10 * to + dir;
			head->next->printblock(state);
		}
		head->printblock(state);          //打印新头
		head->next->printblock(state);

	}
	void sethead(block* p)
	{
		head = p;
	}
	block* gethead()
	{
		return head;
	}
	void move(int* map, int dir = 0)   //实现一次移动，dir为移动方向，为0则不变方向
	{
		if (dir != 0 && state == 4 && dir != head->dir)
			score += 10;
		int dx = 0, dy = 0, di, xh, yh, to = head->dir;
		head->type = 1;        //对头部的操作
		head->printblock(0);
		xh = head->x;
		yh = head->y;
		if (dir == 0)
			di = head->dir;
		else
			di = dir;
		if (di == 1) {
			dx = 1;
			dy = 0;
		}
		else if (di == 2) {
			dx = 0;
			dy = 1;
		}
		else if (di == 3) {
			dx = -1;
			dy = 0;
		}
		else if (di == 4) {
			dx = 0;
			dy = -1;
		}
		block* newblock = new(nothrow) block;
		if (newblock == NULL)
			return;
		newblock->next = head;
		head = newblock;
		head->x = xh + dx;
		head->y = yh + dy;
		head->type = 0;
		head->dir = di;
		map[37 * (head->x + 1) + head->y + 1] = -1;
		if (dir && size > 2) {           //若蛇头转向，原头方向变为拐方向
			head->next->dir = 10 * to + dir;
			head->next->printblock(state);
		}
		else if (size == 2)
			head->next->dir = di;
		block* p = head, * q = p;
		while (p->next != NULL) {
			q = p;
			p = p->next;
		}
		map[37 * (p->x + 1) + p->y + 1] = 0;
		p->printblock(0);
		q->printblock(0);
		delete p;
		q->type = 2;
		q->next = NULL;
		if (q->dir >= 10)            //若新尾为拐，方向改为直方向
			q->dir = q->dir % 10;
		q->printblock(state);
		head->printblock(state);          //打印新头
		head->next->printblock(state);
		map[37 * (q->x + 1) + q->y + 1] = -2;
	}
	int judge(int* map, int dir = 0)         //判定移动后的结果
	{
		int dx = 0, dy = 0, di, xh, yh;
		xh = head->x;
		yh = head->y;
		if (dir == 0)
			di = head->dir;
		else
			di = dir;
		if (di == 1) {
			dx = 1;
			dy = 0;
		}
		else if (di == 2) {
			dx = 0;
			dy = 1;
		}
		else if (di == 3) {
			dx = -1;
			dy = 0;
		}
		else if (di == 4) {
			dx = 0;
			dy = -1;
		}
		int addsco = 0;
		if (map[37 * (xh + dx + 1) + yh + dy + 1] == 1)
			addsco += 100;
		else if (map[37 * (xh + dx + 1) + yh + dy + 1] == 2)
			addsco -= 100;
		else if (map[37 * (xh + dx + 1) + yh + dy + 1] == 3)
			addsco += 500;
		else if (map[37 * (xh + dx + 1) + yh + dy + 1] == 4) {
			state = 2;
			printsnake();
			step = 50;
		}
		else if (map[37 * (xh + dx + 1) + yh + dy + 1] == 5) {
			state = 3;
			printsnake();
			step = 80;
		}
		else if (map[37 * (xh + dx + 1) + yh + dy + 1] == 6) {
			state = 4;
			printsnake();
			step = 50;
		}
		if (state == 2)
			addsco *= 2;
		score += addsco;
		return map[37 * (xh + dx + 1) + yh + dy + 1];
	}
	void ui(long t0, int max, int game, int num = 0)
	{
		setbkmode(OPAQUE);
		settextcolor(WHITE);
		settextstyle(20, 0, _T("ANTIALIASED_QUALITY"));
		wchar_t str[1024] = L"";
		swprintf_s(str, L"分数:%d       ", score);
		outtextxy(60, 10, str);
		swprintf_s(str, L"蛇长:%d     ", size);
		outtextxy(200, 10, str);
		long t = clock();
		swprintf_s(str, L"游戏时间:%.1f秒           ", (t * 1.0 - t0) * 0.001);
		outtextxy(300, 10, str);
		swprintf_s(str, L"最高分:%d分", max);
		outtextxy(500, 10, str);
		swprintf_s(str, L"状态:");
		outtextxy(60, 40, str);
		if (state == 1) {
			swprintf_s(str, L"普通");
			settextcolor(RGB(68, 206, 246));
		}
		else if (state == 2) {
			swprintf_s(str, L"加速");
			settextcolor(RGB(238, 17, 170));
		}
		else if (state == 3) {
			swprintf_s(str, L"太阳");
			settextcolor(RGB(247, 175, 76));
		}
		else if (state == 4) {
			swprintf_s(str, L"植株");
			settextcolor(RGB(16, 184, 58));
		}
		outtextxy(110, 40, str);
		settextcolor(WHITE);
		if (state == 1) {
			swprintf_s(str, L"状态剩余步数:无限   ");
			outtextxy(170, 40, str);
		}
		else {
			swprintf_s(str, L"状态剩余步数:%d      ", step);
			outtextxy(170, 40, str);
		}
		if (game == 3) {
			swprintf_s(str, L"剩余生命值:                            ");
			outtextxy(350, 40, str);
			int n = 6 - num;
			IMAGE img;
			loadimage(&img, _T("shrimp.png"), 24, 24, 1);
			for (int i = 0; i < n; i++) {
				putimage(460 + 30 * i, 38, &img);
			}
		}
	}
	~snake()
	{
		block* p1 = NULL, * p = head;
		while (p != NULL) {
			p1 = p->next;
			delete p;
			p = p1;
		}
	}
	void turnto(int* map, int c)         //c为0时变为墙，c为1时变为食物
	{
		if (c == 0) {
			printsnake(0);
			block* p = head;
			while (p != NULL) {
				map[37 * (p->x + 1) + p->y + 1] = -1;
				p = p->next;
			}
		}
		else {
			IMAGE img;
			loadimage(&img, _T("easteregg.png"), 16, 16, 1);
			printsnake(-1);
			block* p = head;
			while (p != NULL) {
				map[37 * (p->x + 1) + p->y + 1] = 1;
				putimage(40 + 16 * p->x, 80 + 16 * p->y, &img);
				p = p->next;
			}
		}
	}
	int restart(int* map)
	{
		block* p1 = NULL, * p2 = head;
		while (p2 != NULL) {
			p1 = p2->next;
			delete p2;
			p2 = p1;
		}
		int headx, heady, tailx, taily, k = 0, di;
		for (int i = 1; i <= 40; i++) {
			for (int j = 1; j <= 35; j++) {
				if (map[i * 37 + j] == 0) {
					if (map[(i - 1) * 37 + j] == 0) {
						headx = i;
						heady = j;
						tailx = i - 1;
						taily = j;
						k = 1;
						di = 3;
						break;
					}
					if (map[(i + 1) * 37 + j] == 0) {
						headx = i;
						heady = j;
						tailx = i + 1;
						taily = j;
						k = 1;
						di = 1;
						break;
					}
					if (map[i * 37 + j - 1] == 0) {
						headx = i;
						heady = j;
						tailx = i;
						taily = j - 1;
						k = 1;
						di = 4;
						break;
					}
					if (map[i * 37 + j + 1] == 0) {
						headx = i;
						heady = j;
						tailx = i;
						taily = j + 1;
						k = 1;
						di = 2;
						break;
					}
				}
			}
			if (k)
				break;
		}
		if (!k)
			return 0;
		size = 2;
		block* p = new(nothrow) block;
		if (p == NULL)
			return -1;
		head = p;
		p->x = headx - 1;
		p->y = heady - 1;
		p->dir = di;
		p->type = 0;
		p->next = new(nothrow) block;
		if (p->next == NULL)
			return -1;
		p = p->next;
		p->x = tailx - 1;
		p->y = taily - 1;
		p->dir = di;
		p->type = 2;
		p->next = NULL;
		map[tailx * 37 + taily] = -2;
		map[headx * 37 + heady] = -1;
		return 1;
	}
};

class recorditem {
public:
	int edition;
	char* username;
	int score;
	recorditem()
	{
		username = new(nothrow) char[100];
		if (username == NULL)
			exit(EXIT_FAILURE);
		edition = 0;
		score = 0;
	}
	~recorditem()
	{
		delete[] username;
	}
};

void border()
{
	setfillcolor(WHITE);
	solidrectangle(40, 80, 680, 640);
}

void SDFcircle(int x, int y, int R, double r, COLORREF color, COLORREF backcolor, int k = 0)   //绘制SDF消锯齿后的圆，(x,y)为圆心坐标，R为圆半径，r为画线宽度，参数k判断是否填充
{
	double d;
	if (!k) {
		for (int i = (int)(x - R - r - 3); i <= (int)(x + R + r + 3); i++) {
			for (int j = (int)(y - R - r - 3); j <= (int)(y + R + r + 3); j++) {
				d = fabs(sqrt((i - x) * (i - x) + (j - y) * (j - y))) - R;
				if (fabs(d) > r)
					continue;
				putpixel(i, j, RGB(int((1 - fabs(d) / r) * GetRValue(color) + fabs(d) / r * GetRValue(backcolor)), int((1 - fabs(d) / r) * GetGValue(color) + fabs(d) / r * GetGValue(backcolor)), int((1 - fabs(d) / r) * GetBValue(color) + fabs(d) / r * GetBValue(backcolor))));
			}
		}
	}
	else {
		for (int i = (int)(x - R - r - 1); i <= (int)(x + R + r + 1); i++) {
			for (int j = (int)(y - R - r - 3); j <= (int)(y + R + r + 3); j++) {
				d = fabs(sqrt((i - x) * (i - x) + (j - y) * (j - y))) - R;
				if (d > r)
					continue;
				if (d <= 0) {
					putpixel(i, j, color);
					continue;
				}
				putpixel(i, j, RGB(int((1 - fabs(d) / r) * GetRValue(color) + fabs(d) / r * GetRValue(backcolor)), int((1 - fabs(d) / r) * GetGValue(color) + fabs(d) / r * GetGValue(backcolor)), int((1 - fabs(d) / r) * GetBValue(color) + fabs(d) / r * GetBValue(backcolor))));
			}
		}
	}
	return;
}

void SDFpie(int x, int y, int R, double r, COLORREF color, COLORREF backcolor, int c, int k = 0)   //绘制SDF消锯齿后的1/4扇形
{
	double d;
	int xmi, ymi, xma, yma;
	if (c == 1) {
		xmi = x;
		xma = (int)(x + R + r);
		ymi = (int)(y - R - r);
		yma = y;
	}
	else if (c == 2) {
		xmi = x;
		xma = (int)(x + R + r);
		ymi = y;
		yma = (int)(y + R + r);

	}
	else if (c == 3) {
		xmi = (int)(x - R - r);
		xma = x;
		ymi = y;
		yma = (int)(y + R + r);

	}
	else {
		xmi = (int)(x - R - r);
		xma = x;
		ymi = (int)(y - R - r);
		yma = y;
	}
	if (!k) {
		for (int i = (int)(x - R - r - 3); i <= (int)(x + R + r + 3); i++) {
			for (int j = (int)(y - R - r - 3); j <= (int)(y + R + r + 3); j++) {
				d = fabs(sqrt((i - x) * (i - x) + (j - y) * (j - y))) - R;
				if (fabs(d) > r)
					continue;
				if (i >= xmi && i <= xma && j >= ymi && j <= yma)
					putpixel(i, j, RGB(int((1 - fabs(d) / r) * GetRValue(color) + fabs(d) / r * GetRValue(backcolor)), int((1 - fabs(d) / r) * GetGValue(color) + fabs(d) / r * GetGValue(backcolor)), int((1 - fabs(d) / r) * GetBValue(color) + fabs(d) / r * GetBValue(backcolor))));
			}
		}
	}
	else {
		for (int i = (int)(x - R - r - 1); i <= (int)(x + R + r + 1); i++) {
			for (int j = (int)(y - R - r - 3); j <= (int)(y + R + r + 3); j++) {
				d = fabs(sqrt((i - x) * (i - x) + (j - y) * (j - y))) - R;
				if (d > r)
					continue;
				if (d <= 0) {
					if (i >= xmi && i <= xma && j >= ymi && j <= yma)
						putpixel(i, j, color);
					continue;
				}
				if (i >= xmi && i <= xma && j >= ymi && j <= yma)
					putpixel(i, j, RGB(int((1 - fabs(d) / r) * GetRValue(color) + fabs(d) / r * GetRValue(backcolor)), int((1 - fabs(d) / r) * GetGValue(color) + fabs(d) / r * GetGValue(backcolor)), int((1 - fabs(d) / r) * GetBValue(color) + fabs(d) / r * GetBValue(backcolor))));
			}
		}
	}
	return;
}

void menuitem(int a, int k)
{
	BeginBatchDraw();
	setbkmode(TRANSPARENT);
	if (k == 0) {
		if (a == -1)
			settextcolor(RGB(68, 206, 246));
		else if (a == -3)
			settextcolor(RGB(228, 122, 176));
		else
			settextcolor(BLACK);
	}
	else
		settextcolor(WHITE);
	settextstyle(30, 0, _T("ANTIALIASED_QUALITY"));
	switch (a) {
		case 0:
			outtextxy(280, 580, _T("退出游戏"));
			break;
		case 1:
			outtextxy(280, 280, _T("开始游戏"));
			break;
		case 2:
			outtextxy(280, 355, _T("双人游戏"));
			break;
		case 3:
			outtextxy(265, 505, _T("修改用户名"));
			break;
		case 4:
			outtextxy(280, 430, _T("历史记录"));
			break;
		case -1:
			setfillcolor(RGB(100, 78, 128));
			solidrectangle(50, 150, 600, 250);
			outtextxy(55, 170, _T("入门版"));
			settextstyle(20, 0, _T("ANTIALIASED_QUALITY"));
			outtextxy(55, 215, _T("——只有一次机会，蛇死则游戏结束"));
			break;
		case -2:
			setfillcolor(RGB(100, 78, 128));
			solidrectangle(50, 300, 600, 400);
			outtextxy(55, 320, _T("进阶版"));
			settextstyle(20, 0, _T("ANTIALIASED_QUALITY"));
			outtextxy(55, 365, _T("——蛇的尸体将会保留，但会成为新的障碍"));
			break;
		case -3:
			setfillcolor(RGB(100, 78, 128));
			solidrectangle(50, 450, 600, 550);
			outtextxy(55, 470, _T("高级版"));
			settextstyle(20, 0, _T("ANTIALIASED_QUALITY"));
			outtextxy(55, 515, _T("——蛇有五条生命，死后变为食物"));
			break;
		case 24:
			setfillcolor(RGB(100, 78, 128));
			solidrectangle(50, 150, 600, 250);
			outtextxy(55, 170, _T("合作版"));
			settextstyle(20, 0, _T("ANTIALIASED_QUALITY"));
			outtextxy(55, 215, _T("——两名玩家合作，目标是得到更高的总分"));
			break;
		case 25:
			setfillcolor(RGB(100, 78, 128));
			solidrectangle(50, 300, 600, 400);
			outtextxy(55, 320, _T("竞争版"));
			settextstyle(20, 0, _T("ANTIALIASED_QUALITY"));
			outtextxy(55, 365, _T("——两名玩家竞争，最后得分高的获胜"));
			break;
		default:
			break;
	}
	EndBatchDraw();
}
  
int menu()
{
	cleardevice();
	BeginBatchDraw();
	for (int i = 0; i < 720; i++) {
		for (int j = 0; j < 680; j++) {
			putpixel(i, j, RGB(120 + 30 * sin(i / 50), 180 + 60 * sin(j / 50), 160 + i * j % 90));
		}
	}
	setbkmode(TRANSPARENT);
	settextcolor(BLACK);
	settextstyle(60, 0, _T("ANTIALIASED_QUALITY"));
	outtextxy(200, 90, _T("贪吃蛇游戏"));
	settextstyle(30, 0, _T("ANTIALIASED_QUALITY"));
	menuitem(0, 0);
	menuitem(1, 0);
	menuitem(2, 0);
	menuitem(3, 0);
	menuitem(4, 0);
	EndBatchDraw();
	MOUSEMSG m{ 0 };
	int num = 0;
	while (true) {
		if (MouseHit())m = GetMouseMsg();
		if (m.x > 280 && m.x < 440 && m.y>280 && m.y < 310) {
			menuitem(1, 1);
			if (m.uMsg == WM_LBUTTONDOWN){
				num = 1;
				break;
			}
		}
		else
			menuitem(1, 0);
		if (m.x > 280 && m.x < 440 && m.y>580 && m.y < 610) {
			menuitem(0, 1);
			if (m.uMsg == WM_LBUTTONDOWN)
				return 0;
		}
		else
			menuitem(0, 0);
		if (m.x > 280 && m.x < 440 && m.y>355 && m.y < 385) {
			menuitem(2, 1);
			if (m.uMsg == WM_LBUTTONDOWN) {
				num = 2;
				break;
			}
		}
		else
			menuitem(2, 0);
		if (m.x > 265 && m.x < 455 && m.y>505 && m.y < 535) {
			menuitem(3, 1);
			if (m.uMsg == WM_LBUTTONDOWN) {
				return 33;
			}
		}
		else
			menuitem(3, 0);
		if (m.x > 280 && m.x < 440 && m.y>430 && m.y < 460) {
			menuitem(4, 1);
			if (m.uMsg == WM_LBUTTONDOWN)
				return 6;
		}
		else
			menuitem(4, 0);
	}

	cleardevice();
	BeginBatchDraw();
	for (int i = 0; i < 720; i++) {
		for (int j = 0; j < 680; j++) {
			putpixel(i, j, RGB(120 + 30 * sin(i / 50), 180 + 60 * sin(j / 50), 160 + i * j % 90));
		}
	}
	setbkmode(TRANSPARENT);
	settextcolor(BLACK);
	settextstyle(30, 0, _T("ANTIALIASED_QUALITY"));
	outtextxy(50, 90, _T("请选择游戏模式："));
	if (num == 1) {
		menuitem(-1, 0);
		menuitem(-2, 0);
		menuitem(-3, 0);
		EndBatchDraw();
		while (true) {
			if (MouseHit())m = GetMouseMsg();
			if (m.x > 50 && m.x < 600 && m.y>150 && m.y < 250) {
				menuitem(-1, 1);
				if (m.uMsg == WM_LBUTTONDOWN)
					return 1;
			}
			else
				menuitem(-1, 0);
			if (m.x > 50 && m.x < 600 && m.y>300 && m.y < 400) {
				menuitem(-2, 1);
				if (m.uMsg == WM_LBUTTONDOWN)
					return 2;
			}
			else
				menuitem(-2, 0);
			if (m.x > 50 && m.x < 600 && m.y>450 && m.y < 550) {
				menuitem(-3, 1);
				if (m.uMsg == WM_LBUTTONDOWN)
					return 3;
			}
			else
				menuitem(-3, 0);
		}
	}
	else {
		menuitem(24, 0);
		menuitem(25, 0);
		EndBatchDraw();
		while (true) {
			if (MouseHit())m = GetMouseMsg();
			if (m.x > 50 && m.x < 600 && m.y>150 && m.y < 250) {
				menuitem(24, 1);
				if (m.uMsg == WM_LBUTTONDOWN)
					return 4;
			}
			else
				menuitem(24, 0);
			if (m.x > 50 && m.x < 600 && m.y>300 && m.y < 400) {
				menuitem(25, 1);
				if (m.uMsg == WM_LBUTTONDOWN)
					return 5;
			}
			else
				menuitem(25, 0);
		}
	}
	return 0;
}

void block::printblock(int k)      //打印块k为0则覆盖块
{
	if (!k) {
		setfillcolor(WHITE);
		solidrectangle(40 + 16 * x, 80 + 16 * y, 40 + 16 * x + 16, 80 + 16 * y + 16);
		return;
	}
	int x0 = 40 + 16 * x, y0 = 80 + 16 * y;
	//solidrectangle(40 + 16 * x, 80 + 16 * y, 40 + 16 * x + 16, 80 + 16 * y + 16);
	COLORREF color;
	if (k == 1)
		color = RGB(68, 206, 246);
	else if (k == 2)
		color = RGB(238, 17, 170);
	else if (k == 3)
		color = RGB(247, 175, 76);
	else if (k == 4)
		color = RGB(16, 184, 58);
	else if(k == -1)
		color = RGB(34, 17, 187);
	else
		color = BLACK;
	if (type == 0 || type == 2) {
		SDFcircle(x0 + 8, y0 + 8, 8, 0.5, color, WHITE, 1);
		setfillcolor(color);
		if ((type == 0 && dir == 1) || (type == 2 && dir == 3))
			solidrectangle(x0, y0, x0 + 8, y0 + 16);
		else if ((type == 0 && dir == 3) || (type == 2 && dir == 1))
			solidrectangle(x0 + 8, y0, x0 + 16, y0 + 16);
		else if ((type == 0 && dir == 2) || (type == 2 && dir == 4))
			solidrectangle(x0, y0, x0 + 16, y0 + 8);
		else if ((type == 0 && dir == 4) || (type == 2 && dir == 2))
			solidrectangle(x0, y0 + 8, x0 + 16, y0 + 16);
	}
	else {
		if (dir == 12 || dir == 43)
			SDFpie(x0, y0 + 16, 16, 0.5, color, WHITE, 1, 1);
		else if (dir == 23 || dir == 14)
			SDFpie(x0, y0, 16, 0.5, color, WHITE, 2, 1);
		else if (dir == 34 || dir == 21)
			SDFpie(x0 + 16, y0, 16, 0.5, color, WHITE, 3, 1);
		else if (dir == 41 || dir == 32)
			SDFpie(x0 + 16, y0 + 16, 16, 0.5, color, WHITE, 4, 1);
		else {
			setfillcolor(color);
			solidrectangle(x0, y0, x0 + 16, y0 + 16);
		}
	}
}

int start(snake* sn, int* map, int k = 1)
{
	block* p = new(nothrow) block;
	if (p == NULL)
		return -1;
	if (k == 2) {
		sn->sethead(p);
		p->x = 37;
		p->y = 0;
		p->dir = 3;
		p->type = 0;
		p->next = new(nothrow) block;
		if (p->next == NULL)
			return -1;
		p = p->next;
		p->x = 38;
		p->y = 0;
		p->dir = 3;
		p->type = 1;
		p->next = new(nothrow) block;
		if (p->next == NULL)
			return -1;
		p = p->next;
		p->x = 39;
		p->y = 0;
		p->dir = 3;
		p->type = 2;
		p->next = NULL;
		map[40 * 37 + 1] = -2;
		map[39 * 37 + 1] = -1;
		map[38 * 37 + 1] = -1;
	}
	else {
		sn->sethead(p);
		p->x = 2;
		p->y = 0;
		p->dir = 1;
		p->type = 0;
		p->next = new(nothrow) block;
		if (p->next == NULL)
			return -1;
		p = p->next;
		p->x = 1;
		p->y = 0;
		p->dir = 1;
		p->type = 1;
		p->next = new(nothrow) block;
		if (p->next == NULL)
			return -1;
		p = p->next;
		p->x = 0;
		p->y = 0;
		p->dir = 1;
		p->type = 2;
		p->next = NULL;
		map[1 * 37 + 1] = -2;
		map[2 * 37 + 1] = -1;
		map[3 * 37 + 1] = -1;
	}
	return 0;
}

void laid(int* map, int a)
{
	int x, y;
	while (1) {
		x = rand() % 40 + 1;
		y = rand() % 35 + 1;
		if (x == 1 && (y == 1 || y == 35))
			continue;
		if (x == 40 && (y == 1 || y == 35))
			continue;
		if (map[x * 37 + y] == 0)
			break;
	}
	IMAGE img;
	if (a == 1)
		loadimage(&img, _T("easteregg.png"), 15, 15, 1);
	else if (a == 2)
		loadimage(&img, _T("clover.png"), 15, 15, 1);
	else if (a == 3)
		loadimage(&img, _T("clover2.png"), 15, 15, 1);
	else if (a == 4)
		loadimage(&img, _T("item_red_tin.png"), 15, 15, 1);
	else if (a == 5)
		loadimage(&img, _T("item_start_lightbulb.png"), 15, 15, 1);
	else if (a == 6)
		loadimage(&img, _T("item_start_branch.png"), 15, 15, 1);
	putimage(40 + 16 * (x - 1) + 1, 80 + 16 * (y - 1) + 1, &img);
	map[x * 37 + y] = a;
}

int things(int* map, int sco, snake* sn)
{
	int sco1 = 100 * (sco / 100);
	int eggn = (int)sqrt(sco1 / 200);
	if (eggn < 1)
		eggn = 1;
	if (eggn > 5)
		eggn = 5;
	int room = 0, need = 0;         //剩余空间和需要使用的新空间
	for (int i = 1; i <= 40; i++) {
		for (int j = 0; j <= 35; j++) {
			if (map[i * 37 + j] == 0)
				room++;
		}
	}
	if (sco1 % 2000 == 0 && sco1 != 0)
		need++;
	if (sco1 % 500 == 0 && sco1 != 0)
		need++;
	need += eggn;
	if (need > room)
		return -1;
	int clover;
	int a = rand() % 3;
	if (a == 0)
		clover = 3;
	else
		clover = 2;
	if (sco1 % 2000 == 0 && sco1 != 0)
		laid(map, a + 4);
	if (sn->state == 3)
		clover = 3;
	if (sco1 % 500 == 0 && sco1 != 0)
		laid(map, clover);
	for (int i = 1; i <= 40; i++) {
		for (int j = 0; j <= 35; j++) {
			if (map[i * 37 + j] == 1)
				return 0;
		}
	}
	for (int i = 0; i < eggn; i++) {
		laid(map, 1);
	}
	return 0;
}

int maxscore(int c)    //从记录文件中读取当前模式的最高分
{
	int max = 0;
	ifstream in;
	in.open("history.record", ios::binary);
	if (!in)
		return -1;
	in.seekg(4ll * (c - 1ll), ios::beg);
	in.read((char*)&max, 4 * sizeof(char));
	in.close();
	return max;
}

void record(int c, int score, int max)
{
	ofstream out;
	out.open("history.record", ios::binary | ios::ate | ios::out | ios::in);
	if (!out)
		return;
	if (score > max) {
		out.seekp(4ll * (c - 1ll), ios::beg);
		out.write((char*)&score, 4 * sizeof(char));
	}
	out.seekp(0, ios::end);
	out << c <<" " << usernamestr << " ";
	/*TCHAR szUser[80];
	DWORD cbUser = 80;
	if (GetUserName(szUser, &cbUser)){
		int size = WideCharToMultiByte(CP_ACP, 0, szUser, -1, NULL, 0, NULL, 0);
		char* str = new char[sizeof(char) * size];
		WideCharToMultiByte(CP_ACP, 0, szUser, -1, str, size, NULL, 0);
		out << c;
		out << str;
		out << " ";
	}
	else
		out << "user ";*/
	out.write((char*)&score, 4 * sizeof(char));
	out.close();
}

void clearkey()
{
	while (_kbhit() == 1)
		_getch();
}

void clearhistory()
{
	ofstream out;
	out.open("history.record", ios::binary);
	if (!out)
		return;
	int a = 0;
	for (int i = 0; i < 3; i++) {
		out.write((char*)&a, 4 * sizeof(char));
	}
	out.close();
}

int check(int* map)
{
	int room = 0;
	for (int i = 1; i <= 40; i++) {
		for (int j = 0; j <= 35; j++) {
			if (map[i * 37 + j] == 0)
				room++;
		}
	}
	if (!room)
		return 0;
	else
		return 1;
}

void ui_twoperson(int c, snake* sn1, snake* sn2)            //双人模式下的ui
{
	setbkmode(OPAQUE);
	settextcolor(WHITE);
	settextstyle(20, 0, _T("ANTIALIASED_QUALITY"));
	wchar_t str[1024] = L"";
	swprintf_s(str, L"玩家1：");
	outtextxy(60, 5, str);
	swprintf_s(str, L"分数:%d       ", sn1->score);
	outtextxy(130, 5, str);
	swprintf_s(str, L"蛇长:%d     ", sn1->size);
	outtextxy(270, 5, str);
	swprintf_s(str, L"状态:");
	outtextxy(350, 5, str);
	if (sn1->state == 1) {
		swprintf_s(str, L"普通");
		settextcolor(RGB(68, 206, 246));
	}
	else if (sn1->state == 2) {
		swprintf_s(str, L"加速");
		settextcolor(RGB(238, 17, 170));
	}
	else if (sn1->state == 3) {
		swprintf_s(str, L"太阳");
		settextcolor(RGB(247, 175, 76));
	}
	else if (sn1->state == 4) {
		swprintf_s(str, L"植株");
		settextcolor(RGB(16, 184, 58));
	}
	outtextxy(400, 5, str);
	settextcolor(WHITE);
	if (sn1->state == 1) {
		swprintf_s(str, L"状态剩余步数:无限   ");
		outtextxy(460, 5, str);
	}
	else {
		swprintf_s(str, L"状态剩余步数:%d      ", sn1->step);
		outtextxy(460, 5, str);
	}
	swprintf_s(str, L"玩家2：");
	outtextxy(60, 30, str);
	swprintf_s(str, L"分数:%d       ", sn2->score);
	outtextxy(130, 30, str);
	swprintf_s(str, L"蛇长:%d     ", sn2->size);
	outtextxy(270, 30, str);
	swprintf_s(str, L"状态:");
	outtextxy(350, 30, str);
	if (sn2->state == -1) {
		swprintf_s(str, L"普通");
		settextcolor(RGB(34, 17, 187));
	}
	else if (sn2->state == 2) {
		swprintf_s(str, L"加速");
		settextcolor(RGB(238, 17, 170));
	}
	else if (sn2->state == 3) {
		swprintf_s(str, L"太阳");
		settextcolor(RGB(247, 175, 76));
	}
	else if (sn2->state == 4) {
		swprintf_s(str, L"植株");
		settextcolor(RGB(16, 184, 58));
	}
	outtextxy(400, 30, str);
	settextcolor(WHITE);
	if (sn2->state == -1) {
		swprintf_s(str, L"状态剩余步数:无限   ");
		outtextxy(460, 30, str);
	}
	else {
		swprintf_s(str, L"状态剩余步数:%d      ", sn2->step);
		outtextxy(460, 30, str);
	}
	if (c == 4) {
		swprintf_s(str, L"游戏模式:合作模式");
		outtextxy(60, 55, str);
		swprintf_s(str, L"总分:%d       ", sn1->score + sn2->score);
		outtextxy(270, 55, str);
	}
	else {
		swprintf_s(str, L"游戏模式:竞争模式");
		outtextxy(60, 55, str);
		if (sn1->score > sn2->score)
			swprintf_s(str, L"玩家1暂时领先");
		else if (sn1->score < sn2->score)
			swprintf_s(str, L"玩家2暂时领先");
		else
			swprintf_s(str, L"暂时平分       ");
		outtextxy(270, 55, str);
	}
}

int game(int c)
{
	cleardevice();
	mciSendString(_T("open Phosphor.mp3 alias bkmusic"), NULL, 0, 0);
	mciSendString(_T("play bkmusic repeat"), NULL, 0, NULL);
	mciSendString(TEXT("open eategg.mp3 alias music"), NULL, 0, 0);
	srand((unsigned int)(time(0)));
	int map[42][37] = { 0 };
	for (int i = 0; i < 42; i++) {
		for (int j = 0; j < 37; j++) {
			if (i == 0 || i == 41 || j == 0 || j == 36)
				map[i][j] = -1;
		}
	}
	long t0 = clock();
	if (c <= 3) {
		int max = maxscore(c);
		border();
		snake sn;
		int number = 0, can = 1;
		while (1) {
			number++;
			if (number == 1) {
				if (start(&sn, *map) == -1)
					return -1;
			}
			else {
				sn.turnto(*map, c - 2);
				sn.restart(*map);
				sn.printsnake();
			}
			if (number == 1) {
				sn.printsnake();
				things(*map, sn.score, &sn);
			}
			clearkey();
			while (1) {
				BeginBatchDraw();
				if (sn.step != 0) {
					sn.step--;
					if (sn.step == 0) {
						sn.state = 1;
						sn.printsnake();
					}
				}
				if (_kbhit() == 1) {
					char key = _getch();
					if (key == -32)
						key = -_getch();
					if (key == 'w' || key == 'W' || key == -72) {
						if (sn.gethead()->dir == 2)
							continue;
						int j = sn.judge(*map, 4);
						if (j == -1) {
							break;
						}
						else if (j >= 1 && j <= 3) {
							if (j == 1)
								mciSendString(TEXT("play music from 0"), NULL, 0, NULL);
							sn.add(*map, 4);
							sn.printsnake();
							if (things(*map, sn.score, &sn) == -1) {
								can = 0;
								break;
							}
							FlushBatchDraw();
							if (sn.state == 2)
								Sleep(T1);
							else
								Sleep(T2);
							continue;
						}
						sn.move(*map, 4);
					}
					else if (key == 'a' || key == 'A' || key == -75) {
						if (sn.gethead()->dir == 1)
							continue;
						int j = sn.judge(*map, 3);
						if (j == -1) {
							break;
						}
						else if (j >= 1 && j <= 3) {
							if (j == 1)
								mciSendString(TEXT("play music from 0"), NULL, 0, NULL);
							sn.add(*map, 3);
							if (things(*map, sn.score, &sn) == -1) {
								can = 0;
								break;
							}
							FlushBatchDraw();
							if (sn.state == 2)
								Sleep(T1);
							else
								Sleep(T2);
							continue;
						}
						sn.move(*map, 3);
					}
					else if (key == 's' || key == 'S' || key == -80) {
						if (sn.gethead()->dir == 4)
							continue;
						int j = sn.judge(*map, 2);
						if (j == -1) {
							break;
						}
						else if (j >= 1 && j <= 3) {
							if (j == 1)
								mciSendString(TEXT("play music from 0"), NULL, 0, NULL);
							sn.add(*map, 2);
							if (things(*map, sn.score, &sn) == -1) {
								can = 0;
								break;
							}
							FlushBatchDraw();
							if (sn.state == 2)
								Sleep(T1);
							else
								Sleep(T2);
							continue;
						}
						sn.move(*map, 2);
					}
					else if (key == 'd' || key == 'D' || key == -77) {
						if (sn.gethead()->dir == 3)
							continue;
						int j = sn.judge(*map, 1);
						if (j == -1) {
							break;
						}
						else if (j >= 1 && j <= 3) {
							if (j == 1)
								mciSendString(TEXT("play music from 0"), NULL, 0, NULL);
							sn.add(*map, 1);
							if (things(*map, sn.score, &sn) == -1) {
								can = 0;
								break;
							}
							FlushBatchDraw();
							if (sn.state == 2)
								Sleep(T1);
							else
								Sleep(T2);
							continue;
						}
						sn.move(*map, 1);
					}
					else if (key == 27) {
						can = 0;
						break;
					}
					else {
						int j = sn.judge(*map);
						if (j == -1) {
							break;
						}
						else if (j >= 1 && j <= 3) {
							if (j == 1)
								mciSendString(TEXT("play music from 0"), NULL, 0, NULL);
							sn.add(*map);
							if (things(*map, sn.score, &sn) == -1) {
								can = 0;
								break;
							}
							FlushBatchDraw();
							if (sn.state == 2)
								Sleep(T1);
							else
								Sleep(T2);
							continue;
						}
						sn.move(*map);
					}
				}
				else {
					int j = sn.judge(*map);
					if (j == -1) {
						break;
					}
					else if (j >= 1 && j <= 3) {
						if (j == 1)
							mciSendString(TEXT("play music from 0"), NULL, 0, NULL);
						sn.add(*map);
						if (things(*map, sn.score, &sn) == -1) {
							can = 0;
							break;
						}
						FlushBatchDraw();
						if (sn.state == 2)
							Sleep(T1);
						else
							Sleep(T2);
						continue;
					}
					sn.move(*map);
				}
				sn.ui(t0, max, c, number);
				FlushBatchDraw();
				if (!check(*map)) {
					can = 0;
					break;
				}
				if (sn.state == 2)
					Sleep(T1);
				else
					Sleep(T2);
			}
			if (c == 1) {
				sn.ui(t0, max, c);
				break;
			}
			else if (c == 2 && can == 0) {
				sn.ui(t0, max, c);
				break;
			}
			else if (c == 3 && (can == 0 || number == 5)) {
				sn.ui(t0, max, c, 6);
				break;
			}
			if (c == 2)
				sn.score -= 1000;
			else if (c == 3)
				sn.score -= 100 * (sn.size - 3);
		}
		mciSendString(_T("close bkmusic"), NULL, 0, NULL);
		setbkmode(TRANSPARENT);
		settextcolor(RGB(170, 0, 255));
		settextstyle(60, 0, _T("ANTIALIASED_QUALITY"));
		outtextxy(230, 290, _T("游戏结束"));
		settextstyle(30, 0, _T("ANTIALIASED_QUALITY"));
		wchar_t str[1024] = L"";
		if (sn.score > max)
			swprintf_s(str, L"最终分数:%d，新纪录！", sn.score);
		else
			swprintf_s(str, L"最终分数:%d", sn.score);
		outtextxy(230, 350, str);
		outtextxy(230, 390, _T("按任意键返回菜单"));
		record(c, sn.score, max);
		EndBatchDraw();
		_getch();
	}
	else {
		border();
		snake sn1, sn2;
		sn2.state = -1;
		int number = 0, can = 1;
		number++;
		if (start(&sn1, *map) == -1)
			return -1;
		sn1.printsnake();
		if (start(&sn2, *map, 2) == -1)
			return -1;
		sn2.printsnake();
		things(*map, sn1.score + sn2.score, &sn1);
		clearkey();
		int dead = 0;
		while (1) {
			BeginBatchDraw();
			if (sn1.step != 0) {
				sn1.step--;
				if (sn1.step == 0) {
					sn1.state = 1;
					sn1.printsnake();
				}
			}
			if (sn2.step != 0) {
				sn2.step--;
				if (sn2.step == 0) {
					sn2.state = -1;
					sn2.printsnake();
				}
			}
			int k = 1, k1 = 0, k2 = 0;
			if (_kbhit() == 1) {
				char key = _getch();
				if (key == -32)
					key = -_getch();
				if (key == 'w' || key == 'W') {
					if (sn1.gethead()->dir == 2)
						continue;
					k1 = 1;
					int j = sn1.judge(*map, 4);
					if (j == -1) {
						dead = 1;
						break;
					}
					else if (j >= 1 && j <= 3) {
						if (j == 1)
							mciSendString(TEXT("play music from 0"), NULL, 0, NULL);
						sn1.add(*map, 4);
						sn1.printsnake();
						if (sn2.state == 3) {
							if (things(*map, sn1.score + sn2.score, &sn2) == -1) {
								can = 0;
								break;
							}
						}
						else {
							if (things(*map, sn1.score + sn2.score, &sn1) == -1) {
								can = 0;
								break;
							}
						}
						FlushBatchDraw();
						k = 0;
					}
					if (k)
						sn1.move(*map, 4);
				}
				else if (key == 'a' || key == 'A') {
					if (sn1.gethead()->dir == 1)
						continue;
					k1 = 1;
					int j = sn1.judge(*map, 3);
					if (j == -1) {
						dead = 1;
						break;
					}
					else if (j >= 1 && j <= 3) {
						if (j == 1)
							mciSendString(TEXT("play music from 0"), NULL, 0, NULL);
						sn1.add(*map, 3);
						if (sn2.state == 3) {
							if (things(*map, sn1.score + sn2.score, &sn2) == -1) {
								can = 0;
								break;
							}
						}
						else {
							if (things(*map, sn1.score + sn2.score, &sn1) == -1) {
								can = 0;
								break;
							}
						}
						FlushBatchDraw();
						k = 0;
					}
					if (k)
						sn1.move(*map, 3);
				}
				else if (key == 's' || key == 'S') {
					if (sn1.gethead()->dir == 4)
						continue;
					k1 = 1;
					int j = sn1.judge(*map, 2);
					if (j == -1) {
						dead = 1;
						break;
					}
					else if (j >= 1 && j <= 3) {
						if (j == 1)
							mciSendString(TEXT("play music from 0"), NULL, 0, NULL);
						sn1.add(*map, 2);
						if (sn2.state == 3) {
							if (things(*map, sn1.score + sn2.score, &sn2) == -1) {
								can = 0;
								break;
							}
						}
						else {
							if (things(*map, sn1.score + sn2.score, &sn1) == -1) {
								can = 0;
								break;
							}
						}
						FlushBatchDraw();
						k = 0;
					}
					if (k)
						sn1.move(*map, 2);
				}
				else if (key == 'd' || key == 'D') {
					if (sn1.gethead()->dir == 3)
						continue;
					k1 = 1;
					int j = sn1.judge(*map, 1);
					if (j == -1) {
						dead = 1;
						break;
					}
					else if (j >= 1 && j <= 3) {
						if (j == 1)
							mciSendString(TEXT("play music from 0"), NULL, 0, NULL);
						sn1.add(*map, 1);
						if (sn2.state == 3) {
							if (things(*map, sn1.score + sn2.score, &sn2) == -1) {
								can = 0;
								break;
							}
						}
						else {
							if (things(*map, sn1.score + sn2.score, &sn1) == -1) {
								can = 0;
								break;
							}
						}
						FlushBatchDraw();
						k = 0;
					}
					if (k)
						sn1.move(*map, 1);
				}
				else if (key == 27) {
					can = 0;
					break;
				}
				else if (key == -72) {
					if (sn2.gethead()->dir == 2)
						continue;
					k2 = 1;
					int j = sn2.judge(*map, 4);
					if (j == -1) {
						dead = 2;
						break;
					}
					else if (j >= 1 && j <= 3) {
						if (j == 1)
							mciSendString(TEXT("play music from 0"), NULL, 0, NULL);
						sn2.add(*map, 4);
						sn2.printsnake();
						if (sn2.state == 3) {
							if (things(*map, sn1.score + sn2.score, &sn2) == -1) {
								can = 0;
								break;
							}
						}
						else {
							if (things(*map, sn1.score + sn2.score, &sn1) == -1) {
								can = 0;
								break;
							}
						}
						FlushBatchDraw();
						k = 0;
					}
					if (k)
						sn2.move(*map, 4);
				}
				else if (key == -75) {
					if (sn2.gethead()->dir == 1)
						continue;
					k2 = 1;
					int j = sn2.judge(*map, 3);
					if (j == -1) {
						dead = 2;
						break;
					}
					else if (j >= 1 && j <= 3) {
						if (j == 1)
							mciSendString(TEXT("play music from 0"), NULL, 0, NULL);
						sn2.add(*map, 3);
						if (sn2.state == 3) {
							if (things(*map, sn1.score + sn2.score, &sn2) == -1) {
								can = 0;
								break;
							}
						}
						else {
							if (things(*map, sn1.score + sn2.score, &sn1) == -1) {
								can = 0;
								break;
							}
						}
						FlushBatchDraw();
						k = 0;
					}
					if (k)
						sn2.move(*map, 3);
				}
				else if (key == -80) {
					if (sn2.gethead()->dir == 4)
						continue;
					k2 = 1;
					int j = sn2.judge(*map, 2);
					if (j == -1) {
						dead = 2;
						break;
					}
					else if (j >= 1 && j <= 3) {
						if (j == 1)
							mciSendString(TEXT("play music from 0"), NULL, 0, NULL);
						sn2.add(*map, 2);
						if (sn2.state == 3) {
							if (things(*map, sn1.score + sn2.score, &sn2) == -1) {
								can = 0;
								break;
							}
						}
						else {
							if (things(*map, sn1.score + sn2.score, &sn1) == -1) {
								can = 0;
								break;
							}
						}
						FlushBatchDraw();
						k = 0;
					}
					if (k)
						sn2.move(*map, 2);
				}
				else if (key == -77) {
					if (sn2.gethead()->dir == 3)
						continue;
					k2 = 1;
					int j = sn2.judge(*map, 1);
					if (j == -1) {
						dead = 2;
						break;
					}
					else if (j >= 1 && j <= 3) {
						if (j == 1)
							mciSendString(TEXT("play music from 0"), NULL, 0, NULL);
						sn2.add(*map, 1);
						if (sn2.state == 3) {
							if (things(*map, sn1.score + sn2.score, &sn2) == -1) {
								can = 0;
								break;
							}
						}
						else {
							if (things(*map, sn1.score + sn2.score, &sn1) == -1) {
								can = 0;
								break;
							}
						}
						FlushBatchDraw();
						k = 0;
					}
					if (k)
						sn2.move(*map, 1);
				}
			}
			k = 1;
			if (_kbhit() == 1) {
				char key = _getch();
				if (key == -32)
					key = -_getch();
				if (key == 'w' || key == 'W' && k1 == 0) {
					if (sn1.gethead()->dir == 2)
						continue;
					k1 = 1;
					int j = sn1.judge(*map, 4);
					if (j == -1) {
						dead = 1;
						break;
					}
					else if (j >= 1 && j <= 3) {
						if (j == 1)
							mciSendString(TEXT("play music from 0"), NULL, 0, NULL);
						sn1.add(*map, 4);
						sn1.printsnake();
						if (sn2.state == 3) {
							if (things(*map, sn1.score + sn2.score, &sn2) == -1) {
								can = 0;
								break;
							}
						}
						else {
							if (things(*map, sn1.score + sn2.score, &sn1) == -1) {
								can = 0;
								break;
							}
						}
						FlushBatchDraw();
						k = 0;
					}
					if (k)
						sn1.move(*map, 4);
				}
				else if (key == 'a' || key == 'A' && k1 == 0) {
					if (sn1.gethead()->dir == 1)
						continue;
					k1 = 1;
					int j = sn1.judge(*map, 3);
					if (j == -1) {
						dead = 1;
						break;
					}
					else if (j >= 1 && j <= 3) {
						if (j == 1)
							mciSendString(TEXT("play music from 0"), NULL, 0, NULL);
						sn1.add(*map, 3);
						if (sn2.state == 3) {
							if (things(*map, sn1.score + sn2.score, &sn2) == -1) {
								can = 0;
								break;
							}
						}
						else {
							if (things(*map, sn1.score + sn2.score, &sn1) == -1) {
								can = 0;
								break;
							}
						}
						FlushBatchDraw();
						k = 0;
					}
					if (k)
						sn1.move(*map, 3);
				}
				else if (key == 's' || key == 'S' && k1 == 0) {
					if (sn1.gethead()->dir == 4)
						continue;
					k1 = 1;
					int j = sn1.judge(*map, 2);
					if (j == -1) {
						dead = 1;
						break;
					}
					else if (j >= 1 && j <= 3) {
						if (j == 1)
							mciSendString(TEXT("play music from 0"), NULL, 0, NULL);
						sn1.add(*map, 2);
						if (sn2.state == 3) {
							if (things(*map, sn1.score + sn2.score, &sn2) == -1) {
								can = 0;
								break;
							}
						}
						else {
							if (things(*map, sn1.score + sn2.score, &sn1) == -1) {
								can = 0;
								break;
							}
						}
						FlushBatchDraw();
						k = 0;
					}
					if (k)
						sn1.move(*map, 2);
				}
				else if (key == 'd' || key == 'D' && k1 == 0) {
					if (sn1.gethead()->dir == 3)
						continue;
					k1 = 1;
					int j = sn1.judge(*map, 1);
					if (j == -1) {
						dead = 1;
						break;
					}
					else if (j >= 1 && j <= 3) {
						if (j == 1)
							mciSendString(TEXT("play music from 0"), NULL, 0, NULL);
						sn1.add(*map, 1);
						if (sn2.state == 3) {
							if (things(*map, sn1.score + sn2.score, &sn2) == -1) {
								can = 0;
								break;
							}
						}
						else {
							if (things(*map, sn1.score + sn2.score, &sn1) == -1) {
								can = 0;
								break;
							}
						}
						FlushBatchDraw();
						k = 0;
					}
					if (k)
						sn1.move(*map, 1);
				}
				else if (key == 27) {
					can = 0;
					break;
				}
				else if (key == -72 && k2 == 0) {
					if (sn2.gethead()->dir == 2)
						continue;
					k2 = 1;
					int j = sn2.judge(*map, 4);
					if (j == -1) {
						dead = 2;
						break;
					}
					else if (j >= 1 && j <= 3) {
						if (j == 1)
							mciSendString(TEXT("play music from 0"), NULL, 0, NULL);
						sn2.add(*map, 4);
						sn2.printsnake();
						if (sn2.state == 3) {
							if (things(*map, sn1.score + sn2.score, &sn2) == -1) {
								can = 0;
								break;
							}
						}
						else {
							if (things(*map, sn1.score + sn2.score, &sn1) == -1) {
								can = 0;
								break;
							}
						}
						FlushBatchDraw();
						k = 0;
					}
					if (k)
						sn2.move(*map, 4);
				}
				else if (key == -75 && k2 == 0) {
					if (sn2.gethead()->dir == 1)
						continue;
					k2 = 1;
					int j = sn2.judge(*map, 3);
					if (j == -1) {
						dead = 2;
						break;
					}
					else if (j >= 1 && j <= 3) {
						if (j == 1)
							mciSendString(TEXT("play music from 0"), NULL, 0, NULL);
						sn2.add(*map, 3);
						if (sn2.state == 3) {
							if (things(*map, sn1.score + sn2.score, &sn2) == -1) {
								can = 0;
								break;
							}
						}
						else {
							if (things(*map, sn1.score + sn2.score, &sn1) == -1) {
								can = 0;
								break;
							}
						}
						FlushBatchDraw();
						k = 0;
					}
					if (k)
						sn2.move(*map, 3);
				}
				else if (key == -80 && k2 == 0) {
					if (sn2.gethead()->dir == 4)
						continue;
					k2 = 1;
					int j = sn2.judge(*map, 2);
					if (j == -1) {
						dead = 2;
						break;
					}
					else if (j >= 1 && j <= 3) {
						if (j == 1)
							mciSendString(TEXT("play music from 0"), NULL, 0, NULL);
						sn2.add(*map, 2);
						if (sn2.state == 3) {
							if (things(*map, sn1.score + sn2.score, &sn2) == -1) {
								can = 0;
								break;
							}
						}
						else {
							if (things(*map, sn1.score + sn2.score, &sn1) == -1) {
								can = 0;
								break;
							}
						}
						FlushBatchDraw();
						k = 0;
					}
					if (k)
						sn2.move(*map, 2);
				}
				else if (key == -77 && k2 == 0) {
					if (sn2.gethead()->dir == 3)
						continue;
					k2 = 1;
					int j = sn2.judge(*map, 1);
					if (j == -1) {
						dead = 2;
						break;
					}
					else if (j >= 1 && j <= 3) {
						if (j == 1)
							mciSendString(TEXT("play music from 0"), NULL, 0, NULL);
						sn2.add(*map, 1);
						if (sn2.state == 3) {
							if (things(*map, sn1.score + sn2.score, &sn2) == -1) {
								can = 0;
								break;
							}
						}
						else {
							if (things(*map, sn1.score + sn2.score, &sn1) == -1) {
								can = 0;
								break;
							}
						}
						FlushBatchDraw();
						k = 0;
					}
					if (k)
						sn2.move(*map, 1);
				}
			}
			k = 1;
			if (k1 == 0) {
				int j = sn1.judge(*map);
				if (j == -1) {
					dead = 1;
					break;
				}
				else if (j >= 1 && j <= 3) {
					if (j == 1)
						mciSendString(TEXT("play music from 0"), NULL, 0, NULL);
					sn1.add(*map);
					if (sn2.state == 3) {
						if (things(*map, sn1.score + sn2.score, &sn2) == -1) {
							can = 0;
							break;
						}
					}
					else {
						if (things(*map, sn1.score + sn2.score, &sn1) == -1) {
							can = 0;
							break;
						}
					}
					FlushBatchDraw();
					k = 0;
				}
				if (k)
					sn1.move(*map);
			}
			k = 1;
			if (k2 == 0) {
				int j = sn2.judge(*map);
				if (j == -1) {
					dead = 2;
					break;
				}
				else if (j >= 1 && j <= 3) {
					if (j == 1)
						mciSendString(TEXT("play music from 0"), NULL, 0, NULL);
					sn2.add(*map);
					if (sn2.state == 3) {
						if (things(*map, sn1.score + sn2.score, &sn2) == -1) {
							can = 0;
							break;
						}
					}
					else {
						if (things(*map, sn1.score + sn2.score, &sn1) == -1) {
							can = 0;
							break;
						}
					}
					FlushBatchDraw();
					k = 0;
				}
				if (k)
					sn2.move(*map);
			}
			FlushBatchDraw();
			if (!check(*map)) {
				can = 0;
				break;
			}
			ui_twoperson(c, &sn1, &sn2);
			if (sn1.state == 2 || sn2.state == 2)
				Sleep(T1);
			else
				Sleep(T2);
		}
		if (c == 5 && dead == 1)
			sn1.score -= 2000;
		else if (c == 5 && dead == 2)
			sn2.score -= 2000;
		ui_twoperson(c, &sn1, &sn2);
		mciSendString(_T("close bkmusic"), NULL, 0, NULL);
		setbkmode(TRANSPARENT);
		settextcolor(BLACK);
		settextstyle(60, 0, _T("ANTIALIASED_QUALITY"));
		outtextxy(230, 290, _T("游戏结束"));
		settextstyle(30, 0, _T("ANTIALIASED_QUALITY"));
		wchar_t str[1024] = L"";
		if (c == 4)
			swprintf_s(str, L"最终总分数:%d ", sn1.score + sn2.score);
		else {
			if (sn1.score > sn2.score)
				swprintf_s(str, L"玩家1获胜");
			else if (sn1.score < sn2.score)
				swprintf_s(str, L"玩家2获胜");
			else
				swprintf_s(str, L"两玩家平分");
		}
		outtextxy(230, 350, str);
		outtextxy(230, 390, _T("按任意键返回菜单"));
		EndBatchDraw();
		_getch();
	}
	return 0;
}

void line(recorditem* re, int n)
{
	n = n % 20;
	settextcolor(BLACK);
	settextstyle(20, 0, _T("ANTIALIASED_QUALITY"));
	wchar_t str[1024] = L"";
	wchar_t s[1024] = L"";
	int len = MultiByteToWideChar(CP_ACP, 0, re->username, (int)strlen(re->username), NULL, 0);
	MultiByteToWideChar(CP_ACP, 0, re->username, (int)strlen(re->username), s, len);
	if (re->edition == 1)
		swprintf_s(str, L"版本：入门版   用户名：%-10s  得分：%d", s, re->score);
	else if (re->edition == 2)
		swprintf_s(str, L"版本：进阶版   用户名：%-10s  得分：%d", s, re->score);
	else if (re->edition == 3)
		swprintf_s(str, L"版本：高级版   用户名：%-10s  得分：%d", s, re->score);
	outtextxy(50, 140 + 25 * n, str);
}

void page(int c, int n, recorditem* re, int max1, int max2, int max3)
{
	cleardevice();
	BeginBatchDraw();
	for (int i = 0; i < 720; i++) {
		for (int j = 0; j < 680; j++) {
			putpixel(i, j, RGB(120 + 30 * sin(i / 50), 180 + 60 * sin(j / 50), 160 + i * j % 90));
		}
	}
	setbkmode(TRANSPARENT);
	settextcolor(RGB(0, 30, 50));
	settextstyle(20, 0, _T("ANTIALIASED_QUALITY"));
	wchar_t str[1024] = L"";
	swprintf_s(str, L"入门版最高分数:%d", max1);
	outtextxy(50, 40, str);
	swprintf_s(str, L"进阶版最高分数:%d", max2);
	outtextxy(50, 75, str);
	swprintf_s(str, L"高级版最高分数:%d", max3);
	outtextxy(50, 110, str);
	for (int i = 20 * (c - 1); i < min(n, 20 * c); i++) {
		line(&re[i], i);
	}
	swprintf_s(str, L"第%d页", c);
	outtextxy(320, 635, str);
	swprintf_s(str, L"按AD或左右键切换页码，按回车退回到菜单，按delete清空记录");
	outtextxy(80, 660, str);
	EndBatchDraw();
}

void history()
{
	int n = 0, max1, max2, max3;
	recorditem* re=new(nothrow) recorditem[1024];
	if (re == NULL)
		exit(EXIT_FAILURE);
	ifstream in;
	in.open("history.record", ios::binary);
	if(!in)
		exit(EXIT_FAILURE);
	in.read((char*)&max1, 4 * sizeof(char));
	in.read((char*)&max2, 4 * sizeof(char));
	in.read((char*)&max3, 4 * sizeof(char));
	while (in.peek() >= 0) {
		char c;
		in.read(&c, sizeof(char));
		re[n].edition = c - '0';
		in >> re[n].username;
		in.seekg(1, ios::cur);
		in.read((char*)&re[n].score, 4 * sizeof(char));
		n++;
	}
	in.close();
	int spag = (int)ceil(n / 20.0), pa = 1;
	if (n == 0)
		spag = 1;
	page(1, n, re, max1, max2, max3);
	clearkey();
	while (true) {
		char key = _getch();
		if (key == -32)
			key = -_getch();
		if (key == 'a' || key == 'A' || key == -75) {
			if (pa != 1) {
				pa--;
				page(pa, n, re, max1, max2, max3);
			}
		}
		else if (key == 'd' || key == 'D' || key == -77) {
			if (pa != spag) {
				pa++;
				page(pa, n, re, max1, max2, max3);
			}
		}
		else if (key == 13)
			break;
		else if (key == -83) {
			clearhistory();
			history();
			return;
		}
	}
	delete[] re;
}

void changdusername()
{
	cleardevice();
	BeginBatchDraw();
	for (int i = 0; i < 720; i++) {
		for (int j = 0; j < 680; j++) {
			putpixel(i, j, RGB(120 + 30 * sin(i / 50), 180 + 60 * sin(j / 50), 160 + i * j % 90));
		}
	}
	EndBatchDraw();
	clearkey();
	setbkmode(TRANSPARENT);
	settextcolor(BLACK);
	settextstyle(20, 0, _T("ANTIALIASED_QUALITY"));
	outtextxy(50, 100, _T("当前使用的用户名为："));
	int x0 = 270;
	unsigned int n = 0;
	while (n< usernamestr.length()) {
		wchar_t str[1024] = L"";
		swprintf_s(str, L"%c", usernamestr[n]);
		outtextxy(x0, 100, str);
		x0 += 10;
		n++;
	}
	outtextxy(50, 170, _T("请在下方的输入框中输入要修改的用户名，输完后，请按回车键确认。"));
	outtextxy(50, 240, _T("注意：输入的用户名不要超过10个字符，且只能包含字母、数字及部分符号。"));
	outtextxy(50, 310, _T("按esc键即可取消用户名修改并返回菜单。"));
	outtextxy(50, 370, _T("如有输入错误可以按backspace键清除错误字符。"));
	setfillcolor(WHITE);
	solidrectangle(50, 440, 300, 470);
	string s;
	int x = 55, y = 445;
	setbkmode(TRANSPARENT);
	char c;
	while (true) {
		while (_kbhit() == 0) {
			settextcolor(BLACK);
			settextstyle(20, 0, _T("ANTIALIASED_QUALITY"));
			outtextxy(x, y, _T("|"));
			Sleep(100);
			settextcolor(WHITE);
			settextstyle(20, 0, _T("ANTIALIASED_QUALITY"));
			outtextxy(x, y, _T("|"));
			Sleep(50);
		}
		c = _getch();
		if (c == 13||(x-55)/10==10) {
			if(s.length()>0)
				usernamestr = s;
			break;
		}
		else if (c == 8&&x!=55) {
			solidrectangle(x - 10, y, x, y + 20);
			x -= 10;
			s.pop_back();
		}
		else if (c == 27)
			break;
		else if(c>=32&&c<=126&&c!=' ') {
			settextcolor(BLACK);
			settextstyle(20, 0, _T("ANTIALIASED_QUALITY"));
			s += c;
			wchar_t str[1024] = L"";
			swprintf_s(str, L"%c", c);
			outtextxy(x, y, str);
			x += 10;
		}
	}
}

int main()
{
	initgraph(720, 680);
	while (true) {
		int a = menu();
		if (a >= 1 && a <= 5) {
			if (game(a) == -1)
				return -1;
		}
		else if (a == 0)
			return 0;
		else if (a == 6)
			history();
		else if (a == 33) {
			changdusername();
		}
	}
	closegraph();
	return 0;
} 