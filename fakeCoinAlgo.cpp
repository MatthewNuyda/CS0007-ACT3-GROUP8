#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>

using namespace std;

class ErrorManagement{
    public:
        void numGatekeep(int& num);
};

class Utility{
    public:
        void viewArr(vector<int> arr);
};

class Weigh : public Utility{
private:
    vector<int> coinPouch;

    void viewPartitions(vector<int> p1, vector<int> p2, vector<int> p3);
    void calculatePartitionWeight(vector<int> p1, vector<int> p2, int& w1, int& w2);
    
public:
    Weigh(const vector<int>& coins){
        this->coinPouch = coins;
    }

    int findFakeCoin(vector<int>& arr, int& iter, int& index);
};

class CoinPouch : public Utility{
private:
    int generate(int min, int max);

public:
    CoinPouch(){
        srand(static_cast<unsigned int>(time(0)));
    }

    void addCoinToArr(vector<int>& arr, int n);
    void displayAnswer(int w, int iter, int index);
};

int Weigh::findFakeCoin(vector<int>& arr, int& iter, int& index){
    iter++;
    int n = arr.size();
    //handles if the array is empty
    if (n == 0){
        cout << "Array is empty";
        return -1;
    }
    //base case to return the last coin found in the algorithm
    if (n == 1)
        return arr[0];

    //handle the array if it's size is even
    if (n%2 == 0) {
        //split the original array into 2 halves
        int half = n/2;
        vector<int> part1(arr.begin(), arr.begin() + half);
        vector<int> part2(arr.begin() + half, arr.end());

        //display the array of the current iterations and its partitions
        cout << "============================================================\n";
        cout << "Iteration "<< iter <<" || Current Array: \n";
        viewArr(arr);
        viewPartitions(part1, part2, {});

        //get the weight of the each partition
        int part1Weight = 0, part2Weight = 0;
        calculatePartitionWeight(part1, part2, part1Weight, part2Weight);

        /*weighing logic: 
            if one side of the array weighs less than the other, feed it to the recursive call
            if the weight of the 2 sides are equal, then return 15 which would indicate that there are no fake coins in the array 
        */

        if (part1Weight < part2Weight) {
            return findFakeCoin(part1, iter, index);
        } 
        else if (part2Weight < part1Weight) {
            index += 2; 
            return findFakeCoin(part2, iter, index);
        }
        else
            return 15;
    }

    //handles the array if n is odd

    //split size of the array into 3 partitions where the first 2 partitions have equal sizes
    int third = n/3;
    int remainder = n%3;
    int midPart1 = third + (remainder > 0 ? 1:0);
    int midPart2 = midPart1 + third + (remainder > 0 ? 1:0);

    vector<int> part1(arr.begin(), arr.begin() + midPart1);
    vector<int> part2(arr.begin() + midPart1, arr.begin() + midPart2);
    vector<int> part3(arr.begin() + midPart2, arr.end());

    //display the array of the current iteration and its partitions
    cout << "============================================================\n";
    cout << "Iteration "<< iter <<" || Current Array: \n";
    viewArr(arr);
    viewPartitions(part1, part2, part3);

    //get the weights of the 1st and 2nd partitions
    int part1Weight = 0, part2Weight = 0;
    calculatePartitionWeight(part1, part2, part1Weight, part2Weight);

    /*weighing logic: 
        if one side of the array weighs less than the other, feed it to the recursive call
        if the weight of the 2 sides are equal, then the fake coin must be in the 3rd partition, feed it to the recursive call
    */

    if (part1Weight < part2Weight){
        cout << "Is the fake coin in the first group?\n\n";
        return findFakeCoin(part1, iter, index);
    }
    else if (part2Weight < part1Weight){
        cout << "Is the fake coin in the second group?\n\n";
        index+=midPart1;
        return findFakeCoin(part2, iter, index);
    }
    else{
        cout << "Is the fake coin in the third group?\n\n";
        index+=midPart2;
        return findFakeCoin(part3, iter, index);
    }
}

void Utility::viewArr(vector<int> arr){
    //go through the array and print each of its contents
    int n = arr.size();
    cout << "{";
    for(int i = 0; i < n; i++){
        cout << arr[i];
        if (i != n-1)
            cout << ", ";
    }
    cout << "} \n\n";
};

void Weigh::viewPartitions(vector<int> p1, vector<int> p2, vector<int> p3){
    cout << "Group 1: ";
    viewArr(p1);

    cout << "Group 2: ";
    viewArr(p2);

    cout << "Group 3: ";
    viewArr(p3);
}

void Weigh::calculatePartitionWeight(vector<int> p1, vector<int> p2, int& w1, int& w2){
    //get weight by adding the n and n+1 of the array
    for (int i = 0; i < p1.size(); i++){
        w1 += p1[i];
    }
    for (int i = 0; i < p2.size(); i++){
        w2 += p2[i];
    }
}

int CoinPouch::generate(int min, int max){
    //generate random number withe the rand functuion
    return rand() % (max - min + 1) + min;
};

void CoinPouch::addCoinToArr(vector<int>& arr, int n){
    /*there is a 50% chance thet there are no fake coins in the array
        this is generating a random number from 0-1
        if it returns 1 then the if statement would allow the for loop to add a fake coin in the array with a random index
    */
    int decide = generate(0, 1);
    int fakeCoinIndex = generate(0, n-1);
    int original = generate(10, 20);

    for(int i = 0; i < n; i++){
        if(i == fakeCoinIndex && decide == 1)
            arr[i] = generate(1, 9);
        else
            arr[i] = original;
    }
}

void CoinPouch::displayAnswer(int w, int iter, int index){
    if(w >= 10){
        cout << "NO! There are no fake coins in this coin pouch";
        return;
    }

    cout << "YES!, The program terminated at iteration " << iter 
         << " and has found a fake coin at the index: " << index << endl
         <<"\nThe fake coin has a weight of: " << w ;
}

void ErrorManagement::numGatekeep(int& num){
	bool loop = true;

	//If there is an invalid input, clear the flag and ask the user to input a new value
	while (loop) {
        cin >> num;

        if (cin.fail()) {
            cin.clear(); 
			cin.ignore();
            cout << "Invalid input.\n Please enter a valid number: ";
        } else {
            loop = false;
        }
    }
}

int main() {
    ErrorManagement err;
    int n = 0, iter = 0, index = 0;
    cout << "Number of coins in the coin pouch: ";
    err.numGatekeep(n);

    vector<int> coins(n);
    CoinPouch pouch;
    
    //add random numbers to the coins array
    pouch.addCoinToArr(coins, n);
    
    //display the weights added to the array
    cout << "Coin Weights: ";
    pouch.viewArr(coins);
    
    //process the array to show what the weight of the fake coin is
    Weigh findFake(coins);
    int fakeWeight = findFake.findFakeCoin(coins, iter, index);

    pouch.displayAnswer(fakeWeight, iter, index);
    
    return 0;
}