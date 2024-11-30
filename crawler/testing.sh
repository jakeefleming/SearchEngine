#!/bin/bash
# Jake Fleming, 10/21/2024
# test crawler

# check if crawler executable exists
CRAWLER=./crawler

if [ ! -f "$CRAWLER" ]; then
  echo "Error: Crawler executable not found."
  exit 1
fi

echo "Test 1: No arguments"
$CRAWLER
if [ $? -eq 0 ]; then
  echo "FAIL: Expected non-zero exit status for missing arguments"
else
  echo "PASS"
fi
echo

echo "Test 2: Too many arguments"
$CRAWLER arg1 arg2 arg3 arg4
if [ $? -eq 0 ]; then
  echo "FAIL: Expected non-zero exit status for missing arguments"
else
  echo "PASS"
fi
echo

echo "Test 3: Missing maxDepth argument"
$CRAWLER http://cs50tse.cs.dartmouth.edu/tse/letters/index.html data
if [ $? -eq 0 ]; then
  echo "FAIL: Expected non-zero exit status for missing maxDepth"
else
  echo "PASS"
fi
echo

echo "Test 4: Invalid URL"
$CRAWLER invalid_url data 1
if [ $? -eq 0 ]; then
  echo "FAIL: Expected non-zero exit status for invalid URL"
else
  echo "PASS"
fi
echo

echo "Test 5: Invalid maxDepth (negative)"
$CRAWLER http://cs50tse.cs.dartmouth.edu/tse/letters/index.html data -1
if [ $? -eq 0 ]; then
  echo "FAIL: Expected non-zero exit status for invalid maxDepth"
else
  echo "PASS"
fi
echo

echo "Test 6: Valid crawl - letters site at depth 0"
mkdir -p data/letters-0
$CRAWLER http://cs50tse.cs.dartmouth.edu/tse/letters/index.html data/letters-0 0
if [ $? -ne 0 ]; then
  echo "FAIL: Crawler failed with valid arguments"
else
  # check if .crawler exists
  if [ -f "data/letters-0/.crawler" ]; then
    echo "PASS: .crawler exists"
  else
    echo "FAIL: .crawler does not exist"
  fi
  # check if the first file exists
  if [ -f "data/letters-0/1" ]; then
    echo "PASS: Page 1 exists"
  else
    echo "FAIL: Page 1 does not exist"
  fi
fi
echo

echo "Test 7: Valgrind memory check"
mkdir -p data/letters-valgrind
valgrind --leak-check=full --show-leak-kinds=all $CRAWLER http://cs50tse.cs.dartmouth.edu/tse/letters/index.html data/letters-valgrind 0 &> valgrind.out
if grep -q "no leaks are possible" valgrind.out; then
  echo "PASS: No memory leaks"
else
  echo "FAIL: Memory leaks found"
fi
echo

echo "Test 8: Larger Valgrind memory check"
mkdir -p data/letters-valgrind3
valgrind --leak-check=full --show-leak-kinds=all $CRAWLER http://cs50tse.cs.dartmouth.edu/tse/letters/index.html data/letters-valgrind3 3 &> valgrind.out
if grep -q "no leaks are possible" valgrind.out; then
  echo "PASS: No memory leaks"
else
  echo "FAIL: Memory leaks found"
fi
echo

echo "Test 9: Letters site at depth 2"
mkdir -p data/letters-2
$CRAWLER http://cs50tse.cs.dartmouth.edu/tse/letters/index.html data/letters-2 2
if [ $? -ne 0 ]; then
  echo "FAIL: Crawler failed with valid arguments"
else
  # check if multiple files exist
  if [ -f "data/letters-2/1" ] && [ -f "data/letters-2/2" ] && [ -f "data/letters-2/3" ]; then
    echo "PASS: Multiple pages exist"
  else
    echo "FAIL: Not all expected pages exist"
  fi
fi
echo

echo "Test 10: Letters site at depth 5"
mkdir -p data/letters-5
$CRAWLER http://cs50tse.cs.dartmouth.edu/tse/letters/index.html data/letters-5 5
if [ $? -ne 0 ]; then
  echo "FAIL: Crawler failed with valid arguments"
else
  page_count=$(ls data/letters-5 | wc -l)
  if [ $page_count -ge 9 ]; then
    echo "PASS: Crawled $page_count pages at depth 5"
  else
    echo "FAIL: Expected more pages, but only found $page_count"
  fi
fi
echo

echo "Test 11: Letters site at depth 5 from new seedURL to check for cycles"
mkdir -p data/letters-6
$CRAWLER http://cs50tse.cs.dartmouth.edu/tse/letters/B.html data/letters-6 6
if [ $? -ne 0 ]; then
  echo "FAIL: Crawler failed with valid arguments"
else
  page_count=$(ls data/letters-6 | wc -l)
  if [ $page_count -ge 9 ]; then
    echo "PASS: Crawled $page_count pages at depth 5"
  else
    echo "FAIL: Expected more pages, but only found $page_count"
  fi
fi
echo


echo "Test 12: Crawl toscrape site at depth 1"
mkdir -p data/toscrape-1
$CRAWLER http://cs50tse.cs.dartmouth.edu/tse/toscrape/ data/toscrape-1 1
if [ $? -ne 0 ]; then
  echo "FAIL: Crawler failed with valid arguments"
else
  page_count=$(ls data/toscrape-1 | wc -l)
  if [ $page_count -ge 5 ]; then
    echo "PASS: Crawled $page_count pages at depth 1"
  else
    echo "FAIL: Expected more pages, but only found $page_count"
  fi
fi
echo

echo "Test 13: Crawl wikipedia site at depth 1"
mkdir -p data/wikipedia-1
$CRAWLER http://cs50tse.cs.dartmouth.edu/tse/wikipedia/ data/wikipedia-1 1
if [ $? -ne 0 ]; then
  echo "FAIL: Crawler failed with valid arguments"
else
  page_count=$(ls data/wikipedia-1 | wc -l)
  if [ $page_count -ge 5 ]; then
    echo "PASS: Crawled $page_count pages at depth 1"
  else
    echo "FAIL: Expected more pages, but only found $page_count"
  fi
fi
echo

# clean directories
rm -rf data valgrind.out