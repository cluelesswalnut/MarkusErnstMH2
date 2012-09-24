#include "cinder/app/AppBasic.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/Texture.h"
#include "cinder/ImageIo.h"
#include "boost/date_time/posix_time/posix_time.hpp"
#include "node.h"
#include "Resources.h"
#include <cstdlib> 
#include <iostream>
#include <math.h>
#include <cinder\app\App.h>
#include <cinder\Text.h>
#include <cinder\Font.h>

using namespace ci;
using namespace ci::app;
using namespace std;

class MarkusErnstHW2App : public AppBasic {
  public:
	void setup();
	void mouseDown( MouseEvent event );	
	void update();
	void draw();
	/**set the three parts of a pixel using just the x and y value of and the color
	*/
	void setPixel(uint8_t* ar, int x,int y, Color8u c);
	/**
	*draw a rectangle givent he top left corner and its width and hight
	*/
	void drawRect(uint8_t* ar, int x,int y, int w, int h, Color8u c);
	/**
	*draw a circle given the center and the randius and the color
	*/
	void drawCirc(uint8_t* ar, int x,int y, double r, Color8u c);
	/**
	*draw a line given the two end points
	*/
	void drawLine(uint8_t* ar, int x, int y, int x2, int y2, Color8u c);
	/**
	*draw a triangle given three points
	*/
	void drawTri(uint8_t* ar, int x, int y, int x2, int y2, int x3, int y3, Color8u c);
	/**
	*implement any kernel, in this program a blur is inplemented. All the values in the kernel can be changed though.
	*/
	void blur(uint8_t* ar, float a,  float b,  float c,  float d,  float e,  float f,  float g,  float h,  float i);

	void keyDown(KeyEvent event);

  private:
	Surface* mySurface_;
	Surface tex;
	uint8_t* pixelArray;
	node* lst;
	node* temp;
	node* background;
	node* rec1;
	node* rec2;
	node* circ1;
	node* circ2;
	node* tri;
	char shape;
	TextLayout* text;
	Font font;
	int help;
};

void MarkusErnstHW2App::setPixel(uint8_t* ar, int x,int y, Color8u c)
{
	//set all three of the colors individually using the cordinates of the pixel
	ar[y * 1024 * 3 + 3 * x] = c.r;
	ar[y * 1024 * 3 + 3 * x + 1] = c.g;
	ar[y * 1024 * 3 + 3 * x + 2] = c.b;

};

void MarkusErnstHW2App::drawRect(uint8_t* ar, int x,int y, int w, int h, Color8u c)
{
	//loop through all of the pixels in the rectangle and change their color
	for(int i = x; i < x + w; i++){
		for(int j = y; j < y + h; j++){
			setPixel(ar, i, j, c);
		};
	};

};

void MarkusErnstHW2App::drawCirc(uint8_t* ar, int x,int y, double r, Color8u c)
{
	//travers around the outside of the circle using the sin and cos finctions
	/*for(double i = 0; i < 360; i = i + .01){
		setPixel(ar, cos(i) * r + x, sin(i) * r + y, c);
	};*/

	//creat a filled circle
	for(int i = y-r; i<=y+r; i++){
			for(int j = x-r; j <= x+r; j++){
				int distance = (int)sqrt((double)((j - x)*(j-x) + (i - y)*(i - y)));
				if(distance <= r){
					setPixel(ar,j,i,c);
					
				};
			};
		};

};

void MarkusErnstHW2App::drawLine(uint8_t* ar, int x, int y, int x2, int y2, Color8u c){
	//handle if the slope is infinite
	if(x == x2){
		for(int i = y; i<=y2;i = i + 1){
			setPixel(ar,x,i,c);
		};
	}
	else{
	//determine the slope of the line
	int dx = (x2 - x);
	int dy = (y2 - y);
	float m = (1.0 * dy) / (1.0 * dx);
	int yv;
	int temp;
	//if it has a slope between 1 and -1 then use this first part of the algorithm
	if(m <= 1.0 && m >= -1.0){
		//make sure the line is oriented correctly
		if(x > x2){
			temp = x;
			x = x2;
			x2 = temp;
			temp = y;
			y = y2;
			y2 = temp;
		};
		//go throught all of the x values and find the y value of that pixel
		for (int i = x; i < x2; i++){
			yv = m * i - m * x + y + 0.5;
			setPixel(ar, i, yv, c);
		};
	};
	//the same as above, but using going through the y values instead of x. This i because the slope is not between 1 and -1.
	if(!(m <= 1.0 && m >= -1.0)){
		if(y > y2){
			temp = y;
			y = y2;
			y2 = temp;
			temp = x;
			x = x2;
			x2 = temp;
		};
		for (int i = y; i < y2; i++){
			yv = m * i - m * x + y + 0.5;
			yv = (i - .5 - y + m * x) / m;
			setPixel(ar, yv, i, c);
		};
	};
	};
};

void MarkusErnstHW2App::drawTri(uint8_t* ar, int x, int y, int x2, int y2, int x3, int y3, Color8u c){
	//to draw a triangle one must just draw three lines connecting the points of the triangle
	drawLine(ar, x, y, x2, y2, c);
	drawLine(ar, x, y, x3, y3, c);
	drawLine(ar, x3, y3, x2, y2, c);
	
};

void MarkusErnstHW2App::blur(uint8_t* ar , float a,  float b,  float c,  float d,  float e,  float f,  float g,  float h,  float i){
	//creat a array to hold the kernel
	float kernel [9] = {a,b,c,d,e,f,g,h,i};
	//travers the pixels colors and apply the kernel
	for(int i = 0; i < 640; i++){
		for(int j = 0; j < 480; j++){
			ar[j * 1024 * 3 + 3 * i] = ar[(j - 1) * 1024 * 3 + 3 * (i - 1)] * kernel[0] + ar[(j - 1) * 1024 * 3 + 3 * (i)] * kernel[1] + ar[(j - 1) * 1024 * 3 + 3 * (i + 1)] * kernel[2] + ar[(j) * 1024 * 3 + 3 * (i - 1)] * kernel[3] + ar[(j) * 1024 * 3 + 3 * (i)] * kernel[4] + ar[(j) * 1024 * 3 + 3 * (i + 1)] * kernel[5] + ar[(j + 1) * 1024 * 3 + 3 * (i - 1)] * kernel[6] + ar[(j + 1) * 1024 * 3 + 3 * (i)] * kernel[7] + ar[(j + 1) * 1024 * 3 + 3 * (i + 1)] * kernel[8];
			ar[j * 1024 * 3 + 3 * i + 1] = ar[(j - 1) * 1024 * 3 + 3 * (i - 1) + 1] * kernel[0] + ar[(j - 1) * 1024 * 3 + 3 * (i) + 1] * kernel[1] + ar[(j - 1) * 1024 * 3 + 3 * (i + 1) + 1] * kernel[2] + ar[(j) * 1024 * 3 + 3 * (i - 1) + 1] * kernel[3] + ar[(j) * 1024 * 3 + 3 * (i) + 1] * kernel[4] + ar[(j) * 1024 * 3 + 3 * (i + 1) + 1] * kernel[5] + ar[(j + 1) * 1024 * 3 + 3 * (i - 1) + 1] * kernel[6] + ar[(j + 1) * 1024 * 3 + 3 * (i) + 1] * kernel[7] + ar[(j + 1) * 1024 * 3 + 3 * (i + 1) + 1] * kernel[8];
			ar[j * 1024 * 3 + 3 * i + 2] = ar[(j - 1) * 1024 * 3 + 3 * (i - 1) + 2] * kernel[0] + ar[(j - 1) * 1024 * 3 + 3 * (i) + 2] * kernel[1] + ar[(j - 1) * 1024 * 3 + 3 * (i + 1) + 2] * kernel[2] + ar[(j) * 1024 * 3 + 3 * (i - 1) + 2] * kernel[3] + ar[(j) * 1024 * 3 + 3 * (i) + 2] * kernel[4] + ar[(j) * 1024 * 3 + 3 * (i + 1) + 2] * kernel[5] + ar[(j + 1) * 1024 * 3 + 3 * (i - 1) + 2] * kernel[6] + ar[(j + 1) * 1024 * 3 + 3 * (i) + 2] * kernel[7] + ar[(j + 1) * 1024 * 3 + 3 * (i + 1) + 2] * kernel[8];
		};
	};
};

void MarkusErnstHW2App::keyDown( KeyEvent event ) {
	//if the key pressed is one of the shapes then set shape to that shape's key
	if(event.getChar() == 'q' || event.getChar() == 'w'|| event.getChar() == 'e'|| event.getChar() == 'r'|| event.getChar() == 't'|| event.getChar() == 'y'){
	shape = event.getChar();
	};
	// toggle help for ? key
	if(event.getCode() == 47){
		if(help == 1)
			help = 0;
		else
			help = 1;
	};
	// call reverse list method for key f
	if(event.getChar() == 'f'){
		reverse(lst);
	};

	// for key a move the coresponding shape up in the list
	if( event.getChar() == 'a' ) {
		if(shape == 'q'){
			if(background->next != NULL){
				remove(background);
				insertAfter(background, background->next, lst);
			};
		};
		if(shape == 'w'){
			if(rec1->next != NULL){
				remove(rec1);
				insertAfter(rec1, rec1->next, lst);
			};
		};
		if(shape == 'e'){
			if(rec2->next != NULL){
				remove(rec2);
				insertAfter(rec2, rec2->next, lst);
			};
		};
		if(shape == 't'){
			if(circ1->next != NULL){
				remove(circ1);
				insertAfter(circ1, circ1->next, lst);
			};
		};
		if(shape == 'r'){
			if(circ2->next != NULL){
				remove(circ2);
				insertAfter(circ2, circ2->next, lst);
			};
		};
		if(shape == 'y'){
			if(tri->next != NULL){
				remove(tri);
				insertAfter(tri, tri->next, lst);
			};
		};
    };

	//for key z move the coresponding shape back in the list
	if( event.getCode() == 'z' ) {
		if(shape == 'q'){
			if(background->prev->prev != NULL){
				remove(background);
				insertAfter(background, background->prev->prev, lst);
			};
		};
		if(shape == 'w'){
			if(rec1->prev->prev != NULL){
				remove(rec1);
				insertAfter(rec1, rec1->prev->prev, lst);
			};
		};
		if(shape == 'e'){
			if(rec2->prev->prev != NULL){
				remove(rec2);
				insertAfter(rec2, rec2->prev->prev, lst);
			};
		};
		if(shape == 't'){
			if(circ1->prev->prev != NULL){
				remove(circ1);
				insertAfter(circ1, circ1->prev->prev, lst);
			};
		};
		if(shape == 'r'){
			if(circ2->prev->prev != NULL){
				remove(circ2);
				insertAfter(circ2, circ2->prev->prev, lst);
			};
		};
		if(shape == 'y'){
			if(tri->prev->prev != NULL){
				remove(tri);
				insertAfter(tri, tri->prev->prev, lst);
			};
		};
    };
	// for the arrow keys move the selected shape in the right derection
	if(event.getCode() == 273){
		if(shape == 'q'){
		};
		if(shape == 'w'){
			move(rec1, 0, 10);
		};
		if(shape == 'e'){
			move(rec2, 0, 10);
		};
		if(shape == 'r'){
			move(circ2, 0, 10);
		};
		if(shape == 't'){
			move(circ1, 0, 10);
		};
		if(shape == 'y'){
			move(tri, 0, 10);
		};
	};
	if(event.getCode() == 274){
		if(shape == 'q'){
		};
		if(shape == 'w'){
			move(rec1, 0, -10);
		};
		if(shape == 'e'){
			move(rec2, 0, -10);
		};
		if(shape == 'r'){
			move(circ2, 0, -10);
		};
		if(shape == 't'){
			move(circ1, 0, -10);
		};
		if(shape == 'y'){
			move(tri, 0, -10);
		};
	};
	if(event.getCode() == 275){
		if(shape == 'q'){
		};
		if(shape == 'w'){
			move(rec1, 10, 0);
		};
		if(shape == 'e'){
			move(rec2, 10, 0);
		};
		if(shape == 'r'){
			move(circ2, 10, 0);
		};
		if(shape == 't'){
			move(circ1, 10, 0);
		};
		if(shape == 'y'){
			move(tri, 10, 0);
		};
	};
	if(event.getCode() == 276){
		if(shape == 'q'){
		};
		if(shape == 'w'){
			move(rec1, -10, 0);
		};
		if(shape == 'e'){
			move(rec2, -10, 0);
		};
		if(shape == 'r'){
			move(circ2, -10, 0);
		};
		if(shape == 't'){
			move(circ1, -10, 0);
		};
		if(shape == 'y'){
			move(tri, -10, 0);
		};
	};
};

void MarkusErnstHW2App::setup()
{
	//create surface
	mySurface_ = new Surface(1024,1024,false);
	// set variable to toggle help
	help = 1;
	//set font of help
	font = Font( "", 20.0 );
	//set initial selected shape
	shape = 'q';
	// create text for the help
	text = new TextLayout();
	//set font of the text
	text->setFont(font);
	//set color of the text
	text->setColor( ColorA( 1.0f, 1.0f, 1.0f, 1.0f ) );
	//add what the text should say
	text->addLine("You can select the different shapes by pressing the keys:");
	text->addLine("	q: Background");
	text->addLine("	w: Green Rectangle");
	text->addLine("	e: Blue Rectangle");
	text->addLine("	r: Yellow Circle");
	text->addLine("	t: Red Circle");
	text->addLine("	y: Triangle");
	text->addLine(" ");
	text->addLine("After you have selected a shape:");
	text->addLine("	a: Moves the shape up in the list");
	text->addLine("	z: Moves the shape back in the list");
	text->addLine("	Arrow Keys: Move the shape around");
	text->addLine("Other:");
	text->addLine("	?: Toggle help display on/off");
	text->addLine("	f: reverses list");
	// make a surface out of the text
	tex = text->render(true,true);

	//creat head of shape list
	lst = new node();
	//temp variable to hold the spot in the list
	temp = lst;

	//add the shapes and point to them with extra variables so the correct shape can be moved by the user
	insertAfter(new node(pixelArray,0,0,0,640,480,Color8u(0, 0, 255)),temp,lst);

	background = lst->next;

	temp = temp->next;
	insertAfter(new node(pixelArray,0,100,100,100,100,Color8u(0, 255, 0)),temp,lst);
	temp = temp->next;
	rec1 = temp;
	insertAfter(new node(pixelArray,1,320,240,200,Color8u(255, 0, 0)),temp,lst);
	temp = temp->next;
	circ1 = temp;
	insertAfter(new node(pixelArray,1,450,340,50,Color8u(255, 255, 0)),temp,lst);
	temp = temp->next;
	circ2 = temp;
	insertAfter(new node(pixelArray,3,450,340,300,100,50,400,Color8u(0, 0, 0)),temp,lst);
	temp = temp->next;
	tri = temp;
	insertAfter(new node(pixelArray,0,225,25,100,100,Color8u(0, 255, 255)),temp,lst);
	rec2 = temp->next;
	
};

void MarkusErnstHW2App::mouseDown( MouseEvent event )
{
};

void MarkusErnstHW2App::update()
{
	uint8_t* pixelArray = (*mySurface_).getData();

	//set temp back to the start of the list of shapes
	temp = lst;
	temp = temp->next;

	//draw the shapes in the list
	while(temp != NULL){
		if(temp->type == 0){
			drawRect(pixelArray, temp->x, temp->y, temp->w, temp->h, temp->c);
		};
		if(temp->type == 1){
			drawCirc(pixelArray, temp->x, temp->y, temp->r, temp->c);
		};
		if(temp->type == 2){
			drawLine(pixelArray, temp->x, temp->y, temp->w, temp->h, temp->c);
		};
		if(temp->type == 3){
			drawTri(pixelArray, temp->x, temp->y, temp->w, temp->h, temp->x3, temp->y3, temp->c);
		};
		temp = temp->next;

		//blur the image each iteration, so the farther back the shapes are the blurrier they become
		blur(pixelArray, 1/9.0,  1/9.0,  1/9.0,  1/9.0, 1/9.0,  1/9.0,  1/9.0,  1/9.0,  1/9.0);
	};

};

void MarkusErnstHW2App::draw()
{
	//show help or shapes
	if(help == 1){
	gl::draw(tex);
	}
	else
	gl::draw(*mySurface_);
	// clear out the window with black
	//gl::clear( Color( 0, 0, 0 ) ); 
};

CINDER_APP_BASIC( MarkusErnstHW2App, RendererGl )
