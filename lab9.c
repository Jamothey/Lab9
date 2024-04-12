#include <stdio.h>

// RecordType
struct RecordType
{
	int id;
	char name;
	int order;
};

// Fill out this structure
struct HashType
{
	struct RecordType *buckets[10];
	int count[10];
	int size;
};

// Compute the hash function
int hash(int x)
{
	int a = 31;
	int b = 7;
	return (a * x + b) % 10;
}

void insertRecord(struct HashType *hashTable, struct RecordType record)
{
	int idx = hash(record.id);
	int count = hashTable->count[idx];
	hashTable->buckets[idx] = realloc(hashTable->buckets[idx], (count + 1) * sizeof(struct RecordType));
	if (hashTable->buckets[idx] == NULL)
	{
		fprintf(stderr, "Memory allocation failed\n");
		exit(-1);
	}
	hashTable->buckets[idx][count] = record;
	hashTable->count[idx]++;
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

// display records in the hash structure
// skip the indices which are free
// the output will be in the format:
// index x -> id, name, order -> id, name, order ....
void displayRecordsInHash(struct HashType *pHashArray, int hashSz)
{
	int i;
	printf("Contents of Hash Table:\n");
	for (i = 0; i < hashSz; ++i)
	{
		printf("Bucket %d:", i);
		if (pHashArray->count[i] > 0)
		{
			for (int j = 0; j < pHashArray->count[i]; j++)
			{
				printf(" -> [%d, %c, %d]", pHashArray->buckets[i][j].id, pHashArray->buckets[i][j].name, pHashArray->buckets[i][j].order);
			}
			printf(" -> NULL\n");
		}
		else
		{
			printf(" Empty\n");
		}
	}
}

int main(void)
{
	struct RecordType *pRecords;
	int recordSz = 0;
	recordSz = parseData("input_lab_9.txt", &pRecords);
	printRecords(pRecords, recordSz);
	struct HashType hashTable;
	hashTable.size = 10;
	for (int i = 0; i < hashTable.size; i++)
	{
		hashTable.buckets[i] = NULL;
		hashTable.count[i] = 0;
	}
	for (int i = 0; i < recordSz; i++)
	{
		insertRecord(&hashTable, pRecords[i]);
	}
	displayRecordsInHash(&hashTable, hashTable.size);
	for (int i = 0; i < hashTable.size; i++)
	{
		free(hashTable.buckets[i]);
	}
	free(pRecords);

	return 0;
}