#ifndef _TCompare_H_
#define _TCompare_H_

//////////////////////////////////////////////////////////////////////////////
//
// Compare Object
//
//////////////////////////////////////////////////////////////////////////////

class DefaultEquals {
public:
    template<class TEqualsValue>
    bool operator () (const TEqualsValue& value1, const TEqualsValue& value2)
    {
        return value1 == value2;
    }
};

class DefaultCompare {
public:
    template<class TEqualsValue>
    bool operator () (const TEqualsValue& value1, const TEqualsValue& value2)
    {
        return value1 > value2;
    }
};

class DefaultNoEquals {
public:
    template<class TEqualsValue>
    bool operator () (const TEqualsValue& value1, const TEqualsValue& value2)
    {
        ZError(L"DefaultNoEquals(...) called");
        return false;
    }
};


class DefaultNoCompare {
public:
    template<class TEqualsValue>
    bool operator () (const TEqualsValue& value1, const TEqualsValue& value2)
    {
        ZError(L"DefaultNoCompare(...) called");
        return false;
    }
};

#endif
