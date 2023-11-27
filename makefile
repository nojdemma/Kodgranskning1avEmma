CFLAGS = -Wall -g
BEFILES = hash_table.c backend.c backend_tests.c iterator.c linked_list.c utils.c
FEFILES = hash_table.c backend.c frontend.c iterator.c linked_list.c utils.c
	
backend_tests: $(BEFILES)
		gcc $(CFLAGS) $^ -o backend_tests -lcunit

frontend: $(FEFILES)
		gcc $(CFLAGS) $^ -o frontend -lcunit

test_coverage_backend: $(BEFILES)
		gcc $(CFLAGS) -coverage $^ -o test_coverage_backend -lcunit
		./test_coverage_backend
		gcov -abcfu test_coverage_backend-backend.gcno

coverage_report_backend: test_coverage_backend
		lcov -c -d . -o coverage_backend.info
		genhtml coverage_backend.info -o coverage_report_backend


