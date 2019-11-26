#include <iostream>
#include <string.h>
#include <fstream>
#include <typeinfo>
#include <stdlib.h> //rand?
#include <algorithm> // std::count
#include <cstring>
#include <limits>
#include <sstream>
#include <unordered_map>
#include <iomanip>
#include <set>
#include <cmath>
#include "my_vector.h"
#include "curve.h"
#include "cluster_object.h"
#include "utils.h"
#include "ht.h" //gia range search lsh
#include <chrono> // time measurements
#include <time.h>
#include "update.hpp"
#include "inits.hpp"
#include "assign.hpp"

std::string repeat_answer = "n";

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
  int diastaseis_vecs; //self explanatory
  std::ifstream infile(dataset_path); //dataset: me tabs anamesa, ka8e grammi: id1    x11     x12     x13...
  std::string line;
  bool datatype_set = false;
  //std::vector<my_vector<double>> vectors_array; //pinakas gia vectors
  std::unordered_map<std::string, my_vector<double> > vectors_array; //key == id tou vector, mapped value == my_vectors. Ta ids einai o,ti nai nai g auto....
  std::unordered_map<std::string, curve<double> > curves_array; //pinakas gia kampyles
  while (std::getline(infile, line))
  { //read files
    if((datatype_set == false)&&(line.find("vectors") != std::string::npos)){
      what_is_the_input = "vectors";
      datatype_set = true;
      continue;
    }

    //std::cout << line << "\n";
    if((datatype_set == false)&&(line.find("curves") != std::string::npos)){
      what_is_the_input = "curves";
      datatype_set = true;
      continue;
    }

    if(what_is_the_input == "vectors"){ // exoume na kanoyme me vectors
      my_vector<double> one_v_atime(line);
      //std::cout << one_v_atime.get_id()  <<"\n" ;
      diastaseis_vecs = one_v_atime.get_v().size();
      vectors_array[one_v_atime.get_id()] = one_v_atime;

      //vectors_array.push_back(one_v_atime);
      //std::cout << one_v_atime.get_id() << "\n";
      n++;
    }

    if(what_is_the_input == "curves"){ // exoume na kanoyme me vectors
      curve<double> one_v_atime(line);
      //std::cout << one_v_atime.get_id()  <<"\n" ;
      curves_array[one_v_atime.get_id()] = one_v_atime;
      n++;
    }

  };
  infile.close();
  //KSEKINAME ANALOGWS TO INPUT TYPE
  if(what_is_the_input == "vectors"){ //EXOUME NA KANOYME ME VECTORS

    //edw mporeis na tsekareis oti ta inputs apo8hkeuthkan ok
    /*int jujuju = 0;
    for(auto x:vectors_array){ //to x einai pair me first = kleidi (to id ws string edw) kai second to antikeimno my_vector
      std::cout << x.first << x.second.get_id() << "\n";
      for(unsigned int i=0; i< x.second.get_v().size(); i++)
        std::cout << std::setprecision(12) << x.second.get_v()[i] << " ";
      std::cout << "\n";
      jujuju +=1;
    }
    std::cout << jujuju;*/

    std::vector<cluster<double>> clusters; //ta arxika mas clusters

    //cluster_centers = initialise_centers<double>(number_of_clusters, &vectors_array);
    initialise_centers_plus<double>(number_of_clusters, &vectors_array, &clusters);
    //format_clusters(&cluster_centers, &clusters);

    //edw mporeis na tsekareis ta kentra oti einai ok
    //for(unsigned int i = 0; i < clusters.size(); i++){
      //clusters[i].print_cluster();
    //}

    //checkarw oti ta clusters einai ok
    /*for (unsigned int i = 0; i < clusters.size(); i++)
    {
      std::cout << "Eimai to cluster " << clusters[i].get_center_id() << "kai eimai kala \n";
      for (int j = 0; j < clusters[i].get_center_coords().size(); j++)
      {
        std::cout << clusters[i].get_center_coords()[j] << " " ;
      }
      std::cout << std::endl;
    }*/

    /*simplest approach - ka8e shmeio arxika anati8etai sto kontinotero tou kentro*/
    //lloyd_ass(&clusters, &vectors_array);
    LSH_range_ass(&clusters, &vectors_array, diastaseis_vecs, number_of_vector_hash_tables, number_of_vector_hash_functions);
    for(unsigned int i = 0; i < clusters.size(); i++){
      clusters[i].print_cluster();
    }

    //update_mean(&clusters, diastaseis_vecs);
    update_pam(&clusters);
    LSH_range_ass(&clusters, &vectors_array, diastaseis_vecs, number_of_vector_hash_tables, number_of_vector_hash_functions);
    for(unsigned int i = 0; i < clusters.size(); i++){
      clusters[i].print_cluster();
    }

  }
  else if(what_is_the_input == "curves"){ //an mas do8oun kampyles
    std::vector<curve_cluster<double>> clusters; //ta arxika mas clusters
    //initialise_centers_curve(number_of_clusters, &curves_array, &clusters);
    initialise_centers_plus_curve(number_of_clusters, &curves_array, &clusters);
    for(unsigned int i = 0; i < clusters.size(); i++){
      clusters[i].print_cluster();
    }
  }
  else{
    std::cout << "Den orises ti typou dedomena exoyme sthn prwth grammh opws eipe h ekfnwhsh, Enjoy the exit :* xoxo\n";
    exit(-1);
  }



}
