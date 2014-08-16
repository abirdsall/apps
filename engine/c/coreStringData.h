#ifndef CORE_STRING_DATA
#define CORE_STRING_DATA

namespace core
{
    class StringData
    {
    protected:
        
        c8* _data;
        
    public:
        
        inline c8* Data()
        {
            return _data;
        }
        
        virtual void Alloc() = 0;
        
        virtual void Free() = 0;
        
        virtual s32 Size() = 0;
    };
    
    class StringData16 : public StringData
    {
    public:
        
        void Alloc();
        
        void Free();
        
        inline s32 Size()
        {
            return 16;
        }
    };
    
    class StringData256 : public StringData
    {
    public:
        
        void Alloc();
        
        void Free();
        
        inline s32 Size()
        {
            return 256;
        }
    };
    
    class StringData4096 : public StringData
    {
    public:
        
        void Alloc();
        
        void Free();
        
        inline s32 Size()
        {
            return 4096;
        }
    };
}

#endif