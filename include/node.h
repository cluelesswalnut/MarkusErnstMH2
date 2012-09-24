class node{
public:
	node();
	node(uint8_t* ar, int typ, int xx,int yy, int ww, int hh, cinder::Color8u cc);//type 0
	node(uint8_t* ar, int typ, int xx,int yy, double rr, cinder::Color8u cc);//type 1
	//line same as rectangle type 2
	node(uint8_t* ar, int typ, int xx, int yy, int xx2, int yy2, int xx3, int yy3, cinder::Color8u cc);//type 3
	node* next;
	node* prev;

	//what shape is in the node
	int type;

	//rectangle
	int x;
	int y;
	int w;
	int h;
	cinder::Color8u c;

	//circle
	double r;

	//line

	//triangle
	int x3;
	int y3;
};

void insertAfter(node* insert, node* after, node* head);

void remove(node* out);

void move(node* move, int x, int y);
void reverse(node* head);