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
#include <string> 
#include <experimental/algorithm>
#include <random>

#include "knapsack.hpp"

default_random_engine Random::eng(static_cast<long unsigned int>(0));

size_t hash_item::operator()(const Item* item) const{
    return hash<int>()(item->getId());
}

size_t hash_element::operator()(const Element* elem) const{
    return hash<int>()(elem->getId());
}

Element::Element(int id, int p) : id(id), weight(p) { }

int Element::getWeight() {   return weight; }

bool Element::is_contained(Item* I){
    return items.find(I)!=items.end();
}

Item::Item(int id_item, int profit) : id(id_item),
        profit(profit), element_weight(0) { }

int Item::getProfit() { return profit; }
int Item::getId() const {  return id; }
int Element::getId() const {  return id; }
int Item::getElementWeight() {  return element_weight; }
float Item::getRatio() {
    float a = (float)profit / (float)element_weight;
     return a;
     };

bool Item::contains(Element* h)
{
    return elements.find(h)!=elements.end();
}

void Item::add_element(Element* el){
    element_weight+=el->getWeight();
    elements.insert(el);

}

Knapsack::Knapsack(int c, list<Item>& items): capacity(c), total_weight(0), total_profit(0)
{
    for(Item& item:items){
        unselected_items[&item] = item.getElementWeight();
        mapItems.insert(make_pair(item.getId(),&item)); // Save the references of all items in the knapsack
    }
        
    
}

int Knapsack::getCapacity()
{
    return capacity;
}

int Knapsack::getTotalWeight()
{
    return total_weight;
}

int Knapsack::getTotalProfit()
{
    return total_profit;
}


bool Knapsack::addElement(Element* el){
    if(!contains_element(el)){
        //if(el->getWeight() > capacity- total_weight) return false;
        elements.insert(make_pair(el,0));
        total_weight+=el->getWeight();

        //se actualiza beneficio de la mochila
        //y pesos restantes de los items no seleccionados
        for(auto it:el->items){
            Item* item =(Item*) it;
            unselected_items[it]-=el->getWeight();
            if (unselected_items[it]==0){
               total_profit += item->getProfit();  
               unselected_items.erase(it);
               
               items.insert(it);
               for(Element* elem : item->elements) elements[(Element*) elem]++;
            }
        }
        return true;
    }
    return false;
}

void Knapsack::removeElement(Element* el, bool remove_itemless){
    if(contains_element(el)){
        total_weight-=el->getWeight();

        for(Item* it:el->items){
                Item* item=(Item*) it;
                if(unselected_items.find(item)==unselected_items.end())
                {
                    unselected_items[item] = el->getWeight();
                    total_profit -= item->getProfit();
                    
                    //elimina el item
                    items.erase(item);
                    
                    //elimina elementos que quedan sin item
                    for(Element* ele : item->elements){
                        Element* elem = (Element*) ele;
                        int n = elements[el]--;
                        if (n==0 && remove_itemless) elements.erase(el);
                    }

                }else unselected_items[item] += el->getWeight(); 
        }
        elements.erase(el);
    }
}

bool Knapsack::addItem(Item* item)
{
        
    if(contains_item(item)) return false;
    if(unselected_items[item] > capacity-total_weight) return false;
    
    //se agregan los elementos que no están en la mochila
    for(Element* el:item->elements){
        Element* elem = (Element*) el;
        if(!contains_element(elem)){
            addElement(elem);
        }
    }

    return true;
}


bool Knapsack::contains_item(Item* i){
    return (items.find(i)!=items.end());
}

bool Knapsack::contains_element(Element* h){
    return (elements.find(h)!=elements.end());
}

bool Knapsack::removeItem(Item* item, bool remove_itemless, bool unique_elements)
{
    if(!contains_item(item)) return false;
    
    for(Element* elem: item->elements){
        Element* element = elem;
        if(contains_element(element)){
            if(!unique_elements || elements[element]==1){ 
                removeElement(element, remove_itemless);
            }
        }
    }

    return true;
}

void Knapsack::removeElements( list<Element*>& elements, bool remove_itemless){
    
    for(Element* element:elements)
        removeElement(element, remove_itemless);
}

void Knapsack::addElements( list<Element*>& elements){
    
    for(Element* element:elements)
        addElement(element);
}

//n corresponde a la cantidad de items que selecciona como candidatos
//en cada iteracion. Si n es grande se comporta como un greedy,
//si es pequeño (n=1), como un algoritmo que escoge aleatoriamente items.
void Knapsack::simple_greedy(list< Move >& moves, int n){
    if(n==-1) n=999999;
    
    while(true){
        //items that can be placed in the knapsack
        unordered_set <Item*, hash_item > candidate_items;
        for(auto item: unselected_items){
            if(item.second <= getCapacity() - getTotalWeight())
                candidate_items.insert(item.first);
        }

        if(candidate_items.size()==0) break;
        int d=0;

        list <Item*> cand;
        std::experimental::sample(candidate_items.begin(), candidate_items.end(), std::back_inserter(cand),
                n, Random::eng);

        Item* selected_item=NULL;
        double ratio_max=0.0;

        for(Item* it: cand){
            Item* item= (Item*)it;
            double ratio= (double)item->getProfit() / (double)  unselected_items[item];
            if(ratio>ratio_max){
                ratio_max=ratio;
                selected_item=item;
            }
        }

        
        addItem(selected_item);
        moves.push_back(Move(selected_item, Move::ADD));
    }
}

void Knapsack::apply_moves(list< Move >& moves, list< Element* >& removals, list< Element* >& additions, bool fill, bool remove_only_unique_elements, int n_greedy)
{
    std::unordered_map<Element *, int, hash_element> elements0 = elements;
    
    for(auto m: moves)
    {
        if ( m.type == Move::ADD )
            addItem(m.item);
        

        if ( m.type == Move::REMOVE )
            removeItem(m.item, true, remove_only_unique_elements); //remove itemless elements
        
    }

    if(fill)
        simple_greedy(moves, n_greedy);
    

    for (auto el : elements0)
        if(elements.find(el.first)==elements.end()) removals.push_back((Element*)el.first);

    for (auto el : elements)
        if(elements0.find(el.first)==elements0.end()) additions.push_back((Element*) el.first);

}
