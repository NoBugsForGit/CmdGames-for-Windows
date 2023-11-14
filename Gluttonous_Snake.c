#include "snake.h"
/*
    预期更新：
        显示时间
        存储记录
        优化运行Sleep
    

        游戏平衡调整:
            提高最低速度挡位
            削弱药品
            增强警报器的功能
            提高障碍物的生成概率和生成上限
            降低列车生成概率和上限
*/
int main()
{
    main_init("GluttonousSnake");
    init();
    start_game();
    while (true)
    {
        body_generate();
        generate();
        CL;
        print_map();
        if(get_input())
            break;
        move();
        clock_count();
        Sleep(speed);
        if (life == 0)
            break;
    };
    CL;
    printf("%s:%d\n", name, score);
    system("pause");
    return 0;
}