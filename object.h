#ifndef OBJECT_H
#define OBJECT_H

#include<QString>
#include<vector.h>

class object
{
private:
        QString _name;
        long _ID;
protected:
        void setID(long id);
public:
        object(QString _n,long id);
        object();
        ~object();
        QString name();
        long ID();
        void set_name(QString name);
        virtual bool set_ID(long id)=0;
};

#endif // OBJECT_H
