#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <random>
//#include "Deque.hpp"

#include <iostream>
#include <string.h>


// May assume memcpy()-able.
// May assume = operator.




//WHAT ARE WE GOING TO DO WITH LESS BY ID, IS IT FOR TESTING PURPOSES?

struct MyClass {
    int id;
    char name[10];
};

//required for the equal func if a isnt less than b, and b isnt less than a
bool
MyClass_less_by_id(const MyClass &o1, const MyClass &o2) {
    return o1.id < o2.id;
}

void
MyClass_print(const MyClass *o) {
    printf("%d\n", o->id);
    printf("%s\n", o->name);
}

//Deque_DEFINE(MyClass)

const int INIT_SIZE = 10;

struct Deque_MyClass;

struct Deque_MyClass_Iterator{
   Deque_MyClass *deque_my;
   int index;
   void (*inc)(Deque_MyClass_Iterator *);//inc_i
   void (*dec)(Deque_MyClass_Iterator *);//dec_i

   MyClass &(*deref)(Deque_MyClass_Iterator *);//deref_i

};

//TYPEDEF  point, comprasion func


struct Deque_MyClass{

   /*Variables for the deque*/
   MyClass *q_arr;
   int q_limit;
   int q_size;

   int back_index;
   int front_index;

   /*Is true if a fatal error happened, like memory allocation error.
   If this errorFlag is on, most operations will not work.*/
   bool errorFlag;
	
   /*The name of the type that is held in the datastructure*/
   char type_name[sizeof("Deque_MyClass")/sizeof(char)];

   /*Function Pointers*/

   int (*size)(Deque_MyClass *);
   bool (*empty)(Deque_MyClass *);

   MyClass &(*front)(Deque_MyClass *);
   MyClass &(*back)(Deque_MyClass *);


   Deque_MyClass_Iterator (*begin)(Deque_MyClass *);
   Deque_MyClass_Iterator (*end)(Deque_MyClass *);

   void (*push_back)(Deque_MyClass *, MyClass);
   void (*push_front)(Deque_MyClass *, MyClass);
   void (*pop_back)(Deque_MyClass *);
   void (*pop_front)(Deque_MyClass *);

   MyClass &(*at)(Deque_MyClass *, int);

   bool (*compare)(const MyClass &, const MyClass &);

   void (*dtor)(Deque_MyClass *);
   void (*clear)(Deque_MyClass *);
	
};
//FUNCTION SIGNATURES for Deque_MyClass_Iterator
void inc_i(Deque_MyClass_Iterator *);
void dec_i(Deque_MyClass_Iterator *);

MyClass &deref_i(Deque_MyClass_Iterator *);

//FUNCTION SIGNATURES for Deque_MyClass_
void Deque_MyClass_ctor(Deque_MyClass *object, bool (*comprasion)(const MyClass &, const MyClass &));

int getSize(Deque_MyClass *obj);
bool isEmpty(Deque_MyClass *obj);

MyClass & front_i(Deque_MyClass *);
MyClass & back_i(Deque_MyClass *);

bool allocateMeMoreSpace(Deque_MyClass *);

Deque_MyClass_Iterator begin_i(Deque_MyClass *);
Deque_MyClass_Iterator end_i(Deque_MyClass *);

void push_back_i(Deque_MyClass *, MyClass);
void push_front_i(Deque_MyClass *, MyClass);
void pop_back_i(Deque_MyClass *);
void pop_front_i(Deque_MyClass *);

MyClass &at_i(Deque_MyClass *, int);

void clear_i(Deque_MyClass *);
void dtor_i(Deque_MyClass *);

void Deque_MyClass_ctor(Deque_MyClass *object, bool (*comprasion_func)(const MyClass &, const MyClass &))
{
	//Operations related to the deque
	object->q_arr =  new MyClass[INIT_SIZE];

	if(INIT_SIZE > 0)
	   object->q_limit = INIT_SIZE;
	else
	   object->q_limit = 10;

	object->q_size = 0;

	object->errorFlag = false;

	if(object->q_arr == NULL)
	{
		std::cerr<<"MEMORY ERROR: Memory couldn't be allocated for the array";
		object->q_limit = 0;
		object->errorFlag = true;
	}
	object->back_index = 0;
	object->front_index = 0;
	
	int arrSize = sizeof("Deque_MyCla ss");

	memcpy(object->type_name,"Deque_MyClass",(arrSize-1)*sizeof(char));
	object->type_name[arrSize-1]='\0';

	//Initilazing function pointers
	object->size = &getSize;
	object->empty = &isEmpty;

	object->back = &back_i;
	object->front = &front_i;

	object->begin = &begin_i;
	object->end = &end_i;

	object->push_back = &push_back_i;
	object->push_front = &push_front_i;
	object->pop_back = &pop_back_i;
	object->pop_front = &pop_front_i;

        object->at = &at_i;


	object->compare = comprasion_func;

	object->clear = &clear_i;

        object->dtor = &dtor_i;
}


int getSize(Deque_MyClass *obj)
{
	return obj->q_size;
}

bool isEmpty(Deque_MyClass *obj)
{
	return !((bool)(obj->q_size));
}

MyClass &front_i(Deque_MyClass *object)
{
   return (object->q_arr[object->front_index]);
}

MyClass &back_i(Deque_MyClass *object)
{
   return (object->q_arr[object->back_index]);   
}


//Allocates space.Returns false.
// errorFlag isn't set because it is still possible to use the deque.
bool allocateMeMoreSpace(Deque_MyClass *object)
{
   if(object == NULL)
   {
      std::cerr<<"NULL REFERENCE: Memory couldn't be allocated for the array";
      return false;  
   }

   if((object->q_size != 0 && (object->q_limit == object->q_size + 1)) || (object->q_size == 0 && object->q_limit == 1))
   {

      MyClass *old_arr = object->q_arr;

      object->q_arr = new MyClass[object->q_limit*2];
      /*If memory is allocated*/
      if(object->q_arr != NULL)
      {
         int current = 0;

         /*Copy the values*/

         while(object->back_index != object->front_index){
            memcpy(&(object->q_arr[current]),&old_arr[object->back_index], sizeof(MyClass));
            current++;
            object->back_index = (object->back_index+1) % object->q_limit;
         }
        
	if(object->back_index == object->front_index)
           memcpy(&(object->q_arr[current]),&old_arr[object->back_index], sizeof(MyClass));

	if(object->q_size != 0)
	{
           delete []old_arr;
	   object->front_index=object->q_size-1;
	}

        object->q_limit*=2;
	object->back_index=0;
      }
      else
      {
         object->q_arr = old_arr;
         std::cerr<<"MEMORY ERROR: Memory couldn't be allocated for the array";
         return false;    
      }
   }
   return true;
}

/*PUSH BACK*/
void push_back_i(Deque_MyClass *object, MyClass insert){

   if(object->errorFlag == false)
   {
      if(allocateMeMoreSpace(object))
      {
        int current_back = object->back_index;
		
		if(object->q_size != 0)
		   current_back -= 1;

         if(current_back < 0)
            current_back += object->q_limit;

         /*Insert the element*/
         memcpy(&(object->q_arr[current_back]),&insert, sizeof(MyClass));

         object->q_size++;
         object->back_index = current_back;
      }
      else
      {
         std::cerr<<"Value cannot be inserted because of the memory problem";
      }
   }
   else
   {
      std::cerr<<"Value cannot be inserted because of the memory problem";
      return;
   }
}

/*PUSH FRONT*/
void push_front_i(Deque_MyClass *object, MyClass append){
   if(object->errorFlag == false)
   {
       if(!allocateMeMoreSpace(object))
       {
          std::cerr<<"Value cannot be inserted because of the memory problem";
          return;
       }	
		if(object->q_size != 0)
		   object->front_index = ((object->front_index+1)%object->q_limit);
		
		/*Insert the element*/
		memcpy(&(object->q_arr[object->front_index]),&append, sizeof(MyClass));
		object->q_size++; 
    }
   else
   {
      std::cerr<<"Value cannot be inserted because of the memory problem";
   }
}


void pop_back_i(Deque_MyClass *object){

   if(object->q_size==0)
	  return;

   if(object->q_size>1)
   {
	   object->back_index = ((object->back_index+1)%object->q_limit);      
   }
   object->q_size--;
}

void pop_front_i(Deque_MyClass *object){

   if(object->q_size==0)
	  return;

   if(object->q_size>1)
   {
      object->front_index--;

      if(object->front_index < 0)
         object->front_index += object->q_limit;
   }
   
   object->q_size--;
}

MyClass &at_i(Deque_MyClass *object, int position)
{
//   return object->q_arr[(object->back_index + position)%object->q_limit];


int cur = object->front_index - position;
if(cur<0)
   cur += object->q_limit;
return object->q_arr[cur];
}

void clear_i(Deque_MyClass *object)
{
   object->q_size = 0;
   object->front_index = 0;
   object->back_index = 0;   
}

void dtor_i(Deque_MyClass *object)
{
   if(object->q_arr != NULL)
      delete []object->q_arr;
}








Deque_MyClass_Iterator begin_i(Deque_MyClass *object)
{
   return Deque_MyClass_Iterator{object, 0, &inc_i, &dec_i, &deref_i};
}

Deque_MyClass_Iterator end_i(Deque_MyClass *object)
{
   return Deque_MyClass_Iterator{object, object->q_size, &inc_i, &dec_i, &deref_i};
}





void inc_i(Deque_MyClass_Iterator *object)
{
   object->index++;
}

void dec_i(Deque_MyClass_Iterator *object)
{
   object->index--;
}

MyClass &deref_i(Deque_MyClass_Iterator *object)
{
   return object->deque_my->at(object->deque_my,object->index);
}

bool Deque_MyClass_Iterator_equal(Deque_MyClass_Iterator &object1,Deque_MyClass_Iterator object2)
{
   if(&object1.deque_my->at(object1.deque_my,object1.index) == &object2.deque_my->at(object2.deque_my,object2.index))
      return true;
   return false; 
}

bool Deque_MyClass_equal(Deque_MyClass &object1, Deque_MyClass &object2)
{
   if(object1.q_size != object2.q_size)
      return false;

   if(object1.compare != object2.compare)
      return false;

   int index = 0;
   for( ; index < object2.q_size ; index++)
   {
      MyClass o1 = object1.at(&object1,index);
      MyClass o2 = object2.at(&object2,index); 
      if(object1.compare(o1, o2) || object2.compare(o2, o1))
         return false;   
   }
   return true;
}

int
main() {
        Deque_MyClass deq;
        Deque_MyClass_ctor(&deq, MyClass_less_by_id);

        assert(deq.size(&deq) == 0);
        assert(deq.empty(&deq));

        // Should print "---- Deque_MyClass, 14".
        printf("---- %s, %d\n", deq.type_name, (int) sizeof(deq.type_name));
        // std::cout << "---- " << deq.type_name << ", " << sizeof(deq.type_name) << std::endl;
        assert(sizeof deq.type_name == 14);

        deq.push_back(&deq, MyClass{1, "Joe"});
        deq.push_back(&deq, MyClass{2, "Mary"});
        deq.push_back(&deq, MyClass{3, "Tom"});
        deq.push_front(&deq, MyClass{0, "Mike"});
        deq.push_front(&deq, MyClass{-1, "Mary"});

        MyClass_print(&deq.front(&deq));
        MyClass_print(&deq.back(&deq));
        assert(deq.front(&deq).id == -1);
        assert(deq.back(&deq).id == 3);

        deq.pop_front(&deq);
        deq.pop_back(&deq);
        assert(deq.front(&deq).id == 0);
        assert(deq.back(&deq).id == 2);

        assert(deq.size(&deq) == 3);

        for (Deque_MyClass_Iterator it = deq.begin(&deq);
         !Deque_MyClass_Iterator_equal(it, deq.end(&deq)); it.inc(&it)) {
            MyClass_print(&it.deref(&it));
        }

        // Multiple iterators?
        for (Deque_MyClass_Iterator it1 = deq.begin(&deq);
         !Deque_MyClass_Iterator_equal(it1, deq.end(&deq)); it1.inc(&it1)) {
            MyClass_print(&it1.deref(&it1));
            for (Deque_MyClass_Iterator it2 = deq.begin(&deq);
             !Deque_MyClass_Iterator_equal(it2, deq.end(&deq)); it2.inc(&it2)) {
                MyClass_print(&it2.deref(&it2));
                for (Deque_MyClass_Iterator it3 = deq.begin(&deq);
                 !Deque_MyClass_Iterator_equal(it3, deq.end(&deq)); it3.inc(&it3)) {
                    MyClass_print(&it3.deref(&it3));
                }
            }
        }

        // Test decrement.
        {
            auto it = deq.end(&deq);
            it.dec(&it);
            assert(it.deref(&it).id == 2);
        }

        // printf("Using at.\n");

        for (size_t i = 0; i < 3; i++) {
            MyClass_print(&deq.at(&deq, i));
        }

        deq.clear(&deq);

        deq.dtor(&deq);

        // Test equality.  It is undefined behavior if the two deques were constructed with different
        // comparison functions.
        {
            Deque_MyClass deq1, deq2;
            Deque_MyClass_ctor(&deq1, MyClass_less_by_id);
            Deque_MyClass_ctor(&deq2, MyClass_less_by_id);

            deq1.push_back(&deq1, MyClass{1, "Joe"});
            deq1.push_back(&deq1, MyClass{2, "Jane"});
            deq1.push_back(&deq1, MyClass{3, "Mary"});
            deq2.push_back(&deq2, MyClass{1, "Joe"});
            deq2.push_back(&deq2, MyClass{2, "Jane"});
            deq2.push_back(&deq2, MyClass{3, "Mary2"});

            assert(Deque_MyClass_equal(deq1, deq2));
            deq1.pop_back(&deq1);
            assert(!Deque_MyClass_equal(deq1, deq2));
            deq1.push_back(&deq1, MyClass{4, "Mary"});
            assert(!Deque_MyClass_equal(deq1, deq2));

            deq1.dtor(&deq1);
            deq2.dtor(&deq2);
        }

   return 0;
}

