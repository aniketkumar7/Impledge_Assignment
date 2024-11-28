#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <unordered_set>
#include <algorithm>
#include <chrono>
#include <queue>

using namespace std;

unordered_set<string> wordSet;
unordered_set<string> confirmedCompoundWords;
unordered_set<string> nonCompoundWords;

bool isCompoundWord(const string &word)
{
    // Check if word has already been processed
    if (confirmedCompoundWords.find(word) != confirmedCompoundWords.end())
    {
        return true;
    }

    // Check if word is not a compound word
    if (nonCompoundWords.find(word) != nonCompoundWords.end())
    {
        return false;
    }

    // Base case: Empty word is not a compound word
    if (word.empty())
    {
        return false;
    }

    for (int i = 1; i <= word.length(); ++i)
    {
        string prefix = word.substr(0, i);

        // Check if prefix exists in dictionary
        if (wordSet.find(prefix) != wordSet.end())
        {
            // Avoid considering the word as its own compound
            if (i == word.length() && prefix == word)
            {
                continue;
            }

            // Get suffix of the word
            string suffix = word.substr(i);

            // Check if suffix is either a word or can be further decomposed
            if (wordSet.find(suffix) != wordSet.end() || isCompoundWord(suffix))
            {
                confirmedCompoundWords.insert(word);
                return true;
            }
        }
    }

    nonCompoundWords.insert(word);
    return false;
}

void processWordList(const string &inputFileName)
{
    auto startTime = chrono::high_resolution_clock::now();

    // Clear previous data
    wordSet.clear();
    confirmedCompoundWords.clear();
    nonCompoundWords.clear();

    // Read words from input file
    ifstream inputFile(inputFileName);
    string word;
    vector<string> allWords;

    while (inputFile >> word)
    {
        wordSet.insert(word);
        allWords.push_back(word);
    }
    inputFile.close();

    // Priority queue to store compound words sorted by length
    priority_queue<pair<int, string>> compoundWordsByLength;

    // Identify all compound words
    for (const string &word : allWords)
    {
        if (isCompoundWord(word))
        {
            compoundWordsByLength.push({word.length(), word});
        }
    }

    // Extract the two longest compound words
    string longestCompoundWord = "";
    string secondLongestCompoundWord = "";

    if (!compoundWordsByLength.empty())
    {
        longestCompoundWord = compoundWordsByLength.top().second;
        compoundWordsByLength.pop();

        if (!compoundWordsByLength.empty())
        {
            secondLongestCompoundWord = compoundWordsByLength.top().second;
        }
    }

    auto endTime = chrono::high_resolution_clock::now();
    auto processingTime = chrono::duration_cast<chrono::milliseconds>(endTime - startTime);

    // Display results
    cout << "Results for " << inputFileName << ":\n";
    cout << "Longest Compound Word: " << longestCompoundWord << "\n";
    cout << "Second Longest Compound Word: " << secondLongestCompoundWord << "\n";
    cout << "Processing Time: " << processingTime.count() << " milliseconds\n\n";
}

int main()
{
    // Process both input files
    processWordList("Input_01.txt");
    processWordList("Input_02.txt");

    return 0;
}