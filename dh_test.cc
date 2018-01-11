/* File: dh_test.cc
 * Author: Dan McFalls (dmcfalls@stanford.edu)
 * -------------------------------------------
 * Test file for the DH-Module class, using Faulkner's The Sound and the Fury
 * as an example text. Tests all class methods and reports the results by section.
 */

#include "dh.h"
#include <string>
#include <iostream>
#include <iomanip>

using namespace std;

const string kFilename("faulkner.txt");
const int kNumFreqWords = 50;
const int kNumFreqWordsSec = 20;

int main(int argc, char* argv[]) {
  DHModule faulknerStats = DHModule(kFilename);
  cout << fixed;
  cout << setprecision(2);

  cout << "Overview: " << endl;
  cout << endl;
  vector<string> mostFreq = faulknerStats.getMostFrequentWords(kNumFreqWords);
  cout << "Most frequent words: " << endl;
  for(size_t i = 0; i < mostFreq.size(); i++) {
    cout << "  " << mostFreq[i] << endl;
  }
  cout << "Total words: " << faulknerStats.getWordCount() << endl;
  cout << "Total sentences: " << faulknerStats.getSentenceCount() << endl;
  cout << "Unique words: " << faulknerStats.getUniqueWordCount() << endl;
  cout << "Average word length: " << faulknerStats.getAverageWordLength() << " characters" << endl;
  cout << "Average sentence length: " << faulknerStats.getAverageSentenceLength() << " words" << endl;
  cout << endl;

  cout << "Section Anaylsis: " << endl;
  cout << endl;
  vector<string> sectionNames = faulknerStats.getSectionNames();
  cout << "Sections:" << endl;
  for(string sectionName : sectionNames) {
    cout << "  " << sectionName << endl;
  }
  cout << endl;
  for (string sectionName : sectionNames) {
    cout << "Average word length in " << sectionName << ": " << faulknerStats.getAverageWordLengthFromSection(sectionName) << " characters" << endl;
    cout << "Average sentence length in " << sectionName << ": " << faulknerStats.getAverageSentenceLengthFromSection(sectionName) << " words" << endl;
    cout << "Most frequent words in " << sectionName << ": " << endl;
    vector<string> mostFreqSec = faulknerStats.getMostFrequentWordsFromSection(kNumFreqWordsSec, sectionName);
    for(size_t i = 0; i < mostFreqSec.size(); i++) {
      cout << "  " << mostFreqSec[i] << endl;
    }
    cout << endl;
  }
  cout << endl;

  cout << "Parts of Speech Analysis: " << endl << endl;
  cout << "Total number of nouns: " << faulknerStats.getTotalNumPartOfSpeech(noun) << endl;
  cout << "Total number of verbs: " << faulknerStats.getTotalNumPartOfSpeech(verb) << endl;
  cout << "Total number of adjectivs: " << faulknerStats.getTotalNumPartOfSpeech(adjective) << endl;
  cout << "Total number of adverbs: " << faulknerStats.getTotalNumPartOfSpeech(adverb) << endl;
  cout << "Total number of other words: " << faulknerStats.getTotalNumPartOfSpeech(other) << endl;
  cout << endl;

  for (string sectionName : sectionNames) {
    cout << "In section " << sectionName << ":" << endl;
    cout << "  Number of nouns: " << faulknerStats.getNumPartOfSpeechFromSection(noun, sectionName) << endl;
    cout << "  Number of verbs: " << faulknerStats.getNumPartOfSpeechFromSection(verb, sectionName) << endl;
    cout << "  Number of adjectivs: " << faulknerStats.getNumPartOfSpeechFromSection(adjective, sectionName) << endl;
    cout << "  Number of adverbs: " << faulknerStats.getNumPartOfSpeechFromSection(adverb, sectionName) << endl;
    cout << "  Number of other words: " << faulknerStats.getNumPartOfSpeechFromSection(other, sectionName) << endl;
    cout << endl;
  }

  return 0;
}
