#include<sys/types.h>
#include<unistd.h>
#include<stdio.h>
#include<sys/wait.h>

//Name : LI Chi
//ID : 11808749D

int main(int argc, char * argv[] )
{
    int childNum = atoi(argv[1]);
    int cardNum = 0;
    int i;
    int childNO = 0;
    int pid = 1;
    for (i= 0; i<childNum; i++) // create children
    {
        if (pid < 0)
        {
            printf("erro");
        }
        else if ( pid == 0)
        {
        }

        else
        {
            childNO++;
            pid  = fork();

        }
    }

    if (pid < 0)
    {
        printf("erro");
    }
    else if ( pid == 0)
    {
        i = 0;   //count number of card one child has
        while (childNum * i + childNO  <= argc -2)
        {
            cardNum ++;
            i++;

        }

        char *card[cardNum];  //  give card to child
        i = 0;
        while (i < cardNum)
        {
            card[i] = argv[childNum * i + childNO +1];
            i++;
        }
        printf("Child %d, pid %d: I have %d cards\n",childNO, getpid(),cardNum);
        sleep(1);
        printf("Child %d, pid %d: ",childNO, getpid());
        for (i = 0; i < cardNum; i++)
        {
            printf("%s ",card[i]);
        }
        printf("\n");

        sleep(1);
        int newCard[cardNum];
        convert(card,newCard,cardNum);
        straightFlush (card,newCard,cardNum,childNO);
        fourOfKing(card,newCard,cardNum,childNO);
        fullHouse (card,newCard,cardNum,childNO);
        flush (card,newCard,cardNum,childNO);
        straight (card,newCard,cardNum,childNO);
        return 0;
    }

    else
    {
        wait(NULL);
    }
}

//convert card into integer: A=1, J=11, Q=12, K=13,  same suit with same hundred number
convert(char *card[], int newCard[], int cardNum)
{

    int i;
    for (i = 0;i < cardNum; i++)
    {
        switch(card[i][0])
        {
            case 'S' : newCard[i] = 0; break;
            case 'H' : newCard[i] = 100; break;
            case 'C' : newCard[i] = 200; break;
            case 'D' : newCard[i] = 300; break;
            default: break;
        }

        switch(card[i][1])
        {
            case 'A' : newCard[i] = newCard[i] + 1; break;
            case '2' : newCard[i] = newCard[i] + 2; break;
            case '3' : newCard[i] = newCard[i] + 3; break;
            case '4' : newCard[i] = newCard[i] + 4; break;
            case '5' : newCard[i] = newCard[i] + 5; break;
            case '6' : newCard[i] = newCard[i] + 6; break;
            case '7' : newCard[i] = newCard[i] + 7; break;
            case '8' : newCard[i] = newCard[i] + 8; break;
            case '9' : newCard[i] = newCard[i] + 9; break;
            case 'T' : newCard[i] = newCard[i] + 10; break;
            case 'J' : newCard[i] = newCard[i] + 11; break;
            case 'Q' : newCard[i] = newCard[i] + 12; break;
            case 'K' : newCard[i] = newCard[i] + 13; break;
            default: break;
        }
    }
}

int searchSameRank(int card[], int cardNum, int number, int index[])
{
    int j,i;
    int count = 0;

    for (j=2; j>0; j--) // search number of Rank 2 and A
    {
        if (j == index[4])
            continue;
        for (i=0; i< cardNum; i++)
        {
            if (card[i] % 100 == j)
            {
                index[count] = i;
                count++;
            }
            if (count == number)
                return 1;
        }
        count = 0;
    }


    for (j=13; j>2; j--) // search number of Rank K->3
    {
        if (j == index[4])
            continue;
        for (i=0; i< cardNum; i++)
        {
            if (card[i] % 100 == j)
            {
                index[count] = i; //record the index of card
                count++;
            }
            if (count == number)
                return 1;
        }
        count = 0;
    }

    return -1;
}

// function four of king
fourOfKing(char *card[], int newCard[], int cardNum,int childNO)
{
    int index[5];
    if (searchSameRank(newCard, cardNum, 4,index) <0)  // no 4 same rank
    {
        printf("Child %d, pid %d: ",childNO, getpid());
        printf("no four-of-a-kind\n");
    }
    else
    {
        printf("Child %d, pid %d: ",childNO, getpid()); // 4 same rank
        printf("highest four-of-a-kind <%s, %s, %s, %s, ",card[index[0]],card[index[1]],card[index[2]],card[index[3]]);
        index[4] = newCard[index[0]]; //store the rank of 4 same rank
        searchSameRank( newCard, cardNum, 1,index);  // get 1 largest rank, card[index[0]]
        printf("%s>\n",card[index[0]]);
    }
}

//function of full house
fullHouse (char *card[], int newCard[], int cardNum,int childNO)
{
    int index[5];
    if (searchSameRank(newCard, cardNum, 3,index) <0)  // no 3 same rank
    {
        printf("Child %d, pid %d: ",childNO, getpid());
        printf("no full-house\n");
    }
    else
    {
        printf("Child %d, pid %d: ",childNO, getpid()); // 3 same rank
        printf("highest full-house <%s, %s, %s, ",card[index[0]],card[index[1]],card[index[2]]);
        index[4] = newCard[index[0]]; //store the rank of 3 same rank
        searchSameRank( newCard, cardNum, 2,index);  // get 2 largest rank, except card[index[0]]
        printf("%s, %s>\n",card[index[0]], card[index[1]]);
    }
}

//function of straight flush
straightFlush (char *card[], int newCard[], int cardNum,int childNO)
{
    int index[5];
    int a,b,c,d;
    int count = 0;
    for (a=13; a>4; a--) //check from highest 13 to lowest A
    {
        for (b=0; b<4; b++) // check from S,H,C,D
        {
            count = 0;
            for (d=0; d<5; d++) //check has 5 consecutive card
            {
                for (c=0; c<cardNum; c++) //check
                {
                    if (newCard[c] % 100 == a-d && newCard[c] > b*100 && newCard[c] < b*100 +100)
                    {
                        index[d] = c; //record the index of card
                        count++;
                    }
                }

            }
            if (count == 5)
                break;
        }
        if (count == 5)
            break;
    }
    if (count ==5)
    {
        printf("Child %d, pid %d: ",childNO, getpid());
        printf("highest straight-flush <%s, %s, %s, %s, %s>\n",card[index[0]],card[index[1]],card[index[2]],card[index[3]],card[index[4]]);
    }
    else
    {
        printf("Child %d, pid %d: ",childNO, getpid());
        printf("no straight-flush\n");
    }
}

//function of straight
straight (char *card[], int newCard[], int cardNum,int childNO)
{
    int index[5];
    int a,b,c,d;
    int count = 0;
    for (a=13; a>4; a--) //check from highest 13 to lowest A
    {
        count = 0;
        for (d=0; d<5; d++) //check has 5 consecutive card
        {
            for (c=0; c<cardNum; c++) //check
            {
                if (newCard[c] % 100 == a-d)
                {
                    index[d] = c; //record the index of card
                    count++;
                    break;
                }
            }

        }
        if (count == 5)
            break;
    }
    if (count ==5)
    {
        printf("Child %d, pid %d: ",childNO, getpid());
        printf("highest straigh <%s, %s, %s, %s, %s>\n",card[index[0]],card[index[1]],card[index[2]],card[index[3]],card[index[4]]);
    }
    else
    {
        printf("Child %d, pid %d: ",childNO, getpid());
        printf("no straight\n");
    }
}

//function of flush
flush (char *card[], int newCard[], int cardNum,int childNO)
{
    int index[5];
    int count  = 0;
    int a,b,c,d;
    int suit[4] = {0,0,0,0};
    for (b = 0; b<cardNum; b++)
    {
        suit[newCard[b]/100] ++;  //count the number of card in the same suit
    }
    for (a=0; a<4; a++)
    {
        if (suit[a]>=5)
        {
            count = 0;
            for (b = 0; b<cardNum; b++)
            {
                if ( newCard[b] > a*100 && newCard[b] < a*100 + 100) //find card in same suit
                {
                    index[count] = b; //record card position
                    count ++; //count card in smae suit
                }
                if (count == 5)
                    break;
            }
        }
        if (count == 5)
            break;
    }

    if (count ==5)
    {
        printf("Child %d, pid %d: ",childNO, getpid());
        printf("flush: <%s, %s, %s, %s, %s>\n",card[index[0]],card[index[1]],card[index[2]],card[index[3]],card[index[4]]);
    }
    else
    {
        printf("Child %d, pid %d: ",childNO, getpid());
        printf("no flush\n");
    }
}
