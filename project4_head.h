/**************************************************************************
Class:		CSCI 3333.01 Data Structures and Algorithms
			Spring 2025
Project 4:	The Word Ladder Problem
Name:		Abel Rodriguez
ID:         20627502
Description: In this header file, we define wordPairType 
			and other needed parts. 
**************************************************************************/
#ifndef PROJECT4_HEAD_H
#define PROJECT4_HEAD_H

#include <iostream>
#include <string>
using namespace std;

class wordPairType
{
private:
    string wd;   // word
    string pv;   // parent word

public:
    // default constructor
    wordPairType() : wd(""), pv("") {}

    // constructor with parameters
    wordPairType(const string &w, const string &p) : wd(w), pv(p) {}

    // copy constructor
    wordPairType(const wordPairType &other)
    {
        wd = other.wd;
        pv = other.pv;
    }

    // assignment operator
    wordPairType& operator=(const wordPairType &other)
    {
        if (this != &other) {
            wd = other.wd;
            pv = other.pv;
        }
        return *this;
    }

    // setter
    void setWordPair(string w, string p)
    {
        wd = w;
        pv = p;
    }

    string getWord() const { return wd; }
    string getParent() const { return pv; }

    void clear()
    {
        wd.clear();
        pv.clear();
    }
};

#endif
