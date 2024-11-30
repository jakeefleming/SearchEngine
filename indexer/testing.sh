#!/bin/bash
#
# Jake Fleming, 10/28/2024

# define paths for executables and directories
INDEXER="./indexer"
INDEXTEST="./indextest"
INDEXCMP="$HOME/cs50-dev/shared/tse/indexcmp"
PAGE_DIR_1="$HOME/cs50-dev/shared/tse/output/letters-3"
PAGE_DIR_2="$HOME/cs50-dev/shared/tse/output/toscrape-2"     
PAGE_DIR_3="$HOME/cs50-dev/shared/tse/output/wikipedia-1"    

INDEX_FILE_1="letters.index"
INDEX_FILE_2="toscrape.index"
INDEX_FILE_3="wikipedia.index"
TEST_INDEX_FILE="test.index"

# utility function for running a command and checking the exit status
function test_command() {
    $@ &> /dev/null
    if [ $? -ne 0 ]; then
        echo "Test '$@' failed as expected"
    else
        echo "Test '$@' succeeded (unexpected)"
    fi
}

# cleanup old index files to restart tests
rm -f $INDEX_FILE_1 $INDEX_FILE_2 $INDEX_FILE_3 $TEST_INDEX_FILE

echo "Running invalid argument tests 1-6..."
test_command $INDEXER           # no arguments
test_command $INDEXER $PAGE_DIR_1 # only one argument
test_command $INDEXER $PAGE_DIR_1 $INDEX_FILE_1 extra_arg # extra argument
test_command $INDEXER "invalid_path" $INDEX_FILE_1 # non-existent page directory
test_command $INDEXER . $INDEX_FILE_1 # not a crawler directory
test_command $INDEXER $PAGE_DIR_1 "invalid_dir/index" # non-existent index path
echo

echo "Running valid integration tests 7-9..."
echo

# run indexer on various page directories and validate with indextest
$INDEXER $PAGE_DIR_1 $INDEX_FILE_1
$INDEXER $PAGE_DIR_2 $INDEX_FILE_2
$INDEXER $PAGE_DIR_3 $INDEX_FILE_3

# display a sample of the indexer output for letters.index
echo "Sample output from letters.index (first 10 lines):"
head -n 10 $INDEX_FILE_1
echo

# test if indextest can read and rewrite index files correctly
echo "Test 10-12 indextest re-writing of index files..."

$INDEXTEST $INDEX_FILE_1 $TEST_INDEX_FILE
$INDEXCMP $INDEX_FILE_1 $TEST_INDEX_FILE
echo "Comparison complete for letters index"
echo

$INDEXTEST $INDEX_FILE_2 $TEST_INDEX_FILE
$INDEXCMP $INDEX_FILE_2 $TEST_INDEX_FILE
echo "Comparison complete for toscrape index"
echo

$INDEXTEST $INDEX_FILE_3 $TEST_INDEX_FILE
$INDEXCMP $INDEX_FILE_3 $TEST_INDEX_FILE
echo "Comparison complete for wikipedia index"
echo

echo "Test 13: Compare wikipedia-1 with provided output"
$INDEXCMP $INDEX_FILE_3 "$HOME/cs50-dev/shared/tse/output/wikipedia-1.index"
echo

echo "Nothing printed by indexcmp implies files were all the same"
echo



# clean up the temporary test index file
rm -f $TEST_INDEX_FILE

echo "Running valgrind tests 14-15..."

valgrind --leak-check=full --show-leak-kinds=all $INDEXER $PAGE_DIR_1 $INDEX_FILE_1 &> valgrind_indexer.out
if grep -q "no leaks are possible" valgrind_indexer.out; then
    echo "PASS: No memory leaks in indexer"
else
    echo "FAIL: Memory leaks found in indexer"
fi
echo

valgrind --leak-check=full --show-leak-kinds=all $INDEXTEST $INDEX_FILE_1 $TEST_INDEX_FILE &> valgrind_indextest.out
if grep -q "no leaks are possible" valgrind_indextest.out; then
    echo "PASS: No memory leaks in indextest"
else
    echo "FAIL: Memory leaks found in indextest"
fi
echo

# clean up final test files
rm -f $INDEX_FILE_1 $INDEX_FILE_2 $INDEX_FILE_3 $TEST_INDEX_FILE valgrind_indexer.out valgrind_indextest.out

echo "All tests completed."
