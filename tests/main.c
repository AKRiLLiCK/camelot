/*
 * Camelot Kernel Verification Suite
 * Entry Point
 */

#include "tests.h"

// External Test Suites
extern void test_types();
extern void test_memory();
extern void test_io();
extern void test_files();
extern void test_ds();

int main() {
	// 1. Setup
	printf("\n[ASC-1.2] RUNNING VERIFICATION SUITE\n");
	printf("--------------------------------------\n");

	// 2. Execute Suites
	test_types();
	test_memory();
	test_io();
	test_files();
	test_ds();

	// 3. Report
	printf("--------------------------------------\n");
	printf("Tests Run:    %d\n", tests_run);
	printf("Tests Failed: %d\n", tests_failed);

	// 4. THE DECISION GATE
	// Return 0 = Success (Green Checkmark)
	// Return 1 = Failure (Red X, blocks merge)
	if (tests_failed == 0) {
		printf("[V] SYSTEM INTEGRITY VERIFIED.\n");
		return 0;
	} else {
		printf("[X] INTEGRITY VIOLATION DETECTED.\n");
		return 1;
	}
}