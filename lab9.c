#include <stdio.h>
#include <stdlib.h>

// RecordType
struct RecordType
{
    int id;
    char name;
    int order;
    struct RecordType * next;
};

// Fill out this structure
struct HashType
{
    struct RecordType ** records;
};

// Compute the hash function
int hash(int x, int tableSize)
{
    return (x % tableSize);
}

// parses input file to an integer array
int parseData(char *inputFileName, struct RecordType **ppData)
{
    FILE *inFile = fopen(inputFileName, "r");
    int dataSz = 0;
    int i, n;
    char c;
    struct RecordType *pRecord;
    *ppData = NULL;

    if (inFile)
    {
        fscanf(inFile, "%d\n", &dataSz);
        *ppData = (struct RecordType *)malloc(sizeof(struct RecordType) * dataSz);
        // Implement parse data block
        if (*ppData == NULL)
        {
            printf("Cannot allocate memory\n");
            exit(-1);
        }
        for (i = 0; i < dataSz; ++i)
        {
            pRecord = *ppData + i;
            fscanf(inFile, "%d ", &n);
            pRecord->id = n;
            fscanf(inFile, "%c ", &c);
            pRecord->name = c;
            fscanf(inFile, "%d ", &n);
            pRecord->order = n;
            pRecord->next = NULL;
        }

        fclose(inFile);
    }

    return dataSz;
}

// prints the records
void printRecords(struct RecordType pData[], int dataSz)
{
    int i;
    printf("\nRecords:\n");
    for (i = 0; i < dataSz; ++i)
    {
        printf("\t%d %c %d\n", pData[i].id, pData[i].name, pData[i].order);
    }
    printf("\n\n");
}


// use separate chaining (more details in image in repository)
void insertRecord(struct HashType *hashTable, struct RecordType *record, int tableSize)
{
	int h = hash(record->id, tableSize);
	if(hashTable->records[h] == NULL) {
		hashTable->records[h] = record;
	}
	else {
		struct RecordType * current = hashTable->records[h];
		while(current->next != NULL) {
			current = current->next;
		}
		current->next = record;
	}
}

// display records in the hash structure
// skip the indices which are free
// the output will be in the format:
// index x -> id, name, order -> id, name, order ....
void displayRecordsInHash(struct HashType *hashTable, int tableSize)
{
	struct RecordType * current;
	for(int i = 0; i < tableSize; i++) {
		if(hashTable->records[i] != NULL) {
			printf("Index %d -> ", i);
			current = hashTable->records[i];
			while(current != NULL) {
				printf("%d, %c, %d -> ", current->id, current->name, current->order);
				current = current->next;
			}
			printf("NULL\n");
		}
	}
}

int main(void)
{
    struct RecordType *pRecords;
    int recordSz = 0;

    recordSz = parseData("input.txt", &pRecords);
    printRecords(pRecords, recordSz);

	// This block is if you want to try variable hashTableSize via user input
	//int hashTableSize;
	//printf("hashTableSize = ");
	//scanf("%d", &hashTableSize);
	//printf("\n");

	int hashTableSize = 30;

	struct HashType * hashTable = (struct HashType *)malloc(sizeof(struct HashType));
	hashTable->records = (struct RecordType **)calloc(hashTableSize, sizeof(struct RecordType *));
	for(int j = 0; j < recordSz; j++) {
		insertRecord(hashTable, &pRecords[j], hashTableSize);
	}
	displayRecordsInHash(hashTable, hashTableSize);

	free(hashTable->records);
	free(hashTable);
    free(pRecords);

    return 0;
}