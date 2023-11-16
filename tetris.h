/*
    开发期bug:没有对move和rotate进行检查，总不能撞上了还动吧？

    代码大规模重写:将block更改为[4][4]
*/

#include "cmdgame.h"

#define ID_NUM 9
#define SIZE_X 15
#define SIZE_Y 29

#define SPACE_8 "⚫⚫⚫⚫⚫⚫⚫⚫"
#define CAT_SPACE strcat(chars, SPACE_8)
#define fori4 for (int i = 0; i < 4; i++)
#define forj4 for (int j = 0; j < 4; j++)

char *id_dict[ID_NUM] = {
    "🟧", // 0
    "🟨", // 1
    "🟩", // 2
    "🟦", // 3
    "🟪", // 4
    "🟫", // 5
    "🟥", // 6 0-6方块实体
    "⏺️", // 7：边框
    "⚫"  // 8:空实体
};

typedef struct
{
    bool blockmap[4][4];
    int x, y; // 储存blockmap[0][0]的坐标
    int_1 id;
} Block;

int_1 map[SIZE_X][SIZE_Y] = {0};
int_1 direction = EMPTY_DIC;
int score = 0;
char name[200];
int speed;

static Block blocks[7] = {
    {0, 0, 0, 0, 1, 1, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0},
    {0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 1, 0, 1, 0, 1},
    {0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 1, 1, 0, 1, 0, 2},
    {0, 0, 0, 0, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 0, 3},
    {0, 0, 0, 0, 0, 1, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 0, 4},
    {0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 0, 5},
    {0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 1, 0, 6},
};
static Block empty_block = {0};
Block block[2];

void clear_out_of_map(Block temp)
{
    fori4
    {
        forj4
        {
            if (temp.blockmap[j][i])
                map[temp.x + j][temp.y + i] = 8;
        }
    }
}
bool scan(Block temp) // 扫描方块block[0]是否和map发生冲突，冲突返回true
{
    clear_out_of_map(temp);
    fori4
    {
        forj4
        {
            if (block[0].blockmap[j][i])
                if (map[block[0].x + j][block[0].y + i] != 8)
                    return true;
        }
    }
    return false;
}

bool move_down() // 触底返回1
{
    Block temp = block[0];
    block[0].y++;
    if (scan(temp))
    {
        block[0] = temp;
        return true;
    }
    return false;
}
void init()
{
    for (int i = 1; i < SIZE_X - 1; i++)
    {
        for (int j = 1; j < SIZE_Y - 1; j++)
        {
            map[i][j] = 8;
        }
    }
    for (int i = 0; i < SIZE_X; i++)
    {
        map[i][0] = 8;
        map[i][SIZE_Y - 1] = 7;
    }
    for (int i = 0; i < SIZE_Y - 1; i++)
    {
        map[0][i] = 7;
        map[SIZE_X - 1][i] = 7;
    }
    srand(SEED);
    block[0] = blocks[abs(rand()) % 7];
    block[0].x = 1;
    block[0].y = 0;
    score = 0;
}

void generate()
{
    srand(SEED);
    block[1] = blocks[abs(rand()) % 7];
}

bool move()
{
    if (direction == LEFT || direction == RIGHT)
    {
        Block temp = block[0];
        block[0].x += (direction - 1);
        if (scan(temp))
        {
            block[0] = temp;
        }
    }
    else if (direction == UP || direction == DOWN)
        return move_down();
    return false;
}

void rotate()
{
    Block temp = block[0];
    fori4
    {
        forj4
        {
            block[0].blockmap[i][j] = temp.blockmap[j][3 - i];
        }
    }
    if (scan(temp))
    {
        block[0] = temp;
    }
}

void clear_line(int line)
{
    score++;
    
    for (int i = line; i > 0; i--)
    {
        for (int j = 1; j < SIZE_X - 1; j++)
        {
            map[j][i] = map[j][i - 1];
        }
    }
    for (int i = 1; i < SIZE_X - 1; i++)
    {
        map[i][0] = 8;
    }
}

void check_line()
{
    bool flag;
    for (int i = SIZE_Y - 2; i >= 0; i--)
    {
        flag = true;
        for (int j = 1; j < SIZE_X - 1; j++)
        {
            if (map[j][i] == 8)
            {
                flag = false;
                break;
            }
        }
        if (flag)
        {
            clear_line(i++);
        }
    }
}

void put_block_in_map()
{
    fori4
    {
        forj4
        {
            if (block[0].blockmap[j][i])
                map[block[0].x + j][block[0].y + i] = block[0].id;
        }
    }
}

void print_map()
{
    char chars[SIZE_X * 5 + 100];
    for (int i = 0; i < SIZE_Y; i++)
    {
        strcpy(chars, "");
        for (int j = 0; j < SIZE_X; j++)
        {
            strcat(chars, id_dict[map[j][i]]);
        }

        if (i == 5)
        {
            CAT_SPACE;
            fori4
            {
                if (block[1].blockmap[i][0])
                {
                    strcat(chars, id_dict[block[1].id]);
                }
                else
                {
                    strcat(chars, id_dict[8]);
                }
            }
        }
        else if (i == 6)
        {
            CAT_SPACE;
            fori4
            {
                if (block[1].blockmap[i][1])
                {
                    strcat(chars, id_dict[block[1].id]);
                }
                else
                {
                    strcat(chars, id_dict[8]);
                }
            }
        }
        else if (i == 7)
        {
            CAT_SPACE;
            fori4
            {
                if (block[1].blockmap[i][2])
                {
                    strcat(chars, id_dict[block[1].id]);
                }
                else
                {
                    strcat(chars, id_dict[8]);
                }
            }
        }
        else if (i == 8)
        {
            CAT_SPACE;
            fori4
            {
                if (block[1].blockmap[i][3])
                {
                    strcat(chars, id_dict[block[1].id]);
                }
                else
                {
                    strcat(chars, id_dict[8]);
                }
            }
        }
        else if (i == SIZE_Y / 2 - 1)
        {
            CAT_SPACE;
            strcat(chars, name);
        }
        else if (i == SIZE_Y / 2 + 1)
        {
            CAT_SPACE;
            char temp[12];
            itoa(score, temp, 10);
            strcat(chars, "score:");
            strcat(chars, temp);
        }
        puts(chars);
    }
}

bool get_input()
{
    char ch = 0;
    if (kbhit())
        ch = getch();
    if (ch == -32)
        ch = getch();
    switch (ch)
    {
    case 97:
    case 52:
    case 75:
        direction = LEFT;
        break;
    case 100:
    case 56:
    case 77:
        direction = RIGHT;
        break;
    case 115:
    case 54:
    case 80:
        direction = DOWN;
        break;
    case SPACE:
        rotate();
        break;
    case '[':
        while (!kbhit())
        {
            Sleep(500);
        }
    case ']':
    case ESC:
        CL;
        printf("Quit?Y/N:");
        if (getch() == 'y')
            return true;
        break;
    default:
        direction = EMPTY_DIC;
        break;
    }
    return false;
}

void start_game()
{
    int sp;
    printf("Please input your name:");
    scanf("%199s", name);
    CL;
    printf("Hello,%s!\nPlease choose your level(1~6):", name);
    scanf("%d", &sp);
    switch (sp)
    {
    case 1:
        speed = 1000;
        break;
    case 2:
        speed = 700;
        break;
    case 3:
        speed = 400;
        break;
    case 4:
        speed = 300;
        break;
    case 5:
        speed = 150;
        break;
    case 6:
        speed = 50;
        break;
    case -1:
        speed = 20;
        break;
    default:
        speed = 150;
        break;
    }
    CL;
}