#ifndef VECTOR_H
#define VECTOR_H

template<class T>
class Vector
{
private:
    T* value;
    int _now;
    int _max;
    void enlarge();
public:
    Vector();
    void add(T t);
    bool remove_value(T t);
    bool remove_index(int index);
    int count();
    T& operator[](int index);
};


template<class T>
void Vector<T>::enlarge()
{
    if(_now<_max)
        return ;
    _max*=2;
    T* temp=new T[_max];
    for(int i=0;i<_now;i++)
        temp[i]=value[i];
    delete value;
    value=temp;
}

template<class T>
Vector<T>::Vector():_now(0),_max(5)
{
    value=new T[5];
}

template<class T>
void Vector<T>::add(T t)
{
    enlarge();
    value[_now++]=t;
}

template<class T>
bool Vector<T>::remove_value(T t)
{
    bool flag=false;
    int i;
    for(i=0;i<_now;i++)
    {
        if(value[i]==t)
        {
            flag=true;
            break;
        }
    }
    if(flag)
    {
        for(int j=i;j<_now-1;j++)
        {
            value[j]=value[j+1];
        }
        _now--;
    }
    return flag;
}

template<class T>
bool Vector<T>::remove_index(int index)
{
    if(index<0||index>=_now)
        return false;
    for(int j=index;j<_now-1;j++)
        value[j]=value[j+1];
    _now--;
    return true;
}

template<class T>
int Vector<T>::count()
{
    return _now;
}

template<class T>
T &Vector<T>::operator[](int index)
{
    T *t=new T();
    if(index<0||index>=_now)
        return *t;
    return value[index];
}

#endif // VECTOR_H
