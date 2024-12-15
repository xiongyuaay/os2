#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

typedef struct _page
{
    bool InsideOrUsed; // 是否在内存中或者是否被使用
    int PageNumber; // ap中对应的页号或者pp中对应的页号
}Page;


int main()
{
    int FIFO = 1; // 使用的页面置换算法
    int pp = 10; // 进程总共的页面数
    int ap = 5; // 内存为此进程分配的页面数
    int length = 19; // 程序运行使用的页面序列长度
    int flag = 0; // 是否手动输入

    printf("选择使用的页面置换算法（1：FIFO, 0：LRU）：");
    scanf("%d", FIFO);
    printf("请输入pp、ap值：");
    scanf("%d,%d", &pp, &ap);
    printf("请输入运行序列长度：");
    scanf("%d", &length);
    printf("是否手动输入序列（1：手动，0：随机）：");
    scanf("%d", &flag);

    Page page[pp];
    Page pageContrl[ap];
    for(int i=0;i<pp;i++)
    {
        page[i].InsideOrUsed = false;
        page[i].PageNumber = -1;
    }
    for(int i=0;i<ap;i++)
    {
        pageContrl[i].InsideOrUsed = false;
        pageContrl[i].PageNumber = -1;
    }

    int cur_ap = 0; //指向下一个内存待分配页面的指针
    int run[length]; // 使用的页面序列
    if(flag)
    {
        for(int i=0;i<length;i++)
        {
            scanf("%d", &run[i]);
            printf("%d ", run[i]);
        }
    }
    else
    {
        for(int i=0;i<length;i++)
        {
            run[i] = rand() % pp;
            printf("%d ", run[i]);
        }
    }

    int diseffect = 0;

    for (int i = 0; i < length; i++)
    {
        int cur = run[i];
        if (page[cur].InsideOrUsed == true)
        {
            printf("\npage %d in frame %d already", cur, page[cur].PageNumber);
            continue;
        }

        page[cur].InsideOrUsed = true;
        page[cur].PageNumber = cur_ap;
        diseffect++;
    
        if (pageContrl[cur_ap].InsideOrUsed == false)
        {
            pageContrl[cur_ap].InsideOrUsed = true;
            pageContrl[cur_ap].PageNumber = cur;
            printf("\npage %d in frame %d", cur, cur_ap);
            cur_ap = (cur_ap + 1) % ap;
        }
        else
        {
            printf("\npage %d in frame %d replace page %d", cur, cur_ap, pageContrl[cur_ap].PageNumber);
            page[pageContrl[cur_ap].PageNumber].InsideOrUsed = false;
            pageContrl[cur_ap].InsideOrUsed = true;
            pageContrl[cur_ap].PageNumber = cur;
            cur_ap = (cur_ap + 1) % ap;
        }        
    }
    
    double rate = diseffect / (double)length;
    rate = rate * 100;
    printf("\nrate = diseffect / total_instruction*100%% = %.2f%%\n", rate);

    return 0;
}