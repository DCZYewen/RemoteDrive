#pragma once
#include <stdio.h>
#include <filesystem>
#include "LightLOG/lightlog.h"
namespace fileOP{
typedef std::size_t size_t;
typedef std::fpos_t fpos_t;
typedef int         bufferMode;
#define F_FBF _IOFBF
#define F_LBF _IOLBF
#define F_NBF _IONBF

class F_file{
protected:
FILE* fileobj;

public:
    F_file(std::string filename, std::string mode){
        F_file(filename.c_str() , mode.c_str());
        return;
    }
    F_file(const char* filename, const char* mode){
        fileobj = fopen(filename , mode);
    }
    ~F_file(){
        F_close();
    }

    int F_reopen(std::string filename, std::string mode);
    int F_reopen(const char* filename, const char* mode);
/*
Close the file owned by this class is automatically done
by its destructor, but when this class is not newed, but
was generated by compiler, mannual destruct might needed
*/
    void F_close();

/*
This func does not handles ub, never open this class as a
input stream and call this function.
If you want to flush all openning streams, call flushall.
*/
    int F_flush();
    void F_flushall();

/*
This static function is used to allocate buffers to be used
in setbuf and setvbuf or release them
*/
    static char* allocBuffer(size_t bufsz);
    static void  freeBuffer(char * buf);

/*
These functions are used to set the buffer of the stream,
the size of buf should be BUFSIZ. If buf is null, it will
set the buffer mode to _IONBF, which close the buffer.
*/
    void F_setBuf(char* buf);
/*
When calling, it will change the buffer mode of stream,
be advised, buffer is set to defalut. If you modified
buffer size, call setBuf again.
*/
    void F_setBufMode(bufferMode mode);
/*
When calling, it will change the buffer mode of stream to
mode, stream buffer to buf. MAKE SURE size is valid to 
avoid access to invlid memory. If buf is NULL, change the
internal buffer to size.
*/
    int  F_setvBuf(char* buf , bufferMode mode , size_t size);
    

    bool isValidFile(){
        if(fileobj == NULL){
            return false;
        }else return true;
    }

protected:

private:

};

int F_file::F_reopen(const char* filename, const char* mode){
    FILE* f = freopen(filename , mode , fileobj);
    if (f == NULL){
        fileobj = NULL;
        return -1;
    }else{
        return 0;
    }
}

int F_file::F_reopen(std::string filename, std::string mode){
    return F_reopen( filename.c_str() , mode.c_str() );
}

void F_file::F_close(){
    //This func is used to destruct the class,
    //called mannually or by destructor
    if (fileobj == NULL){
        return;
    }else{
        fclose(fileobj);
    }
}

int F_file::F_flush(){
    if(fileobj == NULL){
        return -1;
    }else {
        if (!fflush(fileobj)){
            return 0;
        }else{
            return EOF;
        }
    }
}

void F_file::F_flushall(){
    fflush(NULL);
    return;
}

char* F_file::allocBuffer(size_t bufsz){
    return (char*)malloc(bufsz);
}

void  F_file::freeBuffer(char* buf){
    if(buf != NULL){
        free(buf);
    }
    return;
}

void  F_file::F_setBufMode(bufferMode mode){
    setvbuf(fileobj , NULL , mode , BUFSIZ);
}

void  F_file::F_setBuf(char* buf){
    setbuf(fileobj, buf);
}

int   F_file::F_setvBuf(char* buf, bufferMode mode, size_t size){
    return setvbuf(fileobj , buf , mode , size);
}
//end of namespace fileOP
}