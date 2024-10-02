#include <stdlib.h>
#include <string.h>
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <stdio.h>
#include <math.h>


class LoadBMPObj{
	private:
	
	GLuint texture[2];

	struct Image {
	    unsigned long sizeX;
	    unsigned long sizeY;
	    char *data;
	};
	
	typedef struct Image Image;
	
	int ImageLoad(char *filename, Image *image) {
	    FILE *file;
	    unsigned long size; // size of the image in bytes.
	    unsigned long i; // standard counter.
	    unsigned short int planes; // number of planes in image (must be 1)
	    unsigned short int bpp; // number of bits per pixel (must be 24)
	    char temp; // temporary color storage for bgr-rgb conversion.
	    // make sure the file is there.
	    if ((file = fopen(filename, "rb"))==NULL){
	        printf("File Not Found : %s\n",filename);
	        return 0;
	    }
	    // seek through the bmp header, up to the width/height:
	    fseek(file, 18, SEEK_CUR);
	    // read the width
	    if ((i = fread(&image->sizeX, 4, 1, file)) != 1) {
	        printf("Error reading width from %s.\n", filename);
	        return 0;
	    }
	    //printf("Width of %s: %lu\n", filename, image->sizeX);
	    // read the height
	    if ((i = fread(&image->sizeY, 4, 1, file)) != 1) {
	        printf("Error reading height from %s.\n", filename);
	        return 0;
	    }
	    //printf("Height of %s: %lu\n", filename, image->sizeY);
	    // calculate the size (assuming 24 bits or 3 bytes per pixel).
	    size = image->sizeX * image->sizeY * 3;
	    // read the planes
	    if ((fread(&planes, 2, 1, file)) != 1) {
	        printf("Error reading planes from %s.\n", filename);
	        return 0;
	    }
	    if (planes != 1) {
	        printf("Planes from %s is not 1: %u\n", filename, planes);
	        return 0;
	    }
	    // read the bitsperpixel
	    if ((i = fread(&bpp, 2, 1, file)) != 1) {
	        printf("Error reading bpp from %s.\n", filename);
	        return 0;
	    }
	    if (bpp != 24) {
	        printf("Bpp from %s is not 24: %u\n", filename, bpp);
	        return 0;
	    }
	    // seek past the rest of the bitmap header.
	    fseek(file, 24, SEEK_CUR);
	    // read the data.
	    image->data = (char *) malloc(size);
	    if (image->data == NULL) {
	        printf("Error allocating memory for color-corrected image data");
	        return 0;
	    }
	    if ((i = fread(image->data, size, 1, file)) != 1) {
	        printf("Error reading image data from %s.\n", filename);
	        return 0;
	    }
	    for (i=0;i<size;i+=3) { // reverse all of the colors. (bgr -> rgb)
	        temp = image->data[i];
	        image->data[i] = image->data[i+2];
	        image->data[i+2] = temp;
	    }
	    // we're done.
	    return 1;
	}
	
	Image * loadTexture(){
	    Image *image1;
	    // allocate space for texture
	    image1 = (Image *) malloc(sizeof(Image));
	    if (image1 == NULL) {
	        printf("Error allocating space for image");
	        exit(0);
	    }
	    if (!ImageLoad("Image/Beutiful.bmp", image1)) {
	        exit(1);
	    }
	    return image1;
	}
	
	public:
		
	void initialize()
	{
	    //glClearColor (0.5, 0.5, 0.5, 0.0);
	    //glEnable(GL_DEPTH_TEST);
	    //glDepthFunc(GL_LESS);
	    Image *image1 = loadTexture();
	    if(image1 == NULL){
	        printf("Image was not returned from loadTexture\n");
	        exit(0);
	    }
	    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	    // Create Texture
	    glGenTextures(2, texture);
	    glBindTexture(GL_TEXTURE_2D, texture[0]);
	    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR); //scale linearly when image bigger than texture
	    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR); //scale linearly when image smalled than texture
	    glTexImage2D(GL_TEXTURE_2D, 0, 3, image1->sizeX, image1->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, image1->data);
	    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
	    //glEnable(GL_TEXTURE_2D);
	    //glShadeModel(GL_FLAT);
	    //glDisable(GL_TEXTURE_2D);
	}	
	
		
		
	LoadBMPObj(){
		
	}
	
	
		
	void showImage(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4, float tx1 = 0.0, float ty1 = 0.0, float tx2 = 1.0, float ty2 = 0.0, float tx3 = 1.0, float ty3 = 1.0, float tx4 = 0.0, float ty4 = 1.0){
		glEnable(GL_TEXTURE_2D);
	    //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	    glBindTexture(GL_TEXTURE_2D, texture[0]);
	    glBegin(GL_QUADS);
	    glTexCoord2f(tx1, ty1);
	    glVertex3f(x1, y1, 1);
	    glTexCoord2f(tx2, ty2);
	    glVertex3f(x2, y2, 1);
	    glTexCoord2f(tx3, ty3); 
	    glVertex3f(x3, y3, 1);
	    glTexCoord2f(tx4, ty4);
	    glVertex3f(x4, y4, 1);
	    /*glTexCoord2f(0.0, 0.0);
	    glVertex2f(100, 300);
	    glTexCoord2f(1.0, 0.0);
	    glVertex2f(200, 300);
	    glTexCoord2f(1.0, 1.0); 
	    glVertex2f(200, 100);
	    glTexCoord2f(0.0, 1.0);
	    glVertex2f(100, 100);*/
	    glEnd();
	    glDisable(GL_TEXTURE_2D);
	    glColor3f(0, 1, 0);
	    glPointSize(5);
	    glBegin(GL_POINTS);
	    glVertex2f(1.2, 1.2);
	    glEnd();
	    
	    //glutSwapBuffers();
	}
	
};


