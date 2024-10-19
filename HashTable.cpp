#include <iostream>
using namespace std;

class HashTable
{
    int *arr;           // Array for the hash table
    bool *isOccupied;   // Boolean array to track if a slot is occupied
    int m;              // Size of the table
    double load_factor;
    int total;          // Total number of values inserted into the table

public: 
    HashTable(int capacity)
    {
        m = capacity;
        arr = new int[m];
        isOccupied = new bool[m];  // Initialize the boolean array
        load_factor = 0.7;         // Optimized load factor for resizing
        total = 0;
        for(int i = 0; i < m; i++)
        {
            arr[i] = 0;            
            isOccupied[i] = false; // no slots are occupied initially
        }
    }

private:
    int hash(int key)  // Hash function
    {
        return (key % m);
    }

    bool isprime(int n)  // Prime checker for ideal resizing
    {
        if (n <= 1)
            return false;
        if (n == 2)
            return true;
        if (n % 2 == 0)
            return false;
        for (int i = 3; i * i <= n; i += 2)
        {
            if (n % i == 0)
                return false;
        }
        return true;
    }

    void resize()
    {
        int n_size = 2 * m + 1;
        while(!isprime(n_size))  // Find the next prime size for the table
        {
            n_size += 2;
        }

        int *table_re = new int[n_size];
        bool *isOccupied_re = new bool[n_size]; // new boolean array for the resized table

        for(int i = 0; i < n_size; i++)
        {
            table_re[i] = 0;          
            isOccupied_re[i] = false; // marking all slots as unoccupied
        }

        // Rehashing old values into the new table
        for (int i = 0; i < m; i++) 
        {
            if (isOccupied[i])  // rehashing for occupied slots
            {
                int bucket = arr[i] % n_size;  // hashing values into the new table
                int j = 1;  // Quadratic probing trial count
                while (isOccupied_re[bucket]) 
                {
                    bucket = (bucket + j * j) % n_size;  // Probing the new table
                    j++;
                }
                table_re[bucket] = arr[i];
                isOccupied_re[bucket] = true;  // marking slot as occupied
            }
        }

        delete[] arr;
        delete[] isOccupied;
        arr = table_re;
        isOccupied = isOccupied_re;
        m = n_size;
    }

    bool isEmpty()
    {
        if(total == 0)
            return true;
        return false;
    }

public:
    void insert(int value)
    {
        if (total / (double)m > load_factor)
        {
            resize();  // Resize when load factor exceeds 0.7
        }

        if (search(value) != -1)
        {
            //cout << "Duplicate key found" << endl;
            return;
        }

        int seen = 0;  // Visited indices counter to avoid infinite loop
        int i = 1;
        int bucket = hash(value);
        int ind = bucket;

        if (!isOccupied[bucket])  // Slot is unoccupied
        {
            arr[bucket] = value;
            isOccupied[bucket] = true;
            total++;
        }
        else
        {
            // Use quadratic probing to find an empty slot
            while (isOccupied[ind] && seen < (m + 1) / 2)
            {
                seen++;
                ind = (bucket + i * i) % m;
                i++;
            }
            if (seen == m)
            {
                //cout << "All possible indices probed" << endl;
                return;
            }
            else
            {
                arr[ind] = value;
                isOccupied[ind] = true;
                total++;
            }
        }
    }

    void remove(int value)
    {
        if (isEmpty())
        {
            //cout << "Key does not exist" << endl;
            return;
        }

        int bucket = hash(value);
        int ind = bucket;
        int seen = 0;  // Visited indices counter to avoid infinite loop
        int i = 1;

        while (seen < (m + 1) / 2)
        {
            seen++;
            if (isOccupied[ind] && arr[ind] == value)  // Slot is occupied and value matches
            {
                isOccupied[ind] = false;  // Marking the slot unoccupied
                total--;
                return;
            }
            ind = (bucket + i * i) % m;
            i++;
        }

        //cout << "Key does not exist" << endl;
    }

    int search(int value)
    {
        if (isEmpty())
        {
            return -1;
        }

        int bucket = hash(value);
        int ind = bucket;
        int seen = 0;  // Number of indices seen
        int i = 1;

        while (seen < m)
        {
            seen++;
            if (isOccupied[ind] && arr[ind] == value)  // Slot is occupied and value matches
            {
                return ind;
            }
            ind = (bucket + i * i) % m;
            i++;
        }

        return -1;  // Value not found
    }

    void printTable()
    {
        for (int i = 0; i < m; i++)
        {
            if (isOccupied[i])
                cout << arr[i] << " "; 
            else
                cout << "- ";  
        }
        cout << endl;
    }
};