#include "fw.h"
#include "gs.h"
#include "game.h"
#include "cubeRenderer.h"

using namespace fw; //Test
using namespace gs;

static gs::ShaderHandle sFillShader;

CubeRenderer mCubeRenderer;

static fw::CameraHandle sCameraHandle;

static const f32 TouchDistancePerFrameLimit = 1.0f;
static const f32 TouchDistancePerFrameLimitSq = TouchDistancePerFrameLimit * TouchDistancePerFrameLimit;

static const f32 TapTolerance = 8.0f;
static const f32 TapToleranceSq = TapTolerance * TapTolerance;

static const f32 PointTolerance = 256.0f;
static const f32 PointToleranceSq = PointTolerance * PointTolerance;
static const f32 LineTolerance = 32.0f;
static const f32 LineToleranceSq = LineTolerance * LineTolerance;

static const s32 BlockLimitSqrt = 16;
static const s32 BlockLimit = BlockLimitSqrt * BlockLimitSqrt;
static const s32 PointLimitSqrt = BlockLimitSqrt + 1;
static const s32 PointLimit = PointLimitSqrt * PointLimitSqrt;
static const s32 LineLimit = PointLimitSqrt * (PointLimitSqrt - 1) + PointLimitSqrt * (PointLimitSqrt - 1);

struct GardenPoint : v2
{
};

struct GardenLine
{
    bool _active;
    s32 _from;
    s32 _to;
};

struct GardenBlock
{
    s32 _lines[ 4 ];
    s32 _requiredLines;
};

struct Garden
{
    GardenBlock _blocks[ BlockLimit ];
    GardenPoint _points[ PointLimit ];
    GardenLine _lines[ LineLimit ];
    
    s32 _pointCount;
    s32 _pointCountX;
    s32 _pointCountY;
    
    s32 _hLineCount;
    s32 _hLineCountX;
    s32 _hLineCountY;
    
    s32 _vLineCount;
    s32 _vLineCountX;
    s32 _vLineCountY;
    
    s32 _lineCount;

    s32 _activePoint;
    
    v2 _touchLocation;
    f32 _touchDistanceSq;
    
    bool _touching;
    
    Garden( fw::Rect area, s32 pointCountX, s32 pointCountY )
    {
        _pointCountX = pointCountX;
        _pointCountY = pointCountY;
        _pointCount = _pointCountX * _pointCountY;
        
        _hLineCountX = _pointCountX - 1;
        _hLineCountY = _pointCountY;
        _hLineCount = _hLineCountX * _hLineCountY;

        _vLineCountX = _pointCountX;
        _vLineCountY = _pointCountY - 1;
        _vLineCount = _vLineCountX * _vLineCountY;
        
        _lineCount = _hLineCount + _vLineCount;
        
        _activePoint = -1;

        ASSERT( _pointCountX >= 2 );
        ASSERT( _pointCountY >= 2 );
        
        ASSERT( _pointCount <= PointLimit );
        ASSERT( _lineCount <= LineLimit );

        f32 xStep = area.Size().x / f32( _pointCountX - 1 );
        f32 yStep = area.Size().y / f32( _pointCountY - 1 );
        f32 step = core::min( xStep, yStep );
        
        f32 x = area.Min().x;
        f32 y = area.Min().y;
        
        for( s32 i = 0; i < _pointCountX; i++ )
        {
            for( s32 j = 0; j < _pointCountY; j++ )
            {
                s32 pointIndex = j * _pointCountX + i;
                
                GardenPoint& point = _points[ pointIndex ];
        
                point.x = x + step * f32( i );
                point.y = y + step * f32( j );
                
                if( i < _hLineCountX )
                {
                    s32 nextPointIndex = pointIndex + 1;
                    s32 hLineIndex = j * _hLineCountX + i;
                    GardenLine& line = _lines[ hLineIndex ];
                    line._active = false;
                    line._from = pointIndex;
                    line._to = nextPointIndex;
                }

                if( j < _vLineCountY )
                {
                    s32 nextPointIndex = pointIndex + _pointCountX;
                    s32 vLineIndex = _hLineCount + ( j * _vLineCountX + i );
                    GardenLine& line = _lines[ vLineIndex ];
                    line._active = false;
                    line._from = pointIndex;
                    line._to = nextPointIndex;
                }
            }
        }
    }
    
    void Draw()
    {
        glLineWidth(4.0f);
        
        gs::Put();
        gs::SetWrite(gs::eWriteRgb);
        gs::SetBlend(gs::eBlendRgba);
        gs::SetDepth(gs::eDepthNone);
        gs::SetCull(gs::eCullNone);
        gs::Set2d();
        
        fw::BatchQuad2dBegin( fw::Quad2dShaderFilled, gs::InvalidTextureHandle );
        for( s32 i = 0; i < _pointCountX; i++ )
        {
            for( s32 j = 0; j < _pointCountY; j++ )
            {
                s32 pointIndex = j * _pointCountX + i;
                
                GardenPoint& point = _points[ pointIndex ];

                fw::BatchQuad2d( fw::Rect(
                                          point.x - 4.0f,
                                          point.y - 4.0f,
                                          point.x + 4.0f,
                                          point.y + 4.0f), v4( 1.0f, 1.0f, 1.0f, 0.25f ) );
            }
        }
        fw::BatchQuad2dEnd( gs::ePrimTriangles );
        /*
        fw::DrawLinesBegin();
        for( s32 i = 0; i < _lineCount; i++ )
        {
            GardenPoint& from = _points[ _lines[ i ]._from ];
            GardenPoint& to = _points[ _lines[ i ]._to ];
            if( _lines[ i ]._active )
            {
                fw::DrawLinesAdd( from, to, v4( 1.0f, 1.0f, 1.0f, 0.75f ) );
            }
            else
            {
                fw::DrawLinesAdd( from, to, v4( 1.0f, 1.0f, 1.0f, 0.1f ) );
            }
        }
        
        if( _touching && _activePoint != -1 )
        {
            fw::DrawLinesAdd( _points[ _activePoint ], _touchLocation, v4( 1.0f, 1.0f, 1.0f, 0.5f ) );
        }
        
        fw::DrawLinesEnd();
    */
        gs::Pop();
        
        if( _activePoint != -1 )
        {
            GardenPoint& point = _points[ _activePoint ];
            
            gs::Put();
            gs::SetWrite(gs::eWriteRgb);
            gs::SetBlend(gs::eBlendRgba);
            gs::SetDepth(gs::eDepthNone);
            gs::SetCull(gs::eCullNone);
            gs::Set2d();
            fw::BatchQuad2dBegin( fw::Quad2dShaderFilled, gs::InvalidTextureHandle );
            fw::BatchQuad2d( fw::Rect(
                                   point.x - 4.0f,
                                   point.y - 4.0f,
                                   point.x + 4.0f,
                                   point.y + 4.0f), v4( 1.0f, 1.0f, 1.0f, 1.0f ) );
            fw::BatchQuad2dEnd( gs::ePrimTriangles );
            gs::Pop();
        }
    }
    
    s32 ClosestPoint( v2 location )
    {
        s32 closestPoint = -1;
        
        float closestDistance = PointToleranceSq;
        
        for( s32 i = 0; i < _pointCount; i++ )
        {
            f32 distance = distanceSquared( _points[ i ], location );
            
            if( distance < closestDistance )
            {
                closestDistance = distance;
                closestPoint = i;
            }
        }
        
        return closestPoint;
    }
    
    float DistanceSquared(v2 from, v2 to, v2 point)
    {
        const float lineLengthSq = distanceSquared( from, to );
        
        if( lineLengthSq == 0.0f )
        {
            return distanceSquared( point, from );
        }
        
        const float t = dot(point - from, to - from) / lineLengthSq;
        
        if(t < 0.0f )
        {
            return distanceSquared( point, from );
        }
        else if( t > 1.0f)
        {
            return distanceSquared( point, to );
        }
        
        const v2 projection = from + t * ( to - from );

        return distanceSquared( point, projection );
    }
    
    s32 ClosestLine( v2 location )
    {
        s32 closestLine = -1;
        
        float closestDistance = LineToleranceSq;
        
        for(s32 i = 0; i < _lineCount; i++)
        {
            v2 from = _points[ _lines[ i ]._from ];
            
            v2 to = _points[ _lines[ i ]._to ];
            
            f32 distance = DistanceSquared( from, to, location );
            
            if(distance < closestDistance)
            {
                closestDistance = distance;
                closestLine = i;
            }
        }
        
        return closestLine;
    }
    
    void TouchBegin( v2 touch )
    {
        _activePoint = -1;
        _touching = true;
        
        _touchDistanceSq = 0.0f;
    }

    void TouchEnd( v2 touch )
    {
        _activePoint = -1;
        _touching = false;
        
        if( _touchDistanceSq < TapToleranceSq )
        {
            s32 closestLine = ClosestLine( touch );
            if( closestLine != -1 )
            {
                _lines[ closestLine ]._active = !_lines[ closestLine ]._active;
            }
        }
    }

    void TouchUpdateCore( v2 touch )
    {
        s32 oldActivePoint = _activePoint;
        s32 newActivePoint = ClosestPoint( touch );
        
        if( newActivePoint != -1 )
        {
            if( oldActivePoint != newActivePoint )
            {
                s32 line = FindLine( oldActivePoint, newActivePoint );
                
                if( line != -1 )
                {
                    _lines[ line ]._active = true;//!_lines[ line ]._active;
                }
            }
            
            _activePoint = newActivePoint;
        }
    }
    
    void TouchUpdate( v2 touch )
    {
        f32 movementSq = _activePoint == -1 ? 0.0f : distanceSquared( _touchLocation, touch );
        
        if(movementSq > TouchDistancePerFrameLimitSq)
        {
            f32 slice = TouchDistancePerFrameLimit / core::sqrt( movementSq );
            
            for( f32 fraction = slice; fraction < 1.0f; fraction += slice )
            {
                TouchUpdateCore( lerp( _touchLocation, touch, fraction ) );
            }
        }
        
        TouchUpdateCore( touch );
        
        _touchLocation = touch;
        _touchDistanceSq += movementSq;
    }
    
    s32 FindLine( s32 pointA, s32 pointB )
    {
        for( s32 i = 0; i < _lineCount; i++ )
        {
            GardenLine& line = _lines[ i ];
            
            if( ( line._from == pointA || line._to == pointA ) && ( line._from == pointB || line._to == pointB ) )
            {
                return i;
            }
        }
        
        return -1;
    }
};

static Garden sGarden = Garden( fw::Rect( 32.0f, 256.0f, 640.0f - 32.0f, 1136.0f - 256.0f ), 7, 7 );

void gameInit()
{
    fw::Init();
	
    sCameraHandle = fw::CameraNew( fw::Rect(0.0f, 0.0f, ( f32 )os::WindowSizeX(), ( f32 )os::WindowSizeY()), &gameDraw );

    //fw::CameraSetFocus( sCameraHandle, v3( 0.0f, 0.0f, 0.0f ) );

    mCubeRenderer.Init( 128 );

    mCubeRenderer.Add( fw::CubeComponent( v3( 0.0f, 0.0f, -50.0f ), v3( 1.0f, 1.0f, 1.0f ), v4( 1.0f, 1.0f, 1.0f, 1.0f ) ) );
    mCubeRenderer.Add( fw::CubeComponent( v3( 0.0f, 0.0f, 50.0f ), v3( 1.0f, 1.0f, 1.0f ), v4( 1.0f, 1.0f, 1.0f, 1.0f ) ) );
    /*
#if kBuildOpenGles3
    String vShader = "#version 300 es\n";
#else //kBuildOpenGl3
    String vShader = "#version 150\n";
#endif
    vShader = vShader + "in vec3 vertex_position;\n";
    vShader += "in vec4 vertex_colour;\n";
    vShader += "in vec4 vertex_tcoord;\n";
    vShader += "out vec4 fragment_colour;\n";
    vShader += "uniform mat4 viewMatrix;\n";
    vShader += "uniform mat4 projMatrix;\n";
    vShader += "void main()\n";
    vShader += "{\n";
    vShader = vShader + "\tgl_Position = projMatrix * viewMatrix * vec4(vertex_position.x, vertex_position.y, vertex_position.z, 1);\n";
    vShader += "\tfragment_colour = vertex_colour;\n";
    vShader += "}\n";
    
#if kBuildOpenGles3
    String fShader = "#version 300 es\n";
    fShader += "precision highp float;\n";
#else //kBuildOpenGl3
    String fShader = "#version 150\n";
#endif
    fShader += "in vec4 fragment_colour;\n";
    fShader += "out vec4 output_colour;\n";
    fShader += "void main()\n";
    fShader += "{\n";
    fShader += "\toutput_colour = fragment_colour;\n";
    fShader += "}\n";
    
    sFillShader = gs::ShaderNew( vShader.toStr(), fShader.toStr() );
     */
}

void gameTick( f32 dt )
{
    
}

void gameTouch( const os::Touch* touches, s32 touchCount )
{
    for( s32 i = 0; i < 1/*touchCount*/; i++ )
    {
        const os::Touch& touch = touches[ i ];
        
        switch( touch._phase )
        {
            case os::eTouchPhaseBegan:
            {
                sGarden.TouchBegin( v2( touch._x, touch._y ) );
                sGarden.TouchUpdate( v2( touch._x, touch._y ) );
                break;
            }
            case os::eTouchPhaseCancelled:
            case os::eTouchPhaseEnded:
            {
                sGarden.TouchEnd( v2( touch._x, touch._y ) );
                break;
            }
            case os::eTouchPhaseMoved:
            {
                sGarden.TouchUpdate( v2( touch._x, touch._y ) );
                break;
            }
            default:
            {
                break;
            }
        }
    }
}

void gameDraw()
{
    sGarden.Draw();
/*
    gs::Put();
    
    gs::ShaderSet(sFillShader);
    gs::SetCull(gs::eCullNone);
    gs::SetWrite(gs::eWriteRgb);
    gs::SetBlend(gs::eBlendRgba);
    gs::SetDepth(gs::eDepthNone);
    
    mCubeRenderer.Draw( gs::ePrimLineLoop, false );
    
    gs::Pop();
 
 */

}

void gameKill()
{
    fw::Kill();
}