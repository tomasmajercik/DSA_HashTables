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
        if (str1[i] > str2[i])
            return -1;
        if (str1[i] < str2[i])
            return 1;
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

    // Allocate memory and copy strings
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
int hash(const char *firstname, const char *lastname, const char *date, int table_size)
{
    int result = 0;
    for (int i = 0; firstname[i] != '\0'; i++)
        result = 33 * result + firstname[i];
    for (int i = 0; lastname[i] != '\0'; i++)
        result = 33 * result + lastname[i];
    for (int i = 0; date[i] != '\0'; i++)
        result = 33 * result + date[i];

    return result % table_size;
}
// hashMap function
void insert(hashTable *ht, const char *name, const char *surname, const char *birthdate, int balance)
{
    int index = hash(name, surname, birthdate, ht->size);

    // Create a new person
    Person *newPerson = createPerson(name, surname, birthdate, balance);
    if (newPerson == NULL)
    {
        printf("failed");
        return;
    }

    // If the slot is empty, insert the new person directly
    if (ht->table[index] == NULL)
    {
        ht->table[index] = newPerson;
    } else // If there's already a person at this index, chain the new person to it
    {
        Person *current = ht->table[index];
        while (current->next != NULL)
            current = current->next;
        current->next = newPerson;
    }
}
Person *search(hashTable *ht, const char *name, const char *surname, const char *birthdate)
{
    int index = hash(name, surname, birthdate, ht->size);
    Person *current = ht->table[index];
    while (current != NULL)
    {
        // Compare name, surname, and birthdate
        if (stringCmp(current->name, name) == 0 &&
            stringCmp(current->surname, surname) == 0 &&
            stringCmp(current->birthdate, birthdate) == 0)
        {
            return current; // Found a match, return the person
        }
        current = current->next; // Move to the next person in the linked list
    }

    return NULL; // No match found
}
void update(hashTable *ht, const char *name, const char *surname, const char *birthdate, int amount)
{
    Person *person = search(ht, name, surname, birthdate);
    if (person != NULL)
    {
        if ((person->accountBalance + amount) >= 0)
        {
            person->accountBalance += amount;
        } else
        {
            printf("failed");
        }
    } else
    {
        printf("failed");
    }
}
void delete(hashTable *ht, const char *name, const char *surname, const char *birthdate)
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
            free(current);
            return;
        }
        prev = current;
        current = current->next;
    }
    printf("failed");
}

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

    freeAll(HT);
    return 0;
}
