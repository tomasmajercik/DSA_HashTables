#include <stdio.h>

#define SIZE 30

typedef struct Person
{
    char name;
    char surname;
    char birthdate;
    double accountBalance;
} Person;
typedef struct NodeInHashTable
{
    char key;
    Person data;
    struct NodeInHashTable *next;
}NodeInHashTable;
typedef struct hashTable
{
    unsigned int size;
    NodeInHashTable **table;
}hashTable;


hashTable *createHashTable(int i)
{
    return NULL;
}

int main()
{
    hashTable *HT = createHashTable(100);
    char DO;
    while (scanf(" %c", &DO) == 1) // while something is in buffer
    {
        if (DO == 'i') // insert
        {
            char name[SIZE], surname[SIZE], birthDate[11];
            double balance;

            scanf("%s %s %s %lf", name, surname, birthDate, &balance);
        }
        else if (DO == 'd') // delete
        {
            char name[SIZE], surname[SIZE], birthDate[11];
            scanf("%s %s %s", name, surname, birthDate);
        }
        else if (DO == 's') // search
        {
            char name[SIZE], surname[SIZE], birthDate[11];
            scanf("%s %s %s", name, surname, birthDate);
        }
        else if(DO == 'u') // update
        {
            char name[SIZE], surname[SIZE], birthDate[11];
            double balance;

            scanf("%s %s %s %lf", name, surname, birthDate, &balance);
        }
    }
    return 0;
}
