#ifndef BOOLGUARD_H
#define BOOLGUARD_H


class BoolGuard
{
public:
    BoolGuard(bool* wsk);
    virtual ~BoolGuard();
private:
    bool *_wsk;
};

#endif // BOOLGUARD_H
