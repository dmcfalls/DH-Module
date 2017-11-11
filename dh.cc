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

//Section header constants
const static string kDefaultSection = "KDEFAULT";
const static string kSectionHeader = "*BEGINSECTION_";
const static size_t kSectionHeaderLen = kSectionHeader.length();

//Part of speech analysis constants
const string kPartsOfSpeechDir = "parts_of_speech";
const int kNumPartsOfSpeech = 5;
const string kNounsPath = "nouns.txt";
const string kVerbsPath = "verbs.txt";
const string kAdjectivesPath = "adjectives.txt";
const string kAdverbsPath = "adverbs.txt";
const string kPartsOfSpeechFilenames[kNumPartsOfSpeech] = {kNounsPath, kVerbsPath, kAdjectivesPath, kAdverbsPath};

/* (repeated from dh.h for convenience)
enum part_of_speech_t {
  noun = 0,
  verb = 1,
  adjective = 2,
  adverb = 3,
  other = 4
};
*/

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
  initializePartsOfSpeechMap(headerName);

  //Process text on a line-by-line basis, which will allow processing at the paragraph level
  while(getline(textFile, line)) {
    stringstream lineStream;
    lineStream << line;
    while(lineStream >> word) {
      //Check for section header, and if appears, change current section
      if(isSectionMarker(word)) {
        headerName = word.substr(kSectionHeaderLen);
        initializePartsOfSpeechMap(headerName);
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
      //Increment the appropriate part of speech count for the given word
      part_of_speech_t pos = partOfSpeechOf(word);
      sectionDataModules[headerName].wordPosKinds[pos]++;
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
    if (sectionName == kDefaultSection) continue;
    results.push_back(sectionName);
  }
  return results;
}

int DHModule::getTotalNumPartOfSpeech(part_of_speech_t kind) {
  int total = 0;
  for(auto section = sectionDataModules.begin(); section != sectionDataModules.end(); section++) {
    string sectionName = section->first;
    total += sectionDataModules[sectionName].wordPosKinds[kind];
  }
  return total;
}

int DHModule::getNumPartOfSpeechFromSection(part_of_speech_t kind, string& sectionName) {
  return sectionDataModules[sectionName].wordPosKinds[kind];
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
vector<string> DHModule::getMostFrequentWordsFromSection(size_t numResults, string& sectionName) {
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
  for(size_t i = 0; i < kNumPartsOfSpeech; i++) {
    string path = kPartsOfSpeechDir + "/" + kPartsOfSpeechFilenames[i];
    ifstream stream;
    string word;
    stream.open(path);

    while(getline(stream, word)) {
      stripAndClean(word);
      part_of_speech_t pos = (part_of_speech_t) i;
      if (pos == noun) {
        english_nouns.insert(word);
      } else if (pos == verb) {
        english_verbs.insert(word);
      } else if (pos == adjective) {
        english_adjectives.insert(word);
      } else if (pos == adverb) {
        english_adverbs.insert(word);
      } else {
        stream.close();
        return;
      }
    }

    stream.close();
  }
}

void DHModule::initializePartsOfSpeechMap(string& sectionName) {
  for (size_t i = 0; i < kNumPartsOfSpeech; i++) {
    part_of_speech_t pos = (part_of_speech_t) i;
    sectionDataModules[sectionName].wordPosKinds[pos] = 0;
  }
}

/* Returns the part of speech of the given word */
part_of_speech_t DHModule::partOfSpeechOf(string& word) {
  if (english_nouns.count(word)) {
    return noun;
  } else if (english_verbs.count(word)) {
    return verb;
  } else if (english_adjectives.count(word)) {
    return adjective;
  } else if (english_adverbs.count(word)) {
    return adverb;
  }
  return other;
}

/* Returns true if the given word is a special string to mark the beginning of a section */
bool DHModule::isSectionMarker(string& word) {
  return (word.length() >= kSectionHeaderLen) && (word.substr(0, kSectionHeaderLen) == kSectionHeader);
}
