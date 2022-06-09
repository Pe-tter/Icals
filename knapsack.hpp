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
//#include<time.h>
#include<math.h>
#include<unordered_set>
#include<unordered_map>
#include <string> 
#include <experimental/algorithm>
#include <random>

#ifndef KNAPSACK
#define KNAPSACK

using namespace std;


class Item;
class Element;
class Knapsack;

class Random{
    public:
    static default_random_engine eng;
    static void seed(int s){
        eng = default_random_engine({static_cast<long unsigned int>(s)});
    }

};

class hash_item {
public:
    size_t operator()(const Item* p) const;
};

class hash_element {
public:
    size_t operator()(const Element* p) const;
};

class Element { // clase para cada elemento
    private:
        int weight; // variable que guardará el peso del elemento
        int id;
        
      public:
        Element(int id, int p);
        int getWeight();
        int getId() const;
        bool is_contained(Item* I); //return true if the elemet is in I
        std::unordered_set <Item*, hash_item> items; //items que contienen el elemento
};



class Item {
    private:
        int profit;
        int element_weight =0;
        int id;

    public:      
        Item(int id_item, int profit);
        int getProfit();
        int getElementWeight();
        int getId() const;
        float getRatio();
        void add_element(Element* el);
        bool contains(Element* h);
        
        std::unordered_set<Element*, hash_element> elements;
};

class Move{
    public:
        enum {ADD, REMOVE};
        int type;
        Item* item;
        Move(Item* item, int t) : type(t), item(item) {}
};


//usar unsorted_set 
// multimap para los no seleccionados
class Knapsack{
    private:
        int capacity;
        int total_weight =0;
        int total_profit=0;

    public:
        Knapsack(int c, list<Item>& items);
        int getCapacity();
        int getTotalWeight();
        int getTotalProfit();
        bool addItem(Item* i);
        //Si unique_elements=true, solo elimina elementos que estan en ESTE item
        bool removeItem(Item* i, bool remove_itemless=true, bool unique_elements=false);
        bool addElement(Element* e);

        //Elimina el elemnto de la mochila
        //Si remove_itemless=true, elimina también elementos sin item
        void removeElement(Element* e, bool remove_itemless=true);

        void addElements( list<Element*>& elements);
        void removeElements( list<Element*>& elements, bool remove_itemless=true);


        // realiza los movimientos indicados
        // si fill=true rellena aleatoriamente
        // la lista moves se actualiza incorporando los movimientos de rellenado
        void apply_moves(list< Move >& moves, list< Element* >& removals, list< Element* >& additions, bool fill=true, bool remove_only_unique_elements=false, int n_greedy=20);

        void simple_greedy(list< Move >& moves, int n=-1);
        

        bool contains_item(Item* i);
        bool contains_element(Element* h);

        std::unordered_set<Item*, hash_item> items;
        std::unordered_map<Element*, int, hash_element> elements; //elements -> n_items

        // para cada item, guarda el peso faltante para completar dicho item
        unordered_map<Item*, int, hash_item> unselected_items; 
        map<int,Item*> mapItems;
        
        
};

#endif


