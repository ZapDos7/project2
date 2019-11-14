#include <iostream>
#include <string.h>
#include <fstream>
#include <typeinfo>
#include <stdlib.h> //rand?
#include <algorithm> // std::count
#include <cstring>
#include <limits>
#include <sstream>
#include <iomanip>
#include <set>
#include <cmath>
#include "my_vector.h"
#include "curve.h"
#include "utils.h"
#include <chrono> // time measurements
#include <time.h>

std::string repeat_answer = "n";

//random init
std::vector<my_vector<int>> initialise_centers(int clusters, int plithos_vectors, std::vector<my_vector<int>> *vectors_array) {
  srand(time(NULL));
  std::set<int> ids;
  for (int i = 0; i < clusters; i++)
  { //epilegw tuxaio int anamesa sto 0 kai to n
    int rand_id = rand()%plithos_vectors;
    while (ids.find(rand_id) != ids.end()) //uparxei hdh auto pou brhke h rand
    {
      rand_id = rand()%plithos_vectors;
    }
    ids.insert(rand_id); //en telei bazw to unique value sto set m
  }
  std::vector<int> v_ids(ids.begin(), ids.end()); //epistrefw to set alla se vector apo id
  std::vector<my_vector<int>> ta_kentra;
  ta_kentra.clear();
  for (int i = 0; i < clusters; i++)
  {
    my_vector<int> one_v_atime;
    one_v_atime.set_id((*vectors_array)[v_ids[i]].get_id());
    one_v_atime.set_v((*vectors_array)[v_ids[i]].get_v());
    ta_kentra.push_back(one_v_atime);
  }
  if (ta_kentra.size() != (unsigned int)clusters)
  {
    std::cerr << "Error in initialise_centers.\n";
    exit(-2);
  }
  return ta_kentra;
}

//kmeans++
std::vector<my_vector<int>> initialise_centers_plus(int clusters){

}


int main(int argc, char *argv[])
{

  //test chamber

  //
  std::setprecision(14);
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
    while(number_of_vector_hash_functions < 0){
      std::cout << "number of hash functions not defined. Provide it:\n";
      std::cin >> number_of_vector_hash_functions;
    }
    while(number_of_clusters < 0){
      std::cout << "number of clusters not defined. Provide it:\n";
      std::cin >> number_of_clusters;
    }
  }
  else{ //keep default values and ask the 2 essentials (not default)
    while(number_of_vector_hash_functions < 0){
      std::cout << "number of hash functions not defined. Provide it:\n";
      std::cin >> number_of_vector_hash_functions;
    }
    while(number_of_clusters < 0){
      std::cout << "number of clusters not defined. Provide it:\n";
      std::cin >> number_of_clusters;
    }
  }

  //EAN DEN ORISTHKE APO GRAMMH ENTOLWN, DWSE MONOPATI DATASET:
  if (iset == false)
  {
    std::cout << "Define dataset path:\n";
    std::string inp1;
    std::cin >> dataset_path;
  }

  std::string what_is_the_input;
  int n = 0;                          //plithos twn vectors tou input file
  std::ifstream infile(dataset_path); //dataset: me tabs anamesa, ka8e grammi: id1    x11     x12     x13...
  std::string line;
  std::vector<my_vector<double>> vectors_array; //pinakas gia vectors
  std::vector<curve<double>> curves_array; //pinakas gia kampyles
  while (std::getline(infile, line))
  { //read files
    if(line == "vectors"){
      what_is_the_input = "vectors";
      continue;
    }

    if(line == "curves"){
      what_is_the_input = "curves";
      continue;
    }

    if(what_is_the_input == "vectors"){ // exoume na kanoyme me vectors
      my_vector<double> one_v_atime(line);
      //std::cout << one_v_atime.get_id()  <<"\n" ;
      vectors_array.push_back(one_v_atime);
      //std::cout << one_v_atime.get_id() << "\n";
      n++;
    }

    if(what_is_the_input == "curves"){ // exoume na kanoyme me vectors
      curve<double> one_v_atime(line);
      //std::cout << one_v_atime.get_id()  <<"\n" ;
      curves_array.push_back(one_v_atime);
      n++;
    }

  };
  infile.close();
  //KSEKINAME ANALOGWS TO INPUT TYPE
  if(what_is_the_input == "vectors"){ //EXOUME NA KANOYME ME VECTORS
    for(int i =0; i< vectors_array.size(); i++){
      for(int j=0; j< vectors_array[i].get_v().size(); j++)
        std::cout << std::setprecision(20) << vectors_array[i].get_v()[j] << " ";
      std::cout << "\n";
    }

    //random initialization se uparxonta vectors
    /*std::vector<my_vector<int>> cluster_centers; //ta arxika kentra twn clusters mas
    cluster_centers = initialise_centers(number_of_clusters, n, &vectors_array);

    for (unsigned int i = 0; i < cluster_centers.size(); i++)
    {
      std::cout << "Eimai to kentro " << cluster_centers[i].get_id() << "\n";
      std::vector<int> temp = cluster_centers[i].get_v();
      for (unsigned int j = 0; j < temp.size(); j++)
      {
        std::cout << temp[j] << " ";
      }
      std::cout << "\n";
    }*/
    //otan teleiwsei auto to init, cluster_centers.clear() kai meta kanoume to epomeno init!

  }
  else if(what_is_the_input == "curves"){

  }
  else{
    std::cout << "Den orises ti typou dedomena exoyme sthn prwth grammh opws eipe h ekfnwhsh, Enjoy the exit :* xoxo\n";
    exit(-1);
  }



}
