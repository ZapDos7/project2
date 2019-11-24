#ifndef _UTILS_H_
#define _UTILS_H_

#include <stdio.h>
#include <iostream>
#include <vector>
#include <string>
#include <typeinfo>
#include <stdlib.h> //labs, llabs
#include <sstream>
#include <cmath> //abs with overload for float
#include <limits>
#include "cluster_object.h"
#include "my_vector.h"
/*#include "curve_point.h"
#include "curve.h"
#include "traversal.h"*/

/*Manhattan Distance*/
template <typename T>
double manhattan_distance(std::vector<T> v1, std::vector<T> v2);

template <typename T> /*template <class//typename T>*/
double manhattan_distance(std::vector<T> v1, std::vector<T> v2)
{ //sum twn (apoluti timi twn diaforwn twn  (suntetagmeni i tou v1, syntetagmeni i tou v2) )
	if (v1.size() != v2.size())
	{
		std::cerr << "Can't calculate distance, need same dimensions! Aborting...\n";
		exit(-1);
	}
	T temp;
	if ((typeid(temp) == typeid(float)) || (typeid(temp) == typeid(double)))
	{
		temp = 0.0;
	}
	else if (typeid(temp) == typeid(int))
	{
		temp = 0;
	}

	std::vector<T> diffs;
	for (unsigned int i = 0; i < v1.size(); ++i)
	{
		temp = abs(v1[i] - v2[i]);
		diffs.push_back(temp);
		//ws edw leitourgei ok
	}
	double result = 0.0;

	for (typename std::vector<T>::iterator it = diffs.begin(); it != diffs.end(); ++it)
	{
		//std::cerr << *it << "\n";
		result = result + (double)*it; //to athroisma twn apol. diaforwn
									   //std::cerr << result << "\n";
	}
	//std::cerr << "Ara telika exw " << result << "\n";
	return result;
}


/*Functions for DTW*/

/*template <typename T>
double true_euclidean(curve_point<T> cp1, curve_point<T> cp2)
{
  T x1 = cp1.get_x();
  T y1 = cp1.get_y();

  T x2 = cp2.get_x();
  T y2 = cp2.get_y();

  T x = x1 - x2; //calculating number to square in next step
  T y = y1 - y2;
  double dist;

  if (typeid(x) != typeid(y))
  {
    std::cerr << "Incompatible curve point types (What are you doing?!)\n";
    exit(-1);
  }
  if (typeid(x) == typeid(int))
  {
    int x_faux = x;
    int y_faux = y;
    dist = pow((double)x_faux, 2) + pow((double)y_faux, 2); //calculating Euclidean distance
    dist = sqrt(dist);
  }
  else if (typeid(x) == typeid(double))
  {
    double x_faux = x;
    double y_faux = y;
    dist = pow(x_faux, 2) + pow(y_faux, 2); //calculating Euclidean distance
    dist = sqrt(dist);
  }

  return dist;
}

//dtw me dynamic programming
template <typename T>
double dtw(curve<T> *c1, curve<T> *c2){

  double dist_mat[c1->get_size()+1][c2->get_size()+1];
  for(unsigned int i=1; i<= c1->get_size(); i++)
    dist_mat[i][0] = std::numeric_limits<double>::max();
  for(unsigned int i=1; i<= c2->get_size(); i++)
    dist_mat[0][i] = std::numeric_limits<double>::max();
  dist_mat[0][0] = 0.0;
  for(unsigned int i=1; i<= c1->get_size(); i++){
    for(unsigned int j=1; j<= c2->get_size(); j++){
      double mintrash = std::min(dist_mat[i-1][j-1], dist_mat[i][j-1]);
      mintrash = std::min(mintrash, dist_mat[i-1][j]);
      dist_mat[i][j] = true_euclidean(c1->get_points()[i-1], c2->get_points()[j-1] ) + mintrash;
    }
  }
  return dist_mat[c1->get_size()][c2->get_size()];

}*/


//format clusters based on kentra pou brethikan parapanw
template <typename T>
void format_clusters (std::vector<my_vector<T>> *cluster_centers, std::vector<cluster<T>> *clusters) {
  (*clusters).clear();
  for(unsigned int i=0; i<(*cluster_centers).size(); i++){
      cluster<T> onecatatime(&((*cluster_centers)[i]));
      (*clusters).push_back(onecatatime);
    }
}


#endif
