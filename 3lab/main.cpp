#include <algorithm>
#include <iostream>
#include <numeric>
#include <random>
#include <set>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

struct Combination {
  int nums[4];
};

vector<Combination> generateAllCombinations() {
  vector<Combination> all;
  for (int a = 1; a <= 17; ++a) {
    for (int b = a + 1; b <= 18; ++b) {
      for (int c = b + 1; c <= 19; ++c) {
        for (int d = c + 1; d <= 20; ++d) {
          all.push_back({a, b, c, d});
        }
      }
    }
  }
  return all;
}

int countMatches(const int *user, const int *draw) {
  int matches = 0;
  for (int i = 0; i < 4; ++i) {
    for (int j = 0; j < 4; ++j) {
      if (user[i] == draw[j])
        matches++;
    }
  }
  return matches;
}

void get_input(int *field, int id) {
  while (true) {
    cout << "Поле " << id << " (4 числа 1-20): ";
    string line;
    getline(cin, line);
    stringstream ss(line);
    set<int> unique_nums;
    int val, count = 0;
    bool ok = true;

    while (ss >> val) {
      if (val < 1 || val > 20 || !unique_nums.insert(val).second) {
        ok = false;
        break;
      }
      if (count < 4)
        field[count] = val;
      count++;
    }

    if (ok && count == 4)
      break;
    cout << "Ошибка! Нужно 4 неповторяющихся числа от 1 до 20.\n";
  }
}

int main() {
  int userField1[4], userField2[4];

  get_input(userField1, 1);
  get_input(userField2, 2);

  auto pool = generateAllCombinations();
  size_t poolSize = pool.size();

  vector<int> idx1(poolSize), idx2(poolSize);
  iota(idx1.begin(), idx1.end(), 0);
  iota(idx2.begin(), idx2.end(), 0);

  random_device rd;
  mt19937 g(rd());
  shuffle(idx1.begin(), idx1.end(), g);
  shuffle(idx2.begin(), idx2.end(), g);

  unsigned long long drawCount = 0;
  unsigned long long halfWinDraw = 0;
  bool jackpotFound = false;

  for (size_t i = 0; i < poolSize && !jackpotFound; ++i) {
    for (size_t j = 0; j < poolSize; ++j) {
      drawCount++;

      int m1 = countMatches(userField1, pool[idx1[i]].nums);
      int m2 = countMatches(userField2, pool[idx2[j]].nums);

      if (halfWinDraw == 0 && (m1 + m2 >= 4)) {
        halfWinDraw = drawCount;
      }

      if (m1 == 4 && m2 == 4) {
        jackpotFound = true;
        break;
      }
    }
  }

  cout << "\nРезультат:\n";
  cout << "Суперприз: " << drawCount << '\n';
  if (halfWinDraw)
    cout << "4 совпадения: " << halfWinDraw << '\n';

  return 0;
}
