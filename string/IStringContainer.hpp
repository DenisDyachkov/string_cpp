#ifndef ISTRINGCONTAINER_HPP
#define ISTRINGCONTAINER_HPP

template<typename Type>
class IStringContainer
{
public:
    virtual Type *begin() = 0;
    virtual Type *end() = 0;
    virtual const Type *data() const = 0;
    virtual void push(const Type &) = 0;
    virtual void pop() = 0;
    virtual void push_back(const Type &) = 0;
    virtual void pop_back() = 0;
    virtual bool empty() const = 0;
    virtual ~IStringContainer()
    {};
};

#endif //ISTRINGCONTAINER_HPP