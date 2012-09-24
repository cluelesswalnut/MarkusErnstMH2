#include "cinder/gl/gl.h"
#include "cinder/app/AppBasic.h"
#include "node.h"
#include "cinder/Rand.h"

using namespace ci;
using namespace ci::app;
using namespace std;

node::node(){
	next = NULL;
	prev = NULL;

	//rest
	type = NULL;
	x = NULL;
	y = NULL;
	w = NULL;
	h = NULL;
	c = Color(255,0,0);
	r = NULL;
	x3 = NULL;
	y3 = NULL;
};

node::node(uint8_t* ar, int typ, int xx,int yy, int ww, int hh, Color8u cc){
	next = NULL;
	prev = NULL;
	type = typ;
	x = xx;
	y = yy;
	w = ww;
	h = hh;
	c = cc;

	//rest
	r = NULL;
	x3 = NULL;
	y3 = NULL;
};

node::node(uint8_t* ar, int typ, int xx,int yy, double rr, Color8u cc){
	next = NULL;
	prev = NULL;
	type = typ;
	x = xx;
	y = yy;
	r = rr;
	c = cc;

	//rest
	w = NULL;
	h = NULL;
	x3 = NULL;
	y3 = NULL;
};

node::node(uint8_t* ar, int typ, int xx,int yy, int xx2, int yy2, int xx3, int yy3, Color8u cc){
	next = NULL;
	prev = NULL;
	type = typ;
	x = xx;
	y = yy;
	w = xx2;
	h = yy2;
	x3 = xx3;
	y3 = yy3;
	c = cc;

	//rest
	r = NULL;
};

void insertAfter(node* insert, node* after, node* head){
	if(insert != NULL && after != NULL && head != NULL){
		if(insert == head->next){
		head->next = after;
		};

		insert->next = after->next;

		insert->prev = after;

		if(after->next != NULL)
			after->next->prev = insert;

		after->next = insert;

	};
};

void remove(node* out){
	if(out != NULL){
	if(out->next == NULL){
		out->prev->next = NULL;
	}
	else{
		out->prev->next = out->next;
		out->next->prev = out->prev;
	}
	};
};

void move(node* move, int x, int y){

		if(move->type == 0){
			move->x = move->x + x;
			move->y = move->y - y;
		};
		if(move->type == 1){
			move->x = move->x + x;
			move->y = move->y - y;
		};
		if(move->type == 2){
			move->x = move->x + x;
			move->y = move->y - y;
			move->w = move->w + x;
			move->h = move->h - y;
		};
		if(move->type == 3){
			move->x = move->x + x;
			move->y = move->y - y;
			move->w = move->w + x;
			move->h = move->h - y;
			move->x3 = move->x3 + x;
			move->y3 = move->y3 - y;
		};
};

void reverse(node* head){
	node* temp;
	node* hold;
	temp = head->next;

	temp->prev = temp->next;
	temp->next = NULL;

	temp = temp->prev;

	while(temp->next != NULL){
		hold = temp->prev;
		temp->prev = temp->next;
		temp->next = hold;
		temp = temp->prev;
	};
	temp->next = temp->prev;
	temp->prev = head;
	head->next = temp;
};