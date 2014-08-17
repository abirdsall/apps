#ifndef kMathLineH
#define kMathLineH

#include "os.h"

class line
{
public:
				line(void) :
					mPos(V3Zero),
					mDir(V3Zero),
					mEnd(V3Zero)
				{
				}

				line(const v3& pos, const v3& dir)
				{
					mPos = pos;
					mDir = dir;
					fixEnd();
					normalise(mDir);
				}
	
	void		setPath(const v3& pos, const v3& end)
				{
					mPos = pos;
					mEnd = end;
					fixDir();
				}

	v3			getPos(void) const		{return mPos;}
	v3			getEnd(void) const		{return mEnd;}
	v3			getDir(void) const		{return mDir;}

	void		setPos(const v3& pos)	{mPos = pos; fixDir();}
	void		setEnd(const v3& end)	{mEnd = end; fixDir();}
	void		setDir(const v3& dir)	{mDir = dir; fixEnd();}

private:
	void		fixPos(void)			{mPos = mEnd - mDir;}
	void		fixEnd(void)			{mEnd = mPos + mDir;}
	void		fixDir(void)			{mDir = normalise(mEnd - mPos);}

	v3			mPos;
	v3			mDir;
	v3			mEnd;
};

#endif