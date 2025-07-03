# Search Engine
## Overview

The tiny search engine is a simplified version of how a search engine might operate, using a web crawler, word indexer, and search querier. Given a seedURL, we can crawl to a certain depth and collect all the websites embedded within other websites we search. From here we create a pageDirectory that saves every visited URL, depth, and the html code for the site. Then, we search through every word in every webpage and add it to an index that counts the number of times the word showed up in every document. Finally we can query words with "and" and "or" operators and have sites returned to us with the most matches. 

See the README.md file in the crawler and indexer and querier subdirectories for more detailed descriptions of these modules.
