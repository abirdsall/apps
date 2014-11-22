#ifndef kMathLineH
#define kMathLineH

#include "os.h"

class line
{
public:
				line() :
					_pos(V3Zero),
					_dir(V3Zero),
					_end(V3Zero)
				{
				}

				line(const v3& pos, const v3& dir)
				{
					_pos = pos;
					_dir = dir;
					fixEnd();
					normalise(_dir);
				}
	
	void		setPath(const v3& pos, const v3& end)
				{
					_pos = pos;
					_end = end;
					fixDir();
				}

	v3			getPos() const		{return _pos;}
	v3			getEnd() const		{return _end;}
	v3			getDir() const		{return _dir;}

	void		setPos(const v3& pos)	{_pos = pos; fixDir();}
	void		setEnd(const v3& end)	{_end = end; fixDir();}
	void		setDir(const v3& dir)	{_dir = dir; fixEnd();}

private:
	void		fixPos()			{_pos = _end - _dir;}
	void		fixEnd()			{_end = _pos + _dir;}
	void		fixDir()			{_dir = normalise(_end - _pos);}

	v3			_pos;
	v3			_dir;
	v3			_end;
};

#endif