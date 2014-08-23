namespace fw
{
    void InitQuad2dBatches();
    void KillQuad2dBatches();
    
    void DrawQuad2d( const Rect& vcoords, const v4& colour );
    void DrawQuad2d( const Rect& vcoords, const Rect& tcoords );
    void DrawQuad2d( const Rect& vcoords, const Rect& tcoords, const v4& colour );
}