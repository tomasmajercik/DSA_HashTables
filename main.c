#include <stdio.h>
#include <stdlib.h>

#define SIZE 13
#define HASHNUM 39
#define TABLESIZE 11587

//3 testy 13, 31, 11587 
// best one so far 13, 39, 11587


typedef struct Person
{
//    char *name;
//    char *surname;
//    char *birthdate;
    char *key;
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

void connect(char key[], const char name[], const char surname[], const char date[])
{
    int i = 0;
    while (name[i] != '\0')
    {
        key[i] = name[i];
        i++;
    }
    int j = 0;
    while (surname[j] != '\0')
    {
        key[i] = surname[j];
        i++;
        j++;
    }
    j = 0;
    while (date[j] != '\0')
    {
        key[i] = date[j];
        i++;
        j++;
    }
    key[i] = '\0';
}


void copyString(char *dest, const char *src)
{
    if (dest == NULL)
        return;
    while (*src)
        *dest++ = *src++;
    *dest = '\0';
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
        if (str1[i] < str2[i])
            return 1;
        if (str1[i] > str2[i])
            return -1;
        i++;
    }
    return 0; // else they must be the same
}

// function to create new person
Person *createPerson(const char *key, int balance)
{
    Person *person = (Person *) malloc(sizeof(Person));
    if (person == NULL)
        return NULL;

    // person->key = (char *) malloc((stringLength(key) + 1) * sizeof(char));
    person->key = (char *) malloc((2 * SIZE) * sizeof(char)); // 3x wrong, zbytok timeout

    if (person->key == NULL)
    {
        free(person);
        return NULL;
    }

    // Copy strings
    copyString(person->key, key);
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
unsigned long long hash(const char *key, int table_size) //?
{
    unsigned long long result = 0;
    for (int i = 0; key[i] != '\0'; i++)
        result = HASHNUM * result + key[i];

    return result % table_size;
}

// hashMap functions
void insert(hashTable *ht, const char *key, int balance, char *newline)
{
    unsigned long long index = hash(key, ht->size);

    // Create a new person
    Person *newPerson = createPerson(key, balance);
    if (newPerson == NULL)
    {
        if (*newline == '1')
        {
            printf("\n");
        }
        printf("insert failed");
        *newline = '1';
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

Person *search(hashTable *ht, const char *key)
{
    unsigned long long index = hash(key, ht->size);
    Person *current = ht->table[index];
    while (current != NULL)
    {
        if (stringCmp(current->key, key) == 0)
        {
            return current;
        }
        current = current->next;
    }

    return NULL; // No match found
}

void update(hashTable *ht, const char *key, int amount, char *newline)
{
    Person *person = search(ht, key);
    if (person != NULL)
    {
        if ((person->accountBalance + amount) >= 0)
        {
            person->accountBalance += amount;
        } else
        {
            if (*newline == '1')
            {
                printf("\n");

            }
            printf("update failed");
            *newline = '1';
        }
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

void delete(hashTable *ht, const char *key, char *newline)
{
    unsigned long long index = hash(key, ht->size);
    Person *current = ht->table[index];
    Person *prev = NULL;
    while (current != NULL)
    {
        if (stringCmp(current->key, key) == 0)
        {
            if (prev == NULL)
            {
                ht->table[index] = current->next;
            } else
            {
                prev->next = current->next;
            }
            free(current->key);
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

            free(temp->key);
            temp->key = NULL;

            free(temp);
            temp = NULL;

        }
        ht->table[i] = NULL;
    }
    free(ht->table);
    ht->table = NULL;
    free(ht);
}


int main()
{
    hashTable *HT = createHashTable(TABLESIZE);
    char DO;
    int balance;
    char newline = '0';
    char key[SIZE * 4];

    while (scanf(" %c", &DO) == 1) // while something is in buffer
    {
        if (DO == 'i') // insert
        {
            char name[SIZE], surname[SIZE], birthDate[11];
            char e[SIZE];
            scanf("%s %s %s %s", name, surname, birthDate, e);

            balance = string2int(e);
            connect(key, name, surname, birthDate);

            insert(HT, key, balance, &newline);
        } else if (DO == 'd') // delete
        {
            char name[SIZE], surname[SIZE], birthDate[11];
            scanf("%s %s %s", name, surname, birthDate);
            connect(key, name, surname, birthDate);

            delete(HT, key, &newline);
        } else if (DO == 's') // search
        {
            char name[SIZE], surname[SIZE], birthDate[11];
            scanf("%s %s %s", name, surname, birthDate);
            connect(key, name, surname, birthDate);

            Person *person = search(HT, key);
            if (person == NULL)
            {
                if (newline == '1')
                    printf("\n");
                printf("search failed");
                newline = '1';
            } else
            {
                if (newline == '1')
                    printf("\n");
                if (person->accountBalance % 100 < 10 && person->accountBalance % 100 >= 0)
                    printf("%d,0%d", person->accountBalance / 100, person->accountBalance % 100);
                else
                    printf("%d,%d", person->accountBalance / 100, person->accountBalance % 100);
                newline = '1';
            }

        } else if (DO == 'u') // update
        {
            char name[SIZE], surname[SIZE], birthDate[11], e[SIZE];

            scanf("%s %s %s %s", name, surname, birthDate, e);
            balance = string2int(e);
            connect(key, name, surname, birthDate);


            update(HT, key, balance, &newline);
        }
    }

    //  freeAll(HT);
    return 0;
}