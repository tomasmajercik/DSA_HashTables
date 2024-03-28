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
int hash(const char *str)
{
    int H = 0;
    for (int i = 0; str[i] != '\0'; i++)
        H = 31 * H + str[i];
    return H;
}
// insert function
void insert(hashTable *ht, const char *name, const char *surname, const char *birthdate, int balance)
{
    int index = hash(name) % ht->size;
    // Create a new person
    Person *newPerson = createPerson(name, surname, birthdate, balance);
    if (newPerson == NULL)
    {
        printf("Memory allocation failed. Unable to insert.\n"); //? delete this line
        return;
    }
    // Insert the new person into the hash table
    if (ht->table[index] == NULL)
        ht->table[index] = newPerson; // If the slot is empty, insert the new person directly
    else // there's already a person at this index, chain the new person to it
    {
        Person *current = ht->table[index];
        while (current->next != NULL)
            current = current->next;
        current->next = newPerson;
    }
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
