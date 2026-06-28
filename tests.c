#include "get_next_line.h"
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

void test_basic_file(void) {
    int fd = open("test.txt", O_RDONLY);
    char *line;
    int line_num = 0;

    printf("\n=== Basic File Test ===\n");
    while ((line = get_next_line(fd)) != NULL) {
        printf("Line %d: %s", ++line_num, line);
        free(line);
    }
	line = get_next_line(fd);
	printf("Line %d: %s", ++line_num, line);
    free(line);
	line = get_next_line(fd);
	printf("Line %d: %s", ++line_num, line);
    free(line);
	line = get_next_line(fd);
	printf("Line %d: %s", ++line_num, line);
    free(line);
	close(fd);
    // Expected output:
    // Line 1: "Hello"
    // Line 2: "World"
    // Line 3: "42"
}

void test_multiple_fds(void) {
    int fd1 = open("file_a.txt", O_RDONLY);
    int fd2 = open("file_b.txt", O_RDONLY);
    char *line1, *line2;

    printf("\n=== Multiple FDs Test ===\n");

    // Interleave reads
    line1 = get_next_line(fd1);
    printf("FD1: \"%s\"\n", line1); free(line1);

    line2 = get_next_line(fd2);
    printf("FD2: \"%s\"\n", line2); free(line2);

    line1 = get_next_line(fd1);
    printf("FD1: \"%s\"\n", line1); free(line1);

    line2 = get_next_line(fd2);
    printf("FD2: \"%s\"\n", line2); free(line2);

    line1 = get_next_line(fd1);
    printf("FD1: \"%s\"\n", line1); free(line1);

    line2 = get_next_line(fd2);
    printf("FD2: \"%s\"\n", line2); free(line2);

	line1 = get_next_line(fd1);
    printf("FD1: \"%s\"\n", line1); free(line1);

    line2 = get_next_line(fd2);
    printf("FD2: \"%s\"\n", line2); free(line2);

	line1 = get_next_line(fd1);
    printf("FD1: \"%s\"\n", line1); free(line1);

    line2 = get_next_line(fd2);
    printf("FD2: \"%s\"\n", line2); free(line2);

    close(fd1);
    close(fd2);
    // Expected: A1, B1, A2, B2, A3, B3
}

void test_multiple_fds_hardmode(void) {
    int fd1 = open("file_a.txt", O_RDONLY);
    int fd2 = open("file_b.txt", O_RDONLY);
	int fd3 = open("file_c.txt", O_RDONLY);
	int fd4 = open("file_d.txt", O_RDONLY);
	int fd5 = open("file_e.txt", O_RDONLY);
    char *line;
	int fds[5] = {fd1, fd2, fd3, fd4, fd5};
	int	line_num;

    printf("\n=== Multiple FDs Test Reading Null Many Times ===\n");

    // Interleave reads
	line_num = 0;
	for (int i = 0; i < 50; i++) {
		++line_num;
		for (int fd_no = 1; fd_no < 6;fd_no++)
		{
			line = get_next_line(fds[fd_no - 1]);
			printf("FD %d, line %d,: %s", fd_no, line_num, line);
			free(line);
		}
    }
	for (int fd_no = 0; fd_no < 5;fd_no++)
	{
    	close(fds[fd_no]);
	}
    // Expected: A1, B1, A2, B2, A3, B3
}

void test_empty_file(void) {
    int fd = open("empty.txt", O_RDONLY);
    char *line;

    printf("\n=== Empty File Test ===\n");
    line = get_next_line(fd);
    if (line == NULL) {
        printf("✓ Correctly returned NULL for empty file\n");
    } else {
        printf("✗ Should return NULL, got: \"%s\"\n", line);
        free(line);
    }
    close(fd);
}

void test_no_newline(void) {
    int fd = open("no_newline.txt", O_RDONLY);
    char *line;
    int line_num = 0;

    printf("\n=== No Newline at EOF Test ===\n");
    while ((line = get_next_line(fd)) != NULL) {
        printf("Line %d: \"%s\"\n", ++line_num, line);
        free(line);
    }
    close(fd);
    // Expected: 3 lines, last one "No newline at end" (no \n)
}

void create_big_file(void) {
    FILE *f = fopen("big.txt", "w");
    for (int i = 0; i < 10000; i++) {
        fputc('A' + (i % 26), f);
    }
    fputs("\nShort line\n", f);
    fclose(f);
}

void test_big_line(void) {
    int fd = open("big.txt", O_RDONLY);
    char *line;

    printf("\n=== Big Line Test ===\n");
    line = get_next_line(fd);
    if (line) {
        printf("Line length: %zu characters\n", strlen(line));
        printf("First 50 chars: \"%.50s...\"\n", line);
        free(line);
    }

    line = get_next_line(fd);
    if (line) {
        printf("Second line: \"%s\"\n", line);
        free(line);
    }
    close(fd);
}

void test_alternating_fds(void) {
    int fd1 = open("file_a.txt", O_RDONLY);
    int fd2 = open("file_b.txt", O_RDONLY);
    char *line;

    printf("\n=== Alternating FDs Test ===\n");

    // Read all from fd1 first
    printf("Reading all from FD1:\n");
    while ((line = get_next_line(fd1)) != NULL) {
        printf("FD1: \"%s\"\n", line);
        free(line);
    }

    // Then read all from fd2 (should start from beginning)
    printf("\nReading all from FD2:\n");
    while ((line = get_next_line(fd2)) != NULL) {
        printf("FD2: \"%s\"\n", line);
        free(line);
    }

    close(fd1);
    close(fd2);
}

void test_invalid_fd(void) {
    printf("\n=== Invalid FD Test ===\n");

    // Test with invalid FD
    char *line = get_next_line(-1);
    if (line == NULL) {
        printf("✓ Correctly returned NULL for invalid FD\n");
    } else {
        printf("✗ Should return NULL, got: \"%s\"\n", line);
        free(line);
    }

    // Test with closed FD
    int fd = open("test.txt", O_RDONLY);
    close(fd);
    line = get_next_line(fd);
    if (line == NULL) {
        printf("✓ Correctly returned NULL for closed FD\n");
    } else {
        printf("✗ Should return NULL, got: \"%s\"\n", line);
        free(line);
    }
}

void test_memory_leaks(void) {
    printf("\n=== Memory Leak Test ===\n");
    printf("Check with valgrind: valgrind --leak-check=full ./test\n");

    // Read a file completely
    int fd = open("test.txt", O_RDONLY);
    char *line;
    while ((line = get_next_line(fd)) != NULL) {
        free(line);
    }
    close(fd);

    // All memory should be freed
    // Should show: "All heap blocks were freed -- no leaks are possible"
}

void test_special_chars(void) {
    int fd = open("special.txt", O_RDONLY);
    char *line;

    printf("\n=== Special Characters Test ===\n");
    while ((line = get_next_line(fd)) != NULL) {
        printf("Line: \"%s\"\n", line);
        free(line);
    }
    close(fd);
}

void test_large_file(void) {
    printf("\n=== Large File Test ===\n");
    printf("Creating 10000 line file...\n");

    FILE *f = fopen("large.txt", "w");
    for (int i = 0; i < 10000; i++) {
        fprintf(f, "Line number %d\n", i);
    }
    fclose(f);

    int fd = open("large.txt", O_RDONLY);
    char *line;
    int count = 0;

    while ((line = get_next_line(fd)) != NULL) {
        count++;
        if (count % 1000 == 0) {
            printf("Read %d lines...\n", count);
        }
        free(line);
    }

    printf("Total lines: %d (should be 10000)\n", count);
    close(fd);
}

void test_stdin(void) {
    printf("\n=== Stdin Test ===\n");
    printf("Enter some lines (Ctrl+D to finish):\n");

    char *line;
    int line_num = 0;
    while ((line = get_next_line(0)) != NULL) {
        printf("You entered line %d: \"%s\"\n", ++line_num, line);
        free(line);
    }
    printf("Done reading stdin\n");
}

int main(void) {
    printf("=== GET_NEXT_LINE TESTS ===\n");

    // Create test files
    // system("echo 'Hello\\nWorld\\n42' > test.txt");
    system("echo 'A1\\nA2\\nA3' > file_a.txt");
    system("echo 'B1\\nB2\\nB3\\nB4' > file_b.txt");
     system("touch empty.txt");
    system("echo -n 'Hello\\nWorld\\nNo newline at end' > no_newline.txt");

    // Run tests
    test_basic_file();
    test_multiple_fds();
	test_multiple_fds_hardmode();
     test_empty_file();
     test_no_newline();
	 create_big_file();
     test_big_line();
    test_alternating_fds();
    test_invalid_fd();
    test_special_chars();
    test_large_file();

     test_stdin(); // Uncomment for interactive testing
    // test_memory_leaks();

    // Cleanup
    system("rm -f file_a.txt file_b.txt empty.txt no_newline.txt big.txt large.txt");

    printf("\n=== ALL TESTS COMPLETE ===\n");
    return 0;
}
