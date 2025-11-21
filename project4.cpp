/**************************************************************************
Class:		CSCI 3333.01 Data Structures and Algorithms
		Spring 2025
Project 4:	The Word Ladder Problem
Name:		Abel Rodriguez
ID:          20627502
Description: Use a certain combination of graph, hash, list, stack,
	and queue data structures to solve the word ladder problem.
Input files: words_alpha.txt, wordpairs.txt
	Note: The dictionary is large, and it has 370,105 words.
	the wordpairs.txt contains five pairs of words for testing your program.
Output file: wordladders.txt
	First, output the loading time of the dictionary into this file.
	To save time, output the word ladder for each pair of two words in this file.
	You must print the ladder from the start word to the end word,
	and time used to find the ladder.
Example format of output file:
	Loading time in seconds of the dictionary
	...
	first pair of two words: startWord endWord
	Shortest word ladder (if any) and its length: ...
	Time to find the ladder: ...
	......
**************************************************************************/

#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <queue>
#include <stack>
#include <string>
#include <ctime>
#include <cctype>

#include "project4_head.h"

using namespace std;

/* ------------------------------------------------------
   BFS-based shortest word ladder finder
------------------------------------------------------ */
bool findWordLadder(const string &startWord,
                    const string &endWord,
                    map<string,string> &dict,
                    const string &letters,
                    queue<wordPairType> &wqueue,
                    stack<string> &wstack,
                    ofstream &outfile)
{
    // Clear queue & stack
    while (!wqueue.empty()) wqueue.pop();
    while (!wstack.empty()) wstack.pop();

    map<string,string> parent;
    parent.clear();

    // Initialize BFS queue
    wqueue.push(wordPairType(startWord, ""));
    parent[startWord] = "";

    bool found = false;
    unsigned int len = startWord.length();
    string current, candidate;

    while (!wqueue.empty() && !found)
    {
        wordPairType tmp = wqueue.front();
        wqueue.pop();

        current = tmp.getWord();
        if (current == endWord) {
            found = true;
            break;
        }

        candidate = current;

        // Generate neighbors
        for (unsigned int pos = 0; pos < len; pos++)
        {
            char original = candidate[pos];
            for (char c : letters)
            {
                if (c == original) continue;

                candidate[pos] = c;

                // Valid word and not visited
                if (dict.find(candidate) != dict.end() &&
                    parent.find(candidate) == parent.end())
                {
                    parent[candidate] = current;
                    wqueue.push(wordPairType(candidate, current));

                    if (candidate == endWord) {
                        found = true;
                        break;
                    }
                }
            }
            candidate[pos] = original;
            if (found) break;
        }
    }

    if (!found)
    {
        outfile << "No word ladder between " << startWord
                << " and " << endWord << "." << endl;
        return false;
    }

    // Reconstruct ladder using stack
    string cur = endWord;
    while (cur != "")
    {
        wstack.push(cur);
        cur = parent[cur];
    }

    outfile << "Shortest ladder from " << startWord
            << " to " << endWord << " (" << wstack.size() << " words):" << endl;

    while (!wstack.empty())
    {
        outfile << wstack.top();
        wstack.pop();
        if (!wstack.empty()) outfile << " => ";
    }
    outfile << endl;

    return true;
}


/* ------------------------------------------------------
   MAIN PROGRAM
------------------------------------------------------ */

int main()
{
    ifstream myfile;
    ifstream infile;
    ofstream outfile;

    string letters = "abcdefghijklmnopqrstuvwxyz";
    map<string,string> words[100];
    queue<wordPairType> wqueue;
    stack<string> wstack;

    string startWord, endWord, str;
    unsigned int len;
    long long cnt = 0;

    time_t start_time, end_time;

    // ------------------------------------------
    // Load Dictionary
    // ------------------------------------------
    cout << "Loading dictionary..." << endl;
    myfile.open("words_alpha.txt");
    outfile.open("wordladders.txt");

    if (!myfile.is_open()) {
        cerr << "ERROR: Cannot open words_alpha.txt" << endl;
        return 1;
    }

    start_time = time(NULL);

    while (myfile >> str)
    {
        // convert to lowercase
        for (char &c : str)
            c = tolower(c);

        // check all letters
        bool ok = true;
        for (char c : str)
            if (c < 'a' || c > 'z') ok = false;

        if (!ok) continue;

        len = str.length();
        if (len < 100 && len > 0) {
            words[len][str] = "";
            cnt++;
        }
    }
    myfile.close();

    end_time = time(NULL);

    outfile << "++++++++++++++++++++++++++++++\n";
    outfile << "Total words loaded: " << cnt << endl;
    outfile << "Loading time: " << difftime(end_time, start_time) << " sec\n\n";


    // ------------------------------------------
    // Process Word Pairs
    // ------------------------------------------
    infile.open("wordpairs.txt");
    if (!infile.is_open()) {
        cerr << "ERROR: Cannot open wordpairs.txt" << endl;
        return 1;
    }

    while (infile >> startWord >> endWord)
    {
        // lowercase
        for (char &c : startWord) c = tolower(c);
        for (char &c : endWord)   c = tolower(c);

        outfile << "----------------------------------------\n";
        outfile << "Word pair: " << startWord << "  " << endWord << endl;

        start_time = time(NULL);

        if (startWord.length() != endWord.length()) {
            outfile << "Words have different lengths. No ladder.\n";
        }
        else {
            len = startWord.length();

            if (words[len].find(startWord) == words[len].end() ||
                words[len].find(endWord) == words[len].end())
            {
                outfile << "One or both words not in dictionary.\n";
            }
            else {
                findWordLadder(startWord, endWord,
                               words[len], letters,
                               wqueue, wstack, outfile);
            }
        }

        end_time = time(NULL);
        outfile << "Time used: "
                << difftime(end_time, start_time) << " sec\n\n";
    }
    infile.close();

    outfile.close();
    cout << "Finished. Output written to wordladders.txt\n";
    return 0;
}
