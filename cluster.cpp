#include <iostream>
#include <string.h>
#include <fstream>
#include <typeinfo>
#include <stdlib.h>
#include <algorithm> // std::count
#include <cstring>
#include <limits>
#include <sstream>
#include <set>
#include <cmath>
#include "my_vector.h"
#include "utils.h"
#include <chrono> // time measurements

std::string repeat_answer = "n";

int main(int argc, char *argv[])
{

  //test chamber

  //

  bool complete = false;
  bool iset = false; ////an oxi orisma grammis entolos, 8a parw ta files apo path pou grafei o user
  bool cset = false;
  bool oset = false;
  char dataset_path[256];
  char config_path[256];
  char output_path[256];
  //    $./lsh -d <input file> -q <query file> -k <int> -L <int> -ο <output file>
  for (int i = 0; i < argc; i++)
  {
    if (strcmp("-i", argv[i]) == 0)
    {
      strcpy(dataset_path, argv[i + 1]);
      iset = true; /*std::cout <<"toxw\n";*/
    }              //pairnw to swsto onoma arxeiou apo to command line

    if (strcmp("-c", argv[i]) == 0)
    {
      strcpy(config_path, argv[i + 1]);
      cset = true; /*std::cout <<"toxw\n";*/
    }              //dinw timh sto K bash command line

    if (strcmp("-o", argv[i]) == 0)
    {
      strcpy(output_path, argv[i + 1]);
      oset = true; /*std::cout <<"toxw\n";*/
    }

    if (strcmp("-complete", argv[i]) == 0) //plithos hi functions gia dimiourgia twn g
    {
      complete = true;
    }
  }

  int number_of_grids = 2;
  int number_of_clusters = -1; //KALO EINAI NA YPARXEI STO ARXEIO
  int number_of_vector_hash_tables = 3;
  int number_of_vector_hash_functions = -1; //KALO EINAI NA YPARXEI STO ARXEIO
  if(cset){
    std::ifstream confile(config_path); //dataset: me tabs anamesa, ka8e grammi: id1    x11     x12     x13...
    std::string line;
    while (std::getline(confile, line))
    { //read files
      std::vector<std::string> tokens;
      std::stringstream check1(line);
      std::string intermediate;
      while (getline(check1, intermediate, ':'))
      {
          tokens.push_back(intermediate); //tokens has token[0] = id, token[1] = megethos, token[2] = all points (space separated now)
      }
      if(tokens[0] == "number_of_clusters")
        number_of_clusters = atoi(tokens[1].c_str());
      if(tokens[0] == "number_of_grids")
        number_of_grids = atoi(tokens[1].c_str());
      if(tokens[0] == "number_of_vector_hash_tables")
        number_of_vector_hash_tables =  atoi(tokens[1].c_str());
      if(tokens[0] == "number_of_vector_hash_functions")
        number_of_vector_hash_functions =  atoi(tokens[1].c_str());
    };
    confile.close();
    //ask values of essential vars (not default)
    if(number_of_vector_hash_functions < 0){
      std::cout << "number of hash functions not defined. Provide it:\n";
      std::cin >> number_of_vector_hash_functions;
    }
    if(number_of_clusters < 0){
      std::cout << "number of clusters not defined. Provide it:\n";
      std::cin >> number_of_clusters;
    }
  }
  else{ //keep default values and ask the 2 essentials (not default)
    if(number_of_vector_hash_functions < 0){
      std::cout << "number of hash functions not defined. Provide it:\n";
      std::cin >> number_of_vector_hash_functions;
    }
    if(number_of_clusters < 0){
      std::cout << "number of clusters not defined. Provide it:\n";
      std::cin >> number_of_clusters;
    }
  }

  int n = 0;                          //plithos twn vectors tou input file
  std::ifstream infile(dataset_path); //dataset: me tabs anamesa, ka8e grammi: id1    x11     x12     x13...
  std::string line;
  std::vector<my_vector<int>> vectors_array;
  while (std::getline(infile, line))
  { //read files
    my_vector<int> one_v_atime(line);
    //std::cout << one_v_atime.get_id()  <<"\n" ;
    vectors_array.push_back(one_v_atime);
    n++;
  };
  infile.close();



}
