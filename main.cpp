#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <cctype>
#include <filesystem>
#include <map>
#include <vector>
#include <utility>
#include <sstream>

using namespace std;
namespace fs = filesystem;

void buildIndex(map<string, vector<string>>& index)
{
    for (const auto& entry : fs::directory_iterator("documents"))
    {
        if (entry.path().extension() == ".txt")
    {
            ifstream file(entry.path());
            string word;
            
            while (file >> word)
    {
                transform(word.begin(), word.end(), word.begin(), ::tolower);
               if (find(index[word].begin(), index[word].end(),
                        entry.path().filename().string()) == index[word].end())
    {
                   index[word].push_back(entry.path().filename().string());
    }
    }
        
    file.close();
    }
    }
}

string cleanWord(string word);

int countMatches(string fileName, string query)
{
    ifstream file(fileName);
    string line;
    int count = 0;
    
    while (getline(file, line))
    {
        string word;
        stringstream ss(line);
        
        while (ss >> word)
    {
            word = cleanWord(word);
            transform(word.begin(), word.end(), word.begin(), ::tolower);
            
            if (word == query)
    {
                count++;
    }
    }
    }
    
    file.close();
    return count;
}

void showResults(string fileName, string query)
{
    ifstream file(fileName);
    string line;
    
    while (getline(file, line))
    {
        string lowerLine = line;
        transform(lowerLine.begin(), lowerLine.end(), lowerLine.begin(), ::tolower);
        if (lowerLine.find(query) != string::npos)
    {
            cout << line << endl;
    }
    }
     
    file.close();
}

bool compareResults(const pair<string, int>& a, const pair<string, int>& b)
{
    return a.second > b. second;
}

int calculateScore(string fileName, const vector<string>& searchWords)
{
    int score = 0;
    
    for (const string& word : searchWords)
    {
        score += countMatches(fileName, word);
    }
    
    return score;
}

vector<string> splitQuery(string query)
{
    vector<string> words;
    string word;
    stringstream ss(query);
    
    while (ss >> word)
    {
        transform(word.begin(), word.end(), word.begin(), ::tolower);
        word = cleanWord(word);
        
        if (!word.empty())
    {
            words.push_back(word);
    }
    }
    
    return words;
}

string cleanWord(string word)
{
    string cleaned;
    
    for (char c : word)
    {
       if (isalnum(c) || c == '+')
    {
           cleaned += c;
    }
    }
    
    return cleaned;
}

int main ()
{
    map<string, vector<string>> index;
    buildIndex(index);
    
    int documentCount = 0;
    
    for (const auto& entry : fs::directory_iterator("documents"))
    {
        if (entry.path().extension() == ".txt")
    {
            documentCount++;
    }
    }

    cout << " ✦ Mini Search Engine ✦\n";
    cout << "📁 Documents: " << documentCount << "\n\n";
    
    while (true)
    {
        string query;
        
        cout << "🔎 Search: ";
        getline(cin, query);
        
        if (query == "exit")
    {
            cout << "\n ⚬ Farewell!" << endl;
            break;
    }
        
        vector<string> searchWords = splitQuery(query);
        
        vector<pair<string, int>> results;
        
        for (const auto& entry : fs::directory_iterator("documents"))
    {
            if (entry.path().extension() == ".txt")
    {
                string fileName = entry.path().filename().string();
                string fullPath = entry.path().string();
                
                int score = calculateScore(fullPath, searchWords);
                
                if (score > 0)
    {
                    results.push_back({fileName, score});
    }
    }
    }
        
        sort(results.begin(), results.end(), [](const pair<string, int>& a, const pair<string, int>& b)
    {
            
            return a.second > b.second;
    });
        
        if (!results.empty())
    {
            int rank = 1;
            
            for (const auto& result : results)
    {
                cout << "\n" << rank << ". " << result.first << endl;
                cout << "   Score: " << result.second << endl;
                
                rank++;
    }
    }
        
        else
    {
            cout << "\n   No results found." << endl;
    }
    }
    
    return 0;
}
