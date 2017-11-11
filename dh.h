/* File: dh.h
 * ----------
 * Defines a digital humanities text analysis object. Initialized wih a filename containing a text
 * and allows queries for information about the text.
 * TODO: Implement sentence-level processing.
 * TODO: Add simple part-of-speech analysis.
 */

#include <iostream>
#include <string>
#include <set>
#include <map>
#include <vector>

//TODO: add enum to associate an integer with a part of speech (to remove messiness)

typedef struct sectionData {
  //Same function as the overall uniqueWords and wordFreqs maps, just for individual chapters
  std::map<std::string, int> uniqueWords;
  std::map<int, std::string> wordFreqs;
  //Both of these map "words of length _" : "number of times words of length _ appears"
  //std::map<int, int> sentenceLengths; (not yet implemented)
  std::map<int, int> wordLengths;
  //Parts of speech in section
  //TODO: add
  //Total number of words in the section
  int wordCount;
} sectionData;

class DHModule {
  public:
    //Constructor
    DHModule();
    DHModule(const std::string& filename);

    //Destructor
    ~DHModule();

    //Reads an entire text from a filename and gathers data from it
    void readTextFromFilename(const std::string& filename);

    //Returns word counts over the entire text
    int getWordCount();
    int getUniqueWordCount();

    std::vector<std::string> getSectionNames();

    //Computes average values from the database
    float getAverageWordLength();
    //int getAverageSentenceLength(); (not yet implemented)

    //Does the same as above, except on the section level
    float getAverageWordLengthFromSection(std::string sectionName);
    //int getAverageSentenceLengthFromSection(std::string sectionName); (not yet implemented)

    //Gets a list of the most frequent words and their frequencies from the text 
    std::vector<std::string> getMostFrequentWords(size_t numResults);
    std::vector<std::string> getMostFrequentWordsFromSection(size_t numResults, std::string sectionName);

  private:
    int wordCount;
    int totalChars;

    //Store the unique words in the text mapped to their frequenceis and vice-versa
    std::map<std::string, int> uniqueWords;
    std::map<int, std::string> wordFreqs;

    //Both of these map "item (sentence or word) of length _ (words/characters)" : "number of times item of length _ appears"
    std::map<int, int> sentenceLengths;
    std::map<int, int> wordLengths;

    //Maps the name of a section to its sectionData
    std::map<std::string, sectionData> sectionDataModules;

    //Collections of parts of speech (nouns, verbs, adjectives, adverbs)
    std::set<std::string> english_nouns;
    std::set<std::string> english_verbs;
    std::set<std::string> english_adjectives;
    std::set<std::string> english_adverbs;

    //Helper methods
    void stripAndClean(std::string& word);
    void populatePartsOfSpeechSets();
    bool isSectionMarker(std::string& word);
};
