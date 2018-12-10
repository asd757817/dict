#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "bench.c"
#include "bloom.h"
#include "tst.h"

#define TableSize 1350000 /* size of bloom filter */
#define HashNumber 10     /* number of hash functions */

/** constants insert, delete, max word(s) & stack nodes */
enum { INS, DEL, WRDMAX = 256, STKMAX = 512, LMAX = 1024 };

#define REF INS
#define CPY DEL

#define BENCH_TEST_FILE "bench_ref.txt"

long poolsize = 2000000 * WRDMAX;

/* simple trim '\n' from end of buffer filled by fgets */
static void rmcrlf(char *s)
{
    size_t len = strlen(s);
    if (len && s[len - 1] == '\n')
        s[--len] = 0;
}

#define IN_FILE "cities.txt"

void bloom_dict_insert(bloom_t bloom, tst_node *root)
{
    FILE *fp = fopen(IN_FILE, "r");
    char *pool = (char *) malloc(poolsize * sizeof(char));
    char *Top = pool;
    char buf[WORDMAX];

    while (fgets(buf, WORDMAX, fp)) {
        char *token = ",";
        strcpy(Top, strtok(buf, token));
        rmcrlf(Top);
        char *p = Top;
        /* insert reference to each string */
        if (!tst_ins_del(&root, &p, INS, REF)) { /* fail to insert */
            fprintf(stderr, "error: memory exhausted, tst_insert.\n");
            fclose(fp);
            return;
        } else { /* update bloom filter */
            bloom_add(bloom, Top);
        }

        Top += (strlen(Top) + 1);
    }
    fclose(fp);
}

int main(int argc, char **argv)
{
    char word[WRDMAX] = "";
    char *sgl[LMAX] = {NULL};
    tst_node *root = NULL, *res = NULL;
    // int rtn = 0;
    int idx = 0, sidx = 0;
    FILE *fp = fopen(IN_FILE, "r");
    double t1, t2;

    if (!fp) { /* prompt, open, validate file for reading */
        fprintf(stderr, "error: file open failed '%s'.\n", argv[1]);
        return 1;
    }
    t1 = tvgetf();

    bloom_t bloom = bloom_create(TableSize, HashNumber);

    /* memory pool */
    char *pool = (char *) malloc(poolsize * sizeof(char));
    char *Top = pool;
    char buf[WORDMAX];

    while (fgets(buf, WORDMAX, fp)) {
        char *token = ",";
        strcpy(Top, strtok(buf, token));
        rmcrlf(Top);
        char *p = Top;
        /* insert reference to each string */
        if (!tst_ins_del(&root, &p, INS, REF)) { /* fail to insert */
            fprintf(stderr, "error: memory exhausted, tst_insert.\n");
            fclose(fp);
            return 1;
        } else { /* update bloom filter */
            bloom_add(bloom, Top);
        }
        idx++;

        Top += (strlen(Top) + 1);
    }

    t2 = tvgetf();
    fclose(fp);
    printf("ternary_tree, loaded %d words in %.6f sec\n", idx, t2 - t1);

    if (argc == 2 && strcmp(argv[1], "--bench") == 0) {
        FILE *output_file = fopen("ref_accuracy.txt", "w");
        FILE *bench_ref = fopen(BENCH_TEST_FILE, "w");
        for (int hash_num = 1; hash_num < 16; hash_num++) {
            for (int table_size = 1; table_size < 41; table_size++) {
                bloom_t bloom_err_test =
                    bloom_create(table_size * 50000, hash_num);
                bloom_dict_insert(bloom_err_test, root);
                bench_test_bloom_acc(root, output_file, bench_ref, LMAX,
                                     bloom_err_test, hash_num);
                bloom_free(bloom_err_test);
            }
        }
        tst_free(root);
        fclose(output_file);
        fclose(bench_ref);
        return 0;
    }

    for (;;) {
        char *p;
        printf(
            "\nCommands:\n"
            " a  add word to the tree\n"
            " f  find word in tree\n"
            " s  search words matching prefix\n"
            " d  delete word from the tree\n"
            " q  quit, freeing all data\n\n"
            "choice: ");
        if (argc > 1 && strcmp(argv[1], "--bench") == 0)  // a for auto
            strcpy(word, argv[2]);
        else
            fgets(word, sizeof word, stdin);

        p = NULL;
        switch (*word) {
        case 'a':
            printf("enter word to add: ");
            if (argc > 1 && strcmp(argv[1], "--bench") == 0)
                strcpy(Top, argv[3]);

            else if (!fgets(Top, sizeof word, stdin)) {
                fprintf(stderr, "error: insufficient input.\n");
                break;
            }
            rmcrlf(Top);

            p = Top;
            t1 = tvgetf();
            if (bloom_test(bloom, Top) == 1) /* if detected by filter, skip */
                res = NULL;
            else { /* update via tree traversal and bloom filter */
                bloom_add(bloom, Top);
                res = tst_ins_del(&root, &p, INS, REF);
            }
            t2 = tvgetf();
            if (res) {
                idx++;
                Top += (strlen(Top) + 1);
                printf("  %s - inserted in %.10f sec. (%d words in tree)\n",
                       (char *) res, t2 - t1, idx);
            } else
                printf("  %s - already exists in list.\n", (char *) res);

            if (argc > 1 && strcmp(argv[1], "--bench") == 0)  // a for auto
                goto quit;


            break;
        case 'f':
            printf("find word in tree: ");
            if (argc > 1 && strcmp(argv[1], "--bench") == 0)
                strcpy(word, argv[3]);
            else if (!fgets(word, sizeof word, stdin)) {
                fprintf(stderr, "error: insufficient input.\n");
                break;
            }

            rmcrlf(word);
            t1 = tvgetf();

            if (bloom_test(bloom, word) == 1) {
                t2 = tvgetf();
                printf("  Bloomfilter found %s in %.6f sec.\n", word, t2 - t1);
                printf(
                    "  Probability of false positives:%lf\n",
                    pow(1 - exp(-(double) HashNumber /
                                (double) ((double) TableSize / (double) idx)),
                        HashNumber));
                t1 = tvgetf();
                res = tst_search(root, word);
                t2 = tvgetf();
                if (res)
                    printf("  ----------\n  Tree found %s in %.6f sec.\n",
                           (char *) res, t2 - t1);
                else
                    printf("  ----------\n  %s not found by tree.\n", word);
            } else
                printf("  %s not found by bloom filter.\n", word);

            if (argc > 1 && strcmp(argv[1], "--bench") == 0)  // a for auto
                goto quit;
            break;

        case 's':
            printf("find words matching prefix (at least 1 char): ");

            if (argc > 1 && strcmp(argv[1], "--bench") == 0)
                strcpy(word, argv[3]);
            else if (!fgets(word, sizeof word, stdin)) {
                fprintf(stderr, "error: insufficient input.\n");
                break;
            }
            rmcrlf(word);
            t1 = tvgetf();
            res = tst_search_prefix(root, word, sgl, &sidx, LMAX);
            t2 = tvgetf();
            if (res) {
                printf("  %s - searched prefix in %.6f sec\n\n", word, t2 - t1);
                for (int i = 0; i < sidx; i++)
                    printf("suggest[%d] : %s\n", i, sgl[i]);
            } else
                printf("  %s - not found\n", word);

            if (argc > 1 && strcmp(argv[1], "--bench") == 0)  // a for auto
                goto quit;
            break;
        case 'd':
            printf("enter word to del: ");
            if (!fgets(word, sizeof word, stdin)) {
                fprintf(stderr, "error: insufficient input.\n");
                break;
            }
            rmcrlf(word);
            p = word;
            printf("  deleting %s\n", word);
            t1 = tvgetf();
            /* FIXME: remove reference to each string */
            res = tst_ins_del(&root, &p, DEL, REF);
            t2 = tvgetf();
            if (res)
                printf("  delete failed.\n");
            else {
                printf("  deleted %s in %.6f sec\n", word, t2 - t1);
                idx--;
            }
            break;
        quit:
        case 'q':
            tst_free(root);
            return 0;
            break;
        default:
            fprintf(stderr, "error: invalid selection.\n");
            break;
        }
    }
    bloom_free(bloom);
    return 0;
}
