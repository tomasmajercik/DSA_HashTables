#include <stdio.h>
#include <stdlib.h>

#define SIZE 30

typedef struct Person
{
    char *name;
    char *surname;
    char *birthdate;
    int accountBalance;
    struct Person *next; // Pointer to the next person in case of collisions
} Person;

typedef struct hashTable
{
    int size;
    Person **table;
} hashTable;

// string operations
int string2int(const char e[])
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
void copyString(char *dest, const char *src)
{
    if (dest == NULL)
        return;
    while (*src)
        *dest++ = *src++;
    *dest = '\0';
}
int stringLength(const char s[])
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
        if (str1[i] < str2[i])
            return 1;
        if (str1[i] > str2[i])
            return -1;
        i++;
    }
    return 0; // else they must be the same
}

// function to create new person
Person *createPerson(const char *name, const char *surname, const char *birthdate, int balance)
{
    Person *person = (Person *) malloc(sizeof(Person));
    if (person == NULL)
        return NULL;

    person->name = (char *) malloc((stringLength(name) + 1) * sizeof(char));
    person->surname = (char *) malloc((stringLength(surname) + 1) * sizeof(char));
    person->birthdate = (char *) malloc((stringLength(birthdate) + 1) * sizeof(char));

    if (person->name == NULL || person->surname == NULL || person->birthdate == NULL)
    {
        free(person);
        return NULL;
    }

    // Copy strings
    copyString(person->name, name);
    copyString(person->surname, surname);
    copyString(person->birthdate, birthdate);

    person->accountBalance = balance;
    return person;
}

// function to create hash table
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
// hash function
unsigned int hash(const char *firstname, const char *lastname, const char *date, int table_size)
{
    unsigned int result = 0;
    int num = 31;
    for (int i = 0; firstname[i] != '\0'; i++)
        result = num * result + firstname[i];
    for (int i = 0; lastname[i] != '\0'; i++)
        result = num * result + lastname[i];
    for (int i = 0; date[i] != '\0'; i++)
        result = num * result + date[i];

    return result % table_size;
}
// hashMap functions
void insert(hashTable *ht, const char *name, const char *surname, const char *birthdate, float balance, char *newline)
{
    unsigned int index = hash(name, surname, birthdate, ht->size);

    // Create a new person
    Person *newPerson = createPerson(name, surname, birthdate, balance);
    if (newPerson == NULL)
    {
        if((*newline) == '1') {
            printf("\n");
        }
        printf("insert failed");
        (*newline) = '1';
        return;
    }
    if (ht->table[index] == NULL)
    {
        ht->table[index] = newPerson;
    } else // If there's person at taht index, chain new person to it
    {
        Person *current = ht->table[index];
        if (current == NULL)
        {
            ht->table[index] = newPerson;
            return;
        }
        while (current->next != NULL)
            current = current->next;
        current->next = newPerson;
    }
}
Person *search(hashTable *ht, const char *name, const char *surname, const char *birthdate)
{
    unsigned int index = hash(name, surname, birthdate, ht->size);
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
void update(hashTable *ht, const char *name, const char *surname, const char *birthdate, float amount, char *newline)
{
    Person *person = search(ht, name, surname, birthdate);
    if (person != NULL)
    {
        if ((person->accountBalance + amount) >= 0) {
            person->accountBalance += amount;
        } else {
            if(*newline == '1') {
                printf("\n");
            }
            printf("update failed");
            *newline = '1';
        }
    } else {
        if(*newline == '1') {
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
            }
            else
            {
                prev->next = current->next;
            }
            free(current->name);
            free(current->surname);
            free(current->birthdate);

            current->name = NULL;
            current->surname = NULL;
            current->birthdate = NULL;
            free(current);
            return;
        }
        prev = current;
        current = current->next;
    }
    if(*newline == '1') {
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

            free(temp->name);
            free(temp->surname);
            free(temp->birthdate);
            free(temp);

        }
    }
    free(ht->table);
    free(ht);
}

int main() //4,5,6,9 / 10,7,8,9,
{
    hashTable *HT = createHashTable(1); //7949
    char DO;
    float balance;
    char newline = '0';

    while (scanf(" %c", &DO) == 1) // while something is in buffer
    {
        if (DO == 'i') // insert
        {
            char name[SIZE], surname[SIZE], birthDate[11];
            int e, c;
            scanf("%s %s %s %d,%d", name, surname, birthDate, &e, &c);
            balance = e + (float)c/100;
            insert(HT, name, surname, birthDate, balance, &newline);
        }
        else if (DO == 'd') // delete
        {
            char name[SIZE], surname[SIZE], birthDate[11];
            scanf("%s %s %s", name, surname, birthDate);

            delete(HT, name, surname, birthDate, &newline);
        }
        else if (DO == 's') // search
        {
            char name[SIZE], surname[SIZE], birthDate[11];
            scanf("%s %s %s", name, surname, birthDate);

            Person *person = search(HT, name, surname, birthDate);
            if(person == NULL)
            {
                if(newline == '1')
                    printf("\n");
                printf("search failed");
                newline = '1';
            }
            else
            {
                if(newline == '1')
                    printf("\n");
                if(person->accountBalance%100 < 10 && person->accountBalance%100 >= 0)
                    printf("%d,0%d", person->accountBalance/100, person->accountBalance%100);
                else
                    printf("%d,%d", person->accountBalance/100, person->accountBalance%100);
                newline = '1';
            }

        }
        else if (DO == 'u') // update
        {
            char name[SIZE], surname[SIZE], birthDate[11];

            int e, c;
            scanf("%s %s %s %d,%d", name, surname, birthDate, &e, &c);
            balance = e + (float)c/100;

            update(HT, name, surname, birthDate, balance, &newline);
        }
    }

     freeAll(HT);
    return 0;
}
