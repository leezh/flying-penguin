//  The Flying Penguin
//  Copyright (C) 2010 Lee Zher Huei <lee@leezh.net>
//
//  This program is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation; either version 2 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program; if not, write to the Free Software
//  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
//  MA 02110-1301, USA.
//

//  SOL Game Framework
//  SOL: Object-orientated Loop-based Game Framework
//
//  I created this for use as the underlaying framework for The Flying
//  Penguin while at the same time it was designed such that it could
//  be reused in the future. However, the lack of documentation and
//  narrow design aspects does not make it ideal for an actual library.
//  - LeeZH

#include <map>
#include <list>
#include <stack>
#include <algorithm>

#ifdef DEBUG
    #include <iostream>
    #define ERR(x) std::cout << "Error: " << x << std::endl;
    #define LOG(x) std::cout << x << std::endl;
#else
    #define ERR(x)
    #define LOG(X)
#endif

#include <SDL.h>
#include <SDL_image.h>
#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_GLYPH_H

#include "sol.hpp"
using namespace std;

namespace sol {
//=============================================== Backend Variables ====
SDL_Surface* screen;
FT_Library ftLibrary;
GLuint whiteTex;
glyph *whiteGlyph;
//============================================== Texture Management ====
map<GLuint, int> texList;
void texAdd(GLuint t) {
	map<GLuint, int>::iterator it = texList.find(t);
	if (it != texList.end()) {
		it->second++;
	} else {
		texList.insert(std::pair<GLuint, int>(t, 1));
	}
}
void texDelete(GLuint t) {
	map<GLuint, int>::iterator it = texList.find(t);
	if (it != texList.end()) {
		it->second--;
		if (it->second <= 0) {
			glDeleteTextures(1, &it->first);
			texList.erase(it);
		}
	}
}
void texDeleteAll() {
	map<GLuint, int>::iterator it;
	for (it = texList.begin(); it != texList.end(); it++) {
		glDeleteTextures(1, &it->first);
	}
	texList.clear();
}
//================================================= Font Management ====
map<FT_Face, int> fntList;
void fntAdd(FT_Face t) {
	std::map<FT_Face, int>::iterator it = fntList.find(t);
	if (it != fntList.end()) {
		it->second++;
	} else {
		fntList.insert(pair<FT_Face, int>(t, 1));
	}
}
void fntDelete(FT_Face t) {
	map<FT_Face, int>::iterator it = fntList.find(t);
	if (it != fntList.end()) {
		it->second--;
		if (it->second == 0) {
			FT_Done_Face(it->first);
			fntList.erase(it);
		}
	}
}
void fntDeleteAll() {
	map<FT_Face, int>::iterator it;
	for (it = fntList.begin(); it != fntList.end(); it++) {
		FT_Done_Face(it->first);
	}
	fntList.clear();
}
//==================================================== Initialisers ====
bool init(int w, int h, bool fullscreen, int depth) {
    LOG("Initialising SOL Game Framework");
	if (FT_Init_FreeType(&ftLibrary) != 0) {
	    ERR("Initialising Freetype failed");
		return false;
	}

	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		FT_Done_FreeType(ftLibrary);
		ERR(SDL_GetError());
		return false;
	}
	IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG);

	uint32_t flags = SDL_OPENGL | SDL_DOUBLEBUF | SDL_HWSURFACE;
	if (fullscreen) {
		flags = flags | SDL_FULLSCREEN;
	}

	if (depth != 0) {
		screen = SDL_SetVideoMode(w, h, depth, flags);
	} else {
	    LOG("Screen depth not defined, trying one by one")
		depth = 32;
		while (depth >= 8 && screen == NULL) {
			depth = depth - 8;
			screen = SDL_SetVideoMode(w, h, depth, flags);
		}
	}

	if (screen == NULL) {
	    ERR(SDL_GetError());
		FT_Done_FreeType(ftLibrary);
		return false;
	}
	LOG("Created a screen of size " << w << "x" << h << "@" << depth);

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, depth);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	clear2d(0, 0);
	glViewport(0, 0, screen->w, screen->h);

	unsigned char whiteImg[4] = {0xff,0xff,0xff,0xff};
	glGenTextures(1, &whiteTex);
	glBindTexture(GL_TEXTURE_2D, whiteTex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, 4, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, whiteImg);
	texAdd(whiteTex);

	whiteGlyph = new glyph(whiteTex);

	return true;
}
void uninit() {
    LOG("Unitialising SOL Game Framework");
    delete whiteGlyph;
	texDeleteAll();
	fntDeleteAll();
	SDL_FreeSurface (screen);
	SDL_Quit();
	FT_Done_FreeType(ftLibrary);
}
//===================================================== GL Graphics ====
void clear2d(float offsetX, float offsetY) {
    offsetX = -offsetX;
    offsetY = -offsetY;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(screen->w * offsetX, screen->w * (1 + offsetX), screen->h * (1 + offsetY), screen->h * offsetY, -1.0f, 1.0f);
	glMatrixMode(GL_MODELVIEW);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
}
void colour(float r, float g, float b, float a) {
	glColor4f(r, g, b, a);
}

void refresh() {SDL_GL_SwapBuffers();}
//================================================== Time Functions ====
unsigned long int ticks() {return SDL_GetTicks();}
void delay(unsigned long int milliSeconds) {SDL_Delay(milliSeconds);}
limiter::limiter() {}
float limiter::tick(float capRate)
{
	if (capRate > 1)
	{
	    if ((SDL_GetTicks() - lasttick) < (1000 / capRate))
	    {
	        SDL_Delay((1000 / capRate) - (SDL_GetTicks() - lasttick));
	    }
	}
	unsigned long diff = SDL_GetTicks() - lasttick;
	lasttick = SDL_GetTicks();
	return 1000 / float(diff);
}
void limiter::reset() {lasttick = SDL_GetTicks();}
//======================================== Window Manager Functions ====
namespace wm {
	std::string caption() {
		char* value;
		SDL_WM_GetCaption(&value, NULL);
		return string(value);
	}
	void caption(std::string value, std::string icon) {
		SDL_WM_SetCaption(value.c_str(), NULL);
		if (icon != "")
            SDL_WM_SetIcon(IMG_Load(icon.c_str()), NULL);
	}
	bool fullscreen() {
		return ((screen->flags & SDL_FULLSCREEN) != 0);
	}
	void fullscreen(bool value) {
		if (screen != NULL) {
		    Uint32 flags = screen->flags;
		    if (((flags & SDL_FULLSCREEN) != 0) != value) {
		        flags = flags^SDL_FULLSCREEN;
		    }
		    SDL_SetVideoMode(screen->w, screen->h, screen->format->BitsPerPixel, flags);
		}
	}
	rect size() {
		return rect(0, 0, screen->w, screen->h);
	}
	void size(rect &dim) {
		SDL_SetVideoMode(dim.w, dim.h, screen->format->BitsPerPixel, screen->flags);
	}
	bool grab() {
		return (SDL_WM_GrabInput(SDL_GRAB_QUERY) == SDL_GRAB_ON);
	}
	void grab(bool value) {
		if (value) {
			SDL_WM_GrabInput(SDL_GRAB_ON);
		} else {
			SDL_WM_GrabInput(SDL_GRAB_OFF);
		}
	}
}
//======================================================== Textures ====
texture::texture(): texid(0), w(0), h(0) {}
texture::texture(const texture& t) {
	texid = t.texid;
	w = t.w;
	h = t.h;
	texAdd(t.texid);
}
texture::texture(GLuint t): texid(t) {texAdd(t);}
texture::texture(std::string filename): texid(0), w(0), h(0) {open(filename);}
texture::~texture() {texDelete(texid);}
texture& texture::operator =(const texture& t){
	if (texid != 0) {
		texDelete(texid);
	}
	texid = t.texid;
	w = t.w;
	h = t.h;
	texAdd(t.texid);
	return *this;
}
texture::operator GLuint() {return texid;}
bool texture::open(std::string filename) {
	SDL_Surface* s = IMG_Load(filename.c_str());
	LOG("Loading image: " << filename);
	if (s == NULL) {
	    ERR(IMG_GetError());
		return false;
	}

	// Many computers even today still cannot handle this
	if ((s->w & (s->w - 1)) != 0 || (s->h & (s->h - 1)) != 0) {
	    ERR("Image not power of 2");
		SDL_FreeSurface(s);
		return false;
	}
	w = s->w;
	h = s->h;

	GLenum format;
	GLint bpp = s->format->BytesPerPixel;
	if (bpp == 4) {
		if (s->format->Rmask == 0x000000ff) {
			format = GL_RGBA;
		} else {
			format = GL_BGRA;
        }
	} else if (bpp == 3) {
		if (s->format->Rmask == 0x000000ff) {
			format = GL_RGB;
		} else {
			format = GL_BGR;
		}
	} else {
	    ERR("Image format not supported");
		SDL_FreeSurface(s);
		return false;
	}

	glGenTextures(1, &texid);
	glBindTexture(GL_TEXTURE_2D, texid);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, bpp, s->w, s->h, 0, format, GL_UNSIGNED_BYTE, s->pixels);

	texAdd(texid);
	SDL_FreeSurface(s);
	return true;
}
void texture::bind() {
	glBindTexture(GL_TEXTURE_2D, texid);
}
void texture::unbind() {
	glBindTexture(GL_TEXTURE_2D, whiteTex);
}
rect texture::size() {
	return rect(0, 0, w, h);
}
//====================================================== Rectangles ====
rect::rect(): x(0), y(0), w(0), h(0), centreX(0), centreY(0), angle(0) {}
rect::rect(float w, float h): x(0), y(0), centreX(0), centreY(0), angle(0) {
	this->w = w;
	this->h = h;
}
rect::rect(float x, float y, float w, float h): centreX(0), centreY(0), angle(0) {
	this->x = x;
	this->y = y;
	this->w = w;
	this->h = h;
}
void rect::draw() {
    glPushMatrix();
    glTranslatef(x + centreX * w, y + centreY * h, 0);
    glRotatef(angle, 0.f, 0.f, 1.f);
	glBegin(GL_QUADS);
	glVertex2f((- centreX * w), (- centreY * h));
	glVertex2f((1 - centreX) * w, (- centreY * h));
	glVertex2f((1 - centreX) * w, (1 - centreY) * h);
	glVertex2f((- centreX * w), (1 - centreY) * h);
	glEnd();
	glPopMatrix();
}
void rect::draw(texture* tex) {
    glPushMatrix();
    glTranslatef(x + centreX * w, y + centreY * h, 0);
    glRotatef(angle, 0.f, 0.f, 1.f);
	tex->bind();
	glBegin(GL_QUADS);
	glTexCoord2i(0, 0); glVertex2f((- centreX * w), (- centreY * h));
	glTexCoord2i(1, 0); glVertex2f((1 - centreX) * w, (- centreY * h));
	glTexCoord2i(1, 1); glVertex2f((1 - centreX) * w, (1 - centreY) * h);
	glTexCoord2i(0, 1); glVertex2f((- centreX * w), (1 - centreY) * h);
	glEnd();
	tex->unbind();
	glPopMatrix();
}
void rect::draw(texture* tex, const rect& texcoord) {
    glPushMatrix();
    glTranslatef(x + centreX * w, y + centreY * h, 0);
    glRotatef(angle, 0.f, 0.f, 1.f);
	tex->bind();
	glBegin(GL_QUADS);
	glTexCoord2f(texcoord.x, texcoord.y); glVertex2f((- centreX * w), (- centreY * h));
	glTexCoord2f(texcoord.x + texcoord.w, texcoord.y); glVertex2f((1 - centreX) * w, (- centreY * h));
	glTexCoord2f(texcoord.x + texcoord.w, texcoord.y + texcoord.h); glVertex2f((1 - centreX) * w, (1 - centreY) * h);
	glTexCoord2f(texcoord.x, texcoord.y + texcoord.h); glVertex2f((- centreX * w), (1 - centreY) * h);
	glEnd();
	tex->unbind();
	glPopMatrix();
}
void rect::alignCentre() {
    x = (screen->w - w) / 2;
    y = (screen->h - h) / 2;
}
void rect::rotate(float angle, float centreX, float centreY) {
    this->angle = angle;
    this->centreX = centreX;
    this->centreY = centreY;
}
//=========================================================== Fonts ====
glyph::glyph(GLuint t): tex(t) {
	texW = 0;
	texH = 0;
	offsetX = 0;
	offsetY = 0;
	advance = 0;
}
glyph::glyph(const glyph& g): tex(g.tex) {
	texW = g.texW;
	texH = g.texH;
	offsetX = g.offsetX;
	offsetY = g.offsetY;
	advance = g.advance;
}
glyph* font::character(unsigned int c) {
    if (ftFace == NULL) return whiteGlyph;
	FT_Face face = (FT_Face)ftFace;
	std::map<unsigned int, glyph*>::iterator it = glyphs.find(c);
	if (it != glyphs.end()) {
		return it->second;
	}

	FT_Glyph ftGlyph;
	FT_Load_Char(face, c, FT_LOAD_DEFAULT);
	FT_Get_Glyph(face->glyph, &ftGlyph);

	// This is the high quality version
	FT_Glyph_To_Bitmap(&ftGlyph, FT_RENDER_MODE_NORMAL, 0, 1);
	FT_BitmapGlyph bGlyph = (FT_BitmapGlyph)ftGlyph;
	FT_Bitmap bitmap = bGlyph->bitmap;

	int new_w = 1, new_h = 1;
	while (new_w <= bitmap.width){
		new_w = new_w << 1;
	}
	while (new_h <= bitmap.rows){
		new_h = new_h << 1;
	}

	GLubyte* data = new GLubyte[2 * new_w * new_h];
	for (int j = 0; j < new_h; j++) {
		for (int i = 0; i < new_w; i++) {
			if (i < bitmap.width && j < bitmap.rows) {
				data[2 * (i + (j * new_w))] = bitmap.buffer[i + (j * bitmap.width)];
				data[2 * (i + (j * new_w)) + 1] = bitmap.buffer[i + (j * bitmap.width)];
			} else {
				data[2 * (i + (j * new_w))] = 0;
				data[2 * (i + (j * new_w)) + 1] = 0;
			}
		}
	}
	GLuint texid;
	glGenTextures(1, &texid);
	glBindTexture(GL_TEXTURE_2D, texid);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA16, new_w, new_h, 0, GL_LUMINANCE_ALPHA, GL_UNSIGNED_BYTE, data);

	glyph *newGlyph = new glyph(texid);
	newGlyph->w = bitmap.width;
	newGlyph->h = bitmap.rows;
	newGlyph->tex.w = new_w;
	newGlyph->tex.h = new_h;
	newGlyph->texW = float(bitmap.width) / float(new_w);
	newGlyph->texH = float(bitmap.rows) / float(new_h);
	newGlyph->offsetX = float(face->glyph->metrics.horiBearingX >> 6);
	newGlyph->offsetY = size - float((face->glyph->metrics.horiBearingY - face->size->metrics.descender) >> 6);
	newGlyph->advance = float((face->glyph->advance.x) >> 6);
	glyphs.insert(std::pair<unsigned int, glyph*>(c, newGlyph));

	delete[] data;
	it = glyphs.find(c);
	return it->second;
}
font::font() {ftFace = NULL;}
font::font(std::string filename, int size) {open(filename, size);}
font::~font() {
    std::map<unsigned int, glyph*>::iterator it;
    for (it = glyphs.begin(); it != glyphs.end(); it++) {
        delete it->second;
    }

    fntDelete((FT_Face)ftFace);
}
bool font::open(std::string filename, int size) {
    LOG("Loading font: " << filename);
	this->size = size;
	if (FT_New_Face(ftLibrary, filename.c_str(), 0, (FT_Face*)&ftFace) != 0) {
	    ftFace = NULL;
	    ERR("Loading font failed");
		return false;
	}
	fntAdd((FT_Face)ftFace);
	FT_Set_Char_Size((FT_Face)ftFace, size<<6, size<<6, 72, 72);
	return true;
}
void font::preloadASCII() {
	for (int i = 32; i < 128; i++) {
		character(i);
	}
}
float font::draw(unsigned int c, float x, float y) {
	glyph* g = character(c);
	rect box(x + g->offsetX, y + g->offsetY, g->w, g->h);
	box.draw(&g->tex, rect(0, 0, g->texW, g->texH));
	return float(g->advance);
}
float font::draw(std::string text, float x, float y) {
	std::string::iterator it;
	float dx = 0.f;
	for (it = text.begin(); it != text.end(); it++) {
		dx += draw(*it, x + dx, y);
	}
	return dx;
}
float font::width(std::string text) {
	std::string::iterator it;
	float totalWidth = 0;
	for (it = text.begin(); it != text.end(); it++) {
		glyph* g = character(*it);
		totalWidth += float(g->advance);
	}
	return totalWidth;
}
//==================================================== Applications ====
app *activeApp = NULL;
stack<app *> suspendedApps;

void activate(app *a, bool close) {
	if (close) deactivate();
	if (activeApp != NULL) {
		activeApp->suspend();
		suspendedApps.push(activeApp);
	}
	activeApp = a;
	if (!activeApp->init()) deactivate();
}
void deactivate() {
	if (activeApp == NULL) return;
	activeApp->quit();
	if (!suspendedApps.empty()) {
		activeApp = suspendedApps.top();
		suspendedApps.pop();
		activeApp->resume();
	} else {
		activeApp = NULL;
	}
}
void deactivateAll() {
	if (activeApp == NULL) return;
	activeApp->quit();
	while (!suspendedApps.empty()) {
		activeApp = suspendedApps.top();
		suspendedApps.pop();
		activeApp->quit();
	}
	activeApp = NULL;
}
bool run_single() {
	if (activeApp == NULL) return false;
	activeApp->loop();
	return true;
}
void run() {
	while (activeApp != NULL) {
		SDL_Event event;
		while (SDL_PollEvent(&event) && activeApp != NULL) {
			if (event.type == SDL_KEYDOWN || event.type == SDL_KEYUP){
				activeApp->onKeyboard(event.key.keysym.sym, (event.key.state == SDL_PRESSED));
			}
			if (event.type == SDL_MOUSEMOTION){
				activeApp->onMouseMove(event.motion.x, event.motion.y);
			}
			if (event.type == SDL_MOUSEBUTTONDOWN || event.type == SDL_MOUSEBUTTONUP){
				activeApp->onMouseButton(event.button.button, (event.button.state == SDL_PRESSED));
			}
			if (event.type == SDL_QUIT){
				activeApp->onClose();
			}
		}
		run_single();
	}
}
} // namespace sol
