# Inlupp2

# How to build the program and run the tests
To build the program with its frontend: 
make frontend
./frontend

Files needed for frontend: hash_table.c backend.c frontend.c iterator.c linked_list.c utils.c

To run the tests on the backend part of the program:
make backend_tests

Files needed for backend_tests: hash_table.c backend.c backend_tests.c iterator.c linked_list.c

With the commands "make test_coverage_backend" and "make coverage_report_backend" you will see how much of the code the tests actually execute on with the coverage tool "gcov". Look in the coverage_backend_report index's HTML files to see a visual code coverage report.

make test_coverage_backend:
Lines executed: 94.59% of 259
Branches executed 92.31% of 78
Functions executed: 100%

hash_table.c from Maja
utils.c from Fadi
linked_list.c from Fadi

# Error handling
Errors are mostly handled by the frontend, checking if the input is valid both for its type and if its for example present in the database. Some error handling are handled by the backend, for example only excecuting a certain branch if the input is valid.