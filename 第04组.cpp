#undef UNICODE
#include <stdio.h>
#include <stdlib.h>
#include <graphics.h>
#include <conio.h>
#include <time.h>
#include <windows.h>
#include <mmsystem.h>
#pragma comment(lib, "Winmm.lib")

/***********2048��Ϸ���뿪ʼ***********/
//�洢12��ͼƬ
IMAGE img[12];
//ͼƬ���±�
int imgIndex[12] = {0, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024, 2048};
int i, j, k;
int SCORE = 0;
int fflag = 0; //�ж��Ƿ���Ϸ�ѽ�����1��ʾ�ѽ���
//�������4*4������
int map[4][4] = {0};
//��������
void musicPlay()
{
    PlaySound("��������.wav", NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
}
//����ͼƬ
void loadPic()
{
    for (i = 0; i < 12; i++)
    {
        char fileName[20] = "";
        sprintf(fileName, "%d.bmp", imgIndex[i]);
        loadimage(&img[i], fileName);
    }
}
//������ͼ
void drawMap()
{
    setbkcolor(RGB(178, 161, 199));
    cleardevice(); //ʹ���õı���ɫ������
    settextcolor(BLACK);
    settextstyle(40, 0, "���Ŀ���"); //�������ָ�ʽ
    outtextxy(50, 10, "2048��Ϸ");
    settextstyle(18, 0, "���Ŀ���"); //���裬���ǵ���ǰ�����ָ�ʽ
    outtextxy(20, 310, "��w��s��a��d���������ƶ�");
    outtextxy(65, 335, "����������Ч");
    outtextxy(45, 360, "��m��ֹͣ��������");
    outtextxy(60, 385, "��t����������");
    outtextxy(60, 410, "��q��������Ϸ");
    char count[1000];
    sprintf(count, "������%d", SCORE);
    settextstyle(25, 0, "����");
    outtextxy(65, 450, count);
    //������ֵ��ͼ
    int x, y;
    for (i = 0; i < 4; i++)
    {
        for (j = 0; j < 4; j++)
        {
            x = 60 * j;
            y = 60 * i + 60;
            for (k = 0; k < 12; k++)
            {
                //�ҳ�������ͼƬ
                if (imgIndex[k] == map[i][j])
                    break;
            }
            //����Ӧ��λ���Ϸ�ͼƬ
            putimage(x, y, img + k);
        }
    }
}
//�������2��4
void randNum()
{
    srand((unsigned int)time(NULL)); //��ɢ���������
    while (1)
    {
        i = rand() % 4;
        j = rand() % 4;
        if (map[i][j] == 0)
        {
            map[i][j] = (rand() % 3) * 2;
            if (map[i][j] == 0)
                continue; //��rand���������0��ʹ�䲻����0
            break;
        }
    }
}
//�ж��Ƿ���ڿ��ƶ���ͼƬ
int moveablePic()
{
    int flag = 1; //�ж��Ƿ���ƶ���1��ʾ�����ƶ�
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
//������(a)
int moveLeft()
{
    int flag = 0; //0��ʾ�÷����ϲ����ƶ�
    //����������ͬ�ͺϲ�
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
                        break; //�˳�ѭ��
                    }
                    k++; //�����ж���һ��
                }
            }
        }
    }
    //�ƶ�����
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
//������(d)
int moveRight()
{
    int flag = 0; //0��ʾ�÷����ϲ����ƶ�
    //����������ͬ�ͺϲ�
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
                        break; //�˳�ѭ��
                    }
                    k--; //�����ж���һ��
                }
            }
        }
    }
    //�ƶ�����
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
//������(w)
int moveUp()
{
    int flag = 0; //0��ʾ�÷����ϲ����ƶ�
    //����������ͬ�ͺϲ�
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
                        break; //�˳�ѭ��
                    }
                    k++; //�����ж���һ��
                }
            }
        }
    }
    //�ƶ�����
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
//������(s)
int moveDown()
{
    int flag = 0; //0��ʾ�÷����ϲ����ƶ�
    //����������ͬ�ͺϲ�
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
                        break; //�˳�ѭ��
                    }
                    k--; //�����ж���һ��
                }
            }
        }
    }
    //�ƶ�����
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
//�ж�����ʱ�Ƿ��ܼ����ƶ�
void moveAll()
{
    int flag = 1; //�ж��Ƿ�������1Ϊ����
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
        MessageBox(NULL, "GAME OVER!", "SORRY.", MB_OK | MB_SYSTEMMODAL | MB_ICONWARNING); //��Ϣ��ʾʧ����
        closegraph();                                      //�ر���Ϸ����
        PlaySound(NULL, NULL, SND_FILENAME);
        fflag = 1;
    }
}
//�ж���Ϸ�Ƿ�ɹ�
void gameEnd()
{
    for (i = 0; i < 4; i++)
    {
        for (j = 0; j < 4; j++)
        {
            if (map[i][j] == 2048)
            {
                MessageBox(NULL, "YOU WIN!", "CONGRATULATIONS.", MB_OK | MB_SYSTEMMODAL | MB_ICONWARNING); //��Ϣ��ʾ�ɹ���
                closegraph();
                PlaySound(NULL, NULL, SND_FILENAME); //ֹͣ��������
                fflag = 1;
                break;
            }
        }
    }
    moveAll();
}
//�����û��ļ������룬��������Ӧ
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
            MessageBox(NULL, "���������ƶ�", "SORRY.", MB_OK | MB_SYSTEMMODAL | MB_ICONWARNING);
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
            MessageBox(NULL, "���������ƶ�", "SORRY.", MB_OK | MB_SYSTEMMODAL | MB_ICONWARNING);
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
            MessageBox(NULL, "���������ƶ�", "SORRY.", MB_OK | MB_SYSTEMMODAL | MB_ICONWARNING);
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
            MessageBox(NULL, "���������ƶ�", "SORRY.", MB_OK | MB_SYSTEMMODAL | MB_ICONWARNING);
        }
        gameEnd();
        break;
    case 'm':
        PlaySound(NULL, NULL, SND_FILENAME);
        break;
    case 't':
    case 'T':
        PlaySound("��������.wav", NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
        break;
    case 'q':
    case 'Q':
        closegraph();
        PlaySound(NULL, NULL, SND_FILENAME);
        fflag = 1;
        break;
    }
}
//2048������
void main_2048()
{
    loadPic();
    musicPlay();
    //������ʾͼƬ
    initgraph(60 * 4, 60 * 9);
    drawMap();
    //���������ػ����
    randNum();
    drawMap();
    while (1)
    {
        keyDo();
        drawMap();
        if (fflag)
            break;
    }
    //�������ͼ�ν��棬�Ա���һ��ʹ��
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
/***********2048��Ϸ�������***********/

/***********̰������Ϸ���뿪ʼ***********/
#define ROW 46
#define COL 64
//ö�٣��յء�ǽ���ߡ�ʳ��
enum game
{
    SPACE,
    WALL,
    SNAKE,
    FOOD,
    HEAD
};
//ȫ�ֱ���
int mode = 0;       //��Ϸģʽ
int score = 0;      //����
DWORD t1, t2;       //��������ʱ������ƶ��ٶ�
int MAP[ROW][COL];  //��ͼ��С
COORD snake[1024];  //��
size_t SnakeSize;   //�ߵĳ߶�
char SnakeDir;      //���ƶ�����
extern int flag_snake = 0; //ȷ�����˳�̰����
//���ÿ���̨��ɫ
void setColor(int x)
{
    if (x >= 0 && x <= 15) //������0-15�ķ�Χ��ɫ
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), x);
    else //Ĭ�ϵ���ɫ��ɫ
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
}
//ȫ�ֺ���
void start();     //��ʼ����
void chose();     //ѡ��ģʽ  0:��ͨģʽ  1����ǽģʽ  2���޵�ģʽ
void DrawMap();   //��ͼ
void init();      //��ʼ����Ϸ����
void addfood();   //���ʳ��
void ChangeDir(); //�޸��ߵķ���
void move();      //�ߵ��ƶ�
//̰���ߵ�������
void main_snake()
{
    musicPlay();
    initgraph(640, 480);
    start();
    outtextxy(260, 150, "->"); //��ʼĬ��ģʽ1
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
    setbkcolor(WHITE);               //���ñ�����ɫ
    memset(MAP, SPACE, sizeof(MAP)); //��ʼ��MAP����Ϊ0��SPACE
    //ÿһ�еĵ�һ�������һ����ǽ
    for (int i = 0; i < ROW; i++)
    {
        MAP[i][0] = MAP[i][COL - 1] = WALL;
    }
    //ÿһ�еĵڶ����͵����ڶ�����ǽ
    for (int j = 1; j < COL - 1; j++)
    {
        MAP[0][j] = MAP[ROW - 1][j] = WALL;
    }
    //������ͷ���ߵ�����
    MAP[3][5] = HEAD;
    MAP[3][4] = MAP[3][3] = SNAKE;
    //��ʼ����
    SnakeSize = 4;  //�߳�
    SnakeDir = 'D'; //�߷�������
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
    //��Ϸ����
    outtextxy(290, 100, "ģʽѡ��:");
    outtextxy(280, 150, "1����ͨģʽ");
    outtextxy(280, 200, "2����ǽģʽ");
    outtextxy(280, 250, "3���޵�ģʽ");
    outtextxy(200, 300, "��1��2��3ѡ��ģʽ����Enter��������Ϸ");
    outtextxy(240, 350, "��W��S��A��D(��д)���Ʒ���");
    outtextxy(180, 400, "��q�˳���Ϸ����mֹͣ�������֣���t��������");
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
            PlaySound("��������.wav", NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
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
    BeginBatchDraw();               //��ʼ��ͼ
    setbkcolor(RGB(238, 193, 230)); //���ñ�����ɫΪ��ɫ
    settextcolor(RGB(0, 0, 0));
    cleardevice();
    char arr[100];                               //����ɼ�
    sprintf(arr, "Ŀǰ�ߵĳ��ȣ�%d", 4 + score); //���ɼ���ʽ��������ַ���arr��
    outtextxy(0, 0, arr);                        //��ʾ�ɼ�
    outtextxy(250, 0, "����Ϊ�̣�ʳ��Ϊ��");
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
                setfillcolor(RGB(238, 233, 233)); //��ɫ
                fillrectangle(x * 10, y * 10 + 20, x * 10 + 10, y * 10 + 30);
                break;
            case SNAKE:
                setlinecolor(RGB(12, 198, 69)); //ǳ��ɫ
                setfillcolor(WHITE);
                fillrectangle(x * 10, y * 10 + 20, x * 10 + 10, y * 10 + 30);
                break;
            case HEAD:
                setfillcolor(RGB(12, 198, 69)); //ǳ��ɫ
                solidrectangle(x * 10, y * 10 + 20, x * 10 + 10, y * 10 + 30);
                break;
            case FOOD:
                setfillcolor(RGB(0, 130, 255)); //��ɫ
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
            SnakeDir = 'A'; //�߲��ܺ���
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
        PlaySound("��������.wav", NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
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
    COORD next; //��ͷ����һ��λ��
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
    case SPACE:                                                      //ֱ���ƶ�
        MAP[snake[SnakeSize - 1].X][snake[SnakeSize - 1].Y] = SPACE; //��ͼ��β���ڵ��ÿ�
        for (i = SnakeSize - 1; i > 0; i--)                          //��β����ͷ�����ƶ�һλ
        {
            snake[i] = snake[i - 1];
        }
        MAP[snake[0].X][snake[0].Y] = SNAKE; //��ͷ�� ��
        snake[0] = next;                     //����һ��λ�ø�ֵ����ͷ
        MAP[snake[0].X][snake[0].Y] = HEAD;  //����ͷ
        break;
    case WALL:
        if (mode) //ģʽ1ģʽ2�ɴ�ǽ
        {
            MAP[snake[SnakeSize - 1].X][snake[SnakeSize - 1].Y] = SPACE; //��β�ÿ�
            for (i = SnakeSize - 1; i > 0; i--)                          //��β����ͷ�����ƶ�һλ
            {
                snake[i] = snake[i - 1];
            }
            MAP[snake[0].X][snake[0].Y] = SNAKE; //��ͷ�� ��
            switch (SnakeDir)                    //��ǽ
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
            snake[0] = next;                    //��ͷ�ƶ�����λ��
            MAP[snake[0].X][snake[0].Y] = HEAD; //�µ���ͷ���ڵ�λ��
        }
        else
        {
            MessageBox(GetHWnd(), "��Ϸ����", "SORRY", MB_OK | MB_SYSTEMMODAL);
            flag_snake = 1;
            closegraph();
        }
        break;
    case SNAKE:
        if (mode == 2) //ģʽ2:�޵�
        {
            break;
        }
        else
        {
            MessageBox(GetHWnd(), "��Ϸ����", "SORRY", MB_OK | MB_SYSTEMMODAL);
            flag_snake = 1;
            closegraph();
        }
        break;
    case FOOD:                          //ʳ��				��β����
        for (i = SnakeSize; i > 0; i--) //��β����ͷ�����ƶ�һλ
        {
            snake[i] = snake[i - 1];
        }
        MAP[snake[0].X][snake[0].Y] = SNAKE; //��ͷ �� ��
        snake[0] = next;                     //����һ��λ�ø�ֵ����ͷ
        score++;                             //������һ
        (SnakeSize)++;                       //�߳߶ȼ�һ
        MAP[snake[0].X][snake[0].Y] = HEAD;  //��ͼ��������ͷ
        addfood();
        break;
    default:
        break;
    }
}
/*̰������Ϸ�������*/

/*�桤������*/
void main()
{
    while (1)
    {
        printf("��ѡ��Ҫ�����Ϸ(�������ּ���)��\n");
        printf("1��2048��Ϸ��2��̰������Ϸ\n");
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
                printf("��������ȷ������.\n");
                setColor(15);
            }
        }
    }
}