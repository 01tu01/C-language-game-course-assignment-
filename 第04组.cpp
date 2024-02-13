#undef UNICODE
#include <stdio.h>
#include <stdlib.h>
#include <graphics.h>
#include <conio.h>
#include <time.h>
#include <windows.h>
#include <mmsystem.h>
#pragma comment(lib, "Winmm.lib")

/***********2048游戏代码开始***********/
//存储12张图片
IMAGE img[12];
//图片的下标
int imgIndex[12] = {0, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024, 2048};
int i, j, k;
int SCORE = 0;
int fflag = 0; //判断是否游戏已结束，1表示已结束
//定义界面4*4的数组
int map[4][4] = {0};
//播放音乐
void musicPlay()
{
    PlaySound("背景音乐.wav", NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
}
//加载图片
void loadPic()
{
    for (i = 0; i < 12; i++)
    {
        char fileName[20] = "";
        sprintf(fileName, "%d.bmp", imgIndex[i]);
        loadimage(&img[i], fileName);
    }
}
//画界面图
void drawMap()
{
    setbkcolor(RGB(178, 161, 199));
    cleardevice(); //使设置的背景色起作用
    settextcolor(BLACK);
    settextstyle(40, 0, "华文楷体"); //设置文字格式
    outtextxy(50, 10, "2048游戏");
    settextstyle(18, 0, "华文楷体"); //重设，覆盖掉先前的文字格式
    outtextxy(20, 310, "按w、s、a、d上下左右移动");
    outtextxy(65, 335, "按其他键无效");
    outtextxy(45, 360, "按m键停止播放音乐");
    outtextxy(60, 385, "按t键播放音乐");
    outtextxy(60, 410, "按q键结束游戏");
    char count[1000];
    sprintf(count, "步数：%d", SCORE);
    settextstyle(25, 0, "黑体");
    outtextxy(65, 450, count);
    //根据数值放图
    int x, y;
    for (i = 0; i < 4; i++)
    {
        for (j = 0; j < 4; j++)
        {
            x = 60 * j;
            y = 60 * i + 60;
            for (k = 0; k < 12; k++)
            {
                //找出是哪张图片
                if (imgIndex[k] == map[i][j])
                    break;
            }
            //在相应的位置上放图片
            putimage(x, y, img + k);
        }
    }
}
//随机产生2或4
void randNum()
{
    srand((unsigned int)time(NULL)); //播散随机数种子
    while (1)
    {
        i = rand() % 4;
        j = rand() % 4;
        if (map[i][j] == 0)
        {
            map[i][j] = (rand() % 3) * 2;
            if (map[i][j] == 0)
                continue; //因rand函数会产生0，使其不产生0
            break;
        }
    }
}
//判断是否存在可移动的图片
int moveablePic()
{
    int flag = 1; //判断是否可移动，1表示不可移动
    for (i = 0; i < 4; i++)
    {
        for (j = 0; j < 4; j++)
        {
            if (map[i][j] != 0)
            {
                if (i != 3 && j != 3)
                {
                    if ((map[i][j] == map[i][j + 1]) || (map[i][j] == map[i + 1][j]))
                    {
                        flag = 0;
                        break;
                    }
                }
                else
                {
                    if (i == 3 && j != 3 && (map[i][j] == map[i][j + 1]))
                    {
                        flag = 0;
                        break;
                    }
                    else
                    {
                        if (i != 3 && j == 3 && (map[i][j] == map[i + 1][j]))
                        {
                            flag = 0;
                            break;
                        }
                        else
                        {
                            if (i == 3 && j == 3 && (map[i][j] == map[i - 1][j] || map[i][j] == map[i][j - 1]))
                            {
                                flag = 0;
                                break;
                            }
                        }
                    }
                }
            }
        }
    }
    return flag;
}
//往左移(a)
int moveLeft()
{
    int flag = 0; //0表示该方向上不可移动
    //相邻数字相同就合并
    for (i = 0; i < 4; i++)
    {
        for (j = 0; j < 4; j++)
        {
            int curValue = map[i][j];
            if (curValue != 0)
            {
                int k = j + 1;
                while (k < 4)
                {
                    int nextValue = map[i][k];
                    if (nextValue != 0)
                    {
                        if (curValue == nextValue)
                        {
                            flag = 1;
                            map[i][j] += map[i][k];
                            map[i][k] = 0;
                        }
                        k = 4;
                        break; //退出循环
                    }
                    k++; //继续判断下一个
                }
            }
        }
    }
    //移动数字
    for (i = 0; i < 4; i++)
    {
        for (j = 0; j < 4; j++)
        {
            int curValue = map[i][j];
            if (curValue == 0)
            {
                int k = j + 1;
                while (k < 4)
                {
                    int nextValue = map[i][k];
                    if (nextValue != 0)
                    {
                        flag = 1;
                        map[i][j] = nextValue;
                        map[i][k] = 0;
                        k = 4;
                    }
                    k++;
                }
            }
        }
    }
    return flag;
}
//往右移(d)
int moveRight()
{
    int flag = 0; //0表示该方向上不可移动
    //相邻数字相同就合并
    for (i = 0; i < 4; i++)
    {
        for (j = 4 - 1; j >= 0; j--)
        {
            int curValue = map[i][j];
            if (curValue != 0)
            {
                int k = j - 1;
                while (k >= 0)
                {
                    int nextValue = map[i][k];
                    if (nextValue != 0)
                    {
                        if (curValue == nextValue)
                        {
                            flag = 1;
                            map[i][j] += map[i][k];
                            map[i][k] = 0;
                        }
                        k = -1;
                        break; //退出循环
                    }
                    k--; //继续判断下一个
                }
            }
        }
    }
    //移动数字
    for (i = 0; i < 4; i++)
    {
        for (j = 3; j >= 0; j--)
        {
            int curValue = map[i][j];
            if (curValue == 0)
            {
                int k = j - 1;
                while (k >= 0)
                {
                    int nextValue = map[i][k];
                    if (nextValue != 0)
                    {
                        flag = 1;
                        map[i][j] = nextValue;
                        map[i][k] = 0;
                        k = -1;
                    }
                    k--;
                }
            }
        }
    }
    return flag;
}
//往上移(w)
int moveUp()
{
    int flag = 0; //0表示该方向上不可移动
    //相邻数字相同就合并
    for (i = 0; i < 4; i++)
    {
        for (j = 0; j < 4; j++)
        {
            int curValue = map[i][j];
            if (curValue != 0)
            {
                int k = i + 1;
                while (k < 4)
                {
                    int nextValue = map[k][j];
                    if (nextValue != 0)
                    {
                        if (curValue == nextValue)
                        {
                            flag = 1;
                            map[i][j] += map[k][j];
                            map[k][j] = 0;
                        }
                        k = 4;
                        break; //退出循环
                    }
                    k++; //继续判断下一个
                }
            }
        }
    }
    //移动数字
    for (i = 0; i < 3; i++)
    {
        for (j = 0; j < 4; j++)
        {
            int curValue = map[i][j];
            if (curValue == 0)
            {
                int k = i + 1;
                while (k < 4)
                {
                    int nextValue = map[k][j];
                    if (nextValue != 0)
                    {
                        flag = 1;
                        map[i][j] = nextValue;
                        map[k][j] = 0;
                        k = 4;
                    }
                    k++;
                }
            }
        }
    }
    return flag;
}
//往下移(s)
int moveDown()
{
    int flag = 0; //0表示该方向上不可移动
    //相邻数字相同就合并
    for (i = 3; i >= 0; i--)
    {
        for (j = 0; j < 4; j++)
        {
            int curValue = map[i][j];
            if (curValue != 0)
            {
                int k = i - 1;
                while (k >= 0)
                {
                    int nextValue = map[k][j];
                    if (nextValue != 0)
                    {
                        if (curValue == nextValue)
                        {
                            flag = 1;
                            map[i][j] += map[k][j];
                            map[k][j] = 0;
                        }
                        k = 0;
                        break; //退出循环
                    }
                    k--; //继续判断下一个
                }
            }
        }
    }
    //移动数字
    for (i = 3; i >= 0; i--)
    {
        for (j = 0; j < 4; j++)
        {
            int curValue = map[i][j];
            if (curValue == 0)
            {
                int k = i - 1;
                while (k >= 0)
                {
                    int nextValue = map[k][j];
                    if (nextValue != 0)
                    {
                        flag = 1;
                        map[i][j] = nextValue;
                        map[k][j] = 0;
                        k = -1;
                    }
                    k--;
                }
            }
        }
    }
    return flag;
}
//判断满屏时是否能继续移动
void moveAll()
{
    int flag = 1; //判断是否满屏，1为满屏
    for (i = 0; i < 4; i++)
    {
        for (j = 0; j < 4; j++)
        {
            if (map[i][j] == 0)
            {
                flag = 0;
                break;
            }
        }
    }
    if (flag && moveablePic())
    {
        MessageBox(NULL, "GAME OVER!", "SORRY.", MB_OK | MB_SYSTEMMODAL | MB_ICONWARNING); //信息提示失败了
        closegraph();                                      //关闭游戏界面
        PlaySound(NULL, NULL, SND_FILENAME);
        fflag = 1;
    }
}
//判断游戏是否成功
void gameEnd()
{
    for (i = 0; i < 4; i++)
    {
        for (j = 0; j < 4; j++)
        {
            if (map[i][j] == 2048)
            {
                MessageBox(NULL, "YOU WIN!", "CONGRATULATIONS.", MB_OK | MB_SYSTEMMODAL | MB_ICONWARNING); //信息提示成功了
                closegraph();
                PlaySound(NULL, NULL, SND_FILENAME); //停止播放音乐
                fflag = 1;
                break;
            }
        }
    }
    moveAll();
}
//接收用户的键盘输入，并做出反应
void keyDo()
{
    char key = _getch();
    switch (key)
    {
    case 'w':
    case 'W':
    case 72:
        if (moveUp())
        {
            randNum();
            SCORE++;
        }
        else
        {
            MessageBox(NULL, "不能向上移动", "SORRY.", MB_OK | MB_SYSTEMMODAL | MB_ICONWARNING);
        }
        gameEnd();
        break;
    case 's':
    case 'S':
    case 80:
        if (moveDown())
        {
            randNum();
            SCORE++;
        }
        else
        {
            MessageBox(NULL, "不能向下移动", "SORRY.", MB_OK | MB_SYSTEMMODAL | MB_ICONWARNING);
        }
        gameEnd();
        break;
    case 'a':
    case 'A':
    case 75:
        if (moveLeft())
        {
            randNum();
            SCORE++;
        }
        else
        {
            MessageBox(NULL, "不能向左移动", "SORRY.", MB_OK | MB_SYSTEMMODAL | MB_ICONWARNING);
        }
        gameEnd();
        break;
    case 'd':
    case 'D':
    case 77:
        if (moveRight())
        {
            randNum();
            SCORE++;
        }
        else
        {
            MessageBox(NULL, "不能向右移动", "SORRY.", MB_OK | MB_SYSTEMMODAL | MB_ICONWARNING);
        }
        gameEnd();
        break;
    case 'm':
        PlaySound(NULL, NULL, SND_FILENAME);
        break;
    case 't':
    case 'T':
        PlaySound("背景音乐.wav", NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
        break;
    case 'q':
    case 'Q':
        closegraph();
        PlaySound(NULL, NULL, SND_FILENAME);
        fflag = 1;
        break;
    }
}
//2048主函数
void main_2048()
{
    loadPic();
    musicPlay();
    //程序显示图片
    initgraph(60 * 4, 60 * 9);
    drawMap();
    //根据内容重绘界面
    randNum();
    drawMap();
    while (1)
    {
        keyDo();
        drawMap();
        if (fflag)
            break;
    }
    //重置清空图形界面，以便下一次使用
    for (i = 0; i < 4; i++)
    {
        for (j = 0; j < 4; j++)
        {
            map[i][j] = 0;
        }
    }
    SCORE = 0;
    system("pause");
}
/***********2048游戏代码结束***********/

/***********贪吃蛇游戏代码开始***********/
#define ROW 46
#define COL 64
//枚举，空地、墙、蛇、食物
enum game
{
    SPACE,
    WALL,
    SNAKE,
    FOOD,
    HEAD
};
//全局变量
int mode = 0;       //游戏模式
int score = 0;      //分数
DWORD t1, t2;       //定义两个时间控制移动速度
int MAP[ROW][COL];  //地图大小
COORD snake[1024];  //蛇
size_t SnakeSize;   //蛇的尺度
char SnakeDir;      //蛇移动方向
extern int flag_snake = 0; //确保能退出贪吃蛇
//设置控制台颜色
void setColor(int x)
{
    if (x >= 0 && x <= 15) //参数在0-15的范围颜色
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), x);
    else //默认的颜色白色
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
}
//全局函数
void start();     //开始界面
void chose();     //选择模式  0:普通模式  1：穿墙模式  2：无敌模式
void DrawMap();   //画图
void init();      //初始化游戏数据
void addfood();   //添加食物
void ChangeDir(); //修改蛇的方向
void move();      //蛇的移动
//贪吃蛇的主函数
void main_snake()
{
    musicPlay();
    initgraph(640, 480);
    start();
    outtextxy(260, 150, "->"); //初始默认模式1
    chose();
    init();
    while (1)
    {
        t2 = GetTickCount();
        DrawMap();
        if (kbhit())
        {
            ChangeDir();
            move();
            t2 = GetTickCount();
            t1 = t2;
        }
        if (t2 - t1 > 85)
        {
            move();
            t1 = t2;
        }
        if (flag_snake)
            break;
    }
    PlaySound(NULL, NULL, SND_FILENAME);
    system("pause");
}
void init()
{
    srand((unsigned)time(NULL));
    setbkcolor(WHITE);               //设置背景颜色
    memset(MAP, SPACE, sizeof(MAP)); //初始化MAP数组为0即SPACE
    //每一行的第一个和最后一个是墙
    for (int i = 0; i < ROW; i++)
    {
        MAP[i][0] = MAP[i][COL - 1] = WALL;
    }
    //每一列的第二个和倒数第二个是墙
    for (int j = 1; j < COL - 1; j++)
    {
        MAP[0][j] = MAP[ROW - 1][j] = WALL;
    }
    //定义蛇头和蛇的身体
    MAP[3][5] = HEAD;
    MAP[3][4] = MAP[3][3] = SNAKE;
    //初始化蛇
    SnakeSize = 4;  //蛇长
    SnakeDir = 'D'; //蛇方向向右
    snake[0].X = 3;
    snake[0].Y = 5;
    snake[1].X = 3;
    snake[1].Y = 4;
    snake[2].X = 3;
    snake[2].Y = 3;
    addfood();
}
void start()
{
    setbkcolor(RGB(238, 193, 230));
    cleardevice();
    settextcolor(BLACK);
    //游戏规则
    outtextxy(290, 100, "模式选择:");
    outtextxy(280, 150, "1、普通模式");
    outtextxy(280, 200, "2、穿墙模式");
    outtextxy(280, 250, "3、无敌模式");
    outtextxy(200, 300, "按1、2、3选择模式，按Enter键进入游戏");
    outtextxy(240, 350, "按W、S、A、D(大写)控制方向");
    outtextxy(180, 400, "按q退出游戏，按m停止播放音乐，按t播放音乐");
}
void chose()
{
    while (1)
    {
        switch (getch())
        {
        case '1':
            start();
            outtextxy(260, 150, "->");
            mode = 0;
            break;
        case '2':
            start();
            outtextxy(260, 200, "->");
            mode = 1;
            break;
        case '3':
            start();
            outtextxy(260, 250, "->");
            mode = 2;
            break;
        case 13:
            return;
            break;
        case 'm':
            PlaySound(NULL, NULL, SND_FILENAME);
            break;
        case 't':
        case 'T':
            PlaySound("背景音乐.wav", NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
            break;
        case 'q':
        case 'Q':
            closegraph();
            PlaySound(NULL, NULL, SND_FILENAME);
            flag_snake = 1;
            break;
        }
    }
}
void DrawMap()
{
    BeginBatchDraw();               //开始绘图
    setbkcolor(RGB(238, 193, 230)); //设置背景颜色为白色
    settextcolor(RGB(0, 0, 0));
    cleardevice();
    char arr[100];                               //保存成绩
    sprintf(arr, "目前蛇的长度：%d", 4 + score); //将成绩格式化输出到字符串arr中
    outtextxy(0, 0, arr);                        //显示成绩
    outtextxy(250, 0, "蛇身为绿，食物为蓝");
    for (int y = 0; y < ROW; y++)
    {
        for (int x = 0; x < COL; x++)
        {
            switch (MAP[y][x])
            {
            case SPACE:
                break;
            case WALL:
                setlinecolor(BLACK);
                setfillcolor(RGB(238, 233, 233)); //灰色
                fillrectangle(x * 10, y * 10 + 20, x * 10 + 10, y * 10 + 30);
                break;
            case SNAKE:
                setlinecolor(RGB(12, 198, 69)); //浅绿色
                setfillcolor(WHITE);
                fillrectangle(x * 10, y * 10 + 20, x * 10 + 10, y * 10 + 30);
                break;
            case HEAD:
                setfillcolor(RGB(12, 198, 69)); //浅绿色
                solidrectangle(x * 10, y * 10 + 20, x * 10 + 10, y * 10 + 30);
                break;
            case FOOD:
                setfillcolor(RGB(0, 130, 255)); //蓝色
                solidrectangle(x * 10, y * 10 + 20, x * 10 + 10, y * 10 + 30);
                break;
            default:
                break;
            }
        }
    }
    EndBatchDraw();
}
void addfood()
{
    int row, col;
    do
    {
        row = rand() % (ROW - 1) + 1;
        col = rand() % (COL - 1) + 1;
    } while (MAP[row][col] != SPACE);
    MAP[row][col] = FOOD;
}
void ChangeDir()
{
    switch (getch())
    {
    case 'A':
    case 'a':
    case 75:
        if (SnakeDir != 'D')
            SnakeDir = 'A'; //蛇不能后退
        break;
    case 'D':
    case 'd':
    case 77:
        if (SnakeDir != 'A')
            SnakeDir = 'D';
        break;
    case 'W':
    case 'w':
    case 72:
        if (SnakeDir != 'S')
            SnakeDir = 'W';
        break;
    case 'S':
    case 's':
    case 80:
        if (SnakeDir != 'W')
            SnakeDir = 'S';
        break;
    case 32:
        getch();
        break;
    case 'm':
        PlaySound(NULL, NULL, SND_FILENAME);
        break;
    case 't':
    case 'T':
        PlaySound("背景音乐.wav", NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
        break;
    case 'q':
    case 'Q':
        closegraph();
        PlaySound(NULL, NULL, SND_FILENAME);
        flag_snake = 1;
        break;
    default:
        break;
    }
}
void move()
{
    COORD next; //蛇头的下一个位置
    int i;
    switch (SnakeDir)
    {
    case 'A':
        next.X = snake[0].X;
        next.Y = snake[0].Y - 1;
        break;
    case 'W':
        next.X = snake[0].X - 1;
        next.Y = snake[0].Y;
        break;
    case 'D':
        next.X = snake[0].X;
        next.Y = snake[0].Y + 1;
        break;
    case 'S':
        next.X = snake[0].X + 1;
        next.Y = snake[0].Y;
        break;
    default:
        break;
    }
    switch (MAP[next.X][next.Y])
    {
    case SPACE:                                                      //直接移动
        MAP[snake[SnakeSize - 1].X][snake[SnakeSize - 1].Y] = SPACE; //地图蛇尾所在地置空
        for (i = SnakeSize - 1; i > 0; i--)                          //蛇尾到蛇头整体移动一位
        {
            snake[i] = snake[i - 1];
        }
        MAP[snake[0].X][snake[0].Y] = SNAKE; //蛇头置 蛇
        snake[0] = next;                     //将下一个位置赋值给蛇头
        MAP[snake[0].X][snake[0].Y] = HEAD;  //设置头
        break;
    case WALL:
        if (mode) //模式1模式2可穿墙
        {
            MAP[snake[SnakeSize - 1].X][snake[SnakeSize - 1].Y] = SPACE; //蛇尾置空
            for (i = SnakeSize - 1; i > 0; i--)                          //蛇尾到蛇头整体移动一位
            {
                snake[i] = snake[i - 1];
            }
            MAP[snake[0].X][snake[0].Y] = SNAKE; //蛇头置 蛇
            switch (SnakeDir)                    //穿墙
            {
            case 'A':
                next.Y = COL - 2;
                break;
            case 'D':
                next.Y = 1;
                break;
            case 'W':
                next.X = ROW - 2;
                break;
            case 'S':
                next.X = 1;
                break;
            default:
                break;
            }
            snake[0] = next;                    //蛇头移动到新位置
            MAP[snake[0].X][snake[0].Y] = HEAD; //新的蛇头所在的位置
        }
        else
        {
            MessageBox(GetHWnd(), "游戏结束", "SORRY", MB_OK | MB_SYSTEMMODAL);
            flag_snake = 1;
            closegraph();
        }
        break;
    case SNAKE:
        if (mode == 2) //模式2:无敌
        {
            break;
        }
        else
        {
            MessageBox(GetHWnd(), "游戏结束", "SORRY", MB_OK | MB_SYSTEMMODAL);
            flag_snake = 1;
            closegraph();
        }
        break;
    case FOOD:                          //食物				蛇尾不变
        for (i = SnakeSize; i > 0; i--) //蛇尾到蛇头整体移动一位
        {
            snake[i] = snake[i - 1];
        }
        MAP[snake[0].X][snake[0].Y] = SNAKE; //蛇头 置 蛇
        snake[0] = next;                     //将下一个位置赋值给蛇头
        score++;                             //分数加一
        (SnakeSize)++;                       //蛇尺度加一
        MAP[snake[0].X][snake[0].Y] = HEAD;  //地图上重置蛇头
        addfood();
        break;
    default:
        break;
    }
}
/*贪吃蛇游戏代码结束*/

/*真·主函数*/
void main()
{
    while (1)
    {
        printf("请选择要玩的游戏(输入数字即可)：\n");
        printf("1：2048游戏；2：贪吃蛇游戏\n");
        int choice;
        scanf("%d", &choice);
        if (choice == 1)
        {
            main_2048();
        }
        else
        {
            if (choice == 2)
            {
                main_snake();
            }
            else
            {
                setColor(4);
                printf("请输入正确的数字.\n");
                setColor(15);
            }
        }
    }
}