# DH-Module: A Tool for Computational Text Analysis
This project provides a module for analyzing literary texts. Given an input text, you can query a <code>DHModule</code> for word frequencies, average word length, data on individual sections/chapters, etc.

I began developing this project to build a tool to help me read Faulkner's <i>The Sound and the Fury</i>. I'm continuing to be surprised by what I learn about the text using brute-force text processing--for example, that "Dilsey" is the name that appears most often in the text, that Quentin's section contains the word "the" vastly more often than any other section, and Jason's section contains more instances of "I" than any other section. These simple facts about the text are interesting, and very tempting for the critic to push forward with.

I hope to eventually evolve this C++ prototype into a more powerful web version with the hopes of building a comprehensive online text with which to read Faulkner and others with all the brute-force data within easy reach, ideally with a nicer interface.

Find a sample output of my simple test program in <code>sample-output.txt</code>.

Some of the module's current capabilities include:
<ul>
  <li>Word frequencies</li>
  <li>Average word length</li>
  <li>Average sentence length</li>
  <li>Part-of-speech analysis</li>
  <li>Section-specific analysis</li>
</ul>

Things I'm hoping to add/change as I continue to develop:
<ul>
  <li>Paragraph level processing</li>
  <li>Option to query on n-grams in text-level and section-level</li>
</ul>
