#include <stdio.h>
#include <stdlib.h>

#define SIZE 20
#define HASHNUM 31
#define TABLESIZE 10000

// struct definitions
typedef struct Person
{
    char name[SIZE];
    char surname[SIZE];
    char birthdate[12];
    int e;
    int c;
    struct Person *next; // Pointer to the next person in case of collisions
} Person;
typedef struct hashTable
{
    int size;
    Person **table;
} hashTable;
// string operations
void copyString(char *dest, const char *src)
{
    int i;
    for (i = 0; src[i] != '\0'; i++)
        dest[i] = src[i];
    dest[i] = '\0';
}
int stringLength(const char *s)
{
    int result = 0;
    while (s[result] != '\0')
        result++;
    return result;
}
int stringCmp(const char str1[], const char str2[])
{
    int i = 0;
    while (str1[i] != '\0' || str2[i] != '\0')
    {
        if (str1[i] != str2[i])
            return 1;
        i++;
    }
    return 0; // else they must be the same
}

// hash functions
Person *createPerson(const char *name, const char *surname, const char *birthdate, int e, int c)
{
    Person *person = (Person *) malloc(sizeof(Person));

    // Copy strings
    copyString(person->name, name);
    copyString(person->surname, surname);
    copyString(person->birthdate, birthdate);
    person->e = e;
    person->c = c;

    person->next = NULL; // set next person to NULL
    return person;
}
hashTable *createHashTable(int size)
{
    hashTable *ht = (hashTable *) malloc(sizeof(hashTable));
    if (ht == NULL)
        return NULL;

    ht->size = size;
    ht->table = (Person **) malloc(size * sizeof(Person *));
    if (ht->table == NULL)
    {
        free(ht);
        return NULL;
    }
    for (int i = 0; i < size; i++)
        ht->table[i] = NULL;

    return ht;
}
int hash(const char *firstname, const char *lastname, const char *date, int table_size) //?
{
    int result = 0;
    for (int i = 0; i < stringLength(firstname); i++)
        result = HASHNUM * result + firstname[i];
    for (int i = 0; i < stringLength(lastname); i++)
        result = HASHNUM * result + lastname[i];
    for (int i = 0; i < stringLength(date); i++)
        result = HASHNUM * result + date[i];

    if (result < 0)
        result *= -1;

    return result % table_size;
}

// hashMap functions
Person *search(hashTable *ht, const char *name, const char *surname, const char *birthdate)
{
    int index = hash(name, surname, birthdate, ht->size);
    Person *current = ht->table[index];
    while (current != NULL)
    {
        if (stringCmp(current->name, name) == 0 &&
            stringCmp(current->surname, surname) == 0 &&
            stringCmp(current->birthdate, birthdate) == 0)
        {
            return current;
        }
        current = current->next;
    }

    return NULL; // No match found
}
void insert(hashTable *ht, const char *name, const char *surname, const char *birthdate, int e, int c, char *newline)
{
    int index = hash(name, surname, birthdate, ht->size);

    Person *existingPerson = search(ht, name, surname, birthdate);
    if (existingPerson != NULL)
    {
        if (*newline == '1')
        {
            printf("\n");
        }
        printf("insert failed");
        *newline = '1';
        return;
    }

    // Create a new person
    Person *newPerson = createPerson(name, surname, birthdate, e, c);

    if (ht->table[index] == NULL)
    {
        ht->table[index] = newPerson;
        return;
    } else // If there's person at taht index, chain new person to it
    {
        Person *current = ht->table[index];
        while (current->next != NULL)
        {
            current = current->next;
        }
        current->next = newPerson;
    }
}
void update(hashTable *ht, const char *name, const char *surname, const char *birthdate, int e, int c, char *newline)
{
    Person *person = search(ht, name, surname, birthdate);

    if (person == NULL)
    {
        if (*newline == '1')
        {
            printf("\n");
        }
        printf("update failed");
        *newline = '1';
        return;
    }
    if (e < 0)
    {
        c *= -1;
    }
    int Cents = person->c + c;
    int Eur = person->e + e;

    if (Cents >= 100)
    {
        Cents -= 100;
        Eur += 1;
    } else if (Cents < 0)
    {
        Cents += 100;
        Eur--;
    }


    if (Eur >= 0)
    {
        person->e = Eur;
        person->c = Cents;
        return;
    } else
    {
        if (*newline == '1')
        {
            printf("\n");
        }
        printf("update failed");
        *newline = '1';
    }
}
void delete(hashTable *ht, const char *name, const char *surname, const char *birthdate, char *newline)
{
    int index = hash(name, surname, birthdate, ht->size);
    Person *current = ht->table[index];
    Person *prev = NULL;
    while (current != NULL)
    {
        if (stringCmp(current->name, name) == 0 &&
            stringCmp(current->surname, surname) == 0 &&
            stringCmp(current->birthdate, birthdate) == 0)
        {
            if (prev == NULL)
            {
                ht->table[index] = current->next;
            } else
            {
                prev->next = current->next;
            }

            free(current);
            return;
        }
        prev = current;
        current = current->next;
    }
    if (*newline == '1')
    {
        printf("\n");
    }
    printf("delete failed");
    *newline = '1';
}
// memory stuff
void freeAll(hashTable *ht)
{
    if (ht == NULL)
        return;

    for (int i = 0; i < ht->size; i++)
    {
        Person *current = ht->table[i];
        while (current != NULL)
        {
            Person *temp = current;
            current = current->next;
            free(temp);
        }
    }
    free(ht->table);
    free(ht);
}


int main()
{
    hashTable *HT = createHashTable(TABLESIZE);
    char DO;
    char newline = '0';

    char name[SIZE], surname[SIZE], birthDate[11];
    int e, c;

    while (scanf(" %c", &DO) == 1)
    {
        switch (DO)
        {
            case 'i':
                scanf("%s %s %s %d,%d", name, surname, birthDate, &e, &c);
                if (e < 0)
                {
                    if (newline == '1')
                    {
                        printf("\n");
                    }
                    printf("insert failed");
                    newline = '1';
                } else
                {
                    insert(HT, name, surname, birthDate, e, c, &newline);
                }
                break;
            case 'd':
                scanf("%s %s %s", name, surname, birthDate);
                delete(HT, name, surname, birthDate, &newline);
                break;
            case 's':
                scanf("%s %s %s", name, surname, birthDate);
                Person *person = search(HT, name, surname, birthDate);
                if (person == NULL)
                {
                    if (newline == '1')
                    {
                        printf("\n");
                    }
                    printf("search failed");
                    newline = '1';
                }
                else
                {
                    if (newline == '1')
                    {
                        printf("\n");
                    }
                    if (person->c >= 0 && person->c < 10)
                    {
                        printf("%d,0%d", person->e, person->c);
                    } else
                    {
                        printf("%d,%d", person->e, person->c);
                    }
                    newline = '1';
                }
                break;
            case 'u':
                scanf("%s %s %s %d,%d", name, surname, birthDate, &e, &c);
                update(HT, name, surname, birthDate, e, c, &newline);
                break;
            default:
                continue;
        }
    }
    freeAll(HT);
    return 0;
}