/*
 * =====================================================================================
 *
 *       Filename:  quickmedian.cpp
 *
 *    Description:  Quick median (C++)
 *
 *        Version:  1.0
 *        Created:  02/14/2014 04:44:03 PM
 *       Revision:  none
 *       Compiler:  g++ (g++ -o test quickmedian.cpp -std=c++11 2>&1; ./test 200)
 *
 *         Author:  Michael Likholet <m.likholet@ya.ru>
 *        Company:  -
 *
 * =====================================================================================
 */

#include <vector>
#include <iterator>
#include <iostream>
#include <string>
#include <algorithm>
#include <utility>
#include <sstream>
#include <chrono>

#include <cstdlib>

class QuickMedian {
	public:
		QuickMedian(std::vector<int> vec): data(vec) {}
		~QuickMedian() {}

		int findMedian();
		int findMedian(int l, int r, int order);

	private:
		std::vector<int> data;

		int Partition(int l, int r);
		int randomizedPartition(int l, int r);
};

int QuickMedian::findMedian() {
	return findMedian(0, data.size(), data.size()/2);
}

int QuickMedian::findMedian(int l, int r, int order) {
	if (l == r) {
    return data[l];
  }
  int pivot = randomizedPartition(l, r);
  if (order == pivot) {
    return data[order];
  } else 
  if (order < pivot) {
    return findMedian(l, pivot - 1, order);
  } else { /* if (order > pivot) */
    return findMedian(pivot + 1, r, order);
  }
}

int QuickMedian::Partition(int l, int r) {
  int t = data[r], i = l, j = l;
  for ( ; i < r; ++i) {
    if (data[i] < t) {
      std::swap(data[i], data[j++]);
    }
  }
  std::swap(data[j], data[r]);
  return j;
}

int QuickMedian::randomizedPartition(int l, int r) {
  int pivot = std::rand() % (r - l + 1) + l;
  std::swap(data[pivot], data[r]);
  return Partition(l,r);
}

namespace FisherYatsShuffle {
  static void shuffle(std::vector<int> *input) {
    std::vector<int>::reverse_iterator it = input->rbegin();
    int idx = 0;
    for ( ; it != input->rend(); it++) {
      idx = std::rand() % (std::distance(it,input->rend()));
      std::iter_swap(it,input->begin() + idx);
    }
  }
}

int main(int argc, char *argv[]) {

  std::chrono::time_point<std::chrono::system_clock> start, end;
  std::vector<int> *data = NULL;

	switch (argc) {
		case 2:
    {
      std::istringstream ss(argv[1]);
      int length;
      if (!(ss >> length)) {
        std::cerr << "Bad argument!" << std::endl;
        exit(1);
      } else {
        std::cout << "Generating " << length << " samples." << std::endl;
      }
      start = std::chrono::system_clock::now();
      data = new std::vector<int>(length);
      for (std::vector<int>::iterator it = data->begin(); it != data->end(); ++it) {
        *it = it - data->begin();
      }
      FisherYatsShuffle::shuffle(data);
      end = std::chrono::system_clock::now();
      std::chrono::duration<double> elapsed_seconds = end - start; 
			std::cout << "Generating took " << elapsed_seconds.count() << " secs" << std::endl;
			break;
    }
		default:
      std::cerr << "Bad arguments count!" << std::endl;
			exit(2);
	};
  if (data == NULL) {
    std::cerr << "Wrong data. Terminating." << std::endl;
    exit(3);
  }

	start = std::chrono::system_clock::now();
  QuickMedian qm(*data);
  int median = qm.findMedian();
  end = std::chrono::system_clock::now();
  std::cout << "Median = " << median << std::endl;
  std::chrono::duration<double> elapsed_seconds = end - start; 
  std::cout << "Computation took " << elapsed_seconds.count() << " secs" << std::endl;

  delete data;
	return 0;
}
