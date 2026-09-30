#include <stdio.h>

#define N 5

struct Inode {
    int no;
    int ref;
    int lock;
    int dirty;
};

struct Inode table[N];

/* Display Inode table */
void display() {
    printf("\nInode\tRef\tlock\tDirty\n");
    for (int i = 0; i < N; i++) {
        printf("%d\t%d\t%d\t%d\n", table[i].no, table[i].ref, table[i].lock, table[i].dirty);
    }
}

/* iget() */
void iget(int n) {
    for (int i = 0; i < N; i++) {
        if (table[i].no == n) {
            if (table[i].lock) {
                printf("\nInode is locked. waiting...\n");
                table[i].lock = 0;
            }
            table[i].lock = 1;
            table[i].ref++;
            printf("\niget(): Inode %d obtained.", n);
            printf("\nReference count=%d\n", table[i].ref);
            return;
        }
    }
    printf("\nInode not found.\n");
}

/* iput() */
void iput(int n) {
    for (int i = 0; i < N; i++) {
        if (table[i].no == n) {
            if (table[i].ref == 0) {
                printf("\nInode is not in use...\n");
                return;
            }
            
            table[i].ref--;
            printf("\niput(): Reference count=%d\n", table[i].ref);
            
            if (table[i].ref == 0) {
                if (table[i].dirty) {
                    printf("Writing dirty inode to disk...\n");
                    table[i].dirty = 0;
                }

                table[i].lock=0;
                printf("Inode %d released.\n");
            }
            return; // Added return to exit loop once the matching Inode is updated
        }
    }

    printf("\nInode not found.\n");
}

int main()
{
    int i, choice, n;

    /* Initialize inode table */
    for(i = 0; i < N; i++)
    {
        table[i].no = i + 1;
        table[i].ref = 0;
        table[i].lock = 0;
        table[i].dirty = 0;
    }

    while(1)
    {
        printf("\n\n1. Display");
        printf("\n2. iget()");
        printf("\n3. iput()");
        printf("\n4. Make inode dirty");
        printf("\n5. Exit");

        printf("\nEnter choice: ");
        scanf("%d", &choice);

        if(choice == 1)
        {
            display();
        }
        else if(choice == 2)
        {
            printf("Enter inode number: ");
            scanf("%d", &n);
            iget(n);
        }
        else if(choice == 3)
        {
            printf("Enter inode number: ");
            scanf("%d", &n);
            iput(n);
        }
        else if(choice == 4)
        {
            printf("Enter inode number: ");
            scanf("%d", &n);

            if(n >= 1 && n <= N)
                table[n-1].dirty = 1;
        }
        else if(choice == 5)
        {
            break;
        }
        else
        {
            printf("Invalid choice.");
        }
    }

    return 0;
}
