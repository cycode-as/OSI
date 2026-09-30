#include<stdio.h>
#include<string.h>

#define DISK_BLOCKS 10
#define BUFFER_COUNT 3
#define DATA_SIZE 50

/* Simulated disk */
char disk[DISK_BLOCKS][DATA_SIZE];

/* Buffer header + buffer data */
struct Buffer
{
    int blockNumber;
    int valid;
    int dirty;
    int busy;
    char data[DATA_SIZE];
};

struct Buffer bufferPool[BUFFER_COUNT];


/* Initialize simulated disk */
void initializeDisk()
{
    int i;

    for(i = 0; i < DISK_BLOCKS; i++)
    {
        sprintf(disk[i], "Original data of Disk Block %d", i);
    }
}


/* Initialize buffer pool */
void initializeBufferPool()
{
    int i;

    for(i = 0; i < BUFFER_COUNT; i++)
    {
        bufferPool[i].blockNumber = -1;
        bufferPool[i].valid = 0;
        bufferPool[i].dirty = 0;
        bufferPool[i].busy = 0;
        strcpy(bufferPool[i].data, "");
    }
}


/* Display buffer pool */
void displayBufferPool()
{
    int i;

    printf("\n---------------------------------------------\n");
    printf("              BUFFER POOL\n");
    printf("---------------------------------------------\n");

    for(i = 0; i < BUFFER_COUNT; i++)
    {
        printf("\nBuffer %d\n", i);

        if(bufferPool[i].valid == 0)
        {
            printf("  Status       : FREE\n");
        }
        else
        {
            printf("  Block Number : %d\n",
                   bufferPool[i].blockNumber);

            printf("  Valid        : %d\n",
                   bufferPool[i].valid);

            printf("  Dirty        : %d\n",
                   bufferPool[i].dirty);

            printf("  Busy         : %d\n",
                   bufferPool[i].busy);

            printf("  Data         : %s\n",
                   bufferPool[i].data);
        }
    }

    printf("---------------------------------------------\n");
}


/* Search for block in buffer cache */
int searchBuffer(int blockNumber)
{
    int i;

    for(i = 0; i < BUFFER_COUNT; i++)
    {
        if(bufferPool[i].valid == 1 &&
           bufferPool[i].blockNumber == blockNumber)
        {
            return i;
        }
    }

    return -1;
}


/* Find a free buffer */
int findFreeBuffer()
{
    int i;

    for(i = 0; i < BUFFER_COUNT; i++)
    {
        if(bufferPool[i].valid == 0)
        {
            return i;
        }
    }

    return -1;
}


/* Read block from disk */
void readBlock()
{
    int blockNumber;
    int bufferNumber;

    printf("\nEnter disk block number (0-%d): ",
           DISK_BLOCKS - 1);

    scanf("%d", &blockNumber);

    if(blockNumber < 0 || blockNumber >= DISK_BLOCKS)
    {
        printf("\nInvalid block number.\n");
        return;
    }

    /* Search buffer cache */
    bufferNumber = searchBuffer(blockNumber);

    if(bufferNumber != -1)
    {
        printf("\nCACHE HIT!\n");

        printf("Block %d is already present in Buffer %d.\n",
               blockNumber,
               bufferNumber);

        printf("Data: %s\n",
               bufferPool[bufferNumber].data);

        return;
    }

    /* Cache miss */
    printf("\nCACHE MISS!\n");

    printf("Block %d is not present in buffer cache.\n",
           blockNumber);

    /* Find free buffer */
    bufferNumber = findFreeBuffer();

    if(bufferNumber == -1)
    {
        printf("\nNo free buffer available.\n");
        printf("For simplicity, this program does not implement\n");
        printf("a replacement algorithm.\n");
        return;
    }

    /* Read disk block into buffer */
    bufferPool[bufferNumber].blockNumber = blockNumber;

    strcpy(bufferPool[bufferNumber].data,
           disk[blockNumber]);

    bufferPool[bufferNumber].valid = 1;
    bufferPool[bufferNumber].dirty = 0;
    bufferPool[bufferNumber].busy = 0;

    printf("\nDisk Block %d loaded into Buffer %d.\n",
           blockNumber,
           bufferNumber);

    printf("Data: %s\n",
           bufferPool[bufferNumber].data);
}


/* Modify a buffer */
void modifyBuffer()
{
    int blockNumber;
    int bufferNumber;
    char newData[DATA_SIZE];

    printf("\nEnter block number to modify: ");
    scanf("%d", &blockNumber);

    bufferNumber = searchBuffer(blockNumber);

    if(bufferNumber == -1)
    {
        printf("\nBlock is not present in buffer cache.\n");
        printf("Read the block first.\n");
        return;
    }

    printf("\nCurrent data: %s\n",
           bufferPool[bufferNumber].data);

    printf("Enter new data: ");

    scanf(" %[^\n]", newData);

    strcpy(bufferPool[bufferNumber].data, newData);

    /* Mark buffer dirty */
    bufferPool[bufferNumber].dirty = 1;

    printf("\nBuffer modified successfully.\n");
    printf("Buffer is now DIRTY.\n");
}


/* Write buffer back to disk */
void writeBlock()
{
    int blockNumber;
    int bufferNumber;

    printf("\nEnter block number to write to disk: ");
    scanf("%d", &blockNumber);

    bufferNumber = searchBuffer(blockNumber);

    if(bufferNumber == -1)
    {
        printf("\nBlock is not present in buffer cache.\n");
        return;
    }

    if(bufferPool[bufferNumber].dirty == 0)
    {
        printf("\nBuffer is not dirty.\n");
        printf("No changes need to be written.\n");
        return;
    }

    /* Write buffer data to disk */
    strcpy(disk[blockNumber],
           bufferPool[bufferNumber].data);

    bufferPool[bufferNumber].dirty = 0;

    printf("\nBuffer %d written back to Disk Block %d.\n",
           bufferNumber,
           blockNumber);

    printf("Disk data is now: %s\n",
           disk[blockNumber]);
}


/* Display simulated disk */
void displayDisk()
{
    int i;

    printf("\n---------------------------------------------\n");
    printf("              SIMULATED DISK\n");
    printf("---------------------------------------------\n");

    for(i = 0; i < DISK_BLOCKS; i++)
    {
        printf("Block %d : %s\n", i, disk[i]);
    }

    printf("---------------------------------------------\n");
}


/* Main function */
int main()
{
    int choice;

    initializeDisk();
    initializeBufferPool();

    while(1)
    {
        printf("\n\n========== BUFFER CACHE SIMULATION ==========\n");

        printf("1. Read Disk Block\n");
        printf("2. Modify Buffer\n");
        printf("3. Write Buffer to Disk\n");
        printf("4. Display Buffer Pool\n");
        printf("5. Display Disk\n");
        printf("6. Exit\n");

        printf("\nEnter your choice: ");
        scanf("%d", &choice);

        switch(choice)
        {
            case 1:
                readBlock();
                break;

            case 2:
                modifyBuffer();
                break;

            case 3:
                writeBlock();
                break;

            case 4:
                displayBufferPool();
                break;

            case 5:
                displayDisk();
                break;

            case 6:
                printf("\nProgram terminated.\n");
                return 0;

            default:
                printf("\nInvalid choice.\n");
        }
    }

    return 0;
}