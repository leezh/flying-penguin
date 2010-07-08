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

#ifndef _SOL_HEADER_
#define _SOL_HEADER_
#include <string>
#include <map>
#include <SDL_opengl.h>
#include <SDL_keysym.h>

namespace sol {
//============================================ Forward Declarations ====
class texture;
class rect;
class glyph;
class font;
class app;
//==================================================== Initialisers ====
bool init(int w, int h, bool fullscreen = false, int depth = 0);
void uninit();
//===================================================== GL Graphics ====
void clear2d(float offsetX = 0, float offsetY = 0);
void colour(float r, float g, float b, float a = 1.f);
void refresh();
//================================================== Time Functions ====
unsigned long int ticks();
void delay(unsigned long int milliSeconds);
struct limiter
{
	unsigned long int lasttick;

	limiter();
	void reset();
	float tick(float capRate = 0);
};
//======================================== Window Manager Functions ====
namespace wm {
	std::string caption();
	void caption(std::string value, std::string icon = "");
	bool fullscreen();
	void fullscreen(bool value);
	rect size();
	void size(rect &dim);
	bool grab();
	void grab(bool value);
}
//======================================================== Textures ====
class texture {
	public:
		GLuint texid;
		unsigned long int w, h;

		texture();
		texture(const texture& t);
		texture(GLuint t);
		texture(std::string filename);
		~texture();
		texture& operator =(const texture& t);
		operator GLuint();

		bool open(std::string filename);
		void bind();
		static void unbind();

		rect size();
};
//====================================================== Rectangles ====
struct rect {
	float x, y, w, h;
	float centreX, centreY, angle;

	rect();
	rect(float w, float h);
	rect(float x, float y, float w, float h);
	void draw();
	void draw(texture* tex);
	void draw(texture* tex, const rect& texcoord);

	void alignCentre();
	void rotate(float angle, float centreX = 0.5, float centreY = 0.5);
};
//=========================================================== Fonts ====
class glyph {
	public:
		texture tex;
		float w, h, texW, texH, offsetX, offsetY, advance;
		glyph(GLuint t);
		glyph(const glyph& g);
};
class font {
	protected:
		std::map<unsigned int, glyph*> glyphs;
		void* ftFace;
		float size;
		glyph* character(unsigned int c);
	public:
		font();
		font(std::string filename, int size);
		~font();
		bool open(std::string filename, int size);
		float width(std::string text);
		void preloadASCII();
		float  draw(std::string text, float x, float y);
		float draw(unsigned int c, float x, float y);
};
//==================================================== Applications ====
void activate(app *a, bool close = true);
void deactivate();
void deactivateAll();
void run();
class app {
	public:
		virtual bool init() {return true;}
		virtual void loop() {}
		virtual void quit() {}

		virtual void suspend() {}
		virtual void resume() {}

		virtual void onMouseButton(int button, bool value) {}
		virtual void onMouseMove(int x, int y) {}
		virtual void onKeyboard(int button, bool value) {}
		virtual void onClose() {sol::deactivateAll();}
};
} // namespace sol
#endif // _SOL_HEADER_
