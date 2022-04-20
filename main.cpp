#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <iomanip>
#include <unordered_map>
#include <algorithm>
#include <chrono>

struct song {
    float acousticness;
    float danceability;
    int duration;
    float energy;
    bool explic;
    float instrumentalness;
    int key;
    float liveness;
    float loudness;
    int mode;
    std::string name;
    int popularity;
    float speechiness;
    float tempo;
    float valence;
    int year;
    std::string artist;
};
void createGraph(std::unordered_map<std::string, std::vector<song>>& uncutGems);
void heapifyDown(std::vector<song>&vec, int size, int root);
song extractMin(std::vector<song>& vec);
void buildHeap(std::vector<song>& vec);

int main() {
    std::unordered_map<std::string, std::vector<song>> uncutGems;
    createGraph(uncutGems);
    std::vector<song> playlist;
    std::string choice = "";
    std::string choice2 = "";
    std::string choice3="";
    std::string choice4 = "";
    std::cout<<"Welcome to the Hidden Gems music tool!"<< std::endl;
    // takes user input, adds to playlist, displays playlist, displays playlist statistics, or exits program based on user input.
    while (choice != "4"){
        choice = "";
        choice2 = "";
        choice3="";
        choice4 = "";
        std::cout<<'\n'<<"1.Search for an artist."<<'\n'<<"2.Check your playlist's statistics."<<'\n'<<"3.Display your playlist." <<'\n'<< "4.Exit Program"<< std::endl;
        std::cin >> choice;
        if (choice == "1"){
            std::cout<<"Enter an artist"<< std::endl;
            std::cin.ignore();
            std::getline(std::cin, choice2);
            std::for_each(choice2.begin(), choice2.end(), [](char & c) {
                c = ::tolower(c);
            });
            auto iter = uncutGems.find(choice2);
            if(iter == uncutGems.end())
                std::cout << "Artist not found in our database. Please try again." << std::endl;
            else {
                int tempMin = 0;
                song tempSong;
                std::vector<song> hiddenGems;
                std::vector<song> heapenGems;
                std::vector<song> tempSongs = iter->second;
                int index = 0;
                auto start = std::chrono::high_resolution_clock::now();
                while(hiddenGems.size() < 10 && hiddenGems.size() < iter->second.size()) {
                    tempMin = 101;
                    for(unsigned int i = 0; i < tempSongs.size(); i++) {
                        if(tempSongs[i].popularity < tempMin) {
                            tempSong = tempSongs[i];
                            tempMin = tempSongs[i].popularity;
                            index = i;
                        }
                    }  
                    tempSongs.erase(tempSongs.begin() + index);
                    hiddenGems.push_back(tempSong);
                } auto stop = std::chrono::high_resolution_clock::now();
                std::chrono::duration<double, std::milli> bruteTime = stop - start;
                tempSongs = iter->second;
                start = std::chrono::high_resolution_clock::now();
                buildHeap(tempSongs);
                while(heapenGems.size() < 10 && heapenGems.size() < iter->second.size())
                    heapenGems.push_back(extractMin(tempSongs));
                stop = std::chrono::high_resolution_clock::now();
                std::chrono::duration<double, std::milli> heapTime = stop - start;
                std::cout << "Time to sort using brute force approach " << bruteTime.count() << std::endl;
                std::cout << "Time to sort using heap sort " << heapTime.count() << std::endl;
                while(choice4 != "-1") {
                    std::cout << "Hidden gems by " << iter->first << std::endl;
                    for(unsigned int i = 0; i < hiddenGems.size(); i++)
                        std::cout << i+1 << ". " << hiddenGems[i].name << " popularity rating: " << hiddenGems[i].popularity << std::endl;
                    std::cout << "Which song would you like to add to the playlist?" << std::endl;
                    std::cout << "Enter -1 to save playlist and exit to start.";
                    std::cin >> choice4;
                    int songChoice = std::stoi(choice4);
                    if(songChoice != -1 && songChoice > 0 && songChoice <= hiddenGems.size())
                        playlist.push_back(hiddenGems[songChoice-1]);
                }
            }
        }
        if (choice == "2" && playlist.size() > 0){
            std::cout<<"Which playlist average would you like to see?"<<'\n'<<std::endl;
            std::cout<<"1.Acousticness"<< '\n'<<"2.Danceability"<< '\n'<<"3.Energy"<<'\n'<<"4.Instrumentalness"<<'\n'<<"5.Liveness"<<std::endl;
            std::cout<<"6.Loudness"<<'\n'<<"7.Popularity"<<'\n'<<"8.Tempo"<<std::endl;
            std::cin>>choice3;
            int statChoice = std::stoi(choice3);
            float average = 0;
            for(unsigned int i = 0; i < playlist.size(); i++) {
                if(statChoice == 1)
                    average += playlist[i].acousticness;
                else if(statChoice == 2)
                    average += playlist[i].danceability;
                else if(statChoice == 3) 
                    average += playlist[i].energy;
                else if(statChoice == 4)
                    average += playlist[i].instrumentalness;
                else if(statChoice == 5)
                    average += playlist[i].liveness;
                else if(statChoice == 6)
                    average += playlist[i].loudness;
                else if(statChoice == 7)
                    average += playlist[i].popularity;
                else if(statChoice == 8) 
                    average += playlist[i].tempo;
            }
            average = average/(float)playlist.size();
            if(statChoice == 1)
                std::cout << "The average acousticness of your playlist is " << average << std::endl;
            else if(statChoice == 2)
                std::cout << "The average danceability of your playlist is " << average << std::endl;
            else if(statChoice == 3) 
                std::cout << "The average energy of your playlist is " << average << std::endl;
            else if(statChoice == 4)
                std::cout << "The average instrumentalness of your playlist is " << average << std::endl;
            else if(statChoice == 5)
                std::cout << "The average liveness of your playlist is " << average << std::endl;
            else if(statChoice == 6)
                std::cout << "The average loudness of your playlist is " << average << std::endl;
            else if(statChoice == 7)
                std::cout << "The average popularity of your playlist is " << average << std::endl;
            else if(statChoice == 8) 
                std::cout << "The average tempo of your playlist is " << average << std::endl;
        } else if(choice == "2")
            std::cout << "You don't have any songs in your playlist!" << std::endl;
        if(choice == "3") {
            if(playlist.size() == 0)
                std::cout << "You don't have any songs in your playlist!" << std::endl;
            else {
                std::cout << "Your playlist:" << std::endl;
                for(unsigned int i = 0; i < playlist.size(); i++)
                    std::cout << i + 1 << ". " << playlist[i].name << " by " << playlist[i].artist << " popularity rating: " << playlist[i].popularity << std::endl;
            }
        }
    }
    return 0; 
}

// takes input data and converts into disconnected graph of artists
void createGraph(std::unordered_map<std::string, std::vector<song>>& uncutGems) {
    // acousticness, artists, danceability, duration_ms, energy, explicit, id, instrument, key, liveness, loudness, mode
    // name, popularity, release, speechiness, tempo, valence, year
    std::ifstream inFile("theRealUncutGems.csv");
    std::string acousticness;
    std::string artists;
    std::string danceability;
    std::string duration_ms;
    std::string energy;
    std::string explic;
    std::string id;
    std::string instrument;
    std::string key;
    std::string liveness;
    std::string loudness;
    std::string mode;
    std::string name;
    std::string popularity;
    std::string release;
    std::string speechiness;
    std::string tempo;
    std::string valence;
    std::string year;
    float acousticFloat;
    float danceFloat;
    unsigned int durationInt;
    float energyFloat;
    bool explicBool;
    float instFloat;
    int keyInt;
    float liveFloat;    
    float loudFloat;
    int modeInt;
    int popInt;
    float speechinessFloat;
    float tempoFloat;
    float valenceFloat;
    int yearInt;
    if (inFile.is_open()) {
        std::string lineFromFile;
        // read heading
        std::getline(inFile, lineFromFile);
        while (std::getline(inFile, lineFromFile)) {
            song mySong; 
            std::vector<std::string> artistVect;
            std::istringstream stream(lineFromFile);
            std::getline(stream, acousticness, ',');
            std::getline(stream, artists, '[');
            std::getline(stream, artists, ']');
            std::istringstream artistStream(artists);
            while(std::getline(artistStream, artists, '\'')) {
                std::getline(artistStream, artists, '\''); 
                artistVect.push_back(artists);
            }
            std::getline(stream, danceability, ',');
            std::getline(stream, danceability, ',');
            std::getline(stream, duration_ms, ',');
            std::getline(stream, energy, ',');
            std::getline(stream, explic, ',');
            std::getline(stream, id, ',');
            std::getline(stream, instrument, ',');
            std::getline(stream, key, ',');
            std::getline(stream, liveness, ',');
            std::getline(stream, loudness, ',');
            std::getline(stream, mode, ',');
            std::getline(stream, popularity, ',');
            std::getline(stream, release, ',');
            std::getline(stream, speechiness, ',');
            std::getline(stream, tempo, ',');
            std::getline(stream, valence, ',');
            std::getline(stream, year, ',');
            std::getline(stream, name);
            mySong.name = name;
            try {
                acousticFloat = std::stof(acousticness);
            }
            catch (...) {
                acousticFloat = 0;
            }
            mySong.acousticness = acousticFloat;
            try {
                danceFloat = std::stof(danceability);
            }
            catch (...) {
                danceFloat = 0;
            }
            mySong.danceability = danceFloat;
            try {
                durationInt = std::stoi(duration_ms);
            }
            catch (...) {
                durationInt = 0;
            }
            mySong.duration = durationInt;
            try {
                energyFloat = std::stof(energy);
            }
            catch (...) {
                energyFloat = 0;
            }
            mySong.energy = energyFloat;
            if(explic == "1")
                explicBool = true;
            else
                explicBool = false;
            mySong.explic = explicBool;
            try {
                instFloat = std::stof(instrument);
            }
            catch (...) {
                instFloat = 0;
            }
            mySong.instrumentalness = instFloat;
            try {
                keyInt = std::stoi(key);
            }
            catch (...) {
                keyInt = 0;
            }
            mySong.key = keyInt;
            try {
                liveFloat = std::stof(liveness);
            }
            catch (...) {
                liveFloat = 0;
            }
            mySong.liveness = liveFloat;
            try {
                loudFloat = std::stof(loudness);
            }
            catch (...) {
                loudFloat = 0;
            }
            mySong.loudness = loudFloat;
            try {
                modeInt = std::stoi(mode);
            }
            catch (...) {
                modeInt = 0;
            }
            mySong.mode = modeInt;
            try {
                popInt = std::stoi(popularity);
            }
            catch (...) {
                popInt = 0;
            }
            mySong.popularity = popInt;
            try {
                speechinessFloat = std::stof(speechiness);
            }
            catch (...) {
                speechinessFloat = 0;
            }
            mySong.speechiness = speechinessFloat;
            try {
                tempoFloat = std::stof(tempo);
            }
            catch (...) {
                tempoFloat = 0;
            }
            mySong.tempo = tempoFloat;
            try {
                valenceFloat = std::stof(valence);
            }
            catch (...) {
                valenceFloat = 0;
            }
            mySong.valence = valenceFloat;
            try {
                yearInt = std::stoi(year);
            }
            catch (...) {
                yearInt = 0;
            }
            mySong.year = yearInt;
            for(unsigned int i = 0; i < artistVect.size(); i++) {
                std::string temp = artistVect[i];
                mySong.artist = temp;
                // convert to lower case for convenient searching
                std::for_each(temp.begin(), temp.end(), [](char & c) {
                       c = ::tolower(c);
                });
                uncutGems[temp].push_back(mySong);
            }
        }
    }
}
// Heapify functions to execute heap sort.
void heapifyDown(std::vector<song>&vec, int size, int root){
    bool swapMade = false;
    int leftChild = root + 1;
    int rightChild = root + 2;
    int currentSmallest = root;
    if(leftChild < size && vec[leftChild].popularity < vec[currentSmallest].popularity){ //using logic of slide 40 of the heaps lecture slide
        currentSmallest = leftChild; 
        swapMade = true;
    }
    if(rightChild < size && vec[rightChild].popularity < vec[currentSmallest].popularity){
        currentSmallest = rightChild;
        swapMade = true;
    }
    if(swapMade == true){
        song mySong = vec[root];
        vec[root] = vec[currentSmallest];
        vec[currentSmallest] = mySong;
        heapifyDown(vec, size, currentSmallest);
    }
}
song extractMin(std::vector<song>& vec)
{
    song root = vec[0];
    vec[0] = vec[vec.size()-1]; //also using logic from slide 40
    vec.erase(vec.end()-1);
    heapifyDown(vec, vec.size(), 0);
    return root; 
}

void buildHeap(std::vector<song>& vec){
    int startIdx = (vec.size() / 2) - 1;
    for (int i = startIdx; i >= 0; i--)
        heapifyDown(vec, vec.size(), i);
}