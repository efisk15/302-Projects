/*Project 1 of COSC 302 at University of Tennessee Knoxville.
 *Written by Eli Fisk and Logan Draper.
 * Date: 1/20/2025
 *
 * Description:
 * We read in a file from the arguments and generate structs of Arists with maps
 *of names of albums paired with the album object pointer. We then print them
 *out "lexicographically" after extracting meta-data like time of album/artist
 *etc.
 */

#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>
#include <string>
using namespace std;

struct Song {
  string name;
  string time;
};

struct Album {
  map<int, Song> songs;
  string name;
  string time;
  int nsongs;

  // General Construtors.
  Album(string name, int nsongs) : name(name), nsongs(nsongs){};
  Album(){};
};

struct Artist {
  map<string, Album> albums;
  string name;
  string time;
  int nsongs;

  // General Construtors.
  Artist(){};
  Artist(int nsongs) : nsongs(nsongs){};
};

// Takes in a string, checks for formatting and then returns the time in
// seconds.
int time_to_integer(string s_time) {
  int colon_idx = -1;
  int colon_counter = 0;
  bool all_nums = true;
  // Find the colon's index and substring the two halves and do arithmetic on
  // them.
  for (int i = 0; i < s_time.length(); i++) {
    // Checks if everything else in the string is a number,
    if (s_time[i] == ':') {
      colon_idx = i;
      colon_counter++;
    }
    // Checks to see if everything else are all numbers.
    else if (s_time[i] < '0' || s_time[i] > '9') {
      all_nums = false;
    }
  }
  // If everything except the colon is a number, and there is only one colon.
  // We know that everything is formatted correctly.
  if (all_nums && colon_counter == 1) {
    string minutes = s_time.substr(0, colon_idx);
    string seconds = s_time.substr(colon_idx + 1, (int)s_time.length());
    return (stoi(seconds) + stoi(minutes) * 60);
  } else {
    cout << "Incorrect Formatting." << endl;
    cout << "Usage: xx:yy" << endl;
    return -1;
  }
}
// Traverses Album map field and returns time in xx:yy format.
string Sum_Album(Album Alb) {
  int sum_time = 0;
  int minutes = 0;
  int seconds = 0;
  for (map<int, Song>::iterator Songs = Alb.songs.begin();
       Songs != Alb.songs.end(); Songs++) {
    Song current_song = Songs->second;
    sum_time += time_to_integer(current_song.time);
  }
  minutes = sum_time / 60;
  seconds = (sum_time - (minutes * 60));
  string total_time;
  total_time.append(to_string(minutes));
  total_time.append(":");
  if (seconds < 10) {
    total_time.append("0");
  }
  total_time.append(to_string(seconds));
  return total_time;
}

// Traverses entire artist to return time in xx:yy format.
string Sum_Artist(Artist AA) {
  int sum = 0;
  int minutes = 0;
  int seconds = 0;
  string total_time;
  for (map<string, Album>::iterator Albums = AA.albums.begin();
       Albums != AA.albums.end(); Albums++) {
    Album current = Albums->second;
    sum += time_to_integer(Sum_Album(current));
  }
  minutes = sum / 60;
  seconds = (sum - (minutes * 60));
  total_time.append(to_string(minutes));
  total_time.append(":");
  if (seconds < 10) {
    total_time.append("0");
  }
  total_time.append(to_string(seconds));
  return total_time;
}
void Print_Albums(Artist AA) {
  // Given the Artist Struct, traverse Album map, then song map and print.
  for (map<string, Album>::iterator Albums = AA.albums.begin();
       Albums != AA.albums.end(); Albums++) {
    Album current_album = Albums->second;
    string time = Sum_Album(current_album);
    cout << setw(8) << " ";
    cout << current_album.name << ": " << current_album.nsongs << ", " << time
         << endl;

    for (map<int, Song>::iterator Songs = current_album.songs.begin();
         Songs != current_album.songs.end(); Songs++) {
      Song current_song = Songs->second;

      cout << setw(16) << " ";
      cout << Songs->first << ". " << current_song.name << ": "
           << current_song.time << endl;
    }
  }
}
map<string, Artist> Read_File(string fileName) {
  map<string, Artist> Artists;

  // Access File
  ifstream inputFile(fileName);
  string title, name, time, album, genre;
  int track = 0;
  while (inputFile >> title) {
    // Create variables and store input from file.
    Artist Artisto;
    Song new_song;
    inputFile >> time;
    inputFile >> name;
    inputFile >> album;
    inputFile >> genre;
    inputFile >> track;

    // Replace all dashes with spaces.
    while (name.find('_') != string::npos) {
      name[name.find('_')] = ' ';
    }
    while (title.find('_') != string::npos) {
      title[title.find('_')] = ' ';
    }
    while (album.find('_') != string::npos) {
      album[album.find('_')] = ' ';
    }
    Artisto.name = name;

    // If artist does not exist, create a new one.
    if (Artists.find(Artisto.name) == Artists.end()) {
      Artisto.nsongs = 0;
      Artists[Artisto.name] = Artisto;
    }

    // If Album for artist does not exist, create a new one.
    if (Artists[Artisto.name].albums.find(album) ==
        Artists[Artisto.name].albums.end()) {
      Album new_album;
      new_album.nsongs = 0;
      new_album.time = "0:00";
      new_album.name = album;
      Artists[Artisto.name].albums[album] = new_album;
    }

    // Store all necessary values.
    Artists[Artisto.name].time = "0:00";
    new_song.name = title;
    new_song.time = time;
    Artists[Artisto.name].albums[album].songs[track] = new_song;
    Artists[Artisto.name].albums[album].nsongs++;
    Artists[Artisto.name].nsongs++;
  }
  inputFile.close();
  return Artists;
}

int main(int argc, char *argv[]) {
  if (argc < 2) {
    cout << "Usage: './main File.txt'" << endl;
    return 0;
  }
  ifstream inputFile(argv[1]);
  if (inputFile.fail()) {
    cout << "Could not find that file." << endl;
    return 0;
  }
  inputFile.close();
  map<string, Artist> Artists = Read_File(argv[1]);
  for (map<string, Artist>::iterator Artist_list = Artists.begin();
       Artist_list != Artists.end(); Artist_list++) {
    Artist current_artist = Artist_list->second;
    string Artist_time = Sum_Artist(current_artist);
    cout << current_artist.name << ": " << current_artist.nsongs << ", "
         << Artist_time << endl;
    Print_Albums(current_artist);
  }
  return 0;
}
