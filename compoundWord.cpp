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

bool isCompoundWord(const string &word)
{
    if (confirmedCompoundWords.find(word) != confirmedCompoundWords.end())
    {
        return true;
    }

    if (word.empty())
    {
        return false;
    }

    for (int i = 1; i <= word.length(); ++i)
    {
        string prefix = word.substr(0, i);

        if (wordSet.find(prefix) != wordSet.end())
        {
            // Avoid considering the word as its own compound
            if (i == word.length() && prefix == word)
            {
                continue;
            }

            string suffix = word.substr(i);

            if (wordSet.find(suffix) != wordSet.end() || isCompoundWord(suffix))
            {
                confirmedCompoundWords.insert(word);
                return true;
            }
        }
    }

    return false;
}

void processWordList(const string &inputFileName)
{
    auto startTime = chrono::high_resolution_clock::now();

    wordSet.clear();
    confirmedCompoundWords.clear();

    ifstream inputFile(inputFileName);
    string word;
    vector<string> allWords;

    while (inputFile >> word)
    {
        wordSet.insert(word);
        allWords.push_back(word);
    }
    inputFile.close();


    vector<pair<int, string>> compoundWordsByLength;

    for (const string &word : allWords)
    {
        if (isCompoundWord(word))
        {
            compoundWordsByLength.push_back({word.length(), word});
        }
    }

    string longestCompoundWord = "";
    string secondLongestCompoundWord = "";

    if (!compoundWordsByLength.empty())
    {
        longestCompoundWord = compoundWordsByLength.back().second;
        compoundWordsByLength.pop_back();

        if (!compoundWordsByLength.empty())
        {
            secondLongestCompoundWord = compoundWordsByLength.back().second;
        }
    }

    auto endTime = chrono::high_resolution_clock::now();
    auto processingTime = chrono::duration_cast<chrono::milliseconds>(endTime - startTime);

    cout << "Results for " << inputFileName << ":\n";
    cout << "Longest Compound Word: " << longestCompoundWord << "\n";
    cout << "Second Longest Compound Word: " << secondLongestCompoundWord << "\n";
    cout << "Processing Time: " << processingTime.count() << " milliseconds\n\n";
}

int main()
{
    processWordList("Input_01.txt");
    processWordList("Input_02.txt");

    return 0;
}
