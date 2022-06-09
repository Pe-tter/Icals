#include<iostream>
#include<stdlib.h>
#include<string.h>
#include<fstream>
#include<cstdlib>
#include<list>
#include<fstream>
#include<iterator>
#include<map>
#include<set>
#include<queue>
#include<vector>
#include<math.h>
#include<unordered_set>
#include <string> 
#include <experimental/algorithm>
#include <algorithm>
#include <queue>
#include <utility>
#include <random>
#include "knapsack.hpp"
#include <iomanip>

using namespace std;



int loadData(list<Element>& ,list<Item>&,int); // Function for loading data set
float updateMatrixAffinity(float** iAffinity,Knapsack& init_kn); // Function that will update the values of the mtrix 
float itemCompare(Item* a, Item* b,unordered_map<Element*, int, hash_element> kn_elements); // Function to compare the affinity of items a and b
Item* cluster_generation(float compatibility_factor,Knapsack& init_kn,float** iAffinity); // Function that will create clusters
float cluster_ratio(Knapsack& init_kn, set<Item*> clusterItems); // Function that will calculate the ratio of the cluster
float cluster_ratio(Knapsack& init_kn, pair<Item*,Item*> pairItems);
float cluster_ratio(Knapsack& init_kn, Item* item);

int main(int argc, char *argv[])
{
    unsigned t0, t1;
    t0=clock();

    // Received parameters
    int seed = atoi(argv[1]);
    int instance= atoi(argv[2]);
    int max_iterations= atoi(argv[3]);
    float compatibility_factor= (float) atoi(argv[4]) / 100;
    // Planted seed
    srand(seed);
    Random::seed(seed);
    
    // Lists for storing items and elements
    list<Item> total_items;
    list<Element> total_elements;

    // Initial knapsack
    Knapsack init_kn( (loadData(total_elements,total_items,instance)), total_items );

    // Best knapsack
    Knapsack best_kn = init_kn;

    // Affinity matrix
    float** iAffinity;

    // Matrix initialization
    iAffinity = new float* [total_items.size()];
    for (int i = 0; i < total_items.size(); i++) {
        iAffinity[i] = new float[total_items.size()];
    }
    for (int i = 0; i < total_items.size(); i++) {
        for (int j = 0; j < total_items.size(); j++) {
            iAffinity[i][j] = 0;
        }
    }
    
    // Prints to check
    cout << endl;
    cout << "***********************Checking****************************" << endl;
    cout << "items in list: " << total_items.size() << endl;
    cout << "Elements in list: " << total_elements.size() << endl;
    cout << "unselect_items in knapsack: " << init_kn.unselected_items.size() << endl;
    cout << "items in knapsack map: " << init_kn.mapItems.size() << endl;
    

    // Strategy Iteration
    int iteration_counter = 0,count=0;
    Item* idAddItem;
    float high_affinity=0;
    int itercount = 0;
    while (iteration_counter < max_iterations)
    {
        /*cout << endl;
        cout << "***********************Iteration number " << (iteration_counter + 1) << "******************" << endl;*/
        // Initial solution creation
        do
        {
            // Matrix update
            high_affinity = updateMatrixAffinity(iAffinity,init_kn);
            
            //if( compatibility_factor > high_affinity ) compatibility_factor = high_affinity;
            
            // Clustering
            idAddItem = cluster_generation(compatibility_factor,init_kn,iAffinity);
         
             // Print current status of the knapsack
            cout << endl;
            cout << "***********************Checking**********************" << endl;
            cout << "Actual profit: " << init_kn.getTotalProfit() << " number of items: " << init_kn.items.size() << endl;
            cout << "Selected item: " << idAddItem->getId() << endl;
            itercount++;
            //exit(1);
            //if(itercount == 3) break;
        } while (init_kn.addItem(idAddItem));
        

        // Local Search

        // Update the best knapsack found
        if(best_kn.getTotalProfit() < init_kn.getTotalProfit())
            best_kn = init_kn;

        // Perturbation of the current knapsack

        // Update iteration counter
        iteration_counter++;

        
    }
   
    
   //cout << best_kn.getTotalProfit() << ";" << endl;
    cout << endl;
    cout << "***********************Results*****************************" << endl;
    for(auto item: init_kn.items)
        cout << "item: " << item->getId() << " with profit: " << item->getProfit() <<endl;
    cout << endl;

    cout << "Final profit of the best knapsack: " << best_kn.getTotalProfit() << endl;
    cout << "(Actual weight / Capacity) of the knapsack: " << init_kn.getTotalWeight() << " / " << init_kn.getCapacity() << endl;
    t1 = clock();
    double time = (double(t1-t0)/CLOCKS_PER_SEC);
    cout << instance << ";" << best_kn.getTotalProfit() << ";" << time << ";" << endl;
}

// Functions

int loadData(list<Element>& total_elements, list<Item>& total_items,int instance) {
    ifstream file; // Variable that will receive the data from the
    string text; // Variable to be used to read the file variable
    int i=0,j=0,a=0,m,n,capac,number, cont = 0; // Reading variables
    
    switch (instance)
    {
    // m = n
    case 1:
        file.open("../Three kinds of SUKP instances (EAs)/SUKP Instances (m equal n)/sukp 100_100_0.10_0.75.txt",ios::in); // se abre el archivo en modo lectura
        break;
    case 2:
        file.open("../Three kinds of SUKP instances (EAs)/SUKP Instances (m equal n)/sukp 100_100_0.15_0.85.txt",ios::in); // se abre el archivo en modo lectura
        break;
    case 3:
        file.open("../Three kinds of SUKP instances (EAs)/SUKP Instances (m equal n)/sukp 200_200_0.10_0.75.txt",ios::in); // se abre el archivo en modo lectura
        break;
    case 4:
        file.open("../Three kinds of SUKP instances (EAs)/SUKP Instances (m equal n)/sukp 200_200_0.15_0.85.txt",ios::in); // se abre el archivo en modo lectura
        break;
    case 5:
        file.open("../Three kinds of SUKP instances (EAs)/SUKP Instances (m equal n)/sukp 300_300_0.10_0.75.txt",ios::in); // se abre el archivo en modo lectura
        break;
    case 6:
        file.open("../Three kinds of SUKP instances (EAs)/SUKP Instances (m equal n)/sukp 300_300_0.15_0.85.txt",ios::in); // se abre el archivo en modo lectura
        break;
    case 7:
        file.open("../Three kinds of SUKP instances (EAs)/SUKP Instances (m equal n)/sukp 400_400_0.10_0.75.txt",ios::in); // se abre el archivo en modo lectura
        break;
    case 8:
        file.open("../Three kinds of SUKP instances (EAs)/SUKP Instances (m equal n)/sukp 400_400_0.15_0.85.txt",ios::in); // se abre el archivo en modo lectura
        break;
    case 9:
        file.open("../Three kinds of SUKP instances (EAs)/SUKP Instances (m equal n)/sukp 500_500_0.10_0.75.txt",ios::in); // se abre el archivo en modo lectura
        break;
    case 10:
        file.open("../Three kinds of SUKP instances (EAs)/SUKP Instances (m equal n)/sukp 500_500_0.15_0.85.txt",ios::in); // se abre el archivo en modo lectura
        break;
    // m > n
    case 11:
        file.open("../Three kinds of SUKP instances (EAs)/SUKP Instances (m greater than n)/sukp 100_85_0.10_0.75.txt",ios::in); // se abre el archivo en modo lectura
        break;
    case 12:
        file.open("../Three kinds of SUKP instances (EAs)/SUKP Instances (m greater than n)/sukp 100_85_0.15_0.85.txt",ios::in); // se abre el archivo en modo lectura
        break;
    case 13:
        file.open("../Three kinds of SUKP instances (EAs)/SUKP Instances (m greater than n)/sukp 200_185_0.10_0.75.txt",ios::in); // se abre el archivo en modo lectura
        break;
    case 14:
        file.open("../Three kinds of SUKP instances (EAs)/SUKP Instances (m greater than n)/sukp 200_185_0.15_0.85.txt",ios::in); // se abre el archivo en modo lectura
        break;
    case 15:
        file.open("../Three kinds of SUKP instances (EAs)/SUKP Instances (m greater than n)/sukp 300_285_0.10_0.75.txt",ios::in); // se abre el archivo en modo lectura
        break;
    case 16:
        file.open("../Three kinds of SUKP instances (EAs)/SUKP Instances (m greater than n)/sukp 300_285_0.15_0.85.txt",ios::in); // se abre el archivo en modo lectura
        break;
    case 17:
        file.open("../Three kinds of SUKP instances (EAs)/SUKP Instances (m greater than n)/sukp 400_385_0.10_0.75.txt",ios::in); // se abre el archivo en modo lectura
        break;
    case 18:
        file.open("../Three kinds of SUKP instances (EAs)/SUKP Instances (m greater than n)/sukp 400_385_0.15_0.85.txt",ios::in); // se abre el archivo en modo lectura
        break;
    case 19:
        file.open("../Three kinds of SUKP instances (EAs)/SUKP Instances (m greater than n)/sukp 500_485_0.10_0.75.txt",ios::in); // se abre el archivo en modo lectura
        break;
    case 20:
        file.open("../Three kinds of SUKP instances (EAs)/SUKP Instances (m greater than n)/sukp 500_485_0.15_0.85.txt",ios::in); // se abre el archivo en modo lectura
        break;
        // m < n
    case 21:
        file.open("../Three kinds of SUKP instances (EAs)/SUKP Instances (m less than n)/sukp 85_100_0.10_0.75.txt",ios::in); // se abre el archivo en modo lectura
        break;
    case 22:
        file.open("../Three kinds of SUKP instances (EAs)/SUKP Instances (m less than n)/sukp 85_100_0.15_0.85.txt",ios::in); // se abre el archivo en modo lectura
        break;
    case 23:
        file.open("../Three kinds of SUKP instances (EAs)/SUKP Instances (m less than n)/sukp 185_200_0.10_0.75.txt",ios::in); // se abre el archivo en modo lectura
        break;
    case 24:
        file.open("../Three kinds of SUKP instances (EAs)/SUKP Instances (m less than n)/sukp 185_200_0.15_0.85.txt",ios::in); // se abre el archivo en modo lectura
        break;
    case 25:
        file.open("../Three kinds of SUKP instances (EAs)/SUKP Instances (m less than n)/sukp 285_300_0.10_0.75.txt",ios::in); // se abre el archivo en modo lectura
        break;
    case 26:
        file.open("../Three kinds of SUKP instances (EAs)/SUKP Instances (m less than n)/sukp 285_300_0.15_0.85.txt",ios::in); // se abre el archivo en modo lectura
        break;
    case 27:
        file.open("../Three kinds of SUKP instances (EAs)/SUKP Instances (m less than n)/sukp 385_400_0.10_0.75.txt",ios::in); // se abre el archivo en modo lectura
        break;
    case 28:
        file.open("../Three kinds of SUKP instances (EAs)/SUKP Instances (m less than n)/sukp 385_400_0.15_0.85.txt",ios::in); // se abre el archivo en modo lectura
        break;
    case 29:
        file.open("../Three kinds of SUKP instances (EAs)/SUKP Instances (m less than n)/sukp 485_500_0.10_0.75.txt",ios::in); // se abre el archivo en modo lectura
        break;
    case 30:
        file.open("../Three kinds of SUKP instances (EAs)/SUKP Instances (m less than n)/sukp 485_500_0.15_0.85.txt",ios::in); // se abre el archivo en modo lectura
        break;
        //set II
    case 31:
        file.open("../Three kinds of SUKP instances (EAs)/SUKP(greaterInstances)/sukp_585_600_0.10_0.75.txt",ios::in); // se abre el archivo en modo lectura
        break;
    case 32:
        file.open("../Three kinds of SUKP instances (EAs)/SUKP(greaterInstances)/sukp_585_600_0.15_0.85.txt",ios::in); // se abre el archivo en modo lectura
        break;
    case 33:
        file.open("../Three kinds of SUKP instances (EAs)/SUKP(greaterInstances)/sukp_600_585_0.10_0.75.txt",ios::in); // se abre el archivo en modo lectura
        break;
    case 34:
        file.open("../Three kinds of SUKP instances (EAs)/SUKP(greaterInstances)/sukp_600_585_0.15_0.85.txt",ios::in); // se abre el archivo en modo lectura
        break;
    case 35:
        file.open("../Three kinds of SUKP instances (EAs)/SUKP(greaterInstances)/sukp_600_600_0.10_0.75.txt",ios::in); // se abre el archivo en modo lectura
        break;
    case 36:
        file.open("../Three kinds of SUKP instances (EAs)/SUKP(greaterInstances)/sukp_600_600_0.15_0.85.txt",ios::in); // se abre el archivo en modo lectura
        break;
    case 37:
        file.open("../Three kinds of SUKP instances (EAs)/SUKP(greaterInstances)/sukp_685_700_0.10_0.75.txt",ios::in); // se abre el archivo en modo lectura
        break;
    case 38:
        file.open("../Three kinds of SUKP instances (EAs)/SUKP(greaterInstances)/sukp_685_700_0.15_0.85.txt",ios::in); // se abre el archivo en modo lectura
        break;
    case 39:
        file.open("../Three kinds of SUKP instances (EAs)/SUKP(greaterInstances)/sukp_700_685_0.10_0.75.txt",ios::in); // se abre el archivo en modo lectura
        break;
    case 40:
        file.open("../Three kinds of SUKP instances (EAs)/SUKP(greaterInstances)/sukp_700_685_0.15_0.85.txt",ios::in); // se abre el archivo en modo lectura
        break;
    case 41:
        file.open("../Three kinds of SUKP instances (EAs)/SUKP(greaterInstances)/sukp_700_700_0.10_0.75.txt",ios::in); // se abre el archivo en modo lectura
        break;
    case 42:
        file.open("../Three kinds of SUKP instances (EAs)/SUKP(greaterInstances)/sukp_700_700_0.15_0.85.txt",ios::in); // se abre el archivo en modo lectura
        break;
    case 43:
        file.open("../Three kinds of SUKP instances (EAs)/SUKP(greaterInstances)/sukp_785_800_0.10_0.75.txt",ios::in); // se abre el archivo en modo lectura
        break;
    case 44:
        file.open("../Three kinds of SUKP instances (EAs)/SUKP(greaterInstances)/sukp_785_800_0.15_0.85.txt",ios::in); // se abre el archivo en modo lectura
        break;
    case 45:
        file.open("../Three kinds of SUKP instances (EAs)/SUKP(greaterInstances)/sukp_800_785_0.10_0.75.txt",ios::in); // se abre el archivo en modo lectura
        break;
    case 46:
        file.open("../Three kinds of SUKP instances (EAs)/SUKP(greaterInstances)/sukp_800_785_0.15_0.85.txt",ios::in); // se abre el archivo en modo lectura
        break;
    case 47:
        file.open("../Three kinds of SUKP instances (EAs)/SUKP(greaterInstances)/sukp_800_800_0.10_0.75.txt",ios::in); // se abre el archivo en modo lectura
        break;
    case 48:
        file.open("../Three kinds of SUKP instances (EAs)/SUKP(greaterInstances)/sukp_800_800_0.15_0.85.txt",ios::in); // se abre el archivo en modo lectura
        break;
    case 49:
        file.open("../Three kinds of SUKP instances (EAs)/SUKP(greaterInstances)/sukp_885_900_0.10_0.75.txt",ios::in); // se abre el archivo en modo lectura
        break;
    case 50:
        file.open("../Three kinds of SUKP instances (EAs)/SUKP(greaterInstances)/sukp_885_900_0.15_0.85.txt",ios::in); // se abre el archivo en modo lectura
        break;
    // m > n
    case 51:
        file.open("../Three kinds of SUKP instances (EAs)/SUKP(greaterInstances)/sukp_900_885_0.10_0.75.txt",ios::in); // se abre el archivo en modo lectura
        break;
    case 52:
        file.open("../Three kinds of SUKP instances (EAs)/SUKP(greaterInstances)/sukp_900_885_0.15_0.85.txt",ios::in); // se abre el archivo en modo lectura
        break;
    case 53:
        file.open("../Three kinds of SUKP instances (EAs)/SUKP(greaterInstances)/sukp_900_900_0.10_0.75.txt",ios::in); // se abre el archivo en modo lectura
        break;
    case 54:
        file.open("../Three kinds of SUKP instances (EAs)/SUKP(greaterInstances)/sukp_900_900_0.15_0.85.txt",ios::in); // se abre el archivo en modo lectura
        break;
    case 55:
        file.open("../Three kinds of SUKP instances (EAs)/SUKP(greaterInstances)/sukp_985_1000_0.10_0.75.txt",ios::in); // se abre el archivo en modo lectura
        break;
    case 56:
        file.open("../Three kinds of SUKP instances (EAs)/SUKP(greaterInstances)/sukp_985_1000_0.15_0.85.txt",ios::in); // se abre el archivo en modo lectura
        break;
    case 57:
        file.open("../Three kinds of SUKP instances (EAs)/SUKP(greaterInstances)/sukp_1000_985_0.10_0.75.txt",ios::in); // se abre el archivo en modo lectura
        break;
    case 58:
        file.open("../Three kinds of SUKP instances (EAs)/SUKP(greaterInstances)/sukp_1000_985_0.15_0.85.txt",ios::in); // se abre el archivo en modo lectura
        break;
    case 59:
        file.open("../Three kinds of SUKP instances (EAs)/SUKP(greaterInstances)/sukp_1000_1000_0.10_0.75.txt",ios::in); // se abre el archivo en modo lectura
        break;
    case 60:
        file.open("../Three kinds of SUKP instances (EAs)/SUKP(greaterInstances)/sukp_1000_1000_0.15_0.85.txt",ios::in); // se abre el archivo en modo lectura
        break;
        
    default:
        break;
    }
    
    
    if(file.fail()){ // In case the file cannot be opened a text line will be displayed on the screen
        cout<<"No se pudo abrir el archivo";
        exit(1);
    }
    file >> number;
    m = number; // The number of items is obtained
  
    file >> number;
    n= number; // The number of elements is obtained
  
    file >> number;
    capac= number; // The capacity of theknapsack is obtained
  
    while(file >> number){
            cont++;
            Item item(cont, number);

            total_items.push_back(item);
            a++;
            if(a == m){
                break;
            }
    }
    a=0;
    int id=1;
    while(file >> number){
        Element el(id, number);
        total_elements.push_back(el);
            cont++;
            a++;
            id++;
            if(a == n){
                break;
            }
    }
    
    int relations[m][n];// Matrix that will store the relationships of the elements with their respective items
    i=0;
    j=0;
            while (file >> number)
            {
                if(j == n){
                    i++;
                    j=0;
                }
                relations[i][j]= number;
                j++;
            }
      

    
    double ratio;
    list<Element>::iterator el;
    el = total_elements.begin();
    list<Item>::iterator it;
    it = total_items.begin();
    for ( i = 0; i < m; i++)
    {
        for ( j = 0; j < n; j++)
        {
            if (relations[i][j] != 0)
            {
                Element* aux = &(*el);
                Item* auxI = &(*it);
                it->add_element(aux);
                el->items.insert(auxI);

            }
            el++;
        }
        it++;
        el =total_elements.begin();
    }
    
    file.close(); // File is closed
    return capac;
}

float updateMatrixAffinity(float** iAffinity,Knapsack& init_kn)
{
    int m = init_kn.mapItems.size(); // Total number of items
    //cout << "m: " << m << endl;
    set<float> factors;
    int i=0,j=0; // Indexes to move within the matrix
    float max_affinity=0; // Highest affinity percentage

    for (int i = 0; i < m; i++)
    {
        Item* a = init_kn.mapItems.find(i+1)->second;
        for (int j = 0; j < m; j++)
        {
            Item* b = init_kn.mapItems.find(j+1)->second;
            iAffinity[i][j] = round(100 * itemCompare(a,b,init_kn.elements)) / 100; // Comparison between items
            //if( (max_affinity < iAffinity[i][j]) ) max_affinity = iAffinity[i][j];
            if( (max_affinity < iAffinity[i][j]) && (i!=j) ) max_affinity = iAffinity[i][j];
            factors.insert(iAffinity[i][j]);
        }
    }
    cout << "Highest affinity parameter: " << max_affinity << endl;
    //for(auto factor: factors)
        //cout  << factor << endl;
    //cout << fixed << setprecision(2) << max_affinity << ";" << endl;
    return max_affinity;
}

float itemCompare(Item* a, Item* b,unordered_map<Element*, int, hash_element> kn_elements)
{
    float affinity = 0.0; // Affinity between "a" and "b"
    int intersectionAB = 0; // Intersection between "a" and "b"
    int elementsA = 0; // Elements in "a" that are not in the knapsack
    int elementsB = 0; // Elements in "b" that are not in the knapsack
    int unionAB = 0; // Union between "a" and "b"

    // Elements inside the knapsack
    unordered_set<Element*> elem_kn; 

    // Elements inside the knapsack are stored in elem_kn
    for(auto element: a->elements) // Elements in a
    {
        if (kn_elements.find(element) != kn_elements.end()) // If element is in a knapsack it is stored
            elem_kn.insert(element);
        
        if (kn_elements.find(element) == kn_elements.end()) // If the element is not in the knapsack, we will proceed to
        {
            if (b->contains(element)) // Check if the element is in b
                intersectionAB++; // The intersection counter is increased by one
            elementsA++; // The elements counter is increased by one
        }
    }
    for(auto element: b->elements) // Elements in b
    {
        if (kn_elements.find(element) != kn_elements.end()) // If element is in a knapsack it is stored
            elem_kn.insert(element);

        if (kn_elements.find(element) == kn_elements.end()) // If the element is not in the knapsack, we will proceed to
            elementsB++; // The elements counter is increased by one
    }

    // Calculations:
    unionAB = elementsA + elementsB - intersectionAB; // The union of elements of a and b is calculated
    affinity = ( (float)elementsA + (float)elementsB ) / (float)unionAB; // The affinity between a and b is calculated
    affinity--;

    return affinity;
}

Item* cluster_generation(float compatibility_factor,Knapsack& init_kn,float** iAffinity)
{
    Item* best_item; // Best item found
    // Compatibility factor between items
    float best_ratio = 0.0; // Variable for comparing ratios
    pair<int,float> best_item_ratio = make_pair(0,0); // Variable for comparing ratios
    int m = init_kn.mapItems.size(); // Number of items
    set<Item*> best_combination; // Best cluster found
    set<Item*> compatible_items;
    //set<list<Item*>> combinations;
    float ibest_ratio=0;
    Item* ibest_id;
    //compatibility_factor = 0.04;
    int compatibility_flag = 0,no_items=0,count;   
    cout << "Compatibility factor: " << compatibility_factor << endl;
    for (int i = 0; i < m; ++i)
    {
        if(init_kn.contains_item((init_kn.mapItems.find(i+1))->second)) continue;
        if( cluster_ratio(init_kn,init_kn.mapItems.find(i+1)->second) > best_item_ratio.second) 
        {
            best_item_ratio.first = i;
            best_item_ratio.second = cluster_ratio(init_kn,init_kn.mapItems.find(i+1)->second);
        }
        ibest_ratio  = 0;
        // Initialize variables
        compatible_items.clear(); // Current cluster
        count = 1;
        //cout << "Compatibility factor: " << compatibility_factor << endl;
        //cout << i+1 << ";; " ;
        for (int j = 0; j < m; ++j)
        {
            
            if ((iAffinity[i][j] == compatibility_factor))
                {
                    count++;
                    //compatible_items.insert((init_kn.mapItems.find(i+1))->second);
                    if(!init_kn.contains_item((init_kn.mapItems.find(j+1))->second)) compatible_items.insert((init_kn.mapItems.find(j+1))->second); // The item is added to the list
                    compatibility_flag++;
                }
        }
        
            // Print to check cluster status
            cout << endl;
            cout << "Item: " << i+1 << endl; 
            cout << "Number of cluster items: " << compatible_items.size() << endl;
            
//if(!init_kn.contains_item((init_kn.mapItems.find(j+1))->second)) 
            // Select the 2 best item in cluster
            int size_i = compatible_items.size();
            while (size_i > 2)
            {
                ibest_ratio = 0;
                for(auto item : compatible_items)
                {
                    if (cluster_ratio(init_kn,item) < ibest_ratio){ ibest_ratio = cluster_ratio(init_kn,item); ibest_id = item;}
                    if(ibest_ratio == 0){ ibest_ratio = cluster_ratio(init_kn,item); ibest_id = item;}
                }
                //cout << "Item with low ratio: " << ibest_id->getId() << " with ratio: " << ibest_ratio << endl;    
                compatible_items.erase(ibest_id);
                size_i--;
                //exit(1);
            }
            if(compatible_items.size() > 0)
            {
                compatibility_flag = 1;
                compatible_items.insert(init_kn.mapItems.find(i+1)->second);
                
                cout << "Ratio of cluster: " << cluster_ratio(init_kn,compatible_items) << endl;
                for(auto item : compatible_items)
                {
                    cout << "item " << item->getId() << " with profit: " << item->getProfit() << " and ratio: " << cluster_ratio(init_kn,item) << endl;
                    for(auto element: item->elements)
                    {
                        if(!init_kn.contains_element(element))
                            cout << "         element: " << element->getId() << " with weight: " << element->getWeight() << endl;
                    }
                         
                }
                    
                cout << endl;

                // Check if the ratio of the current cluster exceeds the best ratio found
                if(best_ratio < cluster_ratio(init_kn,compatible_items))
                {
                    best_combination = compatible_items; // Save the combination
                    best_ratio = cluster_ratio(init_kn,compatible_items); // Update the best ratio
                }
            }
    }
    //cout << "Best cluster found with profit: " << cluster_ratio(init_kn,best_combination) <<  endl;
            
    // Select cluster item
    //exit(1);
    if(compatibility_flag == 0) 
    {
        return init_kn.mapItems.find(best_item_ratio.first+1)->second;
    }
    if (best_combination.size() == 1) return *best_combination.begin();
    
    set<pair<Item*,Item*>> aa;
    pair<Item*,Item*> pairAux;

    //for(auto item: best_combination)
        //cout << "Item: " << item->getId() << " with ratio: " << item->getRatio() << " and best ratio found: " << best_ratio << endl;

    for(auto item: best_combination)
    {
        for(auto itemC: best_combination)
        {
            //if ( (aa.find(make_pair(item,itemC)) == aa.end()) && (aa.find(make_pair(itemC,item)) == aa.end() ) ) 
            if ( (item != itemC) && (aa.find(make_pair(item,itemC)) == aa.end()) && (aa.find(make_pair(itemC,item)) == aa.end() ) )  
                aa.insert(make_pair(item,itemC));
        }
    }
    best_ratio=0;
    //cout << "seccion de pares de items: " << endl;
        for(auto itemC: aa)
        {
            //cout << "item 1: " << itemC.first->getId() << " item 2: " << itemC.second->getId() << " with ratio: " << cluster_ratio(init_kn,itemC) << endl;
            if (best_ratio < cluster_ratio(init_kn,itemC))
            {
                best_ratio = cluster_ratio(init_kn,itemC);
                pairAux = itemC;
            }
            
        }
            
    // Notes: 
    //      Choosing the best items in pair cluster
    best_ratio=0;
    if (cluster_ratio(init_kn,pairAux.first) > cluster_ratio(init_kn,pairAux.second))
        best_item = pairAux.first; // Update the item
    else
        best_item = pairAux.second; // Update the item
    
    // choosing a random item
    //int position_item = rand()%(2) + 1;
    //cout << "postion: " << position_item << endl;
    //if (position_item == 1)
        //best_item = pairAux.first; // Update the item
    //else
        //best_item = pairAux.second; // Update the item


    //cout << "Item selected to add: " << best_item->getId() << endl;
    //      Choosing one of best items in cluster 
    /*int position_item = rand()%(best_combination.size()) + 1;
    count = 1;
    //cout << "Position selected: " << position_item << endl;
    for(auto item: best_combination)
    {
        //cout << "Item: " << item->getId() << " with ratio: " << item->getRatio() << endl;
        // Check if the ratio of the current item exceeds the best ratio found
        if(count == position_item)
        {
            best_item = item; // Save the item
            best_ratio = best_item->getRatio(); // Update the best ratio
        }
        count++;
    }*/

    //cout << endl;
    //cout << "Number of combinations: " << combinations.size() << endl;
    //cout << "Best cluster ratio: " << best_ratio << endl;
    //cout << "Selected item: " << best_item->getId() << "with ratio: " << best_item->getRatio() << endl;
    //cout << "Function cluster is completed" << endl;
    return best_item; 
}

float cluster_ratio(Knapsack& init_kn, set<Item*> clusterItems)
{
    int profit = 0;
    int weight = 0;
    float ratio = 0;

    
    for(auto item: clusterItems)
    {
        // Profit calculation
        profit += item->getProfit();

        // Weight calculation
        for(auto elem: item->elements)
        {
            // If the item is in a backpack, it is not counted
           if(init_kn.elements.find(elem) == init_kn.elements.end())
            {
                weight += elem->getWeight();
            } 
        }
        
        
    }

    // Ratio calculation 
    ratio = (float) profit / (float) weight;
    

    return ratio;
}
float cluster_ratio(Knapsack& init_kn, pair<Item*,Item*> pairItems)
{
    int profit = 0;
    int weight = 0;
    float ratio = 0;

    if(pairItems.first->getId() == pairItems.second->getId())
    {
        return pairItems.first->getRatio();
    }

    // Profit calculation
    profit += pairItems.first->getProfit() + pairItems.second->getProfit();

    // Weight calculation
    for(auto elem: pairItems.first->elements)
    {
        // If the item is in a backpack, it is not counted
        if(init_kn.elements.find(elem) == init_kn.elements.end())
        {
            weight += elem->getWeight();
        } 
    }
    for(auto elem: pairItems.second->elements)
    {
        // If the item is in a backpack, it is not counted
        if(init_kn.elements.find(elem) == init_kn.elements.end())
        {
            weight += elem->getWeight();
        } 
    }
    // Ratio calculation 
    ratio = (float) profit / (float) weight;
    

    return ratio;
}

float cluster_ratio(Knapsack& init_kn, Item* item)
{
    int weight = 0;
    float ratio = 0;


    // Weight calculation
    for(auto elem: item->elements)
    {
        // If the item is in a backpack, it is not counted
        if(init_kn.elements.find(elem) == init_kn.elements.end())
        {
            weight += elem->getWeight();
        } 
    }
    
    // Ratio calculation 
    ratio = (float) item->getProfit() / (float) weight;
    

    return ratio;
}