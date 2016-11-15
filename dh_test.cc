#include "dh.h"
#include <string>
#include <iostream>

using namespace std;

const string kFilename("faulkner.txt");
const int kNumFreqWords = 50;
const int kNumFreqWordsSec = 20;

int main(int argc, char* argv[]) {
  DHModule faulknerStats = DHModule(kFilename);
  cout << "Overview: " << endl;
  cout << endl;
  vector<string> mostFreq = faulknerStats.getMostFrequentWords(kNumFreqWords);
  cout << "Most frequent words: " << endl;
  for(size_t i = 0; i < mostFreq.size(); i++) {
    cout << "  " << mostFreq[i] << endl;
  }
  cout << "Total words: " << faulknerStats.getWordCount() << endl;
  cout << "Unique words: " << faulknerStats.getUniqueWordCount() << endl;
  cout << "Average word length: " << faulknerStats.getAverageWordLength() << endl;
  cout << endl;

  cout << "Section Anaylsis: " << endl;
  cout << endl;
  vector<string> sectionNames = faulknerStats.getSectionNames();
  cout << "Sections:" << endl;
  for(string sectionName : sectionNames) {
    cout << "  " << sectionName << endl;
  }
  for (string sectionName : sectionNames) {
    cout << "Most frequent words in " << sectionName << ": " << endl;
    vector<string> mostFreqSec = faulknerStats.getMostFrequentWordsFromSection(kNumFreqWordsSec, sectionName);
    for(size_t i = 0; i < mostFreqSec.size(); i++) {
      cout << "  " << mostFreqSec[i] << endl;
    }
  }
  return 0;
}