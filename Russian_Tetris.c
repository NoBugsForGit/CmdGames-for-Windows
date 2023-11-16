#include "tetris.h"
#define FRESH 3

int main()
{
    main_init("Russian_Tetris");
    init();
    start_game();

    while (true)
    {
        generate();
        while (true)
        {
            for (int k = 0; k < FRESH; k++)
            {
                put_block_in_map();
                CL;
                print_map();
                if (get_input())
                    goto END;
                if (move())
                {
                    put_block_in_map();
                    goto NEWBLOCK;
                }
                Sleep(speed / FRESH);
            }
            if (move_down())
            {
                put_block_in_map();
                goto NEWBLOCK;
            }
        }
    NEWBLOCK:
        check_line();
        block[0] = block[1];
        if (scan(empty_block))
            goto END;
        
    }
END:
    printf("%s:%d", name, score);
    system("pause");
}