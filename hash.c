#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <ctype.h>
#include <pthread.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

int *a;
int nums = 3500;
FILE* fp;

typedef struct data {
    int l, r;
} data;

void* process(void* dd) {
    data* d = (data*) dd;

    int n = d->r - d->l + 1;

    int hash[n];
    memset(hash, 0, sizeof(hash));

    for(int i = 0; i < nums; ++i) {
        if(a[i] >= d->l && a[i] <= d->r) {
	    hash[a[i] - d->l]++;
        }
    }

    printf("Thread in range [%d, %d]:\n", d->l, d->r);

    for(int i = 0; i < n; ++i) {
	for(int j = 0; j < hash[i]; ++j) {
	    printf("%d ", i + d->l);
	    fprintf(fp, "%d ", i + d->l);
	}
    }

    printf("\n\n");
}

int main() {
    srand(time(NULL));
    a = malloc(sizeof(int) * nums);

    for(int i = 0; i < nums; ++i) {
        a[i] = (int) 1 + rand() % 999;
    }

    printf("unsorted nums (%d)\n", nums);
    /*for(int i = 0; i < nums; ++i) {
        printf("%d ", a[i]);
    } printf("\n");*/

    int buckets = 0;

    printf("Number of buckets: ");
    scanf("%d", &buckets);

    int range = 1000 / buckets + 1;

    fp = fopen("sorted.txt", "w");

    pthread_t p;

    for(int i = 0; i < buckets; ++i) {
        int l = range * i + 1;
        int r = range * (i + 1);

        if(i + 1 == buckets || r > 999) {
            r = 999;
        }

	if(l > 999) break;

        data* d = malloc(sizeof(data));

        d->l = l;
        d->r = r;

        pthread_create(&p, NULL, &process, d);
        pthread_join(p, NULL);
    }

    fclose(fp);

    return 0;
}
