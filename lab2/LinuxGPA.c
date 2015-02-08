#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// Name: LI Chi
// ID: 11808749D

int main(int argc, char *argv[])
{
    /* argv[0] is the name of the program */
    printf("This program name is %s\n", argv[0]);

    if (strcmp(argv[1],"-w") == 0)
    {
        systemW(argc - 2, argv); // call systemW
    }
    else
    {
        systemP(argc-  1,argv); // call systemP
    }
}

systemP(int num_subj,char *grade[])
{
    int i,count=0;
    int num = num_subj;
    int numb = num_subj;
    char in_grade;
    float in_gp,sum_gp = 0;
    for (i = 1; i <= numb; i++)
    {
        if (strlen(grade[i]) == 1)
        {
            in_grade = grade[i][0];
            switch (in_grade)
            {
                case 'A': in_gp = 4.0; break;
                case 'B': in_gp = 3.0; break;
                case 'C': in_gp = 2.0; break;
                case 'D': in_gp = 1.0; break;
                case 'F': in_gp = 0.0; break;
                default: num_subj--; in_gp = 0.0; break;
            }
        }
        else if (strlen(grade[i]) == 2)
        {
            if (grade[i][1] == '+')
            {
                in_grade = grade[i][0];
                switch (in_grade)
                {
                    case 'A': in_gp = 4.5; break;
                    case 'B': in_gp = 3.5; break;
                    case 'C': in_gp = 2.5; break;
                    case 'D': in_gp = 1.5; break;
                    default: num_subj--; in_gp = 0.0; break;
                }
            }
            else
            {
                num_subj--; in_gp = 0.0;
            }
        }

        else
        {
            num_subj--; in_gp = 0.0;
        }
        sum_gp = sum_gp + in_gp;
        if (num_subj == num)
        {
            count++; // output each grade
            printf("Grade for subject %d is %s, GP %5.2f\n",count,grade[i],in_gp);
        }
        else
        {
            num--;
        }
    }
    float gpa = sum_gp/num_subj; // compute and output GPA
    if (gpa>4.0) gpa = 4.0;
    printf("Your GPA for %d subjects is %5.2f, under systemP\n", num_subj, gpa);
}

systemW(int num_subj, char *grade[])
{

    char in_grade;
    int count = 0;
    int num = num_subj;
    int numb = num_subj;
    int i;
    float in_gp,sum_gp = 0;
    for (i = 2; i <= numb+1; i++)
    {
        if (strlen(grade[i]) == 1)
        {
            in_grade = grade[i][0];
            switch (in_grade)
            {
                case 'M': in_gp = 4.3; break;
                case 'A': in_gp = 4.3; break;
                case 'O': in_gp = 4.0; break;
                case 'B': in_gp = 4.0; break;
                case 'T': in_gp = 3.7; break;
                case 'E': in_gp = 3.7; break;
                case 'R': in_gp = 3.3; break;
                case 'F': in_gp = 3.3; break;
                case 'D': in_gp = 3.0; break;
                case 'S': in_gp = 2.7; break;
                case 'H': in_gp = 2.3; break;
                case 'G': in_gp = 2.0; break;
                case 'C': in_gp = 1.3; break;
                case 'P': in_gp = 0.0; break;
                default: num_subj--; in_gp = 0.0; break;
            }
        }

        else if (strlen(grade[i]) == 2)
        {
            if (grade[i][1] == '2')
            {
                in_grade = grade[i][0];
                switch (in_grade)
                {
                    case 'M': in_gp = 1.7; break;
                    case 'D': in_gp = 1.0; break;
                    default: num_subj--; in_gp = 0.0; break;
                }
            }

            else if (grade[i][1] == '-')
            {
                in_grade = grade[i][0];
                switch (in_grade)
                {
                    case 'A': in_gp = 1.7; break;
                    case 'B': in_gp = 1.3; break;
                    case 'E': in_gp = 1.0; break;
                    case 'F': in_gp = 0.0; break;
                    default: num_subj--; in_gp = 0.0; break;
                }
            }

            else if (grade[i][1] == '+')
            {
                in_grade = grade[i][0];
                switch (in_grade)
                {
                    case 'A': in_gp = 3.0; break;
                    case 'B': in_gp = 2.7; break;
                    case 'E': in_gp = 2.3; break;
                    case 'F': in_gp = 2.0; break;
                    default: num_subj--; in_gp = 0.0; break;
                }
            }

            else
            {
                num_subj--; in_gp = 0.0;
            }
        }

        else
        {
            num_subj--; in_gp = 0.0;
        }
        sum_gp = sum_gp + in_gp;
        if (num_subj == num)
        {
            count++; //output each grade
            printf("Grade for subject %d is %s, GP %5.2f\n",count,grade[i],in_gp);
        }
        else
        {
            num--;
        }
    }
    float gpa = sum_gp/num_subj; // compute and output GPA
    if (gpa>4.0) gpa = 4.0;
    printf("Your GPA for %d subjects is %5.2f, under systemW\n", num_subj, gpa);
}

