#include <stdio.h>
#include <stdlib.h>

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
} NodeInHashTable;
typedef struct hashTable
{
    unsigned int size;
    NodeInHashTable **table;
} hashTable;

int string2int(char e[])
{
    char negative = '0';
    int i = 0;
    if (e[0] == '-')
    {
        i++;
        negative = '1';
    }
    int Eur = 0, Cent = 0;
    while (e[i] != ',' && e[i] != '\0')
    {
        Eur = Eur * 10 + (e[i] - '0');
        i++;
    }
    if (e[i] == ',')
    {
        i++;
        int centMultiplier = 10;
        while (e[i] != '\0')
        {
            Cent = Cent * 10 + (e[i] - '0');
            i++;
            centMultiplier *= 10;
        }
        if (centMultiplier == 100)
        {
            Cent *= 10;
        }
    }

    Cent = Eur * 100 + Cent;
    if (negative == '1')
    {
        Cent = -Cent;
    }

    return Cent;
}

void copyString(char *dest, char *src)
{
    while (*src)
        *dest++ = *src++;
    *dest = '\0';
}

hashTable *createHashTable(int i)
{
    return NULL;
}

Person *create_person(char *name, char *surname, char *birthdate, double balance)
{
    Person *person = (Person *)malloc(sizeof(Person));
    if (person == NULL)
        return NULL;

    copyString(&person->name, name);
    copyString(&person->surname, surname);
    copyString(&person->birthdate, birthdate);

    return person;
}


int hash(char *str)
{
    int H = 0;
    for (int i = 0; str[i] != '\0'; i++)
    {
        H = 31 * H + str[i];
    }
    return H;
}

int main()
{
    //hi
    hashTable *HT = createHashTable(100);
    char DO;
    int balance;

    while (scanf(" %c", &DO) == 1) // while something is in buffer
    {
        if (DO == 'i') // insert
        {
            char name[SIZE], surname[SIZE], birthDate[11];
            char e[SIZE];
            scanf("%s %s %s %s", name, surname, birthDate, e);
            balance = string2int(e);
            printf("%d", balance);
        } else if (DO == 'd') // delete
        {
            char name[SIZE], surname[SIZE], birthDate[11];
            scanf("%s %s %s", name, surname, birthDate);
        } else if (DO == 's') // search
        {
            char name[SIZE], surname[SIZE], birthDate[11];
            scanf("%s %s %s", name, surname, birthDate);
        } else if (DO == 'u') // update
        {
            char name[SIZE], surname[SIZE], birthDate[11], e[SIZE];

            scanf("%s %s %s %s", name, surname, birthDate, e);
            balance = string2int(e);
        }
    }
    return 0;
}
