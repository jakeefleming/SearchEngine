#!/bin/bash
#
# testing.sh - test file for querier
#
# Jake Fleming, 11/4/2024

# Define paths for executables and directories
QUERIER="./querier"
PAGE_DIR="$HOME/cs50-dev/shared/tse/output/toscrape-2"
INDEX_FILE="$HOME/cs50-dev/shared/tse/output/toscrape-2.index"

# check exit status command
function test_command() {
    $@ &> /dev/null
    if [ $? -ne 0 ]; then
        echo "Test '$@' failed as expected"
    else
        echo "Test '$@' succeeded (unexpected)"
    fi
}

# cleanup old tests
rm -f test*.query test*.txt valgrind_*.out

echo "Running invalid argument tests 1-4..."
test_command $QUERIER           # no arguments
test_command $QUERIER $PAGE_DIR # only one argument
test_command $QUERIER $PAGE_DIR $INDEX_FILE extra_arg # extra argument
test_command $QUERIER "invalid_dir" $INDEX_FILE # invalid page directory
echo

echo "Run the following tests:"
echo
echo "fifty shades"
echo "tears and joy"
echo "romance or mystery"
echo "love and hate or war and peace"
echo "xyzzy"
echo " "
echo "and fifty"
echo "fifty or"
echo "joy and and tears"
echo "happy or or sad"
echo "science fiction and fantasy or horror"
echo "the"

echo
echo

# create sample query files for testing
echo "fifty shades" > test1.query           # Simple valid query
echo "tears and joy" > test2.query          # AND logic test
echo "romance or mystery" > test3.query     # OR logic test
echo "love and hate or war and peace" > test4.query  # Complex AND/OR test
echo "xyzzy" > test5.query                  # Non-existent word
echo " " > test6.query                      # Empty query
echo "and fifty" > test7.query              # Leading AND
echo "fifty or" > test8.query               # Trailing OR
echo "joy and and tears" > test9.query      # Consecutive AND
echo "happy or or sad" > test10.query       # Consecutive OR
echo "science fiction and fantasy or horror" > test11.query # Mixed genre query
echo "the" > test12.query                   # Common word

echo "Running querier integration tests 5-16..."
# execute querier with various queries
$QUERIER $PAGE_DIR $INDEX_FILE < test1.query > test1.txt
$QUERIER $PAGE_DIR $INDEX_FILE < test2.query > test2.txt
$QUERIER $PAGE_DIR $INDEX_FILE < test3.query > test3.txt
$QUERIER $PAGE_DIR $INDEX_FILE < test4.query > test4.txt
$QUERIER $PAGE_DIR $INDEX_FILE < test5.query > test5.txt

$QUERIER $PAGE_DIR $INDEX_FILE < test6.query > test6.txt
echo "Expected invlid query"
echo
$QUERIER $PAGE_DIR $INDEX_FILE < test7.query > test7.txt
echo "Expected leading AND error"
echo
$QUERIER $PAGE_DIR $INDEX_FILE < test8.query > test8.txt
echo "Expected trailing OR error"
echo
$QUERIER $PAGE_DIR $INDEX_FILE < test9.query > test9.txt
echo "Expected consecutive error"
echo
$QUERIER $PAGE_DIR $INDEX_FILE < test10.query > test10.txt
echo "Expected consecutive error"
echo
$QUERIER $PAGE_DIR $INDEX_FILE < test11.query > test11.txt
$QUERIER $PAGE_DIR $INDEX_FILE < test12.query > test12.txt
echo

# display a sample of the querier output for verification
echo "Sample output from test1.txt (fifty shades):"
head -n 10 test1.txt
echo

echo "Sample output from test4.txt (love and hate or war and peace):"
head -n 10 test4.txt
echo

echo "Running valgrind tests for memory leaks..."

# test querier with valgrind for memory leaks
valgrind --leak-check=full --show-leak-kinds=all $QUERIER $PAGE_DIR $INDEX_FILE < test1.query &> valgrind_querier1.out
if grep -q "no leaks are possible" valgrind_querier1.out; then
    echo "PASS: No memory leaks in querier for 'test1.query'"
else
    echo "FAIL: Memory leaks found in querier for 'test1.query'"
fi

valgrind --leak-check=full --show-leak-kinds=all $QUERIER $PAGE_DIR $INDEX_FILE < test4.query &> valgrind_querier2.out
if grep -q "no leaks are possible" valgrind_querier2.out; then
    echo "PASS: No memory leaks in querier for 'test4.query'"
else
    echo "FAIL: Memory leaks found in querier for 'test4.query'"
fi
echo

# cleanup
rm -f test*.query test*.txt valgrind_*.out

echo "All tests completed."