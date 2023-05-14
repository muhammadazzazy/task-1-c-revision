#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

typedef struct Student
{
    char name[32];
    char ID[9];
    float GPA;
    struct Student *next;
} Student;

void append(Student** v, char name[32], char ID[9], float GPA)
{
    Student *new_student;
    new_student = (Student*)malloc(sizeof(Student));
    strcpy(new_student->name, name);
    strcpy(new_student->ID, ID);
    new_student->GPA = GPA;
    new_student->next = NULL;
    if(*v == NULL)
    {
        (*v) = new_student;
    }
    else
    {
        Student *temp;
        temp = (*v);
        while(temp->next != NULL)
        {
            temp = temp->next;
        }
        temp->next = new_student;
    }
    return;
}

void print_data(Student* v)
{
    Student *temp;
    temp = v;
    printf("Name ID GPA\n");
    while(temp->next != NULL)
    {
        printf("%32s %12s %10f\n", temp->name, temp->ID, temp->GPA);
        temp = temp->next;
    }
    printf("%32s %12s%10f\n", temp->name, temp->ID, temp->GPA);
    return;
}

bool write_data(Student *head, char file_name[])
{
    FILE* out;
    out = fopen(file_name, "w");
    if(out == NULL)
    {
        perror("Error");
        return 0;
    }
    if(head == NULL)
    {
        printf("Linked List is empty\n");
    }
    Student *ptr = NULL;
    ptr = head;
    while(ptr != NULL)
    {
        fprintf(out, "%s,%s,%f\n", (*ptr).name, (*ptr).ID, (*ptr).GPA);
        ptr = ptr->next;
    }
    fclose(out);
    return 1;
}

Student* get_data(char file_name[])
{
    Student* head = NULL;
    FILE* in;
    char name[32];
    char ID[9];
    float GPA;
    in = fopen(file_name, "r");
    if(in == NULL)
    {
        printf("Read unsuccessful\n");
    }
    char line[100];
    char* sp;
    while(fgets(line, 100, in) != NULL)
    {       
        sp = strtok(line, ",");
        strcpy(name, sp);
        
        sp = strtok(NULL, ",");
        
        strcpy(ID, sp);
        
        sp = strtok(NULL, ",");
        
        GPA = atof(sp);

        if(ferror(in))
        {
            printf("Error reading file.\n");
        }
        append(&head, name, ID, GPA);
    }
    fclose(in);
    return head;
}

void print_stats(Student* v)
{
    int count = 0;
    float sum = 0;
    float min = 4.0;
    float max = 0.0;
    float sum_of_squares = 0.0;
    Student *ptr = NULL;
    ptr = v;
    while(ptr != NULL)
    {
        sum += ptr->GPA;
        count++;
        if(ptr->GPA > max)
        {
            max = ptr->GPA;
        }
        if(ptr->GPA < min)
        {
            min = ptr->GPA;
        }
        sum_of_squares += pow(ptr->GPA, 2);
        ptr = ptr->next;
    }
    float average = sum / count;
    float std = pow((sum_of_squares / count) - pow(average, 2), 0.5);
    printf("Average GPA: %f\n", average);
    printf("Max GPA: %f\n", max);
    printf("Min GPA: %f\n", min);
    printf("Standard deviation: %f\n", std);
    return;
}

int main(int argc, char* argv[])
{
    Student *head = NULL;
    char choice;
    char file_name[32];
    printf("Choose whether to:\n");
    printf("a.\tRead students' data.\n");
    printf("b.\tEnter students' data.\n");
    printf("c.\tCalculate statistics of students' data, or\n");
    printf("d.\tExit the program.\n");
    scanf("%c", &choice);
    getchar();
    while(choice < 'a' || choice > 'd')
    {
        printf("Choose whether to:\n");
        printf("a.\tRead students' data.\n");
        printf("b.\tEnter students' data.\n");
        printf("c.\tCalculate statistics of students' data, or\n");
        printf("d.\tExit the program.\n");
        scanf("%c", &choice);
        getchar();
    }
    while(choice != 'd')
    {
        printf("Enter name of the file: ");
        scanf("%s", file_name);
        getchar();
        if(choice == 'a')
        {
            head = get_data(file_name);
            if(head != NULL)
            {
                print_data(head);   
            }
        }
        else if(choice == 'b')
        {
            bool flag;
            char ID[9];
            char name[32];
            float GPA;
            char character;
            printf("Enter s to stop: ");
            scanf("%c", &character);
            getchar();
            while(character != 's')
            {
                int i;
                do
                {
                    flag = true;
                    printf("Enter the student name: ");
                    scanf("%s", name);
                    getchar();
                    for(i = 0; i < strlen(name); i++)   
                    {
                        if(!isalpha(name[i]))
                        {
                            flag = false;   
                            break;
                        }
                        
                    }
                } while(!flag);
                do
                {
                    flag = true;
                    printf("Enter the student ID: ");
                    scanf("%s", ID);
                    getchar();
                    for(i = 0; i < strlen(ID); i++) 
                    {
                        if(!isdigit(ID[i]))
                        {   
                            flag = false;
                            break;
                        }
                    }
                } while(!flag);
                while(strlen(ID) < 9)
                {
                    printf("Enter ID: ");
                    scanf("%s", ID);
                    getchar();  
                }

                printf("Enter GPA: ");
                scanf("%f", &GPA);
                getchar();
                while(GPA < 0 || GPA > 4)
                {
                    printf("Enter GPA: ");
                    scanf("%f", &GPA);
                    getchar();
                }
                append(&head, name, ID, GPA);
                printf("Enter s if you want to stop: ");
                scanf("%c", &character);
                getchar();
            }
            flag = write_data(head, file_name);
            if(flag)
            {
                printf("Write successful\n");
            }
            else
            {
                printf("Write unsuccessful\n");
            }
        }
        else if(choice == 'c')
        {
            head = get_data(file_name);
            print_stats(head);
        }
        printf("Choose whether to:\n");
        printf("a.\tRead students' data.\n");
        printf("b.\tEnter students' data.\n");
        printf("c.\tCalculate statistics of students' data, or\n");
        printf("d.\tExit the program.\n");
        scanf("%c", &choice);
        getchar();
    }
    return 0;
}
