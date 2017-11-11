/* File: dh.cc
 * Author: Dan McFalls (dmcfalls@stanford.edu)
 * -----------
 * This class is a framework of analyzing a digital text of a literary work.
 * Objects are initialized around a filename of a primary text, and they can
 * be queried for various information about the text.
 */

#include "dh.h"
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <set>
#include <map>
#include <vector>
#include <cstddef>

using namespace std;

const static string kDefaultSection = "";
const static string kSectionHeader = "*BEGINSECTION_";
const static size_t kSectionHeaderLen = kSectionHeader.length();

const string kPartsOfSpeechDir = "parts_of_speech";
const string kNounsPath = "nouns.txt";
const string kVerbsPath = "verbs.txt";
const string kAdjectivesPath = "adjectives.txt";
const string kAdverbsPath = "adverbs.txt";
const string kPartsOfSpeechFilenames[4] = {kNounsPath, kVerbsPath, kAdjectivesPath, kAdverbsPath};

/* Default Constructor */
DHModule::DHModule() {
  wordCount = 0;
  totalChars = 0;
  populatePartsOfSpeechSets();
}

/* Constructor */
DHModule::DHModule(const string& filename) {
  wordCount = 0;
  totalChars = 0;
  populatePartsOfSpeechSets();
  readTextFromFilename(filename);
}

/* Destructor */
DHModule::~DHModule() {
    //Do nothing (yet, at least)
}

/* Populates all the data structures given a primary text file name */
void DHModule::readTextFromFilename(const string& filename) {
  ifstream textFile;
  textFile.open(filename);
  string line;
  string word;
  string headerName = kDefaultSection;

  //Process text on a line-by-line basis, which will allow processing at the paragraph level
  while(getline(textFile, line)) {
    stringstream lineStream;
    lineStream << line;
    while(lineStream >> word) {
      //Check for section header, and if appears, change current section
      if(isSectionMarker(word)) {
        headerName = word.substr(kSectionHeaderLen);
        continue;
      }

      //Clean up the input string
      stripAndClean(word);

      //Process unique words (build map with word:freq as key:value pairing)
      wordCount++;
      totalChars += word.length();
      if(uniqueWords.find(word) == uniqueWords.end()) {
        if(headerName != kDefaultSection) {
            sectionDataModules[headerName].uniqueWords[word] = 1;
        }
        uniqueWords[word] = 1;
      } else {
        if(headerName != kDefaultSection) {
            sectionDataModules[headerName].uniqueWords[word]++;
        }
        uniqueWords[word]++;
      }
      if(headerName != kDefaultSection) {
        sectionDataModules[headerName].wordCount++;
        sectionDataModules[headerName].wordLengths[word.length()]++;
      }
    }
  }
  //Build word frequecy map (which is sorted with freq:word as key:value pairing)
  for(auto it = uniqueWords.begin(); it != uniqueWords.end(); it++) {
    wordFreqs[it->second] = it->first;
  }
  
  //Build word frequency map for each section
  for(auto section = sectionDataModules.begin(); section != sectionDataModules.end(); section++) {
    string key = section->first;
    for(auto it = sectionDataModules[key].uniqueWords.begin(); it != sectionDataModules[key].uniqueWords.end(); it++) {
      sectionDataModules[key].wordFreqs[it->second] = it->first;
    }
  }

  textFile.close();
}

/* Returns the total word count for the text */
int DHModule::getWordCount() {
  return wordCount;
}

/* Returns the number of unique words used in the text */
int DHModule::getUniqueWordCount() {
  return uniqueWords.size();
}

/* Returns all section names (these are usually hard-coded by the user */
vector<string> DHModule::getSectionNames() {
  vector<string> results;
  for(auto section = sectionDataModules.begin(); section != sectionDataModules.end(); section++) {
    string sectionName = section->first;
    results.push_back(sectionName);
  }
  return results;
}

/* Returns the average length over all words in the text */
float DHModule::getAverageWordLength() {
  float result = (float)totalChars / wordCount;
  return result;
}

/* Returns the n most frequently used words in the text where n = numResults */
vector<string> DHModule::getMostFrequentWords(size_t numResults) {
  vector<string> freqsInOrder;
  string word, freq;
  for(auto it = wordFreqs.begin(); it != wordFreqs.end(); it++) {
    freq = to_string(it->first);
    word = it->second;
    freqsInOrder.push_back(word + ": " + freq);
  }
  vector<string> mostFrequent;
  for(size_t i = 0; i < numResults; i++) {
    mostFrequent.push_back(freqsInOrder[freqsInOrder.size() - i - 1]);
  }
  return mostFrequent;
}

/* Returns the n most frequently used words from the given section */
vector<string> DHModule::getMostFrequentWordsFromSection(size_t numResults, string sectionName) {
  vector<string> freqsInOrder;
  string word, freq;
  for(auto it = sectionDataModules[sectionName].wordFreqs.begin(); it != sectionDataModules[sectionName].wordFreqs.end(); it++) {
    freq = to_string(it->first);
    word = it->second;
    freqsInOrder.push_back(word + ": " + freq);
  }
  vector<string> mostFrequent;
  for(size_t i = 0; i < numResults; i++) {
    mostFrequent.push_back(freqsInOrder[freqsInOrder.size() - i - 1]);
  }
  return mostFrequent;
}

  /******************************/
  /*** Private Helper Methods ***/
  /******************************/

/* Strips words of non-alphabet characters and makes them all lower-case */
void DHModule::stripAndClean(string& word) {
  for(size_t i = 0; i < word.length(); i++) {
    if(!isalpha(word[i])) {
      word.erase(i--, 1);
    } else {
      word[i] = tolower(word[i]);
    }
  }
}

/* Populates parts of speech maps with their respective lists of words */
void DHModule::populatePartsOfSpeechSets() {
  for(size_t i = 0; i < 4; i++) {
    string path = kPartsOfSpeechDir + "/" + kPartsOfSpeechFilenames[i];
    ifstream stream;
    string word;
    stream.open(path);

    while(getline(stream, word)) {
      stripAndClean(word);
      if (i == 0) {
        english_nouns.insert(word);
      } else if (i == 1) {
        english_verbs.insert(word);
      } else if (i == 2) {
        english_adjectives.insert(word);
      } else {
        english_adverbs.insert(word);
      }
    }

    stream.close();
  }
}

/* Returns true if the given word is a special string to mark the beginning of a section */
bool DHModule::isSectionMarker(string& word) {
  return (word.length() >= kSectionHeaderLen) && (word.substr(0, kSectionHeaderLen) == kSectionHeader);
}
