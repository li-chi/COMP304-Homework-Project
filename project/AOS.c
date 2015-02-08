/*=========================================================================
Course:		COMP304: Operating System
Project title:	AOS
Group members:	GUO, Donghao <11807347d>
		HE, Jiqiong <11804425d>
		JU, Sida <11839382d>
		LI, Chi <11808749d>
		ZHOU, Guangyu <11816172d>
Date:		05/05/2013
=========================================================================
System:		apollo
Compile:	gcc AOS.c -o aos
Run:		./aos x1 x2 x3...
		(for example, <./aos paul peter lucy mary>, they are users
		in the program. The number of users should be in the range
		of 3 to 10.)
Alternate:	./aos x1 x2 x3... < input.dat
-------------------------------------------------------------------------
Description:
This program produce the Appointment Organizer System (AOS).
-------------------------------------------------------------------------
User Interface:

addClass -xxx YYYY-MM-DD hh:mm n.n
addMeeting -xxx YYYY-MM-DD hh:mm n.n xxx xxx
addGathering -xxx YYYY-MM-DD hh:mm n.n xxx xxx
([-xxx] is the name of caller.
 [xxx] at the end of addMeeting and addGathering are the names of callees.

 Both caller and callees' names must be the same with the names which are
 input in the command line. For example, <./aos paul peter lucy mary> is
 input in the command line, when callling addClass/addMeeting/addGathering,
 the names input should be selected from paul, peter, lucy and mary, and
 the upper or lower case should also be the same, that is, Paul is not
 correct.

 YYYY is the date of year, and must have four figures, e.g. 2013.
 MM is the date of month, and must have two figures, e.g. <05>.
 DD must have two figures, e.g. <05>.
 hh must have two figures, e.g. <08>.
 mm must have two figures, e.g. <59>.
 n.n is the duration of a activity and must have 3 figures, e.g. <3.0>,
 and just <3> is not correct.
)

printSchd -xxx -[f/t/r] [filename]
([-xxx] is the name of caller.
 -f is to print timetable+rejected list. -r is to print just rejected list.
 -t is to print just time table.
 [filename] saves the timetable and rejected list for [xxx], and should
 indicate the  type of the file, e.g. <paulschd.dat>.
)

endProgram
(This ends our program completely, collect all children processes and
close all files.
)
========================================================================= */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

struct list* listCreate();
double convertNum(char num_char);

struct list{
	char date[12]; //yyyy-mm-dd
	char time[7];//hh:mm
	char end[7]; //hh:mm
	char callee[10][10]; //name of the callee
	char type[12]; // Class/Meeting/Gathering
	char reason[50];
	struct list* next; //next activity
};

struct list * listCreate(){		//create the head of the list
	struct list * a = NULL;
	a = (struct list *)malloc(sizeof(struct list));		//allocate the momery
	if (a == NULL)		// erro
    {
        printf("newNode ERROR: memory size is 0\n");
    }
	a->next = NULL;
	return a;		//return the head of the list
}

struct list* searchPosition(struct list* head, char date[12], char time[7])
{		//search the position the add the node
    struct list* temp = head->next;
    struct list* prev = head;
    while (temp != NULL){
        if (strcmp(date,temp->date) <0)		//compare the date
            break;
        else if (strcmp(date,temp->date) >0){}
        else
        {
            if (strcmp(time,temp->time) < 0)		//compare the time
                break;
            else if (strcmp(time,temp->time) >= 0) {}
        }
        temp = temp->next;
        prev = prev->next;
    }
    return prev;
}

void listAdd(struct list* head, char date[12], char time[7], char end[7], char callee[10][10], char functiontype[14], char unavailable[50]){
	struct list* position = searchPosition(head, date, time);	//search the position to add the node
	int i;
	struct list* temp = (struct list*) malloc(sizeof(struct list));		//allocate memery
	strcpy(temp->date,date);
	strcpy(temp->time,time);
	strcpy(temp->end,end);
	if (strcmp (functiontype,"addClass") ==0)
		strcpy(temp->type,"Class");
	else if (strcmp (functiontype,"addMeeting") ==0)
		strcpy(temp->type,"Meeting");
	else if (strcmp (functiontype,"addGathering") ==0)
		strcpy(temp->type,"Gathering");
	for (i=0; i<10; i++)
	{
		strcpy(temp->callee[i],callee[i]);
	}
	if (unavailable[0] != '\0')
	{
		strcpy(temp->reason, unavailable);
		strcat(temp->reason, "is(are) not available.");
	}
	temp->next = position->next;
	position->next = temp;
}

void deleteList(struct list* prev){
    prev->next = prev->next->next;
    free((void *) prev->next);  // free memory
}

void printlist(struct list* head){		//print the linkedlist
	int i = 0;
	struct list* temp = head->next;
	while (temp != NULL){
		printf("%s\t%s\t%s\t%s\t",temp->date,temp->time,temp->end, temp->type);
		if (strcmp(temp->type, "Class") == 0)
			printf("\t");
		if (temp->reason[0] == '\0')	//no reason, it is schedule
		{
			if (strcmp(temp->type, "Class") == 0)		// no callees in the class
				printf("-");
			else
				while (temp->callee[i][0] != '\0'){
					printf("%s ",temp->callee[i]);
					i++;
				}
		}
		else	// printf the reason
		{
			printf("%s",temp->reason);
		}
		printf("\n");
        temp = temp->next;
	}
}

int checkAvailable(struct list* head, char date[12],char time[7],char end[7]){		//for each child to check whether is available to add activity
    struct list* temp = head;
    while (temp != NULL){
        if (strcmp(date,temp->date) == 0)	//if it is on the same date
        {
            if(strcmp(end,temp->time) <= 0 || strcmp(time,temp->end) >= 0) {}	//the activity ends before each activity starts or starts after each ends
            else																// --> it is available
            {
                return 0;		// return 0 to indicate not available
            }
        }
        temp = temp->next;
    }
    return 1;		// return 1 to indicate available
}

void calEnd(char start[7], char end[7], char dur[5]){	//calculate the end time of an activity
    double hour_double = convertNum(start[0]) * 10.0 + convertNum(start[1]);
    double min_double = (convertNum(start[3]) * 10.0 + convertNum(start[4]))/60.0;
    double start_double = hour_double + min_double;		//present start time as a double type
    double dur_double = convertNum(dur[0]) + convertNum(dur[2])/10.0;	//present duration as a double
    double end_double = start_double + dur_double;		//compute the end time as a double type
    int hour_int = (int)end_double;		//cast the hour to a integer type
	sprintf(end,"%d",hour_int);		//cast the hour to a string
    strcat(end,":");
	int min_int = (end_double - hour_int) * 60;		//calculate the minite as a integer
	char min_str[5];
	sprintf(min_str,"%d",min_int);		//cast the minite to the string
	if (min_int < 10)
	{
		strcat(end,"0");
		strcat(end, min_str);
	}
	else
		strcat(end, min_str);
}

double convertNum(char num_char){		//convert character to a double type
    double num = 10;
    switch(num_char){
        case '1' : num = 1.0; break;
        case '2' : num = 2.0; break;
        case '3' : num = 3.0; break;
        case '4' : num = 4.0; break;
        case '5' : num = 5.0; break;
        case '6' : num = 6.0; break;
        case '7' : num = 7.0; break;
        case '8' : num = 8.0; break;
        case '9' : num = 9.0; break;
        case '0' : num = 0.0; break;
        default: break;
    }
    return num;
}

int countlist(struct list* head)	//count the number of activities in the list
{
	int count = 0;
	struct list* temp = head->next;
    while (temp != NULL){
		count++;
		temp = temp->next;
	}
	return count;
}

void printSchd(char u[10],struct list* sch_h,struct list* rej_h,char r[1]) //print the content of timetable and rejected list.
{
    if(r[0] == 'f') //full
    {
        printf("%s, you have %d appointments.\n",u,countlist(sch_h));
        printf("***Appointment Schedule***\n");
        printf("Date\t\tStart\tEnd\tType\t\tPeople\n");
        printf("===========================================================================\n");
        printlist(sch_h);
        printf("\t- End -\n");
        printf("===========================================================================\n");
        printf("***Rejected List***\n");
        printf("%s, you have %d appointments rejected.\n",u,countlist(rej_h));
        printf("Date\t\tStart\tEnd\tType\t\tReason\n");
        printf("===========================================================================\n");
        printlist(rej_h);
        printf("\t- End -\n");
    }
    else if (r[0] == 't') //just timetable
    {
        printf("%s, you have %d appointments.\n",u,countlist(sch_h));
        printf("***Appointment Schedule***\n");
        printf("Date\t\tStart\tEnd\tType\t\tPeople\n");
        printf("===========================================================================\n");
        printlist(sch_h);
        printf("\t- End -\n");
    }
    else // just rejected list
    {
        printf("***Rejected List***\n");
        printf("%s, you have %d appointments rejected.\n",u, countlist(rej_h));
        printf("Date\t\tStart\tEnd\tType\t\tReason\n");
        printf("===========================================================================\n");
        printlist(rej_h);
        printf("\t- End -\n");
    }
}

int main(int argc, char* argv[]){
	int num_of_child=argc-1, i=0, m, j, k, l, num_callee, h;
	int pid, cid, fdP2C[num_of_child][2], fdC2P[num_of_child][2];
	char childname[10], end_sig[1], return_value[1], add[1],report[1],filename[20],user[10], callee_str[1];
	char functiontype[14], caller[10], ymd[12], hm[7], dur[5], end[7], reject_name[50],callee[num_of_child][10];

	printf("~~WELCOME TO AOS~~\nnum_of_child=%d\n",num_of_child);

	for(i=0;i<num_of_child;i++){	  //create 2 series of pipes
		if(pipe(fdP2C[i])<0){
			printf("Pipe creation error\n");
			exit(1);
		}
		if(pipe(fdC2P[i])<0){
			printf("Pipe creation error\n");
			exit(1);
		}
	}
	for(m=0;m<num_of_child;m++){	   // fork child processes
		pid=fork();
		if(pid==0 || pid<0)            //break out if it's a child process
			break;
	}
	if (pid < 0) {
		printf("Fork failed\n");
		exit(1);
	}

    /* child */
	if (pid == 0){
        for(i=0;i<num_of_child;i++){   //close unnecessary pipes
			close(fdP2C[i][1]);
			close(fdC2P[i][0]);
		}
		strcpy(childname,argv[m+1]);
		struct list* sch_head = listCreate();
		struct list* rej_head = listCreate();
        while (1){
            memset(functiontype, 0, 14);     //set all the container to be empty in every turn
            memset(ymd, 0, 12);
            memset(hm, 0, 7);
            memset(dur, 0, 5);
            memset(end, 0, 7);
            memset(report, 0, 1);
            memset(filename, 0, 20);
			memset(reject_name, 0, 50);
			memset(callee, 0, sizeof(callee));
            callee_str[0] = 0;
			end_sig[0] = 0;     //end signal to terminate child process
            return_value[0] = 0;    //child return to parent for its own availibility
            add[0] = 0;//parent to tell whether all callee are availible and to add
			
            read(fdP2C[m][0], end_sig, 1);
            if(end_sig[0] == 'N')
                break;
            read(fdP2C[m][0], functiontype, 14);
            if(strcmp(functiontype,"printSchd") == 0){
                read(fdP2C[m][0],report,1);
                read(fdP2C[m][0],filename,20);
                freopen(filename,"w",stdout); //redirect the content of standard input into the file [filename]
                printSchd(argv[m+1],sch_head,rej_head,report);
                fclose(stdout);
            }else{
                read(fdP2C[m][0], ymd, 12);
                read(fdP2C[m][0], hm, 7);
                read(fdP2C[m][0], dur, 5);
                calEnd(hm, end, dur);
                if(strcmp(functiontype,"addClass") != 0){
                    read(fdP2C[m][0], callee_str, 1);
                    num_callee = (int)convertNum(callee_str[0]);
					for (h = 0;h < num_callee;h++)
                    {
                        read(fdP2C[m][0], callee[h], 10);
                    }
                }
				if(checkAvailable(sch_head, ymd, hm, end))
                    return_value[0]= 'Y';
                else
                    return_value[0]= 'N';
                write(fdC2P[m][1], return_value, 1);
                read(fdP2C[m][0], add, 1);
                if(add[0] == 'Y')
                    listAdd(sch_head, ymd, hm, end, callee, functiontype, reject_name);
                else
                {
					read(fdP2C[m][0], reject_name, 50);
					listAdd(rej_head, ymd, hm, end, callee, functiontype, reject_name);
				}
                if(strcmp(functiontype,"addClass") != 0){
                    for(k = 0;k < num_callee;k++)
                        memset(callee[k], 0, 10);
                }
            }
		}

        for(i=0;i<num_of_child;i++){   //close pipes
            close(fdP2C[i][0]);
            close(fdC2P[i][1]);
        }
        exit(0);
	}

	/* parent */
	else{
		for(i=0;i<num_of_child;i++){   //close unnecessary pipes
			close(fdP2C[i][0]);
			close(fdC2P[i][1]);
		}
		while(1){
			printf("Please enter appointment:\n");
            memset(functiontype, 0, 14);    //reset parameters every turn
            memset(ymd, 0, 12);
            memset(hm, 0, 7);
            memset(dur, 0, 5);
            memset(user, 0, 10);
            memset(report, 0, 1);
            memset(filename, 0, 20);
			memset(reject_name, 0, 50);
			memset(callee, 0, sizeof(callee));
            callee_str[0] = 0;
            end_sig[0] = 'Y';
            return_value[0] = 0;
            add[0] = 'N';
            i = 0, l = 0;
			FILE* fpin=stdin;

			fscanf(fpin,"%s",functiontype);     //read input
			if(strcmp(functiontype,"printSchd") != 0 && strcmp(functiontype,"endProgram") != 0)
            {
                fscanf(fpin," -%s %s %s %s",caller,ymd,hm,dur);
                while(getc(fpin)!='\n'){
                    fscanf(fpin," %s",callee[i]);
                    i++;
                }
            }
            num_callee = i;     //number of callee
			if(strcmp(functiontype,"addClass")==0){  //addclass
			    for(j = 0; j<num_of_child; j++)
			    {
			        if(strcmp(argv[j+1],caller)==0)
			        {
			            write(fdP2C[j][1],end_sig,1);   //pass to children
                        write(fdP2C[j][1], functiontype, 14);
                        write(fdP2C[j][1], ymd, 12);
                        write(fdP2C[j][1], hm, 7);
                        write(fdP2C[j][1], dur, 5);
                        read(fdC2P[j][0], return_value, 1);
                        if(return_value[0]!='Y')
                        {
                            printf("-> [Rejected] - %s is unavailable\n", argv[j+1]);
                            write(fdP2C[j][1],add,1);
							strcat(reject_name, argv[j+1]);
                            strcat(reject_name, " ");
							write(fdP2C[j][1], reject_name, 50);    //pass reject reason
                        }
                        else
                        {
                            printf("-> [Accepted]\n");
                            add[0] = 'Y';
                            write(fdP2C[j][1], add, 1);
                        }
			        }
			    }
			}
			else if(strcmp(functiontype,"addMeeting") == 0||strcmp(functiontype,"addGathering")==0){
                strcpy(callee[num_callee],caller);
				memset(callee[num_callee+1],0,10);
                num_callee++;
                for(k = 0;k < num_callee;k++){
                    for(j = 0;j < num_of_child;j++){
                        if(strcmp(argv[j+1], callee[k]) == 0){   //check for callee mentioned
                            write(fdP2C[j][1], end_sig, 1);
                            write(fdP2C[j][1], functiontype, 14);
                            write(fdP2C[j][1], ymd, 12);
                            write(fdP2C[j][1], hm, 7);
                            write(fdP2C[j][1], dur, 5);
                            sprintf(callee_str,"%d",num_callee);
                            write(fdP2C[j][1], callee_str, 1);  //pass num of callee
							for (h = 0;h < num_callee;h++)
								write(fdP2C[j][1], callee[h], 10);
                            read(fdC2P[j][0], return_value, 1);     //read check available result from every child
                            if(return_value[0] != 'Y'){
                                printf("-> [Rejected] - %s is unavailable\n", argv[j+1]);
                                l++;
								strcat(reject_name, argv[j+1]);
                                strcat(reject_name, " ");
                            }
                        }else
                            continue;
                    }
                }
                if(l == 0){     //if no unavailible callee
                    printf("-> [Accepted]\n");
                    add[0] = 'Y';
                    for(k = 0;k < num_callee;k++){
                        for(j = 0;j < num_of_child;j++){
                            if(strcmp(argv[j+1], callee[k]) == 0){    //send add instruction to all callee
                                write(fdP2C[j][1], add, 1);
                            }
                        }
                    }
                }else{
                    for(k = 0;k < num_callee;k++){
                        for(j = 0;j < num_of_child;j++){
                            if(strcmp(argv[j+1], callee[k]) == 0){    //send not add instruction to all callee
                                write(fdP2C[j][1], add, 1);
								write(fdP2C[j][1], reject_name, 50);    //pass reject reason
                            }
                        }
                    }
                }
                for(k = 0;k < num_callee;k++)
                    memset(callee[k], 0, 10);
			}
			else if(strcmp(functiontype,"printSchd")==0){

			    fscanf(fpin," -%s -%c %s",user,report,filename);
			    if(report[0] == 'f'||report[0] == 't'||report[0] == 'r')
			    {
			        for(j=0;j<num_of_child;j++)
                    {
                        if(strcmp(user,argv[j+1]) == 0){
                            write(fdP2C[j][1], end_sig, 1);
                            write(fdP2C[j][1], functiontype, 14);
                            write(fdP2C[j][1],report,1);
                            write(fdP2C[j][1],filename,20);
                            break;
                        }
                    }
			    }
			    else
                    fprintf(stderr, "error\n");
                //It is to print time table and the rejected list for a user.
                //printSchd ®Cxxx ®C[f/t/r] [filename]
			}
			else if(strcmp(functiontype,"endProgram")==0){
                // This ends the program completely, upon collecting
                //all the child processes and closing all the files.
                end_sig[0] = 'N';
                for(i=0;i<num_of_child;i++){
                    write(fdP2C[i][1], end_sig, 1);    //send end signal to all child process
                    close(fdP2C[i][1]);     //close pipes
                    close(fdC2P[i][0]);
                }
                sleep(1);
                for(i = 0;i < num_of_child;i++){    //collect children
                    cid = wait(NULL);
                    printf("child collected: %d\n", cid);
                }
                exit(0);
			}
		}
	}
}
