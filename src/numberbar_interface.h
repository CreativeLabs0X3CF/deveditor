#ifndef __NUMBERBAR_INTERFACE_H
#define __NUMBERBAR_INTERFACE_H

class NumberBarInterface
{
public:
    NumberBarInterface() {}
    virtual ~NumberBarInterface() {}


private:
    NumberBarInterface( const NumberBarInterface& source );
    void operator = ( const NumberBarInterface& source );
};


#endif // __NUMBERBAR_INTERFACE_H
