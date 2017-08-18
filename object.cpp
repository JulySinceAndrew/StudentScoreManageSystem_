#include"object.h"

void object::setID(long id)
{
    _ID=id;
}

object::object(QString _n, long id):_name(_n),_ID(id)
{

}

object::object()
{

}

object::~object()
{

}

QString object::name()
{
    return _name;
}

long object::ID()
{
    return _ID;
}

void object::set_name(QString name)
{
    _name=name;
}

