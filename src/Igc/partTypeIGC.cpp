/*
**  Copyright (C) 1996, 1997 Microsoft Corporation. All Rights Reserved.
**
**  File:	partTypeIGC.cpp
**
**  Author: 
**
**  Description:
**      Implementation of the CpartTypeIGC class. This file was initially created by
**  the ATL wizard for the core object.
**
**  History:
*/
// partTypeIGC.cpp : Implementation of CpartTypeIGC
#include "pch.h"
#include "partTypeIGC.h"

/////////////////////////////////////////////////////////////////////////////
// CpartTypeIGC
HRESULT         CpartTypeIGC::Initialize(ImissionIGC*   pMission,
                                         Time           now,
                                         const void*    data,
                                         int            dataSize)
{
    assert (pMission);
    m_pMission = pMission;

    ZRetailAssert (data && (dataSize >= sizeof(DataPartTypeIGC)));
    {
        //Note ... we need to allocate & copy the DataHullTypeIGC + the variable length fields
        m_dataSize = dataSize;
        m_data = (DataPartTypeIGC*)(new char [m_dataSize]);
        assert (m_data);
        memcpy(m_data, data, m_dataSize);

        if (m_data->successorPartID != NA)
        {
            m_pptSuccessor = pMission->GetPartType(m_data->successorPartID);
            assert (m_pptSuccessor);
        }

        pMission->AddPartType(this);
    }

    return S_OK;
}


void        CpartTypeIGC::Terminate(void)
{
    m_pMission->DeletePartType(this);
}

int         CpartTypeIGC::Export(void*  data) const
{
    if (data)
        memcpy(data, m_data, m_dataSize);

    return m_dataSize;
}


/////////////////////////////////////////////////////////////////////////////
// IpartTypeIGC

const wchar_t* IpartTypeIGC::GetEquipmentTypeName(EquipmentType et)
{
    switch (et)
    {
    case ET_ChaffLauncher:
        return L"chaff";

    case ET_Weapon:
		return L"weapon";

    case ET_Magazine:
		return L"missile";

    case ET_Dispenser:
		return L"mine";

    case ET_Shield:
		return L"shield";

    case ET_Cloak:
		return L"cloak";

    case ET_Pack:
		return L"ammo";

    case ET_Afterburner:
		return L"afterburner";

    default:
        assert(false);
		return L"";
    }
}

