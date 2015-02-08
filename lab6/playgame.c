#include<sys/types.h>
#include<unistd.h>
#include<stdio.h>
#include<sys/wait.h>
#include <string.h>
#include <stdlib.h>

//Name : LI Chi
//ID : 11808749D

//this code fulfill all 3 required levels and bonus level

#define cardNum 13  // Every child has 13 cards
#define childNum 4  //totally 4 children

int main(int argc, char * argv[] )
{
    int pid = 1;
	int n;
	int i;
	// for the pipe, fdp is used to communicate from parent to child, fdc is used to communicate from child to parent
	int fdp1[2], fdp2[2], fdp3[2],fdp4[2],fdc1[2],fdc2[2],fdc3[2], fdc4[2]; 
	char buf[80];
	int newCard[13];
	int childNO = 0;
	//store all the cards in allCard
	char allCard[52][3];
	for (i=0; i < 52; i++)
		scanf("%s", allCard[i]);
		
    // create 8 pipes
	if (pipe(fdp1) < 0) 
	{
		printf("Pipe creation error\n");
		exit(1);
	}
	if (pipe(fdp2) < 0) 
	{
		printf("Pipe creation error\n");
		exit(1);
	}
	if (pipe(fdp3) < 0) 
	{
		printf("Pipe creation error\n");
		exit(1);
	}
    if (pipe(fdp4) < 0) 
	{
		printf("Pipe creation error\n");
		exit(1);
	}
	if (pipe(fdc1) < 0) 
	{
		printf("Pipe creation error\n");
		exit(1);
	}
	if (pipe(fdc2) < 0) 
	{
		printf("Pipe creation error\n");
		exit(1);
	}
	if (pipe(fdc3) < 0) 
	{
		printf("Pipe creation error\n");
		exit(1);
	}
	if (pipe(fdc4) < 0) 
	{
		printf("Pipe creation error\n");
		exit(1);
	} 
	
	// create 4 children
	for (i= 0; i<childNum; i++) 
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
	else if (pid == 0)  //child process
	{ 
		close(fdp1[1]);		//close parent out
		close(fdp2[1]);
		close(fdp3[1]);
		close(fdp4[1]);
		
		close(fdc1[0]);		//close child in
		close(fdc2[0]);
		close(fdc3[0]);
		close(fdc4[0]);
		
		// cast child's pid to string and write to parent
		sprintf(buf,"%d",getpid());
		if (childNO == 1)
			write(fdc1[1],buf,10); 
		if (childNO == 2)
			write(fdc2[1],buf,10); 
		if (childNO == 3)
			write(fdc3[1],buf,10); 
		if (childNO == 4)
			write(fdc4[1],buf,10);

		sleep(1);
		
		char card[cardNum][3];  
        i = 0;
		char winCard[3];
		int index;
        
		//give the cards to child and print cards
		while (i < cardNum)
        {
			strcpy(card[i], allCard[ i * childNum + childNO -1]);
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
		
		//convert the card to integer number
		for (i = 0; i < cardNum; i++)
        {
            newCard[i] = convert(card[i]);
        }
		
		sleep(1);
		
		// start to read instruction from parent and play game
		while (1)
		{
			// read the instruction
			if (childNO == 1)
				read(fdp1[0],buf,80);
			if (childNO == 2)
				read(fdp2[0],buf,80);
			if (childNO == 3)
				read(fdp3[0],buf,80);
			if (childNO == 4)
				read(fdp4[0],buf,80);
			
			// toplay instruction, start a new round
			if (buf[0] == 't')
			{
				index = toplay(newCard); // call toplay function
				printf("Child %d: play %s\n",childNO,card[index]);
				strcpy(buf,"play");
				strcpy(buf,card[index]);
			}
			
			// play instruction, second and third player in a round
			if(buf[0] == 'p')
			{
				winCard[0] = buf[6];
				winCard[1] = buf[7];
				index = playCard(newCard,winCard); //call playCard function
				printf("Child %d: play %s\n",childNO,card[index]);
				strcpy(buf,card[index]);
			}
			
			// lastplay instruction, the last player in a round
			if(buf[0] == 'l')
			{
				winCard[0] = buf[10];
				winCard[1] = buf[11];
				index = lastPlayCard(newCard,winCard); //call lastPlayCard function
				printf("Child %d: play %s\n",childNO,card[index]);
				strcpy(buf,card[index]);
			}
			
			// done instruction, break the loop, game over
			if (buf[0] == 'd')
				break;
			
			// write the card to parent
			if (childNO == 1)
				write(fdc1[1],buf,6); 
			if (childNO == 2)
				write(fdc2[1],buf,6); 
			if (childNO == 3)
				write(fdc3[1],buf,6); 
			if (childNO == 4)
				write(fdc4[1],buf,6); 
		}
		
		
		close(fdc1[1]);		 //close child out
		close(fdc2[1]);
		close(fdc3[1]);
		close(fdc4[1]);
		
		close(fdp1[0]);		//close parent in
		close(fdp2[0]);
		close(fdp3[0]);
		close(fdp4[0]);
	} 
	else  // parent process
	{ 
		close(fdp1[0]);		// close parent in
		close(fdp2[0]);
		close(fdp3[0]);
		close(fdp4[0]); 
		
		close(fdc1[1]);		// close child out 
		close(fdc2[1]);
		close(fdc3[1]);
		close(fdc4[1]);
		int winner = 1;
		int score[5] = {0,0,0,0,0};
		int add;
		char played[4][3];
		char winCard[3];
		int turn;
		int round = 1;
		turn = 1;
		char childpid[5][10];
		
		// read the pid from children and print the pid
		read(fdc1[0],buf,80);
		strcpy(childpid[1],buf);
		read(fdc2[0],buf,80);
		strcpy(childpid[2],buf);
		read(fdc3[0],buf,80);
		strcpy(childpid[3],buf);
		read(fdc4[0],buf,80);
		strcpy(childpid[4],buf);
		printf("Parent: the child players are %s %s %s %s\n",childpid[1],childpid[2],childpid[3],childpid[4]);
		sleep(1);
		
		// start the game, total 13 rounds
		while (round<14)
		{
			add = 0;
			turn = winner;  // the winner of last round starts to play
			
			// give the instruction to the first player in a round
			strcpy(buf,"toplay");
			if (turn == 1)
				write(fdp1[1],buf,6);
			if (turn == 2)
				write(fdp2[1],buf,6);
			if (turn == 3)
				write(fdp3[1],buf,6); 
			if (turn == 4)
				write(fdp4[1],buf,6);
			
			// read the card played from the child
			if (turn == 1)
			{
				read(fdc1[0],buf,80);
				printf("parent: child 1 plays %s\n",buf);
			}
			if (turn == 2)
			{
				read(fdc2[0],buf,80);
				printf("parent: child 2 plays %s\n",buf);
			}
			if (turn == 3)
			{
				read(fdc3[0],buf,80);
				printf("parent: child 3 plays %s\n",buf);
			}
			if (turn == 4)
			{
				read(fdc4[0],buf,80);
				printf("parent: child 4 plays %s\n",buf);
			}
			
			add = add + addScore(buf);   // compute the score added in this round
			strcpy(winCard,buf);   // record the winner's card
			
			// second and third player in a round
			for(i = 0; i < 2; i++)
			{
				strcpy(buf,"played");
				strcat(buf,winCard);
				turn = nextTurn(turn);		// call nextTrun function to decide the next player
				
				// write the instuction and the winner card to child
				if (turn == 1)
					write(fdp1[1],buf,9);
				if (turn == 2)
					write(fdp2[1],buf,9);
				if (turn == 3)
					write(fdp3[1],buf,9); 
				if (turn == 4)
					write(fdp4[1],buf,9);
				
				// read the card played from child
				if (turn == 1)
				{
					read(fdc1[0],buf,80);
					printf("parent: child 1 plays %s\n",buf);
				}
				if (turn == 2)
				{
					read(fdc2[0],buf,80);
					printf("parent: child 2 plays %s\n",buf);
				}
				if (turn == 3)
				{
					read(fdc3[0],buf,80);
					printf("parent: child 3 plays %s\n",buf);
				}
				if (turn == 4)
				{
					read(fdc4[0],buf,80);
					printf("parent: child 4 plays %s\n",buf);
				}
				
				add = add + addScore(buf);		// compute the score added in this round
				
				//	check the winner and winner's card after every card played
				if (checkWin(winCard, buf) == 1)
				{
					winner = turn;
					strcpy(winCard,buf);
				}
			}
			
			//give instruction and winner's card to the last player
			strcpy(buf,"lastplayed");
			strcat(buf,winCard);
			turn = nextTurn(turn);
			if (turn == 1)
				write(fdp1[1],buf,13);
			if (turn == 2)
				write(fdp2[1],buf,13);
			if (turn == 3)
				write(fdp3[1],buf,13); 
			if (turn == 4)
				write(fdp4[1],buf,13);
			
			//read the card played from the last player	
			if (turn == 1)
			{
				read(fdc1[0],buf,80);
				printf("parent: child 1 plays %s\n",buf);
			}
			if (turn == 2)
			{
				read(fdc2[0],buf,80);
				printf("parent: child 2 plays %s\n",buf);
			}
			if (turn == 3)
			{
				read(fdc3[0],buf,80);
				printf("parent: child 3 plays %s\n",buf);
			}
			if (turn == 4)
			{
				read(fdc4[0],buf,80);
				printf("parent: child 4 plays %s\n",buf);
			}
				
			add = add + addScore(buf);		// compute the score added in this round
				
			//	check the winner and winner's card after the last card played
			if (checkWin(winCard, buf) == 1)
			{
				winner = turn;
				strcpy(winCard,buf);
			}
			
			// print the winner
			printf("Parent: child %d wins the trick\n",winner);
			
			// add the score to the winner
			score[winner] = score[winner] + add;
			round++;
		}
		
		// give instruction to all children to end the process
		strcpy(buf,"done");
		write(fdp1[1],buf,5); 
		write(fdp2[1],buf,5); 
		write(fdp3[1],buf,5); 
		write(fdp3[1],buf,5); 
		
		close(fdp1[1]);		/* close child out */
		close(fdp2[1]);
		close(fdp3[1]);
		close(fdp4[1]);
		
		close(fdc1[0]);
		close(fdc2[0]);
		close(fdc3[0]);
		close(fdc4[0]);
		
		// check whether a player has got all hearts as well as spade Q
		for (i = 1; i<5; i++)
		{
			if (score[i] == 26)  // if found, his score becomes 0, others are 26
			{
				score[i] = 0;
				score[nextTurn(i)] = 26;
				score[nextTurn(nextTurn(i))] = 26;
				score[nextTurn(nextTurn(nextTurn(i)))] = 26;
				break;
			}
		}
		// pirnt their score
		printf("Parent: score = <%d %d %d %d>", score[1],score[2],score[3],score[4]);
		
		wait(NULL);
		
	}	
}

// common function: convert the card into number 
int convert(char card[2])
{
	int newCard = 0;
	switch(card[0])		// different suits has different hundred number
    {
		case 'S' : newCard = 400; break;
		case 'H' : newCard = 100; break;
		case 'C' : newCard = 200; break;
		case 'D' : newCard = 300; break;
		default: break;
	}

    switch(card[1])		// A is the largest
	{
		case 'A' : newCard = newCard + 14; break;
		case '2' : newCard = newCard + 2; break;
		case '3' : newCard = newCard + 3; break;
		case '4' : newCard = newCard + 4; break;
		case '5' : newCard = newCard + 5; break;
		case '6' : newCard = newCard + 6; break;
		case '7' : newCard = newCard + 7; break;
		case '8' : newCard = newCard + 8; break;
		case '9' : newCard = newCard + 9; break;
		case 'T' : newCard = newCard + 10; break;
		case 'J' : newCard = newCard + 11; break;
		case 'Q' : newCard = newCard + 12; break;
		case 'K' : newCard = newCard + 13; break;
		default: break;
    }
	return newCard;		
}

// parent function: decide the next turn
int nextTurn(int turn)
{
	if (turn == 4)
		return 1;
	else
		return turn + 1;
}

// child function: the strategy of first player in a round
int toplay(int newCard[])
{
	int i;
	int index = 0;
	for (i=0;i<cardNum;i++)		// get a card that has not been played
	{
		if (newCard[i] > 0)
		{
			index = i;
			break;
		}
	}
	
	for (i=0;i<cardNum;i++)		// find the smallest rank of cards
	{
		if (newCard[i] % 100 < newCard[index] % 100 && newCard[i] > 0)
			index = i;
	}
	newCard[index] = 0;		// remove the card from hand
	return index;
}

// child function: the strategy of second and third player
int playCard(int newCard[], char winCard[])
{
	int i;
	int index = 99;
	int win = convert(winCard);
	for (i=0;i<cardNum;i++)		// find a card in the same suit
	{
		if (newCard[i] - (newCard[i] % 100) == win - (win % 100))
		{
			index = i;
		}
	}
	if (index == 99)	// can not find a card in the same suit
		index = discard(newCard);		//call discard function
	else		// find a card in the same suit
	{
		for (i=0;i<cardNum;i++)
		{
			if (newCard[i] - (newCard[i] % 100) == win - (win % 100))
			{
				// if the card is larger than the largest card played, try to play the smallest as possible
				if (newCard[index] > win)
				{
					if (newCard[i] < newCard[index])	
						index = i;
				}
				
				// if the card is smaller than the largest card played, try to play the largest as possible
				else
				{
					if (newCard[i] > newCard[index] && newCard[i] < win)
						index = i;
				}
			}
		}
	}
	newCard[index] = 0;		// remove the card from hand
	return index;
}

// child function: the strategy of the last player in a round
int lastPlayCard(int newCard[], char winCard[])
{
	int i;
	int index = 99;
	int win = convert(winCard);
	for (i=0;i<cardNum;i++)			// find a card in the same suit
	{
		if (newCard[i] - (newCard[i] % 100) == win - (win % 100))
		{
			index = i;
		}
	}
	if (index == 99)		// if not find, call disCard function
		index = discard(newCard);
	else		// find the card in the same suit
	{
		for (i=0;i<cardNum;i++)		// find the smallest in the same suit
		{
			if (newCard[i] - (newCard[i] % 100) == win - (win % 100) && newCard[i] < newCard[index])
			{
				index = i;
			}
		}
		
		// smallest rank is larger than the largest card, win the round anyway, try to play the largest to same good card
		if(newCard[index] > win)
		{
			for (i=0;i<cardNum;i++)
			{
				if (newCard[i] - (newCard[i] % 100) == win - (win % 100) && newCard[i] > newCard[index])
				{
					index = i;
				}
			}
		}
		else  //lose the round anyway, try to play the largest to same the good card
		{
			for (i=0;i<cardNum;i++)
			{
				if (newCard[i] - (newCard[i] % 100) == win - (win % 100) && newCard[i] > newCard[index] && newCard[i] < win)
				{
					index = i;
				}
			}
		}
	}
	newCard[index] = 0;
	return index;
}

//  child function: discard strategy
int discard(int newCard[])
{
	int i;
	int index = 0;
	for (i=0;i<cardNum;i++)
	{
		if (newCard[i] == 412)		// find spade Q
		{
			index = i;
			break;
		}
		else if (newCard[i] > 100 && newCard[i] < 200)		// find the largest rank of heart 
			index = i;
	}
	
	if (index == 0)		//no spade Q or hearts
	{
		for (i=0;i<cardNum;i++)
		{
			if (newCard[i] % 100 > newCard[index] % 100)	//find largest rank
				index = i;
		}
	}
	else
	{
		for (i=0;i<cardNum;i++)
		{
			if (newCard[i] == 412)		// find spade Q
			{
				index = i;
				break;
			}
			else if (newCard[i] > newCard[index] && newCard[i] > 100 && newCard[i] < 200)		// find the largest rank
				index = i;
		}
	}
	
	return index;
}

//  parent function: check the winner of a round
int checkWin(char winCard[2], char playedCard[])
{
	int win = convert(winCard);
	int played = convert(playedCard);
	if (win - (win % 100) == played - (played % 100))
	{
		if (played > win)
			return 1;
		else 
			return 0;
	}
	else 
		return 0;
}

//	parent function: check the score should be added of a round
int addScore(char card[2])
{
	int new = convert(card);
	if (new == 412)
		return 13;
	else if (new > 100 && new < 200)
		return 1;
	else
		return 0;
}


	