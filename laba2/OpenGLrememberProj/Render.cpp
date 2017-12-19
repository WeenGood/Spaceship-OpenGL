#include "Render.h"

#include <windows.h>
#include <GL\GL.h>
#include <GL\GLU.h>
#include "MyOGL.h"

#include "Camera.h"
#include "Light.h"
#include "Primitives.h"


bool textureMode = true;
bool lightMode = true;
GLuint texId[5];
int fring = 0, pause = 1;

float *fn;
//класс для настройки камеры
class CustomCamera	 : public Camera
{
public:
	//дистанция камеры
	double camDist;
	//углы поворота камеры
	double fi1, fi2;

	
	//значния масеры по умолчанию
	CustomCamera()
	{
		camDist = 300;
		fi1 = 1;
		fi2 = 1;
	}

	
	//считает позицию камеры, исходя из углов поворота, вызывается движком
	void SetUpCamera()
	{
		//отвечает за поворот камеры мышкой
		lookPoint.setCoords(0, 0, 0);

		pos.setCoords(camDist*cos(fi2)*cos(fi1),
			camDist*cos(fi2)*sin(fi1),
			camDist*sin(fi2));

		if (cos(fi2) <= 0)
			normal.setCoords(0, 0, -1);
		else
			normal.setCoords(0, 0, 1);

		LookAt();
	}

	void CustomCamera::LookAt()
	{
		//функция настройки камеры
		gluLookAt(pos.X(), pos.Y(), pos.Z(), lookPoint.X(), lookPoint.Y(), lookPoint.Z(), normal.X(), normal.Y(), normal.Z());
	}



}  camera;   //создаем объект камеры


//Класс для настройки света
class CustomLight : public Light
{
public:
	CustomLight()
	{
		//начальная позиция света
		pos = Vector3(1, 1, 3);
	}

	
	//рисует сферу и линии под источником света, вызывается движком
	void  DrawLightGhismo()
	{
		glDisable(GL_LIGHTING);

		
		glColor3d(0.9, 0.8, 0);
		Sphere s;
		s.pos = pos;
		s.scale = s.scale*0.01;
		s.Show();
		
		if (OpenGL::isKeyPressed('G'))
		{
			glColor3d(0, 0, 0);
			//линия от источника света до окружности
			glBegin(GL_LINES);
			glVertex3d(pos.X(), pos.Y(), pos.Z());
			glVertex3d(pos.X(), pos.Y(), 0);
			glEnd();

			//рисуем окруность
			Circle c;
			c.pos.setCoords(pos.X(), pos.Y(), 0);
			c.scale = c.scale*5;
			c.Show();
		}

	}

	void SetUpLight()
	{
		GLfloat amb[] = { 0.2, 0.2, 0.2, 0 };
		GLfloat dif[] = { 1.0, 1.0, 1.0, 0 };
		GLfloat spec[] = { .7, .7, .7, 0 };
		GLfloat position[] = { pos.X(), pos.Y(), pos.Z(), 1. };

		// параметры источника света
		glLightfv(GL_LIGHT0, GL_POSITION, position);
		// характеристики излучаемого света
		// фоновое освещение (рассеянный свет)
		glLightfv(GL_LIGHT0, GL_AMBIENT, amb);
		// диффузная составляющая света
		glLightfv(GL_LIGHT0, GL_DIFFUSE, dif);
		// зеркально отражаемая составляющая света
		glLightfv(GL_LIGHT0, GL_SPECULAR, spec);

		glEnable(GL_LIGHT0);
	}


} light;  //создаем источник света

float W = 0, updown = 0, S = 0, rightleft = 0, br = 0;
//старые координаты мыши
int mouseX = 0, mouseY = 0;

void mouseEvent(OpenGL *ogl, int mX, int mY)
{
	int dx = mouseX - mX;
	int dy = mouseY - mY;
	mouseX = mX;
	mouseY = mY;

	//меняем углы камеры при нажатой левой(ПРАВОЙ!!!) кнопке мыши
	if (OpenGL::isKeyPressed(VK_RBUTTON))
	{
		camera.fi1 += 0.01*dx;
		camera.fi2 += -0.01*dy;
	}

	if (OpenGL::isKeyPressed(VK_LBUTTON))
	{
		
		
	}
	
	//двигаем свет по плоскости, в точку где мышь
	if (OpenGL::isKeyPressed('G') && !OpenGL::isKeyPressed(VK_LBUTTON))
	{
		LPPOINT POINT = new tagPOINT();
		GetCursorPos(POINT);
		ScreenToClient(ogl->getHwnd(), POINT);
		POINT->y = ogl->getHeight() - POINT->y;

		Ray r = camera.getLookRay(POINT->x, POINT->y);

		double z = light.pos.Z();

		double k = 0, x = 0, y = 0;
		if (r.direction.Z() == 0)
			k = 0;
		else
			k = (z - r.origin.Z()) / r.direction.Z();

		x = k*r.direction.X() + r.origin.X();
		y = k*r.direction.Y() + r.origin.Y();

		light.pos = Vector3(x, y, z);
	}

	if (OpenGL::isKeyPressed('G') && OpenGL::isKeyPressed(VK_LBUTTON))
	{
		light.pos = light.pos + Vector3(0, 0, 0.02*dy);
	}

	
}

void mouseWheelEvent(OpenGL *ogl, int delta)
{

	if (delta < 0 && camera.camDist <= 1)
		return;
	if (delta > 0 && camera.camDist >= 500)
		return;

	camera.camDist += 0.02*delta;

}

GLfloat trx = 0, trY = 0, trZ = 0;
bool alfa = false, laba2 = false, cam2 = 0;
float fX = 0, fY = 0, vert = 0, goriz = 0;
int ffW = 0;
float up=0, down=0, left=0, right=0;

int moveFlag = 0;

void keyDownEvent(OpenGL *ogl, int key)
{
	if (key == 'L')
	{
		lightMode = !lightMode;
	}

	if (key == 'T')
	{
		textureMode = !textureMode;
	}

	if (key == 'R')
	{
		camera.fi1 = 2;
		camera.fi2 = 1;//y
		camera.camDist = 30;

		//light.pos = Vector3(1,1,0);
	}
	if (key == 'Q')
	{
		rightleft=1;
		
	}
	if (key == 'S')
	{
		updown=-1;
		
	}

	if (key == 'E')
	{
		
		rightleft=-1;
	
	}

	if (key == 'W')
	{
		updown=1;
	
	}
	if (key == 'A')br=1;
	if (key == 'D')br=-1;

	

	if (key == 'F')
	{
		light.pos = camera.pos;
	}
	if (key == VK_NUMPAD4) {
		trx++;
		goriz--;
	}
	if (key == VK_NUMPAD6) {
		trx--;
		goriz++;
	}
	if (key == VK_NUMPAD8) {
		trY++;
		vert++;
	}
	if (key == VK_NUMPAD2) {
		trY--;
		vert--;
	}
	if (key == VK_ADD) {
		trZ--;
	}
	if (key == VK_SUBTRACT) {
		trZ++;
	}
	if (key == 'X' && !alfa) {
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		alfa = true;
	}
	else if (key == 'X' && alfa) {
		glDisable(GL_BLEND);
		alfa = false;
	}
	if (key == 'P'&&pause) {
		pause = 0;
	}
	else if (key == 'P' && !pause) {
		pause = 1;
	}
	if (key == 'K'&&!cam2) {
		cam2 = 1;
	}
	else if (key == 'K'&&cam2) cam2 = 0;
	if (key == 'W')moveFlag = 1;
	if (key == 'S')moveFlag = -1;
	if (key == VK_UP) 
	{
		 up += 1;
	}
	if (key == VK_DOWN)
	{
			up-=1;
	}
	if (key == VK_RIGHT)
	{
		right+=1;
	}
	if (key == VK_LEFT)
	{	right-=1;
	}
}

void keyUpEvent(OpenGL *ogl, int key)
{
	if (key == 'W') moveFlag = 0; 
	if (key == 'S')moveFlag = 0;
	if (key == 'A');
	if (key == 'D');
	if (key == 'Q')
	{
		if(rightleft == 1)
		rightleft = 0;
	}
	if (key == 'W')
	{
		if(updown == 1	)
		updown = 0;
	}

	if (key == 'E')
	{
		if (rightleft == -1)
		rightleft = 0;
	}

	if (key == 'S')
	{
		if(updown == -1)
		updown = 0;
	}
	if (key == 'D') 
	{
		if (br == -1)
			br = 0;
	}
	if (key == 'A')
	{
		if (br == 1)
			br = 0;
	}
}


//выполняется перед первым рендером
void initRender(OpenGL *ogl)
{
	//настройка текстур

	//4 байта на хранение пикселя
	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

	//настройка режима наложения текстур
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	//включаем текстуры
	glEnable(GL_TEXTURE_2D);
	

	//массив трехбайтных элементов  (R G B)
	RGBTRIPLE *texarray;	

	//массив символов, (высота*ширина*4      4, потомучто   выше, мы указали использовать по 4 байта на пиксель текстуры - R G B A)
	char *texCharArray;
	int texW, texH;
	OpenGL::LoadBMP("texture.bmp", &texW, &texH, &texarray);
	OpenGL::RGBtoChar(texarray, texW, texH, &texCharArray);

	
	//генерируем ИД для текстуры
	glGenTextures(3, texId);
	//биндим айдишник, все что будет происходить с текстурой, будте происходить по этому ИД
	glBindTexture(GL_TEXTURE_2D, texId[0]);

	//загружаем текстуру в видеопямять, в оперативке нам больше  она не нужна
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texW, texH, 0, GL_RGBA, GL_UNSIGNED_BYTE, texCharArray);

	//отчистка памяти
	free(texCharArray);
	free(texarray);

	//наводим шмон
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	//OpenGL::LoadBMP("kosmospls.bmp", &texW, &texH, &texarray);
	OpenGL::LoadBMP("kosmospls.bmp", &texW, &texH, &texarray);
	OpenGL::RGBtoChar(texarray, texW, texH, &texCharArray);
	glBindTexture(GL_TEXTURE_2D, texId[1]);//биндим айдишник, все что будет происходить с текстурой, будте происходить по этому ИД
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texW, texH, 0, GL_RGBA, GL_UNSIGNED_BYTE, texCharArray);//загружаем текстуру в видеопямять, в оперативке нам больше  она не нужнаOpenGL::LoadBMP("texture2.bmp", &texW, &texH, &texarray);
																									//отчистка памяти
	free(texCharArray);
	free(texarray);

	//наводим шмон
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	OpenGL::LoadBMP("zvezda.bmp", &texW, &texH, &texarray);
	OpenGL::RGBtoChar(texarray, texW, texH, &texCharArray);
	glBindTexture(GL_TEXTURE_2D, texId[2]);//биндим айдишник, все что будет происходить с текстурой, будте происходить по этому ИД
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texW, texH, 0, GL_RGBA, GL_UNSIGNED_BYTE, texCharArray);//загружаем текстуру в видеопямять, в оперативке нам больше  она не нужнаOpenGL::LoadBMP("texture2.bmp", &texW, &texH, &texarray);
																									//отчистка памяти
	free(texCharArray);
	free(texarray);

	//наводим шмон
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	OpenGL::LoadBMP("asteroid.bmp", &texW, &texH, &texarray);
	OpenGL::RGBtoChar(texarray, texW, texH, &texCharArray);
	glBindTexture(GL_TEXTURE_2D, texId[3]);//биндим айдишник, все что будет происходить с текстурой, будте происходить по этому ИД
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texW, texH, 0, GL_RGBA, GL_UNSIGNED_BYTE, texCharArray);//загружаем текстуру в видеопямять, в оперативке нам больше  она не нужнаOpenGL::LoadBMP("texture2.bmp", &texW, &texH, &texarray);
																									//отчистка памяти
	free(texCharArray);
	free(texarray);

	//наводим шмон
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	////камеру и свет привязываем к "движку"
	ogl->mainCamera = &camera;
	ogl->mainLight = &light;

	// нормализация нормалей : их длины будет равна 1
	glEnable(GL_NORMALIZE);

	// устранение ступенчатости для линий
	glEnable(GL_LINE_SMOOTH); 


	//   задать параметры освещения
	//  параметр GL_LIGHT_MODEL_TWO_SIDE - 
	//                0 -  лицевые и изнаночные рисуются одинаково(по умолчанию), 
	//                1 - лицевые и изнаночные обрабатываются разными режимами       
	//                соответственно лицевым и изнаночным свойствам материалов.    
	//  параметр GL_LIGHT_MODEL_AMBIENT - задать фоновое освещение, 
	//                не зависящее от сточников
	// по умолчанию (0.2, 0.2, 0.2, 1.0)

	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, 0);
	
}





GLfloat angleCube = 0.0f;
GLfloat angleC = 0, angleC2 = 0, forkurs = 0;
int refreshMills = 15;
int i = 45, i2 = 0, winX = 1300, winY = 800;
int count = 0;
int flag1 = 1;
int moon = -1;
float xlast = 0, ylast = 0;
float a;


class asteroid 
{
public:
	float A[3], B[3], C[3], D2[3], D[3], E[3], F[3], G[3], H[3], I[3], J[3], K[3], L[3];
	float randpoint[3];
	
	 asteroid() 
	{
		 A[0] = rand() % 50; A[1] = rand() % 50; A[2] = rand() % 50;
	 
		 B[0] = rand() % 50; B[1] = rand() % 50; B[2] = -rand() % 50;
	 
		 C[0] = rand() % 50; C[1] = -rand() % 50; C[2] = rand() % 50;
	 
		
		 D2[0] = rand() % 50; D2[1] = -rand() % 50; D2[2] = -rand() % 50;
	 
		 E[0] = 0; E[1] = rand() % 50; E[2] = rand() % 50;
	 
		 F[0] = 0; F[1] = rand() % 50; F[2] = -rand() % 50;
	 
		 G[0] = 0; G[1] = -rand() % 50; G[2] = rand() % 50;
	 
		 H[0] = 0; H[1] = -rand() % 50; H[2] = -rand() % 50;
	 
		 I[0] = -rand() % 50; I[1] = rand() % 50; I[2] = rand() % 50;
	
		 J[0] = -rand() % 50; J[1] = rand() % 50; J[2] = -rand() % 50;
	 
		 K[0] = -rand() % 50; K[1] = -rand() % 50; K[2] = rand() % 50;
	 
		 L[0] = -rand() % 50; L[1] = -rand() % 50; L[2] = -rand() % 50;
		 for (int i = 0; i < 3; i++)
		 {
				 randpoint[i] = rand() % 10000-6000;
		 }
	}

	void kamen()
	{
		glBindTexture(GL_TEXTURE_2D, texId[3]);
		
		glColor3f(1, 1, 1);
		glBegin(GL_QUAD_STRIP);
		
		glTexCoord2d(0, 1);
		glVertex3fv(A);
		glTexCoord2d(1, 1);
		glVertex3fv(B);

		glTexCoord2d(0, 0);
		glVertex3fv(C);
		glTexCoord2d(1, 0);
		glVertex3fv(D);

		glTexCoord2d(0, 1);
		glVertex3fv(G);
		glTexCoord2d(1, 1);
		glVertex3fv(H);

		glTexCoord2d(0, 0);
		glVertex3fv(K);
		glTexCoord2d(1, 0);
		glVertex3fv(L);

		glTexCoord2d(0, 1);
		glVertex3fv(I);
		glTexCoord2d(1, 1);
		glVertex3fv(J);

		glTexCoord2d(0, 0);
		glVertex3fv(E);
		glTexCoord2d(1, 0);
		glVertex3fv(F);

		glTexCoord2d(0, 1);
		glVertex3fv(A);
		glTexCoord2d(1, 1);
		glVertex3fv(B);
		glEnd();
		glBegin(GL_QUAD_STRIP);
		glTexCoord2d(1, 1);
		glVertex3fv(A);
		glTexCoord2d(1, 0);
		glVertex3fv(C);

		glTexCoord2d(0, 1);
		glVertex3fv(E);
		glTexCoord2d(0, 0);
		glVertex3fv(G);

		glTexCoord2d(1, 1);
		glVertex3fv(I);
		glTexCoord2d(1, 0);
		glVertex3fv(K);
		glEnd();
		glBegin(GL_QUAD_STRIP);
		glTexCoord2d(0, 1);
		glVertex3fv(B);
		glTexCoord2d(0, 0);
		glVertex3fv(D);

		glTexCoord2d(1, 1);
		glVertex3fv(F);
		glTexCoord2d(1, 0);
		glVertex3fv(H);

		glTexCoord2d(0, 1);
		glVertex3fv(J);
		glTexCoord2d(0, 0);
		glVertex3fv(L);
		glEnd();
	}
}astrs[300];

class ray
{
	float l, y, z;
	ray() 
	{
		l = rand() % 10;
		y = rand() % 10;
		z = rand() % 10;
	}
	void fray() 
	{
		glBegin(GL_LINE);
		glVertex3f(0, 0, 0);
		glVertex3f(l,y,z);
		glEnd();
	}
};
struct point
{
	float X, Y, Z;
	point(float x, float y, float z)
	{
		X = x;
		Y = y;
		Z = z;
	}
};

struct color {
	float r, g, b;
	color(float R, float G, float B) {
		r = R;
		g = G;
		b = B;
	}

};

void circle(float x, float y, color c);
void ring(float x, float y, float x2, float y2, color c);
void trydosphere(float x, float y, float r, color c);
void try2dosphere(float x, float y, float r, color c);
void forpolygon(float r);
void try2dosphere2(float x, float y, float r, color c);
void quadrosphere(float x, float y, float r, color c);
void forquad(float x, float y, float z, float r, int c);
void forquad(float x, float y, float z, float r);
void forwall(float x, float y, float y1, float z, float r, int i, int imax, int kol);
void floor(float x, float  y, float z, float r, int i, int imax);
void floor2(float x, float y, float z, float r, float i, float imax, int kol);
void kursachbody(float r);
void samolet(float r, float x, float y, float z, float rx, float ry, float rz, float angle);
void rocket(float r, float x, float y, float z, float rx, float ry, float rz, float angle, color c1, color c2, color c3);
void konus(float x, float y, float z, float r, float h);
void nlo(float r, float x, float y, float z, float sp, color c1, color c2, color c3);
void nebo();
void gunfornlo(float x, float y, float z, color c);
float *normal(float x, float y, float z, float x1, float y1, float z1, float x2, float y2, float z2);
void dvigatel();
void cirlefor2lab();
void laba3();
void beze(float A[], float B[], float C[], float D[], float E[], float F[], float G[], float H[]);
void beze(float A[], float B[], float C[], float D[], int i);
void ermit(float A[], float A2[], float B[], float B2[]);
void klevoedvigenie(float A[], float B[], float C[], float D[],int i);
void klevoedvigenie(float A[], float B[], float C[], float D[], float E[], float F[], float G[], float H[], int i);
void poverhnostbeze();
void sphere(float R, int s2, int s1, int m, int l, bool f);
void planet(float R, int s2, int s1, int m, int l, bool f);
void manipulate();
void normnormal(float x, float y, float z, float x1, float y1, float z1, float x2, float y2, float z2);
void shina(float x, float y, float z, float rx, float ry, float rz, float iin, float imaxin, float iout, float imaxout);
void badupr();
void guns();
void gunfork();
void other2();
void illum(float x, float y, float z);
void Render(OpenGL *ogl)
{
	
	
	glRotatef(90, 1, 0, 0);
	if (cam2) {
		glTranslatef(-600, 0, 0);
		glRotatef(-angleC/10, 0, 1, 0);
	}
	glTranslatef(trx, trY, trZ);
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_LIGHTING);

	glEnable(GL_DEPTH_TEST);
	if (textureMode)
		glEnable(GL_TEXTURE_2D);

	if (lightMode)
		glEnable(GL_LIGHTING);
	//glDisable(GL_LIGHT0);
	glEnable(GL_LIGHT2);
	GLfloat amb3[] = { 1, 1, 1, 1 };
	GLfloat dif3[] = { 1,1,1,1 };
	GLfloat spec3[] = { 1,1,1, 0 };
	GLfloat position3[] = { 0,0,0, 1 };

	// параметры источника света
	glLightfv(GL_LIGHT2, GL_POSITION, position3);
	// характеристики излучаемого света
	// фоновое освещение (рассеянный свет)
	glLightfv(GL_LIGHT2, GL_AMBIENT, amb3);
	// диффузная составляющая света
	glLightfv(GL_LIGHT2, GL_DIFFUSE, dif3);
	// зеркально отражаемая составляющая света
	glLightfv(GL_LIGHT2, GL_SPECULAR, spec3);


	GLfloat amb2[] = {0,0.2,0.3, 1 };//{ 0.2, 0.2, 0.1, 1. };
	GLfloat dif2[] = {0.3,0.5,0.1, 1 };// { 0.4, 0.65, 0.5, 1. };
	GLfloat spec2[] = {1,1,1, 0 };//{ 0.9, 0.8, 0.3, 1. };
	GLfloat sh2= 0.1f * 256;

	//фоновая
	glMaterialfv(GL_FRONT, GL_AMBIENT, amb2);
	//дифузная
	glMaterialfv(GL_FRONT, GL_DIFFUSE, dif2);
	//зеркальная
	glMaterialfv(GL_FRONT, GL_SPECULAR, spec2);
	//размер блика
	glMaterialf(GL_FRONT, GL_SHININESS, sh2);

	
	GLfloat amb[] = { 0.2, 0.2, 0.1, 1. };//{ 0.2, 0.2, 0.1, 1. };
	GLfloat dif[] = { 0.4, 0.65, 0.5, 1. };// { 0.4, 0.65, 0.5, 1. };
	GLfloat spec[] = { 0.9, 0.8, 0.3, 1. };//{ 0.9, 0.8, 0.3, 1. };
	GLfloat sh = 0.1f * 256;

	//фоновая
	glMaterialfv(GL_FRONT, GL_AMBIENT, amb);
	//дифузная
	glMaterialfv(GL_FRONT, GL_DIFFUSE, dif);
	//зеркальная
	glMaterialfv(GL_FRONT, GL_SPECULAR, spec);
	//размер блика
	glMaterialf(GL_FRONT, GL_SHININESS, sh);


	//чтоб было красиво, без квадратиков (сглаживание освещения)
	glShadeModel(GL_SMOOTH);
	glClearColor(0, 0, 0, 0);
	

	//cirlefor2lab();
//	forpolygon(0.3);//лаба по компьютерной графике
	//laba3();
	//glPushMatrix();
	//manipulate();
	////dvigatel();
	//samolet(0.3, 10, 0, 0, 0, 0, 0, 0);
	//glPopMatrix(); 
	kursachbody(0.05);//курсач
	//	glDisable(GL_LIGHT2);
	//glPushMatrix();
	//glTranslatef(3,0,0);
	//poverhnostbeze();
	//glPopMatrix();
	
	color c4(0, 0, 1);
	color c6(0, 10, 3);
	/*{
	glPushMatrix();
	glRotatef(angleC*	0.1, -1, 50, 0.0f);
	trydosphere(-50, 1, 6, c4);
	glTranslatef(-50, 0, 1);
	glRotatef(angleC * 3, 1, 9.3, 0);
	forquad(9.3, -1, 0, 0.3, c6);
	glPopMatrix();
	color c(1, 0, 0);



	glPushMatrix();
	glRotatef(angleC*	0.3, 10.0f, 35.0f, 0.0f);
	try2dosphere(41, -10, 4, c);
	glPopMatrix();

	color c2(0, 1, 0);
	color c3(0, 0, 1);

	glPushMatrix();
	glRotatef(angleC*	0.7, -10.0f, 20.0f, 0.0f);
	try2dosphere(20, 10, 2, c);
	glPopMatrix();

	glPushMatrix();
	glRotatef(angleC*0.6, 0, 23, 0);
	try2dosphere(23, 0, 3, c);
	glPopMatrix();

	glPushMatrix();
	color c5(0.5, 0, 10);



	forquad(0, 0, 0, 8, c5);
	glPopMatrix();

	glPushMatrix();
	glRotatef(angleC * 3, 10, -10, 0);
	forquad(10, 10, 10, 0.4, c2);
	glPopMatrix();


	glPushMatrix();
	glRotatef(angleC * 3, 10, 10, 0);
	forquad(-10, 10, -10, 0.4, c3);
	glPopMatrix();


	glPushMatrix();
	glRotatef(angleC * 3, 0, -10, 0);
	forquad(-10, 0, 0, 0.4, c3);
	glPopMatrix();


	glRotatef(angleC, 0.0f, 1.0f, 1.0f);
	ring(12, 12, 11, 11, c3);
	glRotatef(angleC, 1.0f, 0.0f, 0.0f);
	ring(10.5, 8.5, 9.5, 9.5, c2);
	}*/
	angleC += 0.1*pause;
	angleC2 += 0.1;
	
}

void circle(float x, float y, color c) {
	float x3, y3;
	//printf("%f %f %f %f" ,x, x2, y, y2);
	int i = 0;
	while (i < 180) {
		glBegin(GL_POLYGON);
		glColor3f(c.r, c.g, c.b);
		x3 = x*cos(i * (M_PI / 180)) + y*sin(i * (M_PI / 180));
		y3 = -x*sin(i * (M_PI / 180)) + y*cos(i*(M_PI / 180));
		glVertex3f(x3, 0, y3);

		x3 = x*cos(i * (M_PI / 180)) - y*sin(i * (M_PI / 180));
		y3 = -x*sin(i * (M_PI / 180)) - y*cos(i * (M_PI / 180));
		glVertex3f(x3, 0, y3);

		x3 = -x*cos(i * (M_PI / 180)) - y*sin(i * (M_PI / 180));
		y3 = x*sin(i * (M_PI / 180)) - y*cos(i * (M_PI / 180));
		glVertex3f(x3, 0, y3);

		x3 = -x*cos(i * (M_PI / 180)) + y*sin(i * (M_PI / 180));
		y3 = x*sin(i * (M_PI / 180)) + y*cos(i * (M_PI / 180));
		glVertex3f(x3, 0, y3);
		glEnd();
		i += 1;
	}
}
void ring(float x, float y, float x2, float y2, color c) {
	int i = 0;
	glBegin(GL_QUADS);
	for (i = 0; i <= 26; i++) {
		glColor3f(c.r, c.g, c.b);
		a = (float)i / 26 * M_PI * 2;
		glVertex3f(cos(a) * x, 0, sin(a) * y);
		i++;
		a = (float)i / 26 * M_PI * 2;
		glVertex3f(cos(a) * x, 0, sin(a) * y);
		glVertex3f(cos(a) * x2, 0, sin(a) * y2);
		i--;
		a = (float)i / 26 * M_PI * 2;
		glVertex3f(cos(a) * x2, 0, sin(a) * y2);
	}
	glEnd();
}
void trydosphere(float x, float y, float r, color c) {
	float a2 = 0;
	glBegin(GL_LINES);
	glColor3f(c.r, c.g, c.b);
	for (float i2 = 0; i2 <= 600; i2 += 4) {
		a2 = i2 / 600 * M_PI * 2;
		for (float i = 0; i < 50; i++) {
			a = i / 50 * M_PI * 2;
			//glVertex3f(r*cos(a)*sin(a2)+x, r*sin(a)*sin(a2)+y, r*cos(a2));
			glVertex3f(r*cos(a)*sin(a2) + x, r*cos(a2), r*sin(a)*sin(a2) + y);
		}

		//	printf(" %f ", a2);
	}
	glEnd();
}
void try2dosphere(float x, float y, float r, color c) {
	float a2 = 0;

	glBegin(GL_LINES);
	glColor3f(c.r, c.g, c.b);
	for (float i2 = 0; i2 <300; i2++) {
		a2 = i2 / 200 * M_PI * 2;
		//printf("| a2 = %f cos(%f) = %f |", a2, 15*i2, cos(a2));
		for (float i = 0; i < 50; i++) {
			a = i / 50 * M_PI * 2;
			//glVertex3f(r*sin(a)*sin(a2)+x, r*sin(a)*cos(a2),r*cos(a)+y);
			glVertex3f(r*sin(a)*sin(a2) + x, r*cos(a) + y, r*sin(a)*cos(a2));
		}
	}
	glEnd();
}
void try2dosphere2(float x, float y, float r, color c) {
	float a2 = 0;

	glBegin(GL_LINES);
	glColor3f(c.r, c.g, c.b);
	for (float i2 = 0; i2 <300; i2++) {
		a2 = i2 / 200 * M_PI * 2;
		//printf("| a2 = %f cos(%f) = %f |", a2, 15*i2, cos(a2));
		for (float i = 0; i < 50; i++) {
			a = i / 50 * M_PI * 2;
			glVertex3f(r*sin(a)*sin(a2) + x, r*sin(a)*cos(a2), r*cos(a) + y);
			//glVertex3f(r*sin(a)*sin(a2) + x, r*cos(a) + y, r*sin(a)*cos(a2));
		}
	}
	glEnd();
}
void forpolygon(float r) {
	glScalef(r, r, r);
	glRotatef(angleC*0.3, 0, 1, 0);
	glTranslatef(0, 0, 45);
	glBegin(GL_POLYGON);
	glVertex3f(-60, 20, -10);
	glVertex3f(-70, 20, -80);
	glVertex3f(-40, 20, -50);
	glEnd();

	glColor3f(1, 0, 0);//фронт
	glBegin(GL_QUADS);

	glColor3f(0, 0, 1);//верх
	glVertex3f(30, 20, -50);
	glVertex3f(30, 50, -50);
	glVertex3f(-40, 50, -50);
	glVertex3f(-40, 20, -50);

	glColor3f(0, 1, 1);
	glVertex3f(-40, 20, -50);//верх
	glVertex3f(-40, 50, -50);
	glVertex3f(-70, 50, -80);
	glVertex3f(-70, 20, -80);//

	glColor3f(1, 1, 0);
	glVertex3f(-60, 20, -10);//лево
	glVertex3f(-60, 50, -10);
	glVertex3f(-70, 50, -80);
	glVertex3f(-70, 20, -80);
	glEnd();

	glBegin(GL_POLYGON);
	glVertex3f(-60, 50, -10);
	glVertex3f(-70, 50, -80);
	glVertex3f(-40, 50, -50);
	glEnd();

	forwall(45, 20, 50, -25, 30, -24, 12, 72);
	floor(45, 20, -25, 30, 16, 28);
	floor(45, 50, -25, 30, 16, 28);
	forwall(-9.7, 20, 50, 111.47, 131.47, -47, -24, 150);
	floor2(-9.7, 50, -111.47, 131.47, 24, 47, 150);
	floor2(-9.7, 20, -111.47, 131.47, 24, 47, 150);

}
void forwall(float x, float y, float y1, float z, float r, int i, int imax, int kol) {
	float a;
	glBegin(GL_QUADS);

	for (; i < imax; i++) {
		a = (i*M_PI * 2) / kol;
		glColor3f(1, 0, 0);

		glVertex3f(r*cos(a) + x, y, r*sin(a) + z);//60 20 0

		glColor3f(0, 1, 0);

		glVertex3f(r*cos(a) + x, y1, r*sin(a) + z);//60 50 -50

		i++;
		a = (i*M_PI * 2) / kol;
		glColor3f(0, 0, 1);

		glVertex3f(r*cos(a) + x, y1, r*sin(a) + z);//30 50 0

		glColor3f(1, 0, 1);

		glVertex3f(r*cos(a) + x, y, r*sin(a) + z);//30 20 -50
		i--;
		a = (i*M_PI * 2) / kol;
	}

	glEnd();
}
void floor(float x, float  y, float z, float r, int i, int imax) {
	float a;
	glBegin(GL_TRIANGLES);
	for (; i < imax; i++) {
		a = (i*M_PI * 2) / 24;
		glVertex3f(x, y, z);
		glVertex3f(r*cos(a) + x, y, r*sin(a) + z);
		i++;
		a = (i*M_PI * 2) / 24;
		glVertex3f(r*cos(a) + x, y, r*sin(a) + z);
		i--;
		a = (i*M_PI * 2) / 24;
	}
	glEnd();
}
void floor2(float x, float y, float z, float r, float i, float imax, int kol) {
	float a;
	glColor3f(0, 1, 0);
	glBegin(GL_POLYGON);
	for (; i < imax; i++) {
		a = (i*M_PI * 2) / kol;
		glVertex3f(-40, y, -50);
		glVertex3f(30, y, -50);
		glVertex3f((r)*cos(a) + x, y, -1 * ((r)*sin(a) + z));
		i++;
		a = (i*M_PI * 2) / kol;

		glVertex3f((r)*cos(a) + x, y, -1 * ((r)*sin(a) + z));
		i--;
		a = (i*M_PI * 2) / kol;
	}
	glEnd();
}
void quadrosphere(float x, float y, float r, color c) {
	float a2 = 0;

	glBegin(GL_QUADS);
	glColor3f(c.r, c.g, c.b);
	for (float i2 = 0; i2 <300; i2++) {
		a2 = i2 / 200 * M_PI * 2;
		//printf("| a2 = %f cos(%f) = %f |", a2, 15*i2, cos(a2));
		for (float i = 0; i < 50; i++) {
			a = i / 50 * M_PI * 2;
			//glVertex3f(r*sin(a)*sin(a2)+x, r*sin(a)*cos(a2),r*cos(a)+y);
			glVertex3f(r*sin(a)*sin(a2) + x + 2, r*cos(a) + y + 2, r*sin(a)*cos(a2));
			glVertex3f(r*sin(a)*sin(a2) + x + 2, r*cos(a) + y - 2, r*sin(a)*cos(a2));
			glVertex3f(r*sin(a)*sin(a2) + x - 2, r*cos(a) + y - 2, r*sin(a)*cos(a2));
			glVertex3f(r*sin(a)*sin(a2) + x - 2, r*cos(a) + y + 2, r*sin(a)*cos(a2));
		}
	}
	glEnd();
}
void forquad(float x, float y, float z, float r, int c) {
	glBindTexture(GL_TEXTURE_2D, texId[c]);
	float a = 1, a2 = 1, mb = 1;
	glBegin(GL_QUADS);
	for (int i2 = 0; i2 <26; i2++) {
		
		a2 = (i2*M_PI * 2) / 26;
		for (int i =  0; i <13; i++) {
			a = (i * M_PI * 2) / 26;
			glTexCoord2d(0.5 - cos(a)*cos(a2) / 2, 0.5 + sin(a2) / 2);
			glNormal3f(-cos(a)*cos(a2), -sin(a2), -sin(a)*cos(a2));
			glVertex3f(r*cos(a)*cos(a2) + x, r*sin(a2) + y, r*sin(a)*cos(a2) + z);
			i++;
			a = (i * M_PI * 2) / 26;
			glTexCoord2d(0.5 - cos(a)*cos(a2) / 2, 0.5 + sin(a2 ) / 2);
			glVertex3f(r*cos(a)*cos(a2) + x, r*sin(a2) + y, r*sin(a)*cos(a2) + z);
			i2++;
			a2 = (i2*M_PI * 2) / 26;
			glTexCoord2d(0.5 - cos(a)*cos(a2) / 2, 0.5 + sin(a2) / 2);
			glVertex3f(r*cos(a)*cos(a2) + x, r*sin(a2) + y, r*sin(a)*cos(a2) + z);
			i--;
			a = (i * M_PI * 2) / 26;
			glTexCoord2d(0.5 - cos(a)*cos(a2) / 2, 0.5 + sin(a2) / 2);
			glVertex3f(r*cos(a)*cos(a2) + x, r*sin(a2) + y, r*sin(a)*cos(a2) + z);
			i2--;
			a2 = (i2*M_PI * 2) / 26;
		}
	}
	glEnd();
}
void forquad(float x, float y, float z, float r) {
	glBindTexture(GL_TEXTURE_2D, texId[1]);
	float a = 1, a2 = 1, mb = 1;
	double t1 = 0, t2 = 0;
	glColor3f(1, 1, 1);
	glBegin(GL_QUADS);
	for (int i2 = 1; i2 <= 26; i2++) {
		a2 = (i2*M_PI * 2) / 26;
		t1 = 0;
		t2 += 512 / 26;
		for (int i = 1; i <= 13; i++) {
			a = (i * M_PI * 2) / 26;
			glTexCoord2d(0.5 - cos(a)*cos(a2) / 2, 0.5 + sin(a2) / 2);
			glNormal3f(-cos(a)*cos(a2), -sin(a2), -sin(a)*cos(a2));
			glVertex3f(r*cos(a)*cos(a2) + x, r*sin(a2) + y, r*sin(a)*cos(a2) + z);
			i++;
			a = (i * M_PI * 2) / 26;
			glTexCoord2d(0.5 - cos(a)*cos(a2) / 2, 0.5 + sin(a2) / 2);
			glVertex3f(r*cos(a)*cos(a2) + x, r*sin(a2) + y, r*sin(a)*cos(a2) + z);
			i2++;
			a2 = (i2*M_PI * 2) / 26;
			glTexCoord2d(0.5 - cos(a)*cos(a2) / 2, 0.5 + sin(a2) / 2);
			glVertex3f(r*cos(a)*cos(a2) + x, r*sin(a2) + y, r*sin(a)*cos(a2) + z);
			i--;
			a = (i * M_PI * 2) / 26;
			glTexCoord2d(0.5 - cos(a)*cos(a2) / 2, 0.5 + sin(a2) / 2);
			glVertex3f(r*cos(a)*cos(a2) + x, r*sin(a2) + y, r*sin(a)*cos(a2) + z);
			i2--;
			a2 = (i2*M_PI * 2) / 26;
		}
	}
	glEnd();
}
void back(float x, float y, float z, float rx, float rz) {
	float a = 1;
	glBegin(GL_TRIANGLES);
	for (int i = -13; i < 13; i++) {
		a = (i*M_PI * 2) / 52;

		glVertex3f(x, y, z);
		glVertex3f(x + rx*cos(a), y, z + rz*sin(a));
		i++;
		a = (i*M_PI * 2) / 52;
		glVertex3f(x + rx*cos(a), y, z + rz*sin(a));
		i--;
		a = (i*M_PI * 2) / 52;
	}
	glEnd();
}
void backwall(float x, float y, float y1, float z, float rx, float rz) {
	float a = 1;
	glBegin(GL_QUADS);
	for (int i = -13; i < 13; i++) {
		a = (i*M_PI * 2) / 52;
		glNormal3f(cos(a), 0, sin(a));
		glVertex3f(x + rx*cos(a), y, z + rz*sin(a));
		glVertex3f(x + rx*cos(a), y1, z + rz*sin(a));
		i++;
		a = (i*M_PI * 2) / 52;
		glVertex3f(x + rx*cos(a), y1, z + rz*sin(a));
		glVertex3f(x + rx*cos(a), y, z + rz*sin(a));
		i--;
		a = (i*M_PI * 2) / 52;
	}
	glEnd();
}
void paluba() {
	glBindTexture(GL_TEXTURE_2D, texId[0]);
	float y = 0;// высота, на которой находится палуба
	glColor3f(1,1,1);
	glBegin(GL_POLYGON);//палуба
	glNormal3d(0, -1, 0);
	glVertex3f(70, y, -80);
 	glVertex3f(-35, y, 0);
	glVertex3f(130, y, -20);
	glVertex3f(200, y, -20);
	glVertex3f(210, y, -30);
	glVertex3f(250, y, -30);
	glVertex3f(290, y, -40);
	glVertex3f(290, y, -70);
	glVertex3f(275, y, -110);
	glVertex3f(240, y, -110);
	glVertex3f(230, y, -120);
	glVertex3f(200, y, -120);
	glVertex3f(195, y, -125);
	glVertex3f(0, y, -125);
	glVertex3f(-70, y, -110);
	glVertex3f(-160, y, -100);
	glVertex3f(-160, y, -70);
	glVertex3f(-60, y, -50);
	glVertex3f(-35, y, 0);
	glEnd();
	back(145, y, -70, 200, 65);
	y = y + 5;
	glBegin(GL_POLYGON);//палуба

	glNormal3d(0, 1, 0); 
	glTexCoord2d(0.449, 0.15625);		glVertex3f(70, y, -80);
	glTexCoord2d(0.244, 0); 			glVertex3f(-35, y, 0);//360=100%
	glTexCoord2d(0.566, 0.039);			glVertex3f(130, y, -20);
	glTexCoord2d(0.7, 0.039);			glVertex3f(200, y, -20);
	glTexCoord2d(0.7, 0.244);			glVertex3f(200, y, -125);
	glTexCoord2d(0.3125, 0.244);		glVertex3f(0, y, -125);
	glTexCoord2d(0.1757, 0.214);		glVertex3f(-70, y, -110);
	glTexCoord2d(0, 0.1953);			glVertex3f(-160, y, -100);
	glTexCoord2d(0, 0.1367);			glVertex3f(-160, y, -70);
	glTexCoord2d(0.1953, 0.097);		glVertex3f(-60, y, -50);//100 50
	glTexCoord2d(0.244, 0);				glVertex3f(-35, y, 0);
	glEnd();
	glNormal3f(0, 0, 0);
	back(145, y, -70, 200, 65);

	glColor3f(0.6, 0.5, 0.4);
	glBegin(GL_QUADS);
	glNormal3f(-20, 0, 180);
	glVertex3f(-35, y, 0);
	glVertex3f(-35, y - 5, 0);
	glVertex3f(145, y - 5, -20);
	glVertex3f(145, y, -20);
	
	glNormal3f(-50, 0, 25);
	glVertex3f(-35, y, 0);
	glVertex3f(-35, y - 5, 0);
	glVertex3f(-60, y - 5, -50);
	glVertex3f(-60, y, -50);

	glNormal3f(-20, 0, 100);
	glVertex3f(-60, y - 5, -50);
	glVertex3f(-60, y, -50);
	glVertex3f(-160, y, -70);
	glVertex3f(-160, y - 5, -70);

	glNormal3f(-30, 0, 0);
	glVertex3f(-160, y, -70);
	glVertex3f(-160, y - 5, -70);
	glVertex3f(-160, y - 5, -100);
	glVertex3f(-160, y, -100);

	glNormal3f(10, 0, -90);
	glVertex3f(-160, y - 5, -100);
	glVertex3f(-160, y, -100);
	glVertex3f(-70, y, -110);
	glVertex3f(-70, y - 5, -110);

	glNormal3f(-15, 0, -70);
	glVertex3f(-70, y, -110);
	glVertex3f(-70, y - 5, -110);
	glVertex3f(0, y - 5, -125);
	glVertex3f(0, y, -125);

	glNormal3f(0, 0, -145);
	glVertex3f(0, y - 5, -125);
	glVertex3f(0, y, -125);
	glVertex3f(145, y, -125);
	glVertex3f(145, y - 5, -125);

	glNormal3f(-130, 0, 0);
	glVertex3f(145, y, -135);
	glVertex3f(145, y - 5, -135);
	glVertex3f(145, y, -5);
	glVertex3f(145, y - 5, -5);
	glEnd();
	glNormal3f(0, 0, 0);
	backwall(145, y, y - 5, -70, 200, 65);

	samolet(0.05, 1700, 120, -750, 0, 1, 0, -80);
	samolet(0.05, 2200, 120, -850, 0, 1, 0, -80);
	samolet(0.05, 2700, 120, -950, 0, 1, 0, -80);
	samolet(0.05, 3200, 120, -1050, 0, 1, 0, -80);
	color c1(1, 1, 0.3);
	color c2(0.3, 0.3, 0.45);
	color c3(0.1, 0.1, 0.4);
	rocket(0.05, -140, -3, -85, 0, 0, 0, 0, c1, c2, c3);
	color c11(0.1, 0.5, 0.3);
	color c22(0.2, 0.55, 0.45);
	color c33(0.54, 0.33, 0.4);
	rocket(0.05, -100, -3, -85, 0, 0, 0, 0, c11, c22, c33);
	//samolet(1, 000, 100, 0, 0, 1, 0, -80);//для редактирования
	color c111(1, 0.5, 0.9);
	color c222(0.7, 1, 0.7);
	color c333(1, 0.1, 0.3);
	nlo(0.08, 50, 12, -100, 0, c111, c222, c333);
	nlo(0.08, 10, 12, -100, 0, c1, c2, c33);
	color c1111(0, 0.2, 0.8);
	color c2222(0, 1, 1);
	color c3333(0, 1, 0.1);	
	nlo(0.08, -30, 12, -100, 0, c1111, c2222, c3333);
	
}
void backcorpus(float x, float y, float z, float rx, float ry, float rz, float iin, float imaxin, float i, float imax, float kol) {
	float a = 1, a2 = 1;

	glBegin(GL_QUADS);
	for (; i <imax; i++) {
		a2 = (i*M_PI * 2) / kol;
		for (int k = iin; k <imaxin; k++) {
			a = (k*M_PI * 2) / 23;
			glNormal3f(cos(a)*cos(a2), sin(a2), sin(a)*cos(a2));
			glVertex3f(x + rx*cos(a)*cos(a2), y + ry*sin(a2), z + rz*sin(a)*cos(a2));
			k++;
			a = (k*M_PI * 2) / 23;
			glVertex3f(x + rx*cos(a)*cos(a2), y + ry*sin(a2), z + rz*sin(a)*cos(a2));
			i++;
			a2 = (i*M_PI * 2) / kol;
			glVertex3f(x + rx*cos(a)*cos(a2), y + ry*sin(a2), z + rz*sin(a)*cos(a2));
			k--;
			a = (k*M_PI * 2) / 23;
			glVertex3f(x + rx*cos(a)*cos(a2), y + ry*sin(a2), z + rz*sin(a)*cos(a2));
			i--;
			a2 = (i*M_PI * 2) / kol;
		}
	}
	glEnd();
}
void corpus() {
	glColor4f(0, 10, 10,0);
	backcorpus(130, 0, -70, 214, 50, 67, -17, -6, 24, 36, 48);
	glColor4f(0.1, 0.5, 1,0.5);
	backcorpus(43, 5, -70, 305, 50, 67, -16, -7, 12, 24, 48);
	backcorpus(100, 0, -70, 160, 50, 50, 0, 14, -12, 0, 24);
}
void krutilka(float x, float y, float z, float r, float shir) {
	float a = 1;
	glBegin(GL_QUADS);
	for (int i = 0; i < 50; i++) {
		a = (i*M_PI * 2) / 50;
		glNormal3f(0, cos(a), sin(a));
		glVertex3f(x + shir, r*cos(a) + y, r*sin(a) + z);
		i++;
		a = (i*M_PI * 2) / 50;
		glVertex3f(x + shir, r*cos(a) + y, r*sin(a) + z);

		glVertex3f(x, r*cos(a) + y, r*sin(a) + z);
		i--;
		a = (i*M_PI * 2) / 50;
		glVertex3f(x, r*cos(a) + y, r*sin(a) + z);
	}
	glEnd();
}

void cily(float x, float y, float z, float r, float vis) {
	float a = 1;
	glBegin(GL_QUADS);
	for (int i = 0; i < 50; i++) {
		a = (i*M_PI * 2) / 50;
		glNormal3f(cos(a), 0, sin(a));
		glVertex3f(x + r*cos(a), vis + y, r*sin(a) + z);
		i++;
		a = (i*M_PI * 2) / 50;
		glVertex3f(x + r*cos(a), vis + y, r*sin(a) + z);

		glVertex3f(x + r*cos(a), y, r*sin(a) + z);
		i--;
		a = (i*M_PI * 2) / 50;
		glVertex3f(x + r*cos(a), y, r*sin(a) + z);
	}
	glEnd();
}
void cilz(float x, float y, float z, float r, float vis) {
	float a = 1;
	glBegin(GL_QUADS);
	for (int i = 0; i < 50; i++) {
		a = (i*M_PI * 2) / 50;
		glNormal3f(cos(a), sin(a), 0);
		glVertex3f(x + r*cos(a), r*sin(a) + y, vis + z);
		i++;
		a = (i*M_PI * 2) / 50;
		glVertex3f(x + r*cos(a), r*sin(a) + y, vis + z);

		glVertex3f(x + r*cos(a), r*sin(a) + y, z);
		i--;
		a = (i*M_PI * 2) / 50;
		glVertex3f(x + r*cos(a), r*sin(a) + y, z);
	}
	glEnd();
}
void other() {
	forkurs += 0.2;
	glPushMatrix();
	glRotatef(forkurs, 1, 0, 0);
	glTranslatef(0, 0, 70);	
	GLfloat amb2[] = { 0.9,0.9,0.9, 1 };
	GLfloat dif2[] = { 0.9,0.9,0.9, 1 };
	GLfloat spec2[] = { 1,1,1, 0 };
	GLfloat sh2 = 0.1f * 256;

	glMaterialfv(GL_FRONT, GL_AMBIENT, amb2);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, dif2);
	glMaterialfv(GL_FRONT, GL_SPECULAR, spec2);
	glMaterialf(GL_FRONT, GL_SHININESS, sh2);
	glColor3f(0.9, 0.9, 0.9);
	krutilka(160, 0, -70, 66, 20);
	krutilka(160, 0, -70, 101, 20);
	krutilka(160, 0, -70, 99, 20);
	amb2[0] = 0; amb2[1] = 0, amb2[2] = 0;
	dif2[0] = 0; dif2[1] = 0; dif2[2] = 0;
	glMaterialfv(GL_FRONT, GL_AMBIENT, amb2);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, dif2);
	glColor3f(0, 0, 0);
	krutilka(150, 0, -70, 65, 40);
	krutilka(150, 0, -70, 100, 40);
	glPushMatrix();
	glRotatef(90, 0, 1, 0);
	shina(70, 170, 0, 144, 29, 144, 0, 24, -3, 3);
	glPopMatrix();
	cily(170, 64, -70, 10, 38);
	cily(170, -64, -70, 10, -38);
	cilz(170, 0, -172, 10, 38);
	cilz(170, 0, 32, 10, -38);
	glPopMatrix();
	glPushMatrix();
	glRotatef(-forkurs, 1, 0, 0);
	glTranslatef(0, 0, 70);
	krutilka(240, 0, -70, 57, 20);
	krutilka(240, 0, -70, 83, 20);
	cily(250, 52, -70, 2.5, 33);
	cily(250, -52, -70, 2.5, -33);
	cilz(250, 0, -155, 2.5, 33);
	cilz(250, 0, 15, 2.5, -33);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(305, -10, 40);
	dvigatel();
	
	GLfloat spec[] = { 0.9, 0.8, 0.3, 1. };
	glMaterialfv(GL_FRONT, GL_SPECULAR, spec);
	GLfloat light4_diffuse[] = { 16,0,20 };
	GLfloat light4_position[] = {50,0,0, 1.0 };
	GLfloat light4_spot_direction[] = {0,0,-1};
	glDisable(GL_LIGHT2);
	glEnable(GL_LIGHT4);
	glLightfv(GL_LIGHT4, GL_DIFFUSE, light4_diffuse);
	glLightfv(GL_LIGHT4, GL_POSITION, light4_position);
	glLightf(GL_LIGHT4, GL_SPOT_CUTOFF, 30);
	glLightfv(GL_LIGHT4, GL_SPOT_DIRECTION, light4_spot_direction);
	glLightf(GL_LIGHT4, GL_SPOT_EXPONENT, 0);
	glPopMatrix();
	glPushMatrix();
	glColor3f(0, 0, 0);
	glTranslatef(305, -10, -40);
	glRotatef(180, 1, 0, 0);
	dvigatel();
	glEnable(GL_LIGHT5);
	glLightfv(GL_LIGHT5, GL_DIFFUSE, light4_diffuse);
	glLightfv(GL_LIGHT5, GL_POSITION, light4_position);
	glLightf(GL_LIGHT5, GL_SPOT_CUTOFF, 30);
	glLightfv(GL_LIGHT5, GL_SPOT_DIRECTION, light4_spot_direction);
	glLightf(GL_LIGHT5, GL_SPOT_EXPONENT, 0);
	glPopMatrix();
	glEnable(GL_LIGHT2);
}

void kursachbody(float r) {
	color c1(1, 1, 1);
	//	sphere(300, 26,26,1,1, 0);
	//planet(10,200, 200, 1, 1, 1);
	for (int i = 0; i < 300; i++)
	{
		glPushMatrix();
		glTranslatef(astrs[i].randpoint[0], astrs[i].randpoint[1], astrs[i].randpoint[2]);
		astrs[i].kamen();
		glPopMatrix();
	}
	glPushMatrix();
	glRotatef(90+angleC/2, 1, 1, 0);
	forquad(0, 0, 0, 80, 2);
	glPopMatrix();
	forquad(0, 0, 0, 4000);
	GLfloat amb2[] = { 0.4,0.4,0.4, 1 };//{ 0.2, 0.2, 0.1, 1. };
	GLfloat dif2[] = { 0.5,0.4,0.6, 1 };// { 0.4, 0.65, 0.5, 1. };
	GLfloat spec2[] = { 1,1,1, 0 };//{ 0.9, 0.8, 0.3, 1. };
	GLfloat sh2 = 0.1f * 256;

	//фоновая
	glMaterialfv(GL_FRONT, GL_AMBIENT, amb2);
	//дифузная
	glMaterialfv(GL_FRONT, GL_DIFFUSE, dif2);
	//зеркальная
	glMaterialfv(GL_FRONT, GL_SPECULAR, spec2);
	//размер блика
	glMaterialf(GL_FRONT, GL_SHININESS, sh2);
	glPushMatrix();
	glScalef(r, r, r);
	
	glRotatef(angleC/10, 0, 1, 0);
	
	glTranslatef(12000, 0, 0);
	glRotatef(-90, 0, 1, 0);
	glColor3f(0.1, 0.1, 0.3);
	glPushMatrix();
	glTranslatef(0, 0, 70);
	
	paluba();
	
	 amb2[0] = 0; amb2[1] = 0; amb2[2] = 0.2;
	 dif2[0] = 0; dif2[1] = 0; dif2[2] = 0.3;
	glMaterialfv(GL_FRONT, GL_AMBIENT, amb2);
	//дифузная
	glMaterialfv(GL_FRONT, GL_DIFFUSE, dif2);
	//зеркальная
	glMaterialfv(GL_FRONT, GL_SPECULAR, spec2);
	//размер блика
	glMaterialf(GL_FRONT, GL_SHININESS, sh2);
	corpus();
	glPopMatrix();
	color c111(0.2, 1, 0.8);
	color c222(0.4, 1, 1);
	color c333(0.2, 1, 0.3);
	glPushMatrix();
	badupr();
	nlo(0.08, 10, 12, 6, 0, c222, c111, c333);//для редактирования
	glPopMatrix();
	glPushMatrix();
	other();
	guns();
	other2();
	glPopMatrix();
	glPopMatrix();

	//nebo();//WARNING
	
}
void samolet(float r, float x, float y, float z, float rx, float ry, float rz, float angle) {
	glPushMatrix();
	glScalef(r, r, r);
	glTranslatef(x, y, z);
	glRotatef(angle, rx, ry, rz);
	glColor3f(0.2, 0.2, 0.6);
	GLfloat amb2[] = { 0,0.1,1, 1 };
	GLfloat dif2[] = { 0,0.1,1, 1 };
	GLfloat spec2[] = { 1,1,1, 0 };
	GLfloat sh2 = 0.1f * 256;

	//фоновая
	glMaterialfv(GL_FRONT, GL_AMBIENT, amb2);
	//дифузная
	glMaterialfv(GL_FRONT, GL_DIFFUSE, dif2);
	//зеркальная
	glMaterialfv(GL_FRONT, GL_SPECULAR, spec2);
	//размер блика
	glMaterialf(GL_FRONT, GL_SHININESS, sh2);
	glBegin(GL_POLYGON);
	glNormal3f(0, 1, 0);
	glVertex3f(0, 0, 0);
	glVertex3f(-220, 0, 0);
	glVertex3f(-100, 0, -40);
	glVertex3f(-60, 0, -40);
	glVertex3f(100, 0, -160);
	glVertex3f(120, 0, -160);
	glVertex3f(120, 0, 160);
	glVertex3f(100, 0, 160);
	glVertex3f(-60, 0, 40);;
	glVertex3f(-100, 0, 40);
	glVertex3f(-220, 0, 0);
	glEnd();
	glNormal3f(0, -1, 0);
	glBegin(GL_POLYGON);
	glVertex3f(0, -5, 0);
	glVertex3f(-220, -5, 0);
	glVertex3f(-100, -5, -40);
	glVertex3f(-60, -5, -40);
	glVertex3f(100, -5, -160);
	glVertex3f(120, -5, -160);
	glVertex3f(120, -5, 160);
	glVertex3f(100, -5, 160);
	glVertex3f(-60, -5, 40);;
	glVertex3f(-100, -5, 40);
	glVertex3f(-220, -5, 0);
	glEnd();
	amb2[0] = 0.4; amb2[1] = 0.4; amb2[2] = 0.4;
	dif2[0] = 0.6; dif2[1] = 0.6; amb2[2] = 0.6;
	glMaterialfv(GL_FRONT, GL_AMBIENT, amb2);
	//дифузная
	glMaterialfv(GL_FRONT, GL_DIFFUSE, dif2);
	glColor3f(0.82, 0.8, 0.8);
	backcorpus(120, 0, 0, 240, 25, 80, -18, -5, 0, 6, 24);
	backcorpus(120, 0, 0, 240, 25, 80, -18, -5, -6, 0, 24);
	glColor3f(0.2, 0.2, 0.2);
	amb2[0] = 0; amb2[1] = 0; amb2[2] = 0;
	dif2[0] = 0; dif2[1] = 0; amb2[2] = 0;
	glMaterialfv(GL_FRONT, GL_AMBIENT, amb2);
	//дифузная
	glMaterialfv(GL_FRONT, GL_DIFFUSE, dif2);
	glBegin(GL_POLYGON);
	glVertex3f(-170, 0, 0);
	glVertex3f(-50, 0, -40);
	glVertex3f(-110, 20, 0);
	glVertex3f(-50, 0, 40);
	glEnd();
	glRotatef(-90, 0,1,0 );
	amb2[0] = 0; amb2[1] = 0; amb2[2] = 0;
	dif2[0] = 0.5; dif2[1] = 0.5; amb2[2] = 0.5;
	glMaterialfv(GL_FRONT, GL_AMBIENT, amb2);
	//дифузная
	glMaterialfv(GL_FRONT, GL_DIFFUSE, dif2);
	cilz(25, 0, -160,20,50);
	cilz(-25, 0, -160, 20, 50);
	cilz(-60, 0, -160, 10, 50);
	cilz(60, 0, -160, 10, 50);
	glPopMatrix();
	amb2[0] = 0.2; amb2[1] = 0.3; amb2[2] = 0.5;
	dif2[0] = 0.2; dif2[1] = 0.3; amb2[2] = 0.5;
	glMaterialfv(GL_FRONT, GL_AMBIENT, amb2);
	//дифузная
	glMaterialfv(GL_FRONT, GL_DIFFUSE, dif2);
}
void nlo(float r, float x, float y, float z, float sp, color c1, color c2, color c3) {
	color c(0, 0, 0);
	GLfloat amb2[] = { 1,0,0, 1 };
	GLfloat dif2[] = { 1,0,0, 1 };
	GLfloat spec2[] = { 1,1,1, 0 };
	GLfloat sh2 = 0.1f * 256;

	//фоновая
	glMaterialfv(GL_FRONT, GL_AMBIENT, amb2);
	//дифузная
	glMaterialfv(GL_FRONT, GL_DIFFUSE, dif2);
	//зеркальная
	glMaterialfv(GL_FRONT, GL_SPECULAR, spec2);
	//размер блика
	glMaterialf(GL_FRONT, GL_SHININESS, sh2);
	glPushMatrix();
	glTranslatef(x, y, z);
	glScalef(r, r, r);
	if (fring > -30 && fring < 0)
	{
		glColor3f(c2.r, c2.g, c2.b); amb2[0] = c2.r; amb2[1] = c2.g; amb2[2] = c2.b;
	}
	else
	{
		glColor3f(c1.r, c1.g, c1.b);
		amb2[0] = c1.r; amb2[1] = c1.g; amb2[2] = c1.b;
	}
	glMaterialfv(GL_FRONT, GL_AMBIENT, amb2);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, dif2);
	backcorpus(0, 0, 0, 170, 20, 170, -13, 13, 0, 24, 24);
	if (fring < 30 && fring>0) 
	{
		glColor3f(c2.r, c2.g, c2.b);
		amb2[0] = c2.r; amb2[1] = c2.g; amb2[2] = c2.b;
	}
	else 
	{
		glColor3f(c1.r, c1.g, c1.b); 
		amb2[0] = c1.r; amb2[1] = c1.g; amb2[2] = c1.b;
	}
	
	dif2[0] = 0.2; dif2[1] = 0.3; amb2[2] = 0.5;
	glMaterialfv(GL_FRONT, GL_AMBIENT, amb2);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, dif2);
	backcorpus(0, 0, 0, 75, 55, 75, -13, 13, 0, 24, 24);
	amb2[0] = 0.4; amb2[1] = 0.4; amb2[2] = 0.4;
	dif2[0] = 0.8; dif2[1] = 0.8; amb2[2] = 0.8;
	glMaterialfv(GL_FRONT, GL_AMBIENT, amb2);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, dif2);
	spec2[0] = 0.2; spec2[1] = 0.2; spec2[2] = 0.2;
	glMaterialfv(GL_FRONT, GL_SPECULAR, spec2);
	glColor3f(0, 0, 0);
	
	spec2[0] = 1; spec2[1] = 1; spec2[2] = 1;
	glMaterialfv(GL_FRONT, GL_SPECULAR, spec2);
	glPushMatrix();
	glBegin(GL_QUADS);
	glNormal3f(0, 0, 1);
	glVertex3f(100, -85, 10);
	glVertex3f(100, -85, -10);
	glVertex3f(80, -85, -10);
	glVertex3f(80, -85, 10);
	glEnd();
	glRotatef(45, 0, 0, 1);
	cily(0, -120, 0, 2, 100);
	glPopMatrix();

	glPushMatrix();
	glRotatef(120, 0, 1, 0);
	glBegin(GL_QUADS);
	glVertex3f(100, -85, 10);
	glVertex3f(100, -85, -10);
	glVertex3f(80, -85, -10);
	glVertex3f(80, -85, 10);
	glEnd();
	glRotatef(45, 0, 0, 1);
	cily(0, -120, 0, 2, 100);
	glPopMatrix();

	glPushMatrix();
	glRotatef(240, 0, 1, 0);
	glBegin(GL_QUADS);
	glVertex3f(100, -85, 10);
	glVertex3f(100, -85, -10);
	glVertex3f(80, -85, -10);
	glVertex3f(80, -85, 10);
	glEnd();
	glRotatef(45, 0, 0, 1);
	cily(0, -120, 0, 2, 100);
	glPopMatrix();
	color cr(c3.r, c3.g, c3.b);
	glPushMatrix();
	glRotatef(fring, 1, 0, 1);
	glNormal3f(0, 1, 0);
	ring(180, 180, 190,195, cr);
	glPopMatrix();
	glPushMatrix();
	glRotatef(fring, -1, 0, -1);
	glNormal3f(0, 1, 0);
	ring(170, 170, 180, 185, cr);
	glPopMatrix();

	glPushMatrix();
	gunfornlo(70, 20, 70, c3);
	glPopMatrix();

	glPushMatrix();
	glRotatef(90, 0, 1, 0);
	gunfornlo(70, 20, 70, c3);
	glPopMatrix();

	glPushMatrix();
	glRotatef(180, 0, 1, 0);
	gunfornlo(70, 20, 70, c3);
	glPopMatrix();

	glPushMatrix();
	glRotatef(270, 0, 1, 0);
	gunfornlo(70, 20, 70, c3);
	glPopMatrix();

	glPushMatrix();
	glRotatef(180, 0, 0, 1);
	gunfornlo(70, 20, 70, c3);
	glPopMatrix();

	glPushMatrix();
	glRotatef(180, 0, 0, 1);
	glRotatef(90, 0, 1, 0);
	gunfornlo(70, 20, 70, c3);
	glPopMatrix();

	glPushMatrix();
	glRotatef(180, 0, 0, 1);
	glRotatef(180, 0, 1, 0);
	gunfornlo(70, 20, 70, c3);
	glPopMatrix();

	glPushMatrix();
	glRotatef(180, 0, 0, 1);
	glRotatef(270, 0, 1, 0);
	gunfornlo(70, 20, 70, c3);
	glPopMatrix();
	
	glPopMatrix();
	if (fring > 25||fring<-25) flag1 *= -1; 
	fring=fring+flag1;
}
void konus(float x, float y, float z, float r, float h) {
	float a = 1, a2 = 1;
	glBegin(GL_QUADS);
	for (int i2 = 0; i2 < 24; i2++) {
		a2 = (i2*M_PI * 2) / 24;
		for (int i = 0; i < 24; i++) {
			a = (i*M_PI * 2) / 24;
			glNormal3f(cos(a)*cos(a2), sin(a2), sin(a)*cos(a2));
			glVertex3f(x + r*cos(a)*cos(a2), h*sin(a2) + y, z + r*sin(a)*cos(a2));
			i++;
			a = (i*M_PI * 2) / 24;
			glVertex3f(x + r*cos(a)*cos(a2), h*sin(a2) + y, z + r*sin(a)*cos(a2));

			i2++;
			a2 = (i2*M_PI * 2) / 24;
			glVertex3f(x + r*cos(a)*cos(a2), h*sin(a2) + y, z + r*sin(a)*cos(a2));
			i--;
			a = (i*M_PI * 2) / 24;
			glVertex3f(x + r*cos(a)*cos(a2), h*sin(a2) + y, z + r*sin(a)*cos(a2));
			i2--;
			a2 = (i2*M_PI * 2) / 24;
		}
	}
	glEnd();
}
void rocket(float r, float x, float y, float z, float rx, float ry, float rz, float angle, color c1, color c2, color c3) {

	glPushMatrix();
	glTranslatef(x, y, z);
	glScalef(r, r, r);
	glRotatef(angle, rx, ry, rz);
	GLfloat amb2[] = { c1.r,c1.g,c1.b, 1 };
	GLfloat dif2[] = { c1.r,c1.g,c1.b, 1 };
	GLfloat spec2[] = { 1,1,1, 0 };
	GLfloat sh2 = 0.1f * 256;
	glMaterialfv(GL_FRONT, GL_AMBIENT, amb2);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, dif2);
	glMaterialfv(GL_FRONT, GL_SPECULAR, spec2);
	glMaterialf(GL_FRONT, GL_SHININESS, sh2);
	glColor3f(c1.r, c1.g, c1.b);
	cily(0, 200, 0, 85, 700);
	samolet(1, 95, 700, 0, 0, 0, 1, -90);
	amb2[0] = c2.r; amb2[1] = c2.g; amb2[2] = c2.b;
	glMaterialfv(GL_FRONT, GL_AMBIENT, amb2);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, dif2);
	glColor3f(c2.r, c2.g, c2.b);
	konus(0, 900, 0, 85, 300);

	float *one = normal(-85, 550, 0, -42.5, 240, -70, -250, 150, 0);
	float *two = normal(-85, 550, 0, -42.5, 240, 70, -250, 150, 0);
	glColor3f(c3.r, c3.g, c3.b);
	amb2[0] = c3.r; amb2[1] = c3.g; amb2[2] = c3.b;
	glMaterialfv(GL_FRONT, GL_AMBIENT, amb2);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, dif2);
	glBegin(GL_TRIANGLES);
	glNormal3f(-one[0], -one[1], -one[2]);
	glVertex3f(-85, 550, 0);
	glVertex3f(-42.5, 240, -70);
	glVertex3f(-250, 150, 0);
	
	glNormal3f(-two[0], -two[1], two[2]);
	glVertex3f(-85, 550, 0);
	glVertex3f(-42.5, 240, 70);
	glVertex3f(-250, 150, 0);
	glEnd();
	glNormal3f(0, 0, 0);
	glPushMatrix();
	glRotatef(90, 0, 1, 0);
	glBegin(GL_POLYGON);
	glNormal3f(-one[0], -one[1], -one[2]);
	glVertex3f(-85, 550, 0);
	glVertex3f(-42.5, 240, -70);
	glVertex3f(-250, 150, 0);
	glNormal3f(-two[0], -two[1], two[2]);
	glVertex3f(-85, 550, 0);
	glVertex3f(-42.5, 240, 70);
	glVertex3f(-250, 150, 0);
	glEnd();

	glPopMatrix();
	glPushMatrix();
	glRotatef(-90, 0, 1, 0);
	glBegin(GL_POLYGON);
	glNormal3f(-one[0], -one[1], -one[2]);
	glVertex3f(-85, 550, 0);
	glVertex3f(-42.5, 240, -70);
	glVertex3f(-250, 150, 0);
	glNormal3f(-two[0], -two[1], two[2]);
	glVertex3f(-85, 550, 0);
	glVertex3f(-42.5, 240, 70);
	glVertex3f(-250, 150, 0);
	glEnd();

	glPopMatrix();
	glPushMatrix();
	glRotatef(180, 0, 1, 0);
	glBegin(GL_POLYGON);
	glNormal3f(-one[0], -one[1], -one[2]);
	glVertex3f(-85, 550, 0);
	glVertex3f(-42.5, 240, -70);
	glVertex3f(-250, 150, 0);
	glNormal3f(-two[0], -two[1], two[2]);
	glVertex3f(-85, 550, 0);
	glVertex3f(-42.5, 240, 70);
	glVertex3f(-250, 150, 0);
	glEnd();

	glPopMatrix();
	glPopMatrix();
	delete(one);
	delete(two);
}
void gunfornlo(float x,float y,float z, color c) {
	glPushMatrix();
	glTranslatef(x, y, z);
	glColor3f(c.r, c.g, c.b);
	backcorpus(0, 0, 0, 20, 20, 20, 0, 13, -2, 14, 24);
	glColor3f(0,0,0);
	glPushMatrix();
	glRotatef(45, 0, 1, 0);
	cilz(-10, 4, 4, 2, 30);
	cilz(10, 4, 4, 2, 30);
	glPopMatrix();
}
void nebo() {
	glColor3f(1, 1, 1);
	glBegin(GL_POINTS);
	for (int i = 0; i < 10000; i++)
		glVertex3f(rand()%100,rand()%100, 0);
	glEnd();
}
float *normal(float x, float y, float z, float x1, float y1, float z1, float x2, float y2, float z2) {
	float AB[] = { x1 - x,y1 - y,z1 - z };
	float AC[] = { x2 - x,y2 - y,z2 - z };
	float *OP = new float[3];
	
	OP[0] = (AB[1] - AC[2])*(AC[1] - AB[2]);
	OP[1] = (AB[0] - AC[2])*(AC[0] - AB[2]);
	OP[2] = (AB[0] - AC[1])*(AC[0] - AB[1]);
	return OP;
}
void normnormal(float x, float y, float z, float x1, float y1, float z1, float x2, float y2, float z2)
{
	float OP[3];
	float AB[] = { x1 - x,y1 - y,z1 - z };
	float AC[] = { x2 - x,y2 - y,z2 - z };

	//  i     j    k
	//AB[0] AB[1] AB[2]
	//AC[0] AC[1] AC[2]
	OP[0] = AB[1]*AC[2] - AB[2]*AC[1];
	OP[1] = AB[0] * AC[2] - AB[2] * AC[0];
	OP[2] =AB[0]* AC[1] - AB[1] * AC[0];
	OP[0] = OP[0] / sqrt(pow(OP[0], 2) + pow(OP[1], 2) + pow(OP[2], 2));
	OP[1] = OP[1] / sqrt(pow(OP[0], 2) + pow(OP[1], 2) + pow(OP[2], 2));
	OP[2] = OP[2] / sqrt(pow(OP[0], 2) + pow(OP[1], 2) + pow(OP[2], 2));
	glNormal3f(OP[0], -OP[1], OP[2]);
}
float  a2 = 1;
float volna[] = {1,1,1,1,1,1,1,1,1,1};
void fvolna(float r,float l,int n, float v) {
	float a = 1;

	a2 = (volna[n]*M_PI) / (2*l);
	glBegin(GL_LINES);
	for (int i = 0; i < 24; i++) {
		a = (i*M_PI * 2) / 24;
		glVertex3f(volna[n], sin(a) * r*cos(a2), cos(a2)*cos(a) * r);
		i++;
		a = (i*M_PI * 2) / 24;
		glVertex3f(volna[n], cos(a2)*sin(a) * r, cos(a2)*cos(a) * r);
		i--;
		a = (i*M_PI * 2) / 24;
	}
	volna[n]+=v;
	if (volna[n] > l) volna[n] = 0;
	glEnd();
}
void dvigatel(){
	glPushMatrix();
	glRotatef(-90, 0, 0, 1);
	backcorpus(0, 0, 0, 11, 30, 11, 0, 24,-12, 0, 24);
	glPopMatrix();
	glColor3f(0.8, 0, 1);
	GLfloat amb[] = { 0.2, 0.2, 0.1, 1. };//{ 0.2, 0.2, 0.1, 1. };
	GLfloat dif[] = { 0.4, 0.65, 0.5, 1. };// { 0.4, 0.65, 0.5, 1. };
	GLfloat spec[] = { 0.9, 0.8, 0.3, 1. };//{ 0.9, 0.8, 0.3, 1. };
	GLfloat sh = 0.1f * 256;

	//фоновая
	glMaterialfv(GL_FRONT, GL_AMBIENT, amb);
	//дифузная
	glMaterialfv(GL_FRONT, GL_DIFFUSE, dif);
	//зеркальная
	glMaterialfv(GL_FRONT, GL_SPECULAR, spec);
	//размер блика
	glMaterialf(GL_FRONT, GL_SHININESS, sh);
	fvolna(10,50,0,1);
	fvolna(10, 55, 1,2);
	fvolna(10, 50, 2, 3);
	fvolna(10, 45, 3, 4);
	fvolna(10, 60, 4, 5);
	fvolna(10, 53, 5, 4);
	fvolna(10, 47, 6, 3);
	fvolna(10, 47, 7, 2);
	fvolna(10, 54, 8, 1);

}
void cirlefor2lab() {
	float a2 = 1;
	glColor3f(1, 1, 0);
	glBegin(GL_TRIANGLES);
	glNormal3f(0, 1, 0);
	for (float i = 0; i < 24; i++) {
		a2 = (i*M_PI * 2) / 24;
		glTexCoord2d(0.5, 0.5);
		glVertex3f(0, 0, 0);
		glTexCoord2d(-cos(a2)/2+0.5, sin(a2)/2 + 0.5);
		glVertex3f(cos(a2)*10, 0, sin(a2) * 10);
		i++;
		a2 = (i*M_PI * 2) / 24;
		glTexCoord2d(-cos(a2) / 2 + 0.5, sin(a2) / 2 + 0.5);
		glVertex3f(cos(a2) * 10, 0, sin(a2) * 10);
		i--;
		a2 = (i*M_PI * 2) / 24;

	}
	glEnd();
}
void laba3()  
{
	glDisable(GL_LIGHTING);
	static float A[] = { 0 , 20 , 100 };
	static float B[] = { 0,20,0 };
	static float C[] = { 0,20,0 };
	static float D[] = { 0,20,100 };
	static float E[] = { 0 , 20 ,0 };
	static float t = 0;
	//Первая Безье
		glBegin(GL_LINES);
		glColor3f(0, 0.7, 0);
		glVertex3f(0,20, 100);
		glVertex3f(0, 20, 0);
		glColor3f(0.7, 0, 0);
		glVertex3f(0, 20, 0);
		glVertex3f(100, 20, 0);
		glColor3f(0, 0, 0.7);
		
		glVertex3fv(A);
		glVertex3fv(B);
		
		glEnd();
		glColor3f(0, 1, 1);
		glPointSize(6);
	
		C[0] = B[0] * (t);
		C[2] = A[2] * (1 - t);
		
		glBegin(GL_POINTS);
		glVertex3fv(C);
		glEnd();
		A[2] = A[2]--;
		B[0] = B[0]++;
		
		glBegin(GL_LINE_STRIP);
		for (float I = 0; I<t;I=I+0.01) {
			glVertex3f(E[0]*I,20,D[2]*(1-I));
			D[2]--;
			E[0]++;
			if (D[2] < 0)break;
			if (E[0] > 100)break;
			}
		glEnd();
		D[2] = 100;
		E[0] = 0;

		if (t > 1)t = 0;
		if (A[2] < 0)A[2] = 100;
		if (B[0] > 100)B[0] = 0;
		////вторая Безье
		//glBegin(GL_LINES);
		//glColor3f(0, 0.7, 0);
		//glVertex3f(0, 15, 100);
		//glVertex3f(0, 15, 0);
		//glColor3f(0.7, 0, 0);
		//glVertex3f(0, 15, 0);
		//glVertex3f(-100, 15, 0);
		//glColor3f(0, 0, 0.7);
		//glVertex3f(-100, 15, 0);
		//glVertex3f(-100, 15, 100);
		//glEnd();
		//static float A2[] = { 0, 15, 100 };
		//static float B2[] = { 0,15,0 };
		//static float C2[] = { 0,15,0 };
		//static float D2[] = {0,15,0};
		//D2[0] = A2[0];
		//D2[2] = A2[2];
		//C2[0] = B2[0];
		//C2[2] = B2[2];
		//glColor3f(0.7, 0.6, 0.2);
		//glBegin(GL_LINES);
		//glVertex3f(A2[0], 15,0);
		//glVertex3f(0, 15, A2[2]);

		//glVertex3f(B2[0], 15, 0);
		//glVertex3f(-100, 15, B2[2]);
		//	
		//glEnd();
		//if (--A2[0] < -100) A2[0] = 0;;
		//if (--A2[2] <0) A2[2] = 100;
		//if (--B2[0] < -100) B2[0] = 0;;
		//if (++B2[2] >100) B2[2] = 0;
		//glColor3f(0.5, 0.5, 0.2);
		//C2[0] = C2[0] *t;
		//C2[2] = C2[2] * (1-t);	
		//D2[0] = D2[0] * t;
		//D2[2] = D2[2] * (1 - t);
		//glBegin(GL_LINES);
		//glVertex3fv(C2);
		//glVertex3fv(D2);
		//glEnd();
		t = t + 0.01;
		float A3[] = { 200,0,-100 };
		float B3[] = { -500,500,300 };
		float C3[] = { 200,00,-100 };
		float D3[] = { -200,-100,010 };
		float E3[] = { 100,30,300 };
		float F3[] = { -300,200,200 };
		float G3[] = { -400,-400,0 };
		float H3[] = { -300,110,010 };
		//beze(A3, B3, C3, D3, E3, F3, H3, G3);

		glPushMatrix();
		//klevoedvigenie(A3, B3, C3, D3, E3, F3, H3, G3);
		klevoedvigenie(A3, B3, C3, D3, 0);
		samolet(0.05, 0, 0, 0, 0, 1, 0, 90);
		glPopMatrix();
		beze(E3, F3, H3, G3, 0);
		glPushMatrix();

		klevoedvigenie(E3, F3, H3, G3, 1);
		samolet(0.05, 0, 0, 0, 0, 1, 0, 90);
		glPopMatrix();
		beze(A3, B3, C3, D3, 1);
		float A1[] = { 0,50,-100 };
		float A2[] = { 0,40,-60 };
		float B1[] = { 0,40,100 };
		float B2[] = { 100,50, 0 };
		ermit(A1, A2, B1, B2);
		float C1[] = { 100,60,-100 };
		float C2[] = { 120, 60, -100 };
		float D1[] = { -100,60,-100 };
		float D2[] = { -120,60,-100 };
		ermit(C1, C2, D1, D2);
		poverhnostbeze();
}
void beze(float A[], float B[], float C[], float D[],float E[], float F[],float G[], float H[]) 
{
	static float t = 0;
	glDisable(GL_LIGHTING);
	glColor3f(0.3, 0.5, 0.7);
	glBegin(GL_LINE_STRIP);
	glVertex3fv(A);
	glVertex3fv(B);
	glVertex3fv(C);
	glVertex3fv(D);
	glVertex3fv(E);
	glVertex3fv(F);
	glVertex3fv(G);
	glVertex3fv(H);
	glEnd();
	glColor3f(1, 0, 0);
	glBegin(GL_LINE_STRIP);
	for (float I = 0; I <= t; I = I + 0.01) {
		 float X = pow((1 - I), 7)*A[0] + 7 * I*pow((1 - I), 6)*B[0] + 7 * pow(I, 2)*pow(1 - I,5)*C[0] + 7 * pow(I, 3)*pow(1 - I, 4)*D[0] + 7 * pow(I, 4)*pow(1 - I, 3)*E[0] + 7 * pow(I, 5)*pow(1 - I, 2)*F[0] + 7 * pow(I, 6)*(1 - I)*G[0] + pow(I, 7)*(1 - I)*G[0];
		 float Y = pow((1 - I), 7)*A[1] + 7 * I*pow((1 - I), 6)*B[1] + 7 * pow(I, 2)*pow(1 - I, 5)*C[1] + 7*pow(I, 3)*pow(1 - I, 4)*D[1] + 7 * pow(I, 4)*pow(1 - I, 3)*E[1] + 7 * pow(I, 5)*pow(1 - I, 2)*F[1] + 7 * pow(I, 6)*(1 - I)*G[1] + pow(I, 7)*(1 - I)*G[1];
		 float Z = pow((1 - I), 7)*A[2] + 7 * I*pow((1 - I), 6)*B[2] + 7 * pow(I, 2)*pow(1 - I, 5)*C[2] + 7*pow(I, 3)*pow(1 - I, 4)*D[2] + 7 * pow(I, 4)*pow(1 - I, 3)*E[2] + 7 * pow(I, 5)*pow(1 - I, 2)*F[2] + 7 * pow(I, 6)*(1 - I)*G[2] + pow(I, 7)*(1 - I)*G[2];
		glVertex3f(X, Y, Z);
	}
	glEnd();
	if (t >= 1)t = 0;
		t += 0.01;
}
void beze(float A[], float B[], float C[], float D[], int i)
{
	static float t[] = {0,0,0,0};
	glDisable(GL_LIGHTING);
	glColor3f(0.3, 0.5, 0.7);
	glBegin(GL_LINE_STRIP);
	glVertex3fv(A);
	glVertex3fv(B);
	glVertex3fv(C);
	glVertex3fv(D);
	glEnd();
	glColor3f(1, 0, 0);
	glBegin(GL_LINE_STRIP);
	for (float I = 0; I <= t[i]; I = I + 0.01) {
		float X = pow((1 - I), 3)*A[0] + 3 * I*pow((1 - I), 2)*B[0] + 3 * pow(I, 2)*(1 - I)*C[0] + pow(I, 3)*D[0];
		float Y = pow((1 - I), 3)*A[1] + 3 * I*pow((1 - I), 2)*B[1] + 3 * pow(I, 2)*(1 - I)*C[1] + pow(I, 3)*D[1];
		float Z = pow((1 - I), 3)*A[2] + 3 * I*pow((1 - I), 2)*B[2] + 3 * pow(I, 2)*(1 - I)*C[2] + pow(I, 3)*D[2];
		glVertex3f(X, Y, Z);
	}
	glEnd();
	if (t[i] >= 1)t[i] = 0;
	t[i] += 0.01;
}
void ermit(float A[], float A2[], float B[], float B2[])
{
	glColor3f(0.3, 0.8, 0.3);
	glBegin(GL_LINES);
	glVertex3fv(A);
	glVertex3fv(A2);
	glVertex3fv(B);
	glVertex3fv(B2);
	glEnd();
	glBegin(GL_LINE_STRIP);
	for (float t = 0; t <= 1; t += 0.01) 
	{
		float X = A[0] * (2 * pow(t, 3) - 3 * pow(t, 2) + 1) + B[0] * (-2 * pow(t, 3) + 3 * pow(t, 2)) + A2[0] * (pow(t, 3) - 2 * pow(t, 2) + t) + B2[0] * (pow(t, 3) - pow(t, 2));
		float Y = A[1] * (2 * pow(t, 3) - 3 * pow(t, 2) + 1) + B[1] * (-2 * pow(t, 3) + 3 * pow(t, 2)) + A2[1] * (pow(t, 3) - 2 * pow(t, 2) + t) + B2[1] * (pow(t, 3) - pow(t, 2));
		float Z = A[2] * (2 * pow(t, 3) - 3 * pow(t, 2) + 1) + B[2] * (-2 * pow(t, 3) + 3 * pow(t, 2)) + A2[2] * (pow(t, 3) - 2 * pow(t, 2) + t) + B2[2] * (pow(t, 3) - pow(t, 2));
		
		glVertex3f(X, Y, Z);
			
	}
	glEnd();
}
void klevoedvigenie(float A[], float B[], float C[], float D[], int i)
{
	static float t[] = { 0,0,0,0 };
	float X = pow((1 - t[i]), 3)*A[0] + 3 * t[i] * pow((1 - t[i]), 2)*B[0] + 3 * pow(t[i], 2)*(1 - t[i])*C[0] + pow(t[i], 3)*D[0];
	float Y = pow((1 - t[i]), 3)*A[1] + 3 * t[i] * pow((1 - t[i]), 2)*B[1] + 3 * pow(t[i], 2)*(1 - t[i])*C[1] + pow(t[i], 3)*D[1];
	float Z = pow((1 - t[i]), 3)*A[2] + 3 * t[i] * pow((1 - t[i]), 2)*B[2] + 3 * pow(t[i], 2)*(1 - t[i])*C[2] + pow(t[i], 3)*D[2];
	static float forX[] = { 0,0,0,0 };
	static float forY[] = { 0,0,0,0 };
	static float forZ[] = { 0,0,0,0 };
	float vecos[] = { X - forX[i],Y - forY[i],Z - forZ[i] };
	float vecyaw[] = { -vecos[1],vecos[0],0 };
	float vecpritch[] = { vecos[2],0,vecos[0] };//0,1,0
	float yaw = 180 * acos(vecos[2] / (sqrt(pow(vecos[0], 2) + pow(vecos[1], 2) + pow(vecos[2], 2)))) / M_PI;
	forX[i] = X;
	forY[i] = Y;
	forZ[i] = Z;
	t[i] += 0.01;
	if (t[i] > 1)t[i] = 0;
	glTranslatef(X, Y, Z);
	glRotatef(yaw, vecyaw[0], vecyaw[1], vecyaw[2]);
}
		
void klevoedvigenie(float A[], float B[], float C[], float D[], float E[], float F[], float G[], float H[], int i)
{
	float flagX = 1, flagY = 1, flagZ = 1;
	static float t[] = {0,0,0};
	float X = pow((1 - t[i]), 7)*A[0] + 7 * t[i]*pow((1 - t[i]), 6)*B[0] + 7 * pow(t[i], 2)*pow(1 - t[i], 5)*C[0] + 7 * pow(t[i], 3)*pow(1 - t[i], 4)*D[0] + 7 * pow(t[i], 4)*pow(1 - t[i], 3)*E[0] + 7 * pow(t[i], 5)*pow(1 - t[i], 2)*F[0] + 7 * pow(t[i], 6)*(1 - t[i])*G[0] + pow(t[i], 7)*(1 - t[i])*G[0];
	float Y = pow((1 - t[i]), 7)*A[1] + 7 * t[i]*pow((1 - t[i]), 6)*B[1] + 7 * pow(t[i], 2)*pow(1 - t[i], 5)*C[1] + 7 * pow(t[i], 3)*pow(1 - t[i], 4)*D[1] + 7 * pow(t[i], 4)*pow(1 - t[i], 3)*E[1] + 7 * pow(t[i], 5)*pow(1 - t[i], 2)*F[1] + 7 * pow(t[i], 6)*(1 - t[i])*G[1] + pow(t[i], 7)*(1 - t[i])*G[1];
	float Z = pow((1 - t[i]), 7)*A[2] + 7 * t[i]*pow((1 - t[i]), 6)*B[2] + 7 * pow(t[i], 2)*pow(1 - t[i], 5)*C[2] + 7 * pow(t[i], 3)*pow(1 - t[i], 4)*D[2] + 7 * pow(t[i], 4)*pow(1 - t[i], 3)*E[2] + 7 * pow(t[i], 5)*pow(1 - t[i], 2)*F[2] + 7 * pow(t[i], 6)*(1 - t[i])*G[2] + pow(t[i], 7)*(1 - t[i])*G[2];
	static float forX = 0;
	static float forY = 0;
	static float forZ = 0;
	float vecos[] = { X - forX,Y - forY,Z - forZ };
	float vecyaw[] = { -vecos[1],vecos[0],0 };
	float vecpritch[] = { vecos[2],0,vecos[0] };//0,1,0
	float yaw = 180 * acos(vecos[2] / (sqrt(pow(vecos[0], 2) + pow(vecos[1], 2) + pow(vecos[2], 2)))) / M_PI;
	float pitch = 180 * acos(vecos[1] / (sqrt(pow(vecos[0], 2) + pow(vecos[1], 2) + pow(vecos[2], 2)))) / M_PI;
	float roll = 180 * acos(vecos[0] / (sqrt(pow(vecos[0], 2) + pow(vecos[1], 2) + pow(vecos[2], 2)))) / M_PI;
	forX = X;
	forY = Y;
	forZ = Z;
	t[i] += 0.01;
	if (t[i] > 1)t[i] = 0;
	glTranslatef(X, Y, Z);
	glRotatef(yaw, vecyaw[0], vecyaw[1], vecyaw[2]);
	//glRotatef(pitch, vecpritch[0], vecpritch[1], vecpritch[2]);
	//glRotatef(roll, 0, 0, 1);
}
int factorial(int i) 
{
	int a = 1;
	for (int k = 1; k <= i; k++) 
	{
		a = a*k;
	}
	return a;
}

void poverhnostbeze() 
{
	point A1(-20,0,0);  point B1(-10,10,0);	   point C1(10,0,0); point D1( 20,0,0 );
	point A2( -20,10,10); point B2(-10,0,10); point C2( 10,0,10 ); point D2( 20,0,10 );
	point A3( -20,0,20 ); point B3(-10,0,20); point C3( 10,0,20 ); point  D3(20,-20,20 );
	point A4(-20,0,30); point B4(-10,0,30); point C4(10,0,30); point D4( 20,0,30 );
	point A[4][4] = { {A1,A2,A3,A4},
					  {B1,B2,B3,B4}, 
					  {C1,C2,C3,C4}, 
					  {D1,D2,D3,D4} };
	for (int i =0;i<4;i++) 
	{
		for (int j = 0; j < 4; j++) 
		{
			forquad(A[i][j].X, A[i][j].Y, A[i][j].Z, 0.3, 2);
		}
	}
	float X, Y, Z, X1, Y1, Z1, X2, Y2, Z2,X3,Y3,Z3;
		 glColor3f(1, 1, 1);
		 glPointSize(3);
		 glBegin(GL_QUADS);
		for(float u=0;u<1;u+=0.1)
		{
			for (float w = 0; w<1; w += 0.1)
			{//u=0,w=3
				X = pow(1 - u, 3)*pow(1 - w, 3)*A[0][0].X + pow(1 - u, 3) * 3 * w*pow(1 - w, 2)*A[0][1].X + pow(1 - u, 3) * 3 * pow(w, 2)*(1 - w)*A[0][2].X + pow(1 -	 u, 3)*pow(w, 3)*A[0][3].X + /*u=1 w=0*/3 * u*pow(1 - u, 2)*pow(1 - w, 3)*A[1][0].X + 3 * u*pow(1 - u, 2) * 3 * w*pow(1 - w, 2)*A[1][1].X + 3 * u*pow(1 - u, 2) * 3 * pow(w, 2)*(1 - w)*A[1][2].X + 3 * u*pow(1 - u, 2)* pow(w, 3)*A[1][3].X/*u=2 w=0*/ + 3 * pow(u, 2)*(1 - u)*pow(1 - w, 3)*A[2][0].X + 3 * pow(u, 2)*(1 - u) * 3 * w*pow(1 - w, 2)*A[2][1].X + 3 * pow(u, 2)*(1 - u) * 3 * pow(w, 2)*(1 - w)*A[2][2].X + 3 * pow(u, 2)*(1 - u)*pow(w, 3)*A[2][3].X + /*u=3 w=0*/pow(u, 3)*pow(1 - w, 3)*A[3][0].X + pow(u, 3) * 3 * w*pow(1 - w, 2)*A[3][1].X + pow(u, 3) * 3 * pow(w, 2)*(1 - w)*A[3][2].X + pow(u, 3)*pow(w, 3)*A[3][3].X;
				Y = pow(1 - u, 3)*pow(1 - w, 3)*A[0][0].Y + pow(1 - u, 3) * 3 * w*pow(1 - w, 2)*A[0][1].Y + pow(1 - u, 3) * 3 * pow(w, 2)*(1 - w)*A[0][2].Y + pow(1 - u, 3)*pow(w, 3)*A[0][3].Y + /*u=1 w=0*/3 * u*pow(1 - u, 2)*pow(1 - w, 3)*A[1][0].Y + 3 * u*pow(1 - u, 2) * 3 * w*pow(1 - w, 2)*A[1][1].Y + 3 * u*pow(1 - u, 2) * 3 * pow(w, 2)*(1 - w)*A[1][2].Y + 3 * u*pow(1 - u, 2)* pow(w, 3)*A[1][3].Y/*u=2 w=0*/ + 3 * pow(u, 2)*(1 - u)*pow(1 - w, 3)*A[2][0].Y + 3 * pow(u, 2)*(1 - u) * 3 * w*pow(1 - w, 2)*A[2][1].Y + 3 * pow(u, 2)*(1 - u) * 3 * pow(w, 2)*(1 - w)*A[2][2].Y + 3 * pow(u, 2)*(1-u)*pow(w, 3)*A[2][3].Y + /*u=3 w=0*/pow(u, 3)*pow(1 - w, 3)*A[3][0].Y + pow(u, 3) * 3 * w*pow(1 - w, 2)*A[3][1].Y + pow(u, 3) * 3 * pow(w, 2)*(1 - w)*A[3][2].Y + pow(u, 3)*pow(w, 3)*A[3][3].Y;
				Z = pow(1 - u, 3)*pow(1 - w, 3)*A[0][0].Z + pow(1 - u, 3) * 3 * w*pow(1 - w, 2)*A[0][1].Z + pow(1 - u, 3) * 3 * pow(w, 2)*(1 - w)*A[0][2].Z + pow(1 - u, 3)*pow(w, 3)*A[0][3].Z + /*u=1 w=0*/3 * u*pow(1 - u, 2)*pow(1 - w, 3)*A[1][0].Z + 3 * u*pow(1 - u, 2) * 3 * w*pow(1 - w, 2)*A[1][1].Z + 3 * u*pow(1 - u, 2) * 3 * pow(w, 2)*(1 - w)*A[1][2].Z + 3 * u*pow(1 - u, 2)* pow(w, 3)*A[1][3].Z/*u=2 w=0*/ + 3 * pow(u, 2)*(1 - u)*pow(1 - w, 3)*A[2][0].Z + 3 * pow(u, 2)*(1 - u) * 3 * w*pow(1 - w, 2)*A[2][1].Z + 3 * pow(u, 2)*(1 - u) * 3 * pow(w, 2)*(1 - w)*A[2][2].Z + 3 * pow(u, 2)*(1 - u)*pow(w, 3)*A[2][3].Z + /*u=3 w=0*/pow(u, 3)*pow(1 - w, 3)*A[3][0].Z + pow(u, 3) * 3 * w*pow(1 - w, 2)*A[3][1].Z + pow(u, 3) * 3 * pow(w, 2)*(1 - w)*A[3][2].Z + pow(u, 3)*pow(w, 3)*A[3][3].Z;
				
				u += 0.1;
				X1 = pow(1 - u, 3)*pow(1 - w, 3)*A[0][0].X + pow(1 - u, 3) * 3 * w*pow(1 - w, 2)*A[0][1].X + pow(1 - u, 3) * 3 * pow(w, 2)*(1 - w)*A[0][2].X + pow(1 - u, 3)*pow(w, 3)*A[0][3].X + /*u=1 w=0*/3 * u*pow(1 - u, 2)*pow(1 - w, 3)*A[1][0].X + 3 * u*pow(1 - u, 2) * 3 * w*pow(1 - w, 2)*A[1][1].X + 3 * u*pow(1 - u, 2) * 3 * pow(w, 2)*(1 - w)*A[1][2].X + 3 * u*pow(1 - u, 2)* pow(w, 3)*A[1][3].X/*u=2 w=0*/ + 3 * pow(u, 2)*(1 - u)*pow(1 - w, 3)*A[2][0].X + 3 * pow(u, 2)*(1 - u) * 3 * w*pow(1 - w, 2)*A[2][1].X + 3 * pow(u, 2)*(1 - u) * 3 * pow(w, 2)*(1 - w)*A[2][2].X + 3 * pow(u, 2)*(1 - u)*pow(w, 3)*A[2][3].X + /*u=3 w=0*/pow(u, 3)*pow(1 - w, 3)*A[3][0].X + pow(u, 3) * 3 * w*pow(1 - w, 2)*A[3][1].X + pow(u, 3) * 3 * pow(w, 2)*(1 - w)*A[3][2].X + pow(u, 3)*pow(w, 3)*A[3][3].X;
				Y1 = pow(1 - u, 3)*pow(1 - w, 3)*A[0][0].Y + pow(1 - u, 3) * 3 * w*pow(1 - w, 2)*A[0][1].Y + pow(1 - u, 3) * 3 * pow(w, 2)*(1 - w)*A[0][2].Y + pow(1 - u, 3)*pow(w, 3)*A[0][3].Y + /*u=1 w=0*/3 * u*pow(1 - u, 2)*pow(1 - w, 3)*A[1][0].Y + 3 * u*pow(1 - u, 2) * 3 * w*pow(1 - w, 2)*A[1][1].Y + 3 * u*pow(1 - u, 2) * 3 * pow(w, 2)*(1 - w)*A[1][2].Y + 3 * u*pow(1 - u, 2)* pow(w, 3)*A[1][3].Y/*u=2 w=0*/ + 3 * pow(u, 2)*(1 - u)*pow(1 - w, 3)*A[2][0].Y + 3 * pow(u, 2)*(1 - u) * 3 * w*pow(1 - w, 2)*A[2][1].Y + 3 * pow(u, 2)*(1 - u) * 3 * pow(w, 2)*(1 - w)*A[2][2].Y + 3 * pow(u, 2)*(1 - u)*pow(w, 3)*A[2][3].Y + /*u=3 w=0*/pow(u, 3)*pow(1 - w, 3)*A[3][0].Y + pow(u, 3) * 3 * w*pow(1 - w, 2)*A[3][1].Y + pow(u, 3) * 3 * pow(w, 2)*(1 - w)*A[3][2].Y + pow(u, 3)*pow(w, 3)*A[3][3].Y;
				Z1 = pow(1 - u, 3)*pow(1 - w, 3)*A[0][0].Z + pow(1 - u, 3) * 3 * w*pow(1 - w, 2)*A[0][1].Z + pow(1 - u, 3) * 3 * pow(w, 2)*(1 - w)*A[0][2].Z + pow(1 - u, 3)*pow(w, 3)*A[0][3].Z + /*u=1 w=0*/3 * u*pow(1 - u, 2)*pow(1 - w, 3)*A[1][0].Z + 3 * u*pow(1 - u, 2) * 3 * w*pow(1 - w, 2)*A[1][1].Z + 3 * u*pow(1 - u, 2) * 3 * pow(w, 2)*(1 - w)*A[1][2].Z + 3 * u*pow(1 - u, 2)* pow(w, 3)*A[1][3].Z/*u=2 w=0*/ + 3 * pow(u, 2)*(1 - u)*pow(1 - w, 3)*A[2][0].Z + 3 * pow(u, 2)*(1 - u) * 3 * w*pow(1 - w, 2)*A[2][1].Z + 3 * pow(u, 2)*(1 - u) * 3 * pow(w, 2)*(1 - w)*A[2][2].Z + 3 * pow(u, 2)*(1 - u)*pow(w, 3)*A[2][3].Z + /*u=3 w=0*/pow(u, 3)*pow(1 - w, 3)*A[3][0].Z + pow(u, 3) * 3 * w*pow(1 - w, 2)*A[3][1].Z + pow(u, 3) * 3 * pow(w, 2)*(1 - w)*A[3][2].Z + pow(u, 3)*pow(w, 3)*A[3][3].Z;
			
			
				w += 0.1;
				X2 = pow(1 - u, 3)*pow(1 - w, 3)*A[0][0].X + pow(1 - u, 3) * 3 * w*pow(1 - w, 2)*A[0][1].X + pow(1 - u, 3) * 3 * pow(w, 2)*(1 - w)*A[0][2].X + pow(1 - u, 3)*pow(w, 3)*A[0][3].X + /*u=1 w=0*/3 * u*pow(1 - u, 2)*pow(1 - w, 3)*A[1][0].X + 3 * u*pow(1 - u, 2) * 3 * w*pow(1 - w, 2)*A[1][1].X + 3 * u*pow(1 - u, 2) * 3 * pow(w, 2)*(1 - w)*A[1][2].X + 3 * u*pow(1 - u, 2)* pow(w, 3)*A[1][3].X/*u=2 w=0*/ + 3 * pow(u, 2)*(1 - u)*pow(1 - w, 3)*A[2][0].X + 3 * pow(u, 2)*(1 - u) * 3 * w*pow(1 - w, 2)*A[2][1].X + 3 * pow(u, 2)*(1 - u) * 3 * pow(w, 2)*(1 - w)*A[2][2].X + 3 * pow(u, 2)*(1 - u)*pow(w, 3)*A[2][3].X + /*u=3 w=0*/pow(u, 3)*pow(1 - w, 3)*A[3][0].X + pow(u, 3) * 3 * w*pow(1 - w, 2)*A[3][1].X + pow(u, 3) * 3 * pow(w, 2)*(1 - w)*A[3][2].X + pow(u, 3)*pow(w, 3)*A[3][3].X;
				Y2 = pow(1 - u, 3)*pow(1 - w, 3)*A[0][0].Y + pow(1 - u, 3) * 3 * w*pow(1 - w, 2)*A[0][1].Y + pow(1 - u, 3) * 3 * pow(w, 2)*(1 - w)*A[0][2].Y + pow(1 - u, 3)*pow(w, 3)*A[0][3].Y + /*u=1 w=0*/3 * u*pow(1 - u, 2)*pow(1 - w, 3)*A[1][0].Y + 3 * u*pow(1 - u, 2) * 3 * w*pow(1 - w, 2)*A[1][1].Y + 3 * u*pow(1 - u, 2) * 3 * pow(w, 2)*(1 - w)*A[1][2].Y + 3 * u*pow(1 - u, 2)* pow(w, 3)*A[1][3].Y/*u=2 w=0*/ + 3 * pow(u, 2)*(1 - u)*pow(1 - w, 3)*A[2][0].Y + 3 * pow(u, 2)*(1 - u) * 3 * w*pow(1 - w, 2)*A[2][1].Y + 3 * pow(u, 2)*(1 - u) * 3 * pow(w, 2)*(1 - w)*A[2][2].Y + 3 * pow(u, 2)*(1 - u)*pow(w, 3)*A[2][3].Y + /*u=3 w=0*/pow(u, 3)*pow(1 - w, 3)*A[3][0].Y + pow(u, 3) * 3 * w*pow(1 - w, 2)*A[3][1].Y + pow(u, 3) * 3 * pow(w, 2)*(1 - w)*A[3][2].Y + pow(u, 3)*pow(w, 3)*A[3][3].Y;
				Z2 = pow(1 - u, 3)*pow(1 - w, 3)*A[0][0].Z + pow(1 - u, 3) * 3 * w*pow(1 - w, 2)*A[0][1].Z + pow(1 - u, 3) * 3 * pow(w, 2)*(1 - w)*A[0][2].Z + pow(1 - u, 3)*pow(w, 3)*A[0][3].Z + /*u=1 w=0*/3 * u*pow(1 - u, 2)*pow(1 - w, 3)*A[1][0].Z + 3 * u*pow(1 - u, 2) * 3 * w*pow(1 - w, 2)*A[1][1].Z + 3 * u*pow(1 - u, 2) * 3 * pow(w, 2)*(1 - w)*A[1][2].Z + 3 * u*pow(1 - u, 2)* pow(w, 3)*A[1][3].Z/*u=2 w=0*/ + 3 * pow(u, 2)*(1 - u)*pow(1 - w, 3)*A[2][0].Z + 3 * pow(u, 2)*(1 - u) * 3 * w*pow(1 - w, 2)*A[2][1].Z + 3 * pow(u, 2)*(1 - u) * 3 * pow(w, 2)*(1 - w)*A[2][2].Z + 3 * pow(u, 2)*(1 - u)*pow(w, 3)*A[2][3].Z + /*u=3 w=0*/pow(u, 3)*pow(1 - w, 3)*A[3][0].Z + pow(u, 3) * 3 * w*pow(1 - w, 2)*A[3][1].Z + pow(u, 3) * 3 * pow(w, 2)*(1 - w)*A[3][2].Z + pow(u, 3)*pow(w, 3)*A[3][3].Z;
				
				
				u -= 0.1;
				X3 = pow(1 - u, 3)*pow(1 - w, 3)*A[0][0].X + pow(1 - u, 3) * 3 * w*pow(1 - w, 2)*A[0][1].X + pow(1 - u, 3) * 3 * pow(w, 2)*(1 - w)*A[0][2].X + pow(1 - u, 3)*pow(w, 3)*A[0][3].X + /*u=1 w=0*/3 * u*pow(1 - u, 2)*pow(1 - w, 3)*A[1][0].X + 3 * u*pow(1 - u, 2) * 3 * w*pow(1 - w, 2)*A[1][1].X + 3 * u*pow(1 - u, 2) * 3 * pow(w, 2)*(1 - w)*A[1][2].X + 3 * u*pow(1 - u, 2)* pow(w, 3)*A[1][3].X/*u=2 w=0*/ + 3 * pow(u, 2)*(1 - u)*pow(1 - w, 3)*A[2][0].X + 3 * pow(u, 2)*(1 - u) * 3 * w*pow(1 - w, 2)*A[2][1].X + 3 * pow(u, 2)*(1 - u) * 3 * pow(w, 2)*(1 - w)*A[2][2].X + 3 * pow(u, 2)*(1 - u)*pow(w, 3)*A[2][3].X + /*u=3 w=0*/pow(u, 3)*pow(1 - w, 3)*A[3][0].X + pow(u, 3) * 3 * w*pow(1 - w, 2)*A[3][1].X + pow(u, 3) * 3 * pow(w, 2)*(1 - w)*A[3][2].X + pow(u, 3)*pow(w, 3)*A[3][3].X;
				Y3 = pow(1 - u, 3)*pow(1 - w, 3)*A[0][0].Y + pow(1 - u, 3) * 3 * w*pow(1 - w, 2)*A[0][1].Y + pow(1 - u, 3) * 3 * pow(w, 2)*(1 - w)*A[0][2].Y + pow(1 - u, 3)*pow(w, 3)*A[0][3].Y + /*u=1 w=0*/3 * u*pow(1 - u, 2)*pow(1 - w, 3)*A[1][0].Y + 3 * u*pow(1 - u, 2) * 3 * w*pow(1 - w, 2)*A[1][1].Y + 3 * u*pow(1 - u, 2) * 3 * pow(w, 2)*(1 - w)*A[1][2].Y + 3 * u*pow(1 - u, 2)* pow(w, 3)*A[1][3].Y/*u=2 w=0*/ + 3 * pow(u, 2)*(1 - u)*pow(1 - w, 3)*A[2][0].Y + 3 * pow(u, 2)*(1 - u) * 3 * w*pow(1 - w, 2)*A[2][1].Y + 3 * pow(u, 2)*(1 - u) * 3 * pow(w, 2)*(1 - w)*A[2][2].Y + 3 * pow(u, 2)*(1 - u)*pow(w, 3)*A[2][3].Y + /*u=3 w=0*/pow(u, 3)*pow(1 - w, 3)*A[3][0].Y + pow(u, 3) * 3 * w*pow(1 - w, 2)*A[3][1].Y + pow(u, 3) * 3 * pow(w, 2)*(1 - w)*A[3][2].Y + pow(u, 3)*pow(w, 3)*A[3][3].Y;
				Z3 = pow(1 - u, 3)*pow(1 - w, 3)*A[0][0].Z + pow(1 - u, 3) * 3 * w*pow(1 - w, 2)*A[0][1].Z + pow(1 - u, 3) * 3 * pow(w, 2)*(1 - w)*A[0][2].Z + pow(1 - u, 3)*pow(w, 3)*A[0][3].Z + /*u=1 w=0*/3 * u*pow(1 - u, 2)*pow(1 - w, 3)*A[1][0].Z + 3 * u*pow(1 - u, 2) * 3 * w*pow(1 - w, 2)*A[1][1].Z + 3 * u*pow(1 - u, 2) * 3 * pow(w, 2)*(1 - w)*A[1][2].Z + 3 * u*pow(1 - u, 2)* pow(w, 3)*A[1][3].Z/*u=2 w=0*/ + 3 * pow(u, 2)*(1 - u)*pow(1 - w, 3)*A[2][0].Z + 3 * pow(u, 2)*(1 - u) * 3 * w*pow(1 - w, 2)*A[2][1].Z + 3 * pow(u, 2)*(1 - u) * 3 * pow(w, 2)*(1 - w)*A[2][2].Z + 3 * pow(u, 2)*(1 - u)*pow(w, 3)*A[2][3].Z + /*u=3 w=0*/pow(u, 3)*pow(1 - w, 3)*A[3][0].Z + pow(u, 3) * 3 * w*pow(1 - w, 2)*A[3][1].Z + pow(u, 3) * 3 * pow(w, 2)*(1 - w)*A[3][2].Z + pow(u, 3)*pow(w, 3)*A[3][3].Z;
		
				w -= 0.1;
				 normnormal(X, Y, Z, X3, Y3, Z3, X1, Y1, Z1);
				glVertex3f(X, Y, Z);
				//normnormal(X, Y, Z, X, Y, Z, X2, Y2, Z2);
				glVertex3f(X1, Y1, Z1);
				//normnormal(X, Y, Z, X1, Y1, Z1, X3, Y3, Z3);
				glVertex3f(X2, Y2, Z2);
				//normnormal(X, Y, Z, X, Y, Z, X2, Y2, Z2);
				glVertex3f(X3, Y3, Z3);
				
			}

		}
		glEnd();

}
void poverhnostbezeforgun() 
{
	point A1(-200, 140, 0);  point B1(-100, 80, 0);	   point C1(100, 80, 0); point D1(200, 140, 0);
	point A2(-200, 160, 30); point B2(-100, 160, 140); point C2(100, 160, 140); point D2(200, 160, 30);
	point A3(-200, 220, 30); point B3(-100, 220, 140); point C3(100, 220, 140); point  D3(200, 220, 30);
	point A4(-200, 280, 0); point B4(-100, 300, 0); point C4(100, 300, 0); point D4(200, 280, 0);
	point A[4][4] = { { A1,A2,A3,A4 },
	{ B1,B2,B3,B4 },
	{ C1,C2,C3,C4 },
	{ D1,D2,D3,D4 } };
	float X, Y, Z, X1, Y1, Z1, X2, Y2, Z2, X3, Y3, Z3;
	glBegin(GL_QUADS);
	for (float u = 0; u<1; u += 0.1)
	{
		for (float w = 0; w<1; w += 0.1)
		{//u=0,w=3
			X = pow(1 - u, 3)*pow(1 - w, 3)*A[0][0].X + pow(1 - u, 3) * 3 * w*pow(1 - w, 2)*A[0][1].X + pow(1 - u, 3) * 3 * pow(w, 2)*(1 - w)*A[0][2].X + pow(1 - u, 3)*pow(w, 3)*A[0][3].X + /*u=1 w=0*/3 * u*pow(1 - u, 2)*pow(1 - w, 3)*A[1][0].X + 3 * u*pow(1 - u, 2) * 3 * w*pow(1 - w, 2)*A[1][1].X + 3 * u*pow(1 - u, 2) * 3 * pow(w, 2)*(1 - w)*A[1][2].X + 3 * u*pow(1 - u, 2)* pow(w, 3)*A[1][3].X/*u=2 w=0*/ + 3 * pow(u, 2)*(1 - u)*pow(1 - w, 3)*A[2][0].X + 3 * pow(u, 2)*(1 - u) * 3 * w*pow(1 - w, 2)*A[2][1].X + 3 * pow(u, 2)*(1 - u) * 3 * pow(w, 2)*(1 - w)*A[2][2].X + 3 * pow(u, 2)*(1 - u)*pow(w, 3)*A[2][3].X + /*u=3 w=0*/pow(u, 3)*pow(1 - w, 3)*A[3][0].X + pow(u, 3) * 3 * w*pow(1 - w, 2)*A[3][1].X + pow(u, 3) * 3 * pow(w, 2)*(1 - w)*A[3][2].X + pow(u, 3)*pow(w, 3)*A[3][3].X;
			Y = pow(1 - u, 3)*pow(1 - w, 3)*A[0][0].Y + pow(1 - u, 3) * 3 * w*pow(1 - w, 2)*A[0][1].Y + pow(1 - u, 3) * 3 * pow(w, 2)*(1 - w)*A[0][2].Y + pow(1 - u, 3)*pow(w, 3)*A[0][3].Y + /*u=1 w=0*/3 * u*pow(1 - u, 2)*pow(1 - w, 3)*A[1][0].Y + 3 * u*pow(1 - u, 2) * 3 * w*pow(1 - w, 2)*A[1][1].Y + 3 * u*pow(1 - u, 2) * 3 * pow(w, 2)*(1 - w)*A[1][2].Y + 3 * u*pow(1 - u, 2)* pow(w, 3)*A[1][3].Y/*u=2 w=0*/ + 3 * pow(u, 2)*(1 - u)*pow(1 - w, 3)*A[2][0].Y + 3 * pow(u, 2)*(1 - u) * 3 * w*pow(1 - w, 2)*A[2][1].Y + 3 * pow(u, 2)*(1 - u) * 3 * pow(w, 2)*(1 - w)*A[2][2].Y + 3 * pow(u, 2)*(1 - u)*pow(w, 3)*A[2][3].Y + /*u=3 w=0*/pow(u, 3)*pow(1 - w, 3)*A[3][0].Y + pow(u, 3) * 3 * w*pow(1 - w, 2)*A[3][1].Y + pow(u, 3) * 3 * pow(w, 2)*(1 - w)*A[3][2].Y + pow(u, 3)*pow(w, 3)*A[3][3].Y;
			Z = pow(1 - u, 3)*pow(1 - w, 3)*A[0][0].Z + pow(1 - u, 3) * 3 * w*pow(1 - w, 2)*A[0][1].Z + pow(1 - u, 3) * 3 * pow(w, 2)*(1 - w)*A[0][2].Z + pow(1 - u, 3)*pow(w, 3)*A[0][3].Z + /*u=1 w=0*/3 * u*pow(1 - u, 2)*pow(1 - w, 3)*A[1][0].Z + 3 * u*pow(1 - u, 2) * 3 * w*pow(1 - w, 2)*A[1][1].Z + 3 * u*pow(1 - u, 2) * 3 * pow(w, 2)*(1 - w)*A[1][2].Z + 3 * u*pow(1 - u, 2)* pow(w, 3)*A[1][3].Z/*u=2 w=0*/ + 3 * pow(u, 2)*(1 - u)*pow(1 - w, 3)*A[2][0].Z + 3 * pow(u, 2)*(1 - u) * 3 * w*pow(1 - w, 2)*A[2][1].Z + 3 * pow(u, 2)*(1 - u) * 3 * pow(w, 2)*(1 - w)*A[2][2].Z + 3 * pow(u, 2)*(1 - u)*pow(w, 3)*A[2][3].Z + /*u=3 w=0*/pow(u, 3)*pow(1 - w, 3)*A[3][0].Z + pow(u, 3) * 3 * w*pow(1 - w, 2)*A[3][1].Z + pow(u, 3) * 3 * pow(w, 2)*(1 - w)*A[3][2].Z + pow(u, 3)*pow(w, 3)*A[3][3].Z;

			u += 0.1;
			X1 = pow(1 - u, 3)*pow(1 - w, 3)*A[0][0].X + pow(1 - u, 3) * 3 * w*pow(1 - w, 2)*A[0][1].X + pow(1 - u, 3) * 3 * pow(w, 2)*(1 - w)*A[0][2].X + pow(1 - u, 3)*pow(w, 3)*A[0][3].X + /*u=1 w=0*/3 * u*pow(1 - u, 2)*pow(1 - w, 3)*A[1][0].X + 3 * u*pow(1 - u, 2) * 3 * w*pow(1 - w, 2)*A[1][1].X + 3 * u*pow(1 - u, 2) * 3 * pow(w, 2)*(1 - w)*A[1][2].X + 3 * u*pow(1 - u, 2)* pow(w, 3)*A[1][3].X/*u=2 w=0*/ + 3 * pow(u, 2)*(1 - u)*pow(1 - w, 3)*A[2][0].X + 3 * pow(u, 2)*(1 - u) * 3 * w*pow(1 - w, 2)*A[2][1].X + 3 * pow(u, 2)*(1 - u) * 3 * pow(w, 2)*(1 - w)*A[2][2].X + 3 * pow(u, 2)*(1 - u)*pow(w, 3)*A[2][3].X + /*u=3 w=0*/pow(u, 3)*pow(1 - w, 3)*A[3][0].X + pow(u, 3) * 3 * w*pow(1 - w, 2)*A[3][1].X + pow(u, 3) * 3 * pow(w, 2)*(1 - w)*A[3][2].X + pow(u, 3)*pow(w, 3)*A[3][3].X;
			Y1 = pow(1 - u, 3)*pow(1 - w, 3)*A[0][0].Y + pow(1 - u, 3) * 3 * w*pow(1 - w, 2)*A[0][1].Y + pow(1 - u, 3) * 3 * pow(w, 2)*(1 - w)*A[0][2].Y + pow(1 - u, 3)*pow(w, 3)*A[0][3].Y + /*u=1 w=0*/3 * u*pow(1 - u, 2)*pow(1 - w, 3)*A[1][0].Y + 3 * u*pow(1 - u, 2) * 3 * w*pow(1 - w, 2)*A[1][1].Y + 3 * u*pow(1 - u, 2) * 3 * pow(w, 2)*(1 - w)*A[1][2].Y + 3 * u*pow(1 - u, 2)* pow(w, 3)*A[1][3].Y/*u=2 w=0*/ + 3 * pow(u, 2)*(1 - u)*pow(1 - w, 3)*A[2][0].Y + 3 * pow(u, 2)*(1 - u) * 3 * w*pow(1 - w, 2)*A[2][1].Y + 3 * pow(u, 2)*(1 - u) * 3 * pow(w, 2)*(1 - w)*A[2][2].Y + 3 * pow(u, 2)*(1 - u)*pow(w, 3)*A[2][3].Y + /*u=3 w=0*/pow(u, 3)*pow(1 - w, 3)*A[3][0].Y + pow(u, 3) * 3 * w*pow(1 - w, 2)*A[3][1].Y + pow(u, 3) * 3 * pow(w, 2)*(1 - w)*A[3][2].Y + pow(u, 3)*pow(w, 3)*A[3][3].Y;
			Z1 = pow(1 - u, 3)*pow(1 - w, 3)*A[0][0].Z + pow(1 - u, 3) * 3 * w*pow(1 - w, 2)*A[0][1].Z + pow(1 - u, 3) * 3 * pow(w, 2)*(1 - w)*A[0][2].Z + pow(1 - u, 3)*pow(w, 3)*A[0][3].Z + /*u=1 w=0*/3 * u*pow(1 - u, 2)*pow(1 - w, 3)*A[1][0].Z + 3 * u*pow(1 - u, 2) * 3 * w*pow(1 - w, 2)*A[1][1].Z + 3 * u*pow(1 - u, 2) * 3 * pow(w, 2)*(1 - w)*A[1][2].Z + 3 * u*pow(1 - u, 2)* pow(w, 3)*A[1][3].Z/*u=2 w=0*/ + 3 * pow(u, 2)*(1 - u)*pow(1 - w, 3)*A[2][0].Z + 3 * pow(u, 2)*(1 - u) * 3 * w*pow(1 - w, 2)*A[2][1].Z + 3 * pow(u, 2)*(1 - u) * 3 * pow(w, 2)*(1 - w)*A[2][2].Z + 3 * pow(u, 2)*(1 - u)*pow(w, 3)*A[2][3].Z + /*u=3 w=0*/pow(u, 3)*pow(1 - w, 3)*A[3][0].Z + pow(u, 3) * 3 * w*pow(1 - w, 2)*A[3][1].Z + pow(u, 3) * 3 * pow(w, 2)*(1 - w)*A[3][2].Z + pow(u, 3)*pow(w, 3)*A[3][3].Z;


			w += 0.1;
			X2 = pow(1 - u, 3)*pow(1 - w, 3)*A[0][0].X + pow(1 - u, 3) * 3 * w*pow(1 - w, 2)*A[0][1].X + pow(1 - u, 3) * 3 * pow(w, 2)*(1 - w)*A[0][2].X + pow(1 - u, 3)*pow(w, 3)*A[0][3].X + /*u=1 w=0*/3 * u*pow(1 - u, 2)*pow(1 - w, 3)*A[1][0].X + 3 * u*pow(1 - u, 2) * 3 * w*pow(1 - w, 2)*A[1][1].X + 3 * u*pow(1 - u, 2) * 3 * pow(w, 2)*(1 - w)*A[1][2].X + 3 * u*pow(1 - u, 2)* pow(w, 3)*A[1][3].X/*u=2 w=0*/ + 3 * pow(u, 2)*(1 - u)*pow(1 - w, 3)*A[2][0].X + 3 * pow(u, 2)*(1 - u) * 3 * w*pow(1 - w, 2)*A[2][1].X + 3 * pow(u, 2)*(1 - u) * 3 * pow(w, 2)*(1 - w)*A[2][2].X + 3 * pow(u, 2)*(1 - u)*pow(w, 3)*A[2][3].X + /*u=3 w=0*/pow(u, 3)*pow(1 - w, 3)*A[3][0].X + pow(u, 3) * 3 * w*pow(1 - w, 2)*A[3][1].X + pow(u, 3) * 3 * pow(w, 2)*(1 - w)*A[3][2].X + pow(u, 3)*pow(w, 3)*A[3][3].X;
			Y2 = pow(1 - u, 3)*pow(1 - w, 3)*A[0][0].Y + pow(1 - u, 3) * 3 * w*pow(1 - w, 2)*A[0][1].Y + pow(1 - u, 3) * 3 * pow(w, 2)*(1 - w)*A[0][2].Y + pow(1 - u, 3)*pow(w, 3)*A[0][3].Y + /*u=1 w=0*/3 * u*pow(1 - u, 2)*pow(1 - w, 3)*A[1][0].Y + 3 * u*pow(1 - u, 2) * 3 * w*pow(1 - w, 2)*A[1][1].Y + 3 * u*pow(1 - u, 2) * 3 * pow(w, 2)*(1 - w)*A[1][2].Y + 3 * u*pow(1 - u, 2)* pow(w, 3)*A[1][3].Y/*u=2 w=0*/ + 3 * pow(u, 2)*(1 - u)*pow(1 - w, 3)*A[2][0].Y + 3 * pow(u, 2)*(1 - u) * 3 * w*pow(1 - w, 2)*A[2][1].Y + 3 * pow(u, 2)*(1 - u) * 3 * pow(w, 2)*(1 - w)*A[2][2].Y + 3 * pow(u, 2)*(1 - u)*pow(w, 3)*A[2][3].Y + /*u=3 w=0*/pow(u, 3)*pow(1 - w, 3)*A[3][0].Y + pow(u, 3) * 3 * w*pow(1 - w, 2)*A[3][1].Y + pow(u, 3) * 3 * pow(w, 2)*(1 - w)*A[3][2].Y + pow(u, 3)*pow(w, 3)*A[3][3].Y;
			Z2 = pow(1 - u, 3)*pow(1 - w, 3)*A[0][0].Z + pow(1 - u, 3) * 3 * w*pow(1 - w, 2)*A[0][1].Z + pow(1 - u, 3) * 3 * pow(w, 2)*(1 - w)*A[0][2].Z + pow(1 - u, 3)*pow(w, 3)*A[0][3].Z + /*u=1 w=0*/3 * u*pow(1 - u, 2)*pow(1 - w, 3)*A[1][0].Z + 3 * u*pow(1 - u, 2) * 3 * w*pow(1 - w, 2)*A[1][1].Z + 3 * u*pow(1 - u, 2) * 3 * pow(w, 2)*(1 - w)*A[1][2].Z + 3 * u*pow(1 - u, 2)* pow(w, 3)*A[1][3].Z/*u=2 w=0*/ + 3 * pow(u, 2)*(1 - u)*pow(1 - w, 3)*A[2][0].Z + 3 * pow(u, 2)*(1 - u) * 3 * w*pow(1 - w, 2)*A[2][1].Z + 3 * pow(u, 2)*(1 - u) * 3 * pow(w, 2)*(1 - w)*A[2][2].Z + 3 * pow(u, 2)*(1 - u)*pow(w, 3)*A[2][3].Z + /*u=3 w=0*/pow(u, 3)*pow(1 - w, 3)*A[3][0].Z + pow(u, 3) * 3 * w*pow(1 - w, 2)*A[3][1].Z + pow(u, 3) * 3 * pow(w, 2)*(1 - w)*A[3][2].Z + pow(u, 3)*pow(w, 3)*A[3][3].Z;


			u -= 0.1;
			X3 = pow(1 - u, 3)*pow(1 - w, 3)*A[0][0].X + pow(1 - u, 3) * 3 * w*pow(1 - w, 2)*A[0][1].X + pow(1 - u, 3) * 3 * pow(w, 2)*(1 - w)*A[0][2].X + pow(1 - u, 3)*pow(w, 3)*A[0][3].X + /*u=1 w=0*/3 * u*pow(1 - u, 2)*pow(1 - w, 3)*A[1][0].X + 3 * u*pow(1 - u, 2) * 3 * w*pow(1 - w, 2)*A[1][1].X + 3 * u*pow(1 - u, 2) * 3 * pow(w, 2)*(1 - w)*A[1][2].X + 3 * u*pow(1 - u, 2)* pow(w, 3)*A[1][3].X/*u=2 w=0*/ + 3 * pow(u, 2)*(1 - u)*pow(1 - w, 3)*A[2][0].X + 3 * pow(u, 2)*(1 - u) * 3 * w*pow(1 - w, 2)*A[2][1].X + 3 * pow(u, 2)*(1 - u) * 3 * pow(w, 2)*(1 - w)*A[2][2].X + 3 * pow(u, 2)*(1 - u)*pow(w, 3)*A[2][3].X + /*u=3 w=0*/pow(u, 3)*pow(1 - w, 3)*A[3][0].X + pow(u, 3) * 3 * w*pow(1 - w, 2)*A[3][1].X + pow(u, 3) * 3 * pow(w, 2)*(1 - w)*A[3][2].X + pow(u, 3)*pow(w, 3)*A[3][3].X;
			Y3 = pow(1 - u, 3)*pow(1 - w, 3)*A[0][0].Y + pow(1 - u, 3) * 3 * w*pow(1 - w, 2)*A[0][1].Y + pow(1 - u, 3) * 3 * pow(w, 2)*(1 - w)*A[0][2].Y + pow(1 - u, 3)*pow(w, 3)*A[0][3].Y + /*u=1 w=0*/3 * u*pow(1 - u, 2)*pow(1 - w, 3)*A[1][0].Y + 3 * u*pow(1 - u, 2) * 3 * w*pow(1 - w, 2)*A[1][1].Y + 3 * u*pow(1 - u, 2) * 3 * pow(w, 2)*(1 - w)*A[1][2].Y + 3 * u*pow(1 - u, 2)* pow(w, 3)*A[1][3].Y/*u=2 w=0*/ + 3 * pow(u, 2)*(1 - u)*pow(1 - w, 3)*A[2][0].Y + 3 * pow(u, 2)*(1 - u) * 3 * w*pow(1 - w, 2)*A[2][1].Y + 3 * pow(u, 2)*(1 - u) * 3 * pow(w, 2)*(1 - w)*A[2][2].Y + 3 * pow(u, 2)*(1 - u)*pow(w, 3)*A[2][3].Y + /*u=3 w=0*/pow(u, 3)*pow(1 - w, 3)*A[3][0].Y + pow(u, 3) * 3 * w*pow(1 - w, 2)*A[3][1].Y + pow(u, 3) * 3 * pow(w, 2)*(1 - w)*A[3][2].Y + pow(u, 3)*pow(w, 3)*A[3][3].Y;
			Z3 = pow(1 - u, 3)*pow(1 - w, 3)*A[0][0].Z + pow(1 - u, 3) * 3 * w*pow(1 - w, 2)*A[0][1].Z + pow(1 - u, 3) * 3 * pow(w, 2)*(1 - w)*A[0][2].Z + pow(1 - u, 3)*pow(w, 3)*A[0][3].Z + /*u=1 w=0*/3 * u*pow(1 - u, 2)*pow(1 - w, 3)*A[1][0].Z + 3 * u*pow(1 - u, 2) * 3 * w*pow(1 - w, 2)*A[1][1].Z + 3 * u*pow(1 - u, 2) * 3 * pow(w, 2)*(1 - w)*A[1][2].Z + 3 * u*pow(1 - u, 2)* pow(w, 3)*A[1][3].Z/*u=2 w=0*/ + 3 * pow(u, 2)*(1 - u)*pow(1 - w, 3)*A[2][0].Z + 3 * pow(u, 2)*(1 - u) * 3 * w*pow(1 - w, 2)*A[2][1].Z + 3 * pow(u, 2)*(1 - u) * 3 * pow(w, 2)*(1 - w)*A[2][2].Z + 3 * pow(u, 2)*(1 - u)*pow(w, 3)*A[2][3].Z + /*u=3 w=0*/pow(u, 3)*pow(1 - w, 3)*A[3][0].Z + pow(u, 3) * 3 * w*pow(1 - w, 2)*A[3][1].Z + pow(u, 3) * 3 * pow(w, 2)*(1 - w)*A[3][2].Z + pow(u, 3)*pow(w, 3)*A[3][3].Z;

			w -= 0.1;
			normnormal(X, Y, Z, X3, Y3, Z3, X1, Y1, Z1);
			glVertex3f(X, Y, Z);
			//normnormal(X, Y, Z, X, Y, Z, X2, Y2, Z2);
			glVertex3f(X1, Y1, Z1);
			//normnormal(X, Y, Z, X1, Y1, Z1, X3, Y3, Z3);
			glVertex3f(X2, Y2, Z2);
			//normnormal(X, Y, Z, X, Y, Z, X2, Y2, Z2);
			glVertex3f(X3, Y3, Z3);

		}

	}
	glEnd();
}
float length(float vec[])
{
	return pow(pow(vec[0],2) + pow(vec[1],2) + pow(vec[2],2),0.5);
}
void sphere(float R, int s2, int s1, int m, int l, bool f)
{//радиус и количество полигонов в разные плоскости,на сколько частей резать сферу 
	glBindTexture(GL_TEXTURE_2D, texId[2]);
	float x1, y1, z1, x2, y2, z2;
	float a = 1, a2 = 1;
	glColor3f(1, 1, 1);
	for (int i = 0; i < s1 / m; i++)//что бы обрезать сферу делим s1 на 2 если надо полусферу,на 3 если надо треть и т д 
	{
		a2 = (i*M_PI * 2) / (s1 / m);
		glBegin(GL_TRIANGLE_STRIP);
		for (int j = 0; j <= s2 / l; j++)
		{//что бы обрезать сферу делим s2 на 2 если надо полусферу,на 3 если надо треть и т д 

			x1 = R * sin(-2 * M_PI * j / s2) * sin(M_PI * i / s1);
			x2 = R * sin(-2 * M_PI * j / s2) * sin(M_PI * (i + 1) / s1);
			y1 = R * cos(-2 * M_PI * j / s2) * sin(M_PI * i / s1);
			y2 = R * cos(-2 * M_PI * j / s2) * sin(M_PI * (i + 1) / s1);
			z1 = R * cos(M_PI * i / s1);
			z2 = R * cos(M_PI * (i + 1) / s1);
			if (f == true) {;
				a2 = (i*M_PI * 2) / (s1 / m);
			}
			else {
				a2 = ((i + 1)*M_PI * 2) / (s1 / m);
			}
			glNormal3d(x1 / R * sin(M_PI * i / s1), y1 / R * sin(M_PI * i / s1), z1 / R);
			glTexCoord2d( x1 / 512,  y1 / 512);
			glVertex3d(x1, y1, z1);


			if (f == true) {
				a2 = ((i + 1)*M_PI * 2) / (s1 / m);
			}
			else {
				a2 = (i*M_PI * 2) / (s1 / m);
			}
			glNormal3d(x2 / R * sin(M_PI * (i + 1) / s1), y2 / R * sin(M_PI * (i + 1) / s1), z2 / R);
			glTexCoord2d(x2/ 512,y2/ 512);
			glVertex3d(x2, y2, z2);
		}
		glEnd();
	}
}
void planet(float R, int s2, int s1, int m, int l, bool f)
{//радиус и количество полигонов в разные плоскости,на сколько частей резать сферу 
	glBindTexture(GL_TEXTURE_2D, texId[2]);
	float x1, y1, z1, x2, y2, z2;
	float a = 1, a2 = 1;
	double t1 = 0, t2 = 0;
	//glColor3dv(rgb1); 
	glColor3f(1, 1, 1);
	for (int i = 0; i < s1 / m; i++)//что бы обрезать сферу делим s1 на 2 если надо полусферу,на 3 если надо треть и т д 
	{
		t1 = 0;
		t2 += 512 / s1;
		a2 = (i*M_PI * 2) / (s1 / m);
		glBegin(GL_TRIANGLE_STRIP);
		for (int j = 0; j <= s2 / l; j++)
		{//что бы обрезать сферу делим s2 на 2 если надо полусферу,на 3 если надо треть и т д 

			x1 = R * sin(-2 * M_PI * j / s2) * sin(M_PI * i / s1);
			x2 = R * sin(-2 * M_PI * j / s2) * sin(M_PI * (i + 1) / s1);
			y1 = R * cos(-2 * M_PI * j / s2) * sin(M_PI * i / s1);
			y2 = R * cos(-2 * M_PI * j / s2) * sin(M_PI * (i + 1) / s1);
			z1 = R * cos(M_PI * i / s1);
			z2 = R * cos(M_PI * (i + 1) / s1);
			if (f == true) {
				a2 = (i*M_PI * 2) / (s1 / m);
			}
			else {
				a2 = ((i + 1)*M_PI * 2) / (s1 / m);
			}
			t1 += 512 / s2;
			glNormal3d(x1 / R * sin(M_PI * i / s1), y1 / R * sin(M_PI * i / s1), z1 / R);
			glTexCoord2d(t1 / 512, t2 / 512);
			//glTexCoord2d(x1 / 1024.0, y1 / 1024.0); 
			glVertex3d(x1, y1, z1);


			if (f == true) {
				a2 = ((i + 1)*M_PI * 2) / (s1 / m);
			}
			else {
				a2 = (i*M_PI * 2) / (s1 / m);
			}
			glNormal3d(x2 / R * sin(M_PI * (i + 1) / s1), y2 / R * sin(M_PI * (i + 1) / s1), z2 / R);
			glTexCoord2d(t1 / 512, t2 / 512);
			//glTexCoord2d(x2 / 1024.0, y2 / 1024.0); 
			glVertex3d(x2, y2, z2);
		}
		glEnd();
	}

}

void manipulate()
{
	/*float X = (2 * M_PI*up*10) / 24;
	float Y = (2 * M_PI*right * 10) / 24;
	float vecX[] = {1,0,0 };
	float vecY[] = { 0,1,0 };
	glTranslatef(updown++, 0, 0);
	glRotatef(Y, 0, 1, 0);*/
	
	static float osX;
	static float osY;
	static float osZ;
	osX = osX + updown;
	osY = osY + rightleft;
	osZ = osZ + br;
	glRotatef(asin(updown), 0, 1, 0);
	glRotatef(asin(rightleft), 1, 0, 0);
	glRotatef(asin(br), 0, 0, 1);
	glTranslatef(osX, osY, osZ);
}
void shina(float x,float y, float z, float rx, float ry, float rz, float iin, float imaxin, float iout, float imaxout) 
{
	float a, a2;
	glColor3f(0, 1, 0);
	glBegin(GL_QUADS);

	for (; iout < imaxout;iout++) 
	{
		a2 = (M_PI * 2 * iout) / 24;
		for (iin = 0; iin < imaxin;iin++)
		{
			a = (M_PI * 2 * iin) / 24;
			glNormal3f(cos(a)*cos(a2),  sin(a)*cos(a2),sin(a2));
			glVertex3f(rx*cos(a)*cos(a2) + x, rz*sin(a)*cos(a2) + z, ry*sin(a2) + y);
			iin++;
			a = (M_PI * 2 * iin) / 24;
			glVertex3f(rx*cos(a)*cos(a2) + x, rz*sin(a)*cos(a2) + z, ry*sin(a2) + y);
			iout++;
			a2 = (M_PI * 2 * iout) / 24;
			glVertex3f(rx*cos(a)*cos(a2) + x, rz*sin(a)*cos(a2) + z, ry*sin(a2) + y);
			iin--;
			a = (M_PI * 2 * iin) / 24;
			glVertex3f(rx*cos(a)*cos(a2) + x, rz*sin(a)*cos(a2) + z, ry*sin(a2) +y);
			iout--;
			a2 = (M_PI * 2 * iout) / 24;
		}
	}
	glEnd();
}
void badupr() 
{
	float speed = 5;
	static float x=0,y=0,z=0;
	if (updown == 1) z++;
	if (updown == -1)z--;
	if (rightleft == 1)y++;
	if (rightleft == -1) y--;
	if (br == 1)x++;
	if (br == -1)x--;
	glTranslatef(x*speed, y*speed, z*speed);
}
void orbit() 
{
	float A[] = { 0,0,0 };
	float B[] = { 300,0,0 };
	float C[] = { 300,0,300 };
	float D[] = { 600, 0 , 300 };
	klevoedvigenie(A, B, C, D,0);
}
void truecircle(float x,float y,float z,float r,float i, float imax, float kol) 
{
	float a = 0;
	glNormal3f(0, 1, 0);
	glBegin(GL_POLYGON);
	glVertex3f(x, y, z);
	for (i = 0; i < imax; i++)
	{
		a = (2 * M_PI*i) / kol;
		glVertex3f(r*cos(a)+x, y, r*sin(a)+z);
	}
	glEnd();

}
void tok(float x, float y,float z,float shir)
{
	float Z[3] = { shir*0+x,y+0,0+z };
	float A[3] = { shir*30+x,y+rand() % 20,rand()%20+z };
	float B[3] = { shir*60+x, y+rand() % 20 ,rand()%20+z };
	float C[3] = { shir*90+x, y+rand()%20, rand()%20+z };
	float D[3] = { shir*120+x,y+0,0+z };
	GLfloat amb4[] = { 1,1,0, 1 };
	GLfloat dif4[] = { 1,1,0, 1 };
	GLfloat amb3[] = {0,0,1, 1 };
	GLfloat dif3[] = {0,0,1, 1 };
	glBegin(GL_LINE_STRIP);
	glMaterialfv(GL_FRONT, GL_AMBIENT, amb4);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, dif4);
	glVertex3fv(Z);
	glMaterialfv(GL_FRONT, GL_AMBIENT, amb3);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, dif3);
	glVertex3fv(A);
	glMaterialfv(GL_FRONT, GL_AMBIENT, amb4);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, dif4);
	glVertex3fv(B);
	glMaterialfv(GL_FRONT, GL_AMBIENT, amb3);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, dif3);
	
	glVertex3fv(C);
	glMaterialfv(GL_FRONT, GL_AMBIENT, amb4);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, dif4);
	glVertex3fv(D);
	glEnd();

}
void gunfork() 
{
	
	tok(-90,55, 285,1.5);
	tok(-70, 55, 165, 1);
	tok(-84, 55, 205, 1.2);
	tok(-90, 55, 245, 1.3);
	GLfloat amb2[] = { 0.2,0.2,0.8, 1 };
	GLfloat dif2[] = { 0,0,0, 1 };
	GLfloat spec2[] = { 0.2,0.2,0.8,1 };
	GLfloat sh2 = 0.1f*20;
	//фоновая
	glMaterialfv(GL_FRONT, GL_AMBIENT, amb2);
	//дифузная
	glMaterialfv(GL_FRONT, GL_DIFFUSE, dif2);
	//зеркальная
	glMaterialfv(GL_FRONT, GL_SPECULAR, spec2);
	//размер блика
	glMaterialf(GL_FRONT, GL_SHININESS, sh2);
	cily(0, 0, 0, 100, 50);
	float v = 52;
	float v1 = 37;
	truecircle(0, 50, 0, 100, 0, 41, 40);
	float A[] = { 20,v,20 };
	float B[] = {80,v,300};
	float C[] = { 100,v,300 };
	float D[] = { 50,v,-20 };
	float E[] = { -50,v,-20 };
	float F[] = { -100,v,300 };
	float G[] = { -80,v,300 };
	float H[] = { -20,v,20 };
	glBegin(GL_POLYGON);//низ
	glVertex3fv(A);
	glVertex3fv(B);
	glVertex3fv(C);
	glVertex3fv(D);
	glEnd();
	glBegin(GL_POLYGON);
	glVertex3fv(A);
	glVertex3fv(D);
	glVertex3fv(E);
	glVertex3fv(H);
	glEnd();
	glBegin(GL_POLYGON);
	glVertex3fv(F);
	glVertex3fv(G);
	glVertex3fv(H);
	glVertex3fv(E);
	glEnd();
	v += 15;
	 A[1] = v;
	 B[1] =v;
	 C[1] =v;
	 D[1] =v;
	 E[1] = v;
	 F[1] =v;
	 G[1] = v;
	 H[1] = v;
	glBegin(GL_POLYGON);//верх
	glVertex3fv(A);
	glVertex3fv(B);
	glVertex3fv(C);
	glVertex3fv(D);
	glEnd();
	glBegin(GL_POLYGON);
	glVertex3fv(A);
	glVertex3fv(D);
	glVertex3fv(E);
	glVertex3fv(H);
	glEnd();
	glBegin(GL_POLYGON);
	glVertex3fv(F);
	glVertex3fv(G);
	glVertex3fv(H);
	glVertex3fv(E);
	glEnd();
	glBegin(GL_POLYGON);//бока
	glVertex3fv(B);
	glVertex3fv(C);
	B[1] = v1;
	C[1] = v1;
	glVertex3fv(C);
	glVertex3fv(B);
	glEnd();
	B[1] = v;
	C[1] = v;
	glBegin(GL_POLYGON);
	glVertex3fv(C);
	glVertex3fv(D);
	C[1] = v1;
	D[1] = v1;
	glVertex3fv(D);
	glVertex3fv(C);
	glEnd();
	D[1] = v;
	glBegin(GL_POLYGON);
	glVertex3fv(D);
	glVertex3fv(E);
	D[1] = v1;
	E[1] = v1;
	glVertex3fv(E);
	glVertex3fv(D);
	glEnd();
	E[1] = v;
	glBegin(GL_POLYGON);
	glVertex3fv(F);
	glVertex3fv(E);
	F[1] = v1;
	E[1] = v1;
	glVertex3fv(E);
	glVertex3fv(F);
	glEnd();
	F[1] = v;
	glBegin(GL_POLYGON);
	glVertex3fv(F);
	glVertex3fv(G);
	F[1] = v1;
	G[1] = v1;
	glVertex3fv(G);
	glVertex3fv(F);
	glEnd();
	G[1] = v;
	glBegin(GL_POLYGON);
	glVertex3fv(H);
	glVertex3fv(G);
	H[1] = v1;
	G[1] = v1;
	glVertex3fv(G);
	glVertex3fv(H);
	glEnd();
	H[1] = v;
	glBegin(GL_POLYGON);
	glVertex3fv(H);
	glVertex3fv(A);
	H[1] = v1;
	A[1] = v1;
	glVertex3fv(A);
	glVertex3fv(H);
	glEnd();
	A[1] = v;
	glBegin(GL_POLYGON);
	glVertex3fv(B);
	glVertex3fv(A);
	B[1] = v1;
	A[1] = v1;
	glVertex3fv(A);
	glVertex3fv(B);
	glEnd();
	B[1] = v;
	amb2[0] = 0.8; amb2[1] = 0.8; amb2[2] = 1;
	dif2[0] = 0.8; dif2[1] = 0.8; amb2[2] = 1;
	glMaterialfv(GL_FRONT, GL_AMBIENT, amb2);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, dif2);
	static float radar = 0;
	glPushMatrix();
	glRotatef(radar, 0, 1, 0);
	cily(0, 65, 0, 40, 60);
	truecircle(0, 125, 0, 40, 0, 25, 24);
	glPushMatrix();
	glRotatef(90, 1, 0, 0);
	glRotatef(-90, 0, 1, 0);
	shina(-160, 0, 0, 40, 10, 80, 0, 10, -2,2);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(0, 10, 0);
	glRotatef(180, 0, 1, 0);
	amb2[0] = 0.5; amb2[1] = 0.42; amb2[2] = 0;
	dif2[0] = 0.5; dif2[1] = 0.42; amb2[2] = 0;
	spec2[0] = 1; spec2[1] = 0; spec2[2] = 0;
	glMaterialfv(GL_FRONT, GL_SPECULAR, spec2);
	glMaterialfv(GL_FRONT, GL_AMBIENT, amb2);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, dif2);
	poverhnostbezeforgun();
	glPopMatrix();
	glPopMatrix();
	radar += 0.6;
	amb2[0] = 1; amb2[1] = 1; amb2[2] = 1;
	dif2[0] = 1; dif2[1] = 1; amb2[2] = 1;
	spec2[0] = 0; spec2[1] = 0; spec2[2]=0;
	glMaterialfv(GL_FRONT, GL_SPECULAR, spec2);
	glMaterialfv(GL_FRONT, GL_AMBIENT, amb2);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, dif2);
	shina(50, 0, 80, 20, 150, 20, 0, 24, 0, 24);
	shina(-50, 0, 80, 20, 150, 20, 0, 24, 0, 24);
}
void guns() 
{
	glPushMatrix();
	glRotatef(-forkurs, 1, 0, 0);
	glTranslatef(250, 82, 0);
	glRotatef(90, 0, 1, 0);
	glScalef(0.08, 0.08, 0.08);
	gunfork();
	glPopMatrix();
	glPushMatrix();
	glRotatef(-forkurs, 1, 0, 0);
	glTranslatef(250, 0, 82);
	glRotatef(90, 0, 0, 1);
	glRotatef(90, 1, 0, 0);
	glScalef(0.08, 0.08, 0.08);
	gunfork();
	glPopMatrix();
	glPushMatrix();
	glRotatef(-forkurs, 1, 0, 0);
	glTranslatef(250, 0, -82);
	glRotatef(-90, 0, 0, 1);
	glRotatef(-90, 1, 0, 0);
	glScalef(0.08, 0.08, 0.08);
	gunfork();
	glPopMatrix();
	glPushMatrix();
	glRotatef(-forkurs, 1, 0, 0);
	glTranslatef(250, -82, 0);
	glRotatef(180, 1, 0, 0);
	glRotatef(90, 0, 1, 0);
	glScalef(0.08, 0.08, 0.08);
	gunfork();
	glPopMatrix();
}
void other2() 
{
	glPushMatrix();
	glRotatef(36 + forkurs, 1, 0, 0);
	glRotatef(-10, 0, 0, 1);
	illum(0, -128, -135);
	glPopMatrix();
	glPushMatrix();
	glRotatef(72 + forkurs, 1, 0, 0);
	glRotatef(-10, 0, 0, 1);
	illum(0, -128, -135);
	glPopMatrix();
	glPushMatrix();
	glRotatef(108 + forkurs, 1, 0, 0);
	glRotatef(-10, 0, 0, 1);
	illum(0, -128, -135);
	glPopMatrix();
	glPushMatrix();
	glRotatef(144 + forkurs, 1, 0, 0);
	glRotatef(-10, 0, 0, 1);
	illum(0, -128, -135);
	glPopMatrix();
	glPushMatrix();
	glRotatef(180 + forkurs, 1, 0, 0);
	glRotatef(-10, 0, 0, 1);
	illum(0, -128, -135);
	glPopMatrix();
	glPushMatrix();
	glRotatef(216 + forkurs, 1, 0, 0);
	glRotatef(-10, 0, 0, 1);
	illum(0, -128, -135);
	glPopMatrix();
	glPushMatrix();
	glRotatef(252 + forkurs, 1, 0, 0);
	glRotatef(-10, 0, 0, 1);
	illum(0, -128, -135);
	glPopMatrix();
	glPushMatrix();
	glRotatef(288 + forkurs, 1, 0, 0);
	glRotatef(-10, 0, 0, 1);
	illum(0, -128, -135);
	glPopMatrix();
	glPushMatrix();
	glRotatef(324 + forkurs, 1, 0, 0);
	glRotatef(-10, 0, 0, 1);
	illum(0, -128, -135);
	glPopMatrix();
	glPushMatrix();
	glRotatef(360 + forkurs, 1, 0, 0);
	glRotatef(-10, 0, 0, 1);
	illum(0, -128, -135);
	glPopMatrix();

	glPushMatrix();
	glRotatef(180, 0, 1, 0);
	glRotatef(36-forkurs, 1, 0, 0);
	glRotatef(-10, 0, 0, 1);
	illum(0, 207, -77);
	glPopMatrix();
	glPushMatrix();
	glRotatef(180, 0, 1, 0);
	glRotatef(72 - forkurs, 1, 0, 0);
	glRotatef(-10, 0, 0, 1);
	illum(0, 207, -77);
	glPopMatrix();
	glPushMatrix();
	glRotatef(180, 0, 1, 0);
	glRotatef(108 - forkurs, 1, 0, 0);
	glRotatef(-10, 0, 0, 1);
	illum(0, 207, -77);
	glPopMatrix();
	glPushMatrix();
	glRotatef(180, 0, 1, 0);
	glRotatef(144 - forkurs, 1, 0, 0);
	glRotatef(-10, 0, 0, 1);
	illum(0, 207, -77);
	glPopMatrix();
	glPushMatrix();
	glRotatef(180, 0, 1, 0);
	glRotatef(180 - forkurs, 1, 0, 0);
	glRotatef(-10, 0, 0, 1);
	illum(0, 207, -77);
	glPopMatrix();
	glPushMatrix();
	glRotatef(180, 0, 1, 0);
	glRotatef(216 - forkurs, 1, 0, 0);
	glRotatef(-10, 0, 0, 1);
	illum(0, 207, -77);
	glPopMatrix();
	glPushMatrix();
	glRotatef(180, 0, 1, 0);
	glRotatef(252 - forkurs, 1, 0, 0);
	glRotatef(-10, 0, 0, 1);
	illum(0, 207, -77);
	glPopMatrix();
	glPushMatrix();
	glRotatef(180, 0, 1, 0);
	glRotatef(288 - forkurs, 1, 0, 0);
	glRotatef(-10, 0, 0, 1);
	illum(0, 207, -77);
	glPopMatrix();
	glPushMatrix();
	glRotatef(180, 0, 1, 0);
	glRotatef(324 - forkurs, 1, 0, 0);
	glRotatef(-10, 0, 0, 1);
	illum(0, 207, -77);
	glPopMatrix();
	glPushMatrix();
	glRotatef(180, 0, 1, 0);
	glRotatef(360 - forkurs, 1, 0, 0);
	glRotatef(-10, 0, 0, 1);
	illum(0, 207, -77);
	glPopMatrix();
}

void illum(float x, float y, float z)
{
	glPushMatrix();
	glRotatef(90, 1, 0, 0);
	glRotatef(90, 0, 0,1);
	
	GLfloat amb[] = {0.3,0.3, 0.9,1 };
	GLfloat dif[] = { 0.4, 0.65, 0.5, 1. };
	GLfloat spec[] = { 0.9, 0.8, 0.3, 1. };
	GLfloat sh = 0.1f * 256;
	glMaterialfv(GL_FRONT, GL_AMBIENT, amb);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, dif);
	glMaterialfv(GL_FRONT, GL_SPECULAR, spec);
	glMaterialf(GL_FRONT, GL_SHININESS, sh);
	truecircle(x, y + 0.3, z, 5, 0, 25, 24);
	amb[0] = 0.1; amb[1] = 0.1; amb[2] = 0.1;
	glMaterialfv(GL_FRONT, GL_AMBIENT, amb);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, amb);
	truecircle(x, y, z, 7, 0, 25, 24);
	amb[0] = 0.3; amb[1] = 0.3; amb[2] = 0.3;
	glMaterialfv(GL_FRONT, GL_AMBIENT, amb);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, amb);
	glPushMatrix();
	glTranslatef(x, 0, z);
	cily(6 + x, y + 0, 0, 0.4, 0.4);
	truecircle(x + 6, y+0.4, 0, 0.4, 0, 7, 6);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(x, 0, z);
	glRotatef(60, 0, 1, 0);
	cily(6 + x, y + 0, 0, 0.4, 0.4);
	truecircle(x + 6, y + 0.4, 0, 0.4, 0, 7, 6);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(x, 0, z);
	glRotatef(120, 0, 1, 0);
	cily(6 + x, y + 0, 0, 0.4, 0.4);
	truecircle(x + 6, y + 0.4, 0, 0.4, 0, 7, 6);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(x, 0, z);
	glRotatef(180, 0, 1, 0);
	cily(6 + x, y + 0, 0, 0.4, 0.4);
	truecircle(x + 6, y + 0.4, 0, 0.4, 0, 7, 6);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(x, 0, z);
	glRotatef(240, 0, 1, 0);
	cily(6 + x, y + 0, 0, 0.4, 0.4);
	truecircle(x + 6, y + 0.4, 0, 0.4, 0, 7, 6);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(x, 0, z);
	glRotatef(300, 0, 1, 0);
	cily(6 + x, y + 0, 0, 0.4, 0.4);
	truecircle(x + 6, y + 0.4, 0, 0.4, 0, 7, 6);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(x, 0, z);
	glRotatef(360, 0, 1, 0);
	cily(6 + x, y + 0, 0, 0.4, 0.4);
	truecircle(x + 6, y + 0.4, 0, 0.4, 0, 7, 6);
	glPopMatrix();
	cily(x, y-4.5, z, 7, 5);
	glPopMatrix();
}
