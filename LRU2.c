#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

typedef struct _page
{
    bool InsideOrUsed; // 是否在内存中或者是否被使用
    int PageNumber; // ap中对应的页号或者pp中对应的页号
}Page;

typedef struct _pagec
{
    bool InsideOrUsed; // 是否在内存中或者是否被使用
    int PageNumber; // ap中对应的页号或者pp中对应的页号
    int wait; // 页面等待未使用的次数
}PageC;

int main()
{
    int LRU = 1; // 使用的页面置换算法
    int pp = 10; // 进程总共的页面数
    int ap = 5; // 内存为此进程分配的页面数
    int length = 19; // 程序运行使用的页面序列长度
    int flag = 0; // 是否手动输入

    printf("选择使用的页面置换算法（1：LRU, 0：FIFO）：");
    scanf("%d", &LRU);
    printf("请输入pp、ap值：");
    scanf("%d,%d", &pp, &ap);
    printf("请输入运行序列长度：");
    scanf("%d", &length);
    printf("是否手动输入序列（1：手动，0：随机）：");
    scanf("%d", &flag);

    Page page[pp];
    PageC pageContrl[ap];
    for(int i=0;i<pp;i++)
    {
        page[i].InsideOrUsed = false;
        page[i].PageNumber = -1;
    }
    for(int i=0;i<ap;i++)
    {
        pageContrl[i].InsideOrUsed = false;
        pageContrl[i].PageNumber = -1;
        pageContrl[i].wait = 0;
    }

    int run[length]; // 使用的页面序列
    // Bleady数据：pp = 10, ap = 3, length = 20, run = {0, 1, 2, 3, 4, 0, 1, 2, 3, 4, 0, 1, 2, 3, 4, 0, 1, 2, 3, 4}
    // 局部性数据：pp = 10, ap = 4, length = 20, run = {0, 1, 2, 3, 0, 1, 2, 3, 4, 5, 6, 7, 4, 5, 6, 7, 0, 1, 2, 3}
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
    if (LRU)
    {
        for (int i = 0; i < length; i++)
        {
            int cur = run[i];
            for(int j=0;j<ap;j++)
            {
                pageContrl[j].wait++;
            }

            if(page[cur].InsideOrUsed == true)
            {
                pageContrl[page[cur].PageNumber].wait = 0;
                printf("\npage %d in frame %d already", cur, page[cur].PageNumber);
                continue;
            }

            diseffect++;
            int maxWait = 0;
            bool hasEmpty = false;
            for(int j=0;j<ap;j++)
            {
                if(pageContrl[j].InsideOrUsed == false)
                {
                    maxWait = j;
                    hasEmpty = true;
                    break;
                }
                if(pageContrl[j].wait > pageContrl[maxWait].wait)
                {
                    maxWait = j;
                }
            }
            
            if(hasEmpty)
            {
                printf("\npage %d in frame %d", cur, maxWait);
            }
            else
            {
                printf("\npage %d in frame %d replace page %d", cur, maxWait, pageContrl[maxWait].PageNumber);
            }
            pageContrl[maxWait].InsideOrUsed = true;
            pageContrl[maxWait].PageNumber = cur;
            pageContrl[maxWait].wait = 0;
            page[cur].InsideOrUsed = true;
            page[cur].PageNumber = maxWait;    
        }
    }
    else
    {
        int cur_ap = 0; //指向下一个内存待分配页面的指针
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
        

    }
    
    
    double rate = diseffect / (double)length;
    rate = rate * 100;
    printf("\nrate = diseffect / total_instruction*100%% = %.2f%%\n", rate);

    return 0;
}