#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "bench.h"
#include "bloom.h"
//#define DICT_FILE "cities.txt"
#define DICT_FILE "t.txt"
#define WORDMAX 256
#define test_times 100

double tvgetf()
{
    struct timespec ts;
    double sec;

    clock_gettime(CLOCK_REALTIME, &ts);
    sec = ts.tv_nsec;
    sec /= 1e9;
    sec += ts.tv_sec;

    return sec;
}


int bench_test(const tst_node *root, char *out_file, const int max)
{
    char word[WORDMAX] = "";
    FILE *output_file = fopen(out_file, "w");
    for (int i = 0; i < test_times; i++) {
        FILE *dict = fopen(DICT_FILE, "r");
        int idx = 0;
        double t1, t2;
        double total_time = 0;
        if (!output_file || !dict) {
            if (output_file) {
                fprintf(stderr, "error: file open failed in '%s'.\n",
                        DICT_FILE);
                fclose(output_file);
            }
            if (dict) {
                fprintf(stderr, "error: file open failed in '%s'.\n", out_file);
                fclose(dict);
            }
            return 1;
        }

        while (fscanf(dict, "%s", word) != EOF) {
            t1 = tvgetf();
            tst_search(root, word);
            t2 = tvgetf();
            // fprintf(fp, "%d %f nsec\n", idx, (t2 - t1) * 1000000);
            total_time += (t2 - t1);
            idx++;
        }
        fprintf(output_file, "%d %f\n", i, total_time);
        fclose(dict);
    }
    fclose(output_file);
    return 0;
}

int bench_test_bloom_acc(const tst_node *root,
                         char *out_file,
                         const int max,
                         bloom_t bloom)
{
    // char word[WORDMAX] = "";
    char buf[WORDMAX];
    FILE *output_file = fopen("ref_accuracy.txt", "w");

    FILE *dict = fopen(DICT_FILE, "r");
    int idx = 0;
    double t1, t2;

    if (!output_file || !dict) {
        if (output_file) {
            fprintf(stderr, "error: file open failed in '%s'.\n", DICT_FILE);
            fclose(output_file);
        }
        if (dict) {
            fprintf(stderr, "error: file open failed in '%s'.\n", out_file);
            fclose(dict);
        }
        return 1;
    }

    double tp = 0, fp = 0, tn = 0, fn = 0, count = 0;
    double total_time = 0;
    while (fgets(buf, WORDMAX, dict)) {
        char *token = ",";
        char *c;
        c = strtok(buf, token);
        t1 = tvgetf();
        if (bloom_test(bloom, c) == 1) {
            if (tst_search(root, c)) {
                t2 = tvgetf();
                tp += 1;  // true positive
            } else {
                t2 = tvgetf();
                fp += 1;  // false positive
            }
        } else {
            t2 = tvgetf();
            if (tst_search(root, c))
                fn += 1;  // false negative
            else
                tn += 1;  // true negative
        }
        // fprintf(fp, "%d %f nsec  %s\n", idx, (t2 - t1) * 1000000, c);
        total_time += (t2 - t1);
        idx++;
        count++;
    }
    double acc = (tp + tn) / count;
    // total_time, tp, fp, fn, tn, accuracy
    fprintf(output_file, "%f %f %f %f %f %f\n", total_time, tp, fp, fn, tn,
            acc);
    fclose(dict);
    fclose(output_file);

    return 0;
}


int bench_test_prefix(const tst_node *root, char *out_file, const int max)
{
    char prefix[3] = "";
    char word[WORDMAX] = "";
    char **sgl;
    FILE *output_file = fopen(out_file, "w");

    for (int i = 0; i < test_times; i++) {
        FILE *dict = fopen(DICT_FILE, "r");
        int idx = 0, sidx = 0;
        double t1, t2;
        double total_time = 0;

        if (!output_file || !dict) {
            if (output_file) {
                fprintf(stderr, "error: file open failed in '%s'.\n",
                        DICT_FILE);
                fclose(output_file);
            }
            if (dict) {
                fprintf(stderr, "error: file open failed in '%s'.\n", out_file);
                fclose(dict);
            }
            return 1;
        }

        sgl = (char **) malloc(sizeof(char *) * max);
        while (fscanf(dict, "%s", word) != EOF) {
            if (strlen(word) < 4)
                continue;
            strncpy(prefix, word, 3);
            t1 = tvgetf();
            tst_search_prefix(root, prefix, sgl, &sidx, max);
            t2 = tvgetf();
            // fprintf(fp, "%d %f nsec\n", idx, (t2 - t1) * 1000000);
            total_time += (t2 - t1);
            idx++;
        }
        fprintf(output_file, "%d %f\n", i, total_time);
        fclose(dict);
    }
    fclose(output_file);
    return 0;
}


int bench_test_bloom(const tst_node *root,
                     char *out_file,
                     const int max,
                     bloom_t bloom)
{
    // char word[WORDMAX] = "";
    char buf[WORDMAX];
    FILE *output_file = fopen(out_file, "w");

    for (int i = 0; i < test_times; i++) {
        FILE *dict = fopen(DICT_FILE, "r");
        int idx = 0;
        double t1, t2;

        if (!output_file || !dict) {
            if (output_file) {
                fprintf(stderr, "error: file open failed in '%s'.\n",
                        DICT_FILE);
                fclose(output_file);
            }
            if (dict) {
                fprintf(stderr, "error: file open failed in '%s'.\n", out_file);
                fclose(dict);
            }
            return 1;
        }

        double total_time = 0;
        while (fgets(buf, WORDMAX, dict)) {
            char *token = ",";
            char *c;
            c = strtok(buf, token);
            t1 = tvgetf();
            if (bloom_test(bloom, c) == 1) {
                if (tst_search(root, c)) {
                    t2 = tvgetf();
                } else {
                    t2 = tvgetf();
                }
            } else {
                t2 = tvgetf();
            }
            // fprintf(fp, "%d %f nsec  %s\n", idx, (t2 - t1) * 1000000, c);
            total_time += (t2 - t1);
            idx++;
        }
        fprintf(output_file, "%d %f\n", i, total_time);
        fclose(dict);
    }
    fclose(output_file);

    return 0;
}
